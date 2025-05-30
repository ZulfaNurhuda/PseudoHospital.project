#include "takeMedication.h"

boolean takeMedication(Hospital *hospital, Session *session, int medicationId)
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
    if (medicationId <= 0)
    {
        printError("ID obat tidak valid!");
        return false;
    }

    if (hospital->patients.nEff == 0)
    {
        printError("Tidak ada pasien terdaftar!");
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
        printError("Pasien tidak ditemukan!");
        return false;
    }

    Patient *patient = &hospital->patients.elements[patientIdx];
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

    if (hospital->medications.nEff == 0)
    {
        printError("Tidak ada obat terdaftar!");
        return false;
    }
    boolean isValidMedication = false;
    char medicationName[50] = "Tidak dikenal";
    for (int i = 0; i < hospital->medications.nEff; i++)
    {
        if (hospital->medications.elements[i].id == medicationId)
        {
            isValidMedication = true;
            strcpy(medicationName, hospital->medications.elements[i].name);
            break;
        }
    }
    if (!isValidMedication)
    {
        printError("ID obat tidak valid!");
        return false;
    }

    // Memeriksa apakah semua obat sudah dikonsumsi
    if (patient->medicationsTaken.top + 1 >= patient->medicationsPrescribed.nEff)
    {
        printError("Semua obat sudah dikonsumsi!");
        return false;
    }

    // Memeriksa urutan obat
    int expectedMedication = patient->medicationsPrescribed.medicationId[patient->medicationsTaken.top + 1];
    if (expectedMedication != medicationId)
    {
        patient->life--;
        printHeader("Konsumsi Obat");

        char errorMsg[100] = "Obat ";
        strcat(errorMsg, medicationName);
        strcat(errorMsg, " salah urutan! Nyawa tersisa: ");
        char lifeStr[10] = "";
        int k = 0;
        int life = patient->life;
        if (life == 0)
            lifeStr[k++] = '0';
        else
            while (life > 0)
            {
                lifeStr[k++] = (life % 10) + '0';
                life /= 10;
            }
        for (int m = 0; m < k / 2; m++)
        {
            char temp = lifeStr[m];
            lifeStr[m] = lifeStr[k - 1 - m];
            lifeStr[k - 1 - m] = temp;
        }
        strcat(errorMsg, lifeStr);
        printError(errorMsg);

        if (patient->life <= 0)
        {
            printError("Pasien meninggal!");
            deletePatient(hospital, patient->id);
            session->isLoggedIn = false;
            session->userId = -1;
            session->username[0] = '\0';
            session->role = -1;
        }
        return false;
    }

    // Menambahkan obat ke stack
    if (patient->medicationsTaken.top + 1 < patient->medicationsTaken.capacity)
    {
        patient->medicationsTaken.medicationId[++patient->medicationsTaken.top] = medicationId;
    }
    else
    {
        printError("Kapasitas obat yang diambil penuh!");
        return false;
    }

    printHeader("Konsumsi Obat");
    int widths[] = {15, 20};
    const char *headers[] = {"Obat", "Status"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 2, 1);
    const char *row[] = {medicationName, "Berhasil dikonsumsi"};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    char lifeMsg[50] = "Nyawa tersisa: ";
    char lifeStr[10] = "";
    int k = 0;
    int life = patient->life;
    if (life == 0)
        lifeStr[k++] = '0';
    else
        while (life > 0)
        {
            lifeStr[k++] = (life % 10) + '0';
            life /= 10;
        }
    for (int m = 0; m < k / 2; m++)
    {
        char temp = lifeStr[m];
        lifeStr[m] = lifeStr[k - 1 - m];
        lifeStr[k - 1 - m] = temp;
    }
    strcat(lifeMsg, lifeStr);
    printSuccess(lifeMsg);

    char successMsg[100] = "Obat ";
    strcat(successMsg, medicationName);
    strcat(successMsg, " berhasil dikonsumsi!");
    printSuccess(successMsg);
    return true;
}