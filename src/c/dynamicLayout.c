#include "dynamicLayout.h"

// Function to change the hospital layout (resize the hospital's room layout)
boolean changeLayout(Hospital *hospital, Session *session, int newRowCount, int newColCount)
{
    // Check if the session is valid and if the user is a manager
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mengubah denah.");
        return false;
    }

    // Validate the new row and column counts
    if (newRowCount <= 0 || newColCount <= 0)
    {
        printError("Ukuran denah tidak valid!");
        return false;
    }

    // Check if any doctor is assigned to rooms that will be deleted (out of new bounds)
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            if (i >= newRowCount || j >= newColCount)
            {
                Room *room = &hospital->layout.elements[i][j];
                if (room->doctorId != -1)
                { // If a doctor is assigned to this room
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
                            return false; // Cannot resize if doctors are in the rooms being deleted
                        }
                    }
                }
            }
        }
    }

    // Allocate new layout with new size
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

        // Initialize rooms in the new layout
        for (int j = 0; j < newColCount; j++)
        {
            char code[5] = "";
            code[0] = 'A' + i;              // Row letter
            char numberStr[3] = "";
            integerToString(j + 1, numberStr, sizeof(numberStr)); // Column number
            strcat(code, numberStr);
            strcpy(newLayout[i][j].code, code);
            newLayout[i][j].capacity = 3;
            newLayout[i][j].doctorId = -1;                                                // No doctor assigned initially
            newLayout[i][j].patientInRoom.patientId = (int *)safeMalloc(3 * sizeof(int)); // Capacity for 3 patients
        }
    }

    // Copy existing rooms data into the new layout if within new size
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

    // Free old layout memory
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            free(hospital->layout.elements[i][j].patientInRoom.patientId);
        }
        free(hospital->layout.elements[i]);
    }
    free(hospital->layout.elements);

    // Update layout in the hospital structure
    hospital->layout.elements = newLayout;
    hospital->layout.rowEff = newRowCount;
    hospital->layout.colEff = newColCount;
    hospital->layout.rowCapacity = newRowCount;
    hospital->layout.colCapacity = newColCount;

    printSuccess("Denah rumah sakit berhasil diubah!");
    return true;
}

// Function to move a doctor from one room to another
boolean moveDoctor(Hospital *hospital, Session *session, const char *username, const char *newRoomCode)
{
    // Check if the session is valid and if the user is a manager
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat memindahkan dokter.");
        return false;
    }

    // Find the doctor by username
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

    // Check if the new room is valid and available
    int targetRow = newRoomCode[0] - 'A';      // Convert row letter to index
    int targetCol = atoi(&newRoomCode[1]) - 1; // Convert column number to index

    if (targetRow < 0 || targetRow >= hospital->layout.rowEff || targetCol < 0 || targetCol >= hospital->layout.colEff)
    {
        printError("Kode ruangan tidak valid!");
        return false;
    }

    Room *targetRoom = &hospital->layout.elements[targetRow][targetCol];
    if (targetRoom->doctorId != -1)
    { // Check if the room is already occupied
        printError("Pemindahan gagal. Ruangan tersebut sudah ditempati.");
        return false;
    }

    // Find the current room of the doctor
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

    // Move the doctor to the new room
    currentRoom->doctorId = -1;              // Remove doctor from the current room
    targetRoom->doctorId = doctorToMove->id; // Assign doctor to the new room
    strcpy(doctorToMove->room, newRoomCode);

    // Move patients from the current room to the new room
    for (int i = 0; i < currentRoom->patientInRoom.nEff; i++)
    {
        int patientId = currentRoom->patientInRoom.patientId[i];

        // Find the patient in the hospital's patient list
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
            // Move patient to the new room
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

    // Empty the current room's patient list
    currentRoom->patientInRoom.nEff = 0;

    // Move patients in the queue who are assigned to the doctor's current room
    for (int i = 0; i < hospital->queues.nRooms; i++)
    {
        Queue *queue = &hospital->queues.queues[i];
        if (strcmp(queue->roomCode, currentRoom->code) == 0)
        {
            // Move patients in this queue to the new room's queue
            strcpy(queue->roomCode, newRoomCode);
        }
    }

    printSuccess("Dokter dan pasien berhasil dipindahkan!");
    return true;
}