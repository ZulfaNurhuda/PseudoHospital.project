#ifndef VIEW_QUEUE_H
#define VIEW_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"
#include "hospitalLayout.h"

/**
 * Nama Fungsi: `displayQueue`
 * 
 * Deskripsi: Menampilkan denah rumah sakit diikuti status antrian untuk setiap ruangan yang memiliki dokter, termasuk kapasitas, dokter yang bertugas, pasien di dalam ruangan, dan pasien dalam antrian. Data ditampilkan dalam tabel ASCII dengan warna kuning untuk nama dokter dan pasien. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data denah, ruangan, dan antrian.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses dan status login.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak denah dan status antrian ke konsol.
 */
void displayQueue(Hospital *hospital, Session *session);

#endif // VIEW_QUEUE_H