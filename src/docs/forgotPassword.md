# Berkas: `src/c/forgotPassword.c`

Berkas ini berisi fungsionalitas untuk mengatur ulang kata sandi pengguna jika mereka lupa, menggunakan kode Run-Length Encoding (RLE) yang berasal dari nama pengguna mereka sebagai metode verifikasi.

## Fungsi: `forgotPassword`

### Tanda Tangan (Signature)
```c
boolean forgotPassword(Hospital *hospital, const char *username, const char *rleCode, const char *newPassword)
```

### Ringkasan
Memungkinkan pengguna untuk mengatur ulang kata sandi mereka dengan menyediakan nama pengguna, kode Run-Length Encoding (RLE) (dianggap dihasilkan dari nama pengguna mereka sebagai bentuk pertanyaan/jawaban keamanan), dan kata sandi baru yang diinginkan. Kata sandi baru kemudian dienkripsi dan disimpan.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`, digunakan untuk mengakses daftar pengguna.
*   `const char *username`: Nama pengguna akun yang kata sandinya akan diatur ulang.
*   `const char *rleCode`: Kode Run-Length Encoding yang disediakan oleh pengguna. Ini dibandingkan dengan kode RLE yang dihasilkan dari `username`.
*   `const char *newPassword`: Kata sandi baru yang diinginkan untuk akun tersebut.

### Algoritma
1.  **Validasi Input (Struktur Rumah Sakit)**:
    *   Memeriksa apakah `hospital` adalah `NULL`. Jika ya, mencetak pesan kesalahan ("Struktur rumah sakit tidak valid!") dan mengembalikan `false`.
2.  **Validasi Input (Input Pengguna)**:
    *   Memeriksa apakah `username`, `rleCode`, atau `newPassword` adalah `NULL` atau string kosong. Jika ya, mencetak pesan kesalahan ("Input tidak valid!") dan mengembalikan `false`.
3.  **Validasi Format Nama Pengguna**:
    *   Memanggil `isValidUsername(username)` untuk memeriksa apakah format nama pengguna valid.
    *   Jika tidak, mencetak pesan kesalahan ("Username tidak valid!") dan mengembalikan `false`.
4.  **Validasi Panjang Kata Sandi Baru**:
    *   Memeriksa apakah `strlen(newPassword) < 6`. Jika kata sandi baru kurang dari 6 karakter, mencetak pesan kesalahan ("Password baru harus minimal 6 karakter!") dan mengembalikan `false`.
5.  **Pemeriksaan Keberadaan Pengguna (Umum)**:
    *   Memeriksa apakah `hospital->users.nEff <= 0` (tidak ada pengguna terdaftar). Jika ya, mencetak pesan kesalahan ("Tidak ada pengguna terdaftar!") dan mengembalikan `false`.
6.  **Cari Pengguna**:
    *   Menginisialisasi `userIdx = -1`.
    *   Melakukan iterasi melalui `hospital->users.elements` untuk menemukan pengguna yang `username`-nya cocok dengan input `username`.
    *   Jika ditemukan, `userIdx` diatur ke indeks pengguna.
    *   Jika tidak ditemukan (`userIdx == -1`), mencetak pesan kesalahan ("Username tidak terdaftar!") dan mengembalikan `false`.
7.  **Pemeriksaan Buffer Kata Sandi Baru**:
    *   `int passwordLength = strlen(newPassword);`
    *   Memeriksa apakah `passwordLength >= 100` (mengasumsikan 100 adalah ukuran maksimal buffer kata sandi terenkripsi dalam struct `User`). Jika terlalu panjang, mencetak pesan kesalahan ("Panjang password terlalu besar untuk dienkripsi!") dan mengembalikan `false`.
8.  **Verifikasi Kode RLE**:
    *   Mendeklarasikan `char expectedRle[100];`.
    *   Memanggil `generateRleCode(username, expectedRle, 100)` untuk menghasilkan kode RLE dari `username` yang diberikan.
    *   Jika `generateRleCode` gagal (mengembalikan `false`), mencetak pesan kesalahan ("Gagal menghasilkan kode RLE!") dan mengembalikan `false`.
    *   Membandingkan input `rleCode` dengan `expectedRle` menggunakan `strcmp`.
    *   Jika tidak cocok (`strcmp(...) != 0`), mencetak pesan kesalahan ("Kode RLE salah!") dan mengembalikan `false`.
9.  **Enkripsi dan Simpan Kata Sandi Baru**:
    *   Memanggil `enigmaEncrypt(newPassword, hospital->users.elements[userIdx].password.encryptedContent, 100)` untuk mengenkripsi `newPassword` dan menyimpannya dalam catatan pengguna.
    *   Jika `enigmaEncrypt` gagal, mencetak pesan kesalahan ("Gagal mengenkripsi password baru!") dan mengembalikan `false`.
10. **Pesan Sukses**:
    *   Membuat pesan sukses: "Password untuk [username] berhasil diperbarui!".
    *   Memanggil `printSuccess()` untuk menampilkan pesan tersebut.
11. **Nilai Kembali**: Mengembalikan `true` yang menunjukkan kata sandi berhasil diatur ulang.

### Nilai Kembali
*   `boolean`: `true` jika proses pengaturan ulang kata sandi berhasil, `false` jika tidak (karena kesalahan validasi, kode RLE salah, atau kegagalan enkripsi).
