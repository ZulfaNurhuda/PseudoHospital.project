# Berkas: `src/c/utils.c`

Berkas ini menyediakan kumpulan fungsi utilitas yang digunakan di seluruh sistem manajemen rumah sakit. Ini termasuk alokasi memori yang aman, manipulasi string, rutin input pengguna yang divalidasi, konversi tipe, pemeriksaan validasi khusus domain, dan pembantu output konsol yang diformat.

## Fungsi: `safeMalloc`

### Tanda Tangan (Signature)
```c
void *safeMalloc(size_t size)
```

### Ringkasan
Sebuah wrapper di sekitar `malloc` yang memeriksa apakah alokasi berhasil. Meskipun memeriksa `NULL`, fungsi ini tidak memiliki pencetakan kesalahan eksplisit di dalamnya jika `malloc` gagal, melainkan mengembalikan `NULL`, yang mungkin diperiksa oleh fungsi pemanggil.

### Parameter
*   `size_t size`: Jumlah byte yang akan dialokasikan.

### Algoritma
1.  Memanggil `malloc(size)`.
2.  Jika pointer yang dikembalikan `ptr` adalah `NULL`, mengembalikan `NULL`.
3.  Jika tidak, mengembalikan `ptr`.

### Nilai Kembali
*   `void *`: Pointer ke memori yang dialokasikan, atau `NULL` jika alokasi gagal.

## Fungsi: `charToLower`

### Tanda Tangan (Signature)
```c
char charToLower(char c)
```

### Ringkasan
Mengonversi karakter alfabet huruf besar menjadi padanan huruf kecilnya. Karakter lain dikembalikan tanpa perubahan.

### Parameter
*   `char c`: Karakter yang akan dikonversi.

### Algoritma
1.  Jika `c` berada di antara 'A' dan 'Z' (inklusif), tambahkan selisih `('a' - 'A')` ke `c` untuk mengubahnya menjadi huruf kecil.
2.  Mengembalikan karakter (yang mungkin telah dikonversi).

### Nilai Kembali
*   `char`: Versi huruf kecil dari `c` jika itu huruf besar, jika tidak `c` itu sendiri.

## Fungsi: `caseInsensitiveStrcmp`

### Tanda Tangan (Signature)
```c
int caseInsensitiveStrcmp(const char *s1, const char *s2)
```

### Ringkasan
Membandingkan dua string `s1` dan `s2` tanpa mempedulikan huruf besar/kecil.

### Parameter
*   `const char *s1`: String pertama.
*   `const char *s2`: String kedua.

### Algoritma
1.  Menangani input `NULL`:
    *   Jika keduanya `NULL`, mengembalikan `0`.
    *   Jika `s1` adalah `NULL`, mengembalikan `-1` (s1 < s2).
    *   Jika `s2` adalah `NULL`, mengembalikan `1` (s1 > s2).
2.  Melakukan iterasi melalui string karakter per karakter (`i`) selama akhir string (`\0`) belum tercapai untuk kedua string.
    *   Mengonversi `s1[i]` dan `s2[i]` menjadi huruf kecil menggunakan `charToLower`.
    *   Jika karakter huruf kecil berbeda, mengembalikan selisihnya (`c1Lower - c2Lower`).
3.  Jika satu string berakhir sebelum yang lain, perbandingan dilanjutkan berdasarkan terminator null (atau karakter pada `s1[i]` vs `s2[i]` setelah loop). Perbandingan terakhir adalah `charToLower(s1[i]) - charToLower(s2[i])`.

### Nilai Kembali
*   `int`: `0` jika string sama (tanpa mempedulikan huruf besar/kecil), `< 0` jika `s1 < s2`, `> 0` jika `s1 > s2`.

## Fungsi: `readValidString`

### Tanda Tangan (Signature)
```c
boolean readValidString(char *buffer, int maxLen, const char *prompt, boolean alphanumeric)
```

### Ringkasan
Membaca string dari input standar, memastikan tidak kosong, tidak melebihi `maxLen`, dan secara opsional hanya berisi karakter alfanumerik dan garis bawah. Menggunakan `scanf("%255s", temp)` yang membaca satu kata.

### Parameter
*   `char *buffer`: Buffer untuk menyimpan string yang dibaca.
*   `int maxLen`: Panjang maksimum string yang dapat disimpan di `buffer` (termasuk terminator null).
*   `const char *prompt`: Pesan yang akan ditampilkan kepada pengguna sebelum membaca input.
*   `boolean alphanumeric`: Jika `true`, memvalidasi bahwa input hanya berisi 'A'-'Z', 'a'-'z', '0'-'9', atau '_'.

### Algoritma
1.  Memvalidasi `buffer`, `maxLen`, dan `prompt`. Mengembalikan `false` jika tidak valid.
2.  Mencetak `prompt`.
3.  Membaca input ke buffer sementara `temp` menggunakan `scanf("%255s", temp)`.
    *   Jika `scanf` gagal, bersihkan buffer input, cetak kesalahan, kembalikan `false`.
4.  Membersihkan karakter yang tersisa dari baris input (hingga baris baru atau EOF).
5.  Memeriksa `strlen(temp)` terhadap `maxLen`. Mencetak kesalahan dan mengembalikan `false` jika terlalu panjang.
6.  Memeriksa apakah `len == 0`. Mencetak kesalahan dan mengembalikan `false` jika kosong.
7.  Jika `alphanumeric` adalah `true`:
    *   Melakukan iterasi melalui `temp`. Jika ada karakter yang bukan alfanumerik atau garis bawah, cetak kesalahan dan kembalikan `false`.
8.  `strcpy(buffer, temp);`
9.  Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika string yang valid dibaca, `false` jika tidak.

## Fungsi: `readStringWithSpaces`

### Tanda Tangan (Signature)
```c
boolean readStringWithSpaces(char *buffer, int bufferSize, const char *prompt)
```

### Ringkasan
Membaca sebaris teks dari input standar, termasuk spasi, ke dalam `buffer`. Fungsi ini memangkas spasi di awal/akhir dan meruntuhkan beberapa spasi internal menjadi satu spasi.

### Parameter
*   `char *buffer`: Buffer untuk menyimpan string yang dibaca.
*   `int bufferSize`: Ukuran `buffer`.
*   `const char *prompt`: Pesan yang akan ditampilkan kepada pengguna.

### Algoritma
1.  Memvalidasi `buffer` dan `bufferSize`. Mengembalikan `false` jika tidak valid.
2.  Jika `bufferSize == 1`, atur `buffer[0] = '\0'`, bersihkan input, dan kembalikan `true`.
3.  Mencetak `prompt`.
4.  Membaca hingga 255 karakter atau hingga baris baru ke `rawInput` menggunakan `scanf("%255[^\n]", rawInput)`.
5.  Membersihkan sisa buffer input, termasuk baris baru.
6.  Menangani kasus EOF untuk `scanf`.
7.  Memproses `rawInput` ke `buffer`:
    *   Melewati spasi di awal `rawInput`.
    *   Menyalin karakter dari `rawInput` ke `buffer`, memastikan bahwa beberapa spasi diringkas menjadi satu spasi.
    *   Berhenti jika `j` (indeks untuk `buffer`) mencapai `bufferSize - 1`.
8.  Jika karakter terakhir di `buffer` adalah spasi, hapus.
9.  Terminasi null pada `buffer`.
10. Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika pembacaan berhasil (bahkan jika string kosong setelah diproses), `false` pada kesalahan input kritis.

## Fungsi: `readUsernameWithTrim`

### Tanda Tangan (Signature)
```c
boolean readUsernameWithTrim(char *buffer, int bufferSize, const char *prompt)
```

### Ringkasan
Fungsi pembungkus yang memanggil `readStringWithSpaces`. Ditujukan untuk membaca nama pengguna, menyiratkan bahwa nama pengguna dapat berisi spasi dan harus dipangkas.

### Parameter
*   `char *buffer`, `int bufferSize`, `const char *prompt`: Sama seperti `readStringWithSpaces`.

### Algoritma
1.  Mengembalikan hasil dari `readStringWithSpaces(buffer, bufferSize, prompt)`.

### Nilai Kembali
*   `boolean`: Hasil dari `readStringWithSpaces`.

## Fungsi: `readValidInt`

### Tanda Tangan (Signature)
```c
boolean readValidInt(int *value, const char *prompt)
```

### Ringkasan
Membaca string menggunakan `readValidString` (yang membaca satu kata) dan mencoba mengurainya sebagai integer. Memvalidasi karakter numerik, tanda opsional di awal (+/-), dan memastikan nilainya sesuai dengan rentang `int`.

### Parameter
*   `int *value`: Pointer untuk menyimpan integer yang diurai.
*   `const char *prompt`: Pesan yang akan ditampilkan untuk input.

### Algoritma
1.  Memvalidasi `value` dan `prompt`. Mengembalikan `false` jika `NULL`.
2.  Memanggil `readValidString(buffer, ..., prompt, false)` (mengizinkan non-alfanumerik untuk tanda). Mengembalikan `false` jika pembacaan gagal.
3.  Memeriksa apakah `buffer` kosong. Mencetak kesalahan, mengembalikan `false` jika demikian.
4.  Mengurai `buffer` menjadi `long long result`:
    *   Menangani tanda '-' atau '+' opsional di awal.
    *   Memeriksa karakter non-digit. Mencetak kesalahan, mengembalikan `false` jika ditemukan.
    *   Memeriksa overflow terhadap `MAX_LONG_LONG` selama akumulasi. Mencetak kesalahan, mengembalikan `false`.
5.  Memeriksa apakah `result` (dinegasikan jika perlu) berada dalam rentang `int` (`MAX_INT`). Mencetak kesalahan, mengembalikan `false` jika di luar rentang.
6.  Menetapkan `(int)result` ke `*value`.
7.  Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika integer yang valid dibaca dan diurai, `false` jika tidak.

## Fungsi: `readValidFloat`

### Tanda Tangan (Signature)
```c
boolean readValidFloat(float *value, const char *prompt)
```

### Ringkasan
Membaca string menggunakan `readValidString` dan mencoba mengurainya sebagai angka floating-point. Memvalidasi karakter numerik, titik desimal tunggal opsional, dan tanda opsional di awal. Memeriksa apakah angka tersebut sesuai dengan rentang `float`.

### Parameter
*   `float *value`: Pointer untuk menyimpan float yang diurai.
*   `const char *prompt`: Pesan yang akan ditampilkan untuk input.

### Algoritma
1.  Memvalidasi `value` dan `prompt`. Mengembalikan `false` jika `NULL`.
2.  Memanggil `readValidString(buffer, ..., prompt, false)`. Mengembalikan `false` jika pembacaan gagal.
3.  Memeriksa apakah `buffer` kosong. Mencetak kesalahan, mengembalikan `false`.
4.  Mengurai `buffer` menjadi `double result`:
    *   Menangani tanda '-' atau '+' opsional di awal.
    *   Melakukan iterasi melalui karakter:
        *   Jika '.', menangani titik desimal (memastikan hanya satu).
        *   Jika bukan digit atau '.', cetak kesalahan, kembalikan `false`.
        *   Mengakumulasi bagian integer dan bagian pecahan.
    *   Memastikan setidaknya satu digit ada.
5.  Menerapkan tanda ke `result`.
6.  Memeriksa apakah `result` berada dalam rentang `float` tipikal (sekitar `+/-3.4e38`). Mencetak kesalahan, mengembalikan `false` jika di luar rentang.
7.  Menetapkan `(float)result` ke `*value`.
8.  Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika float yang valid dibaca dan diurai, `false` jika tidak.

## Fungsi: `stringToInt`

### Tanda Tangan (Signature)
```c
int stringToInt(const char *str)
```

### Ringkasan
Mengonversi string digit menjadi integer. Tidak menangani tanda atau karakter non-digit setelah karakter non-digit pertama.

### Parameter
*   `const char *str`: String yang akan dikonversi.

### Algoritma
1.  Menginisialisasi `result = 0`, `i = 0`.
2.  Melakukan iterasi selama `str[i]` bukan `\0'`:
    *   Jika `str[i]` bukan digit ('0'-'9'), mengembalikan `-1` (indikator kesalahan).
    *   `result = result * 10 + (str[i] - '0');`
3.  Mengembalikan `result`.

### Nilai Kembali
*   `int`: Integer yang dikonversi, atau `-1` jika karakter non-digit ditemukan.

## Fungsi: `integerToString`

### Tanda Tangan (Signature)
```c
boolean integerToString(int value, char *buffer, int bufferSize)
```

### Ringkasan
Mengonversi integer `value` menjadi representasi stringnya dan menyimpannya di `buffer`. Menangani angka negatif dan nol.

### Parameter
*   `int value`: Integer yang akan dikonversi.
*   `char *buffer`: Buffer untuk menyimpan string yang dihasilkan.
*   `int bufferSize`: Ukuran `buffer`.

### Algoritma
1.  Memvalidasi `buffer` dan `bufferSize`. Mengembalikan `false` jika tidak valid.
2.  Menangani `value == 0` sebagai kasus khusus.
3.  Menangani `value` negatif dengan mengatur `isNegative = true` dan menggunakan `long long longValue = -value;`.
4.  Mengonversi `longValue` menjadi string `tempReverse` dengan berulang kali mengambil modulo 10 (digit dalam urutan terbalik).
5.  Memeriksa apakah `bufferSize` cukup untuk jumlah digit, tanda (jika ada), dan terminator null. Mengembalikan `false` jika tidak.
6.  Membuat string di `buffer`: menambahkan '-' jika `isNegative`, kemudian menyalin digit dari `tempReverse` (dalam urutan yang benar).
7.  Terminasi null pada `buffer`.
8.  Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika konversi berhasil, `false` jika buffer terlalu kecil atau tidak valid.

## Fungsi: `floatToString`

### Tanda Tangan (Signature)
```c
boolean floatToString(float value, char *buffer, int bufferSize, int precision)
```

### Ringkasan
Mengonversi nilai `float` menjadi representasi stringnya dengan `precision` tertentu untuk bagian pecahan. Menangani NaN, tak terhingga, nol, dan angka negatif. Menerapkan pembulatan untuk bagian pecahan.

### Parameter
*   `float value`: Nilai float yang akan dikonversi.
*   `char *buffer`: Buffer untuk menyimpan string.
*   `int bufferSize`: Ukuran `buffer`.
*   `int precision`: Jumlah digit setelah titik desimal.

### Algoritma
1.  Memvalidasi input. Mengembalikan `false` jika tidak valid.
2.  Menangani kasus khusus: NaN ("nan"), tak terhingga ("inf", "-inf"), nol ("0.00" dengan presisi).
3.  Menangani tanda: `isNegative`, `absValue`.
4.  Memisahkan `absValue` menjadi `intPart` dan `fracPart`.
5.  Mengonversi `intPart` menjadi `intStr` menggunakan `integerToString`.
6.  Memeriksa `bufferSize` terhadap panjang yang diperlukan (tanda, intStr, '.', digit presisi, terminator null). Mengembalikan `false` jika terlalu kecil.
7.  Membuat string di `buffer`:
    *   Menambahkan '-' jika `isNegative`.
    *   Menambahkan `intStr`.
    *   Jika `precision > 0`:
        *   Menambahkan '.'.
        *   Menerapkan pembulatan ke `fracPart` berdasarkan `precision`.
        *   Jika pembulatan `fracPart` menyebabkannya menjadi >= 1.0, naikkan `intPart`, hitung ulang `intStr` dan ukuran yang diperlukan, dan bangun kembali bagian integer di buffer.
        *   Menambahkan sejumlah `precision` digit pecahan ke `buffer` dengan berulang kali mengalikan `fracPart` dengan 10.
8.  Terminasi null pada `buffer`.
9.  Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika berhasil, `false` untuk kesalahan atau buffer tidak cukup.

## Fungsi: `isValidUsername`

### Tanda Tangan (Signature)
```c
boolean isValidUsername(const char *username)
```

### Ringkasan
Memvalidasi string nama pengguna. Harus antara 1 dan 50 karakter (inklusif), tidak hanya terdiri dari spasi, dan hanya berisi karakter alfanumerik, garis bawah, atau spasi.

### Parameter
*   `const char *username`: String nama pengguna untuk divalidasi.

### Algoritma
1.  Mengembalikan `false` jika `username` adalah `NULL`.
2.  Memeriksa panjang: `false` jika `len == 0` atau `len > 50`.
3.  Melakukan iterasi melalui `username`:
    *   Memeriksa apakah setiap karakter adalah alfanumerik, garis bawah, atau spasi. Jika tidak, mengembalikan `false`.
    *   Melacak apakah ada karakter non-spasi yang ditemukan (`allSpaces`).
4.  Jika `allSpaces` adalah `true` (dan `len > 0`), mengembalikan `false`.
5.  Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika nama pengguna valid, `false` jika tidak.

## Fungsi: `isUsernameTaken`

### Tanda Tangan (Signature)
```c
boolean isUsernameTaken(Hospital *hospital, const char *username)
```

### Ringkasan
Memeriksa apakah `username` yang diberikan (tanpa mempedulikan huruf besar/kecil) sudah ada di daftar `hospital->users`.

### Parameter
*   `Hospital *hospital`: Pointer ke data rumah sakit.
*   `const char *username`: Nama pengguna untuk diperiksa.

### Algoritma
1.  Mengembalikan `true` (sebagai tindakan pencegahan/kesalahan) jika `hospital` atau `username` adalah `NULL`.
2.  Mengonversi input `username` menjadi huruf kecil `lowerUsername`.
3.  Melakukan iterasi melalui `hospital->users.elements`:
    *   Mengonversi `existingUsername` menjadi huruf kecil `lowerElementUsername`.
    *   Jika `strcmp(lowerUsername, lowerElementUsername) == 0`, mengembalikan `true`.
4.  Jika loop selesai, mengembalikan `false`.

### Nilai Kembali
*   `boolean`: `true` jika nama pengguna sudah dipakai (atau kesalahan input), `false` jika tidak.

## Fungsi: `isValidRoomCode`

### Tanda Tangan (Signature)
```c
boolean isValidRoomCode(Hospital *hospital, const char *roomCode)
```

### Ringkasan
Memvalidasi format kode ruangan (misalnya, "A1", "C12"). Memeriksa panjang, memastikan karakter pertama adalah huruf besar, karakter berikutnya adalah digit, dan baris/kolom yang diurai berada dalam dimensi tata letak rumah sakit.

### Parameter
*   `Hospital *hospital`: Pointer ke data rumah sakit (untuk dimensi tata letak).
*   `const char *roomCode`: String kode ruangan untuk divalidasi.

### Algoritma
1.  Memeriksa `strlen(roomCode)`: `false` jika bukan antara 2 dan 4 karakter.
2.  `row = roomCode[0]`, `colStr` adalah sisanya.
3.  Memeriksa apakah `row` adalah 'A'-'Z'. `false` jika tidak.
4.  Memeriksa apakah semua karakter di `colStr` adalah '0'-'9'. `false` jika tidak.
5.  Mengonversi `colStr` menjadi integer `col`.
6.  Memeriksa apakah `row - 'A'` berada dalam `hospital->layout.rowEff` dan `col` berada dalam `hospital->layout.colEff` (dan `col > 0`). `false` jika di luar batas.
7.  Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika kode ruangan valid, `false` jika tidak.

## Fungsi Pemformatan Output Konsol

### `void printHeader(const char *title)`
Mencetak kotak header yang diformat dengan `title` yang diberikan.

### `void printError(const char *message)`
Mencetak pesan kesalahan dengan awalan "ERROR: " berwarna merah.

### `void printSuccess(const char *message)`
Mencetak pesan sukses dengan awalan "SUKSES: " berwarna hijau.

### `void printTableBorder(int *widths, int n, int type)`
Mencetak garis batas tabel horizontal (atas, tengah, atau bawah ditentukan oleh `type`) menggunakan karakter gambar kotak, menampung `n` kolom dengan `widths` yang ditentukan.

### `void printTableRow(const char **row, int *widths, int n)`
Mencetak satu baris tabel, dengan `n` sel konten dari array `row`, diformat sesuai `widths` yang diberikan.
