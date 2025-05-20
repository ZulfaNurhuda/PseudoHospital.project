#ifndef DIAGNOSIS_H
#define DIAGNOSIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `diagnosePatient`
 * 
 * Deskripsi: Melakukan diagnosis otomatis untuk pasien berdasarkan data kesehatan (suhu tubuh, tekanan darah, detak jantung, gula darah, berat badan, tinggi badan, kolesterol, trombosit) dibandingkan dengan database penyakit. Menyimpan hasil diagnosis sebagai nama penyakit di patient->disease. Diagnosis hanya dilakukan jika pasien belum didiagnosis dan berada di ruangan dokter. Hasil ditampilkan dalam tabel ASCII dengan pesan sukses/gagal berwarna. Hanya dapat diakses oleh Dokter yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, dokter, denah, dan penyakit.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `patientUsername (const char*)`: Nama pengguna pasien yang akan didiagnosis.
 * 
 * Return: `boolean` - True jika diagnosis berhasil, false jika input tidak valid, pasien tidak ditemukan, pasien sudah didiagnosis, atau dokter tidak berwenang.
 */
boolean diagnosePatient(Hospital *hospital, Session *session, const char *patientUsername);

#endif // DIAGNOSIS_H