# Berkas: `src/c/hospitalLayout.c`

Berkas ini bertanggung jawab untuk menampilkan tata letak ruangan rumah sakit dalam format grid dan menunjukkan informasi detail tentang ruangan tertentu.

## Fungsi: `displayLayout`

### Tanda Tangan (Signature)
```c
void displayLayout(Hospital *hospital, Session *session, boolean printHeaderFlag)
```

### Ringkasan
Menampilkan representasi visual dari tata letak rumah sakit sebagai grid ruangan. Setiap sel dalam grid menunjukkan kode ruangan. Fungsi ini dapat diakses oleh Manajer, Dokter, dan Pasien yang sudah login.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`, digunakan untuk mengakses `hospital->layout`.
*   `Session *session`: Pointer ke struktur `Session`, digunakan untuk otorisasi.
*   `boolean printHeaderFlag`: Sebuah flag yang menunjukkan apakah akan mencetak header "Denah Rumah Sakit". Berguna jika fungsi ini dipanggil sebagai bagian dari tampilan yang lebih besar.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah pointer `hospital` atau `session` adalah `NULL`. Jika ya, mencetak kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan kembali.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn`.
    *   Memeriksa apakah `session->role` adalah `MANAGER`, `DOCTOR`, atau `PATIENT`.
    *   Jika tidak diotorisasi, mencetak kesalahan ("Akses ditolak! Hanya Manajer, Dokter, atau Pasien yang dapat melihat denah.") dan kembali.
3.  **Pemeriksaan Keberadaan Tata Letak**:
    *   Memeriksa apakah `hospital->layout.rowEff <= 0` atau `hospital->layout.colEff <= 0`.
    *   Jika tata letak kosong atau tidak valid, mencetak kesalahan ("Denah rumah sakit kosong!") dan kembali.
4.  **Cetak Header (Opsional)**:
    *   Jika `printHeaderFlag` adalah `true`, memanggil `printHeader("Denah Rumah Sakit")`.
5.  **Tampilkan Grid**:
    *   Mencetak batas atas grid menggunakan karakter gambar kotak (┌, ─, ┬, ┐).
    *   Melakukan iterasi `i` dari `0` hingga `hospital->layout.rowEff - 1` (untuk setiap baris):
        *   Mencetak batas kiri baris (│).
        *   Melakukan iterasi `j` dari `0` hingga `hospital->layout.colEff - 1` (untuk setiap kolom dalam baris):
            *   Mencetak kode ruangan `hospital->layout.elements[i][j].code`, diformat selebar 3 karakter, ditengah, dan berwarna kuning (`COLOR_YELLOW " %-3s " COLOR_RESET "│"`).
        *   Mencetak baris baru.
        *   Jika bukan baris terakhir (`i != hospital->layout.rowEff - 1`):
            *   Mencetak garis pemisah horizontal menggunakan karakter gambar kotak (├, ─, ┼, ┤).
    *   Mencetak batas bawah grid menggunakan karakter gambar kotak (└, ─, ┴, ┘).

### Nilai Kembali
*   `void`: Fungsi ini langsung mencetak ke konsol.

---

## Fungsi: `displayRoomDetails`

### Tanda Tangan (Signature)
```c
void displayRoomDetails(Hospital *hospital, Session *session, const char *roomCode)
```

### Ringkasan
Menampilkan informasi detail tentang ruangan tertentu, yang diidentifikasi oleh `roomCode`-nya. Ini termasuk kapasitas ruangan, dokter yang ditugaskan (jika ada), dan daftar pasien yang saat ini berada di slot langsung ruangan. Dapat diakses oleh Manajer, Dokter, dan Pasien.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke struktur `Session` untuk otorisasi.
*   `const char *roomCode`: Kode ruangan yang detailnya akan ditampilkan (misalnya, "A1").

### Algoritma
1.  **Validasi Input (Pointer)**:
    *   Memeriksa apakah `hospital` atau `session` adalah `NULL`. Jika ya, mencetak kesalahan dan kembali.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan peran adalah `MANAGER`, `DOCTOR`, atau `PATIENT`.
    *   Jika tidak diotorisasi, mencetak kesalahan dan kembali.
3.  **Validasi Kode Ruangan (Dasar)**:
    *   Memeriksa apakah `roomCode` adalah `NULL` atau kosong. Jika ya, mencetak kesalahan ("Kode ruangan tidak valid!") dan kembali.
4.  **Validasi Format/Keberadaan Kode Ruangan**:
    *   Memanggil `isValidRoomCode(hospital, roomCode)`.
    *   Jika tidak valid, mencetak kesalahan ("Kode ruangan tidak valid! Contoh: A1") dan kembali.
5.  **Pemeriksaan Keberadaan Tata Letak**:
    *   Memeriksa apakah `hospital->layout.rowEff <= 0` atau `hospital->layout.colEff <= 0`.
    *   Jika tata letak kosong, mencetak kesalahan ("Denah rumah sakit kosong!") dan kembali.
6.  **Cari Ruangan**:
    *   Menginisialisasi `Room *room = NULL;`.
    *   Melakukan iterasi melalui `hospital->layout.elements` untuk menemukan ruangan tempat `strcmp(hospital->layout.elements[i][j].code, roomCode) == 0`.
    *   Jika ditemukan, pointer `room` diatur, dan loop berhenti.
    *   Jika `room` masih `NULL` setelah pencarian, mencetak kesalahan ("Ruangan [roomCode] tidak ditemukan!") dan kembali.
7.  **Cetak Header Detail Ruangan**:
    *   Membuat string header: "Detail Ruangan [roomCode]".
    *   Memanggil `printHeader()` dengan string ini.
8.  **Tampilkan Kapasitas**:
    *   Mengonversi `room->capacity` menjadi string `capacityStr` (menangani kapasitas 1, 2, atau 3+ digit).
    *   Mencetak baris tabel untuk "Kapasitas" dengan `capacityStr` menggunakan `printTableBorder` dan `printTableRow`.
9.  **Tampilkan Dokter yang Ditugaskan**:
    *   Menginisialisasi `char doctorStr[50];`.
    *   Jika `room->doctorId == -1` atau `hospital->doctors.nEff <= 0`, mengatur `doctorStr` menjadi "Tidak ada dokter".
    *   Lainnya, melakukan iterasi melalui `hospital->doctors.elements` untuk menemukan dokter dengan `id == room->doctorId`.
        *   Jika ditemukan, menyalin `doctor->username` ke `doctorStr`.
        *   Jika tidak ditemukan setelah loop (data tidak konsisten), mengatur `doctorStr` menjadi "Dokter tidak ditemukan".
    *   Mencetak baris tabel untuk "Dokter" dengan `doctorStr`.
10. **Tampilkan Pasien di Ruangan**:
    *   Mencetak baris sub-header tabel: {"No.", "Pasien dalam Ruangan"}.
    *   Jika `room->patientInRoom.nEff == 0` atau `hospital->patients.nEff <= 0`:
        *   Mencetak baris: {"0.", "Tidak ada pasien"}.
    *   Lainnya:
        *   Melakukan iterasi `i` dari `0` hingga `room->patientInRoom.nEff - 1` (untuk setiap ID pasien di slot ruangan):
            *   Menginisialisasi `found = false`.
            *   Melakukan iterasi `j` dari `0` hingga `hospital->patients.nEff - 1` (untuk menemukan detail pasien):
                *   Jika `hospital->patients.elements[j].id == room->patientInRoom.patientId[i]`:
                    *   Mengonversi `i + 1` menjadi `idStr` (untuk penomoran).
                    *   Mencetak baris tabel: `{idStr, hospital->patients.elements[j].username}`.
                    *   Mengatur `found = true` dan menghentikan loop dalam.
            *   Jika `!found` setelah loop dalam (ID pasien di slot ruangan tetapi tidak di daftar pasien utama - data tidak konsisten), mencetak baris: {"0.", "Tidak ada pasien"}.
    *   Mencetak batas bawah akhir untuk tabel daftar pasien.

### Nilai Kembali
*   `void`: Fungsi ini langsung mencetak ke konsol.
