#ifndef TAKE_ANTIDOTE_H
#define TAKE_ANTIDOTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDatatypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `takeAntidote`
 * 
 * Deskripsi: Mengonsumsi penawar untuk membatalkan obat terakhir yang diminum dari medicationsTaken (stack), menambah nyawa pasien (life) hingga maksimum 3 sesuai aturan B04 (Dead or Alive), dan menampilkan sisa nyawa dalam tabel ASCII. Memerlukan pasien memiliki resep (treatedStatus) dan telah mengonsumsi obat (medicationsTaken.top >= 0). Menampilkan nama obat dari medications dan pesan sukses berwarna. Hanya dapat diakses oleh Pasien yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien dan obat.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 * 
 * Return: `boolean` - True jika penawar berhasil digunakan, false jika tidak ada obat yang bisa dibatalkan, pasien tidak memiliki resep, atau input tidak valid.
 */
boolean takeAntidote(Hospital *hospital, Session *session);

#endif // TAKE_ANTIDOTE_H