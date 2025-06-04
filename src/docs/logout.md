# Berkas: `src/c/logout.c`

Berkas ini berisi logika untuk mengeluarkan pengguna dari sistem manajemen rumah sakit.

## Fungsi: `logout`

### Tanda Tangan (Signature)
```c
boolean logout(Session *session)
```

### Ringkasan
Mengeluarkan pengguna yang sedang aktif dengan mengatur ulang informasi sesi mereka. Fungsi ini membersihkan ID pengguna, nama pengguna, dan peran dari struktur `Session` dan mengatur status `isLoggedIn` menjadi `false`.

### Parameter
*   `Session *session`: Pointer ke struktur `Session` yang menyimpan data sesi pengguna saat ini. Struktur ini akan diubah untuk mencerminkan status logout.

### Algoritma
1.  **Validasi Input (Pointer Sesi)**:
    *   Memeriksa apakah `session` adalah `NULL`. Jika ya, mencetak kesalahan ("Sesi tidak valid!") dan mengembalikan `false`.
2.  **Pemeriksaan Status Login**:
    *   Memeriksa apakah `!session->isLoggedIn` (yaitu, jika pengguna saat ini tidak login).
    *   Jika pengguna tidak login, mencetak kesalahan ("Anda belum login! Silakan login terlebih dahulu.") dan mengembalikan `false`.
3.  **Simpan Nama Pengguna Sementara**:
    *   Mendeklarasikan `char tempUsername[50];`.
    *   Menyalin `session->username` saat ini ke `tempUsername` menggunakan `strcpy`. Ini dilakukan untuk menggunakan nama pengguna dalam pesan perpisahan setelah nama pengguna dibersihkan dari sesi.
4.  **Atur Ulang Informasi Sesi**:
    *   `session->userId = -1;` (Mengatur ID pengguna ke status tidak valid atau default).
    *   `session->username[0] = '\0';` (Membersihkan string nama pengguna).
    *   `session->role = -1;` (Mengatur peran ke status tidak valid atau default).
    *   `session->isLoggedIn = false;` (Mengatur status login menjadi false).
5.  **Buat dan Cetak Pesan Sukses**:
    *   Menginisialisasi `char successMessage[100];` menjadi string kosong.
    *   Menggabungkan string untuk membentuk pesan: "Logout berhasil! Sampai jumpa, [tempUsername]!".
    *   Memanggil `printSuccess(successMessage)` untuk menampilkan pesan perpisahan.
6.  **Nilai Kembali**: Mengembalikan `true` yang menunjukkan logout berhasil.

### Nilai Kembali
*   `boolean`: `true` jika logout berhasil, `false` jika tidak (misalnya, jika tidak ada pengguna yang login atau pointer sesi tidak valid).
