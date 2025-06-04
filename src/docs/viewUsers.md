# Berkas: `src/c/viewUsers.c`

Berkas ini menyediakan fungsionalitas untuk menampilkan daftar pengguna, pasien, dan dokter. Ini mencakup berbagai fungsi pembanding untuk mengurutkan daftar ini berdasarkan kriteria yang berbeda (ID, nama, aura) dan dalam urutan menaik atau menurun. Fungsi tampilan ini terbatas untuk Manajer.

## Fungsi Pembanding (untuk `qsort`)

Fungsi statis ini adalah fungsi pembanding standar yang dirancang untuk digunakan dengan `qsort` untuk mengurutkan array struktur `User`, `Patient`, dan `Doctor`.

*   **`compareUserByIdAsc(const void *a, const void *b)`**: Membandingkan dua `User` berdasarkan `id` untuk urutan menaik.
*   **`compareUserByIdDesc(const void *a, const void *b)`**: Membandingkan dua `User` berdasarkan `id` untuk urutan menurun.
*   **`compareUserByNameAsc(const void *a, const void *b)`**: Membandingkan dua `User` berdasarkan `username` (menggunakan `strcmp`) untuk urutan menaik.
*   **`compareUserByNameDesc(const void *a, const void *b)`**: Membandingkan dua `User` berdasarkan `username` (menggunakan `strcmp`) untuk urutan menurun.
*   **`comparePatientByIdAsc(const void *a, const void *b)`**: Membandingkan dua `Patient` berdasarkan `id` untuk urutan menaik.
*   **`comparePatientByIdDesc(const void *a, const void *b)`**: Membandingkan dua `Patient` berdasarkan `id` untuk urutan menurun.
*   **`comparePatientByNameAsc(const void *a, const void *b)`**: Membandingkan dua `Patient` berdasarkan `username` untuk urutan menaik.
*   **`comparePatientByNameDesc(const void *a, const void *b)`**: Membandingkan dua `Patient` berdasarkan `username` untuk urutan menurun.
*   **`compareDoctorByIdAsc(const void *a, const void *b)`**: Membandingkan dua `Doctor` berdasarkan `id` untuk urutan menaik.
*   **`compareDoctorByIdDesc(const void *a, const void *b)`**: Membandingkan dua `Doctor` berdasarkan `id` untuk urutan menurun.
*   **`compareDoctorByNameAsc(const void *a, const void *b)`**: Membandingkan dua `Doctor` berdasarkan `username` untuk urutan menaik.
*   **`compareDoctorByNameDesc(const void *a, const void *b)`**: Membandingkan dua `Doctor` berdasarkan `username` untuk urutan menurun.
*   **`compareDoctorByAuraAsc(const void *a, const void *b)`**: Membandingkan dua `Doctor` berdasarkan `aura` untuk urutan menaik.
*   **`compareDoctorByAuraDesc(const void *a, const void *b)`**: Membandingkan dua `Doctor` berdasarkan `aura` untuk urutan menurun.

### Parameter (Umum untuk Fungsi Pembanding)
*   `const void *a`: Pointer ke elemen pertama.
*   `const void *b`: Pointer ke elemen kedua.

### Nilai Kembali (Umum untuk Fungsi Pembanding)
*   `int`: Negatif jika `a < b`, nol jika `a == b`, positif jika `a > b`.

## Fungsi Pengurutan

Fungsi-fungsi ini bertindak sebagai pembungkus di sekitar `qsort`, menggunakan fungsi pembanding yang sesuai berdasarkan parameter `sortBy` dan `sortOrder`.

### Fungsi: `sortUser`
```c
void sortUser(UserSet *userSet, int sortBy, int sortOrder)
```
*   **Ringkasan**: Mengurutkan array `userSet->elements`.
*   `sortBy`: `1` untuk ID, selain itu berdasarkan nama.
*   `sortOrder`: `1` untuk menaik, selain itu menurun.

### Fungsi: `sortPatients`
```c
void sortPatients(PatientList *patientList, int sortBy, int sortOrder)
```
*   **Ringkasan**: Mengurutkan array `patientList->elements`.
*   `sortBy`: `1` untuk ID, selain itu berdasarkan nama.
*   `sortOrder`: `1` untuk menaik, selain itu menurun.

### Fungsi: `sortDoctors`
```c
void sortDoctors(DoctorList *doctorList, int sortBy, int sortOrder)
```
*   **Ringkasan**: Mengurutkan array `doctorList->elements`.
*   `sortBy`: `1` untuk ID, `2` untuk nama, selain itu berdasarkan aura.
*   `sortOrder`: `1` untuk menaik, selain itu menurun.

## Fungsi Tampilan

Fungsi-fungsi ini bertanggung jawab untuk menampilkan daftar pengguna, pasien, atau dokter yang sudah diurutkan dalam tabel yang diformat. Fungsi ini terbatas untuk pengguna `MANAGER`.

### Fungsi: `displayUsers`

#### Tanda Tangan (Signature)
```c
void displayUsers(Hospital *hospital, Session *session, int sortBy, int sortOrder)
```

#### Ringkasan
Menampilkan daftar semua pengguna terdaftar (Manajer, Dokter, Pasien), diurutkan sesuai dengan kriteria yang ditentukan. Untuk pasien, penyakit mereka saat ini juga ditampilkan.

#### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke `Session` pengguna saat ini untuk otorisasi.
*   `int sortBy`: Kriteria pengurutan: `1` untuk ID, jika tidak berdasarkan nama pengguna.
*   `int sortOrder`: Urutan pengurutan: `1` untuk menaik, jika tidak menurun.

#### Algoritma
1.  **Validasi & Otorisasi**:
    *   Memeriksa `hospital` dan `session` apakah `NULL`. Mencetak kesalahan jika true.
    *   Memeriksa `session->isLoggedIn` dan `session->role == MANAGER`. Mencetak kesalahan jika tidak diotorisasi.
2.  **Output Awal**: Mencetak header "Daftar Pengguna".
3.  **Pemeriksaan Kosong**: Jika `hospital->users.nEff == 0`, mencetak "Tidak ada pengguna terdaftar." dan kembali.
4.  **Buat Daftar Sementara**:
    *   Mengalokasikan memori untuk `tempList.elements` (salinan dari `hospital->users`). Mencetak kesalahan dan kembali jika alokasi gagal.
    *   Menyalin pengguna dari `hospital->users.elements` ke `tempList.elements`.
5.  **Urutkan Daftar Sementara**: Memanggil `sortUser(&tempList, sortBy, sortOrder)`.
6.  **Cetak Info Pengurutan**: Mencetak pesan yang menunjukkan kriteria pengurutan (misalnya, "Menampilkan semua pengguna dengan ID terurut ascending...").
7.  **Cetak Tabel**:
    *   Mendefinisikan `widths` tabel dan `headers`: "ID", "Username", "Role", "Penyakit".
    *   Mencetak batas tabel dan baris header.
    *   Melakukan iterasi melalui `tempList.elements`:
        *   Untuk setiap `User *user`:
            *   Mengonversi `user->id` menjadi `idStr`.
            *   Menentukan `roleStr` ("Manajer", "Dokter", "Pasien", "Tidak diketahui").
            *   Menginisialisasi `diseaseStr` menjadi "-". Jika `user->role == PATIENT`, mencari `hospital->patients` untuk ID pasien yang cocok dan menyalin `disease` mereka jika ada.
            *   Mencetak baris: `{idStr, user->username, roleStr, diseaseStr}`.
    *   Mencetak batas bawah tabel.
8.  **Pembersihan**: Membebaskan `tempList.elements`.

### Fungsi: `displayPatients`

#### Tanda Tangan (Signature)
```c
void displayPatients(Hospital *hospital, Session *session, int sortBy, int sortOrder)
```

#### Ringkasan
Menampilkan daftar semua pasien terdaftar, diurutkan berdasarkan ID atau nama pengguna. Informasi mencakup ID, Nama Pengguna, dan Penyakit.

#### Parameter
*   `Hospital *hospital`, `Session *session`, `int sortBy`, `int sortOrder`: Mirip dengan `displayUsers`.

#### Algoritma
1.  **Validasi & Otorisasi**: Mirip dengan `displayUsers`.
2.  **Output Awal**: Mencetak header "Daftar Pasien".
3.  **Pemeriksaan Kosong**: Jika `hospital->patients.nEff == 0`, mencetak "Tidak ada pasien terdaftar." dan kembali.
4.  **Buat Daftar Sementara**: Menyalin `hospital->patients.elements` ke `tempList.elements`. Menangani kegagalan alokasi.
5.  **Urutkan Daftar Sementara**: Memanggil `sortPatients(&tempList, sortBy, sortOrder)`.
6.  **Cetak Info Pengurutan**: Mencetak pesan yang menunjukkan kriteria pengurutan.
7.  **Cetak Tabel**:
    *   Header: "ID", "Username", "Penyakit".
    *   Melakukan iterasi melalui `tempList.elements` yang sudah diurutkan:
        *   Untuk setiap `Patient *patient`:
            *   Mengonversi `patient->id` menjadi `idStr`.
            *   Menyalin `patient->disease` ke `diseaseStr` (atau "-" jika kosong).
            *   Mencetak baris.
    *   Mencetak batas tabel.
8.  **Pembersihan**: Membebaskan `tempList.elements`.

### Fungsi: `displayDoctors`

#### Tanda Tangan (Signature)
```c
void displayDoctors(Hospital *hospital, Session *session, int sortBy, int sortOrder)
```

#### Ringkasan
Menampilkan daftar semua dokter terdaftar, diurutkan berdasarkan ID, nama pengguna, atau aura. Informasi mencakup ID, Nama Pengguna, dan Aura.

#### Parameter
*   `Hospital *hospital`, `Session *session`, `int sortBy`, `int sortOrder`: Mirip dengan `displayUsers`. `sortBy` bisa `1` (ID), `2` (nama), atau lainnya (aura).

#### Algoritma
1.  **Validasi & Otorisasi**: Mirip dengan `displayUsers`.
2.  **Output Awal**: Mencetak header "Daftar Dokter".
3.  **Pemeriksaan Kosong**: Jika `hospital->doctors.nEff == 0`, mencetak "Tidak ada dokter terdaftar." dan kembali.
4.  **Buat Daftar Sementara**: Menyalin `hospital->doctors.elements` ke `tempList.elements`. Menangani kegagalan alokasi.
5.  **Urutkan Daftar Sementara**: Memanggil `sortDoctors(&tempList, sortBy, sortOrder)`.
6.  **Cetak Info Pengurutan**: Mencetak pesan yang menunjukkan kriteria pengurutan (ID, nama, atau aura).
7.  **Cetak Tabel**:
    *   Header: "ID", "Username", "Aura".
    *   Melakukan iterasi melalui `tempList.elements` yang sudah diurutkan:
        *   Untuk setiap `Doctor *doctor`:
            *   Mengonversi `doctor->id` menjadi `idStr`.
            *   Mengonversi `doctor->aura` menjadi `auraStr`.
            *   Mencetak baris.
    *   Mencetak batas tabel.
8.  **Pembersihan**: Membebaskan `tempList.elements`.
