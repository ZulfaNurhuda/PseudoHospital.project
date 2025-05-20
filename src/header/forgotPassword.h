#ifndef FORGOT_PASSWORD_H
#define FORGOT_PASSWORD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"
#include "enigma.h"
#include "runLengthEncoding.h"

/**
 * Nama Fungsi: `forgotPassword`
 * 
 * Deskripsi: Mengatur ulang kata sandi pengguna berdasarkan nama pengguna dan kode RLE untuk verifikasi. Memeriksa apakah nama pengguna ada, memvalidasi kode RLE, mengenkripsi kata sandi baru menggunakan enigma_encrypt, dan memperbarui data pengguna.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar pengguna.
 *   - `username (const char*)`: Nama pengguna yang kata sandinya akan diatur ulang.
 *   - `rleCode (const char*)`: Kode RLE untuk verifikasi identitas pengguna.
 *   - `newPassword (const char*)`: Kata sandi baru yang akan dienkripsi dan disimpan.
 * 
 * Return: `boolean` - True jika pengaturan ulang berhasil, false jika nama pengguna tidak ditemukan, kode RLE salah, atau input tidak valid.
 */
boolean forgotPassword(Hospital *hospital, const char *username, const char *rleCode, const char *newPassword);

#endif // FORGOT_PASSWORD_H