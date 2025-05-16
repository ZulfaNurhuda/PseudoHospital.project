#ifndef ADD_DOCTOR_H
#define ADD_DOCTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"
#include "enigma.h"

/**
 * Nama Fungsi: `addDoctor`
 * 
 * Deskripsi: Menambahkan dokter baru ke sistem dengan nama pengguna, kata sandi, dan spesialisasi. Memeriksa nama pengguna unik, panjang kata sandi minimal 6 karakter, validitas input, kapasitas daftar, dan mengenkripsi kata sandi. Menampilkan pesan sukses atau gagal dengan kode warna. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit untuk menambahkan dokter.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `username (const char*)`: Nama pengguna untuk dokter baru.
 *   - `password (const char*)`: Kata sandi untuk dokter baru, akan dienkripsi.
 *   - `specialization (const char*)`: Spesialisasi dokter (misalnya, "Bedah").
 * 
 * Return: `boolean` - True jika penambahan berhasil, false jika input tidak valid, nama pengguna sudah ada, kapasitas penuh, atau enkripsi gagal.
 */
boolean addDoctor(Hospital *hospital, Session *session, const char *username, const char *password, const char *specialization);

/**
 * Nama Fungsi: `assignDoctor`
 * 
 * Deskripsi: Menugaskan dokter ke ruangan tertentu. Memeriksa validitas dokter, kode ruangan, dan ketersediaan ruangan. Jika dokter sudah ditugaskan ke ruangan lain, dokter dipindahkan. Menampilkan pesan sukses atau gagal dengan kode warna. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data dokter dan denah.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `username (const char*)`: Nama pengguna dokter yang akan ditugaskan.
 *   - `roomCode (const char*)`: Kode ruangan tujuan (misalnya, "A1").
 * 
 * Return: `boolean` - True jika penugasan berhasil, false jika input tidak valid, dokter atau ruangan tidak ditemukan, atau ruangan sudah ditempati.
 */
boolean assignDoctor(Hospital *hospital, Session *session, const char *username, const char *roomCode);

#endif // ADD_DOCTOR_H