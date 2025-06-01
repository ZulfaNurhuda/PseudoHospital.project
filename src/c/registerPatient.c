#include "registerPatient.h"

boolean registerPatient(Hospital *hospital, Session *session, const char *inputUsername, const char *password)
{

    if (hospital == NULL || session == NULL)
    {

        printError("Struktur rumah sakit atau sesi tidak valid!");

        return false;
    }

    if (inputUsername == NULL || inputUsername[0] == '\0' || password == NULL || password[0] == '\0')
    {

        printError("Username (setelah trim) atau password tidak valid!");

        return false;
    }

    if (session->isLoggedIn)
    {

        printError("Anda sudah login! Silakan logout terlebih dahulu.");

        return false;
    }

    if (!isValidUsername(inputUsername))
    {

        printError("Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore.");

        return false;
    }

    if (isUsernameTaken(hospital, inputUsername))
    {

        printError("Registrasi gagal! Pasien dengan nama tersebut sudah terdaftar.");

        return false;
    }

    if (strlen(password) < 6)
    {

        printError("Password harus minimal 6 karakter!");

        return false;
    }

    if (hospital->users.nEff >= hospital->users.capacity || hospital->patients.nEff >= hospital->patients.capacity)
    {

        printError("Kapasitas pengguna atau pasien penuh!");

        return false;
    }

    int passwordLength = strlen(password);

    if (passwordLength >= 100)
    {

        printError("Panjang password terlalu besar untuk dienkripsi!");

        return false;
    }

    int maxId = 0;
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (hospital->users.elements[i].id > maxId)
        {
            maxId = hospital->users.elements[i].id;
        }
    }

    int newPatientId = maxId + 1;

    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = newPatientId;
    strcpy(newUser->username, inputUsername);

    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100))
    {

        printError("Gagal mengenkripsi password!");

        return false;
    }

    newUser->role = PATIENT;

    Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];
    newPatient->id = newPatientId;
    strcpy(newPatient->username, inputUsername);
    newPatient->bananaRich = 100.0;
    newPatient->life = 3;
    newPatient->diagnosedStatus = false;
    newPatient->treatedStatus = false;

    newPatient->medicationsPrescribed.medicationId = (int *)safeMalloc(10 * sizeof(int));
    if (newPatient->medicationsPrescribed.medicationId == NULL)
    {

        printError("Gagal alokasi memori untuk resep pasien!");

        return false;
    }
    newPatient->medicationsPrescribed.capacity = 10;
    newPatient->medicationsPrescribed.nEff = 0;

    newPatient->medicationsTaken.medicationId = (int *)safeMalloc(10 * sizeof(int));
    if (newPatient->medicationsTaken.medicationId == NULL)
    {

        printError("Gagal alokasi memori untuk obat yang diminum pasien!");

        free(newPatient->medicationsPrescribed.medicationId);
        newPatient->medicationsPrescribed.medicationId = NULL;

        return false;
    }
    newPatient->medicationsTaken.capacity = 10;
    newPatient->medicationsTaken.top = -1;
    newPatient->queueRoom[0] = '\0';
    newPatient->queuePosition = 0;

    hospital->users.nEff++;
    hospital->patients.nEff++;

    char successMessage[100] = "";

    strcat(successMessage, "Pasien dengan nama ");
    strcat(successMessage, inputUsername);
    strcat(successMessage, " berhasil terdaftar!");
    printSuccess(successMessage);

    session->userId = newPatientId;
    strcpy(session->username, inputUsername);
    session->role = PATIENT;
    session->isLoggedIn = true;

    printf("\n[ℹ️  | Info]: Fitur ini terintegrasi dengan fitur login, kamu tidak perlu login lagi.\n");
    printSuccess(FORMAT_BOLD "Anda langsung masuk sebagai pasien!" FORMAT_RESET);

    return true;
}