#ifndef QUEUE_MANAGEMENT_H
#define QUEUE_MANAGEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"
#include "hospital.h"

/**
 * Nama Fungsi: `skipQueue`
 *
 * Deskripsi: Memungkinkan pasien melewati antrian ke posisi pertama dalam antrian ruangan yang ditentukan oleh patient->queueRoom di hospital->queues. Memperbarui queuePosition untuk semua pasien di antrian yang sama. Menampilkan tabel ASCII dengan informasi dokter, ruangan, posisi baru, dan total antrian, diikuti pesan sukses berwarna. Hanya dapat diakses oleh Pasien yang sudah login dan terdaftar dalam antrian.
 *
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, antrian, dan dokter.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *
 * Return: `boolean` - True jika antrian berhasil dilewati, false jika pasien tidak dalam antrian, sudah di posisi pertama, atau input tidak valid.
 */
boolean skipQueue(Hospital *hospital, Session *session);

/**
 * Nama Fungsi: `cancelQueue`
 *
 * Deskripsi: Membatalkan antrian pasien dari ruangan yang ditentukan oleh patient->queueRoom di hospital->queues. Menghapus pasien dari antrian, memperbarui queuePosition untuk pasien lain, dan mengosongkan patient->queueRoom serta patient->queuePosition. Menampilkan tabel ASCII dengan informasi dokter dan ruangan yang dibatalkan, diikuti pesan sukses berwarna. Hanya dapat diakses oleh Pasien yang sudah login dan terdaftar dalam antrian.
 *
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, antrian, dan dokter.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *
 * Return: `boolean` - True jika antrian berhasil dibatalkan, false jika pasien tidak dalam antrian atau input tidak valid.
 */
boolean cancelQueue(Hospital *hospital, Session *session);

#endif // QUEUE_MANAGEMENT_H