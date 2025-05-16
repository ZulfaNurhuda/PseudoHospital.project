#include "takeAntidote.h"

boolean takeAntidote(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mengonsumsi penawar.");
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
    if (patient->medicationsTaken.top < 0)
    {
        printError("Anda belum mengonsumsi obat apapun!");
        return false;
    }
    if (patient->life < 0)
    {
        printError("Data nyawa tidak valid!");
        return false;
    }

    int lastMedicationId = patient->medicationsTaken.medicationID[patient->medicationsTaken.top--];
    if (patient->life < 3)
    {
        patient->life++;
    }

    char medicationName[50] = "Tidak dikenal";
    if (hospital->medications.nEff > 0)
    {
        for (int i = 0; i < hospital->medications.nEff; i++)
        {
            if (hospital->medications.elements[i].id == lastMedicationId)
            {
                strcpy(medicationName, hospital->medications.elements[i].name);
                break;
            }
        }
    }

    printHeader("Konsumsi Penawar");
    int widths[] = {15, 20, 10};
    const char *headers[] = {"Obat Dibatalkan", "Nyawa Tersisa", "Status"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 3, 1);

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
    lifeStr[k] = '\0';

    const char *row[] = {medicationName, lifeStr, "Penawar berhasil"};
    printTableRow(row, widths, 3);
    printTableBorder(widths, 3, 3);

    char successMsg[100] = "Penawar berhasil! Obat ";
    strcat(successMsg, medicationName);
    strcat(successMsg, " dibatalkan.");
    printSuccess(successMsg);
    return true;
}