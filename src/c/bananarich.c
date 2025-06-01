#include "bananarich.h"

void viewWallet(Hospital *hospital, Session *session)
{
    // Validasi Hak Akses: Memastikan pengguna sudah login
    // dan memiliki peran sebagai Dokter atau Pasien.
    if (!session->isLoggedIn || (session->role != DOCTOR && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Dokter dan Pasien yang dapat melihat dompet."); // Pesan error jika akses tidak sesuai.
        return;                                                                          // Keluar dari fungsi.
    }

    // Mencetak header untuk tampilan saldo dompet.
    printHeader("Saldo Dompet");
    float balance = 0.0; // Variabel untuk menyimpan saldo, diinisialisasi 0.0.
    // Menentukan tipe pengguna (Pasien atau Dokter) berdasarkan peran di sesi.
    char *userType = session->role == PATIENT ? "Pasien" : "Dokter";

    // Jika pengguna yang login adalah Pasien.
    if (session->role == PATIENT)
    {
        // Mencari data pasien yang sesuai berdasarkan username dari sesi.
        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
            {
                // Jika ditemukan, ambil saldo BananaRich pasien.
                balance = hospital->patients.elements[i].bananaRich;
                break; // Keluar dari loop setelah pasien ditemukan.
            }
        }
    }
    // Jika pengguna yang login adalah Dokter.
    else // session->role == DOCTOR
    {
        // Mencari data dokter yang sesuai berdasarkan username dari sesi.
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            if (strcmp(hospital->doctors.elements[i].username, session->username) == 0)
            {
                // Jika ditemukan, ambil saldo BananaRich dokter.
                balance = hospital->doctors.elements[i].bananaRich;
                break; // Keluar dari loop setelah dokter ditemukan.
            }
        }
    }

    // Menggunakan fungsi `floatToString` untuk mengkonversi saldo (float) menjadi string dengan 2 angka desimal.
    char balanceStr[50] = ""; // Buffer untuk menyimpan string saldo.
    // `sizeof(balanceStr)` adalah ukuran buffer, 2 adalah presisi desimal.
    if (!floatToString(balance, balanceStr, sizeof(balanceStr), 2))
    {
        printError("Gagal mengonversi float ke string!"); // Pesan error jika konversi gagal.
        return;                                           // Keluar dari fungsi.
    }

    // Menambahkan unit mata uang " BananaRich" ke string saldo.
    strcat(balanceStr, " BananaRich");

    // Mendefinisikan lebar kolom dan header untuk tabel output.
    int widths[] = {15, 20}; // Lebar untuk kolom Tipe Pengguna dan Saldo.
    // Header kolom pertama disesuaikan berdasarkan tipe pengguna (Pasien/Dokter).
    const char *headers[] = {userType, "Saldo"};
    // Mencetak tabel.
    printTableBorder(widths, 2, 1);    // Border atas tabel (2 kolom, tipe border 1).
    printTableRow(headers, widths, 2); // Baris header.
    printTableBorder(widths, 2, 2);    // Garis pemisah di bawah header.
    // Data untuk baris tabel: username pengguna dan string saldo yang sudah diformat.
    const char *row[] = {session->username, balanceStr};
    printTableRow(row, widths, 2);  // Mencetak baris data.
    printTableBorder(widths, 2, 3); // Border bawah tabel.
}

void viewFinancial(Hospital *hospital, Session *session)
{
    // Validasi Hak Akses: Memastikan pengguna sudah login dan berperan sebagai Manajer.
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat finansial."); // Pesan error.
        return;                                                                   // Keluar dari fungsi.
    }

    // Mencetak header untuk tampilan finansial.
    printHeader("Finansial Rumah Sakit");
    // Mengambil saldo rumah sakit dari struktur `hospital->finance`.
    float balance = hospital->finance.hospitalBalance;

    // Mengkonversi saldo (float) menjadi string dengan 2 angka desimal.
    char balanceStr[50] = ""; // Buffer untuk string saldo.
    if (!floatToString(balance, balanceStr, sizeof(balanceStr), 2))
    {
        printError("Gagal mengonversi float ke string!"); // Pesan error jika konversi gagal.
        return;                                           // Keluar dari fungsi.
    }

    // Menambahkan unit mata uang " BananaRich" ke string saldo.
    strcat(balanceStr, " BananaRich");

    // Mendefinisikan lebar kolom dan header untuk tabel output.
    int widths[] = {15, 20}; // Lebar untuk kolom Entitas dan Saldo.
    const char *headers[] = {"Entitas", "Saldo"};
    // Mencetak tabel.
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    printTableBorder(widths, 2, 2);
    // Data untuk baris tabel: "Rumah Sakit" dan string saldo yang sudah diformat.
    const char *row[] = {"Rumah Sakit", balanceStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);
}

// Variabel statis global untuk melacak berapa kali fungsi gacha dipanggil.
// 'static' berarti variabel ini mempertahankan nilainya antar pemanggilan fungsi
// dan hanya dapat diakses dari dalam file ini. Digunakan sebagai salah satu sumber entropy untuk seed.
static int gachaCallCounter = 0;

// Fungsi hash sederhana untuk string (djb2 algorithm).
// Menghasilkan nilai hash numerik dari sebuah string.
// Parameter str: pointer ke string input.
// Mengembalikan nilai hash (unsigned long).
unsigned long hashString(const char *str)
{
    unsigned long hash = 5381; // Nilai awal hash (angka ajaib).
    int c;                     // Variabel untuk menyimpan karakter saat ini.
    // Melakukan iterasi melalui setiap karakter dalam string hingga null terminator.
    while ((c = *str++)) // Ambil karakter, lalu increment pointer str.
    {
        // Formula hash: hash = hash * 33 + c
        // (hash << 5) sama dengan hash * 32. Jadi, (hash << 5) + hash sama dengan hash * 33.
        hash = ((hash << 5) + hash) + c;
    }
    return hash; // Kembalikan nilai hash akhir.
}

// Fungsi untuk menghasilkan seed pseudo-random tanpa menggunakan time.h.
// Seed ini dibuat dengan mengkombinasikan beberapa sumber data yang bervariasi.
// Parameter patient: pointer ke struktur Patient yang datanya akan digunakan.
// Mengembalikan seed (unsigned long) yang dihasilkan.
unsigned long generateSeed(Patient *patient)
{
    // Increment counter setiap kali fungsi gacha (yang memanggil ini) dipanggil.
    gachaCallCounter++;

    unsigned long seed = 0; // Inisialisasi seed.

    // Mengkombinasikan berbagai sumber "entropy" yang tersedia menggunakan operasi XOR (^).
    // Tujuannya adalah untuk membuat seed yang berbeda untuk setiap pemanggilan dan setiap pasien.
    seed ^= hashString(patient->username);               // Menggunakan hash dari username pasien.
    seed ^= (unsigned long)(patient->bananaRich * 100);  // Menggunakan saldo BananaRich pasien (dikalikan 100 untuk menyertakan bagian desimal).
                                                         // Perhatian: ini bisa dikontrol oleh pemain jika mereka bisa memanipulasi saldo.
    seed ^= (unsigned long)gachaCallCounter * 1664525UL; // Menggunakan gachaCallCounter dikalikan dengan konstanta LCG.
    seed ^= (unsigned long)&patient;                     // Menggunakan alamat memori dari pointer patient.
    seed ^= (unsigned long)&seed;                        // Menggunakan alamat memori dari variabel seed itu sendiri (alamat stack).

    // Langkah pencampuran (mixing step) untuk mendistribusikan bit-bit seed secara lebih merata.
    // Ini membantu meningkatkan kualitas pseudo-randomness. Menggunakan konstanta MurmurHash.
    seed = (seed ^ (seed >> 16)) * 0x45d9f3bUL;
    seed = (seed ^ (seed >> 16)) * 0x45d9f3bUL;
    seed = seed ^ (seed >> 16);

    return seed; // Kembalikan seed yang sudah dihasilkan dan dicampur.
}

// Implementasi Linear Congruential Generator (LCG) sederhana untuk menghasilkan angka pseudo-random.
// Parameter seed: nilai seed input. Seed ini akan diupdate untuk pemanggilan berikutnya jika LCG digunakan secara berurutan.
// Namun, dalam fungsi `gacha`, seed baru digenerate setiap kali, jadi LCG di sini lebih seperti fungsi hash satu arah dari seed.
// Mengembalikan angka pseudo-random (unsigned long) dalam rentang 0 hingga 0xFFFFFFFFUL.
unsigned long lcgRandom(unsigned long seed)
{
    // Formula LCG: X_{n+1} = (a * X_n + c) mod m
    // Di sini, a = 1664525UL, c = 1013904223UL, m = 2^32 (implisit dari & 0xFFFFFFFFUL).
    // UL menandakan unsigned long.
    return (seed * 1664525UL + 1013904223UL) & 0xFFFFFFFFUL; // Masking dengan 0xFFFFFFFFUL untuk memastikan hasil 32-bit.
}

boolean gacha(Hospital *hospital, Session *session)
{
    // Validasi Hak Akses: Hanya Pasien yang dapat melakukan gacha.
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat melakukan gacha.");
        return false;
    }

    // Mencari data pasien yang sedang login.
    int patientIndex = -1;
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
        {
            patientIndex = i;
            break;
        }
    }
    // Jika pasien tidak ditemukan (seharusnya tidak terjadi jika sesi valid).
    if (patientIndex == -1)
    {
        printError("Pasien tidak ditemukan!");
        return false;
    }

    // Mendapatkan pointer ke struktur Patient.
    Patient *patient = &hospital->patients.elements[patientIndex];

    // Menghasilkan seed dinamis berdasarkan data pasien dan state program.
    unsigned long seed = generateSeed(patient);

    // Menghasilkan beberapa angka pseudo-random menggunakan LCG dengan seed yang dihasilkan.
    // Setiap pemanggilan lcgRandom menggunakan hasil sebelumnya sebagai seed baru, menciptakan urutan.
    unsigned long rand1 = lcgRandom(seed);  // Angka random pertama.
    unsigned long rand2 = lcgRandom(rand1); // Angka random kedua, berdasarkan pertama.
    unsigned long rand3 = lcgRandom(rand2); // Angka random ketiga, berdasarkan kedua.

    float reward = 0.0;           // Variabel untuk menyimpan jumlah hadiah BananaRich.
    char rewardMessage[100] = ""; // Buffer untuk pesan hasil gacha.

    // Menggunakan `rand2` untuk menentukan tipe hadiah (probabilitas).
    // `rand2 % 100` menghasilkan angka antara 0 dan 99.
    int rewardType = rand2 % 100;

    // Menggunakan `rand3` untuk memilih item spesifik dalam kategori hadiah (jika ada beberapa pilihan).
    // `rand3 % 3` menghasilkan angka antara 0, 1, atau 2.
    int itemChoice = rand3 % 3;

    // ---------------------------------------------------
    // | Distribusi probabilitas hadiah gacha:           |
    // ---------------------------------------------------
    // | 40% (0-39)   - Tidak dapat apa-apa              |
    // | 35% (40-74)  - Hadiah kecil (Small rewards)     |
    // | 15% (75-89)  - Hadiah sedang (Medium rewards)   |
    // |  8% (90-97)  - Hadiah besar (Large rewards)     |
    // |  2% (98-99)  - Jackpot                          |
    // ---------------------------------------------------

    // Logika penentuan hadiah berdasarkan `rewardType`.
    if (rewardType < 40) // Probabilitas 40%
    {
        // Tidak mendapatkan hadiah (zonk).
        // Mencetak header dan pesan hasil gacha.
        printHeader("Hasil Gacha");
        printf("Selamat datang %s, di Mesin Gacha %sXXeon06%s!\n", patient->username, FORMAT_BOLD COLOR_BLUE, FORMAT_RESET);
        strcpy(rewardMessage, "[👀 Yahhh] - Kamu kurang beruntung, coba lagi nanti!");
        // Pesan ini akan dicetak lagi di bawah, jadi pencetakan di sini mungkin redundan atau untuk efek langsung.
        printf(COLOR_YELLOW FORMAT_BOLD "%s\n" FORMAT_RESET, rewardMessage);
        return true; // Proses gacha selesai (meskipun tidak dapat hadiah).
    }
    else if (rewardType < 75) // Probabilitas 35% (indeks 40 hingga 74)
    {
        // Hadiah kecil: 2, 5, atau 10 BananaRich, dipilih berdasarkan `itemChoice`.
        int smallRewards[] = {2, 5, 10};
        reward = smallRewards[itemChoice]; // itemChoice adalah 0, 1, atau 2.
        // Membangun pesan hadiah.
        strcat(rewardMessage, "[😲 WOW] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];                                     // Buffer untuk string hadiah.
        floatToString(reward, rewardStr, sizeof(rewardStr), 2); // Konversi float ke string.
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }
    else if (rewardType < 90) // Probabilitas 15% (indeks 75 hingga 89)
    {
        // Hadiah sedang: 15, 20, atau 25 BananaRich.
        int mediumRewards[] = {15, 20, 25};
        reward = mediumRewards[itemChoice];
        strcat(rewardMessage, "[🎉 YAY] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];
        floatToString(reward, rewardStr, sizeof(rewardStr), 2);
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }
    else if (rewardType < 98) // Probabilitas 8% (indeks 90 hingga 97)
    {
        // Hadiah besar: 50 atau 100 BananaRich.
        int largeRewards[] = {50, 100};
        reward = largeRewards[itemChoice % 2]; // `itemChoice % 2` untuk memilih antara 2 opsi (0 atau 1).
        strcat(rewardMessage, "[🥳 HOREE] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];
        floatToString(reward, rewardStr, sizeof(rewardStr), 2);
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }
    else // Probabilitas 2% (indeks 98 hingga 99)
    {
        // Hadiah Jackpot: 200 BananaRich.
        reward = 200;
        strcat(rewardMessage, "[🎊 JACKPOT] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];
        floatToString(reward, rewardStr, sizeof(rewardStr), 2);
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }

    // Menambahkan hadiah ke saldo BananaRich pasien.
    patient->bananaRich += reward;

    // Mencetak header dan pesan hasil gacha.
    printHeader("Hasil Gacha");
    // Pesan selamat datang ke mesin gacha.
    printf("Selamat datang %s, di Mesin Gacha %sXXeon06%s!\n", patient->username, FORMAT_BOLD COLOR_BLUE, FORMAT_RESET);
    // Mencetak pesan hadiah yang sudah dibangun.
    printf(COLOR_YELLOW FORMAT_BOLD "%s\n" FORMAT_RESET, rewardMessage);

    // Menyiapkan dan mencetak informasi saldo sebelum dan sesudah gacha.
    char beforeStr[50] = "";                            // Buffer untuk saldo sebelum.
    char afterStr[50] = "";                             // Buffer untuk saldo sesudah.
    float beforeBalance = patient->bananaRich - reward; // Hitung saldo sebelum (setelah ditambah reward, lalu dikurangi lagi).
    float afterBalance = patient->bananaRich;           // Saldo saat ini adalah saldo sesudah.

    // Konversi saldo sebelum dan sesudah ke string.
    if (!floatToString(beforeBalance, beforeStr, sizeof(beforeStr), 2) ||
        !floatToString(afterBalance, afterStr, sizeof(afterStr), 2))
    {
        printError("Gagal mengonversi saldo ke string!"); // Pesan error jika konversi gagal.
        return false;                                     // Gacha dianggap gagal jika tampilan saldo error.
    }

    // Mencetak informasi saldo.
    printf("\n[💰 | Info]: Saldo sebelum: %s\n", beforeStr);
    printf("[🤑 | Info]: Saldo sesudah: %s\n", afterStr);

    return true; // Proses gacha berhasil.
}