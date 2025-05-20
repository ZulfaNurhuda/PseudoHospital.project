#ifndef EXIT_H
#define EXIT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include "abstractDataTypes.h"
#include "utils.h"
#include "hospital.h"

/**
 * Nama Fungsi: `exitProgram`
 * 
 * Deskripsi: Mengakhiri program dengan membebaskan semua memori yang dialokasikan untuk struktur rumah sakit menggunakan free_hospital dan menampilkan pesan perpisahan dengan tabel ASCII sederhana serta kode warna ANSI. Program diakhiri dengan exit(0). Tidak mengatur ulang sesi. Dapat diakses oleh semua pengguna.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang akan dibebaskan.
 *   - `session (Session*)`: Pointer ke struktur sesi (tidak digunakan secara langsung).
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya membebaskan memori dan keluar dari program.
 */
void exitProgram(Hospital *hospital, Session *session);

#endif // EXIT_H