#include "addDoctor.h"
// #include "stringSet.h" // Removed as StringSet ADT was deleted
// utils.h should provide isUsernameTaken, ensure it's included if not via addDoctor.h
// No local static isUsernameTaken needed here anymore.

boolean addDoctor(Hospital *hospital, Session *session, const char *username, const char *password, const char *specialization)
{
    if (hospital == NULL || session == NULL || username == NULL || password == NULL || specialization == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau input tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat menambah dokter.");
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

    if (!isValidUsername(specialization))
    {
        printError("Spesialisasi tidak valid! Gunakan huruf, angka, atau underscore.");
        return false;
    }

    // Check username uniqueness using the utility isUsernameTaken function
    if (isUsernameTaken(hospital, username)) { // This now calls the util version
        printError("Username sudah terdaftar!"); // F10 spec message
        return false;
    }

    if (hospital->users.nEff >= hospital->users.capacity || hospital->doctors.nEff >= hospital->doctors.capacity)
    {
        printError("Kapasitas pengguna atau dokter penuh!");
        return false;
    }

    // Menambahkan ke UserMap
    int max_id = 0;
    for (int i = 0; i < hospital->users.nEff; i++) {
        if (hospital->users.elements[i].id > max_id) {
            max_id = hospital->users.elements[i].id;
        }
    }
    int new_doctor_id = max_id + 1;

    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = new_doctor_id;
    strcpy(newUser->username, username);
    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password!");
        return false;
    }
    newUser->role = DOCTOR;

    // Menambahkan ke DoctorList
    Doctor *newDoctor = &hospital->doctors.elements[hospital->doctors.nEff];
    newDoctor->id = new_doctor_id; // Use the same new_doctor_id
    strcpy(newDoctor->username, username);
    strcpy(newDoctor->specialization, specialization);
    newDoctor->aura = 0;
    newDoctor->bananaRich = 100.0f;
    newDoctor->room[0] = '\0';

    hospital->users.nEff++;
    hospital->doctors.nEff++;

    // Add the new username to the StringSet // This section is removed
    // if (!addToStringSet(&hospital->registeredUsernames, newUser->username)) {
    //     printError("Gagal menambahkan username dokter ke daftar unik sistem. Penambahan dokter mungkin tidak konsisten.");
    // }

    // Membuat pesan sukses
    char successMsg[100] = "Dokter ";
    strcat(successMsg, username);
    strcat(successMsg, " berhasil ditambahkan!");
    printSuccess(successMsg);
    return true;
}

boolean assignDoctor(Hospital *hospital, Session *session, const char *username, const char *roomCode)
{
    if (hospital == NULL || session == NULL || username == NULL || roomCode == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau input tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat menugaskan dokter.");
        return false;
    }

    if (!isValidUsername(username))
    {
        printError("Username dokter tidak valid!");
        return false;
    }

    if (!isValidRoomCode(hospital, roomCode))
    {
        printError("Kode ruangan tidak valid! Contoh: A1");
        return false;
    }

    int doctorIdx = -1;
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        if (strcmp(hospital->doctors.elements[i].username, username) == 0)
        {
            doctorIdx = i;
            break;
        }
    }
    if (doctorIdx == -1)
    {
        printError("Dokter tidak ditemukan!");
        return false;
    }

    Room *room = NULL;
    if (hospital->layout.rowEff > 0 && hospital->layout.colEff > 0)
    {
        for (int i = 0; i < hospital->layout.rowEff; i++)
        {
            for (int j = 0; j < hospital->layout.colEff; j++)
            {
                if (strcmp(hospital->layout.elements[i][j].code, roomCode) == 0)
                {
                    room = &hospital->layout.elements[i][j];
                    break;
                }
            }
        }
    }
    if (room == NULL)
    {
        printError("Ruangan tidak ditemukan!");
        return false;
    }

    if (room->doctorID != -1)
    {
        printError("Ruangan sudah ditempati dokter lain!");
        return false;
    }

    Doctor *doctor = &hospital->doctors.elements[doctorIdx];
    if (doctor->room[0] != '\0')
    {
        for (int i = 0; i < hospital->layout.rowEff; i++)
        {
            for (int j = 0; j < hospital->layout.colEff; j++)
            {
                if (strcmp(hospital->layout.elements[i][j].code, doctor->room) == 0)
                {
                    hospital->layout.elements[i][j].doctorID = -1;
                    break;
                }
            }
        }
    }

    room->doctorID = doctor->id;
    strcpy(doctor->room, roomCode);

    // Membuat pesan sukses
    char successMsg[100] = "Dokter %s ditugaskan ke ruangan %s!";
    strcat(successMsg, username);
    strcat(successMsg, " ditugaskan ke ruangan ");
    strcat(successMsg, roomCode);
    strcat(successMsg, "!");
    printSuccess(successMsg);
    return true;
}