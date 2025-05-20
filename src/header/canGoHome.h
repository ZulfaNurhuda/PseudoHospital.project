#ifndef CAN_GO_HOME_H
#define CAN_GO_HOME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `canGoHome`
 * 
 * Deskripsi: Memeriksa apakah pasien dapat pulang berdasarkan statusnya: belum didiagnosis, belum diberi obat, salah urutan konsumsi obat, atau sudah boleh pulang. Pemeriksaan menggunakan diagnosedStatus, treatedStatus, dan perbandingan medicationsTaken dengan medicationsPrescribed. Jika boleh pulang, meningkatkan aura dokter berdasarkan treatmentHistory dan menghapus pasien dari antrian (HospitalQueueList). Menampilkan hasil dalam tabel ASCII dengan pesan sukses/gagal berwarna. Hanya dapat diakses oleh Pasien yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, dokter, antrian, dan riwayat pengobatan.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 * 
 * Return: `boolean` - True jika pasien boleh pulang, false jika belum memenuhi syarat atau sesi tidak valid.
 */
boolean canGoHome(Hospital *hospital, Session *session);

#endif // CAN_GO_HOME_H