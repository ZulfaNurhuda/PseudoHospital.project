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
    int widths[] = {20, 10};
    const char *headers[] = {"Pasien", "Nyawa"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 2, 1);

    char lifeStr[10] = "";
    strcat(lifeStr, COLOR_YELLOW);
    for (int i = 0; i < 3; i++)
    {
        if (i < patient->life)
        {
            strcat(lifeStr, "O");
        }
        else
        {
            strcat(lifeStr, "X");
        }
    }
    strcat(lifeStr, COLOR_RESET);

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