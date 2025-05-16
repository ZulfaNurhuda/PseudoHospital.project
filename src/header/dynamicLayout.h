#ifndef DYNAMIC_LAYOUT_H
#define DYNAMIC_LAYOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `changeLayout`
 * 
 * Deskripsi: Mengubah ukuran denah rumah sakit menjadi newRows x newCols. Gagal jika ada dokter di ruangan yang akan dihapus.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data denah.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses (hanya Manajer).
 *   - `newRows (int)`: Jumlah baris baru untuk denah.
 *   - `newCols (int)`: Jumlah kolom baru untuk denah.
 * 
 * Return: `boolean` - True jika perubahan berhasil, false jika gagal (akses ditolak, ukuran tidak valid, atau ada dokter).
 */
boolean changeLayout(Hospital *hospital, Session *session, int newRows, int newCols);

/**
 * Nama Fungsi: `moveDoctor`
 * 
 * Deskripsi: Memindahkan dokter dengan username tertentu ke ruangan baru. Gagal jika ruangan sudah ditempati atau tidak valid.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data denah dan dokter.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses (hanya Manajer).
 *   - `username (const char*)`: Username dokter yang akan dipindahkan.
 *   - `newRoomCode (const char*)`: Kode ruangan tujuan (misalnya, "A1").
 * 
 * Return: `boolean` - True jika pemindahan berhasil, false jika gagal (akses ditolak, dokter tidak ditemukan, atau ruangan tidak valid).
 */
boolean moveDoctor(Hospital *hospital, Session *session, const char *username, const char *newRoomCode);

#endif // DYNAMIC_LAYOUT_H