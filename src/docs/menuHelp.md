# Berkas: `src/c/menuHelp.c`

Berkas ini menyediakan fungsionalitas untuk menampilkan menu perintah yang tersedia dan informasi bantuan detail untuk perintah tertentu, yang disesuaikan dengan status sesi pengguna saat ini (login atau tidak) dan peran mereka (Dokter, Manajer, atau Pasien).

## Fungsi Pembantu: `normalizeCommand`

### Tanda Tangan (Signature)
```c
static void normalizeCommand(char *command)
```

### Ringkasan
Fungsi pembantu statis yang menormalkan string perintah. Fungsi ini mengubah semua karakter alfabet menjadi huruf besar dan menghapus semua karakter garis bawah (`_`). Hal ini memungkinkan input perintah yang lebih fleksibel dari pengguna (misalnya, "lihat_denah", "LihatDenah", "LIHATDENAH" semuanya akan dinormalkan menjadi "LIHATDENAH").

### Parameter
*   `char *command`: String perintah yang akan dinormalkan di tempat.

### Algoritma
1.  Menginisialisasi dua variabel indeks, `i` (untuk iterasi melalui perintah asli) dan `j` (untuk menempatkan karakter dalam perintah yang dinormalkan).
2.  Melakukan iterasi melalui string `command` input karakter per karakter menggunakan indeks `i`:
    *   Jika `command[i]` bukan garis bawah (`_`):
        *   Mengubah `command[i]` menjadi huruf besar jika merupakan huruf kecil (`'a'` hingga `'z'`).
        *   Menetapkan karakter yang diproses ke `command[j]`.
        *   Menaikkan `j`.
    *   Menaikkan `i`.
3.  Setelah loop, menempatkan terminator null di `command[j]` untuk menandai akhir string yang dinormalkan.

## Fungsi Pembantu: `printFootnote`

### Tanda Tangan (Signature)
```c
static void printFootnote()
```

### Ringkasan
Fungsi pembantu statis yang mencetak catatan kaki generik, identik dengan yang ada di `help.c`. Catatan kaki ini memberi tahu pengguna cara berinteraksi dengan aplikasi.

### Algoritma
1.  Mencetak baris baru dan "Footnote:".
2.  Mencetak: "Untuk menggunakan aplikasi, silahkan masukkan nama fungsi yang terdaftar".
3.  Mencetak: "Jangan lupa untuk memasukkan input yang valid".

## Fungsi: `displayMenu`

### Tanda Tangan (Signature)
```c
void displayMenu(Session *session)
```

### Ringkasan
Menampilkan daftar perintah yang tersedia untuk pengguna. Daftar ini disesuaikan berdasarkan apakah pengguna sudah login dan peran spesifik mereka (Manajer, Dokter, atau Pasien). Perintah disajikan dalam format tabel sederhana.

### Parameter
*   `Session *session`: Pointer ke struktur `Session`, yang berisi status login dan peran pengguna saat ini.

### Algoritma
1.  **Validasi Sesi**:
    *   Jika `session == NULL`, mencetak kesalahan ("Sesi tidak valid!") dan kembali.
2.  **Cetak Header**:
    *   Memanggil `printHeader("Daftar Perintah")`.
3.  **Inisialisasi Array Menu**:
    *   `const char *menu[20];` (array untuk menyimpan string perintah).
    *   `int count = 0;` (penghitung untuk perintah).
    *   `int widths[] = {20};` (lebar kolom untuk tabel).
4.  **Isi Menu Berdasarkan Status Sesi dan Peran**:
    *   **Belum Login (`!session->isLoggedIn`)**:
        *   Mencetak pesan: "Anda belum login. Perintah yang tersedia:".
        *   Menambahkan "LOGIN", "REGISTER_PASIEN", "LUPA_PASSWORD", "MENU", "HELP", "EXIT" ke array `menu`. (Catatan: "EXIT" terdaftar dua kali, mungkin salah ketik di sumber).
    *   **Sudah Login (`session->isLoggedIn`)**:
        *   Menggunakan pernyataan `switch` berdasarkan `session->role`:
            *   **DOCTOR**:
                *   Mencetak pesan yang dipersonalisasi.
                *   Menambahkan perintah: "LOGOUT", "MENU", "HELP", "LIHAT_DENAH", "LIHAT_RUANGAN", "DIAGNOSIS", "NGOBATIN", "LIHAT_DOMPET", "BATALKAN_ANTRIAN", "EXIT".
            *   **MANAGER**:
                *   Mencetak pesan yang dipersonalisasi.
                *   Menambahkan perintah: "LOGOUT", "MENU", "HELP", "LIHAT_DENAH", "LIHAT_RUANGAN", "LIHAT_USER", "CARI_USER", "CARI_PASIEN", "CARI_DOKTER", "LIHAT_ANTRIAN", "TAMBAH_DOKTER", "UBAH_DENAH", "PINDAH_DOKTER", "LIHAT_FINANSIAL", "LEWATI_ANTRIAN", "BATALKAN_ANTRIAN", "EXIT".
            *   **PATIENT**:
                *   Mencetak pesan yang dipersonalisasi.
                *   Menambahkan perintah: "LOGOUT", "MENU", "HELP", "LIHAT_DENAH", "LIHAT_RUANGAN", "PULANG_DOK", "DAFTAR_CHECKUP", "ANTRIAN_SAYA", "MINUM_OBAT", "MINUM_PENAWAR", "LIHAT_DOMPET", "LIHAT_NYAWA", "GACHA", "LEWATI_ANTRIAN", "BATALKAN_ANTRIAN", "EXIT".
5.  **Cetak Tabel Menu**:
    *   Memanggil `printTableBorder` untuk batas atas.
    *   Melakukan iterasi dari `i = 0` hingga `count - 1`, memanggil `printTableRow(&menu[i], widths, 1)` untuk setiap perintah.
    *   Memanggil `printTableBorder` untuk batas bawah.
6.  **Cetak Catatan Kaki**:
    *   Memanggil `printFootnote()`.

### Nilai Kembali
*   `void`: Fungsi ini mencetak langsung ke konsol.

## Fungsi: `displayHelp`

### Tanda Tangan (Signature)
```c
void displayHelp(Session *session, const char *command)
```

### Ringkasan
Menampilkan informasi bantuan detail untuk perintah *spesifik* yang diberikan oleh pengguna. Teks bantuan disesuaikan dengan peran pengguna saat ini dan apakah mereka sudah login. Fungsi ini menormalkan perintah input sebelum mencocokkan.

### Parameter
*   `Session *session`: Pointer ke `Session` pengguna saat ini.
*   `const char *command`: String perintah yang bantuannya diminta.

### Algoritma
1.  **Validasi Input**:
    *   Jika `session == NULL`, mencetak kesalahan ("Sesi tidak valid!") dan kembali.
    *   Jika `command == NULL` atau `command[0] == '\0'`, mencetak kesalahan ("Perintah tidak valid!") dan kembali.
2.  **Normalisasi Perintah**:
    *   Menyalin `command` ke `normalCommand`.
    *   Memanggil `normalizeCommand(normalCommand)`.
3.  **Cetak Header**:
    *   Memanggil `printHeader("Bantuan Perintah")`.
    *   Mencetak baris baru.
4.  **Tangani Belum Login**:
    *   Jika `!session->isLoggedIn`:
        *   Mencetak "Anda belum login! Silakan login terlebih dahulu.\n".
        *   Mencetak "Bantuan untuk perintah '[command]':\n" dan garis pemisah.
        *   Menggunakan pernyataan `if-else if` pada `normalCommand` untuk menampilkan bantuan untuk: "LOGIN", "REGISTERPATIENT", "LUPAPASSWORD", "MENU", "HELP", "EXIT".
        *   Jika perintah tidak dikenali, mencetak kesalahan.
5.  **Tangani Sudah Login**:
    *   Lainnya (pengguna sudah login):
        *   Mencetak "Anda login sebagai [RoleName].\n".
        *   Mencetak "Bantuan untuk perintah '[command]':\n" dan garis pemisah.
        *   Menggunakan pernyataan `switch` berdasarkan `session->role`:
            *   **MANAGER**: Menyediakan deskripsi detail untuk perintah seperti "LOGOUT", "MENU", "HELP", "LIHATDENAH", "LIHATUSER", "CARIUSER", "LIHATANTRIAN", "TAMBAHDOKTER", "UBAHDENAH", "PINDAHDOKTER", "LIHATFINANSIAL", "EXIT". Jika perintah tidak diketahui untuk Manajer, mencetak kesalahan.
            *   **DOCTOR**: Menyediakan deskripsi detail untuk "LOGOUT", "MENU", "HELP", "LIHATDENAH", "DIAGNOSIS", "NGOBATIN", "LIHATDOMPET", "EXIT". Jika perintah tidak diketahui untuk Dokter, mencetak kesalahan.
            *   **PATIENT**: Menyediakan deskripsi detail untuk "LOGOUT", "MENU", "HELP", "LIHATDENAH", "PULANGDOK", "DAFTARCHECKUP", "ANTRIANSAYA", "MINUMOBAT", "MINUMPENAWAR", "LIHATDOMPET", "GACHA", "LEWATIANTRIAN", "BATALKANANTRIAN", "EXIT". Jika perintah tidak diketahui untuk Pasien, mencetak kesalahan.
            *   **Default (Peran Tidak Dikenal)**: Mencetak kesalahan "Peran pengguna tidak dikenal!".

### Nilai Kembali
*   `void`: Fungsi ini mencetak langsung ke konsol.
