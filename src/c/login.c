#include "login.h"

boolean login(Hospital *hospital, Session *session, const char *username, const char *password)
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
    if (!isValidUsername(username))
    {
        printError("Username tidak valid!");
        return false;
    }
    if (strlen(password) < 6)
    {
        printError("Password harus minimal 6 karakter!");
        return false;
    }
    if (session->isLoggedIn)
    {
        printError("Anda sudah login! Silakan logout terlebih dahulu.");
        return false;
    }

    if (hospital->users.nEff <= 0)
    {
        printError("Tidak ada pengguna terdaftar!");
        return false;
    }

    // Mencari pengguna
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (strcmp(hospital->users.elements[i].username, username) == 0)
        {
            // Validasi ukuran buffer untuk enkripsi
            int passwordLength = strlen(password);
            if (passwordLength >= 100)
            {
                printError("Panjang password terlalu besar untuk dienkripsi!");
                return false;
            }

            char encryptedPassword[100];
            if (!enigmaEncrypt(password, encryptedPassword, 100))
            {
                printError("Gagal mengenkripsi password!");
                return false;
            }
            if (strcmp(hospital->users.elements[i].password.encryptedContent, encryptedPassword) == 0)
            {
                session->userId = hospital->users.elements[i].id;
                strcpy(session->username, username);
                session->role = hospital->users.elements[i].role;
                session->isLoggedIn = true;
                printSuccess("Login berhasil!");
                return true;
            }
            printError("Password salah!");
            return false;
        }
    }
    printError("Username tidak ditemukan!");
    return false;
}