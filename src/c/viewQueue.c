#include "viewQueue.h"

void displayQueue(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat antrian.");
        return;
    }
    if (hospital->layout.rowEff <= 0 || hospital->layout.colEff <= 0)
    {
        printError("Denah rumah sakit kosong!");
        return;
    }

    printHeader("Status Antrian");
    displayLayout(hospital, session);

    boolean hasRoomsWithDoctor = false;
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            Room *room = &hospital->layout.elements[i][j];
            if (room->doctorID != -1)
            {
                hasRoomsWithDoctor = true;
                // Header
                char header[100] = "Antrian Ruangan ";
                strcat(header, room->code);
                printHeader(header);

                // Tabel kapasitas
                char capacityStr[20];
                int capacity = room->capacity;
                if (capacity < 10)
                {
                    capacityStr[0] = '0' + capacity;
                    capacityStr[1] = '\0';
                }
                else if (capacity < 100)
                {
                    capacityStr[0] = '0' + (capacity / 10);
                    capacityStr[1] = '0' + (capacity % 10);
                    capacityStr[2] = '\0';
                }
                else
                {
                    capacityStr[0] = '1';
                    capacityStr[1] = '0';
                    capacityStr[2] = '0';
                    capacityStr[3] = '\0';
                }
                const char *row1[] = {"Kapasitas", capacityStr};
                int widths[] = {15, 20};
                printTableBorder(widths, 2, 1);
                printTableRow(row1, widths, 2);
                printTableBorder(widths, 2, 3);

                // Dokter
                printf("Dokter:\n");
                char *doctorName = NULL;
                for (int k = 0; k < hospital->doctors.nEff; k++)
                {
                    if (hospital->doctors.elements[k].id == room->doctorID)
                    {
                        doctorName = hospital->doctors.elements[k].username;
                        break;
                    }
                }
                if (doctorName == NULL || hospital->doctors.nEff <= 0)
                {
                    printf(COLOR_YELLOW "  Tidak ada dokter\n" COLOR_RESET);
                }
                else
                {
                    printf(COLOR_YELLOW "  %s\n" COLOR_RESET, doctorName);
                }

                // Pasien di dalam ruangan
                printf("Pasien di dalam ruangan:\n");
                if (room->patientInRoom.nEff == 0 || hospital->patients.nEff <= 0)
                {
                    printf(COLOR_YELLOW "  Tidak ada pasien\n" COLOR_RESET);
                }
                else
                {
                    for (int k = 0; k < room->patientInRoom.nEff; k++)
                    {
                        boolean found = false;
                        for (int l = 0; l < hospital->patients.nEff; l++)
                        {
                            if (hospital->patients.elements[l].id == room->patientInRoom.patientID[k])
                            {
                                printf(COLOR_YELLOW "  %d. %s\n" COLOR_RESET, k + 1, hospital->patients.elements[l].username);
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            printf(COLOR_YELLOW "  %d. Pasien tidak ditemukan\n" COLOR_RESET, k + 1);
                        }
                    }
                }

                // Pasien di antrian
                printf("Pasien di antrian:\n");
                boolean foundQueue = false;
                for (int k = 0; k < hospital->queues.nRooms; k++)
                {
                    if (strcmp(hospital->queues.queues[k].roomCode, room->code) == 0)
                    {
                        foundQueue = true;
                        Queue *queue = &hospital->queues.queues[k];
                        if (queue->idxHead == -1 || hospital->patients.nEff <= 0)
                        {
                            printf(COLOR_YELLOW "  Tidak ada pasien di antrian\n" COLOR_RESET);
                        }
                        else
                        {
                            for (int l = queue->idxHead; l <= queue->idxTail; l++)
                            {
                                boolean found = false;
                                for (int m = 0; m < hospital->patients.nEff; m++)
                                {
                                    if (hospital->patients.elements[m].id == queue->buffer[l].patientID)
                                    {
                                        printf(COLOR_YELLOW "  %d. %s\n" COLOR_RESET, l - queue->idxHead + 1, hospital->patients.elements[m].username);
                                        found = true;
                                        break;
                                    }
                                }
                                if (!found)
                                {
                                    printf(COLOR_YELLOW "  %d. Pasien tidak ditemukan\n" COLOR_RESET, l - queue->idxHead + 1);
                                }
                            }
                        }
                        break;
                    }
                }
                if (!foundQueue)
                {
                    printf(COLOR_YELLOW "  Tidak ada pasien di antrian\n" COLOR_RESET);
                }
            }
        }
    }

    if (!hasRoomsWithDoctor)
    {
        printf(COLOR_YELLOW "Tidak ada ruangan dengan dokter saat ini.\n" COLOR_RESET);
    }
}