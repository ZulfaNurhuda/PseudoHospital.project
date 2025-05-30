#include "registerPatient.h"
// #include "stringSet.h" // Removed as StringSet ADT was deleted
// utils.h should provide isUsernameTaken, ensure it's included if not via registerPatient.h
// No local static isUsernameTaken needed here anymore.

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
    // Check username uniqueness using the utility isUsernameTaken function
    if (isUsernameTaken(hospital, username)) { // This now calls the util version
        printError("Registrasi gagal! Pasien dengan nama tersebut sudah terdaftar."); // F02 spec message
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

    // Menambahkan ke UserMap
    int max_id = 0;
    for (int i = 0; i < hospital->users.nEff; i++) {
        if (hospital->users.elements[i].id > max_id) {
            max_id = hospital->users.elements[i].id;
        }
    }
    int new_patient_id = max_id + 1;

    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = new_patient_id;
    strcpy(newUser->username, username);
    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password!");
        return false;
    }
    newUser->role = PATIENT;

    // Menambahkan ke PatientList
    Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];
    newPatient->id = new_patient_id; // Use the same new_patient_id
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

    // Add the new username to the StringSet // This section is removed
    // if (!addToStringSet(&hospital->registeredUsernames, newUser->username)) {
    //     printError("Gagal menambahkan username ke daftar unik sistem. Registrasi mungkin tidak konsisten.");
    // }

    // Membuat pesan sukses
    char successMessage[100] = "Pasien ";
    strcat(successMessage, username);
    strcat(successMessage, " berhasil terdaftar!");
    printSuccess(successMessage);
    return true;
}