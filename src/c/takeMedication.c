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
        printError("Semua obat yang diresepkan sudah dikonsumsi!");
        return false;
    }

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

    printHeader("Daftar Obat yang Diresepkan");
    printf(COLOR_BLUE "[📋 | Info] - Daftar obat yang harus diminum sesuai dengan urutan\n" COLOR_RESET);

    int widths[] = {15, 30};
    const char *headers[] = {"Urutan Minum", "Nama Obat"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    printTableBorder(widths, 2, 2);

    boolean validOptions[patient->medicationsPrescribed.nEff];
    for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
    {
        int medicationId = patient->medicationsPrescribed.medicationId[i];
        for (int j = 0; j < hospital->medications.nEff; j++)
        {
            if (hospital->medications.elements[j].id == medicationId)
            {
                boolean isMedicationInInventory = true;
                for (int i = 0; i <= patient->medicationsTaken.top; i++)
                {
                    if (patient->medicationsTaken.medicationId[i] == medicationId)
                    {
                        isMedicationInInventory = false;
                        break;
                    }
                }
                if (isMedicationInInventory)
                {
                    validOptions[i] = true;
                    char medicationName[50], medicationIdStr[10];
                    strcpy(medicationName, hospital->medications.elements[j].name);
                    integerToString(i + 1, medicationIdStr, sizeof(medicationIdStr)); // Nomor urut, bukan ID
                    const char *row[] = {medicationIdStr, medicationName};
                    printTableRow(row, widths, 2);
                    break;
                }
                else
                {
                    validOptions[i] = false;
                }
            }
        }
    }
    printTableBorder(widths, 2, 3);

    int choice;
    while (!readValidInt(&choice, ">>> Pilih obat untuk diminum: "))
    {
        printError("Input tidak valid! Silakan masukkan nomor obat yang benar.");
    }

    if (choice < 1 || choice > patient->medicationsPrescribed.nEff)
    {
        if (!validOptions[choice - 1])
        {
            printError("Pilihan nomor untuk obat tidak tersedia!");
            return false;
        }
    }

    int expectedMedicationId;
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

    patient->medicationsTaken.medicationId[++patient->medicationsTaken.top] = selectedMedicationId;
    printSuccess("Obat berhasil diminum!");

    if (selectedMedicationId != expectedMedicationId)
    {
        // Pasien salah pilih obat
        patient->life--;

        if (patient->life <= 0)
        {
            printError("Pasien kehabisan nyawa! Pasien akan dihapus. 🥀");
            deletePatient(hospital, patient->id);
            session->isLoggedIn = false;
            session->userId = -1;
            strcpy(session->username, "");
            return false;
        }
        else
        {
            printf(COLOR_YELLOW "[💀 | Info ] - Obat Salah! Nyawa berkurang satu.\n" COLOR_RESET);
            printf("→ Sisa nyawa:");
            for (int i = 0; i < 3; i++)
            {
                if (i < patient->life) // Fixed logic untuk menampilkan nyawa
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

    return true;
}