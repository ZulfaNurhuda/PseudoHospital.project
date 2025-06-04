# Berkas: `src/c/registerPatient.c`

Berkas ini berisi fungsi spesifik untuk mendaftarkan pasien baru ke dalam sistem rumah sakit. Fungsi ini menangani validasi input, enkripsi kata sandi, pembuatan catatan pengguna dan pasien, serta login otomatis untuk pasien yang baru terdaftar.

## Fungsi: `registerPatient`

### Tanda Tangan (Signature)
```c
boolean registerPatient(Hospital *hospital, Session *session, const char *inputUsername, const char *password)
```

### Ringkasan
Mendaftarkan pasien baru dalam sistem menggunakan nama pengguna dan kata sandi yang diberikan. Fungsi ini melakukan beberapa validasi, termasuk memeriksa apakah pengguna sudah login, validitas dan keunikan nama pengguna, panjang kata sandi, dan kapasitas sistem. Jika berhasil, fungsi ini mengenkripsi kata sandi, membuat entri yang sesuai dalam daftar pengguna dan pasien, menginisialisasi data pasien, dan secara otomatis me-login pasien baru ke dalam sesi yang diberikan.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke struktur `Session`. Ini akan diperbarui jika registrasi dan login otomatis berhasil.
*   `const char *inputUsername`: Nama pengguna untuk pasien baru.
*   `const char *password`: Kata sandi untuk pasien baru.

### Algoritma
1.  **Validasi Input (Pointer)**:
    *   Memeriksa apakah `hospital` atau `session` adalah `NULL`. Jika ya, mencetak kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan mengembalikan `false`.
2.  **Validasi Input (Konten Nama Pengguna/Kata Sandi)**:
    *   Memeriksa apakah `inputUsername` atau `password` adalah `NULL` atau string kosong. Jika ya, mencetak kesalahan ("Username (setelah trim) atau password tidak valid!") dan mengembalikan `false`.
3.  **Pemeriksaan Sesi yang Ada**:
    *   Jika `session->isLoggedIn` adalah `true`, mencetak kesalahan ("Anda sudah login! Silakan logout terlebih dahulu.") dan mengembalikan `false`.
4.  **Validasi Format Nama Pengguna**:
    *   Memanggil `isValidUsername(inputUsername)`. Jika tidak valid, mencetak kesalahan ("Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore.") dan mengembalikan `false`.
5.  **Pemeriksaan Keunikan Nama Pengguna**:
    *   Memanggil `isUsernameTaken(hospital, inputUsername)`. Jika true, mencetak kesalahan ("Registrasi gagal! Pasien dengan nama tersebut sudah terdaftar.") dan mengembalikan `false`.
6.  **Validasi Panjang Kata Sandi**:
    *   Memeriksa apakah `strlen(password) < 6`. Jika terlalu pendek, mencetak kesalahan ("Password harus minimal 6 karakter!") dan mengembalikan `false`.
7.  **Pemeriksaan Kapasitas Sistem**:
    *   Jika `hospital->users.nEff >= hospital->users.capacity` atau `hospital->patients.nEff >= hospital->patients.capacity`, mencetak kesalahan ("Kapasitas pengguna atau pasien penuh!") dan mengembalikan `false`.
8.  **Pemeriksaan Buffer Kata Sandi (untuk enkripsi)**:
    *   `int passwordLength = strlen(password);`
    *   Jika `passwordLength >= 100` (ukuran maksimal untuk buffer enkripsi), mencetak kesalahan ("Panjang password terlalu besar untuk dienkripsi!") dan mengembalikan `false`.
9.  **Hasilkan ID Pasien Baru**:
    *   Mencari `maxId` saat ini di antara semua pengguna di `hospital->users.elements`.
    *   `int newPatientId = maxId + 1;`.
10. **Buat Catatan Pengguna Baru**:
    *   `User *newUser = &hospital->users.elements[hospital->users.nEff];`
    *   `newUser->id = newPatientId;`
    *   `strcpy(newUser->username, inputUsername);`
    *   Memanggil `enigmaEncrypt(password, newUser->password.encryptedContent, 100)` untuk mengenkripsi dan menyimpan kata sandi.
        *   Jika enkripsi gagal, mencetak kesalahan ("Gagal mengenkripsi password!") dan mengembalikan `false`.
    *   `newUser->role = PATIENT;`
11. **Buat Catatan Pasien Baru**:
    *   `Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];`
    *   `newPatient->id = newPatientId;`
    *   `strcpy(newPatient->username, inputUsername);`
    *   `newPatient->bananaRich = 100.0;`
    *   `newPatient->life = 3;`
    *   `newPatient->diagnosedStatus = false;`
    *   `newPatient->treatedStatus = false;`
    *   **Inisialisasi Daftar Obat**:
        *   `medicationsPrescribed`: Mengalokasikan memori untuk 10 `int` menggunakan `safeMalloc`. Jika gagal, mencetak kesalahan dan mengembalikan `false`. Mengatur kapasitas menjadi 10, `nEff` menjadi 0.
        *   `medicationsTaken`: Mengalokasikan memori untuk 10 `int` menggunakan `safeMalloc`. Jika gagal, mencetak kesalahan, membebaskan `medicationsPrescribed.medicationId`, dan mengembalikan `false`. Mengatur kapasitas menjadi 10, `top` menjadi -1.
        *   (Catatan: Field struct adalah `medicationId` di sini, cocok dengan penggunaan lain, tidak seperti salah ketik `medicationID` di `registerPatient` lainnya.)
    *   `newPatient->queueRoom[0] = '\0';` (tidak dalam antrian).
    *   `newPatient->queuePosition = 0;` (menggunakan 0 untuk menunjukkan tidak dalam antrian/di dalam ruangan, konsisten dengan `registerCheckup`).
12. **Tambah Jumlah**:
    *   `hospital->users.nEff++;`
    *   `hospital->patients.nEff++;`
13. **Cetak Pesan Sukses Registrasi**:
    *   Membuat dan mencetak: "Pasien dengan nama [inputUsername] berhasil terdaftar!".
14. **Login Otomatis**:
    *   `session->userId = newPatientId;`
    *   `strcpy(session->username, inputUsername);`
    *   `session->role = PATIENT;`
    *   `session->isLoggedIn = true;`
    *   Mencetak pesan info: "[ℹ️  | Info]: Fitur ini terintegrasi dengan fitur login, kamu tidak perlu login lagi."
    *   Mencetak pesan sukses: "Anda langsung masuk sebagai pasien!".
15. **Nilai Kembali**: Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika registrasi pasien dan login otomatis berhasil, `false` jika tidak.
