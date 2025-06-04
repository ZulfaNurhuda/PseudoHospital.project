# Berkas: `src/c/runLengthEncoding.c`

Berkas ini menyediakan fungsionalitas untuk menghasilkan kode Run-Length Encoding (RLE) dari nama pengguna yang diberikan. Ini digunakan sebagai metode verifikasi, misalnya, dalam fitur "lupa kata sandi".

## Fungsi: `generateRleCode`

### Tanda Tangan (Signature)
```c
boolean generateRleCode(const char *username, char *output, int maxLen)
```

### Ringkasan
Menghasilkan string Run-Length Encoded (RLE) dari input `username`. Dalam implementasi RLE ini, jika sebuah karakter berulang, ia direpresentasikan oleh jumlah pengulangan diikuti oleh karakter itu sendiri. Jika sebuah karakter tidak berulang (jumlahnya 1), hanya karakter tersebut yang dikeluarkan.

### Parameter
*   `const char *username`: String input (nama pengguna) yang akan dikodekan.
*   `char *output`: Buffer karakter tempat kode RLE yang dihasilkan akan disimpan.
*   `int maxLen`: Kapasitas maksimum buffer `output` (termasuk ruang untuk terminator null).

### Algoritma
1.  **Validasi Input (Nama Pengguna)**:
    *   Memeriksa apakah `username` adalah `NULL` atau string kosong. Jika ya, mencetak kesalahan ("Username kosong atau tidak valid untuk kode RLE!") dan mengembalikan `false`.
2.  **Validasi Input (maxLen)**:
    *   Memeriksa apakah `maxLen <= 0`. Jika ya, mencetak kesalahan ("Panjang buffer maksimum tidak valid!") dan mengembalikan `false`.
3.  **Validasi Format Nama Pengguna**:
    *   Memanggil `isValidUsername(username)` (dianggap untuk memeriksa karakter yang diizinkan). Jika tidak valid, mencetak kesalahan ("Username tidak valid untuk kode RLE!") dan mengembalikan `false`.
4.  **Inisialisasi**:
    *   `int usernameLen = strlen(username);`
    *   `int outIndex = 0;` (indeks saat ini untuk buffer `output`).
    *   `int i = 0;` (indeks saat ini untuk iterasi melalui `username`).
5.  **Loop Pembuatan RLE**:
    *   `while (i < usernameLen)`: Melakukan iterasi melalui `username`.
        *   `char currentChar = username[i];`
        *   `int count = 0;`
        *   **Hitung Karakter Berurutan**:
            *   Loop dalam `while (i < usernameLen && username[i] == currentChar)`: Menghitung kemunculan `currentChar`. Menaikkan `count` dan `i`.
        *   **Tambahkan Jumlah (jika > 1)**:
            *   Jika `count > 1`:
                *   Mengonversi `count` menjadi representasi stringnya (`countStr`). Ini melibatkan:
                    *   Mengekstrak digit menggunakan modulo dan pembagian.
                    *   Menyimpan digit di `countStr` dalam urutan terbalik.
                    *   Membalik `countStr` untuk mendapatkan urutan yang benar.
                    *   Pemeriksaan kesalahan untuk buffer overflow `countStr` (`numDigits >= 11`).
                *   Memeriksa apakah ada cukup ruang di buffer `output` untuk `countStr` (`outIndex + numDigits + 1 >= maxLen` - memperhitungkan digit hitungan dan karakter itu sendiri). Jika tidak, mencetak kesalahan dan mengembalikan `false`.
                *   Menyalin digit dari `countStr` ke `output[outIndex++]`.
        *   **Tambahkan Karakter**:
            *   Memeriksa apakah ada cukup ruang di `output` untuk karakter itu sendiri (`outIndex + 1 >= maxLen`). Jika tidak, mencetak kesalahan dan mengembalikan `false`.
            *   `output[outIndex++] = currentChar;`
6.  **Terminasi Null pada Output**:
    *   `output[outIndex] = '\0';`
7.  **Pesan Sukses**:
    *   Mencetak "Kode RLE berhasil dihasilkan!".
    *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika pembuatan kode RLE berhasil, `false` jika tidak (karena input tidak valid, buffer overflow, atau kesalahan lainnya).
