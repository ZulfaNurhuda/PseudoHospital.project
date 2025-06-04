# Berkas: `src/c/canGoHome.c`

Berkas ini berisi logika untuk menentukan apakah seorang pasien memenuhi syarat untuk pulang, dan melakukan tindakan pembersihan yang diperlukan jika mereka memenuhi syarat.

## Fungsi: `canGoHome`

### Tanda Tangan (Signature)
```c
boolean canGoHome(Hospital *hospital, Session *session)
```

### Ringkasan
Memeriksa status pasien yang saat ini login untuk menentukan apakah mereka memenuhi kriteria untuk dipulangkan dari rumah sakit. Fungsi ini menampilkan status pasien dan, jika memenuhi syarat untuk dipulangkan, memperbarui data rumah sakit terkait (misalnya, aura dokter, antrian pasien, catatan pasien).

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`. Ini digunakan untuk mengakses daftar pasien, daftar dokter, riwayat perawatan, tata letak rumah sakit, dan antrian.
*   `Session *session`: Pointer ke struktur `Session`, berisi detail pengguna yang saat ini login, khususnya ID (`session->userId`) dan peran mereka.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah pointer `hospital` atau `session` adalah `NULL`. Jika ya, mencetak pesan kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan mengembalikan `false`.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi bahwa pengguna telah login (`session->isLoggedIn`).
    *   Memeriksa apakah peran pengguna adalah `PATIENT` (`session->role != PATIENT`).
    *   Jika otorisasi gagal, mencetak pesan kesalahan ("Akses ditolak! Hanya Pasien yang dapat memeriksa status pulang.") dan mengembalikan `false`.
3.  **Pemeriksaan Keberadaan Pasien (Umum)**:
    *   Memeriksa apakah ada pasien dalam sistem (`hospital->patients.nEff == 0`). Jika tidak, mencetak pesan kesalahan ("Tidak ada pasien terdaftar!") dan mengembalikan `false`.
4.  **Cari Pasien Saat Ini**:
    *   Menginisialisasi `Patient *patient = NULL;`.
    *   Melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien yang `id`-nya cocok dengan `session->userId`.
    *   Jika ditemukan, pointer `patient` diatur.
    *   Jika tidak ditemukan setelah loop (`!patient`), mencetak pesan kesalahan ("Pasien tidak ditemukan!") dan mengembalikan `false`.
5.  **Tentukan Status Pemulangan**:
    *   Menginisialisasi `statusStr` (pesan status), `descriptionStr` (pesan deskripsi), dan `canGoHomeStatus = false`.
    *   **Kondisi 1: Tidak ada penyakit.**
        *   Jika `strcmp(patient->disease, "Tidak terdeteksi") == 0` (pasien sehat atau sembuh).
        *   `statusStr = "Tidak Terjangkit Penyakit"`.
        *   `descriptionStr = "Anda telah selesai menjalani perawatan."`.
        *   `canGoHomeStatus = true`.
    *   **Kondisi 2: Belum didiagnosis.**
        *   Lain jika `!patient->diagnosedStatus`.
        *   `statusStr = "Belum Diagnosa"`.
        *   `descriptionStr = "Anda belum didiagnosa oleh dokter."`.
    *   **Kondisi 3: Belum dirawat (diberi resep).**
        *   Lain jika `!patient->treatedStatus`.
        *   `statusStr = "Belum Diberi Obat"`.
        *   `descriptionStr = "Anda belum diberikan resep obat."`.
    *   **Kondisi 4: Status pengobatan (diresepkan vs. diminum).**
        *   Lainnya (didiagnosis dan dirawat):
            *   `prescribedCount = patient->medicationsPrescribed.nEff;`
            *   `takenCount = patient->medicationsTaken.top + 1;` (mengasumsikan `top` adalah -1 untuk tumpukan kosong).
            *   **Sub-kondisi 4a: Tidak semua obat diminum.**
                *   Jika `takenCount < prescribedCount`.
                *   `statusStr = "Belum Minum Semua Obat"`.
                *   `descriptionStr = "Anda belum mengonsumsi semua obat yang telah diresepkan."`.
            *   **Sub-kondisi 4b: Terlalu banyak obat diminum (status kesalahan).**
                *   Lain jika `takenCount > prescribedCount`.
                *   `statusStr = "Urutan Obat Salah"` (menyesatkan, lebih seperti "Jumlah obat salah").
                *   `descriptionStr = "Anda mengonsumsi lebih banyak obat dari yang diresepkan."`.
            *   **Sub-kondisi 4c: Jumlah obat yang diminum benar, periksa urutan.**
                *   Lainnya (`takenCount == prescribedCount`):
                    *   `boolean correctOrder = true;`.
                    *   Iterasi dari `i = 0` hingga `takenCount - 1`. Jika `patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i]`, atur `correctOrder = false;` dan hentikan.
                    *   Jika `!correctOrder`:
                        *   `statusStr = "Urutan Obat Salah"`.
                        *   `descriptionStr = "Anda mengonsumsi obat dengan urutan yang salah."`.
                    *   Lainnya (`correctOrder` adalah true):
                        *   `statusStr = "Boleh Pulang"`.
                        *   `descriptionStr = "Anda telah selesai menjalani perawatan."`.
                        *   `canGoHomeStatus = true`.
6.  **Tampilkan Tabel Status**:
    *   Mencetak header "Status Pulang".
    *   Mendefinisikan lebar kolom tabel dan header (`"Status"`, `"Keterangan"`).
    *   Mencetak tabel menggunakan `printTableBorder()` dan `printTableRow()` dengan `statusStr` dan `descriptionStr`.
7.  **Jika `canGoHomeStatus` adalah `true` (Pasien bisa pulang)**:
    *   **Perbarui Aura Dokter**:
        *   Iterasi melalui `hospital->treatmentHistory.elements`.
        *   Jika catatan cocok dengan `patient->id`, temukan `doctorId`.
        *   Iterasi melalui `hospital->doctors.elements`. Jika `doctorId` cocok, naikkan `hospital->doctors.elements[j].aura++` dan hentikan.
    *   **Hapus Pasien dari Ruangan dan Kelola Antrian**:
        *   Jika `patient->queueRoom[0] != '\0'` (pasien berada dalam antrian ruangan):
            *   Temukan `Room *room` yang sesuai dengan `patient->queueRoom`.
            *   Jika `room` ditemukan:
                *   Hapus `patient->id` dari array `room->patientInRoom.patientId` dengan menggeser elemen dan mengurangi `room->patientInRoom.nEff`.
                *   Temukan `Queue *targetQueue` untuk `patient->queueRoom`.
                *   Jika `targetQueue` ada dan tidak kosong:
                    *   `dequeue(targetQueue, &nextPatientInfo.patientId)`.
                    *   Jika dequeue berhasil dan `room->patientInRoom` tidak penuh, tambahkan `nextPatientInfo.patientId` ke `room->patientInRoom`.
    *   **Reset Data Pasien**:
        *   `patient->queueRoom[0] = '\0';`
        *   `patient->queuePosition = 0;`
        *   `patient->diagnosedStatus = false;`
        *   `patient->treatedStatus = false;`
        *   `patient->life = 3;` (mereset nyawa/kesehatan)
        *   `strcpy(patient->disease, "");` (bersihkan penyakit)
        *   Reset `patient->medicationsTaken`: `top = -1`, `free(medicationId)`, `medicationId = NULL`.
        *   Reset `patient->medicationsPrescribed`: `nEff = 0`, `free(medicationId)`, `medicationId = NULL`.
    *   Mencetak pesan sukses ("Selamat, Anda boleh pulang!").
    *   Mengembalikan `true`.
8.  **Lainnya (Pasien tidak bisa pulang)**:
    *   Mencetak pesan kesalahan ("Anda belum boleh pulang!").
    *   Mengembalikan `false`.

### Nilai Kembali
*   `boolean`: `true` jika pasien memenuhi syarat untuk pulang dan pemrosesan berhasil, `false` jika tidak.
