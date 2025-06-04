# Berkas: `src/c/searchUser.c`

Berkas ini menyediakan fungsionalitas untuk mencari pengguna (umum), pasien, dan dokter dalam sistem rumah sakit. Pencarian dapat dilakukan berdasarkan ID (kecocokan pasti menggunakan pencarian biner setelah pengurutan) atau berdasarkan nama/penyakit/spesialisasi (kecocokan pasti dan substring tanpa membedakan huruf besar/kecil). Fungsi pencarian ini biasanya terbatas untuk Manajer.

## Fungsi Pembantu: `localContainsCaseInsensitiveSubstring`

### Tanda Tangan (Signature)
```c
static boolean localContainsCaseInsensitiveSubstring(const char *text, const char *pattern)
```

### Ringkasan
Fungsi pembantu statis lokal untuk memeriksa apakah string `pattern` adalah substring dari `text`, dengan mengabaikan huruf besar/kecil.

### Parameter
*   `const char *text`: Teks yang akan dicari di dalamnya.
*   `const char *pattern`: Pola yang akan dicari.

### Algoritma
1.  Menangani input `NULL`: mengembalikan `false` jika `text` atau `pattern` adalah `NULL`.
2.  Menghitung panjang `text` dan `pattern`.
3.  Jika `patternLen` adalah 0, mengembalikan `true`.
4.  Jika `patternLen > textLen`, mengembalikan `false`.
5.  Melakukan iterasi melalui `text` dari `i = 0` hingga `textLen - patternLen`.
    *   Untuk setiap posisi `i`, mengasumsikan `match = true`.
    *   Membandingkan karakter `text[i+j]` dan `pattern[j]` (dikonversi ke huruf kecil menggunakan `charToLower`) untuk `j` dari `0` hingga `patternLen - 1`.
    *   Jika ada pasangan karakter yang tidak cocok, atur `match = false` dan hentikan loop dalam.
    *   Jika `match` masih `true` setelah loop dalam, mengembalikan `true`.
6.  Jika tidak ada kecocokan ditemukan setelah memeriksa semua posisi, mengembalikan `false`.

## Fungsi Pembanding Pembantu (untuk `qsort` dan pencarian biner)

Fungsi statis ini digunakan untuk mengurutkan dan melakukan pencarian biner pada berbagai jenis pengguna berdasarkan ID mereka.

### `static int compareUsersById(const void *a, const void *b)`
Membandingkan dua struct `User` berdasarkan `id` mereka.
### `static int comparePatientsById(const void *a, const void *b)`
Membandingkan dua struct `Patient` berdasarkan `id` mereka.
### `static int compareDoctorsById(const void *a, const void *b)`
Membandingkan dua struct `Doctor` berdasarkan `id` mereka.

## Fungsi Pembantu Pencarian Biner Kustom

Fungsi statis ini melakukan pencarian biner pada array struct yang sudah diurutkan.

### `static User *customBinarySearchUsers(...)`
### `static Patient *customBinarySearchPatients(...)`
### `static Doctor *customBinarySearchDoctors(...)`

### Ringkasan (untuk semua fungsi pencarian biner kustom)
Melakukan pencarian biner untuk `key` yang diberikan (misalnya, `User` dengan ID target) dalam array `base` sejumlah `num` elemen, menggunakan fungsi `compare` yang disediakan.

### Parameter (generik)
*   `const void *key`: Pointer ke elemen kunci yang akan dicari.
*   `void *base`: Pointer ke awal array yang sudah diurutkan.
*   `int num`: Jumlah elemen dalam array.
*   `int (*compare)(const void *, const void *)`: Fungsi pembanding.

### Algoritma (generik)
1.  Jika `num == 0`, mengembalikan `NULL`.
2.  Menginisialisasi `low = 0`, `high = num - 1`, `foundUser/Patient/Doctor = NULL`.
3.  `while (low <= high)`:
    *   `mid = low + (high - low) / 2;`
    *   `cmp = compare(key, &base[mid]);`
    *   Jika `cmp == 0` (kecocokan ditemukan), mengatur `found... = &base[mid];` dan hentikan.
    *   Jika `cmp < 0`, `high = mid - 1;`.
    *   Lainnya, `low = mid + 1;`.
4.  Mengembalikan `found...` (yang mana `NULL` jika tidak ditemukan).

## Fungsi Pencarian Utama

## Fungsi: `findUser`

### Tanda Tangan (Signature)
```c
void findUser(Hospital *hospital, Session *session, const char *query, boolean byId)
```

### Ringkasan
Mencari pengguna (peran apa pun) berdasarkan ID atau nama pengguna. Terbatas untuk Manajer. Hasil dicetak dalam format tabel. Untuk pencarian nama pengguna, ia melakukan pencocokan persis tanpa membedakan huruf besar/kecil terlebih dahulu, kemudian menyarankan kecocokan parsial jika tidak ada kecocokan persis yang ditemukan.

### Parameter
*   `Hospital *hospital`: Pointer ke data rumah sakit.
*   `Session *session`: Sesi pengguna saat ini untuk otorisasi.
*   `const char *query`: Istilah pencarian (ID atau nama pengguna).
*   `boolean byId`: Jika `true`, cari berdasarkan ID; jika tidak, cari berdasarkan nama pengguna.

### Algoritma
1.  **Validasi & Otorisasi**: Memeriksa input `NULL` dan apakah `session->role == MANAGER`. Mencetak kesalahan dan kembali jika pemeriksaan gagal.
2.  **Header & Pemeriksaan Awal**: Mencetak "Hasil Pencarian Pengguna". Jika tidak ada pengguna (`hospital->users.nEff == 0`), mencetak pesan dan kembali.
3.  **Pencarian berdasarkan ID (`byId == true`)**:
    *   Mengonversi `query` menjadi `targetId` menggunakan `stringToInt`. Memvalidasi konversi.
    *   Membuat `User keyUser` dengan `keyUser.id = targetId`.
    *   Mengurutkan `hospital->users.elements` menggunakan `qsort` dengan `compareUsersById`.
    *   Melakukan pencarian biner menggunakan `customBinarySearchUsers`.
    *   Jika `foundUser` bukan `NULL`:
        *   Mencetak header tabel jika belum dicetak.
        *   Memformat ID pengguna, peran (Manajer, Dokter, Pasien), dan penyakit (jika pasien, "-" jika tidak) menjadi string.
        *   Mencetak detail pengguna sebagai baris tabel.
4.  **Pencarian berdasarkan Nama Pengguna (`byId == false`)**:
    *   **Langkah Pencocokan Persis**: Melakukan iterasi melalui `hospital->users.elements`.
        *   Jika `caseInsensitiveStrcmp(user->username, query) == 0`:
            *   Mencetak header tabel jika perlu. Menandai `exactMatchFound = true`.
            *   Memformat dan mencetak detail pengguna.
    *   **Langkah Saran Substring**: Jika `!exactMatchFound` dan `query` tidak kosong:
        *   Melakukan iterasi melalui `hospital->users.elements`.
        *   Jika `localContainsCaseInsensitiveSubstring(user->username, query)`:
            *   Mencetak header saran ("Tidak ada hasil pencocokan pasti. Mungkin maksud Anda:") jika ini adalah saran pertama.
            *   Mencetak header tabel jika perlu.
            *   Memformat dan mencetak detail pengguna.
5.  **Output Hasil**:
    *   Jika ada pengguna yang ditemukan (`overallFound == true`), mencetak batas bawah tabel.
    *   Lainnya (tidak ada pengguna ditemukan), mencetak pesan "Pengguna ... tidak ditemukan". Jika header dicetak, ia menambahkan baris "Tidak Ditemukan" sebelum pesan.

## Fungsi: `findPatient`

### Tanda Tangan (Signature)
```c
void findPatient(Hospital *hospital, Session *session, const char *query, boolean byId, boolean byDisease)
```

### Ringkasan
Mencari pasien berdasarkan ID, nama pengguna, atau penyakit. Terbatas untuk Manajer. Hasil mencakup ID, Nama Pengguna, dan Penyakit. Menangani kecocokan persis dan substring mirip dengan `findUser`.

### Parameter
*   `Hospital *hospital`, `Session *session`, `const char *query`: Mirip dengan `findUser`.
*   `boolean byId`: Jika `true`, cari berdasarkan ID.
*   `boolean byDisease`: Jika `true` (dan `byId` adalah `false`), cari berdasarkan penyakit; jika tidak (jika `byId` adalah `false` dan `byDisease` adalah `false`), cari berdasarkan nama pengguna.

### Algoritma
1.  **Validasi & Otorisasi**: Mirip dengan `findUser`.
2.  **Header & Pemeriksaan Awal**: Mencetak "Hasil Pencarian Pasien". Jika tidak ada pasien, mencetak pesan dan kembali.
3.  **Pencarian berdasarkan Penyakit (`byDisease == true`)**:
    *   Melakukan langkah pencocokan persis tanpa membedakan huruf besar/kecil untuk `patient->disease`.
    *   Jika tidak ada kecocokan persis, melakukan langkah pencocokan substring untuk `patient->disease`.
    *   Mencetak hasil dalam tabel (ID, Nama Pengguna, Penyakit).
4.  **Pencarian berdasarkan ID (`byId == true`)**:
    *   Mengonversi `query` menjadi `targetId`.
    *   Mengurutkan `hospital->patients.elements` menggunakan `qsort` dengan `comparePatientsById`.
    *   Menggunakan `customBinarySearchPatients`.
    *   Mencetak hasil jika ditemukan.
5.  **Pencarian berdasarkan Nama Pengguna (`byId == false && byDisease == false`)**:
    *   Melakukan langkah pencocokan persis tanpa membedakan huruf besar/kecil untuk `patient->username`.
    *   Jika tidak ada kecocokan persis, melakukan langkah pencocokan substring untuk `patient->username`.
    *   Mencetak hasil.
6.  **Output Hasil**: Mirip dengan `findUser`, mencetak batas tabel atau pesan "Pasien ... tidak ditemukan".

## Fungsi: `findDoctor`

### Tanda Tangan (Signature)
```c
void findDoctor(Hospital *hospital, Session *session, const char *query, boolean byId)
```

### Ringkasan
Mencari dokter berdasarkan ID atau nama pengguna. Terbatas untuk Manajer. Hasil mencakup ID, Nama Pengguna, dan Aura.

### Parameter
*   `Hospital *hospital`, `Session *session`, `const char *query`, `boolean byId`: Mirip dengan `findUser`.

### Algoritma
1.  **Validasi & Otorisasi**: Mirip dengan `findUser`.
2.  **Header & Pemeriksaan Awal**: Mencetak "Hasil Pencarian Dokter". Jika tidak ada dokter, mencetak pesan dan kembali.
3.  **Pencarian berdasarkan ID (`byId == true`)**:
    *   Mengonversi `query` menjadi `targetId`.
    *   Mengurutkan `hospital->doctors.elements` menggunakan `qsort` dengan `compareDoctorsById`.
    *   Menggunakan `customBinarySearchDoctors`.
    *   Mencetak hasil (ID, Nama Pengguna, Aura) jika ditemukan.
4.  **Pencarian berdasarkan Nama Pengguna (`byId == false`)**:
    *   Melakukan langkah pencocokan persis tanpa membedakan huruf besar/kecil untuk `doctor->username`.
    *   Jika tidak ada kecocokan persis, melakukan langkah pencocokan substring untuk `doctor->username` (tidak termasuk kecocokan persis itu sendiri dari saran).
    *   Mencetak hasil.
5.  **Output Hasil**: Mirip dengan `findUser`, mencetak batas tabel atau pesan "Dokter ... tidak ditemukan".
