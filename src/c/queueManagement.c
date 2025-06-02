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
    QueueNode *currentNode = q->head;
    int pos = 1;
    while (currentNode != NULL)
    {
        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            if (hospital->patients.elements[i].id == currentNode->info.patientId)
            {
                if (strcmp(hospital->patients.elements[i].queueRoom, q->roomCode) == 0)
                {
                    hospital->patients.elements[i].queuePosition = pos;
                }
                break;
            }
        }
        currentNode = currentNode->next;
        pos++;
    }
}

boolean skipPatientInQueue(Hospital *hospital, Session *session, const char *patientUsername)
{
    if (hospital == NULL || session == NULL || patientUsername == NULL)
    {
        printError("Data input tidak valid untuk skip antrian.");
        return false;
    }

    if (!session->isLoggedIn || (session->role != MANAGER && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Manajer dan Pasien yang dapat melakukan operasi skip antrian.");
        return false;
    }

    int patientId = -1;
    char roomCode[5];
    roomCode[0] = '\0';

    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].username, patientUsername) == 0)
        {
            if (hospital->patients.elements[i].queuePosition == 0)
            {
                printError("Anda sudah berada didalam ruangan dokter, tidak bisa melewati antrian.");
                return false;
            }
            patientId = hospital->patients.elements[i].id;
            strcpy(roomCode, hospital->patients.elements[i].queueRoom);
            break;
        }
    }

    if (patientId == -1)
    {
        char err[100] = "";
        strcat(err, "Pasien dengan username '");
        strcat(err, patientUsername);
        strcat(err, "' tidak ditemukan.");
        printError(err);
        return false;
    }

    if (roomCode[0] == '\0')
    {
        char err[100] = "";
        strcat(err, "Pasien '");
        strcat(err, patientUsername);
        strcat(err, "' tidak sedang dalam antrian apapun.");
        printError(err);
        return false;
    }

    Queue *q = findQueueByRoomCode(hospital, roomCode);
    if (q == NULL)
    {
        char err[150] = "";
        strcat(err, "Antrian untuk ruangan ");
        strcat(err, roomCode);
        strcat(err, " (tempat pasien ");
        strcat(err, patientUsername);
        strcat(err, " terdaftar) tidak ditemukan.");
        printError(err);
        return false;
    }

    if (isQueueEmpty(q))
    {
        char err[100] = "";
        strcat(err, "Antrian ruangan ");
        strcat(err, roomCode);
        strcat(err, " kosong, tidak ada pasien untuk di-skip.");
        printError(err);
        return false;
    }

    QueueNode *targetNode = NULL;
    QueueNode *prevNode = NULL;
    QueueNode *current = q->head;

    while (current != NULL)
    {
        if (current->info.patientId == patientId)
        {
            targetNode = current;
            break;
        }
        prevNode = current;
        current = current->next;
    }

    if (targetNode == NULL)
    {
        char err[200] = "";
        strcat(err, "Pasien '");
        strcat(err, patientUsername);
        strcat(err, "' tidak ditemukan dalam struktur antrian ruangan ");
        strcat(err, roomCode);
        strcat(err, " meskipun terdaftar di sana. Data mungkin korup.");
        printError(err);
        return false;
    }

    if (targetNode == q->head)
    {
        char msg[150] = "";
        strcat(msg, "Pasien '");
        strcat(msg, patientUsername);
        strcat(msg, "' di ruangan ");
        strcat(msg, roomCode);
        strcat(msg, " sudah berada di posisi pertama.");
        printSuccess(msg);
        return true;
    }

    if (prevNode != NULL)
    {
        prevNode->next = targetNode->next;
    }

    if (targetNode->next != NULL)
    {
        targetNode->next->prev = prevNode;
    }
    else
    {
        q->tail = prevNode;
    }

    targetNode->next = q->head;
    targetNode->prev = NULL;

    if (q->head != NULL)
    {
        q->head->prev = targetNode;
    }

    q->head = targetNode;

    if (q->tail == NULL)
    {
        q->tail = targetNode;
    }

    updatePatientPositionsInQueue(hospital, q);

    char successMsg[200] = "";
    strcat(successMsg, "Pasien '");
    strcat(successMsg, patientUsername);
    strcat(successMsg, "' di ruangan ");
    strcat(successMsg, roomCode);
    strcat(successMsg, " berhasil dipindahkan ke posisi pertama dalam antrian.");
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

    if (!session->isLoggedIn || (session->role != MANAGER && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Manajer dan Pasien yang dapat melakukan operasi skip antrian.");
        return false;
    }

    int patientToCancelIdx = -1;
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].username, patientUsernameToCancel) == 0)
        {
            if (hospital->patients.elements[i].queuePosition == 0)
            {
                printError("Anda sudah berada didalam ruangan dokter, tidak bisa membatalkan antrian.");
                return false;
            }
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