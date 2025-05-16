#ifndef TREAT_H
#define TREAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `treatPatient`
 * 
 * Deskripsi: Meresepkan obat otomatis untuk pasien berdasarkan penyakit yang didiagnosis (patient->disease). Mencocokkan penyakit dengan database penyakit untuk mendapatkan diseaseID, lalu mengambil obat dari daftar resep sesuai urutan konsumsi (doseOrder). Menambahkan obat ke daftar resep pasien. Peresepan hanya dilakukan jika pasien sudah didiagnosis, belum diobati, dan berada di antrian ruangan dokter. Menampilkan daftar obat dengan urutan konsumsi dalam format teks berwarna. Hanya dapat diakses oleh Dokter yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, dokter, antrian, penyakit, resep, dan obat.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `patientUsername (const char*)`: Nama pengguna pasien yang akan diobati.
 * 
 * Return: `boolean` - True jika peresepan berhasil, false jika input tidak valid, pasien tidak ditemukan, belum didiagnosis, sudah diobati, atau dokter tidak berwenang.
 */
boolean treatPatient(Hospital *hospital, Session *session, const char *patientUsername);

#endif // TREAT_H