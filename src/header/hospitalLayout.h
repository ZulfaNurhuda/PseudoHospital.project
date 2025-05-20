#ifndef HOSPITAL_LAYOUT_H
#define HOSPITAL_LAYOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `displayLayout`
 * 
 * Deskripsi: Menampilkan denah rumah sakit dalam format matriks 2D (misalnya, A1, A2, ..., B1, B2, ...) menggunakan tabel ASCII. Kode ruangan ditampilkan dengan warna kuning menggunakan kode warna ANSI. Hanya dapat diakses oleh Manajer, Dokter, atau Pasien yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data denah.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses dan status login.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak denah ke konsol.
 */
void displayLayout(Hospital *hospital, Session *session);

/**
 * Nama Fungsi: `displayRoomDetails`
 * 
 * Deskripsi: Menampilkan detail ruangan tertentu, termasuk kapasitas, dokter yang bertugas, dan daftar pasien di dalam ruangan. Data ditampilkan dalam tabel ASCII dengan warna kuning untuk nama dokter dan pasien. Hanya dapat diakses oleh Manajer, Dokter, atau Pasien yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data ruangan.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses dan status login.
 *   - `roomCode (const char*)`: Kode ruangan yang akan ditampilkan (misalnya, "A1").
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak detail ruangan ke konsol.
 */
void displayRoomDetails(Hospital *hospital, Session *session, const char *roomCode);

#endif // HOSPITAL_LAYOUT_H