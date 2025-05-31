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
                if (room->doctorId != -1)
                {
                    for (int k = 0; k < hospital->doctors.nEff; k++)
                    {
                        if (hospital->doctors.elements[k].id == room->doctorId)
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
    if (newLayout == NULL) {
        printError("Gagal alokasi memori untuk layout baru (rows)!");
        return false;
    }
    // Initialize row pointers to NULL for partial allocation cleanup
    for (int i = 0; i < newRowCount; i++) {
        newLayout[i] = NULL;
    }

    for (int i = 0; i < newRowCount; i++)
    {
        newLayout[i] = (Room *)safeMalloc(newColCount * sizeof(Room));
        if (newLayout[i] == NULL) {
            printError("Gagal alokasi memori untuk layout baru (cols)!");
            for (int k = 0; k < i; k++) { // Free successfully allocated rows
                if (newLayout[k] != NULL) { // Should not be NULL if loop for k ran
                    // Free patientInRoom.patientId within each column of row k
                    for (int l = 0; l < newColCount; l++) { // Assuming newColCount columns were intended for prior rows
                        if (newLayout[k][l].patientInRoom.patientId != NULL) {
                             free(newLayout[k][l].patientInRoom.patientId);
                        }
                    }
                    free(newLayout[k]);
                }
            }
            free(newLayout);
            return false;
        }
        // Initialize patientInRoom.patientId pointers to NULL for this new row
        for (int j = 0; j < newColCount; j++) {
            newLayout[i][j].patientInRoom.patientId = NULL;
        }

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
            for (int k = index - 1; k >= 0; k--) {
                // strncat(code, &colStr[k], 1);
                int currentLen = strlen(code);
                if (currentLen < sizeof(code) - 1) {
                    code[currentLen] = colStr[k];
                    code[currentLen+1] = '\0';
                }
            }
            strcpy(newLayout[i][j].code, code); // strcpy is fine as 'code' is now built safely
            newLayout[i][j].capacity = 3;
            newLayout[i][j].doctorId = -1;
            newLayout[i][j].patientInRoom.patientId = (int *)safeMalloc(3 * sizeof(int));
            if (newLayout[i][j].patientInRoom.patientId == NULL) {
                printError("Gagal alokasi memori untuk patientInRoom.patientId di layout baru!");
                // Cleanup: free all patientInRoom.patientId in current row up to column j-1
                for (int m = 0; m < j; m++) {
                    if (newLayout[i][m].patientInRoom.patientId != NULL) {
                        free(newLayout[i][m].patientInRoom.patientId);
                    }
                }
                // Free current row i itself
                free(newLayout[i]);
                // Free previously allocated full rows (0 to i-1)
                for (int k = 0; k < i; k++) {
                    if (newLayout[k] != NULL) {
                        for (int l = 0; l < newColCount; l++) {
                            if (newLayout[k][l].patientInRoom.patientId != NULL) {
                               free(newLayout[k][l].patientInRoom.patientId);
                            }
                        }
                        free(newLayout[k]);
                    }
                }
                free(newLayout);
                return false;
            }
            newLayout[i][j].patientInRoom.capacity = 3;
            newLayout[i][j].patientInRoom.nEff = 0;
        }
    }

    // Menyalin data dari layout lama ke layout baru
    for (int i = 0; i < hospital->layout.rowEff && i < newRowCount; i++)
    {
        for (int j = 0; j < hospital->layout.colEff && j < newColCount; j++)
        {
            newLayout[i][j].doctorId = hospital->layout.elements[i][j].doctorId;
            newLayout[i][j].patientInRoom.nEff = hospital->layout.elements[i][j].patientInRoom.nEff;
            for (int k = 0; k < hospital->layout.elements[i][j].patientInRoom.nEff; k++)
            {
                newLayout[i][j].patientInRoom.patientId[k] = hospital->layout.elements[i][j].patientInRoom.patientId[k];
            }
        }
    }

    // Membebaskan layout lama
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            free(hospital->layout.elements[i][j].patientInRoom.patientId);
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
        col -= 1; // Convert to 0-based index
        if (row >= newRowCount || col >= newColCount)
        {
            Queue *queueToClear = &hospital->queues.queues[i];
            // Iterate through patients in this queue and update their status
            QueueNode *currentNode = queueToClear->head; // Updated from front
            while (currentNode != NULL)
            {
                int patientId_in_queue = currentNode->info.patientId;
                for (int p_idx = 0; p_idx < hospital->patients.nEff; p_idx++)
                {
                    if (hospital->patients.elements[p_idx].id == patientId_in_queue)
                    {
                        if (strcmp(hospital->patients.elements[p_idx].queueRoom, queueToClear->roomCode) == 0)
                        {
                            hospital->patients.elements[p_idx].queueRoom[0] = '\0';
                            hospital->patients.elements[p_idx].queuePosition = 0;
                        }
                        break;
                    }
                }
                currentNode = currentNode->next;
            }
            // Clear the queue itself
            int dummyPatientId;
            while (!isQueueEmpty(queueToClear))
            {
                dequeue(queueToClear, &dummyPatientId);
            }
            queueToClear->roomCode[0] = '\0'; // Mark queue as inactive
            queueToClear->size = 0;           // Ensure size is reset
        }
    }
    // Note: hospital->queues.nRooms is not adjusted here. If rooms are removed,
    // nRooms might become an overestimation of active queues with valid roomCodes.
    // findQueueByRoomCode should still work as it checks roomCode matching.

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
    if (newRoom->doctorId != -1)
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
                    hospital->layout.elements[i][j].doctorId = -1;
                    break;
                }
            }
        }
    }

    newRoom->doctorId = doctor->id;
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