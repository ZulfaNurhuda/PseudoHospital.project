#include "dynamicLayout.h"

boolean changeLayout(Hospital *hospital, Session *session, int newRowCount, int newColCount)
{
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mengubah denah.");
        return false;
    }

    if (newRowCount <= 0 || newColCount <= 0)
    {
        printError("Ukuran denah tidak valid!");
        return false;
    }

    // Memeriksa apakah ada dokter di ruangan yang akan dihapus
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            if (i >= newRowCount || j >= newColCount)
            {
                Room *room = &hospital->layout.elements[i][j];
                if (room->doctorID != -1)
                {
                    for (int k = 0; k < hospital->doctors.nEff; k++)
                    {
                        if (hospital->doctors.elements[k].id == room->doctorID)
                        {
                            char errorMessage[100] = "Ruangan ";
                            strcat(errorMessage, room->code);
                            strcat(errorMessage, " masih ditempati oleh ");
                            strcat(errorMessage, hospital->doctors.elements[k].username);
                            strcat(errorMessage, "!");
                            printError(errorMessage);
                            return false;
                        }
                    }
                }
            }
        }
    }

    // Membuat layout baru
    Room **newLayout = (Room **)safeMalloc(newRowCount * sizeof(Room *));
    for (int i = 0; i < newRowCount; i++)
    {
        newLayout[i] = (Room *)safeMalloc(newColCount * sizeof(Room));
        for (int j = 0; j < newColCount; j++)
        {
            char code[5] = "";
            code[0] = 'A' + i;
            char colStr[4] = "";
            int col = j + 1;
            int index = 0;
            while (col > 0)
            {
                colStr[index++] = (col % 10) + '0';
                col /= 10;
            }
            for (int k = index - 1; k >= 0; k--)
                strncat(code, &colStr[k], 1);
            strcpy(newLayout[i][j].code, code);
            newLayout[i][j].capacity = 3;
            newLayout[i][j].doctorID = -1;
            newLayout[i][j].patientInRoom.patientID = (int *)safeMalloc(3 * sizeof(int));
            newLayout[i][j].patientInRoom.capacity = 3;
            newLayout[i][j].patientInRoom.nEff = 0;
        }
    }

    // Menyalin data dari layout lama ke layout baru
    for (int i = 0; i < hospital->layout.rowEff && i < newRowCount; i++)
    {
        for (int j = 0; j < hospital->layout.colEff && j < newColCount; j++)
        {
            newLayout[i][j].doctorID = hospital->layout.elements[i][j].doctorID;
            newLayout[i][j].patientInRoom.nEff = hospital->layout.elements[i][j].patientInRoom.nEff;
            for (int k = 0; k < hospital->layout.elements[i][j].patientInRoom.nEff; k++)
            {
                newLayout[i][j].patientInRoom.patientID[k] = hospital->layout.elements[i][j].patientInRoom.patientID[k];
            }
        }
    }

    // Membebaskan layout lama
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            free(hospital->layout.elements[i][j].patientInRoom.patientID);
        }
        free(hospital->layout.elements[i]);
    }
    free(hospital->layout.elements);

    // Memperbarui layout
    hospital->layout.elements = newLayout;
    hospital->layout.rowEff = newRowCount;
    hospital->layout.colEff = newColCount;
    hospital->layout.rowCapacity = newRowCount;
    hospital->layout.colCapacity = newColCount;

    // Memperbarui antrian untuk ruangan yang dihapus
    for (int i = 0; i < hospital->queues.nRooms; i++)
    {
        char *code = hospital->queues.queues[i].roomCode;
        int row = code[0] - 'A';
        int col = 0;
        for (int j = 1; code[j]; j++)
        {
            col = col * 10 + (code[j] - '0');
        }
        col -= 1; // Konversi ke indeks 0-based
        if (row >= newRowCount || col >= newColCount)
        {
            hospital->queues.queues[i].idxHead = -1;
            hospital->queues.queues[i].idxTail = -1;
            for (int j = 0; j < hospital->patients.nEff; j++)
            {
                if (strcmp(hospital->patients.elements[j].queueRoom, code) == 0)
                {
                    hospital->patients.elements[j].queueRoom[0] = '\0';
                    hospital->patients.elements[j].queuePosition = 0;
                }
            }
        }
    }

    printHeader("Perubahan Denah");
    int widths[] = {15, 10};
    const char *headers[] = {"Baris", "Kolom"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    char rowCountStr[10] = "";
    char colCountStr[10] = "";
    int temp = newRowCount;
    int index = 0;
    while (temp > 0)
    {
        rowCountStr[index++] = (temp % 10) + '0';
        temp /= 10;
    }
    for (int k = 0; k < index / 2; k++)
    {
        char swap = rowCountStr[k];
        rowCountStr[k] = rowCountStr[index - 1 - k];
        rowCountStr[index - 1 - k] = swap;
    }
    temp = newColCount;
    index = 0;
    while (temp > 0)
    {
        colCountStr[index++] = (temp % 10) + '0';
        temp /= 10;
    }
    for (int k = 0; k < index / 2; k++)
    {
        char swap = colCountStr[k];
        colCountStr[k] = colCountStr[index - 1 - k];
        colCountStr[index - 1 - k] = swap;
    }
    const char *row[] = {rowCountStr, colCountStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    printSuccess("Denah rumah sakit berhasil diubah!");
    return true;
}

boolean moveDoctor(Hospital *hospital, Session *session, const char *username, const char *newRoomCode)
{
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat memindahkan dokter.");
        return false;
    }

    if (!isValidUsername(username))
    {
        printError("Username dokter tidak valid!");
        return false;
    }

    if (!isValidRoomCode(hospital, newRoomCode))
    {
        printError("Kode ruangan tidak valid! Contoh: A1");
        return false;
    }

    int doctorIndex = -1;
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        if (strcmp(hospital->doctors.elements[i].username, username) == 0)
        {
            doctorIndex = i;
            break;
        }
    }
    if (doctorIndex == -1)
    {
        printError("Dokter tidak ditemukan!");
        return false;
    }

    // Menghitung indeks ruangan dari newRoomCode
    int row = newRoomCode[0] - 'A';
    int col = 0;
    for (int i = 1; newRoomCode[i]; i++)
    {
        col = col * 10 + (newRoomCode[i] - '0');
    }
    col -= 1; // Konversi ke indeks 0-based

    if (row >= hospital->layout.rowEff || col >= hospital->layout.colEff)
    {
        printError("Ruangan tidak ditemukan!");
        return false;
    }

    Room *newRoom = &hospital->layout.elements[row][col];
    if (newRoom->doctorID != -1)
    {
        printError("Ruangan sudah ditempati dokter lain!");
        return false;
    }

    Doctor *doctor = &hospital->doctors.elements[doctorIndex];
    if (doctor->room[0] != '\0')
    {
        for (int i = 0; i < hospital->layout.rowEff; i++)
        {
            for (int j = 0; j < hospital->layout.colEff; j++)
            {
                if (strcmp(hospital->layout.elements[i][j].code, doctor->room) == 0)
                {
                    hospital->layout.elements[i][j].doctorID = -1;
                    break;
                }
            }
        }
    }

    newRoom->doctorID = doctor->id;
    strcpy(doctor->room, newRoomCode);

    printHeader("Pemindahan Dokter");
    int widths[] = {15, 20};
    const char *headers[] = {"Dokter", "Ruangan Baru"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    const char *tableRow[] = {username, newRoomCode};
    printTableRow(tableRow, widths, 2);
    printTableBorder(widths, 2, 3);

    char successMessage[100] = "Dokter ";
    strcat(successMessage, username);
    strcat(successMessage, " berhasil dipindahkan!");
    printSuccess(successMessage);
    return true;
}