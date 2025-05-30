#ifndef VIEW_USERS_H
#define VIEW_USERS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `displayUsers`
 * 
 * Deskripsi: Menampilkan daftar semua pengguna (Manajer, Dokter, Pasien) dengan ID, nama pengguna, peran, dan penyakit (hanya untuk pasien, lainnya ditampilkan "-"). Data ditampilkan dalam tabel ASCII. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar pengguna.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak daftar pengguna ke konsol.
 */
void displayUsers(Hospital *hospital, Session *session, int choiceSortBy, int choiceSortOrder);

/**
 * Nama Fungsi: `displayPatients`
 * 
 * Deskripsi: Menampilkan daftar pasien dengan ID, nama pengguna, dan penyakit. Data ditampilkan dalam tabel ASCII. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar pasien.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak daftar pasien ke konsol.
 */
void displayPatients(Hospital *hospital, Session *session, int choiceSortBy, int choiceSortOrder);

/**
 * Nama Fungsi: `displayDoctors`
 * 
 * Deskripsi: Menampilkan daftar dokter dengan ID, nama pengguna, dan aura (jumlah pasien yang berhasil diobati). Data ditampilkan dalam tabel ASCII. Hanya dapat diakses oleh Manajer yang sudah login.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi daftar dokter.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak daftar dokter ke konsol.
 */
void displayDoctors(Hospital *hospital, Session *session, int choiceSortBy, int choiceSortOrder);

#endif // VIEW_USERS_H