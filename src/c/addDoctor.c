#include "addDoctor.h"

boolean addDoctor(Hospital *hospital, Session *session, const char *inputUsername, const char *password, const char *specialization)
{
    if (hospital == NULL || session == NULL || inputUsername == NULL || password == NULL || specialization == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau input tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat menambah dokter.");
        return false;
    }

    if (!isValidUsername(inputUsername)) {
        printError("Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore.");
        return false;
    }

    if (isUsernameTaken(hospital, inputUsername)) { 
        printError("Username sudah terdaftar!");
        return false;
    }

    if (strlen(password) < 6)
    {
        printError("Password harus minimal 6 karakter!");
        return false;
    }

    // Karena spesialisasi memiliki batasan yang sama dengan username
    // untuk validasi, dapat digunakan fungsi yang sama
    if (!isValidUsername(specialization))
    {
        printError("Spesialisasi tidak valid! Gunakan huruf, angka, atau underscore.");
        return false;
    }

    if (hospital->users.nEff >= hospital->users.capacity || hospital->doctors.nEff >= hospital->doctors.capacity)
    {
        int newCapacity = hospital->users.capacity * 2;
        User *tempUsers = realloc(hospital->users.elements, newCapacity * sizeof(User));
        if (tempUsers == NULL)
        {
            printError("Gagal mengalokasi memori untuk ekspansi pengguna!");
            return false;
        }
        hospital->users.elements = tempUsers;
        hospital->users.capacity = newCapacity;

        int newDoctorCapacity = hospital->doctors.capacity * 2;
        Doctor *tempDoctors = realloc(hospital->doctors.elements, newDoctorCapacity * sizeof(Doctor));
        if (tempDoctors == NULL)
        {
            printError("Gagal mengalokasi memori untuk ekspansi dokter!");
            return false;
        }
        hospital->doctors.elements = tempDoctors;
        hospital->doctors.capacity = newDoctorCapacity;
    }

    int maxId = 0;
    for (int i = 0; i < hospital->users.nEff; i++) {
        if (hospital->users.elements[i].id > maxId)
        {
            maxId = hospital->users.elements[i].id;
        }
    }
    int newDoctorId = maxId + 1;

    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = newDoctorId;
    strcpy(newUser->username, inputUsername);
    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password!");
        return false;
    }
    newUser->role = DOCTOR;

    Doctor *newDoctor = &hospital->doctors.elements[hospital->doctors.nEff];
    newDoctor->id = newDoctorId;
    strcpy(newDoctor->username, inputUsername);
    strcpy(newDoctor->specialization, specialization);
    newDoctor->aura = 0;
    newDoctor->bananaRich = 100.0f;
    newDoctor->room[0] = '\0';

    hospital->users.nEff++;
    hospital->doctors.nEff++;

    char successMsg[100] = "";
    strcat(successMsg, "Dokter ");
    strcat(successMsg, inputUsername);
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

    if (room->doctorId != -1)
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
                    hospital->layout.elements[i][j].doctorId = -1;
                    break;
                }
            }
        }
    }

    room->doctorId = doctor->id;
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