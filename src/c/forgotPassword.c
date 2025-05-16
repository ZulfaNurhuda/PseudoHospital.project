#include "forgotPassword.h"

boolean forgotPassword(Hospital *hospital, const char *username, const char *rleCode, const char *newPassword)
{
    if (hospital == NULL)
    {
        printError("Struktur rumah sakit tidak valid!");
        return false;
    }
    if (username == NULL || username[0] == '\0' || rleCode == NULL || rleCode[0] == '\0' || newPassword == NULL || newPassword[0] == '\0')
    {
        printError("Input tidak valid!");
        return false;
    }
    if (!isValidUsername(username))
    {
        printError("Username tidak valid!");
        return false;
    }
    if (strlen(newPassword) < 6)
    {
        printError("Password baru harus minimal 6 karakter!");
        return false;
    }
    if (hospital->users.nEff <= 0)
    {
        printError("Tidak ada pengguna terdaftar!");
        return false;
    }

    int userIdx = -1;
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (strcmp(hospital->users.elements[i].username, username) == 0)
        {
            userIdx = i;
            break;
        }
    }

    if (userIdx == -1)
    {
        printError("Username tidak terdaftar!");
        return false;
    }

    // Validasi ukuran buffer untuk RLE dan enkripsi
    int passwordLength = strlen(newPassword);
    if (passwordLength >= 100)
    {
        printError("Panjang password terlalu besar untuk dienkripsi!");
        return false;
    }

    char expectedRle[100];
    if (!generateRleCode(username, expectedRle, 100))
    {
        printError("Gagal menghasilkan kode RLE!");
        return false;
    }

    if (strcmp(rleCode, expectedRle) != 0)
    {
        printError("Kode RLE salah!");
        return false;
    }

    if (!enigmaEncrypt(newPassword, hospital->users.elements[userIdx].password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password baru!");
        return false;
    }

    // Membuat pesan sukses
    char successMessage[100] = "Password untuk ";
    strcat(successMessage, username);
    strcat(successMessage, " berhasil diperbarui!");
    printSuccess(successMessage);
    return true;
}