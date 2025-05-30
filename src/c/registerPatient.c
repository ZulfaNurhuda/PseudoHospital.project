#include "registerPatient.h"
// #include "stringSet.h" // Removed as StringSet ADT was deleted
// utils.h should provide isUsernameTaken, ensure it's included if not via registerPatient.h
// No local static isUsernameTaken needed here anymore.

boolean registerPatient(Hospital *hospital, Session *session, const char *inputUsername, const char *password)
{
    // char username[50] will be used to store the trimmed username from inputUsername if needed,
    // or inputUsername directly if function signature is kept for direct input.
    // For now, assuming inputUsername is the one read from console.
    // The prompt asks to change readValidString TO readUsernameWithTrim.
    // This means the function signature of registerPatient itself doesn't change,
    // but how 'username' is obtained *before* calling registerPatient would change (in hospitalSystem.c)
    // OR, if registerPatient itself is responsible for reading, then internal char username[50] is used.
    // The current structure of hospitalSystem.c reads then calls. So inputUsername is already read.
    // This task is to modify THIS file, assuming inputUsername is passed.
    // However, the prompt for this step says "Replace readValidString for username with readUsernameWithTrim"
    // which implies this function *was* doing the reading.
    // Let's look at hospitalSystem.c:
    // else if (strcmp(command, "REGISTERPASIEN") == 0)
    // {
    //     char username[50], password[100];
    //     if (!readValidString(username, 50, "Masukkan username: ", true) || ... )
    //     registerPatient(&hospital, &session, username, password);
    // }
    // So, hospitalSystem.c DOES the reading. This function (registerPatient) gets a username.
    // The task should be to update hospitalSystem.c for reading, and here we just use the result.
    //
    // Re-interpreting: "Replace Username Input" means replace the readValidString in hospitalSystem.c.
    // And then here, we apply isValidUsername and isUsernameTaken to the passed 'username'.
    // The prompt is slightly ambiguous if it means to change reading *within* this file or *before* it.
    // Given the structure, changes are in hospitalSystem.c for reading, and here for validation.
    // For this file, the main change is the VALIDATION sequence.
    // The passed `username` is assumed to be already trimmed by `readUsernameWithTrim` (called in hospitalSystem.c).

    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    // The `username` parameter here is the result of `readUsernameWithTrim` from hospitalSystem.c
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

    // 1. Validate format using the new isValidUsername (allows spaces)
    if (!isValidUsername(inputUsername)) {
        // Error message from F02 spec for format validation: "Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore."
        // The existing message from isValidUsername itself might be generic. F02 asks for a specific one here.
        printError("Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore.");
        return false;
    }

    // 2. Check uniqueness using isUsernameTaken (case-insensitive)
    if (isUsernameTaken(hospital, inputUsername)) { 
        printError("Registrasi gagal! Pasien dengan nama tersebut sudah terdaftar."); // F02 spec message
        return false;
    }

    if (strlen(password) < 6)
    {
        printError("Password harus minimal 6 karakter!");
        return false;
    }
    // The previous isUsernameTaken check is now done above.
    // The isValidUsername check is also done above.
    // The strlen(password) < 6 check can remain here or be moved up.
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

    // Menambahkan ke UserSet
    int max_id = 0;
    for (int i = 0; i < hospital->users.nEff; i++) {
        if (hospital->users.elements[i].id > max_id) {
            max_id = hospital->users.elements[i].id;
        }
    }
    int new_patient_id = max_id + 1;

    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = new_patient_id;
    strcpy(newUser->username, inputUsername); // Use inputUsername
    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password!");
        return false;
    }
    newUser->role = PATIENT;

    // Menambahkan ke PatientList
    Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];
    newPatient->id = new_patient_id; // Use the same new_patient_id
    strcpy(newPatient->username, inputUsername); // Use inputUsername
    newPatient->bananaRich = 100.0;
    newPatient->life = 3;
    newPatient->diagnosedStatus = false;
    newPatient->treatedStatus = false;

    newPatient->medicationsPrescribed.medicationID = (int *)safeMalloc(10 * sizeof(int));
    if (newPatient->medicationsPrescribed.medicationID == NULL) {
        printError("Gagal alokasi memori untuk resep pasien!");
        // No nEff increment yet, so no need to roll back user/patient count
        return false;
    }
    newPatient->medicationsPrescribed.capacity = 10;
    newPatient->medicationsPrescribed.nEff = 0;

    newPatient->medicationsTaken.medicationID = (int *)safeMalloc(10 * sizeof(int));
    if (newPatient->medicationsTaken.medicationID == NULL) {
        printError("Gagal alokasi memori untuk obat yang diminum pasien!");
        free(newPatient->medicationsPrescribed.medicationID); // Free the previously allocated memory
        newPatient->medicationsPrescribed.medicationID = NULL;
        // No nEff increment yet
        return false;
    }
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
    strcat(successMessage, inputUsername); // Use inputUsername
    strcat(successMessage, " berhasil terdaftar!");
    printSuccess(successMessage);
    return true;
}