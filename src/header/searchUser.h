#ifndef SEARCH_USER_H
#define SEARCH_USER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `findUser`
 * 
 * Deskripsi: Mencari pengguna berdasarkan ID (pencocokan eksak) atau nama pengguna (pencocokan substring), menampilkan ID, nama pengguna, peran, dan penyakit (hanya untuk pasien, lainnya "-"). Hasil ditampilkan dalam tabel ASCII dengan pesan berwarna kuning jika tidak ditemukan. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar pengguna.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `query (const char*)`: Kueri pencarian (ID atau nama pengguna).
 *   - `byId (boolean)`: True jika mencari berdasarkan ID, false jika berdasarkan nama pengguna.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak hasil pencarian ke konsol.
 */
void findUser(Hospital *hospital, Session *session, const char *query, boolean byId);

/**
 * Nama Fungsi: `findPatient`
 * 
 * Deskripsi: Mencari pasien berdasarkan ID (pencocokan eksak) atau nama pengguna (pencocokan substring), menampilkan ID, nama pengguna, dan penyakit. Hasil ditampilkan dalam tabel ASCII dengan pesan berwarna kuning jika tidak ditemukan. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar pasien.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `query (const char*)`: Kueri pencarian (ID atau nama pengguna).
 *   - `byId (boolean)`: True jika mencari berdasarkan ID, false jika berdasarkan nama pengguna.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak hasil pencarian ke konsol.
 */
void findPatient(Hospital *hospital, Session *session, const char *query, boolean byId);

/**
 * Nama Fungsi: `findDoctor`
 * 
 * Deskripsi: Mencari dokter berdasarkan ID (pencocokan eksak) atau nama pengguna (pencocokan substring), menampilkan ID, nama pengguna, dan aura. Hasil ditampilkan dalam tabel ASCII dengan pesan berwarna kuning jika tidak ditemukan. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar dokter.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 *   - `query (const char*)`: Kueri pencarian (ID atau nama pengguna).
 *   - `byId (boolean)`: True jika mencari berdasarkan ID, false jika berdasarkan nama pengguna.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak hasil pencarian ke konsol.
 */
void findDoctor(Hospital *hospital, Session *session, const char *query, boolean byId);

#endif // SEARCH_USER_H