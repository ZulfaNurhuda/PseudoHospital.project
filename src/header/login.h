#ifndef LOGIN_H
#define LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"
#include "enigma.h"

/**
 * Nama Fungsi: `login`
 * 
 * Deskripsi: Mengautentikasi pengguna (Manajer, Dokter, atau Pasien) berdasarkan nama pengguna dan kata sandi. Kata sandi input dienkripsi menggunakan enigma_encrypt sebelum dibandingkan dengan kata sandi tersimpan yang sudah terenkripsi. Jika berhasil, mengatur sesi dengan ID pengguna, nama pengguna, peran, dan status login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar pengguna.
 *   - `session (Session*)`: Pointer ke struktur sesi yang akan diperbarui dengan data login.
 *   - `username (const char*)`: Nama pengguna yang akan diautentikasi.
 *   - `password (const char*)`: Kata sandi yang akan diverifikasi.
 * 
 * Return: `boolean` - True jika login berhasil, false jika nama pengguna tidak ditemukan, kata sandi salah, pengguna sudah login, atau input tidak valid.
 */
boolean login(Hospital *hospital, Session *session, const char *username, const char *password);

#endif // LOGIN_H
