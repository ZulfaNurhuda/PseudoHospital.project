#include "viewPatientQueue.h"
#include "myQueue.h" // For isQueueEmpty, queueSize
#include <stdio.h>   // For sprintf
#include <string.h>  // For strcmp

// Function for a patient to view their own queue status.
// (Adapted from the old 'myQueue' function)
void viewMySpecificQueueStatus(Hospital *hospital, Session *session) {
    if (hospital == NULL || session == NULL) {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != PATIENT) {
        printError("Akses ditolak! Hanya Pasien yang dapat melihat status antriannya.");
        return;
    }

    int patientIdx = -1;
    for (int i = 0; i < hospital->patients.nEff; i++) {
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0) {
            patientIdx = i;
            break;
        }
    }

    if (patientIdx == -1) {
        printError("Pasien tidak ditemukan dalam sistem!"); // Should not happen if session is valid
        return;
    }
    Patient *patient = &hospital->patients.elements[patientIdx];

    if (patient->queueRoom[0] == '\0' || patient->queuePosition <= 0) {
        printHeader("Status Antrian Saya");
        printSuccess("Anda tidak terdaftar dalam antrian apapun saat ini.");
        return;
    }

    Queue *roomQueue = NULL;
    for (int i = 0; i < hospital->queues.nRooms; i++) { // Check active queues
        if (strcmp(hospital->queues.queues[i].roomCode, patient->queueRoom) == 0) {
            roomQueue = &hospital->queues.queues[i];
            break;
        }
    }
     if (roomQueue == NULL) { // Fallback to check all queue capacities, in case nRooms is not up-to-date
        for (int i = 0; i < hospital->queues.capacity; i++) {
             if (hospital->queues.queues[i].roomCode[0] != '\0' && strcmp(hospital->queues.queues[i].roomCode, patient->queueRoom) == 0) {
                roomQueue = &hospital->queues.queues[i];
                break;
            }
        }
    }


    if (roomQueue == NULL) {
        char errMsg[200]; // Increased buffer
        // sprintf(errMsg, "Error: Antrian untuk ruangan %s (tempat Anda terdaftar) tidak dapat ditemukan.", patient->queueRoom);
        strcpy(errMsg, "Error: Antrian untuk ruangan ");
        strcat(errMsg, patient->queueRoom);
        strcat(errMsg, " (tempat Anda terdaftar) tidak dapat ditemukan.");
        printError(errMsg);
        // Consider clearing patient's queue state if it's inconsistent
        // patient->queueRoom[0] = '\0';
        // patient->queuePosition = 0;
        return;
    }

    char doctorName[50] = "N/A";
    for (int i = 0; i < hospital->doctors.nEff; i++) {
        if (strcmp(hospital->doctors.elements[i].room, patient->queueRoom) == 0) {
            // customSafeStrcpy(doctorName, hospital->doctors.elements[i].username, sizeof(doctorName));
            strcpy(doctorName, hospital->doctors.elements[i].username); // Assuming doctorName is large enough
            break;
        }
    }

    // Validate patient's actual position in the new linked list structure
    int actualPositionInList = 0;
    int currentPosition = 0;
    QueueNode *currentNode = roomQueue->front;
    while(currentNode != NULL) {
        currentPosition++;
        if(currentNode->info.patientID == patient->id) {
            actualPositionInList = currentPosition;
            break;
        }
        currentNode = currentNode->next;
    }

    if (actualPositionInList == 0) { // Patient ID not found in the queue structure
        char errorMsg[250]; // Increased buffer
        // sprintf(errorMsg, "Inkonsistensi data: Anda terdaftar di antrian %s tapi ID Anda tidak ditemukan dalam struktur antrian.", patient->queueRoom);
        strcpy(errorMsg, "Inkonsistensi data: Anda terdaftar di antrian ");
        strcat(errorMsg, patient->queueRoom);
        strcat(errorMsg, " tapi ID Anda tidak ditemukan dalam struktur antrian.");
        printError(errorMsg);
        patient->queueRoom[0] = '\0'; // Clear inconsistent state
        patient->queuePosition = 0;
        return;
    }
    
    // If patient->queuePosition is stored, it should match actualPositionInList.
    // For this function, we'll display actualPositionInList.
    // patient->queuePosition should be updated by enqueue/dequeue/skip/cancel functions.


    printHeader("Status Antrian Saya");
    int widths[] = {20, 15, 15, 15}; // Adjusted widths
    const char *headers[] = {"Dokter", "Ruangan", "Posisi Anda", "Total Antrian"};
    printTableBorder(widths, 4, 0); // Separator before header
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 4, 1); // Separator after header

    char positionStr[12]; 
    if (!integerToString(actualPositionInList, positionStr, sizeof(positionStr))) {
        strcpy(positionStr, "ERR");
    }
    char totalStr[12]; 
    if (!integerToString(queueSize(roomQueue), totalStr, sizeof(totalStr))) {
        strcpy(totalStr, "ERR");
    }

    const char *row[] = {doctorName, patient->queueRoom, positionStr, totalStr};
    printTableRow(row, widths, 4);
    printTableBorder(widths, 4, 3); // Footer border
}
