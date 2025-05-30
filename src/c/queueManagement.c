#include "queueManagement.h"
#include "myQueue.h" // For linked-list queue operations
#include "utils.h"   // For printError, printSuccess, etc.
#include <string.h>  // For strcmp, strcpy, sprintf
#include <stdio.h>   // For sprintf

// Helper function to find a queue by its room code
static Queue* findQueueByRoomCode(Hospital *hospital, const char *roomCode) {
    if (hospital == NULL || roomCode == NULL || roomCode[0] == '\0') return NULL;
    for (int i = 0; i < hospital->queues.nRooms; i++) { // Assuming nRooms correctly tracks active queues with codes
        if (strcmp(hospital->queues.queues[i].roomCode, roomCode) == 0) {
            return &hospital->queues.queues[i];
        }
    }
    // If not found among nRooms, check full capacity if queues can be sparsely activated
    for (int i = 0; i < hospital->queues.capacity; i++) {
         if (hospital->queues.queues[i].roomCode[0] != '\0' && strcmp(hospital->queues.queues[i].roomCode, roomCode) == 0) {
            return &hospital->queues.queues[i];
        }
    }
    return NULL;
}

// Helper function to update queue positions for all patients in a specific queue
static void updatePatientPositionsInQueue(Hospital *hospital, Queue *q) {
    if (q == NULL || hospital == NULL) {
        return;
    }
    // First, clear positions of patients who might have been in this queue but are no longer
    // This is important if a patient is cancelled and the queue becomes empty.
    for (int i = 0; i < hospital->patients.nEff; i++) {
        if (strcmp(hospital->patients.elements[i].queueRoom, q->roomCode) == 0) { // If they thought they were in this room
            boolean stillInQueue = false;
            QueueNode *scanner = q->front;
            while(scanner != NULL) {
                if(scanner->info.patientID == hospital->patients.elements[i].id) {
                    stillInQueue = true;
                    break;
                }
                scanner = scanner->next;
            }
            if (!stillInQueue) { // If not in the queue structure anymore
                hospital->patients.elements[i].queuePosition = 0; 
                // hospital->patients.elements[i].queueRoom[0] = '\0'; // This should be done by the calling function (cancel/dequeue)
            }
        }
    }

    if (isQueueEmpty(q)) return; // No positions to update if queue is empty

    QueueNode *current_node = q->front;
    int pos = 1;
    while (current_node != NULL) {
        for (int i = 0; i < hospital->patients.nEff; i++) {
            if (hospital->patients.elements[i].id == current_node->info.patientID) {
                if (strcmp(hospital->patients.elements[i].queueRoom, q->roomCode) == 0) { // Double check they are for this queue
                     hospital->patients.elements[i].queuePosition = pos;
                }
                break; 
            }
        }
        current_node = current_node->next;
        pos++;
    }
}

// Function for Manager to move the first patient in a queue to the end of the queue.
boolean skipPatientInQueue(Hospital *hospital, Session *session, const char *roomCode) {
    if (hospital == NULL || session == NULL || roomCode == NULL) {
        printError("Data input tidak valid untuk skip antrian.");
        return false;
    }
    if (!session->isLoggedIn || session->role != MANAGER) {
        printError("Akses ditolak! Hanya Manajer yang dapat melakukan operasi skip antrian.");
        return false;
    }

    Queue *q = findQueueByRoomCode(hospital, roomCode);
    if (q == NULL) {
        char err[100];
        // sprintf(err, "Antrian untuk ruangan %s tidak ditemukan.", roomCode);
        strcpy(err, "Antrian untuk ruangan ");
        strcat(err, roomCode);
        strcat(err, " tidak ditemukan.");
        printError(err);
        return false;
    }
    if (isQueueEmpty(q) || queueSize(q) < 2) {
        printError("Tidak cukup pasien dalam antrian untuk melakukan skip (perlu minimal 2).");
        return false;
    }

    int patientIDToSkip;
    if (!dequeue(q, &patientIDToSkip)) { 
        printError("Gagal mengambil pasien dari depan antrian saat skip.");
        return false;
    }
    if (!enqueue(q, patientIDToSkip)) { 
        printError("Gagal menambahkan pasien ke akhir antrian saat skip.");
        // Attempt to restore: enqueue at front. This is a simplified recovery.
        // A more robust recovery would involve creating a new node and inserting at front.
        // For now, the state might be inconsistent if this second enqueue fails.
        // Consider adding a function like: boolean enqueueAtFront(Queue* q, int patientID);
        return false; 
    }

    updatePatientPositionsInQueue(hospital, q);
    
    char successMsg[150];
    // sprintf(successMsg, "Pasien terdepan di antrian ruangan %s telah dipindahkan ke akhir. Antrian diperbarui.", roomCode);
    strcpy(successMsg, "Pasien terdepan di antrian ruangan ");
    strcat(successMsg, roomCode);
    strcat(successMsg, " telah dipindahkan ke akhir. Antrian diperbarui.");
    printSuccess(successMsg);
    return true;
}

// Function for a Patient to cancel their own queue, or Manager to cancel any patient's queue.
boolean cancelPatientFromQueue(Hospital *hospital, Session *session, const char *patientUsernameToCancel) {
    if (hospital == NULL || session == NULL || patientUsernameToCancel == NULL) {
        printError("Data input tidak valid untuk pembatalan antrian.");
        return false;
    }
    if (!session->isLoggedIn) {
        printError("Akses ditolak. Silakan login terlebih dahulu.");
        return false;
    }

    int patientToCancelIdx = -1;
    for (int i = 0; i < hospital->patients.nEff; i++) {
        if (strcmp(hospital->patients.elements[i].username, patientUsernameToCancel) == 0) {
            patientToCancelIdx = i;
            break;
        }
    }
    if (patientToCancelIdx == -1) {
        char err[100];
        // sprintf(err,"Pasien '%s' yang akan dibatalkan antriannya tidak ditemukan.", patientUsernameToCancel);
        strcpy(err, "Pasien '");
        strcat(err, patientUsernameToCancel);
        strcat(err, "' yang akan dibatalkan antriannya tidak ditemukan.");
        printError(err);
        return false;
    }
    Patient *patientToCancel = &hospital->patients.elements[patientToCancelIdx];

    if (session->role == PATIENT && strcmp(session->username, patientUsernameToCancel) != 0) {
        printError("Akses ditolak! Pasien hanya dapat membatalkan antriannya sendiri.");
        return false;
    }

    if (patientToCancel->queueRoom[0] == '\0') {
        char errMsg[100];
        // sprintf(errMsg, "Pasien %s tidak sedang dalam antrian apapun.", patientUsernameToCancel);
        strcpy(errMsg, "Pasien ");
        strcat(errMsg, patientUsernameToCancel);
        strcat(errMsg, " tidak sedang dalam antrian apapun.");
        printError(errMsg);
        return false;
    }

    char originalRoomCode[5]; // Store for messages and patient list update
    strcpy(originalRoomCode, patientToCancel->queueRoom);
    Queue *q = findQueueByRoomCode(hospital, originalRoomCode);

    if (q == NULL) { // Check if queue actually exists
        char errMsg[150];
        // sprintf(errMsg, "Error: Antrian untuk ruangan %s (tempat pasien %s terdaftar) tidak ditemukan dalam sistem.", originalRoomCode, patientUsernameToCancel);
        strcpy(errMsg, "Error: Antrian untuk ruangan ");
        strcat(errMsg, originalRoomCode);
        strcat(errMsg, " (tempat pasien ");
        strcat(errMsg, patientUsernameToCancel);
        strcat(errMsg, " terdaftar) tidak ditemukan dalam sistem.");
        printError(errMsg);
        patientToCancel->queueRoom[0] = '\0'; // Correct patient's state
        patientToCancel->queuePosition = 0;
        return false;
    }
    
    boolean removed = false;
    int patientIDToCancel = patientToCancel->id;

    int firstPatientID;
    if (peekQueue(q, &firstPatientID) && firstPatientID == patientIDToCancel) {
        if (dequeue(q, &firstPatientID)) { 
            removed = true;
        }
    } else { 
        QueueNode *current = q->front;
        QueueNode *prev = NULL;
        while (current != NULL) {
            if (current->info.patientID == patientIDToCancel) {
                if (prev == NULL) { 
                    q->front = current->next; 
                } else {
                    prev->next = current->next;
                }
                if (current == q->rear) { 
                    q->rear = prev; 
                }
                free(current);
                q->size--;
                removed = true;
                break;
            }
            prev = current;
            current = current->next;
        }
        if (q->front == NULL) { 
            q->rear = NULL;
        }
    }

    if (!removed) {
        char errMsg[250]; // Increased buffer size
        char idStr[12];
        if (!integerToString(patientIDToCancel, idStr, sizeof(idStr))) {
            strcpy(idStr, "ERR");
        }
        strcpy(errMsg, "Pasien ");
        strcat(errMsg, patientToCancel->username);
        strcat(errMsg, " (ID: ");
        strcat(errMsg, idStr);
        strcat(errMsg, ") secara teknis tidak ditemukan dalam struktur antrian ");
        strcat(errMsg, originalRoomCode);
        strcat(errMsg, " meskipun terdaftar di sana. Data mungkin korup.");
        printError(errMsg);
        // Fall-through to clear patient's state anyway
    }

    // Clear patient's queue status definitively
    patientToCancel->queueRoom[0] = '\0';
    patientToCancel->queuePosition = 0;

    if (q != NULL) { // q could be NULL if findQueueByRoomCode failed initially but we proceeded to clear patient state
      updatePatientPositionsInQueue(hospital, q); 
    }
    
    char successMsg[200]; // Increased buffer size
    if (removed) {
        // sprintf(successMsg, "Antrian untuk pasien %s di ruangan %s berhasil dibatalkan.", patientUsernameToCancel, originalRoomCode);
        strcpy(successMsg, "Antrian untuk pasien ");
        strcat(successMsg, patientUsernameToCancel);
        strcat(successMsg, " di ruangan ");
        strcat(successMsg, originalRoomCode);
        strcat(successMsg, " berhasil dibatalkan.");
    } else {
        // If not technically "removed" from queue structure but patient state is cleared due to inconsistency
        // sprintf(successMsg, "Status antrian untuk pasien %s telah dibersihkan karena inkonsistensi data.", patientUsernameToCancel);
        strcpy(successMsg, "Status antrian untuk pasien ");
        strcat(successMsg, patientUsernameToCancel);
        strcat(successMsg, " telah dibersihkan karena inkonsistensi data.");
    }
    printSuccess(successMsg);
    return true; // Return true because patient is no longer in queue from their perspective
}
