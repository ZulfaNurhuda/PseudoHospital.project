#include "viewPatientQueue.h"

void viewMySpecificQueueStatus(Hospital *hospital, Session *session)
{

    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat melihat status antrian.");
        return;
    }

    Patient *patient = NULL;

    for (int i = 0; i < hospital->patients.nEff; i++)
    {

        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
        {
            patient = &hospital->patients.elements[i];
            break;
        }
    }

    if (patient == NULL)
    {
        printError("Pasien tidak ditemukan dalam daftar!");
        return;
    }

    if (patient->queueRoom[0] != '\0')
    {

        boolean roomFound = false;
        boolean patientInRoom = false;

        for (int r = 0; r < hospital->layout.rowEff; r++)
        {
            for (int c = 0; c < hospital->layout.colEff; c++)
            {
                Room *room = &hospital->layout.elements[r][c];

                if (strcmp(room->code, patient->queueRoom) == 0)
                {
                    roomFound = true;

                    for (int i = 0; i < room->patientInRoom.nEff; i++)
                    {
                        if (room->patientInRoom.patientId[i] == patient->id)
                        {
                            patientInRoom = true;
                            break;
                        }
                    }

                    if (patientInRoom)
                    {
                        printf(COLOR_YELLOW "Anda sedang berada di dalam ruangan dokter! Silakan menyelesaikan pemeriksaan.\n" COLOR_RESET);
                        return;
                    }

                    break;
                }
            }
            if (roomFound)
                break;
        }

        if (!roomFound)
        {
            printError("Ruangan yang terdaftar untuk Anda tidak ditemukan!");
            return;
        }
    }

    if (patient->queueRoom[0] != '\0')
    {

        Doctor *doctor = NULL;

        for (int i = 0; i < hospital->doctors.nEff; i++)
        {

            if (strcmp(hospital->doctors.elements[i].room, patient->queueRoom) == 0)
            {
                doctor = &hospital->doctors.elements[i];
                break;
            }
        }

        if (doctor != NULL)
        {

            boolean patientFoundInQueue = false;
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

                printError("Antrian untuk ruangan Anda tidak ditemukan atau Anda tidak ada di dalamnya!");
                return;
            }

            Queue *queueToCheck = &hospital->queues.queues[queueIdx];

            QueueNode *currentNode = queueToCheck->head;
            while (currentNode != NULL)
            {
                if (currentNode->info.patientId == patient->id)
                {
                    patientFoundInQueue = true;
                    break;
                }
                currentNode = currentNode->next;
            }

            if (!patientFoundInQueue)
            {
                printError("Anda belum terdaftar dalam antrian check-up! Silakan daftar terlebih dahulu dengan command DAFTAR_CHECKUP.");

                patient->queueRoom[0] = '\0';
                patient->queuePosition = 0;
                return;
            }

            char queuePositionStr[5], queueSizeStr[5], finalQueueFormat[10] = "";

            integerToString(patient->queuePosition, queuePositionStr, sizeof(queuePositionStr));

            integerToString(queueSize(queueToCheck), queueSizeStr, sizeof(queueSizeStr));

            strcat(finalQueueFormat, queuePositionStr);
            strcat(finalQueueFormat, "/");
            strcat(finalQueueFormat, queueSizeStr);

            printHeader("Status antrian Anda:");

            int widths[] = {20, 20};
            const char *row1[] = {"Dokter", doctor->username};
            const char *row2[] = {"Ruangan", doctor->room};
            const char *row3[] = {"Posisi antrian", finalQueueFormat};

            printTableBorder(widths, 2, 1);
            printTableRow(row1, widths, 2);
            printTableBorder(widths, 2, 2);
            printTableRow(row2, widths, 2);
            printTableBorder(widths, 2, 2);
            printTableRow(row3, widths, 2);
            printTableBorder(widths, 2, 3);

            return;
        }

        else
        {
            printError("Dokter untuk ruangan antrian Anda tidak ditemukan!");

            return;
        }
    }

    printError("Anda belum terdaftar dalam antrian check-up! Silakan daftar terlebih dahulu dengan command DAFTAR_CHECKUP.");
}
