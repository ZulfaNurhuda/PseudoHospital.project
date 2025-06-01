#include "menuHelp.h"

// Fungsi statis untuk menormalisasi string perintah.
// Mengubah semua huruf menjadi huruf besar dan menghapus karakter '_'.
// Fungsi ini bersifat 'static' sehingga hanya dapat diakses dari dalam file ini.
static void normalizeCommand(char *command)
{
    int i = 0, j = 0; // i untuk iterasi string asli, j untuk iterasi string hasil normalisasi.
    // Melakukan iterasi selama karakter saat ini bukan null terminator.
    while (command[i])
    {
        // Jika karakter saat ini bukan underscore.
        if (command[i] != '_')
        {
            // Mengubah huruf kecil menjadi huruf besar, karakter lain tetap.
            // Jika command[i] adalah huruf kecil (antara 'a' dan 'z'), kurangi 32 untuk menjadikannya huruf besar.
            command[j] = (command[i] >= 'a' && command[i] <= 'z') ? command[i] - 32 : command[i];
            j++; // Pindah ke posisi berikutnya di string hasil normalisasi.
        }
        i++; // Pindah ke karakter berikutnya di string asli.
    }
    command[j] = '\0'; // Menambahkan null terminator pada akhir string hasil normalisasi.
}

// Fungsi statis untuk mencetak catatan kaki (footnote) di akhir menu.
// Memberikan informasi tambahan atau pengingat kepada pengguna.
static void printFootnote()
{
    printf("\nFootnote:\n");                                                              // Judul untuk catatan kaki.
    printf("Untuk menggunakan aplikasi, silahkan masukkan nama fungsi yang terdaftar\n"); // Baris pertama catatan.
    printf("Jangan lupa untuk memasukkan input yang valid\n");                            // Baris kedua catatan.
}

void displayMenu(Session *session)
{
    // Memeriksa validitas pointer session.
    if (session == NULL)
    {
        printError("Sesi tidak valid!"); // Mencetak pesan error jika sesi tidak valid.
        return;                          // Keluar dari fungsi.
    }

    // Mencetak header untuk daftar perintah.
    printHeader("Daftar Perintah");
    // Array untuk menyimpan daftar perintah yang akan ditampilkan. Ukuran 20 dianggap cukup.
    const char *menu[20];
    int count = 0; // Penghitung jumlah perintah yang ditambahkan ke array menu.
    // Array untuk menentukan lebar kolom tabel. Dalam kasus ini, hanya satu kolom dengan lebar 20.
    int widths[] = {20};

    // Jika pengguna belum login.
    if (!session->isLoggedIn)
    {
        // Mencetak pesan bahwa pengguna belum login dan menampilkan perintah yang tersedia.
        // COLOR_YELLOW dan COLOR_RESET adalah makro yang diasumsikan untuk mewarnai teks di terminal.
        printf(COLOR_YELLOW "Anda belum login. Perintah yang tersedia:\n" COLOR_RESET);
        // Menambahkan perintah-perintah yang relevan untuk pengguna yang belum login.
        menu[count++] = "LOGIN";
        menu[count++] = "REGISTER_PASIEN";
        menu[count++] = "LUPA_PASSWORD";
        menu[count++] = "MENU";
        menu[count++] = "HELP";
        menu[count++] = "EXIT";
    }
    // Jika pengguna sudah login.
    else
    {
        // Menggunakan switch-case berdasarkan peran pengguna untuk menentukan perintah yang tersedia.
        switch (session->role)
        {
        case DOCTOR: // Jika peran adalah Dokter.
            // Mencetak pesan selamat datang yang disesuaikan untuk Dokter.
            printf(COLOR_YELLOW "Halo Dokter %s. Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n" COLOR_RESET, session->username);
            // Menambahkan perintah-perintah yang relevan untuk Dokter.
            menu[count++] = "LOGOUT";
            menu[count++] = "MENU";
            menu[count++] = "HELP";
            menu[count++] = "LIHAT_DENAH";
            menu[count++] = "DIAGNOSIS";
            menu[count++] = "NGOBATIN"; 
            menu[count++] = "LIHAT_DOMPET";
            menu[count++] = "EXIT";
            break;
        case MANAGER: // Jika peran adalah Manajer.
            // Mencetak pesan selamat datang yang disesuaikan untuk Manajer.
            printf(COLOR_YELLOW "Halo Manager %s. Kenapa kamu memanggil command HELP? Kan kamu manager, tapi yasudahlah kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n" COLOR_RESET, session->username);
            // Menambahkan perintah-perintah yang relevan untuk Manajer.
            menu[count++] = "LOGOUT";
            menu[count++] = "MENU";
            menu[count++] = "HELP";
            menu[count++] = "LIHAT_DENAH";
            menu[count++] = "LIHAT_USER";
            menu[count++] = "CARI_USER";
            menu[count++] = "LIHAT_ANTRIAN";
            menu[count++] = "TAMBAH_DOKTER";
            menu[count++] = "UBAH_DENAH";
            menu[count++] = "PINDAH_DOKTER";
            menu[count++] = "LIHAT_FINANSIAL";
            menu[count++] = "EXIT";
            break;
        case PATIENT: // Jika peran adalah Pasien.
            // Mencetak pesan selamat datang yang disesuaikan untuk Pasien.
            printf(COLOR_YELLOW "Selamat datang, %s. Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n" COLOR_RESET, session->username);
            // Menambahkan perintah-perintah yang relevan untuk Pasien.
            menu[count++] = "LOGOUT";
            menu[count++] = "MENU";
            menu[count++] = "HELP";
            menu[count++] = "LIHAT_DENAH";
            menu[count++] = "PULANG_DOK";
            menu[count++] = "DAFTAR_CHECKUP";
            menu[count++] = "ANTRIAN_SAYA";
            menu[count++] = "MINUM_OBAT";
            menu[count++] = "MINUM_PENAWAR";
            menu[count++] = "LIHAT_DOMPET";
            menu[count++] = "GACHA";
            menu[count++] = "LEWATI_ANTRIAN";
            menu[count++] = "BATALKAN_ANTRIAN";
            menu[count++] = "EXIT";
            break;
            // default: // Bisa ditambahkan case default jika ada peran lain atau untuk menangani peran tidak dikenal.
            //     printError("Peran pengguna tidak dikenal dalam menu!");
            //     break;
        }
    }

    // Mencetak tabel yang berisi daftar perintah.
    // printTableBorder dan printTableRow adalah fungsi utilitas yang diasumsikan untuk memformat output sebagai tabel.
    // Argumen ketiga pada printTableBorder (1 atau 3) mungkin menentukan jenis border (atas, tengah, atau bawah).
    printTableBorder(widths, 1, 1); // Mencetak border atas tabel.
    // Melakukan iterasi melalui array menu dan mencetak setiap perintah sebagai baris tabel.
    for (int i = 0; i < count; i++)
    {
        printTableRow(&menu[i], widths, 1); // Mencetak satu baris tabel.
    }
    printTableBorder(widths, 1, 3); // Mencetak border bawah tabel.
    // Mencetak catatan kaki di akhir menu.
    printFootnote();
}

void displayHelp(Session *session, const char *command)
{
    // Memeriksa validitas pointer session.
    if (session == NULL)
    {
        printError("Sesi tidak valid!");
        return;
    }
    // Memeriksa validitas string command.
    if (command == NULL || command[0] == '\0')
    {
        printError("Perintah tidak valid!");
        return;
    }

    // Buffer untuk menyimpan versi normal dari perintah.
    char normalCommand[50];
    strcpy(normalCommand, command);  // Menyalin perintah asli ke buffer.
    normalizeCommand(normalCommand); // Menormalisasi perintah (uppercase, hapus '_').

    // Mencetak header untuk tampilan bantuan.
    printHeader("Bantuan Perintah");
    printf("\n"); // Spasi setelah header.

    // Logika bantuan jika pengguna BELUM LOGIN.
    if (!session->isLoggedIn)
    {
        printf("Anda belum login! Silakan login terlebih dahulu.\n");
        // Menampilkan bantuan untuk perintah yang diminta (menggunakan 'command' asli untuk tampilan).
        printf("Bantuan untuk perintah '%s%s%s':\n", FORMAT_BOLD, command, FORMAT_RESET);
        printf("────────────────────────────────────────────────────────\n");

        // Membandingkan 'normalCommand' dengan daftar perintah yang diketahui untuk pengguna yang belum login.
        if (strcmp(normalCommand, "LOGIN") == 0)
        {
            printf("  🔑 %sLOGIN%s\n", FORMAT_BOLD, FORMAT_RESET);
            printf("     Deskripsi: Masuk ke sistem dengan username dan password Anda.\n");
        }
        else if (strcmp(normalCommand, "REGISTERPATIENT") == 0) // Normalisasi dari "REGISTER_PATIENT"
        {
            printf("  📝 %sREGISTER_PATIENT%s\n", FORMAT_BOLD, FORMAT_RESET);
            printf("     Deskripsi: Daftar sebagai pasien baru dengan username dan password.\n");
        }
        else if (strcmp(normalCommand, "LUPAPASSWORD") == 0) // Normalisasi dari "LUPA_PASSWORD"
        {
            printf("  ❓ %sLUPA_PASSWORD%s\n", FORMAT_BOLD, FORMAT_RESET);
            printf("     Deskripsi: Atur ulang kata sandi menggunakan username dan kode verifikasi (RLE).\n");
        }
        else if (strcmp(normalCommand, "MENU") == 0)
        {
            printf("  📜 %sMENU%s\n", FORMAT_BOLD, FORMAT_RESET);
            printf("     Deskripsi: Tampilkan daftar semua perintah yang tersedia untuk Anda saat ini.\n");
        }
        else if (strcmp(normalCommand, "HELP") == 0)
        {
            printf("  ℹ️  %sHELP%s\n", FORMAT_BOLD, FORMAT_RESET);
            printf("     Deskripsi: Tampilkan bantuan detail untuk perintah tertentu.\n");
        }
        else if (strcmp(normalCommand, "EXIT") == 0)
        {
            printf("  🚪 %sEXIT%s\n", FORMAT_BOLD, FORMAT_RESET);
            printf("     Deskripsi: Keluar dari sistem aplikasi.\n");
        }
        else // Jika perintah tidak dikenal.
        {
            char errorMessage[100] = "";
            strcat(errorMessage, "Perintah '");
            strcat(errorMessage, command); // Menampilkan 'command' asli dalam pesan error.
            strcat(errorMessage, "' tidak dikenal atau tidak tersedia saat ini.");
            printError(errorMessage);
        }
    }
    // Logika bantuan jika pengguna SUDAH LOGIN.
    else
    {
        // Menampilkan status login dan peran pengguna.
        printf("Anda login sebagai %s%s%s.\n", FORMAT_BOLD, (session->role == MANAGER) ? "Manajer" : (session->role == DOCTOR) ? "Dokter"
                                                                                                                               : "Pasien",
               FORMAT_RESET);
        // Menampilkan bantuan untuk perintah yang diminta (menggunakan 'command' asli).
        printf("Bantuan untuk perintah '%s%s%s':\n", FORMAT_BOLD, command, FORMAT_RESET);
        printf("────────────────────────────────────────────────────────\n");

        // Switch berdasarkan peran pengguna untuk menampilkan bantuan yang relevan.
        switch (session->role)
        {
        case MANAGER: // Bantuan untuk Manajer.
            if (strcmp(normalCommand, "LOGOUT") == 0)
            {
                printf("  🚪 %sLOGOUT%s\n     Deskripsi: Keluar dari sesi Anda saat ini.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "MENU") == 0)
            {
                printf("  📜 %sMENU%s\n     Deskripsi: Tampilkan daftar semua perintah yang tersedia.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "HELP") == 0)
            {
                printf("  ℹ️  %sHELP%s\n     Deskripsi: Tampilkan bantuan untuk perintah tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATDENAH") == 0)
            { // Normalisasi dari "LIHAT_DENAH"
                printf("  🗺️ %sLIHAT_DENAH%s\n     Deskripsi: Tampilkan denah rumah sakit atau detail ruangan tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATUSER") == 0)
            { // Normalisasi dari "LIHAT_USER"
                printf("  👥 %sLIHAT_USER%s\n     Deskripsi: Tampilkan daftar semua pengguna (pasien atau dokter).\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "CARIUSER") == 0)
            { // Normalisasi dari "CARI_USER"
                printf("  🔍 %sCARI_USER%s\n     Deskripsi: Cari pengguna berdasarkan ID atau username.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATANTRIAN") == 0)
            { // Normalisasi dari "LIHAT_ANTRIAN"
                printf("  🚶 %sLIHAT_ANTRIAN%s\n     Deskripsi: Tampilkan antrian pasien di setiap ruangan atau ruangan tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "TAMBAHDOKTER") == 0)
            { // Normalisasi dari "TAMBAH_DOKTER"
                printf("  👨‍⚕️ %sTAMBAH_DOKTER%s\n     Deskripsi: Tambah dokter baru ke sistem atau tetapkan dokter ke ruangan.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "UBAHDENAH") == 0)
            { // Normalisasi dari "UBAH_DENAH"
                printf("  ✏️ %sUBAH_DENAH%s\n     Deskripsi: Ubah ukuran denah rumah sakit (jumlah baris dan kolom).\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "PINDAHDOKTER") == 0)
            { // Normalisasi dari "PINDAH_DOKTER"
                printf("  🔄 %sPINDAH_DOKTER%s\n     Deskripsi: Pindahkan dokter dari satu ruangan ke ruangan lain.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATFINANSIAL") == 0)
            { // Normalisasi dari "LIHAT_FINANSIAL"
                printf("  💰 %sLIHAT_FINANSIAL%s\n     Deskripsi: Tampilkan laporan keuangan rumah sakit.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "EXIT") == 0)
            {
                printf("  🚪 %sEXIT%s\n     Deskripsi: Keluar dari sistem aplikasi.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else
            { // Perintah tidak dikenal untuk Manajer.
                char errorMessage[100] = "";
                strcat(errorMessage, "Perintah '");
                strcat(errorMessage, command); // Menampilkan 'command' asli.
                strcat(errorMessage, "' tidak dikenal atau tidak tersedia untuk Manajer.");
                printError(errorMessage);
            }
            break;

        case DOCTOR: // Bantuan untuk Dokter.
            if (strcmp(normalCommand, "LOGOUT") == 0)
            {
                printf("  🚪 %sLOGOUT%s\n     Deskripsi: Keluar dari sesi Anda saat ini.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "MENU") == 0)
            {
                printf("  📜 %sMENU%s\n     Deskripsi: Tampilkan daftar semua perintah yang tersedia.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "HELP") == 0)
            {
                printf("  ℹ️  %sHELP%s\n     Deskripsi: Tampilkan bantuan untuk perintah tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATDENAH") == 0)
            {
                printf("  🗺️ %sLIHAT_DENAH%s\n     Deskripsi: Tampilkan denah rumah sakit atau detail ruangan tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "DIAGNOSIS") == 0)
            {
                printf("  🩺 %sDIAGNOSIS%s\n     Deskripsi: Lakukan diagnosis otomatis untuk pasien di ruangan Anda.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "NGOBATIN") == 0)
            { 
                printf("  💊 %sNGOBATIN%s\n     Deskripsi: Berikan resep obat otomatis berdasarkan penyakit pasien.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATDOMPET") == 0)
            { // Normalisasi dari "LIHAT_DOMPET"
                printf("  💵 %sLIHAT_DOMPET%s\n     Deskripsi: Tampilkan saldo BananaRich Anda.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "EXIT") == 0)
            {
                printf("  🚪 %sEXIT%s\n     Deskripsi: Keluar dari sistem aplikasi.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else
            { // Perintah tidak dikenal untuk Dokter.
                char errorMessage[100] = "";
                strcat(errorMessage, "Perintah '");
                strcat(errorMessage, command); // Menampilkan 'command' asli.
                strcat(errorMessage, "' tidak dikenal atau tidak tersedia untuk Dokter.");
                printError(errorMessage);
            }
            break;

        case PATIENT: // Bantuan untuk Pasien.
            if (strcmp(normalCommand, "LOGOUT") == 0)
            {
                printf("  🚪 %sLOGOUT%s\n     Deskripsi: Keluar dari sesi Anda saat ini.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "MENU") == 0)
            {
                printf("  📜 %sMENU%s\n     Deskripsi: Tampilkan daftar semua perintah yang tersedia.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "HELP") == 0)
            {
                printf("  ℹ️  %sHELP%s\n     Deskripsi: Tampilkan bantuan untuk perintah tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATDENAH") == 0)
            {
                printf("  🗺️ %sLIHAT_DENAH%s\n     Deskripsi: Tampilkan denah rumah sakit atau detail ruangan tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "PULANGDOK") == 0)
            { // Normalisasi dari "PULANG_DOK"
                printf("  🚶‍♀️ %sPULANG_DOK%s\n     Deskripsi: Periksa apakah Anda sudah boleh pulang berdasarkan status pengobatan.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "DAFTARCHECKUP") == 0)
            { // Normalisasi dari "DAFTAR_CHECKUP"
                printf("  ➕ %sDAFTAR_CHECKUP%s\n     Deskripsi: Daftar untuk pemeriksaan dengan dokter di ruangan tertentu.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "ANTRIANSAYA") == 0)
            { // Normalisasi dari "ANTRIAN_SAYA"
                printf("  🔢 %sANTRIAN_SAYA%s\n     Deskripsi: Tampilkan status antrian Anda saat ini.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "MINUMOBAT") == 0)
            { // Normalisasi dari "MINUM_OBAT"
                printf("  💧 %sMINUM_OBAT%s\n     Deskripsi: Minum obat sesuai urutan yang telah diresepkan oleh dokter.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "MINUMPENAWAR") == 0)
            { // Normalisasi dari "MINUM_PENAWAR"
                printf("  🧪 %sMINUM_PENAWAR%s\n     Deskripsi: Minum penawar untuk membatalkan efek obat terakhir yang diminum.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LIHATDOMPET") == 0)
            {
                printf("  💵 %sLIHAT_DOMPET%s\n     Deskripsi: Tampilkan saldo BananaRich Anda.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "GACHA") == 0)
            {
                printf("  🎰 %sGACHA%s\n     Deskripsi: Gunakan sejumlah BananaRich untuk mencoba peruntungan di mesin gacha.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "LEWATIANTRIAN") == 0)
            { // Normalisasi dari "LEWATI_ANTRIAN"
                printf("  💨 %sLEWATI_ANTRIAN%s\n     Deskripsi: Gunakan item khusus (jika punya) untuk melewati antrian pemeriksaan.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "BATALKANANTRIAN") == 0)
            { // Normalisasi dari "BATALKAN_ANTRIAN"
                printf("  ❌ %sBATALKAN_ANTRIAN%s\n     Deskripsi: Batalkan antrian pemeriksaan Anda saat ini.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else if (strcmp(normalCommand, "EXIT") == 0)
            {
                printf("  🚪 %sEXIT%s\n     Deskripsi: Keluar dari sistem aplikasi.\n", FORMAT_BOLD, FORMAT_RESET);
            }
            else
            { // Perintah tidak dikenal untuk Pasien.
                char errorMessage[100] = "";
                strcat(errorMessage, "Perintah '");
                strcat(errorMessage, command); // Menampilkan 'command' asli.
                strcat(errorMessage, "' tidak dikenal atau tidak tersedia untuk Pasien.");
                printError(errorMessage);
            }
            break;
        default: // Jika peran tidak dikenal.
            printError("Peran pengguna tidak dikenal!");
            break;
        }
    }
}