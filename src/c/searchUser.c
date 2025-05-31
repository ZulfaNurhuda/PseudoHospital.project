#include "searchUser.h"

// Fungsi pembantu (helper function) statis: Mengubah sebuah karakter ke huruf kecil.
// 'static' berarti fungsi ini hanya dapat diakses dari dalam file ini.
// Parameter c: karakter input.
// Mengembalikan karakter versi huruf kecil jika input adalah huruf besar, jika tidak, mengembalikan karakter asli.
static char localCharToLower(char c)
{
    // Memeriksa apakah karakter c adalah huruf besar (antara 'A' dan 'Z').
    if (c >= 'A' && c <= 'Z')
    {
        // Mengkonversi ke huruf kecil dengan menambahkan selisih antara 'a' dan 'A'.
        return c + ('a' - 'A');
    }
    // Jika bukan huruf besar, kembalikan karakter apa adanya.
    return c;
}

// Fungsi pembantu statis: Perbandingan dua string secara case-insensitive (tidak membedakan huruf besar/kecil).
// Parameter s1: string pertama.
// Parameter s2: string kedua.
// Mengembalikan:
//   - 0 jika s1 dan s2 sama (case-insensitive).
//   - nilai negatif jika s1 < s2 (case-insensitive).
//   - nilai positif jika s1 > s2 (case-insensitive).
static int localCaseInsensitiveStrcmp(const char *s1, const char *s2)
{
    // Menangani kasus jika kedua string NULL.
    if (s1 == NULL && s2 == NULL)
        return 0; // Dianggap sama.
    // Menangani kasus jika hanya s1 yang NULL.
    if (s1 == NULL)
        return -1; // s1 dianggap lebih kecil.
    // Menangani kasus jika hanya s2 yang NULL.
    if (s2 == NULL)
        return 1; // s1 dianggap lebih besar.

    int i = 0; // Indeks untuk iterasi.
    // Melakukan iterasi selama kedua string belum mencapai null terminator.
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        // Mengubah karakter dari kedua string ke huruf kecil untuk perbandingan.
        char c1Lower = localCharToLower(s1[i]);
        char c2Lower = localCharToLower(s2[i]);
        // Jika karakter yang sudah dikecilkan berbeda.
        if (c1Lower != c2Lower)
        {
            // Kembalikan selisihnya (menentukan urutan leksikografis).
            return c1Lower - c2Lower;
        }
        i++; // Pindah ke karakter berikutnya.
    }
    // Setelah salah satu string habis, bandingkan karakter null terminator (atau sisa karakter jika panjang berbeda).
    // Ini menangani kasus di mana satu string adalah awalan dari string lain (misalnya, "tes" vs "test").
    return localCharToLower(s1[i]) - localCharToLower(s2[i]);
}

// Fungsi pembantu statis: Memeriksa apakah sebuah string (text) mengandung substring (pattern) secara case-insensitive.
// Parameter text: string utama yang akan dicari di dalamnya.
// Parameter pattern: substring yang akan dicari.
// Mengembalikan true jika pattern ditemukan dalam text (case-insensitive), false jika tidak.
static boolean localContainsCaseInsensitiveSubstring(const char *text, const char *pattern)
{
    // Menangani kasus jika text atau pattern NULL.
    if (text == NULL || pattern == NULL)
        return false; // Tidak bisa mengandung jika salah satunya NULL.

    // Menghitung panjang string text.
    int textLen = 0;
    while (text[textLen] != '\0')
        textLen++;

    // Menghitung panjang string pattern.
    int patternLen = 0;
    while (pattern[patternLen] != '\0')
        patternLen++;

    // Jika pattern kosong, dianggap selalu terkandung.
    if (patternLen == 0)
        return true;
    // Jika pattern lebih panjang dari text, tidak mungkin terkandung.
    if (patternLen > textLen)
        return false;

    // Melakukan iterasi melalui text untuk mencari kemungkinan awal pattern.
    // Loop berjalan dari indeks 0 hingga (textLen - patternLen) agar pattern tidak melebihi batas text.
    for (int i = 0; i <= textLen - patternLen; i++)
    {
        boolean match = true; // Asumsikan cocok pada awalnya.
        // Melakukan iterasi melalui pattern untuk membandingkan karakter demi karakter.
        for (int j = 0; j < patternLen; j++)
        {
            // Membandingkan karakter dari text (mulai dari i+j) dengan karakter pattern (j), keduanya dikecilkan.
            if (localCharToLower(text[i + j]) != localCharToLower(pattern[j]))
            {
                match = false; // Jika ada karakter yang tidak cocok, set match ke false.
                break;         // Keluar dari loop inner (perbandingan pattern).
            }
        }
        if (match)       // Jika semua karakter pattern cocok dengan bagian dari text.
            return true; // Kembalikan true.
    }
    // Jika loop selesai tanpa menemukan kecocokan.
    return false;
}

// Fungsi Pembanding statis untuk qsort atau binary search: Membandingkan dua User berdasarkan ID.
// Ini adalah versi sederhana yang mengembalikan -1, 0, atau 1, umum untuk bsearch.
// qsort biasanya mengharapkan selisih (seperti compareUserByIdAsc sebelumnya).
// Fungsi ini tampaknya disiapkan untuk customBinarySearch.
static int compareUsersById(const void *a, const void *b)
{
    // Cast pointer void ke User.
    User *userA = (User *)a;
    User *userB = (User *)b;
    // Perbandingan ID.
    if (userA->id < userB->id)
        return -1; // userA lebih kecil.
    if (userA->id > userB->id)
        return 1; // userA lebih besar.
    return 0;     // ID sama.
}

// Fungsi Pembanding statis untuk qsort atau binary search: Membandingkan dua Patient berdasarkan ID.
static int comparePatientsById(const void *a, const void *b)
{
    Patient *patientA = (Patient *)a;
    Patient *patientB = (Patient *)b;
    if (patientA->id < patientB->id)
        return -1;
    if (patientA->id > patientB->id)
        return 1;
    return 0;
}

// Fungsi Pembanding statis untuk qsort atau binary search: Membandingkan dua Doctor berdasarkan ID.
static int compareDoctorsById(const void *a, const void *b)
{
    Doctor *doctorA = (Doctor *)a;
    Doctor *doctorB = (Doctor *)b;
    if (doctorA->id < doctorB->id)
        return -1;
    if (doctorA->id > doctorB->id)
        return 1;
    return 0;
}

// Implementasi Binary Search kustom untuk array User.
// Parameter key: pointer ke User yang dicari (hanya field ID yang relevan di sini).
// Parameter base: pointer ke awal array User yang sudah terurut.
// Parameter num: jumlah elemen dalam array.
// Parameter compare: pointer ke fungsi pembanding (seperti compareUsersById).
// Mengembalikan pointer ke User yang ditemukan, atau NULL jika tidak ditemukan.
static User *customBinarySearchUsers(const User *key, User *base, int num, int (*compare)(const void *, const void *))
{
    // Jika array kosong, tidak ada yang bisa dicari.
    if (num == 0)
        return NULL;

    int low = 0;            // Indeks awal pencarian.
    int high = num - 1;     // Indeks akhir pencarian.
    User *foundUser = NULL; // Pointer untuk hasil, diinisialisasi NULL.

    // Loop binary search.
    while (low <= high)
    {
        // Hitung indeks tengah untuk menghindari overflow potensial dari (low + high) / 2.
        int mid = low + (high - low) / 2;
        // Panggil fungsi pembanding.
        int cmp = compare(key, &base[mid]);

        if (cmp == 0) // Jika elemen ditemukan (hasil perbandingan 0).
        {
            foundUser = &base[mid]; // Simpan alamat elemen yang ditemukan.
            break;                  // Keluar dari loop.
        }
        else if (cmp < 0) // Jika key lebih kecil dari elemen tengah.
        {
            high = mid - 1; // Cari di separuh kiri.
        }
        else // Jika key lebih besar dari elemen tengah.
        {
            low = mid + 1; // Cari di separuh kanan.
        }
    }
    return foundUser; // Kembalikan pointer ke user yang ditemukan atau NULL.
}

// Implementasi Binary Search kustom untuk array Patient.
// Logikanya sama dengan customBinarySearchUsers, tetapi untuk tipe Patient.
static Patient *customBinarySearchPatients(const Patient *key, Patient *base, int num, int (*compare)(const void *, const void *))
{
    if (num == 0)
        return NULL;
    int low = 0;
    int high = num - 1;
    Patient *foundPatient = NULL;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = compare(key, &base[mid]);
        if (cmp == 0)
        {
            foundPatient = &base[mid];
            break;
        }
        else if (cmp < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return foundPatient;
}

// Implementasi Binary Search kustom untuk array Doctor.
// Logikanya sama dengan customBinarySearchUsers, tetapi untuk tipe Doctor.
static Doctor *customBinarySearchDoctors(const Doctor *key, Doctor *base, int num, int (*compare)(const void *, const void *))
{
    if (num == 0)
        return NULL;
    int low = 0;
    int high = num - 1;
    Doctor *foundDoctor = NULL;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        int cmp = compare(key, &base[mid]);
        if (cmp == 0)
        {
            foundDoctor = &base[mid];
            break;
        }
        else if (cmp < 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return foundDoctor;
}

void findUser(Hospital *hospital, Session *session, const char *query, boolean byId)
{
    // Validasi input pointer.
    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    // Validasi hak akses: hanya Manajer yang boleh mencari pengguna.
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari pengguna.");
        return;
    }

    // Mencetak header untuk hasil pencarian.
    printHeader("Hasil Pencarian Pengguna");
    // Jika tidak ada pengguna terdaftar.
    if (hospital->users.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pengguna terdaftar.\n" COLOR_RESET);
        return;
    }

    // Variabel-variabel untuk menampilkan hasil dalam format tabel.
    boolean overallFound = false;                                   // Flag apakah ada hasil yang ditemukan secara keseluruhan.
    boolean printedTableHeaders = false;                            // Flag apakah header tabel sudah dicetak.
    int widths[] = {5, 20, 10, 20};                                 // Lebar kolom: ID, Username, Role, Penyakit.
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"}; // Nama header kolom.
    char idStr[12];                                                 // Buffer untuk ID pengguna sebagai string.
    char roleStr[20];                                               // Buffer untuk peran pengguna sebagai string.
    char diseaseStr[50];                                            // Buffer untuk penyakit (jika pengguna adalah pasien).

    // Logika jika pencarian berdasarkan ID (byId == true).
    if (byId)
    {
        // Mengkonversi string query (yang seharusnya ID) menjadi integer.
        // stringToInt adalah fungsi kustom yang diasumsikan ada.
        int targetId = stringToInt(query);
        // Validasi hasil konversi string ke integer.
        // Jika stringToInt mengembalikan -1 untuk input non-numerik (kecuali "-1" atau "0" itu sendiri).
        if (targetId == -1 && strcmp(query, "-1") != 0 && strcmp(query, "0") != 0)
        {
            printError("ID pencarian tidak valid (harus berupa angka).");
            printf(COLOR_YELLOW "Pengguna dengan ID '%s' tidak ditemukan.\n" COLOR_RESET, query);
            return;
        }

        User keyUser;          // Membuat User sementara untuk kunci pencarian binary search.
        keyUser.id = targetId; // Set ID kunci.
        // Mengurutkan array pengguna berdasarkan ID sebelum melakukan binary search.
        // Ini penting karena binary search memerlukan array yang terurut.
        // Perhatian: Ini mengurutkan array asli hospital->users.elements. Jika tidak diinginkan, salin dulu ke array sementara.
        qsort(hospital->users.elements, hospital->users.nEff, sizeof(User), compareUsersById);
        // Melakukan binary search.
        User *foundUser = customBinarySearchUsers(&keyUser, hospital->users.elements, hospital->users.nEff, compareUsersById);

        // Jika pengguna dengan ID tersebut ditemukan.
        if (foundUser != NULL)
        {
            // Jika header tabel belum dicetak, cetak sekarang.
            if (!printedTableHeaders)
            {
                printTableBorder(widths, 4, 1);
                printTableRow(headers, widths, 4);
                printTableBorder(widths, 4, 2);
                printedTableHeaders = true;
            }
            overallFound = true; // Set flag bahwa hasil ditemukan.
            // Konversi ID pengguna yang ditemukan ke string.
            if (!integerToString(foundUser->id, idStr, sizeof(idStr)))
            {
                return printError("Gagal konversi ID ke string!"); // Keluar jika konversi gagal.
            }
            // Tentukan string peran.
            switch (foundUser->role)
            {
            case MANAGER:
                strcpy(roleStr, "Manajer");
                break;
            case DOCTOR:
                strcpy(roleStr, "Dokter");
                break;
            case PATIENT:
                strcpy(roleStr, "Pasien");
                break;
            default:
                strcpy(roleStr, "N/A");
                break; // Peran tidak diketahui.
            }
            strcpy(diseaseStr, "-"); // Default penyakit adalah "-".
            // Jika pengguna adalah pasien, cari info penyakitnya.
            if (foundUser->role == PATIENT)
            {
                for (int j = 0; j < hospital->patients.nEff; j++)
                {
                    if (hospital->patients.elements[j].id == foundUser->id && hospital->patients.elements[j].disease[0] != '\0')
                    {
                        strcpy(diseaseStr, hospital->patients.elements[j].disease);
                        break;
                    }
                }
            }
            // Data baris untuk tabel.
            const char *row[] = {idStr, foundUser->username, roleStr, diseaseStr};
            printTableRow(row, widths, 4); // Cetak baris.
        }
    }
    // Logika jika pencarian berdasarkan Nama (byId == false).
    else
    {
        boolean exactMatchFound = false; // Flag untuk pencocokan nama persis.
        // Pass 1: Cari pencocokan nama persis (case-insensitive).
        for (int i = 0; i < hospital->users.nEff; i++)
        {
            User *user = &hospital->users.elements[i];
            // Perbandingan nama case-insensitive.
            if (localCaseInsensitiveStrcmp(user->username, query) == 0)
            {
                if (!printedTableHeaders) // Cetak header jika belum.
                {
                    printTableBorder(widths, 4, 1);
                    printTableRow(headers, widths, 4);
                    printTableBorder(widths, 4, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                exactMatchFound = true; // Pencocokan persis ditemukan.
                // Proses dan cetak detail pengguna (sama seperti pencarian byId).
                if (!integerToString(user->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                switch (user->role)
                {
                case MANAGER:
                    strcpy(roleStr, "Manajer");
                    break;
                case DOCTOR:
                    strcpy(roleStr, "Dokter");
                    break;
                case PATIENT:
                    strcpy(roleStr, "Pasien");
                    break;
                default:
                    strcpy(roleStr, "N/A");
                    break;
                }
                strcpy(diseaseStr, "-");
                if (user->role == PATIENT)
                {
                    for (int j = 0; j < hospital->patients.nEff; j++)
                    {
                        if (hospital->patients.elements[j].id == user->id && hospital->patients.elements[j].disease[0] != '\0')
                        {
                            strcpy(diseaseStr, hospital->patients.elements[j].disease);
                            break;
                        }
                    }
                }
                const char *row[] = {idStr, user->username, roleStr, diseaseStr};
                printTableRow(row, widths, 4);
            }
        }

        // Pass 2: Jika tidak ada pencocokan nama persis dan query tidak kosong, cari saran (substring).
        if (!exactMatchFound && query[0] != '\0')
        {
            boolean suggestionsFound = false; // Flag apakah ada saran yang ditemukan.
            for (int i = 0; i < hospital->users.nEff; i++)
            {
                User *user = &hospital->users.elements[i];
                // Cek apakah username mengandung query (case-insensitive).
                if (localContainsCaseInsensitiveSubstring(user->username, query))
                {
                    // Jika ini saran pertama, cetak pesan "Mungkin maksud Anda:".
                    if (!suggestionsFound)
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders) // Cetak header jika belum.
                    {
                        printTableBorder(widths, 4, 1);
                        printTableRow(headers, widths, 4);
                        printTableBorder(widths, 4, 2);
                        printedTableHeaders = true;
                    }
                    overallFound = true;
                    // Proses dan cetak detail pengguna (sama seperti pencarian byId).
                    if (!integerToString(user->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
                    switch (user->role)
                    {
                    case MANAGER:
                        strcpy(roleStr, "Manajer");
                        break;
                    case DOCTOR:
                        strcpy(roleStr, "Dokter");
                        break;
                    case PATIENT:
                        strcpy(roleStr, "Pasien");
                        break;
                    default:
                        strcpy(roleStr, "N/A");
                        break;
                    }
                    strcpy(diseaseStr, "-");
                    if (user->role == PATIENT)
                    {
                        for (int j = 0; j < hospital->patients.nEff; j++)
                        {
                            if (hospital->patients.elements[j].id == user->id && hospital->patients.elements[j].disease[0] != '\0')
                            {
                                strcpy(diseaseStr, hospital->patients.elements[j].disease);
                                break;
                            }
                        }
                    }
                    const char *row[] = {idStr, user->username, roleStr, diseaseStr};
                    printTableRow(row, widths, 4);
                }
            }
        }
    }

    // Setelah semua pencarian, jika ada hasil yang dicetak (header sudah dicetak).
    if (overallFound) // Atau bisa juga `if (printedTableHeaders)`
    {
        printTableBorder(widths, 4, 3); // Cetak border bawah tabel.
    }
    else // Jika tidak ada hasil sama sekali.
    {
        // Jika header sempat tercetak (misalnya, karena byId gagal tapi validasi ID lolos),
        // namun binary search tidak menemukan, cetak baris "Tidak Ditemukan".
        if (printedTableHeaders)
        {
            // COLOR_GREY diasumsikan makro untuk warna abu-abu.
            const char *noResults[] = {COLOR_GREY "0." COLOR_RESET, COLOR_GREY "Tidak Ditemukan" COLOR_RESET, COLOR_GREY "-" COLOR_RESET, COLOR_GREY "-" COLOR_RESET};
            printTableRow(noResults, widths, 4);
            printTableBorder(widths, 4, 3);
        }
        // Cetak pesan bahwa pengguna tidak ditemukan.
        printf(COLOR_YELLOW "Pengguna dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}

void findPatient(Hospital *hospital, Session *session, const char *query, boolean byId, boolean byDisease)
{
    // Validasi input dan hak akses (hanya Manajer).
    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari pasien.");
        return;
    }

    printHeader("Hasil Pencarian Pasien");
    if (hospital->patients.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pasien terdaftar.\n" COLOR_RESET);
        return;
    }

    // Variabel untuk tabel dan status pencarian.
    boolean overallFound = false;
    boolean printedTableHeaders = false;
    int widths[] = {5, 20, 50}; // Lebar kolom: ID, Username, Penyakit.
    const char *headers[] = {"ID", "Username", "Penyakit"};
    char idStr[12];
    char diseaseStr[51]; // Buffer untuk penyakit, pastikan cukup panjang.

    // Logika jika pencarian berdasarkan Penyakit.
    if (byDisease)
    {
        boolean exactMatchFound = false;
        // Pass 1: Cari pencocokan penyakit persis.
        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            Patient *patient = &hospital->patients.elements[i];
            if (localCaseInsensitiveStrcmp(patient->disease, query) == 0)
            {
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                exactMatchFound = true;
                if (!integerToString(patient->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                // Menggunakan ternary operator untuk memastikan diseaseStr diisi "-" jika penyakit kosong.
                strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
        // Pass 2: Jika tidak ada pencocokan persis, cari saran (substring penyakit).
        if (!exactMatchFound && query[0] != '\0')
        {
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->patients.nEff; i++)
            {
                Patient *patient = &hospital->patients.elements[i];
                if (localContainsCaseInsensitiveSubstring(patient->disease, query))
                {
                    if (!suggestionsFound)
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk penyakit. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders)
                    { /* Cetak header tabel */
                        printTableBorder(widths, 3, 1);
                        printTableRow(headers, widths, 3);
                        printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }
                    overallFound = true;
                    if (!integerToString(patient->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
                    strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                    const char *row[] = {idStr, patient->username, diseaseStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    }
    // Logika jika pencarian berdasarkan ID Pasien.
    else if (byId)
    {
        int targetId = stringToInt(query);
        if (targetId == -1 && strcmp(query, "-1") != 0 && strcmp(query, "0") != 0) // Validasi ID.
        {
            printError("ID pencarian tidak valid.");
            // Pesan "tidak ditemukan" akan ditangani di akhir.
        }
        else
        {
            Patient keyPatient;
            keyPatient.id = targetId;
            // Urutkan pasien berdasarkan ID sebelum binary search.
            // Perhatian: Ini mengurutkan array asli hospital->patients.elements.
            qsort(hospital->patients.elements, hospital->patients.nEff, sizeof(Patient), comparePatientsById);
            Patient *foundPatient = customBinarySearchPatients(&keyPatient, hospital->patients.elements, hospital->patients.nEff, comparePatientsById);
            if (foundPatient != NULL)
            {
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                if (!integerToString(foundPatient->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                strcpy(diseaseStr, foundPatient->disease[0] != '\0' ? foundPatient->disease : "-");
                const char *row[] = {idStr, foundPatient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
    }
    // Logika jika pencarian berdasarkan Nama Pasien.
    else // byName (karena byDisease dan byId sudah ditangani)
    {
        boolean exactMatchFound = false;
        // Pass 1: Cari pencocokan nama persis.
        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            Patient *patient = &hospital->patients.elements[i];
            if (localCaseInsensitiveStrcmp(patient->username, query) == 0)
            {
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true;
                exactMatchFound = true;
                if (!integerToString(patient->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
        // Pass 2: Jika tidak ada pencocokan persis, cari saran (substring nama).
        if (!exactMatchFound && query[0] != '\0')
        {
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->patients.nEff; i++)
            {
                Patient *patient = &hospital->patients.elements[i];
                if (localContainsCaseInsensitiveSubstring(patient->username, query))
                {
                    if (!suggestionsFound)
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk nama. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders)
                    { /* Cetak header tabel */
                        printTableBorder(widths, 3, 1);
                        printTableRow(headers, widths, 3);
                        printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }
                    overallFound = true;
                    if (!integerToString(patient->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
                    strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                    const char *row[] = {idStr, patient->username, diseaseStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    }

    // Mencetak border bawah tabel jika ada hasil.
    if (overallFound)
    {
        printTableBorder(widths, 3, 3);
    }
    // Jika tidak ada hasil sama sekali.
    else
    {
        // Jika header sempat tercetak tapi tidak ada data (misal ID valid tapi tidak ditemukan).
        if (printedTableHeaders)
        {
            const char *noResults[] = {COLOR_GREY "0." COLOR_RESET, COLOR_GREY "Tidak Ditemukan" COLOR_RESET, COLOR_GREY "-" COLOR_RESET};
            printTableRow(noResults, widths, 3);
            printTableBorder(widths, 3, 3);
        }
        // Pesan "tidak ditemukan" yang lebih spesifik.
        char searchTypeStr[20]; // String untuk tipe pencarian (ID, nama, penyakit).
        if (byDisease)
            strcpy(searchTypeStr, "penyakit");
        else if (byId)
            strcpy(searchTypeStr, "ID");
        else
            strcpy(searchTypeStr, "nama");

        char notFoundMsg[100]; // Buffer untuk pesan "tidak ditemukan".
        strcpy(notFoundMsg, "Pasien dengan ");
        strcat(notFoundMsg, searchTypeStr);
        strcat(notFoundMsg, " '");
        strcat(notFoundMsg, query); // Kueri pencarian.
        strcat(notFoundMsg, "' tidak ditemukan.");
        printf(COLOR_YELLOW "%s\n" COLOR_RESET, notFoundMsg);
    }
}

void findDoctor(Hospital *hospital, Session *session, const char *query, boolean byId)
{
    // Validasi input dan hak akses (hanya Manajer).
    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari dokter.");
        return;
    }

    printHeader("Hasil Pencarian Dokter");
    if (hospital->doctors.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada dokter terdaftar.\n" COLOR_RESET);
        return;
    }

    // Variabel untuk tabel dan status pencarian.
    boolean found = false;               // Flag umum apakah ada dokter yang ditemukan (baik exact maupun suggestion).
    boolean printedTableHeaders = false; // Flag untuk menandai apakah header tabel sudah dicetak.
    int widths[] = {5, 20, 10};          // Lebar kolom: ID, Username, Aura.
    const char *headers[] = {"ID", "Username", "Aura"};
    char idStr[12];   // Buffer untuk ID dokter sebagai string.
    char auraStr[16]; // Buffer untuk Aura dokter sebagai string.

    // Logika jika pencarian berdasarkan ID Dokter.
    if (byId)
    {
        int targetId = stringToInt(query); // Konversi query ID ke integer.
        // Validasi ID yang lebih konsisten dengan findUser:
        // stringToInt mengembalikan -1 jika gagal, tapi "0" dan "-1" adalah ID valid (mungkin).
        if (targetId == -1 && strcmp(query, "0") != 0 && strcmp(query, "-1") != 0)
        {
            printError("ID pencarian tidak valid (harus berupa angka).");
            // Pesan "tidak ditemukan" akan dicetak di akhir jika 'found' tetap false.
        }
        else // ID dianggap valid (atau 0 / -1 yang mungkin valid).
        {
            Doctor keyDoctor;
            keyDoctor.id = targetId; // Kunci untuk binary search.
            // Urutkan dokter berdasarkan ID sebelum melakukan binary search.
            // Perhatian: Ini mengurutkan array asli hospital->doctors.elements.
            qsort(hospital->doctors.elements, hospital->doctors.nEff, sizeof(Doctor), compareDoctorsById);
            Doctor *foundDoctor = customBinarySearchDoctors(&keyDoctor, hospital->doctors.elements, hospital->doctors.nEff, compareDoctorsById);

            if (foundDoctor != NULL) // Jika dokter dengan ID tersebut ditemukan.
            {
                found = true;             // Set flag bahwa hasil ditemukan.
                if (!printedTableHeaders) // Cetak header tabel jika belum.
                {
                    printTableBorder(widths, 3, 1);    // Cetak batas atas tabel.
                    printTableRow(headers, widths, 3); // Cetak header kolom.
                    printTableBorder(widths, 3, 2);    // Cetak pemisah header dan data.
                    printedTableHeaders = true;
                }

                // Konversi ID dokter ke string.
                if (!integerToString(foundDoctor->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }

                // Konversi Aura dokter ke string.
                if (!integerToString(foundDoctor->aura, auraStr, sizeof(auraStr)))
                {
                    return printError("Gagal konversi Aura ke string!");
                }

                const char *row[] = {idStr, foundDoctor->username, auraStr};
                printTableRow(row, widths, 3); // Cetak baris data dokter.
            }
        }
    }
    // Logika jika pencarian berdasarkan Nama Dokter.
    else
    {
        boolean exactMatchFound = false;  // Flag untuk pencocokan nama persis.
        boolean suggestionsFound = false; // Flag untuk mencetak "Mungkin maksud Anda" hanya sekali.

        // Pass 1: Cari pencocokan nama persis (case-insensitive).
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            Doctor *doctor = &hospital->doctors.elements[i];
            if (localCaseInsensitiveStrcmp(doctor->username, query) == 0)
            {
                found = true;           // Hasil ditemukan.
                exactMatchFound = true; // Pencocokan persis.
                if (!printedTableHeaders)
                { /* Cetak header tabel */
                    printTableBorder(widths, 3, 1);
                    printTableRow(headers, widths, 3);
                    printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                // Konversi dan cetak detail dokter.
                if (!integerToString(doctor->id, idStr, sizeof(idStr)))
                {
                    return printError("Gagal konversi ID ke string!");
                }
                if (!integerToString(doctor->aura, auraStr, sizeof(auraStr)))
                {
                    return printError("Gagal konversi Aura ke string!");
                }
                const char *row[] = {idStr, doctor->username, auraStr};
                printTableRow(row, widths, 3);
            }
        }

        // Pass 2: Jika tidak ada pencocokan nama persis dan query tidak kosong, cari saran (substring).
        // `query[0] != '\0'` untuk menghindari pencocokan semua pada query kosong.
        if (!exactMatchFound && query[0] != '\0')
        {
            for (int i = 0; i < hospital->doctors.nEff; i++)
            {
                Doctor *doctor = &hospital->doctors.elements[i];
                // Cek apakah username mengandung query dan BUKAN merupakan exact match yang sudah diproses.
                if (localContainsCaseInsensitiveSubstring(doctor->username, query) &&
                    localCaseInsensitiveStrcmp(doctor->username, query) != 0) // Hindari menampilkan exact match lagi sebagai suggestion.
                {
                    found = true;          // Sebuah kecocokan (saran) ditemukan.
                    if (!suggestionsFound) // Jika ini saran pertama.
                    {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk nama. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    if (!printedTableHeaders)
                    { /* Cetak header tabel */
                        printTableBorder(widths, 3, 1);
                        printTableRow(headers, widths, 3);
                        printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }
                    // Konversi dan cetak detail dokter.
                    if (!integerToString(doctor->id, idStr, sizeof(idStr)))
                    {
                        return printError("Gagal konversi ID ke string!");
                    }
                    if (!integerToString(doctor->aura, auraStr, sizeof(auraStr)))
                    {
                        return printError("Gagal konversi Aura ke string!");
                    }
                    const char *row[] = {idStr, doctor->username, auraStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    }

    // Mencetak border bawah tabel jika header sudah pernah dicetak (artinya ada data yang ditampilkan atau akan ditampilkan).
    if (printedTableHeaders)
    {
        printTableBorder(widths, 3, 3);
    }

    // Jika tidak ada dokter yang ditemukan setelah semua proses pencarian (baik exact maupun suggestion).
    if (!found)
    {
        // Mencetak pesan bahwa dokter tidak ditemukan.
        printf(COLOR_YELLOW "Dokter dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}