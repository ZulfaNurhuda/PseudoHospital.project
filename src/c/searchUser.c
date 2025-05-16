#include "searchUser.h"

/**
 * `[Add-ons]` - Nama Fungsi: `containsSubstring`
 * 
 * Deskripsi: Memeriksa apakah `str` mengandung substring `substr`.
 *
 * Parameter:
 *   - `str (const char*)`: String yang akan dicari.
 *   - `substr (const char*)`: Substring yang akan dicari.
 *
 * Return: `boolean` - True jika `str` mengandung `substr`, false jika tidak.
 */
static boolean containsSubstring(const char *str, const char *substr)
{
    if (str == NULL || substr == NULL || str[0] == '\0' || substr[0] == '\0')
    {
        return false;
    }
    int strLen = strlen(str);
    int substrLen = strlen(substr);
    if (substrLen > strLen)
    {
        return false;
    }
    for (int i = 0; i <= strLen - substrLen; i++)
    {
        boolean match = true;
        for (int j = 0; j < substrLen; j++)
        {
            if (str[i + j] != substr[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            return true;
        }
    }
    return false;
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

    boolean found = false;
    int widths[] = {5, 20, 10, 20};
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"};
    printTableBorder(widths, 4, 1);
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 4, 2);

    for (int i = 0; i < hospital->users.nEff; i++)
    {
        User *user = &hospital->users.elements[i];
        char idStr[10];
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
        if (byId ? (strcmp(idStr, query) == 0) : containsSubstring(user->username, query))
        {
            found = true;
            char roleStr[20], diseaseStr[20] = "-";
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
    }
    printTableBorder(widths, 4, 3);

    if (!found)
    {
        printf(COLOR_YELLOW "Pengguna dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}

void findPatient(Hospital *hospital, Session *session, const char *query, boolean byId)
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

    boolean found = false;
    int widths[] = {5, 20, 20};
    const char *headers[] = {"ID", "Username", "Penyakit"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        Patient *patient = &hospital->patients.elements[i];
        char idStr[10];
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
        if (byId ? (strcmp(idStr, query) == 0) : containsSubstring(patient->username, query))
        {
            found = true;
            char diseaseStr[20] = "-";
            if (strlen(patient->disease) > 0 && patient->disease[0] != '\0')
            {
                strcpy(diseaseStr, patient->disease);
            }
            const char *row[] = {idStr, patient->username, diseaseStr};
            printTableRow(row, widths, 3);
        }
    }
    printTableBorder(widths, 3, 3);

    if (!found)
    {
        printf(COLOR_YELLOW "Pasien dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
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
        if (byId ? (strcmp(idStr, query) == 0) : containsSubstring(doctor->username, query))
        {
            found = true;
            const char *row[] = {idStr, doctor->username, auraStr};
            printTableRow(row, widths, 3);
        }
    }
    printTableBorder(widths, 3, 3);

    if (!found)
    {
        printf(COLOR_YELLOW "Dokter dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}