#ifndef TAKE_MEDICATION_H
#define TAKE_MEDICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"
#include "hospital.h"

/**
 * Nama Fungsi: `takeMedication`
 * 
 * Deskripsi: Mengonsumsi obat berdasarkan medicationId yang diberikan. Memeriksa apakah obat sesuai dengan urutan resep dalam medicationsPrescribed menggunakan medicationsTaken (stack). Jika benar, obat ditambahkan ke medicationsTaken. Jika salah, nyawa pasien (life) berkurang sesuai aturan B04 (Dead or Alive). Jika nyawa habis (life <= 0), pasien dihapus dari sistem menggunakan deletePatient dan sesi direset. Menampilkan hasil dalam tabel ASCII dengan status konsumsi, sisa nyawa, dan pesan sukses/gagal berwarna. Hanya dapat diakses oleh Pasien yang sudah login dan memiliki resep (treatedStatus).
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, obat, dan resep.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `medicationId (int)`: ID obat yang akan dikonsumsi (harus positif).
 * 
 * Return: `boolean` - True jika konsumsi obat berhasil (urutan benar), false jika urutan salah, obat tidak valid, pasien tidak memiliki resep, atau input tidak valid.
 */
boolean takeMedication(Hospital *hospital, Session *session);

#endif // TAKE_MEDICATION_H