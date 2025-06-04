# Berkas: `src/c/viewQueue.c`

Berkas ini berisi fungsi untuk menampilkan status semua antrian pasien di seluruh ruangan yang memiliki dokter yang ditugaskan. Ini adalah fungsi khusus manajer.

## Fungsi: `displayQueue`

### Tanda Tangan (Signature)
```c
void displayQueue(Hospital *hospital, Session *session)
```

### Ringkasan
Menampilkan gambaran komprehensif dari semua antrian pasien di rumah sakit. Fungsi ini pertama-tama menunjukkan tata letak rumah sakit dan kemudian, untuk setiap ruangan yang memiliki dokter yang ditugaskan, ia merinci kapasitas ruangan, dokter yang ditugaskan, pasien yang saat ini menempati slot langsung ruangan, dan daftar pasien dalam antrian tunggu untuk ruangan tersebut. Fungsi ini hanya dapat diakses oleh pengguna dengan peran `MANAGER`.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke struktur `Session`, digunakan untuk otorisasi.

### Algoritma
1.  **Validasi Input (Pointer)**:
    *   Memeriksa apakah `hospital` atau `session` adalah `NULL`. Jika ya, mencetak kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan kembali.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan `session->role == MANAGER`.
    *   Jika tidak diotorisasi, mencetak kesalahan ("Akses ditolak! Hanya Manajer yang dapat melihat antrian.") dan kembali.
3.  **Pemeriksaan Keberadaan Tata Letak**:
    *   Memeriksa apakah `hospital->layout.rowEff <= 0` atau `hospital->layout.colEff <= 0`.
    *   Jika tata letak kosong, mencetak kesalahan ("Denah rumah sakit kosong!") dan kembali.
4.  **Tampilkan Header Awal dan Tata Letak**:
    *   Memanggil `printHeader("Status Antrian")`.
    *   Memanggil `displayLayout(hospital, session, false)` untuk menunjukkan grid ruangan rumah sakit (tanpa header tambahan dari `displayLayout` itu sendiri).
5.  **Iterasi Melalui Ruangan**:
    *   Menginisialisasi `boolean hasRoomsWithDoctor = false;`.
    *   Loop luar untuk baris (`i` dari `0` hingga `hospital->layout.rowEff - 1`).
    *   Loop dalam untuk kolom (`j` dari `0` hingga `hospital->layout.colEff - 1`).
        *   Mendapatkan `Room *room = &hospital->layout.elements[i][j];`.
        *   **Jika Ruangan Memiliki Dokter (`room->doctorId != -1`)**:
            *   Mengatur `hasRoomsWithDoctor = true;`.
            *   Mencetak sub-header untuk ruangan: "Antrian Ruangan [room->code]".
            *   **Tampilkan Kapasitas Ruangan dan Dokter**:
                *   Mengonversi `room->capacity` menjadi `capacityStr`. Mencetak kesalahan jika konversi gagal.
                *   Mencetak tabel untuk "Kapasitas" dan "Dokter".
                *   Mencari `doctorName` dari `hospital->doctors.elements` yang cocok dengan `room->doctorId`.
                *   Jika `doctorName` adalah `NULL` atau tidak ada dokter, mencetak "Tidak ada dokter". Jika tidak, sertakan nama dokter dalam tabel.
            *   **Tampilkan Pasien di Slot Ruangan**:
                *   Mencetak "Pasien di dalam ruangan:".
                *   Jika `room->patientInRoom.nEff == 0` atau tidak ada pasien dalam sistem, mencetak "Tidak ada pasien".
                *   Lainnya, melakukan iterasi `k` dari `0` hingga `room->patientInRoom.nEff - 1`:
                    *   Mencari nama pengguna pasien dari `hospital->patients.elements` yang cocok dengan `room->patientInRoom.patientId[k]`.
                    *   Mencetak "%d. %s" (misalnya, "1. PatientName").
                    *   Jika ID pasien tidak ditemukan di daftar utama, mencetak "Pasien tidak ditemukan".
            *   **Tampilkan Pasien dalam Antrian**:
                *   Mencetak "Pasien di antrian:".
                *   Menginisialisasi `boolean foundQueue = false;`.
                *   Melakukan iterasi `k` dari `0` hingga `hospital->queues.nRooms - 1` untuk menemukan antrian untuk `room->code`.
                    *   Jika antrian ditemukan:
                        *   Mengatur `foundQueue = true;`.
                        *   `Queue *currentQueue = &hospital->queues.queues[k];`.
                        *   Jika `isQueueEmpty(currentQueue)` atau tidak ada pasien dalam sistem, mencetak "Tidak ada pasien di antrian".
                        *   Lainnya, melakukan iterasi melalui `currentQueue` (menggunakan `QueueNode *currentNode`):
                            *   Mencari nama pengguna pasien dari `hospital->patients.elements` yang cocok dengan `currentNode->info.patientId`.
                            *   Mencetak "%d. %s" (misalnya, "1. QueuedPatientName").
                            *   Jika ID pasien dari antrian tidak ada di daftar utama, mencetak kesalahan tentang pasien yang hilang.
                            *   Menaikkan `position`.
                        *   Hentikan dari loop pencarian antrian.
                *   Jika `!foundQueue` setelah loop, mencetak "Tidak ada pasien di antrian".
6.  **Tidak Ada Ruangan dengan Dokter**:
    *   Jika `!hasRoomsWithDoctor` setelah melakukan iterasi melalui semua ruangan, mencetak "Tidak ada ruangan dengan dokter saat ini.".

### Nilai Kembali
*   `void`: Fungsi ini langsung mencetak output ke konsol.
