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

    if (patient->medicationsTaken.top + 1 >= patient->medicationsPrescribed.nEff)
    {
        printError("Semua obat yang diresepkan sudah dikonsumsi!");
        return false;
    }

    printHeader("Daftar Obat yang Diresepkan");
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
                integerToString(medicationId, medicationIdStr, sizeof(medicationIdStr));
                const char *row[] = {medicationIdStr, medicationName};
                printTableBorder(widths, 2, 2);
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
    int expectedMedicationId = patient->medicationsPrescribed.medicationId[patient->medicationsTaken.top + 1];

    if (selectedMedicationId == expectedMedicationId)
    {

        patient->medicationsTaken.medicationId[++patient->medicationsTaken.top] = selectedMedicationId;

        printSuccess("Obat berhasil diminum!");
        return true;
    }
    else
    {

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
                if (i + 1 >= patient->life)
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