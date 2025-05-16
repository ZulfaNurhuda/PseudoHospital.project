#include "registerPatient.h"

// Fungsi untuk memeriksa apakah username sudah ada
static boolean isUsernameTaken(Hospital *hospital, const char *username)
{
    char lowerUsername[strlen(username) + 1];
    for (int i = 0; i < strlen(username); i++)
    {
        char c = username[i];
        if (c >= 'A' && c <= 'Z')
        {
            c = c + ('a' - 'A');
        }
        lowerUsername[i] = c;
    }
    lowerUsername[strlen(username)] = '\0';

    for (int i = 0; i < hospital->users.nEff; i++)
    {
        char lowerElementUsername[strlen(hospital->users.elements[i].username) + 1];
        for (int j = 0; j < strlen(hospital->users.elements[i].username); j++)
        {
            char c = hospital->users.elements[i].username[j];
            if (c >= 'A' && c <= 'Z')
            {
                c = c + ('a' - 'A');
            }
            lowerElementUsername[j] = c;
        }
        lowerElementUsername[strlen(hospital->users.elements[i].username)] = '\0';

        if (strcmp(lowerUsername, lowerElementUsername) == 0)
        {
            return true;
        }
    }
    return false;
}

boolean registerPatient(Hospital *hospital, Session *session, const char *username, const char *password)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    if (username == NULL || username[0] == '\0' || password == NULL || password[0] == '\0')
    {
        printError("Username atau password tidak valid!");
        return false;
    }
    if (session->isLoggedIn)
    {
        printError("Anda sudah login! Silakan logout terlebih dahulu.");
        return false;
    }
    if (!isValidUsername(username))
    {
        printError("Username tidak valid! Gunakan huruf, angka, atau underscore.");
        return false;
    }
    if (strlen(password) < 6)
    {
        printError("Password harus minimal 6 karakter!");
        return false;
    }
    if (isUsernameTaken(hospital, username))
    {
        printError("Username sudah terdaftar!");
        return false;
    }
    if (hospital->users.nEff >= hospital->users.capacity || hospital->patients.nEff >= hospital->patients.capacity)
    {
        printError("Kapasitas pengguna atau pasien penuh!");
        return false;
    }

    // Validasi ukuran buffer untuk enkripsi
    int passwordLength = strlen(password);
    if (passwordLength >= 100)
    {
        printError("Panjang password terlalu besar untuk dienkripsi!");
        return false;
    }

    // Menambahkan ke UserList
    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = hospital->users.nEff + 1;
    strcpy(newUser->username, username);
    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password!");
        return false;
    }
    newUser->role = PATIENT;

    // Menambahkan ke PatientList
    Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];
    newPatient->id = newUser->id;
    strcpy(newPatient->username, username);
    newPatient->bananaRich = 100.0;
    newPatient->life = 3;
    newPatient->diagnosedStatus = false;
    newPatient->treatedStatus = false;
    newPatient->medicationsPrescribed.medicationID = (int *)safeMalloc(10 * sizeof(int));
    newPatient->medicationsPrescribed.capacity = 10;
    newPatient->medicationsPrescribed.nEff = 0;
    newPatient->medicationsTaken.medicationID = (int *)safeMalloc(10 * sizeof(int));
    newPatient->medicationsTaken.capacity = 10;
    newPatient->medicationsTaken.top = -1;
    newPatient->queueRoom[0] = '\0';
    newPatient->queuePosition = 0;

    hospital->users.nEff++;
    hospital->patients.nEff++;

    // Membuat pesan sukses
    char successMessage[100] = "Pasien ";
    strcat(successMessage, username);
    strcat(successMessage, " berhasil terdaftar!");
    printSuccess(successMessage);
    return true;
}