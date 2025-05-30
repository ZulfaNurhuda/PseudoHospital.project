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
#define COLOR_RESET "\033[0m"

/**
 * Nama Fungsi: `safeMalloc`
 *
 * Deskripsi: Mengalokasikan memori dengan malloc dan memeriksa apakah alokasi berhasil.
 *
 * Parameter: `size (size_t)` - Ukuran memori yang akan dialokasikan dalam byte.
 *
 * Return: `void*` - Pointer ke memori yang dialokasikan, atau keluar jika gagal.
 */
void *safeMalloc(size_t size);

/**
 * Nama Fungsi: `readValidString`
 *
 * Deskripsi: Membaca string dari input standar, memastikan panjangnya tidak melebihi batas maksimum dan tidak kosong.
 *
 * Parameter:
 *   - `buffer (char*)`: Array tempat string disimpan.
 *   - `maxLen (int)`: Panjang maksimum string (termasuk null terminator).
 *   - `prompt (const char*)`: Prompt yang ditampilkan sebelum membaca input.
 *   - `alphanumeric (boolean)`: True jika string hanya boleh huruf, angka, atau underscore.
 *
 * Return: `boolean` - True jika input valid, false jika kosong atau terlalu panjang.
 */
boolean readValidString(char *buffer, int maxLen, const char *prompt, boolean alphanumeric);

/**
 * Nama Fungsi: `readValidInt`
 *
 * Deskripsi: Membaca bilangan bulat dari input standar, memastikan input valid.
 *
 * Parameter:
 *   - `value (int*)`: Pointer ke variabel yang menyimpan hasil input.
 *   - `prompt (const char*)`: Prompt yang ditampilkan sebelum membaca input.
 *
 * Return: `boolean` - True jika input valid, false jika tidak.
 */
boolean readValidInt(int *value, const char *prompt);

/**
 * Nama Fungsi: `readValidFloat`
 *
 * Deskripsi: Membaca bilangan desimal dari input standar, memastikan input valid.
 *
 * Parameter:
 *   - `value (float*)`: Pointer ke variabel yang menyimpan hasil input.
 *   - `prompt (const char*)`: Prompt yang ditampilkan sebelum membaca input.
 *
 * Return: `boolean` - True jika input valid, false jika tidak.
 */
boolean readValidFloat(float *value, const char *prompt);

/**
 * Nama Fungsi: `isValidUsername`
 *
 * Deskripsi: Memeriksa apakah username valid (huruf, angka, underscore).
 *
 * Parameter: `username (const char*)` - Username yang divalidasi.
 *
 * Return: `boolean` - True jika valid, false jika tidak.
 */
boolean isValidUsername(const char *username);

/**
 * Nama Fungsi: `isValidRoomCode`
 *
 * Deskripsi: Memeriksa apakah kode ruangan valid sesuai format denah (misalnya "A1") dan ukuran denah rumah sakit (layout.rowEff dan layout.colEff).
 *
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit untuk ukuran denah.
 *   - `roomCode (const char*)`: Kode ruangan yang divalidasi.
 *
 * Return: `boolean` - True jika valid, false jika tidak.
 */
boolean isValidRoomCode(Hospital *hospital, const char *roomCode);

/**
 * Nama Fungsi: `printHeader`
 *
 * Deskripsi: Mencetak header dengan seni ASCII untuk judul, dengan garis berwarna biru.
 *
 * Parameter: `title (const char*)` - Judul yang ditampilkan.
 *
 * Return: `void`
 */
void printHeader(const char *title);

/**
 * Nama Fungsi: `printError`
 *
 * Deskripsi: Mencetak pesan kesalahan dengan warna merah.
 *
 * Parameter: `message (const char*)` - Pesan kesalahan.
 *
 * Return: `void`
 */
void printError(const char *message);

/**
 * Nama Fungsi: `printSuccess`
 *
 * Deskripsi: Mencetak pesan keberhasilan dengan warna hijau.
 *
 * Parameter: `message (const char*)` - Pesan keberhasilan.
 *
 * Return: `void`
 */
void printSuccess(const char *message);

/**
 * Nama Fungsi: `printTableBorder`
 *
 * Deskripsi: Mencetak garis pembatas tabel dengan lebar kolom tertentu.
 *
 * Parameter:
 *   - `widths (int*)`: Array lebar masing-masing kolom.
 *   - `n (int)`: Jumlah kolom.
 *   - `type (int)`: Tipe garis (1: atas, 2: tengah, 3: bawah).
 *
 * Return: `void`
 */
void printTableBorder(int *widths, int n, int type);

/**
 * Nama Fungsi: `printTableRow`
 *
 * Deskripsi: Mencetak satu baris tabel dengan data, menyesuaikan lebar kolom.
 *
 * Parameter:
 *   - `row (const char**)`: Array string data untuk setiap kolom.
 *   - `widths (int*)`: Array lebar masing-masing kolom.
 *   - `n (int)`: Jumlah kolom.
 *
 * Return: `void`
 */
void printTableRow(const char **row, int *widths, int n);

/**
 * Nama Fungsi: `stringToInt`
 *
 * Deskripsi: Mengubah string yang terdiri dari digit angka menjadi bilangan integer. Fungsi ini melakukan parsing manual tanpa menggunakan pustaka `atoi`. Jika string mengandung karakter non-digit, fungsi akan mengembalikan -1.
 *
 * Parameter:
 *   - `str (const char*)`: String yang akan dikonversi ke bilangan integer.
 *
 * Return: `int` — Nilai integer hasil konversi, atau -1 jika input tidak valid.
 */
int stringToInt(const char *str);

// Removed integerToString and floatToString declarations (again)

/**
 * Nama Fungsi: `integerToString`
 * Deskripsi: Mengubah integer menjadi string.
 * Parameter:
 *   - value (int): Nilai integer yang akan dikonversi.
 *   - buffer (char*): Buffer untuk menyimpan string hasil konversi.
 *   - bufferSize (int): Ukuran buffer.
 * Return: `boolean` - True jika konversi berhasil, false jika buffer terlalu kecil.
 */
boolean integerToString(int value, char *buffer, int bufferSize);

/**
 * Nama Fungsi: `floatToString`
 * Deskripsi: Mengubah float menjadi string dengan presisi tertentu.
 * Parameter:
 *   - value (float): Nilai float yang akan dikonversi.
 *   - buffer (char*): Buffer untuk menyimpan string hasil konversi.
 *   - bufferSize (int): Ukuran buffer.
 *   - precision (int): Jumlah digit di belakang koma.
 * Return: `boolean` - True jika konversi berhasil, false jika buffer terlalu kecil.
 */
boolean floatToString(float value, char *buffer, int bufferSize, int precision);

/**
 * Nama Fungsi: `isUsernameTaken`
 *
 * Deskripsi: Memeriksa apakah username sudah terdaftar dalam sistem (case-insensitive).
 *
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit (menggunakan typedef).
 *   - `username (const char*)`: Username yang akan diperiksa.
 *
 * Return: `_Bool` - True jika username sudah terdaftar, false jika belum. (Using _Bool directly)
 */
_Bool isUsernameTaken(Hospital *hospital, const char *username);

/**
 * Nama Fungsi: `readStringWithSpaces`
 * Deskripsi: Membaca sebuah string dari input standar yang mungkin mengandung spasi,
 *            hingga newline atau bufferSize-1 karakter. Membersihkan newline dari buffer.
 * Parameter:
 *   - buffer (char*): Array tempat string disimpan.
 *   - bufferSize (int): Ukuran penuh buffer.
 *   - prompt (const char*): Prompt yang ditampilkan sebelum membaca input.
 * Return: `boolean` - True jika input berhasil dibaca (meskipun kosong), false jika error.
 */
boolean readStringWithSpaces(char *buffer, int bufferSize, const char *prompt);

/**
 * Nama Fungsi: `readUsernameWithTrim`
 * Deskripsi: Membaca username dari input standar, mengizinkan spasi, dan melakukan trim.
 *            Mirip dengan readStringWithSpaces, namun dikhususkan untuk username.
 * Parameter:
 *   - buffer (char*): Array tempat string disimpan.
 *   - bufferSize (int): Ukuran penuh buffer.
 *   - prompt (const char*): Prompt yang ditampilkan sebelum membaca input.
 * Return: `boolean` - True jika input berhasil dibaca, false jika error.
 */
boolean readUsernameWithTrim(char *buffer, int bufferSize, const char *prompt);

/**
 * Nama Fungsi: `customCharToLower`
 * Deskripsi: Mengubah sebuah karakter menjadi lowercase jika ia uppercase.
 * Parameter:
 *   - c (char): Karakter input.
 * Return: `char` - Karakter lowercase atau karakter original jika bukan uppercase.
 */
char customCharToLower(char c);

/**
 * Nama Fungsi: `customCaseInsensitiveStrcmp`
 * Deskripsi: Membandingkan dua string secara case-insensitive.
 * Parameter:
 *   - s1 (const char*): String pertama.
 *   - s2 (const char*): String kedua.
 * Return: `int` - 0 jika identik (ignore case), <0 jika s1<s2, >0 jika s1>s2.
 */
int customCaseInsensitiveStrcmp(const char *s1, const char *s2);

#endif // UTILS_H