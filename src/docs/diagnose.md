# Berkas: `src/c/diagnose.c`

Berkas ini berisi logika untuk seorang dokter mendiagnosis pasien.

## Fungsi: `diagnosePatient`

### Tanda Tangan (Signature)
```c
boolean diagnosePatient(Hospital *hospital, Session *session)
```

### Ringkasan
Memungkinkan `DOCTOR` yang login untuk mendiagnosis pasien pertama dalam antrian ruangan yang ditugaskan padanya. Fungsi ini mencocokkan tanda-tanda vital pasien dan metrik lainnya dengan daftar penyakit yang diketahui. Jika ditemukan kecocokan, catatan pasien diperbarui dengan penyakit yang didiagnosis.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`. Ini digunakan untuk mengakses daftar dokter, daftar pasien, tata letak rumah sakit (ruangan), dan daftar penyakit.
*   `Session *session`: Pointer ke struktur `Session`, berisi detail pengguna (dokter) yang saat ini login, khususnya nama pengguna dan peran mereka.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah pointer `hospital` atau `session` adalah `NULL`. Jika ya, mencetak pesan kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan mengembalikan `false`.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi bahwa pengguna telah login (`session->isLoggedIn`).
    *   Memeriksa apakah peran pengguna adalah `DOCTOR` (`session->role != DOCTOR`).
    *   Jika otorisasi gagal, mencetak pesan kesalahan ("Akses ditolak! Hanya Dokter yang dapat mendiagnosis.") dan mengembalikan `false`.
3.  **Cari Indeks Dokter**:
    *   Menginisialisasi `doctorIdx = -1`.
    *   Melakukan iterasi melalui `hospital->doctors.elements` untuk menemukan dokter yang `username`-nya cocok dengan `session->username`.
    *   Jika ditemukan, `doctorIdx` diatur.
    *   Jika tidak ditemukan (`doctorIdx == -1`), mencetak pesan kesalahan ("Dokter tidak ditemukan dalam daftar!") dan mengembalikan `false`.
    *   Mendapatkan pointer ke dokter: `Doctor *doctor = &hospital->doctors.elements[doctorIdx];`.
4.  **Periksa Apakah Dokter Ditugaskan ke Ruangan**:
    *   Jika `doctor->room[0] == '\0'` (kode ruangan dokter adalah string kosong), mencetak pesan kesalahan ("Dokter tidak ditugaskan ke ruangan manapun!") dan mengembalikan `false`.
5.  **Cari Ruangan Dokter**:
    *   Menginisialisasi `Room *doctorRoom = NULL;`.
    *   Melakukan iterasi melalui `hospital->layout.elements` (array 2D ruangan) untuk menemukan ruangan yang `code`-nya cocok dengan `doctor->room`.
    *   Jika ditemukan, pointer `doctorRoom` diatur.
6.  **Periksa Apakah Ruangan Memiliki Pasien**:
    *   Jika `doctorRoom` adalah `NULL` (seharusnya tidak terjadi jika pemeriksaan sebelumnya lolos) atau `doctorRoom->patientInRoom.nEff == 0` (tidak ada pasien dalam antrian/slot langsung ruangan).
    *   Mencetak pesan kesalahan ("Ruangan dokter ini tidak memiliki pasien yang mengantri.") dan mengembalikan `false`.
7.  **Ambil Pasien Pertama di Ruangan**:
    *   `int patientId = doctorRoom->patientInRoom.patientId[0];` (mengambil ID pasien pertama dalam slot pasien langsung ruangan).
    *   Menginisialisasi `patientIdx = -1`.
    *   Melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien dengan `patientId`.
    *   Jika ditemukan, `patientIdx` diatur.
    *   Jika tidak ditemukan (`patientIdx == -1`), mencetak pesan kesalahan ("Pasien tidak ditemukan!") dan mengembalikan `false`.
    *   Mendapatkan pointer ke pasien: `Patient *patient = &hospital->patients.elements[patientIdx];`.
8.  **Periksa Status Diagnosis Pasien**:
    *   Jika `patient->diagnosedStatus` adalah `true`, mencetak pesan kesalahan ("Pasien sudah didiagnosa sebelumnya!") dan mengembalikan `false`.
9.  **Verifikasi Pasien Berada di Antrian yang Benar**:
    *   Jika `strcmp(patient->queueRoom, doctor->room) != 0` (ruangan antrian terdaftar pasien tidak cocok dengan ruangan dokter saat ini), mencetak pesan kesalahan ("Pasien tidak berada di antrian ruangan dokter!") dan mengembalikan `false`. Ini memastikan dokter mendiagnosis pasien yang secara resmi ada dalam antriannya.
10. **Lakukan Diagnosis**:
    *   Menginisialisasi `char diseaseStr[50] = "Tidak terdeteksi";`.
    *   Jika `hospital->diseases.nEff > 0` (ada penyakit yang ditentukan dalam sistem):
        *   Melakukan iterasi melalui setiap `Disease *d` dalam `hospital->diseases.elements`.
        *   Untuk setiap penyakit, memeriksa apakah tanda-tanda vital dan metrik `patient` berada dalam rentang yang ditentukan untuk penyakit tersebut. Ini termasuk:
            *   `bodyTemperature`
            *   `systolicBloodPressure`
            *   `diastolicBloodPressure`
            *   `heartRate`
            *   `oxygenSaturation`
            *   `bloodSugarLevel`
            *   `weight`
            *   `height`
            *   `cholesterolLevel`
            *   `platelets`
        *   Jika semua kondisi untuk suatu penyakit terpenuhi:
            *   `strcpy(diseaseStr, d->name);`
            *   `patient->disease[0] = '\0'; strcat(patient->disease, diseaseStr);` (Memperbarui penyakit pasien).
            *   `patient->diagnosedStatus = true;`
            *   Loop berhenti (penyakit pertama yang cocok dipilih).
11. **Hasil Diagnosis**:
    *   Jika `strcmp(diseaseStr, "Tidak terdeteksi") == 0` (tidak ada penyakit yang cocok):
        *   `patient->diagnosedStatus = true;` (tetap ditandai sebagai didiagnosis, tetapi tanpa penyakit spesifik).
        *   `strcpy(patient->disease, "Tidak terdeteksi");`
        *   Mencetak pesan sukses ("Pasien tidak terdiagnosis penyakit apapun!").
    *   Lainnya (penyakit ditemukan):
        *   Mencetak pesan yang menunjukkan diagnosis: "[patient->username] terdiagnosa penyakit [COLOR_YELLOW][diseaseStr][COLOR_RESET]!".
12. **Nilai Kembali**: Mengembalikan `true` yang menunjukkan proses diagnosis selesai (terlepas dari apakah penyakit ditemukan atau tidak).

### Nilai Kembali
*   `boolean`: `true` jika proses diagnosis selesai (bahkan jika tidak ada penyakit yang ditemukan), `false` jika ada pemeriksaan awal atau otorisasi yang gagal.
