#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include "abstractDataTypes.h"
#include "boolean.h"
#include "utils.h"

/**
 * Nama Fungsi: `initializeQueue`
 *
 * Deskripsi: Menginisialisasi antrian dengan mengatur front dan rear ke NULL serta ukuran ke 0. Fungsi ini memastikan antrian siap digunakan. Catatan: Fungsi ini mungkin redundan jika initHospital sudah menginisialisasi semua antrian, tetapi disediakan untuk kelengkapan.
 *
 * Parameter:
 *   - `q (Queue*)`: Pointer ke struktur antrian yang akan diinisialisasi.
 *   - `roomCode (const char*)`: Kode ruangan yang terkait dengan antrian.
 *
 * Return: `void` - Tidak mengembalikan nilai, hanya menginisialisasi antrian.
 */
void initializeQueue(Queue *q, const char* roomCode);

/**
 * Nama Fungsi: `isQueueEmpty`
 *
 * Deskripsi: Memeriksa apakah antrian kosong dengan memeriksa apakah pointer front bernilai NULL atau ukuran antrian adalah 0.
 *
 * Parameter:
 *   - `q (const Queue*)`: Pointer ke struktur antrian yang akan diperiksa.
 *
 * Return: `boolean` - Mengembalikan true jika antrian kosong, false jika tidak.
 */
boolean isQueueEmpty(const Queue *q);

/**
 * Nama Fungsi: `queueSize`
 *
 * Deskripsi: Mengembalikan jumlah elemen yang ada dalam antrian berdasarkan nilai ukuran yang tersimpan dalam struktur antrian.
 *
 * Parameter:
 *   - `q (const Queue*)`: Pointer ke struktur antrian yang ukurannya akan diperiksa.
 *
 * Return: `int` - Jumlah elemen dalam antrian.
 */
int queueSize(const Queue *q);

/**
 * Nama Fungsi: `enqueue`
 *
 * Deskripsi: Menambahkan elemen (patientID) ke bagian belakang antrian. Fungsi ini mengalokasikan memori untuk node baru dan mengembalikan true jika berhasil, false jika alokasi memori gagal.
 *
 * Parameter:
 *   - `q (Queue*)`: Pointer ke struktur antrian tempat elemen akan ditambahkan.
 *   - `patientID (int)`: ID pasien yang akan ditambahkan ke antrian.
 *
 * Return: `boolean` - Mengembalikan true jika penambahan berhasil, false jika gagal (misalnya, karena kegagalan alokasi memori).
 */
boolean enqueue(Queue *q, int patientID);

/**
 * Nama Fungsi: `dequeue`
 *
 * Deskripsi: Menghapus elemen dari depan antrian dan menyimpan patientID dari elemen tersebut. Mengembalikan true jika berhasil, false jika antrian kosong.
 *
 * Parameter:
 *   - `q (Queue*)`: Pointer ke struktur antrian tempat elemen akan dihapus.
 *   - `patientID (int*)`: Pointer ke variabel tempat ID pasien dari elemen yang dihapus akan disimpan.
 *
 * Return: `boolean` - Mengembalikan true jika penghapusan berhasil, false jika antrian kosong.
 */
boolean dequeue(Queue *q, int *patientID);

/**
 * Nama Fungsi: `peekQueue`
 *
 * Deskripsi: Melihat elemen di depan antrian tanpa menghapusnya dan menyimpan patientID dari elemen tersebut. Mengembalikan true jika berhasil, false jika antrian kosong.
 *
 * Parameter:
 *   - `q (const Queue*)`: Pointer ke struktur antrian yang elemen depannya akan dilihat.
 *   - `patientID (int*)`: Pointer ke variabel tempat ID pasien dari elemen depan akan disimpan.
 *
 * Return: `boolean` - Mengembalikan true jika operasi berhasil, false jika antrian kosong.
 */
boolean peekQueue(const Queue *q, int *patientID);

#endif // MY_QUEUE_H
