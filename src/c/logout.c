#include "logout.h"

boolean logout(Session *session)
{
    // Memeriksa apakah pointer session adalah NULL.
    // Ini adalah pemeriksaan keamanan dasar untuk mencegah dereferensi pointer NULL.
    if (session == NULL)
    {
        // Mencetak pesan error jika sesi tidak valid.
        printError("Sesi tidak valid!");
        // Mengembalikan false karena logout tidak dapat dilanjutkan.
        return false;
    }

    // Memeriksa apakah pengguna saat ini tidak dalam keadaan login.
    // Jika pengguna belum login, tidak ada operasi logout yang perlu dilakukan.
    if (!session->isLoggedIn)
    {
        // Mencetak pesan error jika pengguna belum login.
        printError("Anda belum login! Silakan login terlebih dahulu.");
        // Mengembalikan false karena tidak ada sesi aktif untuk di-logout.
        return false;
    }

    // Menyimpan username sementara sebelum direset dari sesi.
    // Ukuran buffer tempUsername harus cukup untuk menampung username.
    char tempUsername[50]; // Asumsi panjang username maksimal 49 karakter + null terminator
    // Menyalin username dari sesi ke variabel sementara.
    strcpy(tempUsername, session->username);

    // Mereset informasi pengguna dalam struktur sesi.
    // Mengatur userId menjadi nilai default (misalnya, -1) untuk menandakan tidak ada pengguna yang login.
    session->userId = -1;
    // Mengosongkan username dalam sesi dengan mengatur karakter pertama menjadi null terminator.
    session->username[0] = '\0';
    // Mereset peran pengguna menjadi nilai default (misalnya, -1).
    session->role = -1; // Asumsi -1 atau nilai enum/define lain untuk menandakan tidak ada peran.
    // Mengatur status isLoggedIn menjadi false untuk menandakan pengguna telah logout.
    session->isLoggedIn = false;

    // Membuat dan mencetak pesan sukses setelah logout berhasil.
    char successMessage[100]; // Buffer untuk pesan sukses.
    // Menginisialisasi successMessage sebagai string kosong sebelum menggunakan strcat.
    successMessage[0] = '\0';
    // Menggabungkan string untuk membentuk pesan sukses.
    strcat(successMessage, "Logout berhasil! Sampai jumpa, ");
    strcat(successMessage, tempUsername); // Menambahkan username yang disimpan sementara.
    strcat(successMessage, "!");

    // Mencetak pesan sukses menggunakan fungsi printSuccess.
    printSuccess(successMessage);
    // Mengembalikan true karena logout berhasil.
    return true;
}
