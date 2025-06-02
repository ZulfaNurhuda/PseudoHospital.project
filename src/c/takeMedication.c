#include "takeMedication.h"

boolean takeMedication(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }

    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mengonsumsi obat.");
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
        printError("Pasien tidak ditemukan dalam daftar!");
        return false;
    }

    Patient *patient = &hospital->patients.elements[patientIdx];

    if (strcmp(patient->disease, "Tidak terdeteksi") == 0)
    {
        printf(COLOR_YELLOW "Pasien tidak diresepkan obat karena tidak ada penyakit terdeteksi" COLOR_RESET);
        return false;
    }

    if (!patient->treatedStatus)
    {
        printError("Anda belum diberikan resep obat!");
        return false;
    }

    if (patient->medicationsPrescribed.nEff == 0)
    {
        printError("Tidak ada obat yang diresepkan!");
        return false;
    }

    // Check if patient needs to take antidote first (if last medication was wrong)
    if (patient->medicationsTaken.top >= 0)
    {
        boolean needsAntidote = false;
        int lastMedicationId = patient->medicationsTaken.medicationId[patient->medicationsTaken.top];

        // Find what should have been the medication at that position
        int takenCount = patient->medicationsTaken.top + 1;
        if (takenCount <= patient->medicationsPrescribed.nEff)
        {
            int expectedMedicationId = patient->medicationsPrescribed.medicationId[takenCount - 1];
            if (lastMedicationId != expectedMedicationId)
            {
                needsAntidote = true;
            }
        }
        else
        {
            // More medications taken than prescribed - definitely wrong
            needsAntidote = true;
        }

        if (needsAntidote)
        {
            printError("Urutan obat salah! Silakan minum penawar terlebih dahulu.");
            return false;
        }
    }

    // Check if there are any medications left to take
    boolean isMedicationAvailable = false;
    for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
    {
        boolean foundInTaken = false;
        for (int j = 0; j <= patient->medicationsTaken.top; j++)
        {
            if (patient->medicationsPrescribed.medicationId[i] == patient->medicationsTaken.medicationId[j])
            {
                foundInTaken = true;
                break;
            }
        }

        if (!foundInTaken)
        {
            isMedicationAvailable = true;
            break;
        }
    }

    if (!isMedicationAvailable)
    {
        printError("Semua obat yang diresepkan sudah dikonsumsi!");
        return false;
    }

    // Display available medications
    printHeader("Daftar Obat yang Diresepkan");
    printf(COLOR_BLUE "[📋 | Info] - Daftar obat yang harus diminum sesuai dengan urutan\n" COLOR_RESET);

    int widths[] = {15, 30};
    const char *headers[] = {"Urutan Minum", "Nama Obat"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    printTableBorder(widths, 2, 2);

    // Build list of available medications and display them
    boolean validOptions[patient->medicationsPrescribed.nEff];
    int displayCount = 0;

    for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
    {
        int medicationId = patient->medicationsPrescribed.medicationId[i];

        // Check if this medication has already been taken
        boolean alreadyTaken = false;
        for (int j = 0; j <= patient->medicationsTaken.top; j++)
        {
            if (patient->medicationsTaken.medicationId[j] == medicationId)
            {
                alreadyTaken = true;
                break;
            }
        }

        if (!alreadyTaken)
        {
            validOptions[i] = true;
            displayCount++;

            // Find medication name
            for (int j = 0; j < hospital->medications.nEff; j++)
            {
                if (hospital->medications.elements[j].id == medicationId)
                {
                    char medicationName[50], medicationIdStr[10];
                    strcpy(medicationName, hospital->medications.elements[j].name);
                    integerToString(i + 1, medicationIdStr, sizeof(medicationIdStr));
                    const char *row[] = {medicationIdStr, medicationName};
                    printTableRow(row, widths, 2);
                    break;
                }
            }
        }
        else
        {
            validOptions[i] = false;
        }
    }

    printTableBorder(widths, 2, 3);

    // Get user choice
    int choice;
    while (!readValidInt(&choice, ">>> Pilih obat untuk diminum: "))
    {
        printError("Input tidak valid! Silakan masukkan nomor obat yang benar.");
    }

    // Validate choice range
    if (choice < 1 || choice > patient->medicationsPrescribed.nEff)
    {
        printError("Pilihan nomor untuk obat tidak tersedia!");
        return false;
    }

    // Check if the chosen medication is valid (not already taken)
    if (!validOptions[choice - 1])
    {
        printError("Obat tersebut sudah diminum atau tidak tersedia!");
        return false;
    }

    // Find the next medication that should be taken (in order)
    int expectedMedicationId = -1;
    for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
    {
        boolean foundInTaken = false;
        for (int j = 0; j <= patient->medicationsTaken.top; j++)
        {
            if (patient->medicationsPrescribed.medicationId[i] == patient->medicationsTaken.medicationId[j])
            {
                foundInTaken = true;
                break;
            }
        }

        if (!foundInTaken)
        {
            expectedMedicationId = patient->medicationsPrescribed.medicationId[i];
            break;
        }
    }

    int selectedMedicationId = patient->medicationsPrescribed.medicationId[choice - 1];

    // Add the medication to taken list
    patient->medicationsTaken.medicationId[++patient->medicationsTaken.top] = selectedMedicationId;

    // Check if patient took the correct medication (in order)
    if (selectedMedicationId != expectedMedicationId)
    {
        // Patient chose wrong medication
        patient->life--;

        printf(COLOR_YELLOW "[💀 | Info ] - Obat Salah! Nyawa berkurang satu.\n" COLOR_RESET);

        if (patient->life <= 0)
        {
            printError("Pasien kehabisan nyawa! Pasien akan dihapus. 🥀");

            if (patient->queueRoom[0] != '\0')
            {
                Room *room = NULL;
                boolean roomFound = false;

                for (int i = 0; i < hospital->layout.rowEff && !roomFound; i++)
                {
                    for (int j = 0; j < hospital->layout.colEff && !roomFound; j++)
                    {
                        if (strcmp(hospital->layout.elements[i][j].code, patient->queueRoom) == 0)
                        {
                            room = &hospital->layout.elements[i][j];
                            roomFound = true;
                        }
                    }
                }

                if (room)
                {
                    for (int i = 0; i < room->patientInRoom.nEff; i++)
                    {
                        if (room->patientInRoom.patientId[i] == patient->id)
                        {
                            for (int j = i; j < room->patientInRoom.nEff - 1; j++)
                            {
                                room->patientInRoom.patientId[j] = room->patientInRoom.patientId[j + 1];
                            }
                            room->patientInRoom.nEff--;
                            break;
                        }
                    }

                    Queue *targetQueue = NULL;
                    for (int i = 0; i < hospital->queues.capacity; i++)
                    {
                        if (hospital->queues.queues[i].roomCode[0] != '\0' &&
                            strcmp(hospital->queues.queues[i].roomCode, patient->queueRoom) == 0)
                        {
                            targetQueue = &hospital->queues.queues[i];
                            break;
                        }
                    }

                    if (targetQueue && !isQueueEmpty(targetQueue))
                    {
                        QueueInfo nextPatientInfo;
                        if (dequeue(targetQueue, &nextPatientInfo.patientId))
                        {
                            if (room->patientInRoom.nEff < room->patientInRoom.capacity)
                            {
                                room->patientInRoom.patientId[room->patientInRoom.nEff] = nextPatientInfo.patientId;
                                room->patientInRoom.nEff++;
                            }
                        }
                    }
                }
            }

            deletePatient(hospital, patient->id);

            printf(COLOR_YELLOW "[⭕ | Info ] - Sesi tidak valid setelah pasien kehabisan nyawa.\n" COLOR_RESET);
            session->isLoggedIn = false;
            session->userId = -1;
            strcpy(session->username, "");
            
            return false;
        }
        else
        {
            printf("→ Sisa nyawa: ");
            for (int i = 0; i < 3; i++)
            {
                if (i < patient->life)
                {
                    printf("● ");
                }
                else
                {
                    printf("○ ");
                }
            }
            printf("\n");
            return false;
        }
    }
    else
    {
        // Patient took the correct medication
        printSuccess("Obat berhasil diminum!");
        return true;
    }
}