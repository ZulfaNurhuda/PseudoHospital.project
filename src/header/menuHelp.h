#ifndef MENU_HELP_H
#define MENU_HELP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `displayMenu`
 * 
 * Deskripsi: Menampilkan daftar perintah yang tersedia dalam format tabel berdasarkan status login dan peran pengguna (Manajer, Dokter, Pasien, atau belum login). Perintah ditampilkan dengan warna kuning menggunakan kode warna ANSI.
 * 
 * Parameter:
 *   - `session (Session*)`: Pointer ke struktur sesi untuk menentukan status login dan peran pengguna.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak daftar perintah ke konsol.
 */
void displayMenu(Session *session);

/**
 * Nama Fungsi: `displayHelp`
 * 
 * Deskripsi: Menampilkan bantuan untuk perintah tertentu berdasarkan status login dan peran pengguna. Jika perintah tidak dikenal atau tidak tersedia, menampilkan pesan error.
 * 
 * Parameter:
 *   - `session (Session*)`: Pointer ke struktur sesi untuk menentukan status login dan peran pengguna.
 *   - `command (const char*)`: Nama perintah yang bantuannya akan ditampilkan.
 * 
 * Return: `void` - Tidak mengembalikan nilai, hanya mencetak bantuan atau pesan error ke konsol.
 */
void displayHelp(Session *session, const char *command);

#endif // MENU_HELP_H