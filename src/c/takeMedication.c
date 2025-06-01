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

    // Tampilkan urutan yang benar dari obat
    printHeader("Urutan Obat yang Harus Diminum");
    printf(COLOR_GREY "📋 Informasi: Obat harus diminum sesuai urutan berikut!\n" COLOR_RESET);

    int orderWidths[] = {8, 30};
    const char *orderHeaders[] = {"Urutan", "Nama Obat"};
    printTableBorder(orderWidths, 2, 1);
    printTableRow(orderHeaders, orderWidths, 2);

    for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
    {
        int medicationId = patient->medicationsPrescribed.medicationId[i];
        for (int j = 0; j < hospital->medications.nEff; j++)
        {
            if (hospital->medications.elements[j].id == medicationId)
            {
                char orderNum[10], medicationName[50];
                integerToString(i + 1, orderNum, sizeof(orderNum));
                strcpy(medicationName, hospital->medications.elements[j].name);

                const char *orderRow[] = {orderNum, medicationName};
                printTableRow(orderRow, orderWidths, 2);
                break;
            }
        }
    }
    printTableBorder(orderWidths, 2, 3);
    printf("\n");

    printHeader("Daftar Obat yang Tersedia untuk Dipilih");
    int widths[] = {5, 30};
    const char *headers[] = {"No.", "Nama Obat"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);

    for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
    {
        int medicationId = patient->medicationsPrescribed.medicationId[i];
        for (int j = 0; j < hospital->medications.nEff; j++)
        {
            if (hospital->medications.elements[j].id == medicationId)
            {
                char medicationName[50], medicationIdStr[10];
                strcpy(medicationName, hospital->medications.elements[j].name);
                integerToString(i + 1, medicationIdStr, sizeof(medicationIdStr)); // Nomor urut, bukan ID
                const char *row[] = {medicationIdStr, medicationName};
                printTableRow(row, widths, 2);
                break;
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
        printError("Pilihan nomor untuk obat tidak tersedia!");
        return false;
    }

    int selectedMedicationId = patient->medicationsPrescribed.medicationId[choice - 1];
    int expectedMedicationId = patient->medicationsPrescribed.medicationId[0];

    if (selectedMedicationId == expectedMedicationId)
    {
        patient->medicationsTaken.medicationId[++patient->medicationsTaken.top] = selectedMedicationId;

        for (int i = choice - 1; i < patient->medicationsPrescribed.nEff - 1; i++)
        {
            patient->medicationsPrescribed.medicationId[i] = patient->medicationsPrescribed.medicationId[i + 1];
        }
        patient->medicationsPrescribed.nEff--;

        printSuccess("Obat berhasil diminum!");
        return true;
    }
    else
    {
        // Pasien salah pilih obat
        patient->life--;

        if (patient->life <= 0)
        {
            printError("Pasien kehabisan nyawa! Pasien akan dihapus. 🥀");
            deletePatient(hospital, patient->id);
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
                    printf("💙");
                }
                else
                {
                    printf("🖤");
                }
            }
            printf("\n");
            return false;
        }
    }
}