#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `initHospital`
 * 
 * Deskripsi: Menginisialisasi struktur rumah sakit dengan alokasi memori untuk semua komponen seperti pengguna, pasien, dokter, denah, antrian, penyakit, obat, resep, dan riwayat perawatan. Mengatur nilai awal seperti jumlah elemen efektif ke 0 dan denah ke ukuran tertentu.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang akan diinisialisasi.
 *   - `userCapacity (int)`: Kapasitas awal untuk daftar pengguna.
 *   - `patientCapacity (int)`: Kapasitas awal untuk daftar pasien.
 *   - `doctorCapacity (int)`: Kapasitas awal untuk daftar dokter.
 *   - `roomRows (int)`: Jumlah baris untuk denah rumah sakit.
 *   - `roomCols (int)`: Jumlah kolom untuk denah rumah sakit.
 * 
 * Return: `void` - Tidak mengembalikan nilai, tetapi mengatur struktur hospital.
 */
void initHospital(Hospital *hospital, int userCapacity, int patientCapacity, int doctorCapacity, int roomRows, int roomCols);

/**
 * Nama Fungsi: `freeHospital`
 * 
 * Deskripsi: Membebaskan semua memori yang dialokasikan untuk struktur rumah sakit, termasuk array pengguna, pasien, dokter, denah, antrian, penyakit, obat, resep, dan riwayat perawatan. Memastikan tidak ada kebocoran memori.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit yang akan dibebaskan.
 * 
 * Return: `void` - Tidak mengembalikan nilai, tetapi membebaskan semua memori.
 */
void freeHospital(Hospital *hospital);

/**
 * Nama Fungsi: `deletePatient`
 * 
 * Deskripsi: Menghapus pasien dari sistem berdasarkan ID-nya, termasuk dari daftar pasien, pengguna, antrian, ruangan, dan riwayat perawatan. Digunakan saat pasien kehabisan nyawa (0 nyawa) atau dalam kondisi kritis.
 * 
 * Parameter:
 *   - `hospital (Hospital*)`: Pointer ke struktur rumah sakit tempat pasien berada.
 *   - `patientId (int)`: ID pasien yang akan dihapus.
 * 
 * Return: `boolean` - True jika penghapusan berhasil, false jika pasien tidak ditemukan.
 */
boolean deletePatient(Hospital *hospital, int patientId);

#endif // HOSPITAL_H