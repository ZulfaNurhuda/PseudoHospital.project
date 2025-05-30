#include "hospitalLayout.h"

void displayLayout(Hospital *hospital, Session *session, boolean printHeaderFlag)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || (session->role != MANAGER && session->role != DOCTOR && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Manajer, Dokter, atau Pasien yang dapat melihat denah.");
        return;
    }
    if (hospital->layout.rowEff <= 0 || hospital->layout.colEff <= 0)
    {
        printError("Denah rumah sakit kosong!");
        return;
    }

    if (printHeaderFlag) printHeader("Denah Rumah Sakit");

    printf("┌");
    for (int j = 0; j < hospital->layout.colEff; j++)
    {
        printf("─────┬");
    }
    printf("\b┐\n");

    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        // Baris kode ruangan
        printf("│");
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            printf(COLOR_YELLOW " %-3s " COLOR_RESET "│", hospital->layout.elements[i][j].code);
        }
        printf("\n");

        // Garis pemisah antar baris (kecuali baris terakhir)
        if (i != hospital->layout.rowEff - 1)
        {
            printf("├");
            for (int j = 0; j < hospital->layout.colEff; j++)
            {
                printf("─────┼");
            }
            printf("\b┤\n");
        }
    }

    // Baris bawah
    printf("└");
    for (int j = 0; j < hospital->layout.colEff; j++)
    {
        printf("─────┴");
    }
    printf("\b┘\n");
}

void displayRoomDetails(Hospital *hospital, Session *session, const char *roomCode)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || (session->role != MANAGER && session->role != DOCTOR && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Manajer, Dokter, atau Pasien yang dapat melihat detail ruangan.");
        return;
    }
    if (roomCode == NULL || roomCode[0] == '\0')
    {
        printError("Kode ruangan tidak valid!");
        return;
    }
    if (!isValidRoomCode(hospital, roomCode))
    {
        printError("Kode ruangan tidak valid! Contoh: A1");
        return;
    }
    if (hospital->layout.rowEff <= 0 || hospital->layout.colEff <= 0)
    {
        printError("Denah rumah sakit kosong!");
        return;
    }

    Room *room = NULL;
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            if (strcmp(hospital->layout.elements[i][j].code, roomCode) == 0)
            {
                room = &hospital->layout.elements[i][j];
                break;
            }
        }
        if (room != NULL)
            break;
    }

    if (room == NULL)
    {
        char errorMessage[100] = "Ruangan ";
        strcat(errorMessage, roomCode);
        strcat(errorMessage, " tidak ditemukan!");
        printError(errorMessage);
        return;
    }

    // Header
    char header[100] = "Detail Ruangan ";
    strcat(header, roomCode);
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
    int widths1[] = {15, 20};
    printTableBorder(widths1, 2, 1);
    printTableRow(row1, widths1, 2);
    printTableBorder(widths1, 2, 2);

    // Dokter
    char doctorStr[50];
    if (room->doctorId == -1 || hospital->doctors.nEff <= 0)
    {
        strcpy(doctorStr, "Tidak ada dokter");
    }
    else
    {
        boolean found = false;
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            if (hospital->doctors.elements[i].id == room->doctorId)
            {
                strcpy(doctorStr, hospital->doctors.elements[i].username);
                found = true;
                break;
            }
        }
        if (!found)
        {
            strcpy(doctorStr, "Dokter tidak ditemukan");
        }
    }

    const char *row2[] = {"Dokter", doctorStr};
    int widths2[] = {15, 20};
    printTableRow(row2, widths2, 2);
    printTableBorder(widths2, 2, 3);

    // Pasien
    const char *row3[] = {"No.", "Pasien dalam Ruangan"};
    int widths3[] = {5, 30};
    printTableBorder(widths3, 2, 1);
    printTableRow(row3, widths3, 2);
    printTableBorder(widths3, 2, 2);
    if (room->patientInRoom.nEff == 0 || hospital->patients.nEff <= 0)
    {
        const char *row4[] = {"0.", "Tidak ada pasien"};
        printTableRow(row4, widths3, 2);
    }
    else
    {
        for (int i = 0; i < room->patientInRoom.nEff; i++)
        {
            boolean found = false;
            for (int j = 0; j < hospital->patients.nEff; j++)
            {
                if (hospital->patients.elements[j].id == room->patientInRoom.patientId[i])
                {
                    char idStr[20];
                    strcpy(idStr, i + 1 + "0");
                    strcat(idStr, ".");
                    const char *row4[] = {idStr, hospital->patients.elements[j].username};
                    printTableRow(row4, widths3, 2);

                    found = true;
                    break;
                }
            }
            if (!found)
            {
                const char *row4[] = {"0.", "Tidak ada pasien"};
                printTableRow(row4, widths3, 2);
            }
        }
    }
    printTableBorder(widths3, 2, 3);
}