#include "queueManagement.h"

static Queue *findQueueByRoomCode(Hospital *hospital, const char *roomCode)
{
    if (hospital == NULL || roomCode == NULL || roomCode[0] == '\0')
        return NULL;
    for (int i = 0; i < hospital->queues.capacity; i++)
    {
        if (hospital->queues.queues[i].roomCode[0] != '\0' &&
            strcmp(hospital->queues.queues[i].roomCode, roomCode) == 0)
        {
            return &hospital->queues.queues[i];
        }
    }
    return NULL;
}

static void updatePatientPositionsInQueue(Hospital *hospital, Queue *q)
{
    if (q == NULL || hospital == NULL)
    {
        return;
    }
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].queueRoom, q->roomCode) == 0)
        {
            boolean stillInQueue = false;
            QueueNode *scanner = q->head;
            while (scanner != NULL)
            {
                if (scanner->info.patientId == hospital->patients.elements[i].id)
                {
                    stillInQueue = true;
                    break;
                }
                scanner = scanner->next;
            }
            if (!stillInQueue)
            {
                hospital->patients.elements[i].queuePosition = 0;
            }
        }
    }
    if (isQueueEmpty(q))
        return;
    QueueNode *current_node = q->head;
    int pos = 1;
    while (current_node != NULL)
    {
        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            if (hospital->patients.elements[i].id == current_node->info.patientId)
            {
                if (strcmp(hospital->patients.elements[i].queueRoom, q->roomCode) == 0)
                {
                    hospital->patients.elements[i].queuePosition = pos;
                }
                break;
            }
        }
        current_node = current_node->next;
        pos++;
    }
}

boolean skipPatientInQueue(Hospital *hospital, Session *session, const char *roomCode)
{
    if (hospital == NULL || session == NULL || roomCode == NULL)
    {
        printError("Data input tidak valid untuk skip antrian.");
        return false;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melakukan operasi skip antrian.");
        return false;
    }
    Queue *q = findQueueByRoomCode(hospital, roomCode);
    if (q == NULL)
    {
        char err[100];
        strcpy(err, "Antrian untuk ruangan ");
        strcat(err, roomCode);
        strcat(err, " tidak ditemukan.");
        printError(err);
        return false;
    }
    if (isQueueEmpty(q) || queueSize(q) < 2)
    {
        printError("Tidak cukup pasien dalam antrian untuk melakukan skip (perlu minimal 2).");
        return false;
    }
    int patientIdToSkip;
    if (!dequeue(q, &patientIdToSkip))
    {
        printError("Gagal mengambil pasien dari depan antrian saat skip.");
        return false;
    }
    if (!enqueue(q, patientIdToSkip))
    {
        printError("Gagal menambahkan pasien ke akhir antrian saat skip.");
        return false;
    }
    updatePatientPositionsInQueue(hospital, q);
    char successMsg[150];
    strcpy(successMsg, "Pasien terdepan di antrian ruangan ");
    strcat(successMsg, roomCode);
    strcat(successMsg, " telah dipindahkan ke akhir. Antrian diperbarui.");
    printSuccess(successMsg);
    return true;
}

boolean cancelPatientFromQueue(Hospital *hospital, Session *session, const char *patientUsernameToCancel)
{
    if (hospital == NULL || session == NULL || patientUsernameToCancel == NULL)
    {
        printError("Data input tidak valid untuk pembatalan antrian.");
        return false;
    }
    if (!session->isLoggedIn)
    {
        printError("Akses ditolak. Silakan login terlebih dahulu.");
        return false;
    }
    int patientToCancelIdx = -1;
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].username, patientUsernameToCancel) == 0)
        {
            patientToCancelIdx = i;
            break;
        }
    }
    if (patientToCancelIdx == -1)
    {
        char err[100];
        strcpy(err, "Pasien '");
        strcat(err, patientUsernameToCancel);
        strcat(err, "' yang akan dibatalkan antriannya tidak ditemukan.");
        printError(err);
        return false;
    }
    Patient *patientToCancel = &hospital->patients.elements[patientToCancelIdx];
    if (session->role == PATIENT && strcmp(session->username, patientUsernameToCancel) != 0)
    {
        printError("Akses ditolak! Pasien hanya dapat membatalkan antriannya sendiri.");
        return false;
    }
    if (patientToCancel->queueRoom[0] == '\0')
    {
        char errMsg[100];
        strcpy(errMsg, "Pasien ");
        strcat(errMsg, patientUsernameToCancel);
        strcat(errMsg, " tidak sedang dalam antrian apapun.");
        printError(errMsg);
        return false;
    }
    char originalRoomCode[5];
    strcpy(originalRoomCode, patientToCancel->queueRoom);
    Queue *q = findQueueByRoomCode(hospital, originalRoomCode);
    if (q == NULL)
    {
        char errMsg[150];
        strcpy(errMsg, "Error: Antrian untuk ruangan ");
        strcat(errMsg, originalRoomCode);
        strcat(errMsg, " (tempat pasien ");
        strcat(errMsg, patientUsernameToCancel);
        strcat(errMsg, " terdaftar) tidak ditemukan dalam sistem.");
        printError(errMsg);
        patientToCancel->queueRoom[0] = '\0';
        patientToCancel->queuePosition = 0;
        return false;
    }
    boolean removed = false;
    int patientIdToCancel = patientToCancel->id;
    int firstPatientId;
    if (peekQueue(q, &firstPatientId) && firstPatientId == patientIdToCancel)
    {
        if (dequeue(q, &firstPatientId))
        {
            removed = true;
        }
    }
    else
    {
        QueueNode *current = q->head;
        QueueNode *prev = NULL;
        while (current != NULL)
        {
            if (current->info.patientId == patientIdToCancel)
            {
                if (prev == NULL)
                {
                    q->head = current->next;
                }
                else
                {
                    prev->next = current->next;
                }
                if (current->next == NULL)
                {
                    q->tail = prev;
                }
                else
                {
                    current->next->prev = prev;
                }
                free(current);
                q->size--;
                removed = true;
                break;
            }
            prev = current;
            current = current->next;
        }
        if (q->head == NULL)
        {
            q->tail = NULL;
        }
    }
    if (!removed)
    {
        char errMsg[250];
        char idStr[12];
        if (!integerToString(patientIdToCancel, idStr, sizeof(idStr)))
        {
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
    }
    patientToCancel->queueRoom[0] = '\0';
    patientToCancel->queuePosition = 0;
    if (q != NULL)
    {
        updatePatientPositionsInQueue(hospital, q);
    }
    char successMsg[200];
    if (removed)
    {
        strcpy(successMsg, "Antrian untuk pasien ");
        strcat(successMsg, patientUsernameToCancel);
        strcat(successMsg, " di ruangan ");
        strcat(successMsg, originalRoomCode);
        strcat(successMsg, " berhasil dibatalkan.");
    }
    else
    {
        strcpy(successMsg, "Status antrian untuk pasien ");
        strcat(successMsg, patientUsernameToCancel);
        strcat(successMsg, " telah dibersihkan karena inkonsistensi data.");
    }
    printSuccess(successMsg);
    return true;
}