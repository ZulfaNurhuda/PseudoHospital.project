#include "forgotPassword.h"

boolean forgotPassword(Hospital *hospital, const char *username, const char *rleCode, const char *newPassword)
{
    // Memeriksa apakah pointer hospital adalah NULL.
    if (hospital == NULL)
    {
        // Mencetak pesan error jika struktur rumah sakit tidak valid.
        printError("Struktur rumah sakit tidak valid!");
        // Mengembalikan false karena proses tidak dapat dilanjutkan.
        return false;
    }

    // Memeriksa apakah input username, rleCode, atau newPassword NULL atau string kosong.
    if (username == NULL || username[0] == '\0' || rleCode == NULL || rleCode[0] == '\0' || newPassword == NULL || newPassword[0] == '\0')
    {
        // Mencetak pesan error jika ada input yang tidak valid.
        printError("Input tidak valid!");
        // Mengembalikan false.
        return false;
    }

    // Memeriksa validitas format username.
    if (!isValidUsername(username))
    {
        // Mencetak pesan error jika username tidak valid.
        printError("Username tidak valid!");
        // Mengembalikan false.
        return false;
    }

    // Memeriksa apakah panjang password baru memenuhi syarat minimum (minimal 6 karakter).
    if (strlen(newPassword) < 6)
    {
        // Mencetak pesan error jika password baru terlalu pendek.
        printError("Password baru harus minimal 6 karakter!");
        // Mengembalikan false.
        return false;
    }

    // Memeriksa apakah ada pengguna yang terdaftar di rumah sakit.
    if (hospital->users.nEff <= 0)
    {
        // Mencetak pesan error jika tidak ada pengguna terdaftar.
        printError("Tidak ada pengguna terdaftar!");
        // Mengembalikan false.
        return false;
    }

    // Mencari indeks pengguna berdasarkan username.
    int userIdx = -1; // Inisialisasi indeks pengguna dengan -1 (tidak ditemukan).
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        // Membandingkan username input dengan username pengguna dalam daftar.
        if (strcmp(hospital->users.elements[i].username, username) == 0)
        {
            userIdx = i; // Jika ditemukan, simpan indeksnya.
            break;       // Keluar dari loop karena pengguna sudah ditemukan.
        }
    }

    // Memeriksa apakah pengguna ditemukan.
    if (userIdx == -1)
    {
        // Mencetak pesan error jika username tidak terdaftar.
        printError("Username tidak terdaftar!");
        // Mengembalikan false.
        return false;
    }

    // Validasi ukuran buffer untuk RLE dan enkripsi password baru.
    int passwordLength = strlen(newPassword);
    // Memeriksa apakah panjang password baru melebihi batas buffer enkripsi (misalnya, 99 karakter + null).
    if (passwordLength >= 100) // Asumsi buffer enkripsi adalah 100
    {
        // Mencetak pesan error jika password terlalu panjang.
        printError("Panjang password terlalu besar untuk dienkripsi!");
        // Mengembalikan false.
        return false;
    }

    // Menghasilkan kode RLE yang diharapkan berdasarkan username pengguna.
    char expectedRle[100]; // Buffer untuk menyimpan kode RLE yang diharapkan. Ukuran disesuaikan.
    // Memanggil fungsi generateRleCode untuk membuat kode RLE dari username.
    if (!generateRleCode(username, expectedRle, 100)) // Ukuran buffer 100
    {
        // Mencetak pesan error jika gagal menghasilkan kode RLE.
        printError("Gagal menghasilkan kode RLE!");
        // Mengembalikan false.
        return false;
    }

    // Membandingkan kode RLE yang dimasukkan pengguna dengan kode RLE yang diharapkan.
    if (strcmp(rleCode, expectedRle) != 0)
    {
        // Mencetak pesan error jika kode RLE salah.
        printError("Kode RLE salah!");
        // Mengembalikan false.
        return false;
    }

    // Jika kode RLE cocok, enkripsi password baru dan simpan.
    // Mengenkripsi password baru dan menyimpannya ke dalam data pengguna.
    if (!enigmaEncrypt(newPassword, hospital->users.elements[userIdx].password.encryptedContent, 100)) // Ukuran buffer 100
    {
        // Mencetak pesan error jika gagal mengenkripsi password baru.
        printError("Gagal mengenkripsi password baru!");
        // Mengembalikan false.
        return false;
    }

    // Membuat pesan sukses kosong.
    char successMessage[100] = ""; // Buffer untuk pesan sukses.
    // Menggabungkan string untuk membuat pesan sukses.
    strcat(successMessage, "Password untuk ");
    strcat(successMessage, username);
    strcat(successMessage, " berhasil diperbarui!");
    // Mencetak pesan sukses.
    printSuccess(successMessage);
    // Mengembalikan true menandakan password berhasil diperbarui.
    return true;
}
