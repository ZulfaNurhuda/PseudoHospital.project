# Berkas: `src/c/help.c`

Berkas ini bertanggung jawab untuk menampilkan informasi bantuan kepada pengguna. Informasi yang ditampilkan bersifat kontekstual, tergantung pada apakah pengguna sudah login dan peran mereka (Dokter, Pasien, atau Manajer).

## Fungsi Pembantu: `printFootnote`

### Tanda Tangan (Signature)
```c
static void printFootnote()
```

### Ringkasan
Fungsi pembantu statis yang mencetak catatan kaki generik untuk menu bantuan. Catatan kaki ini memberi tahu pengguna cara menggunakan aplikasi dengan memasukkan nama fungsi yang terdaftar dan memastikan input valid.

### Algoritma
1.  Mencetak baris baru dan "Footnote:".
2.  Mencetak: "Untuk menggunakan aplikasi, silahkan masukkan nama fungsi yang terdaftar".
3.  Mencetak: "Jangan lupa untuk memasukkan input yang valid".

## Fungsi Utama: `displayMenu`

### Tanda Tangan (Signature)
```c
void displayMenu(Session *session)
```

### Ringkasan
Menampilkan menu bantuan yang disesuaikan dengan status sesi pengguna saat ini (login atau tidak) dan peran mereka (Dokter, Pasien, Manajer). Fungsi ini mendaftar perintah dan tindakan yang tersedia untuk pengguna.

### Parameter
*   `Session *session`: Pointer ke struktur `Session`. Struktur ini berisi informasi tentang status login (`isLoggedIn`), nama pengguna (`username`), dan peran (`role`) pengguna saat ini.

### Algoritma
1.  **Cetak Header Utama**:
    *   Mencetak "=========== HELP ===========\n\n".
2.  **Tangani Pengguna yang Belum Login**:
    *   Jika `!session->isLoggedIn`:
        *   Mencetak: "Kamu belum login sebagai role apapun. Silahkan login terlebih dahulu.\n\n"
        *   Mendaftar perintah yang tersedia untuk pengguna yang belum login:
            *   "LOGIN: Masuk ke dalam akun yang sudah terdaftar"
            *   "REGISTER: Membuat akun baru"
3.  **Tangani Pengguna yang Sudah Login**:
    *   Lainnya (jika `session->isLoggedIn`):
        *   **Peran Dokter**:
            *   Jika `session->role == DOCTOR`:
                *   Mencetak sapaan yang dipersonalisasi: "Halo Dokter [session->username]. Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n"
                *   Mendaftar perintah khusus Dokter:
                    *   "LOGOUT: Keluar dari akun yang sedang digunakan"
                    *   "DIAGNOSIS: Melakukan diagnosis penyakit pasien berdasarkan kondisi tubuh pasien"
        *   **Peran Pasien**:
            *   Lain jika `session->role == PATIENT`:
                *   Mencetak sapaan yang dipersonalisasi: "Selamat datang, [session->username]. Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n"
                *   Mendaftar perintah khusus Pasien:
                    *   "LOGOUT: Keluar dari akun yang sedang digunakan"
                    *   "DAFTAR_CHECKUP: Mendaftarkan diri untuk pemeriksaan dokter"
        *   **Peran Manajer**:
            *   Lain jika `session->role == MANAGER`:
                *   Mencetak sapaan yang dipersonalisasi: "Halo Manager [session->username]. Kenapa kamu memanggil command HELP? Kan kamu manager, tapi yasudahlah kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n"
                *   Mendaftar perintah khusus Manajer:
                    *   "LOGOUT: Keluar dari akun yang sedang digunakan"
                    *   "TAMBAH_DOKTER: Mendaftarkan dokter baru ke sistem"
                    *   "ASSIGN_DOKTER: Menugaskan dokter ke ruangan tertentu"
                    *   "LIHAT_DENAH: Melihat denah rumah sakit"
                    *   "LIHAT_RUANGAN: Melihat detail ruangan tertentu"
                    *   "LIHAT_USER: Melihat daftar seluruh pengguna"
                    *   "LIHAT_PASIEN: Melihat daftar pasien"
                    *   "LIHAT_DOKTER: Melihat daftar dokter"
                    *   "CARI_USER: Mencari pengguna berdasarkan kriteria"
                    *   "CARI_PASIEN: Mencari pasien berdasarkan kriteria"
                    *   "CARI_DOKTER: Mencari dokter berdasarkan kriteria"
4.  **Cetak Catatan Kaki**:
    *   Memanggil `printFootnote()` untuk menampilkan instruksi generik di akhir menu bantuan.

### Nilai Kembali
*   `void`: Fungsi ini tidak mengembalikan nilai. Ia langsung mencetak output ke konsol.
