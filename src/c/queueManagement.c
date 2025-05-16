#include "queueManagement.h"

boolean skipQueue(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat melewati antrian.");
        return false;
    }

    if (hospital->patients.nEff == 0)
    {
        printError("Tidak ada pasien terdaftar!");
        return false;
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
        return false;
    }

    Patient *patient = &hospital->patients.elements[patientIdx];
    if (patient->queueRoom[0] == '\0')
    {
        printError("Anda tidak terdaftar dalam antrian apapun!");
        return false;
    }

    if (hospital->queues.nRooms == 0)
    {
        printError("Tidak ada antrian terdaftar!");
        return false;
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
        return false;
    }

    Queue *queue = &hospital->queues.queues[queueIdx];
    if (queue->idxHead == -1 || queue->idxTail == -1)
    {
        printError("Antrian kosong!");
        return false;
    }

    int patientQueuePos = -1;
    for (int i = queue->idxHead; i <= queue->idxTail; i++)
    {
        if (queue->buffer[i].patientID == patient->id)
        {
            patientQueuePos = i;
            break;
        }
    }
    if (patientQueuePos == -1)
    {
        printError("Pasien tidak ditemukan dalam antrian!");
        return false;
    }

    if (patientQueuePos == queue->idxHead)
    {
        printError("Anda sudah berada di posisi pertama antrian!");
        return false;
    }

    QueueInfo temp = queue->buffer[patientQueuePos];
    for (int i = patientQueuePos; i > queue->idxHead; i--)
    {
        queue->buffer[i] = queue->buffer[i - 1];
    }
    queue->buffer[queue->idxHead] = temp;

    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].queueRoom, patient->queueRoom) == 0)
        {
            for (int j = queue->idxHead; j <= queue->idxTail; j++)
            {
                if (queue->buffer[j].patientID == hospital->patients.elements[i].id)
                {
                    hospital->patients.elements[i].queuePosition = j - queue->idxHead + 1;
                    break;
                }
            }
        }
    }

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

    printHeader("Lewati Antrian");
    int widths[] = {15, 20, 20, 10};
    const char *headers[] = {"Dokter", "Ruangan", "Posisi Baru", "Total"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 4, 1);

    char positionStr[10] = "";
    int k = 0;
    int pos = patient->queuePosition;
    if (pos == 0)
        positionStr[k++] = '0';
    else
        while (pos > 0)
        {
            positionStr[k++] = (pos % 10) + '0';
            pos /= 10;
        }
    for (int m = 0; m < k / 2; m++)
    {
        char temp = positionStr[m];
        positionStr[m] = positionStr[k - 1 - m];
        positionStr[k - 1 - m] = temp;
    }
    positionStr[k] = '\0';

    char totalStr[10] = "";
    k = 0;
    int total = queue->idxTail - queue->idxHead + 1;
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

    printSuccess("Berhasil melewati antrian!");
    return true;
}

boolean cancelQueue(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat membatalkan antrian.");
        return false;
    }

    if (hospital->patients.nEff == 0)
    {
        printError("Tidak ada pasien terdaftar!");
        return false;
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
        return false;
    }

    Patient *patient = &hospital->patients.elements[patientIdx];
    if (patient->queueRoom[0] == '\0')
    {
        printError("Anda tidak terdaftar dalam antrian apapun!");
        return false;
    }

    if (hospital->queues.nRooms == 0)
    {
        printError("Tidak ada antrian terdaftar!");
        return false;
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
        return false;
    }

    Queue *queue = &hospital->queues.queues[queueIdx];
    if (queue->idxHead == -1 || queue->idxTail == -1)
    {
        printError("Antrian kosong!");
        return false;
    }

    int patientQueuePos = -1;
    for (int i = queue->idxHead; i <= queue->idxTail; i++)
    {
        if (queue->buffer[i].patientID == patient->id)
        {
            patientQueuePos = i;
            break;
        }
    }
    if (patientQueuePos == -1)
    {
        printError("Pasien tidak ditemukan dalam antrian!");
        return false;
    }

    for (int i = patientQueuePos; i < queue->idxTail; i++)
    {
        queue->buffer[i] = queue->buffer[i + 1];
    }
    queue->idxTail--;
    if (queue->idxHead > queue->idxTail)
    {
        queue->idxHead = -1;
        queue->idxTail = -1;
    }

    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].queueRoom, patient->queueRoom) == 0)
        {
            for (int j = queue->idxHead; j <= queue->idxTail; j++)
            {
                if (queue->buffer[j].patientID == hospital->patients.elements[i].id)
                {
                    hospital->patients.elements[i].queuePosition = j - queue->idxHead + 1;
                    break;
                }
            }
        }
    }

    char canceledRoom[50];
    strcpy(canceledRoom, patient->queueRoom);
    patient->queueRoom[0] = '\0';
    patient->queuePosition = 0;

    char doctorName[50] = "Tidak ada";
    if (hospital->doctors.nEff > 0)
    {
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            if (strcmp(hospital->doctors.elements[i].room, canceledRoom) == 0)
            {
                strcpy(doctorName, hospital->doctors.elements[i].username);
                break;
            }
        }
    }

    printHeader("Pembatalan Antrian");
    int widths[] = {15, 20};
    const char *headers[] = {"Dokter", "Ruangan"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 2, 1);
    const char *row[] = {doctorName, canceledRoom};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    printSuccess("Antrian berhasil dibatalkan!");
    return true;
}