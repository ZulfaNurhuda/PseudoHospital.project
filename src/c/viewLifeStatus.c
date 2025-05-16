#include "viewLifeStatus.h"

void viewLifeStatus(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat melihat status nyawa.");
        return;
    }

    if (hospital->patients.nEff == 0)
    {
        printError("Tidak ada pasien terdaftar!");
        return;
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
        return;
    }

    Patient *patient = &hospital->patients.elements[patientIdx];
    if (patient->life < 0)
    {
        printError("Data nyawa tidak valid!");
        return;
    }

    printHeader("Status Nyawa");
    int widths[] = {15, 10};
    const char *headers[] = {"Pasien", "Nyawa"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 2, 1);

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

    const char *row[] = {patient->username, lifeStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    if (patient->life > 0)
    {
        printSuccess("Status: Hidup");
    }
    else
    {
        printError("Status: Meninggal");
    }
}