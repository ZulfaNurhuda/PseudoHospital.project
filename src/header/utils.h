#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"

// Warna ANSI untuk output terminal
#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_GREY "\033[90m"
#define COLOR_RESET "\033[0m"
#define FORMAT_BOLD "\x1b[1m"
#define FORMAT_RESET "\x1b[0m"

/**
 * Nama Fungsi: `safeMalloc`
 *
 * Deskripsi: Mengalokasikan memori dengan `malloc` dan memeriksa apakah alokasi berhasil.
 * Pemanggil bertanggung jawab untuk menangani kasus return NULL.
 *
 * Parameter: `size (size_t)` - Ukuran memori yang akan dialokasikan dalam byte.
 *
 * Return: `void*` - Pointer ke memori yang dialokasikan, atau `NULL` jika gagal.
 */
void *safeMalloc(size_t size);

/**
 * Nama Fungsi: `charToLower`
 *
 * Deskripsi: Mengonversi sebuah karakter huruf besar menjadi huruf kecil.
 *
 * Parameter: `c (char)` - Karakter yang akan dikonversi.
 *
 * Return: `char` - Karakter dalam versi huruf kecil, atau karakter asli jika bukan huruf besar.
 */
char charToLower(char c);

/**
 * Nama Fungsi: `caseInsensitiveStrcmp`
 *
 * Deskripsi: Membandingkan dua string tanpa memperhatikan besar kecilnya huruf (case-insensitive).
 *
 * Parameter:
 * - `s1 (const char*)`: String pertama untuk dibandingkan.
 * - `s2 (const char*)`: String kedua untuk dibandingkan.
 *
 * Return: `int` - 0 jika string identik (ignore case), <0 jika s1 < s2, >0 jika s1 > s2.
 */
int caseInsensitiveStrcmp(const char *s1, const char *s2);

/**
 * Nama Fungsi: `readValidString`
 *
 * Deskripsi: Membaca sebuah kata (string tanpa spasi) dari input standar, memastikan panjangnya tidak melebihi batas,
 * tidak kosong, dan secara opsional hanya berisi karakter alfanumerik.
 *
 * Parameter:
 * - `buffer (char*)`: Array untuk menyimpan string yang dibaca.
 * - `maxLen (int)`: Panjang maksimum string (termasuk null terminator).
 * - `prompt (const char*)`: Prompt yang ditampilkan kepada pengguna.
 * - `alphanumeric (boolean)`: `true` jika string hanya boleh berisi huruf, angka, atau underscore.
 *
 * Return: `boolean` - `true` jika input valid, `false` jika kosong, terlalu panjang, atau gagal baca.
 */
boolean readValidString(char *buffer, int maxLen, const char *prompt, boolean alphanumeric);

/**
 * Nama Fungsi: `readStringWithSpaces`
 *
 * Deskripsi: Membaca seluruh baris input yang mungkin mengandung spasi, memangkas spasi di awal/akhir,
 * dan mengubah beberapa spasi berurutan di tengah menjadi satu spasi tunggal.
 *
 * Parameter:
 * - `buffer (char*)`: Array untuk menyimpan string yang sudah diproses.
 * - `bufferSize (int)`: Ukuran penuh dari buffer.
 * - `prompt (const char*)`: Prompt yang ditampilkan kepada pengguna.
 *
 * Return: `boolean` - `true` jika pembacaan dan pemrosesan berhasil, `false` jika terjadi error.
 */
boolean readStringWithSpaces(char *buffer, int bufferSize, const char *prompt);

/**
 * Nama Fungsi: `readUsernameWithTrim`
 *
 * Deskripsi: Membaca username dari input standar. Fungsi ini merupakan wrapper untuk `readStringWithSpaces`
 * untuk memastikan username yang mengandung spasi dapat dibaca dan diproses dengan benar.
 *
 * Parameter:
 * - `buffer (char*)`: Array untuk menyimpan username yang dibaca.
 * - `bufferSize (int)`: Ukuran penuh dari buffer.
 * - `prompt (const char*)`: Prompt yang ditampilkan kepada pengguna.
 *
 * Return: `boolean` - `true` jika pembacaan berhasil, `false` jika terjadi error.
 */
boolean readUsernameWithTrim(char *buffer, int bufferSize, const char *prompt);

/**
 * Nama Fungsi: `readValidInt`
 *
 * Deskripsi: Membaca sebuah bilangan bulat dari input standar, memastikan input adalah angka yang valid
 * dan berada dalam rentang tipe data `int`.
 *
 * Parameter:
 * - `value (int*)`: Pointer ke variabel yang akan menyimpan hasil integer.
 * - `prompt (const char*)`: Prompt yang ditampilkan kepada pengguna.
 *
 * Return: `boolean` - `true` jika input valid dan berhasil dikonversi, `false` jika tidak.
 */
boolean readValidInt(int *value, const char *prompt);

/**
 * Nama Fungsi: `readValidFloat`
 *
 * Deskripsi: Membaca sebuah bilangan desimal dari input standar, memastikan input adalah angka desimal
 * yang valid dan berada dalam rentang tipe data `float`.
 *
 * Parameter:
 * - `value (float*)`: Pointer ke variabel yang akan menyimpan hasil float.
 * - `prompt (const char*)`: Prompt yang ditampilkan kepada pengguna.
 *
 * Return: `boolean` - `true` jika input valid dan berhasil dikonversi, `false` jika tidak.
 */
boolean readValidFloat(float *value, const char *prompt);

/**
 * Nama Fungsi: `stringToInt`
 *
 * Deskripsi: Mengonversi string yang terdiri dari digit angka menjadi bilangan integer.
 * Fungsi ini tidak menggunakan pustaka `atoi`.
 *
 * Parameter:
 * - `str (const char*)`: String yang akan dikonversi.
 *
 * Return: `int` - Nilai integer hasil konversi, atau -1 jika string mengandung karakter non-digit.
 */
int stringToInt(const char *str);

/**
 * Nama Fungsi: `integerToString`
 *
 * Deskripsi: Mengonversi nilai integer menjadi representasi string-nya.
 *
 * Parameter:
 * - `value (int)`: Nilai integer yang akan dikonversi.
 * - `buffer (char*)`: Buffer untuk menyimpan string hasil konversi.
 * - `bufferSize (int)`: Ukuran dari buffer.
 *
 * Return: `boolean` - `true` jika konversi berhasil, `false` jika buffer terlalu kecil.
 */
boolean integerToString(int value, char *buffer, int bufferSize);

/**
 * Nama Fungsi: `floatToString`
 *
 * Deskripsi: Mengonversi nilai float menjadi representasi string-nya dengan presisi yang ditentukan.
 * Mendukung penanganan NaN dan Infinity.
 *
 * Parameter:
 * - `value (float)`: Nilai float yang akan dikonversi.
 * - `buffer (char*)`: Buffer untuk menyimpan string hasil konversi.
 * - `bufferSize (int)`: Ukuran dari buffer.
 * - `precision (int)`: Jumlah digit di belakang koma.
 *
 * Return: `boolean` - `true` jika konversi berhasil, `false` jika buffer terlalu kecil.
 */
boolean floatToString(float value, char *buffer, int bufferSize, int precision);

/**
 * Nama Fungsi: `isValidUsername`
 *
 * Deskripsi: Memeriksa apakah format username valid. Username harus memiliki panjang 1-50 karakter,
 * hanya berisi huruf, angka, underscore, atau spasi, dan tidak boleh hanya terdiri dari spasi.
 *
 * Parameter: `username (const char*)` - Username yang akan divalidasi.
 *
 * Return: `boolean` - `true` jika valid, `false` jika tidak.
 */
boolean isValidUsername(const char *username);

/**
 * Nama Fungsi: `isUsernameTaken`
 *
 * Deskripsi: Memeriksa apakah username sudah terdaftar dalam sistem (perbandingan case-insensitive).
 *
 * Parameter:
 * - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pengguna.
 * - `username (const char*)`: Username yang akan diperiksa.
 *
 * Return: `boolean` - `true` jika username sudah terdaftar, `false` jika belum atau jika terjadi error.
 */
boolean isUsernameTaken(Hospital *hospital, const char *username);

/**
 * Nama Fungsi: `isValidRoomCode`
 *
 * Deskripsi: Memeriksa apakah kode ruangan valid sesuai format (misal "A1") dan berada dalam
 * batas dimensi denah rumah sakit (`layout.rowEff` dan `layout.colEff`).
 *
 * Parameter:
 * - `hospital (Hospital*)`: Pointer ke struktur rumah sakit untuk mendapatkan ukuran denah.
 * - `roomCode (const char*)`: Kode ruangan yang akan divalidasi.
 *
 * Return: `boolean` - `true` jika valid, `false` jika tidak.
 */
boolean isValidRoomCode(Hospital *hospital, const char *roomCode);

/**
 * Nama Fungsi: `printHeader`
 *
 * Deskripsi: Mencetak header berbingkai dengan seni ASCII untuk judul, menggunakan warna biru.
 *
 * Parameter: `title (const char*)` - Judul yang akan ditampilkan di dalam header.
 *
 * Return: `void`
 */
void printHeader(const char *title);

/**
 * Nama Fungsi: `printError`
 *
 * Deskripsi: Mencetak pesan kesalahan dengan warna merah dan prefix "ERROR: ".
 *
 * Parameter: `message (const char*)` - Pesan kesalahan yang akan dicetak.
 *
 * Return: `void`
 */
void printError(const char *message);

/**
 * Nama Fungsi: `printSuccess`
 *
 * Deskripsi: Mencetak pesan keberhasilan dengan warna hijau dan prefix "SUKSES: ".
 *
 * Parameter: `message (const char*)` - Pesan keberhasilan yang akan dicetak.
 *
 * Return: `void`
 */
void printSuccess(const char *message);

/**
 * Nama Fungsi: `printTableBorder`
 *
 * Deskripsi: Mencetak satu baris border untuk tabel (atas, tengah, atau bawah)
 * menggunakan karakter box-drawing.
 *
 * Parameter:
 * - `widths (int*)`: Array yang berisi lebar untuk masing-masing kolom.
 * - `n (int)`: Jumlah kolom dalam tabel.
 * - `type (int)`: Tipe garis border (1: atas, 2: tengah, 3: bawah).
 *
 * Return: `void`
 */
void printTableBorder(int *widths, int n, int type);

/**
 * Nama Fungsi: `printTableRow`
 *
 * Deskripsi: Mencetak satu baris data tabel, diformat rata kiri sesuai lebar kolom yang ditentukan.
 *
 * Parameter:
 * - `row (const char**)`: Array string yang berisi data untuk setiap sel di baris.
 * - `widths (int*)`: Array yang berisi lebar untuk masing-masing kolom.
 * - `n (int)`: Jumlah kolom dalam tabel.
 *
 * Return: `void`
 */
void printTableRow(const char **row, int *widths, int n);

#endif // UTILS_H