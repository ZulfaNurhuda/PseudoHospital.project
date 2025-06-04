# Berkas: `src/c/login.c`

Berkas ini berisi logika untuk autentikasi pengguna (login) ke dalam sistem manajemen rumah sakit.

## Fungsi: `login`

### Tanda Tangan (Signature)
```c
boolean login(Hospital *hospital, Session *session, const char *username, const char *password)
```

### Ringkasan
Mengautentikasi pengguna berdasarkan nama pengguna dan kata sandi yang mereka berikan. Jika berhasil, fungsi ini memperbarui struktur `Session` dengan detail pengguna (ID, nama pengguna, peran) dan mengatur status login mereka menjadi true. Fungsi ini melakukan berbagai validasi, termasuk memeriksa sesi login yang ada dan membandingkan kata sandi input yang dienkripsi dengan kata sandi terenkripsi yang tersimpan.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`, digunakan untuk mengakses daftar pengguna terdaftar (`hospital->users`).
*   `Session *session`: Pointer ke struktur `Session`. Struktur ini akan diperbarui dengan informasi pengguna setelah login berhasil.
*   `const char *username`: Nama pengguna yang diberikan oleh pengguna yang mencoba login.
*   `const char *password`: Kata sandi yang diberikan oleh pengguna.

### Algoritma
1.  **Validasi Input (Pointer)**:
    *   Memeriksa apakah pointer `hospital` atau `session` adalah `NULL`. Jika ya, mencetak kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan mengembalikan `false`.
2.  **Validasi Input (Keberadaan Nama Pengguna/Kata Sandi)**:
    *   Memeriksa apakah `username` atau `password` adalah `NULL` atau string kosong. Jika ya, mencetak kesalahan ("Username atau password tidak valid!") dan mengembalikan `false`.
3.  **Validasi Format Nama Pengguna**:
    *   Memanggil `isValidUsername(username)` untuk memeriksa apakah format nama pengguna valid.
    *   Jika tidak, mencetak kesalahan ("Username tidak valid!") dan mengembalikan `false`.
4.  **Validasi Panjang Kata Sandi (Dasar)**:
    *   Memeriksa apakah `strlen(password) < 6`. Jika kata sandi kurang dari 6 karakter, mencetak kesalahan ("Password harus minimal 6 karakter!") dan mengembalikan `false`. (Ini adalah pemeriksaan awal; perbandingan kata sandi aktual dilakukan dengan bentuk terenkripsi).
5.  **Pemeriksaan Sesi yang Ada**:
    *   Jika `session->isLoggedIn` adalah `true`, mencetak kesalahan ("Anda sudah login! Silakan logout terlebih dahulu.") dan mengembalikan `false`.
6.  **Pemeriksaan Keberadaan Pengguna (Umum)**:
    *   Jika `hospital->users.nEff <= 0` (tidak ada pengguna terdaftar), mencetak kesalahan ("Tidak ada pengguna terdaftar!") dan mengembalikan `false`.
7.  **Iterasi Melalui Pengguna untuk Menemukan Kecocokan**:
    *   Melakukan loop melalui setiap pengguna di `hospital->users.elements` (`i` dari `0` hingga `hospital->users.nEff - 1`).
    *   **Kecocokan Nama Pengguna**: Jika `strcmp(hospital->users.elements[i].username, username) == 0`:
        *   **Pemeriksaan Buffer Kata Sandi**:
            *   `int passwordLength = strlen(password);`
            *   Jika `passwordLength >= 100` (panjang maksimal untuk buffer enkripsi), mencetak kesalahan ("Panjang password terlalu besar untuk dienkripsi!") dan mengembalikan `false`.
        *   **Enkripsi Kata Sandi Input**:
            *   Mendeklarasikan `char encryptedPassword[100];`.
            *   Memanggil `enigmaEncrypt(password, encryptedPassword, 100)`.
            *   Jika `enigmaEncrypt` gagal, mencetak kesalahan ("Gagal mengenkripsi password!") dan mengembalikan `false`.
        *   **Bandingkan Kata Sandi Terenkripsi**:
            *   Jika `strcmp(hospital->users.elements[i].password.encryptedContent, encryptedPassword) == 0`:
                *   **Login Berhasil**:
                    *   `session->userId = hospital->users.elements[i].id;`
                    *   `strcpy(session->username, username);`
                    *   `session->role = hospital->users.elements[i].role;`
                    *   `session->isLoggedIn = true;`
                    *   Membuat pesan sukses: "Login berhasil sebagai [username]! (Peran: [RoleName])". RoleName ditentukan berdasarkan `session->role`.
                    *   `printSuccess(successMessage);`
                    *   Mengembalikan `true`.
            *   **Kata Sandi Tidak Cocok**:
                *   `printError("Password salah!");`
                *   Mengembalikan `false`.
8.  **Nama Pengguna Tidak Ditemukan**:
    *   Jika loop selesai tanpa menemukan nama pengguna, mencetak kesalahan ("Username tidak ditemukan!").
    *   Mengembalikan `false`.

### Nilai Kembali
*   `boolean`: `true` jika login berhasil, `false` jika tidak (karena kesalahan validasi, pengguna tidak ditemukan, atau kata sandi salah).
