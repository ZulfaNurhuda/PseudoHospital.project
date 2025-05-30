#include "login.h"

boolean login(Hospital *hospital, Session *session, const char *username, const char *password)
{
    // Memeriksa apakah pointer hospital atau session adalah NULL.
    // Ini adalah pemeriksaan keamanan dasar untuk mencegah dereferensi pointer NULL.
    if (hospital == NULL || session == NULL)
    {
        // Mencetak pesan error jika salah satu pointer tidak valid.
        printError("Struktur rumah sakit atau sesi tidak valid!");
        // Mengembalikan false karena login tidak dapat dilanjutkan.
        return false;
    }

    // Memeriksa apakah username atau password adalah NULL atau string kosong.
    // Username dan password yang valid diperlukan untuk autentikasi.
    if (username == NULL || username[0] == '\0' || password == NULL || password[0] == '\0')
    {
        // Mencetak pesan error jika username atau password tidak valid.
        printError("Username atau password tidak valid!");
        // Mengembalikan false karena input tidak valid.
        return false;
    }

    // Memeriksa apakah format username valid menggunakan fungsi isValidUsername.
    // Fungsi ini diasumsikan ada dan melakukan validasi format username (misalnya, panjang, karakter yang diizinkan).
    if (!isValidUsername(username))
    {
        // Mencetak pesan error jika username tidak memenuhi kriteria validasi.
        printError("Username tidak valid!");
        // Mengembalikan false karena username tidak valid.
        return false;
    }

    // Memeriksa apakah panjang password kurang dari 6 karakter.
    // Ini adalah kebijakan keamanan umum untuk memastikan password cukup kuat.
    if (strlen(password) < 6)
    {
        // Mencetak pesan error jika password terlalu pendek.
        printError("Password harus minimal 6 karakter!");
        // Mengembalikan false karena password tidak memenuhi syarat panjang minimum.
        return false;
    }

    // Memeriksa apakah sesi saat ini sudah dalam keadaan login.
    // Mencegah pengguna yang sudah login untuk mencoba login lagi tanpa logout terlebih dahulu.
    if (session->isLoggedIn)
    {
        // Mencetak pesan error jika pengguna sudah login.
        printError("Anda sudah login! Silakan logout terlebih dahulu.");
        // Mengembalikan false karena pengguna sudah login.
        return false;
    }

    // Memeriksa apakah ada pengguna yang terdaftar di dalam struktur hospital.
    // nEff kemungkinan adalah jumlah elemen efektif (pengguna) dalam array users.
    if (hospital->users.nEff <= 0)
    {
        // Mencetak pesan error jika tidak ada pengguna terdaftar.
        printError("Tidak ada pengguna terdaftar!");
        // Mengembalikan false karena tidak ada akun untuk diverifikasi.
        return false;
    }

    // Melakukan iterasi melalui daftar pengguna yang terdaftar di rumah sakit.
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        // Membandingkan username yang diberikan dengan username pengguna saat ini dalam iterasi.
        // strcmp mengembalikan 0 jika string sama.
        if (strcmp(hospital->users.elements[i].username, username) == 0)
        {
            // Jika username ditemukan, lanjutkan dengan validasi password.

            // Validasi ukuran buffer untuk enkripsi password.
            // Mendapatkan panjang password yang dimasukkan pengguna.
            int passwordLength = strlen(password);
            // Memeriksa apakah panjang password melebihi batas buffer (misalnya, 99 karakter + null terminator).
            if (passwordLength >= 100) // Asumsi buffer enkripsi adalah 100
            {
                // Mencetak pesan error jika password terlalu panjang untuk dienkripsi dengan aman.
                printError("Panjang password terlalu besar untuk dienkripsi!");
                // Mengembalikan false karena potensi buffer overflow.
                return false;
            }

            // Mendeklarasikan array karakter untuk menyimpan password yang dienkripsi.
            char encryptedPassword[100]; // Ukuran buffer harus cukup untuk password terenkripsi.

            // Mencoba mengenkripsi password yang dimasukkan pengguna menggunakan fungsi enigmaEncrypt.
            // Fungsi ini diasumsikan ada dan melakukan enkripsi.
            // Parameter ketiga (100) adalah ukuran buffer output.
            if (!enigmaEncrypt(password, encryptedPassword, 100))
            {
                // Mencetak pesan error jika proses enkripsi gagal.
                printError("Gagal mengenkripsi password!");
                // Mengembalikan false karena kegagalan enkripsi.
                return false;
            }

            // Membandingkan password yang sudah dienkripsi dengan password terenkripsi yang tersimpan di database.
            // hospital->users.elements[i].password.encryptedContent adalah password terenkripsi pengguna yang tersimpan.
            if (strcmp(hospital->users.elements[i].password.encryptedContent, encryptedPassword) == 0)
            {
                // Jika password terenkripsi cocok, login berhasil.
                // Menyimpan ID pengguna, username, dan peran (role) ke dalam struktur session.
                session->userId = hospital->users.elements[i].id;
                strcpy(session->username, username); // Menyalin username ke session.
                session->role = hospital->users.elements[i].role;
                // Mengatur status isLoggedIn menjadi true.
                session->isLoggedIn = true;

                // Membuat pesan sukses yang lebih detail.
                char successMessage[100] = ""; // Ukuran buffer disesuaikan agar cukup
                // Menggabungkan string untuk membuat pesan sukses.
                strcat(successMessage, "Login berhasil sebagai ");
                strcat(successMessage, username);
                strcat(successMessage, "!");
                strcat(successMessage, " (Peran: ");
                // Menambahkan peran pengguna ke pesan berdasarkan nilai session->role.
                strcat(successMessage, (session->role == MANAGER) ? "Manajer" : (session->role == DOCTOR) ? "Dokter"
                                                                                                          : "Pasien");
                strcat(successMessage, ")");
                // Mencetak pesan sukses yang sudah dibentuk.
                printSuccess(successMessage);
                // Mengembalikan true karena login berhasil.
                return true;
            }
            // Jika password terenkripsi tidak cocok.
            printError("Password salah!");
            // Mengembalikan false karena password salah.
            return false; // Penting: Keluar setelah menemukan username tetapi password salah.
        }
    }
    // Jika loop selesai tanpa menemukan username yang cocok.
    printError("Username tidak ditemukan!");
    // Mengembalikan false karena username tidak terdaftar.
    return false;
}
