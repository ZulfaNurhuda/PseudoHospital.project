#include "viewUsers.h"

void displayUsers(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {   
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar pengguna.");
        return;
    }

    printHeader("Daftar Pengguna");
    if (hospital->users.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pengguna terdaftar.\n" COLOR_RESET);
        return;
    }

    int widths[] = {5, 20, 10, 20};
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"};
    printTableBorder(widths, 4, 1);
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 4, 2);

    for (int i = 0; i < hospital->users.nEff; i++)
    {
        User *user = &hospital->users.elements[i];
        char idStr[10], roleStr[20], diseaseStr[20] = "-";
        // Convert ID to string
        int id = user->id;
        if (id < 10)
        {
            idStr[0] = '0' + id;
            idStr[1] = '\0';
        }
        else if (id < 100)
        {
            idStr[0] = '0' + (id / 10);
            idStr[1] = '0' + (id % 10);
            idStr[2] = '\0';
        }
        else
        {
            idStr[0] = '1';
            idStr[1] = '0';
            idStr[2] = '0';
            idStr[3] = '\0';
        }
        // Set role string
        switch (user->role)
        {
        case MANAGER:
            strcpy(roleStr, "Manajer");
            break;
        case DOCTOR:
            strcpy(roleStr, "Dokter");
            break;
        case PATIENT:
            strcpy(roleStr, "Pasien");
            break;
        default:
            strcpy(roleStr, "Tidak diketahui");
        }
        // Set disease for patients
        if (user->role == PATIENT && hospital->patients.nEff > 0)
        {
            for (int j = 0; j < hospital->patients.nEff; j++)
            {
                if (hospital->patients.elements[j].id == user->id)
                {
                    if (strlen(hospital->patients.elements[j].disease) > 0 && hospital->patients.elements[j].disease[0] != '\0')
                    {
                        strcpy(diseaseStr, hospital->patients.elements[j].disease);
                    }
                    break;
                }
            }
        }
        const char *row[] = {idStr, user->username, roleStr, diseaseStr};
        printTableRow(row, widths, 4);
    }
    printTableBorder(widths, 4, 3);
}

void displayPatients(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar pasien.");
        return;
    }

    printHeader("Daftar Pasien");
    if (hospital->patients.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pasien terdaftar.\n" COLOR_RESET);
        return;
    }

    int widths[] = {5, 20, 20};
    const char *headers[] = {"ID", "Username", "Penyakit"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        Patient *patient = &hospital->patients.elements[i];
        char idStr[10], diseaseStr[20] = "-";
        // Convert ID to string
        int id = patient->id;
        if (id < 10)
        {
            idStr[0] = '0' + id;
            idStr[1] = '\0';
        }
        else if (id < 100)
        {
            idStr[0] = '0' + (id / 10);
            idStr[1] = '0' + (id % 10);
            idStr[2] = '\0';
        }
        else
        {
            idStr[0] = '1';
            idStr[1] = '0';
            idStr[2] = '0';
            idStr[3] = '\0';
        }
        // Set disease
        if (strlen(patient->disease) > 0 && patient->disease[0] != '\0')
        {
            strcpy(diseaseStr, patient->disease);
        }
        const char *row[] = {idStr, patient->username, diseaseStr};
        printTableRow(row, widths, 3);
    }
    printTableBorder(widths, 3, 3);
}

void displayDoctors(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar dokter.");
        return;
    }

    printHeader("Daftar Dokter");
    if (hospital->doctors.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada dokter terdaftar.\n" COLOR_RESET);
        return;
    }

    int widths[] = {5, 20, 10};
    const char *headers[] = {"ID", "Username", "Aura"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        Doctor *doctor = &hospital->doctors.elements[i];
        char idStr[10], auraStr[10];
        // Convert ID to string
        int id = doctor->id;
        if (id < 10)
        {
            idStr[0] = '0' + id;
            idStr[1] = '\0';
        }
        else if (id < 100)
        {
            idStr[0] = '0' + (id / 10);
            idStr[1] = '0' + (id % 10);
            idStr[2] = '\0';
        }
        else
        {
            idStr[0] = '1';
            idStr[1] = '0';
            idStr[2] = '0';
            idStr[3] = '\0';
        }
        // Convert aura to string
        int aura = doctor->aura;
        if (aura < 10)
        {
            auraStr[0] = '0' + aura;
            auraStr[1] = '\0';
        }
        else if (aura < 100)
        {
            auraStr[0] = '0' + (aura / 10);
            auraStr[1] = '0' + (aura % 10);
            auraStr[2] = '\0';
        }
        else
        {
            auraStr[0] = '1';
            auraStr[1] = '0';
            auraStr[2] = '0';
            auraStr[3] = '\0';
        }
        const char *row[] = {idStr, doctor->username, auraStr};
        printTableRow(row, widths, 3);
    }
    printTableBorder(widths, 3, 3);
}