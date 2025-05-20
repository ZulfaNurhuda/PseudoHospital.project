#ifndef VIEW_LIFE_STATUS_H
#define VIEW_LIFE_STATUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `viewLifeStatus`
 * 
 * Deskripsi: Menampilkan status nyawa pasien (patient->life) dalam tabel ASCII, bersama dengan status hidup (life > 0) atau meninggal (life <= 0) menggunakan kode warna ANSI. Mendukung fitur B04 (Dead or Alive) dengan menunjukkan sisa nyawa. Hanya dapat diakses oleh Pasien yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data pasien.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya menampilkan tabel dengan nama pasien, sisa nyawa, dan status hidup/meninggal.
 */
void viewLifeStatus(Hospital *hospital, Session *session);

#endif // VIEW_LIFE_STATUS_H