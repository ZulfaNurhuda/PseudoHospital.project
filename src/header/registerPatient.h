#ifndef REGISTER_PATIENT_H
#define REGISTER_PATIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"
#include "enigma.h"

/**
 * Nama Fungsi: `registerPatient`
 * 
 * Deskripsi: Mendaftarkan pasien baru dengan nama pengguna dan kata sandi. Memeriksa apakah pengguna belum login, nama pengguna unik, dan kata sandi valid. Mengenkripsi kata sandi menggunakan enigma_encrypt, lalu menambahkan pasien ke daftar pengguna dan pasien. Pasien baru memiliki 3 nyawa, 100 BananaRich, dan status awal lainnya.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit untuk menambahkan pasien.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa status login (harus belum login).
 *   - `username (const char*)`: Nama pengguna untuk pasien baru.
 *   - `password (const char*)`: Kata sandi untuk pasien baru, akan dienkripsi.
 * 
 * Return: `boolean` - True jika pendaftaran berhasil, false jika nama pengguna sudah ada, input tidak valid, kapasitas penuh, atau pengguna sudah login.
 */
boolean registerPatient(Hospital *hospital, Session *session, const char *username, const char *password);

#endif // REGISTER_PATIENT_H