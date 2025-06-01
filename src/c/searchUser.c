#include "searchUser.h"

static boolean localContainsCaseInsensitiveSubstring(const char *text, const char *pattern)
{

    if (text == NULL || pattern == NULL)
        return false;

    int textLen = 0;
    while (text[textLen] != '\0')
        textLen++;

    int patternLen = 0;
    while (pattern[patternLen] != '\0')
        patternLen++;

    if (patternLen == 0)
        return true;

    if (patternLen > textLen)
        return false;

    for (int i = 0; i <= textLen - patternLen; i++)
    {
        boolean match = true;

        for (int j = 0; j < patternLen; j++)
        {

            if (charToLower(text[i + j]) != charToLower(pattern[j]))
            {
                match = false;
                break;
            }
        }
        if (match)
            return true;
    }

    return false;
}

static int compareUsersById(const void *a, const void *b)
{

    User *userA = (User *)a;
    User *userB = (User *)b;

    if (userA->id < userB->id)
        return -1;
    if (userA->id > userB->id)
        return 1;
    return 0;
}

static int comparePatientsById(const void *a, const void *b)
{
    Patient *patientA = (Patient *)a;
    Patient *patientB = (Patient *)b;
    if (patientA->id < patientB->id)
        return -1;
    if (patientA->id > patientB->id)
        return 1;
    return 0;
}

static int compareDoctorsById(const void *a, const void *b)
{
    Doctor *doctorA = (Doctor *)a;
    Doctor *doctorB = (Doctor *)b;
    if (doctorA->id < doctorB->id)
        return -1;
    if (doctorA->id > doctorB->id)
        return 1;
    return 0;
}

static User *customBinarySearchUsers(const User *key, User *base, int num, int (*compare)(const void *, const void *))
{

    if (num == 0)
        return NULL;

    int low = 0;
    int high = num - 1;
    User *foundUser = NULL;

    while (low <= high)
    {

        int mid = low + (high - low) / 2;

        int cmp = compare(key, &base[mid]);

        if (cmp == 0)
        {
            foundUser = &base[mid];
            break;
        }
        else if (cmp < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return foundUser;
}

static Patient *customBinarySearchPatients(const Patient *key, Patient *base, int num, int (*compare)(const void *, const void *))
{
    if (num == 0)
        return NULL;
    int low = 0;
    int high = num - 1;
    Patient *foundPatient = NULL;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = compare(key, &base[mid]);
        if (cmp == 0)
        {
            foundPatient = &base[mid];
            break;
        }
        else if (cmp < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return foundPatient;
}

static Doctor *customBinarySearchDoctors(const Doctor *key, Doctor *base, int num, int (*compare)(const void *, const void *))
{
    if (num == 0)
        return NULL;
    int low = 0;
    int high = num - 1;
    Doctor *foundDoctor = NULL;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = compare(key, &base[mid]);
        if (cmp == 0)
        {
            foundDoctor = &base[mid];
            break;
        }
        else if (cmp < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return foundDoctor;
}

void findUser(Hospital *hospital, Session *session, const char *query, boolean byId)
{

    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }

    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari pengguna.");
        return;
    }

    printHeader("Hasil Pencarian Pengguna");

    if (hospital->users.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pengguna terdaftar.\n" COLOR_RESET);
        return;
    }

    boolean overallFound = false;
    boolean printedTableHeaders = false;
    int widths[] = {5, 20, 10, 20};
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"};
    char idStr[12];
    char roleStr[20];
    char diseaseStr[50];

    if (byId)
    {

        int targetId = stringToInt(query);

        if (targetId == -1 && strcmp(query, "-1") != 0 && strcmp(query, "0") != 0)
        {
            printError("ID pencarian tidak valid (harus berupa angka).");
            printf(COLOR_YELLOW "Pengguna dengan ID '%s' tidak ditemukan.\n" COLOR_RESET, query);
            return;
        }

        User keyUser;
        keyUser.id = targetId;

        qsort(hospital->users.elements, hospital->users.nEff, sizeof(User), compareUsersById);

        User *foundUser = customBinarySearchUsers(&keyUser, hospital->users.elements, hospital->users.nEff, compareUsersById);

        if (foundUser != NULL)
        {

            if (!printedTableHeaders)
            {
                printTableBorder(widths, 4, 1);
                printTableRow(headers, widths, 4);
                printTableBorder(widths, 4, 2);
                printedTableHeaders = true;
            }
            overallFound = true;

            if (!integerToString(foundUser->id, idStr, sizeof(idStr)))
            {
                return printError("Gagal konversi ID ke string!");
            }

            switch (foundUser->role)
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
                strcpy(roleStr, "N/A");
                break;
            }
            strcpy(diseaseStr, "-");

            if (foundUser->role == PATIENT)
            {
                for (int j = 0; j < hospital->patients.nEff; j++)
                {
                    if (hospital->patients.elements[j].id == foundUser->id && hospital->patients.elements[j].disease[0] != '\0')
                    {
                        strcpy(diseaseStr, hospital->patients.elements[j].disease);
                        break;
                    }
                }
            }

            const char *row[] = {idStr, foundUser->username, roleStr, diseaseStr};
            printTableRow(row, widths, 4);
        }
    }

    else
    {
        boolean exactMatchFound = false;

        for (int i = 0; i < hospital->users.nEff; i++)
        {
            User *user = &hospital->users.elements[i];

            if (caseInsensitiveStrcmp(user->username, query) == 0)
            {
                if (!printedTableHeaders)
                {
                    printTableBorder(widths, 4, 1);
                    printTableRow(headers, widths, 4);
                    printTableBorder(widths, 4, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                exactMatchFound = true;

                if (!integerToString(user->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
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
                    strcpy(roleStr, "N/A");
                    break;
                }
                strcpy(diseaseStr, "-");
                if (user->role == PATIENT)
                {
                    for (int j = 0; j < hospital->patients.nEff; j++)
                    {
                        if (hospital->patients.elements[j].id == user->id && hospital->patients.elements[j].disease[0] != '\0')
                        {
                            strcpy(diseaseStr, hospital->patients.elements[j].disease);
                            break;
                        }
                    }
                }
                const char *row[] = {idStr, user->username, roleStr, diseaseStr};
                printTableRow(row, widths, 4);
            }
        }

        if (!exactMatchFound && query[0] != '\0')
        {
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->users.nEff; i++)
            {
                User *user = &hospital->users.elements[i];

                if (localContainsCaseInsensitiveSubstring(user->username, query))
                {

                    if (!suggestionsFound)
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders)
                    {
                        printTableBorder(widths, 4, 1);
                        printTableRow(headers, widths, 4);
                        printTableBorder(widths, 4, 2);
                        printedTableHeaders = true;
                    }
                    overallFound = true;

                    if (!integerToString(user->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
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
                        strcpy(roleStr, "N/A");
                        break;
                    }
                    strcpy(diseaseStr, "-");
                    if (user->role == PATIENT)
                    {
                        for (int j = 0; j < hospital->patients.nEff; j++)
                        {
                            if (hospital->patients.elements[j].id == user->id && hospital->patients.elements[j].disease[0] != '\0')
                            {
                                strcpy(diseaseStr, hospital->patients.elements[j].disease);
                                break;
                            }
                        }
                    }
                    const char *row[] = {idStr, user->username, roleStr, diseaseStr};
                    printTableRow(row, widths, 4);
                }
            }
        }
    }

    if (overallFound)
    {
        printTableBorder(widths, 4, 3);
    }
    else
    {

        if (printedTableHeaders)
        {

            const char *noResults[] = {COLOR_GREY "0." COLOR_RESET, COLOR_GREY "Tidak Ditemukan" COLOR_RESET, COLOR_GREY "-" COLOR_RESET, COLOR_GREY "-" COLOR_RESET};
            printTableRow(noResults, widths, 4);
            printTableBorder(widths, 4, 3);
        }

        printf(COLOR_YELLOW "Pengguna dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}

void findPatient(Hospital *hospital, Session *session, const char *query, boolean byId, boolean byDisease)
{

    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari pasien.");
        return;
    }

    printHeader("Hasil Pencarian Pasien");
    if (hospital->patients.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pasien terdaftar.\n" COLOR_RESET);
        return;
    }

    boolean overallFound = false;
    boolean printedTableHeaders = false;
    int widths[] = {5, 20, 50};
    const char *headers[] = {"ID", "Username", "Penyakit"};
    char idStr[12];
    char diseaseStr[51];

    if (byDisease)
    {
        boolean exactMatchFound = false;

        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            Patient *patient = &hospital->patients.elements[i];
            if (caseInsensitiveStrcmp(patient->disease, query) == 0)
            {
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                exactMatchFound = true;
                if (!integerToString(patient->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }

                strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }

        if (!exactMatchFound && query[0] != '\0')
        {
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->patients.nEff; i++)
            {
                Patient *patient = &hospital->patients.elements[i];
                if (localContainsCaseInsensitiveSubstring(patient->disease, query))
                {
                    if (!suggestionsFound)
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk penyakit. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders)
                    { /* Cetak header tabel */
                        printTableBorder(widths, 3, 1);
                        printTableRow(headers, widths, 3);
                        printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }
                    overallFound = true;
                    if (!integerToString(patient->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
                    strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                    const char *row[] = {idStr, patient->username, diseaseStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    }

    else if (byId)
    {
        int targetId = stringToInt(query);
        if (targetId == -1 && strcmp(query, "-1") != 0 && strcmp(query, "0") != 0)
        {
            printError("ID pencarian tidak valid.");
        }
        else
        {
            Patient keyPatient;
            keyPatient.id = targetId;

            qsort(hospital->patients.elements, hospital->patients.nEff, sizeof(Patient), comparePatientsById);
            Patient *foundPatient = customBinarySearchPatients(&keyPatient, hospital->patients.elements, hospital->patients.nEff, comparePatientsById);
            if (foundPatient != NULL)
            {
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                if (!integerToString(foundPatient->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                strcpy(diseaseStr, foundPatient->disease[0] != '\0' ? foundPatient->disease : "-");
                const char *row[] = {idStr, foundPatient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
    }

    else
    {
        boolean exactMatchFound = false;

        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            Patient *patient = &hospital->patients.elements[i];
            if (caseInsensitiveStrcmp(patient->username, query) == 0)
            {
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                exactMatchFound = true;
                if (!integerToString(patient->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }

        if (!exactMatchFound && query[0] != '\0')
        {
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->patients.nEff; i++)
            {
                Patient *patient = &hospital->patients.elements[i];
                if (localContainsCaseInsensitiveSubstring(patient->username, query))
                {
                    if (!suggestionsFound)
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk nama. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders)
                    { /* Cetak header tabel */
                        printTableBorder(widths, 3, 1);
                        printTableRow(headers, widths, 3);
                        printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }
                    overallFound = true;
                    if (!integerToString(patient->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
                    strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                    const char *row[] = {idStr, patient->username, diseaseStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    }

    if (overallFound)
    {
        printTableBorder(widths, 3, 3);
    }

    else
    {

        if (printedTableHeaders)
        {
            const char *noResults[] = {COLOR_GREY "0." COLOR_RESET, COLOR_GREY "Tidak Ditemukan" COLOR_RESET, COLOR_GREY "-" COLOR_RESET};
            printTableRow(noResults, widths, 3);
            printTableBorder(widths, 3, 3);
        }

        char searchTypeStr[20];
        if (byDisease)
            strcpy(searchTypeStr, "penyakit");
        else if (byId)
            strcpy(searchTypeStr, "ID");
        else
            strcpy(searchTypeStr, "nama");

        char notFoundMsg[100];
        strcpy(notFoundMsg, "Pasien dengan ");
        strcat(notFoundMsg, searchTypeStr);
        strcat(notFoundMsg, " '");
        strcat(notFoundMsg, query);
        strcat(notFoundMsg, "' tidak ditemukan.");
        printf(COLOR_YELLOW "%s\n" COLOR_RESET, notFoundMsg);
    }
}

void findDoctor(Hospital *hospital, Session *session, const char *query, boolean byId)
{

    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari dokter.");
        return;
    }

    printHeader("Hasil Pencarian Dokter");
    if (hospital->doctors.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada dokter terdaftar.\n" COLOR_RESET);
        return;
    }

    boolean found = false;
    boolean printedTableHeaders = false;
    int widths[] = {5, 20, 10};
    const char *headers[] = {"ID", "Username", "Aura"};
    char idStr[12];
    char auraStr[16];

    if (byId)
    {
        int targetId = stringToInt(query);

        if (targetId == -1 && strcmp(query, "0") != 0 && strcmp(query, "-1") != 0)
        {
            printError("ID pencarian tidak valid (harus berupa angka).");
        }
        else
        {
            Doctor keyDoctor;
            keyDoctor.id = targetId;

            qsort(hospital->doctors.elements, hospital->doctors.nEff, sizeof(Doctor), compareDoctorsById);
            Doctor *foundDoctor = customBinarySearchDoctors(&keyDoctor, hospital->doctors.elements, hospital->doctors.nEff, compareDoctorsById);

            if (foundDoctor != NULL)
            {
                found = true;
                if (!printedTableHeaders)
                {
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }

                if (!integerToString(foundDoctor->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }

                if (!integerToString(foundDoctor->aura, auraStr, sizeof(auraStr)))
                {
                    return printError("Gagal konversi Aura ke string!");
                }

                const char *row[] = {idStr, foundDoctor->username, auraStr};
                printTableRow(row, widths, 3);
            }
        }
    }

    else
    {
        boolean exactMatchFound = false;
        boolean suggestionsFound = false;

        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            Doctor *doctor = &hospital->doctors.elements[i];
            if (caseInsensitiveStrcmp(doctor->username, query) == 0)
            {
                found = true;
                exactMatchFound = true;
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }

                if (!integerToString(doctor->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                if (!integerToString(doctor->aura, auraStr, sizeof(auraStr)))
                {
                    return printError("Gagal konversi Aura ke string!");
                }
                const char *row[] = {idStr, doctor->username, auraStr};
                printTableRow(row, widths, 3);
            }
        }

        if (!exactMatchFound && query[0] != '\0')
        {
            for (int i = 0; i < hospital->doctors.nEff; i++)
            {
                Doctor *doctor = &hospital->doctors.elements[i];

                if (localContainsCaseInsensitiveSubstring(doctor->username, query) &&
                    caseInsensitiveStrcmp(doctor->username, query) != 0)
                {
                    found = true;
                    if (!suggestionsFound)
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk nama. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders)
                    { /* Cetak header tabel */
                        printTableBorder(widths, 3, 1);
                        printTableRow(headers, widths, 3);
                        printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }

                    if (!integerToString(doctor->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
                    if (!integerToString(doctor->aura, auraStr, sizeof(auraStr)))
                    {
                        return printError("Gagal konversi Aura ke string!");
                    }
                    const char *row[] = {idStr, doctor->username, auraStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    }

    if (printedTableHeaders)
    {
        printTableBorder(widths, 3, 3);
    }

    if (!found)
    {

        printf(COLOR_YELLOW "Dokter dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}