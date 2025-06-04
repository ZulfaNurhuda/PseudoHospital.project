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
                            char errorMessage[100] = "";
                            strcat(errorMessage, "Ruangan ");
                            strcat(errorMessage, room->code);
                            strcat(errorMessage, " masih ditempati oleh Dr.");
                            strcat(errorMessage, hospital->doctors.elements[k].username);
                            printError(errorMessage);
                            return false;
                        }
                    }
                }
            }
        }
    }

    Room **newLayout = (Room **)safeMalloc(newRowCount * sizeof(Room *));
    if (newLayout == NULL)
    {
        printError("Gagal alokasi memori untuk layout baru (rows)!");
        return false;
    }

    for (int i = 0; i < newRowCount; i++)
    {
        newLayout[i] = (Room *)safeMalloc(newColCount * sizeof(Room));
        if (newLayout[i] == NULL)
        {
            printError("Gagal alokasi memori untuk layout baru (cols)!");
            for (int k = 0; k < i; k++)
            {
                free(newLayout[k]);
            }
            free(newLayout);
            return false;
        }

        for (int j = 0; j < newColCount; j++)
        {
            char code[5] = "";
            code[0] = 'A' + i;
            char numberStr[3] = "";
            integerToString(j + 1, numberStr, sizeof(numberStr));
            strcat(code, numberStr);
            strcpy(newLayout[i][j].code, code);
            newLayout[i][j].capacity = 3;
            newLayout[i][j].doctorId = -1;
            newLayout[i][j].patientInRoom.patientId = (int *)safeMalloc(3 * sizeof(int));
        }
    }

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

    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL)
            {
                free(hospital->layout.elements[i][j].patientInRoom.patientId);
            }
        }
        if (hospital->layout.elements[i] != NULL)
        {
            free(hospital->layout.elements[i]);
        }
    }
    if (hospital->layout.elements != NULL)
    {
        free(hospital->layout.elements);
    }

    hospital->layout.elements = newLayout;
    hospital->layout.rowEff = newRowCount;
    hospital->layout.colEff = newColCount;
    hospital->layout.rowCapacity = newRowCount;
    hospital->layout.colCapacity = newColCount;

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

    Doctor *doctorToMove = NULL;
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        if (strcmp(hospital->doctors.elements[i].username, username) == 0)
        {
            doctorToMove = &hospital->doctors.elements[i];
            break;
        }
    }

    if (doctorToMove == NULL)
    {
        printError("Dokter tidak ditemukan!");
        return false;
    }

    int targetRow = newRoomCode[0] - 'A';
    int targetCol = atoi(&newRoomCode[1]) - 1;

    if (targetRow < 0 || targetRow >= hospital->layout.rowEff || targetCol < 0 || targetCol >= hospital->layout.colEff)
    {
        printError("Kode ruangan tidak valid!");
        return false;
    }

    Room *targetRoom = &hospital->layout.elements[targetRow][targetCol];
    if (targetRoom->doctorId != -1)
    {
        printError("Pemindahan gagal. Ruangan tersebut sudah ditempati.");
        return false;
    }

    Room *currentRoom = NULL;
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            if (hospital->layout.elements[i][j].doctorId == doctorToMove->id)
            {
                currentRoom = &hospital->layout.elements[i][j];
                break;
            }
        }
        if (currentRoom)
            break;
    }

    if (!currentRoom)
    {
        printError("Dokter tidak memiliki ruangan.");
        return false;
    }

    currentRoom->doctorId = -1;
    targetRoom->doctorId = doctorToMove->id;
    strcpy(doctorToMove->room, newRoomCode);

    for (int i = 0; i < currentRoom->patientInRoom.nEff; i++)
    {
        int patientId = currentRoom->patientInRoom.patientId[i];

        Patient *patientToMove = NULL;
        for (int j = 0; j < hospital->patients.nEff; j++)
        {
            if (hospital->patients.elements[j].id == patientId)
            {
                patientToMove = &hospital->patients.elements[j];
                break;
            }
        }

        if (patientToMove)
        {

            for (int k = 0; k < targetRoom->patientInRoom.capacity; k++)
            {
                if (targetRoom->patientInRoom.patientId[k] == 0)
                {
                    targetRoom->patientInRoom.patientId[k] = patientId;
                    targetRoom->patientInRoom.nEff++;
                    break;
                }
            }
        }
    }

    currentRoom->patientInRoom.nEff = 0;

    for (int i = 0; i < hospital->queues.nRooms; i++)
    {
        Queue *queue = &hospital->queues.queues[i];
        if (strcmp(queue->roomCode, currentRoom->code) == 0)
        {

            strcpy(queue->roomCode, newRoomCode);
        }
    }

    printSuccess("Dokter dan pasien berhasil dipindahkan!");
    return true;
}