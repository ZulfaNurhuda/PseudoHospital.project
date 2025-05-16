#include "myQueue.h"

void myQueue(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat melihat antrian.");
        return;
    }

    if (hospital->patients.nEff == 0)
    {
        printError("Tidak ada pasien terdaftar!");
        return;
    }
    int patientIdx = -1;
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
        {
            patientIdx = i;
            break;
        }
    }
    if (patientIdx == -1)
    {
        printError("Pasien tidak ditemukan!");
        return;
    }

    Patient *patient = &hospital->patients.elements[patientIdx];
    if (patient->queueRoom[0] == '\0')
    {
        printHeader("Status Antrian");
        printSuccess("Anda tidak terdaftar dalam antrian apapun.");
        return;
    }

    if (hospital->queues.nRooms == 0)
    {
        printError("Tidak ada antrian terdaftar!");
        return;
    }
    int queueIdx = -1;
    for (int i = 0; i < hospital->queues.nRooms; i++)
    {
        if (strcmp(hospital->queues.queues[i].roomCode, patient->queueRoom) == 0)
        {
            queueIdx = i;
            break;
        }
    }
    if (queueIdx == -1)
    {
        printError("Antrian tidak ditemukan!");
        return;
    }

    Queue *queue = &hospital->queues.queues[queueIdx];
    char doctorName[50] = "Tidak ada";
    if (hospital->doctors.nEff > 0)
    {
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            if (strcmp(hospital->doctors.elements[i].room, patient->queueRoom) == 0)
            {
                strcpy(doctorName, hospital->doctors.elements[i].username);
                break;
            }
        }
    }

    // Memeriksa konsistensi queuePosition
    int actualPosition = -1;
    for (int i = queue->idxHead; i <= queue->idxTail; i++)
    {
        if (queue->buffer[i].patientID == patient->id)
        {
            actualPosition = i - queue->idxHead + 1;
            break;
        }
    }
    if (actualPosition == -1 || actualPosition != patient->queuePosition)
    {
        printError("Data posisi antrian tidak konsisten!");
        return;
    }

    printHeader("Status Antrian");
    int widths[] = {15, 20, 20, 10};
    const char *headers[] = {"Dokter", "Ruangan", "Posisi", "Total"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 4, 1);

    char positionStr[10] = "";
    char totalStr[10] = "";
    int position = patient->queuePosition;
    int total = queue->idxTail - queue->idxHead + 1;
    int k = 0;
    if (position == 0)
        positionStr[k++] = '0';
    else
        while (position > 0)
        {
            positionStr[k++] = (position % 10) + '0';
            position /= 10;
        }
    for (int m = 0; m < k / 2; m++)
    {
        char temp = positionStr[m];
        positionStr[m] = positionStr[k - 1 - m];
        positionStr[k - 1 - m] = temp;
    }
    positionStr[k] = '\0';
    k = 0;
    if (total == 0)
        totalStr[k++] = '0';
    else
        while (total > 0)
        {
            totalStr[k++] = (total % 10) + '0';
            total /= 10;
        }
    for (int m = 0; m < k / 2; m++)
    {
        char temp = totalStr[m];
        totalStr[m] = totalStr[k - 1 - m];
        totalStr[k - 1 - m] = temp;
    }
    totalStr[k] = '\0';

    const char *row[] = {doctorName, patient->queueRoom, positionStr, totalStr};
    printTableRow(row, widths, 4);
    printTableBorder(widths, 4, 3);
}