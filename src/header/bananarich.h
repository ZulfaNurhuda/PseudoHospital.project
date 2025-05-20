#ifndef BANANARICH_H
#define BANANARICH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `viewWallet`
 * 
 * Deskripsi: Menampilkan jumlah BananaRich dalam dompet pengguna (Dokter atau Pasien). Data ditampilkan dalam tabel dengan warna kuning untuk jumlah.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data keuangan pengguna.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses (Dokter atau Pasien).
 * 
 * Return:
 *   - `void`: Tidak mengembalikan nilai, hanya mencetak jumlah BananaRich ke konsol.
 */
void viewWallet(Hospital *hospital, Session *session);

/**
 * Nama Fungsi: `viewFinancial`
 * 
 * Deskripsi: Menampilkan laporan keuangan rumah sakit, termasuk total BananaRich dari semua pengguna. Data ditampilkan dalam tabel.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data keuangan.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses (hanya Manajer).
 * 
 * Return:
 *   - `void`: Tidak mengembalikan nilai, hanya mencetak laporan keuangan ke konsol.
 */
void viewFinancial(Hospital *hospital, Session *session);

/**
 * Nama Fungsi: `gacha`
 * 
 * Deskripsi: Melakukan gacha untuk mendapatkan BananaRich dengan biaya tertentu. Mengurangi saldo pengguna dan menambahkan jumlah acak BananaRich ke dompet.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang berisi data keuangan pengguna.
 *   - `session (Session*)`: Pointer ke struktur sesi untuk memeriksa akses (Dokter atau Pasien).
 * 
 * Return:
 *   - `boolean`: Mengembalikan true jika gacha berhasil, false jika saldo tidak cukup atau sesi tidak valid.
 */
boolean gacha(Hospital *hospital, Session *session);

#endif // BANANARICH_H