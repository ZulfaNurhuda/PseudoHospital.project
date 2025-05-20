#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `myQueue`
 * 
 * Deskripsi: Menampilkan status antrian pasien saat ini, termasuk nama dokter, kode ruangan, posisi antrian (queuePosition), dan total antrian (jumlah pasien dalam antrian ruangan). Data diambil dari queueRoom, queuePosition, dan hospital->queues. Ditampilkan dalam tabel ASCII jika pasien dalam antrian, atau pesan berwarna jika tidak ada antrian. Hanya dapat diakses oleh Pasien yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien, dokter, dan antrian.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak status antrian ke konsol.
 */
void myQueue(Hospital *hospital, Session *session);

#endif // MY_QUEUE_H