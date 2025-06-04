# Berkas: `src/c/bananarich.c`

Berkas ini menangani fungsionalitas terkait "BananaRich", mata uang atau sistem poin dalam game. Ini termasuk melihat dompet pengguna, melihat keuangan rumah sakit, dan sistem "gacha" bagi pasien untuk memenangkan BananaRich.

## Fungsi: `viewWallet`

### Tanda Tangan (Signature)
```c
void viewWallet(Hospital *hospital, Session *session)
```

### Ringkasan
Menampilkan saldo BananaRich untuk pengguna yang saat ini login, yang harus merupakan `DOCTOR` atau `PATIENT`. Fungsi ini mengambil saldo dari daftar yang sesuai (dokter atau pasien) berdasarkan peran pengguna dan menampilkannya dalam tabel yang diformat.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`, digunakan untuk mengakses daftar pasien dan dokter.
*   `Session *session`: Pointer ke struktur `Session`, berisi detail pengguna yang saat ini login (nama pengguna, peran, status login).

### Algoritma
1.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi bahwa pengguna telah login (`session->isLoggedIn`).
    *   Memeriksa apakah peran pengguna adalah `DOCTOR` atau `PATIENT` (`session->role != DOCTOR && session->role != PATIENT`).
    *   Jika otorisasi gagal, mencetak pesan kesalahan ("Akses ditolak! Hanya Dokter dan Pasien yang dapat melihat dompet.") dan kembali.
2.  **Cetak Header**: Mencetak "Saldo Dompet" sebagai header output.
3.  **Inisialisasi Variabel**:
    *   `float balance = 0.0;`
    *   `char *userType` diatur ke "Pasien" jika `session->role == PATIENT`, jika tidak maka "Dokter".
4.  **Ambil Saldo**:
    *   **Jika pengguna adalah PATIENT**:
        *   Melakukan iterasi melalui `hospital->patients.elements`.
        *   Jika nama pengguna pasien cocok dengan `session->username`, `balance` diatur ke `hospital->patients.elements[i].bananaRich`, dan loop berhenti.
    *   **Jika pengguna adalah DOCTOR**:
        *   Melakukan iterasi melalui `hospital->doctors.elements`.
        *   Jika nama pengguna dokter cocok dengan `session->username`, `balance` diatur ke `hospital->doctors.elements[i].bananaRich`, dan loop berhenti.
5.  **Format String Saldo**:
    *   Mengonversi `balance` (float) ke string `balanceStr` menggunakan `floatToString()`, diformat menjadi 2 angka desimal.
    *   Jika `floatToString()` gagal, mencetak pesan kesalahan ("Gagal mengonversi float ke string!") dan kembali.
    *   Menambahkan " BananaRich" ke `balanceStr`.
6.  **Tampilkan Tabel**:
    *   Mendefinisikan lebar kolom: `int widths[] = {15, 20};`.
    *   Mendefinisikan header tabel: `const char *headers[] = {userType, "Saldo"};`.
    *   Mencetak struktur tabel (border, baris header, baris data) menggunakan `printTableBorder()` dan `printTableRow()`.
    *   Baris data terdiri dari `session->username` dan `balanceStr` yang telah diformat.

---

## Fungsi: `viewFinancial`

### Tanda Tangan (Signature)
```c
void viewFinancial(Hospital *hospital, Session *session)
```

### Ringkasan
Menampilkan saldo keuangan keseluruhan rumah sakit (total BananaRich). Fungsi ini terbatas untuk pengguna yang login dengan peran `MANAGER`.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`, digunakan untuk mengakses informasi keuangan rumah sakit.
*   `Session *session`: Pointer ke struktur `Session`, digunakan untuk memverifikasi peran dan status login pengguna saat ini.

### Algoritma
1.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi bahwa pengguna telah login (`session->isLoggedIn`).
    *   Memeriksa apakah peran pengguna adalah `MANAGER` (`session->role != MANAGER`).
    *   Jika otorisasi gagal, mencetak pesan kesalahan ("Akses ditolak! Hanya Manajer yang dapat melihat finansial.") dan kembali.
2.  **Cetak Header**: Mencetak "Finansial Rumah Sakit" sebagai header.
3.  **Ambil Saldo Rumah Sakit**:
    *   `float balance = hospital->finance.hospitalBalance;`.
4.  **Format String Saldo**:
    *   Mengonversi `balance` ke `balanceStr` menggunakan `floatToString()`, diformat menjadi 2 angka desimal.
    *   Jika `floatToString()` gagal, mencetak pesan kesalahan ("Gagal mengonversi float ke string!") dan kembali.
    *   Menambahkan " BananaRich" ke `balanceStr`.
5.  **Tampilkan Tabel**:
    *   Mendefinisikan lebar kolom: `int widths[] = {15, 20};`.
    *   Mendefinisikan header tabel: `const char *headers[] = {"Entitas", "Saldo"};`.
    *   Mencetak struktur tabel.
    *   Baris data terdiri dari "Rumah Sakit" dan `balanceStr` yang telah diformat.

---

## Fungsi: `hashString`

### Tanda Tangan (Signature)
```c
unsigned long hashString(const char *str)
```

### Ringkasan
Fungsi pembantu (kemungkinan statis atau lokal untuk berkas ini) yang menghitung nilai hash untuk string yang diberikan menggunakan algoritma djb2. Ini digunakan sebagai bagian dari pembuatan seed untuk sistem gacha.

### Parameter
*   `const char *str`: String input yang akan di-hash.

### Algoritma
1.  Menginisialisasi `unsigned long hash = 5381;`.
2.  Melakukan iterasi melalui setiap karakter `c` dari string input `str` hingga mencapai terminator null.
3.  Dalam setiap iterasi, memperbarui hash: `hash = ((hash << 5) + hash) + c;` (hash * 33 + c).
4.  Mengembalikan nilai `hash` akhir.

### Nilai Kembali
*   `unsigned long`: Hash yang dihitung dari string input.

---

## Fungsi: `generateSeed`

### Tanda Tangan (Signature)
```c
unsigned long generateSeed(Patient *patient)
```

### Ringkasan
Fungsi pembantu (kemungkinan statis atau lokal untuk berkas ini) yang menghasilkan seed pseudo-acak untuk sistem gacha berdasarkan data pasien dan penghitung internal. Ini bertujuan untuk memberikan variabilitas dalam hasil gacha.

### Parameter
*   `Patient *patient`: Pointer ke struktur `Patient` yang seed-nya akan dibuat.

### Algoritma
1.  Menaikkan variabel statis global `gachaCallCounter`.
2.  Menginisialisasi `unsigned long seed = 0;`.
3.  Menggabungkan beberapa sumber data ke dalam seed menggunakan operasi XOR (`^`):
    *   `hashString(patient->username)`: Hash dari nama pengguna pasien.
    *   `(unsigned long)(patient->bananaRich * 100)`: Saldo BananaRich pasien (diskala).
    *   `(unsigned long)gachaCallCounter * 1664525UL`: Penghitung panggilan gacha dikalikan dengan konstanta.
    *   `(unsigned long)&patient`: Alamat memori dari struktur pasien.
    *   `(unsigned long)&seed`: Alamat memori dari variabel seed itu sendiri.
4.  Menerapkan serangkaian operasi bitwise (XOR, geser kanan, perkalian dengan konstanta) untuk lebih lanjut mencampur bit seed, teknik umum dalam pembuatan angka pseudo-acak untuk meningkatkan distribusi.
    *   `seed = (seed ^ (seed >> 16)) * 0x45d9f3bUL;`
    *   `seed = (seed ^ (seed >> 16)) * 0x45d9f3bUL;`
    *   `seed = seed ^ (seed >> 16);`
5.  Mengembalikan `seed` akhir.

### Nilai Kembali
*   `unsigned long`: Seed pseudo-acak yang dihasilkan.

---

## Fungsi: `lcgRandom`

### Tanda Tangan (Signature)
```c
unsigned long lcgRandom(unsigned long seed)
```

### Ringkasan
Fungsi pembantu (kemungkinan statis atau lokal untuk berkas ini) yang menghasilkan angka pseudo-acak menggunakan algoritma Linear Congruential Generator (LCG). Fungsi ini mengambil seed awal dan menghasilkan angka berikutnya dalam urutan.

### Parameter
*   `unsigned long seed`: Seed saat ini atau angka acak sebelumnya dalam urutan.

### Algoritma
1.  Menghitung angka acak berikutnya menggunakan rumus LCG: `(seed * 1664525UL + 1013904223UL) & 0xFFFFFFFFUL`.
    *   `1664525UL` adalah pengali.
    *   `1013904223UL` adalah penambah.
    *   `& 0xFFFFFFFFUL` memastikan hasilnya adalah integer unsigned 32-bit.
2.  Mengembalikan angka pseudo-acak yang dihasilkan.

### Nilai Kembali
*   `unsigned long`: Angka pseudo-acak berikutnya dalam urutan LCG.

---

## Fungsi: `gacha`

### Tanda Tangan (Signature)
```c
boolean gacha(Hospital *hospital, Session *session)
```

### Ringkasan
Memungkinkan `PATIENT` yang login untuk mencoba peruntungannya dalam sistem gacha untuk memenangkan BananaRich. Hasil (menang/kalah dan jumlah) ditentukan oleh pembuatan angka pseudo-acak yang di-seed oleh data spesifik pasien.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`, digunakan untuk mengakses data pasien dan memperbarui saldo BananaRich mereka.
*   `Session *session`: Pointer ke struktur `Session`, digunakan untuk mengidentifikasi pasien dan memeriksa peran mereka.

### Algoritma
1.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi bahwa pengguna telah login (`session->isLoggedIn`).
    *   Memeriksa apakah peran pengguna adalah `PATIENT` (`session->role != PATIENT`).
    *   Jika otorisasi gagal, mencetak pesan kesalahan ("Akses ditolak! Hanya Pasien yang dapat melakukan gacha.") dan mengembalikan `false`.
2.  **Cari Pasien**:
    *   Menginisialisasi `patientIndex = -1`.
    *   Melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien yang cocok dengan `session->username`.
    *   Jika ditemukan, `patientIndex` diatur.
    *   Jika tidak ditemukan (`patientIndex == -1`), mencetak pesan kesalahan ("Pasien tidak ditemukan!") dan mengembalikan `false`.
    *   Mendapatkan pointer ke pasien: `Patient *patient = &hospital->patients.elements[patientIndex];`.
3.  **Hasilkan Angka Acak**:
    *   Menghasilkan `seed` awal menggunakan `generateSeed(patient)`.
    *   Menghasilkan tiga angka acak menggunakan `lcgRandom`: `rand1 = lcgRandom(seed)`, `rand2 = lcgRandom(rand1)`, `rand3 = lcgRandom(rand2)`.
4.  **Tentukan Hadiah**:
    *   Menginisialisasi `float reward = 0.0;` dan `char rewardMessage[100] = "";`.
    *   `rewardType = rand2 % 100;` menentukan kategori umum hadiah.
    *   `itemChoice = rand3 % 3;` (atau `% 2` untuk hadiah lebih besar) menentukan hadiah spesifik dalam kategori.
    *   **Tidak Menang (40% kemungkinan: `rewardType < 40`)**:
        *   Mencetak pesan "coba lagi": "[👀 Yahhh] - Kamu kurang beruntung, coba lagi nanti!".
        *   Mengembalikan `true` (operasi gacha itu sendiri berhasil).
    *   **Hadiah Kecil (35% kemungkinan: `rewardType < 75`)**:
        *   `smallRewards[] = {2, 5, 10}; reward = smallRewards[itemChoice];`.
        *   Membuat pesan seperti "[😲 WOW] - Selamat! Anda mendapatkan [reward] BananaRich!".
    *   **Hadiah Sedang (15% kemungkinan: `rewardType < 90`)**:
        *   `mediumRewards[] = {15, 20, 25}; reward = mediumRewards[itemChoice];`.
        *   Membuat pesan seperti "[🎉 YAY] - Selamat! Anda mendapatkan [reward] BananaRich!".
    *   **Hadiah Besar (8% kemungkinan: `rewardType < 98`)**:
        *   `largeRewards[] = {50, 100}; reward = largeRewards[itemChoice % 2];`.
        *   Membuat pesan seperti "[🥳 HOREE] - Selamat! Anda mendapatkan [reward] BananaRich!".
    *   **Jackpot (2% kemungkinan: `else`)**:
        *   `reward = 200;`.
        *   Membuat pesan seperti "[🎊 JACKPOT] - Selamat! Anda mendapatkan [reward] BananaRich!".
5.  **Perbarui Saldo Pasien**:
    *   `patient->bananaRich += reward;`.
6.  **Tampilkan Hasil Gacha**:
    *   Mencetak header "Hasil Gacha".
    *   Mencetak pesan selamat datang: "Selamat datang [patient->username], di Mesin Gacha XXeon06!".
    *   Mencetak `rewardMessage`.
    *   Menghitung `beforeBalance` dan `afterBalance`.
    *   Mengonversi saldo ke string (`beforeStr`, `afterStr`) menggunakan `floatToString()`. Jika konversi gagal, mencetak pesan kesalahan ("Gagal mengonversi saldo ke string!") dan mengembalikan `false`.
    *   Mencetak saldo sebelum dan sesudah.
7.  **Nilai Kembali**: Mengembalikan `true` yang menunjukkan operasi gacha telah selesai.
