# Berkas: `src/c/viewPatientQueue.c`

Berkas ini berisi fungsi yang memungkinkan pasien yang login untuk melihat status spesifik mereka dalam antrian pemeriksaan dokter.

## Fungsi: `viewMySpecificQueueStatus`

### Tanda Tangan (Signature)
```c
void viewMySpecificQueueStatus(Hospital *hospital, Session *session)
```

### Ringkasan
Menampilkan status antrian saat ini untuk pasien yang login. Fungsi ini memeriksa apakah pasien sudah berada di ruangan dokter atau dalam antrian. Jika dalam antrian, ia menunjukkan nama dokter, kode ruangan, dan posisi pasien dalam antrian (misalnya, "3/5"). Fungsi ini hanya dapat diakses oleh pengguna dengan peran `PATIENT`.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke struktur `Session`, digunakan untuk otorisasi dan untuk mengidentifikasi pasien saat ini.

### Algoritma
1.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan `session->role == PATIENT`.
    *   Jika tidak diotorisasi, mencetak kesalahan ("Akses ditolak! Hanya Pasien yang dapat melihat status antrian.") dan kembali.
2.  **Cari Catatan Pasien**:
    *   Menginisialisasi `Patient *patient = NULL;`.
    *   Melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien yang `username`-nya cocok dengan `session->username`.
    *   Jika tidak ditemukan, mencetak kesalahan ("Pasien tidak ditemukan dalam daftar!") dan kembali.
3.  **Periksa Apakah Pasien Saat Ini Berada di Slot Ruangan (Bukan Hanya Antrian)**:
    *   Jika `patient->queueRoom[0] != '\0'` (pasien terkait dengan sebuah ruangan):
        *   Mencari `hospital->layout.elements` untuk ruangan yang cocok dengan `patient->queueRoom`.
        *   Jika ruangan ditemukan:
            *   Memeriksa apakah `patient->id` ada dalam array `room->patientInRoom.patientId`.
            *   Jika ya (`patientInRoom == true`), mencetak pesan ("Anda sedang berada di dalam ruangan dokter! Silakan menyelesaikan pemeriksaan.") dan kembali.
        *   Jika ruangan tidak ditemukan (`!roomFound`), mencetak kesalahan ("Ruangan yang terdaftar untuk Anda tidak ditemukan!") dan kembali.
4.  **Periksa Apakah Pasien Dalam Antrian**:
    *   Jika `patient->queueRoom[0] != '\0'` (pasien terdaftar dalam antrian untuk sebuah ruangan):
        *   **Cari Dokter untuk Ruangan Tersebut**:
            *   Menginisialisasi `Doctor *doctor = NULL;`.
            *   Melakukan iterasi melalui `hospital->doctors.elements` untuk menemukan dokter yang `room`-nya cocok dengan `patient->queueRoom`.
        *   **Jika Dokter Ditemukan**:
            *   **Cari Antrian**:
                *   Menginisialisasi `queueIdx = -1`.
                *   Melakukan iterasi melalui `hospital->queues.queues` (hingga `hospital->queues.nRooms`) untuk menemukan antrian yang cocok dengan `patient->queueRoom`.
                *   Jika `queueIdx == -1` (antrian tidak ditemukan), mencetak kesalahan ("Antrian untuk ruangan Anda tidak ditemukan atau Anda tidak ada di dalamnya!") dan kembali.
            *   `Queue *queueToCheck = &hospital->queues.queues[queueIdx];`.
            *   **Verifikasi Pasien dalam Struktur Antrian**:
                *   Melintasi senarai berantai `queueToCheck`. Jika `patient->id` ditemukan, atur `patientFoundInQueue = true`.
                *   Jika `!patientFoundInQueue`:
                    *   Mencetak pesan kesalahan yang menunjukkan pasien tidak ada dalam struktur antrian meskipun `patient->queueRoom` diatur (potensi ketidakkonsistenan data).
                    *   Mereset `patient->queueRoom[0] = '\0';` dan `patient->queuePosition = 0;`.
                    *   Kembali.
            *   **Tampilkan Status Antrian**:
                *   Mengonversi `patient->queuePosition` dan `queueSize(queueToCheck)` menjadi string.
                *   Memformatnya sebagai "posisi/ukuran" (misalnya, "1/5").
                *   Mencetak header "Status antrian Anda:".
                *   Mencetak tabel dengan baris untuk "Dokter" (nama pengguna dokter), "Ruangan" (ruangan dokter), dan "Posisi antrian" (string posisi/ukuran yang diformat).
                *   Kembali.
        *   **Lainnya (Dokter untuk Ruangan Tidak Ditemukan)**:
            *   Mencetak kesalahan ("Dokter untuk ruangan antrian Anda tidak ditemukan!").
            *   Kembali.
5.  **Pasien Tidak Dalam Antrian Apapun**:
    *   Jika eksekusi mencapai titik ini (yaitu, `patient->queueRoom[0] == '\0'`), itu berarti pasien tidak terdaftar dalam antrian apa pun.
    *   Mencetak kesalahan ("Anda belum terdaftar dalam antrian check-up! Silakan daftar terlebih dahulu dengan command DAFTAR_CHECKUP.").

### Nilai Kembali
*   `void`: Fungsi ini langsung mencetak output ke konsol.
