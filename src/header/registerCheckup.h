#ifndef REGISTER_CHECKUP_H
#define REGISTER_CHECKUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `registerCheckup`
 * 
 * Deskripsi: Mendaftarkan pasien untuk pemeriksaan dengan dokter tertentu, memasukkan pasien ke antrian ruangan dokter, menyimpan data kesehatan ke field spesifik (suhu tubuh, tekanan darah, detak jantung, saturasi oksigen, gula darah, berat badan, tinggi badan, kolesterol, kolesterol LDL, trombosit), dan mengelola biaya checkup (BananaRich). Memvalidasi bahwa pasien tidak sedang dalam antrian, dokter tersedia, saldo cukup, dan data kesehatan valid. Memperbarui riwayat pengobatan dengan informasi dokter dan tanggal. Menampilkan hasil dalam tabel ASCII dengan posisi antrian dan pesan sukses berwarna. Hanya dapat diakses oleh Pasien yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, dokter, antrian, keuangan, dan riwayat pengobatan.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `doctorUsername (const char*)`: Nama pengguna dokter yang dipilih.
 *   - `healthData (float[])`: Array berisi 11 data kesehatan pasien (dalam urutan: suhu tubuh, tekanan darah sistolik, diastolik, detak jantung, saturasi oksigen, gula darah, berat badan, tinggi badan, kolesterol, kolesterol LDL, trombosit).
 * 
 * Return: `boolean` - True jika pendaftaran berhasil, false jika input tidak valid, dokter tidak ditemukan, pasien sudah dalam antrian, saldo tidak cukup, atau antrian penuh.
 */
boolean registerCheckup(Hospital *hospital, Session *session, const char *doctorUsername, float healthData[]);

#endif // REGISTER_CHECKUP_H