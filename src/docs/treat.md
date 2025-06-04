# Berkas: `src/c/treat.c`

Berkas ini berisi logika bagi dokter untuk "merawat" pasien. Merawat, dalam konteks ini, berarti mencari set obat yang diresepkan untuk penyakit yang didiagnosis pasien dan menetapkan resep ini (daftar ID obat secara berurutan) ke catatan pasien.

## Fungsi: `treatPatient`

### Tanda Tangan (Signature)
```c
boolean treatPatient(Hospital *hospital, Session *session)
```

### Ringkasan
Memungkinkan `DOCTOR` yang login untuk meresepkan obat kepada pasien pertama dalam antrian ruangan yang ditugaskan padanya. Fungsi ini memeriksa berbagai kondisi: apakah pasien telah didiagnosis, apakah mereka belum dirawat, dan apakah mereka memang berada dalam antrian ruangan dokter. Kemudian, fungsi ini menemukan obat-obatan yang terkait dengan penyakit yang didiagnosis pasien dari daftar resep utama rumah sakit, mengurutkan obat-obatan ini berdasarkan `doseOrder`, dan memperbarui daftar `medicationsPrescribed` pasien.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke `Session` pengguna saat ini (harus seorang DOCTOR).

### Algoritma
1.  **Validasi Input & Otorisasi**:
    *   Memeriksa apakah `hospital` atau `session` adalah `NULL`. Mencetak kesalahan dan mengembalikan `false`.
    *   Memverifikasi `session->isLoggedIn` dan `session->role == DOCTOR`. Mencetak kesalahan dan mengembalikan `false` jika tidak.
2.  **Cari Catatan Dokter**:
    *   Menemukan `doctorIdx` dan `Doctor *doctor` menggunakan `session->username`. Mencetak kesalahan dan mengembalikan `false` jika tidak ditemukan.
3.  **Pemeriksaan Penugasan Ruangan Dokter**:
    *   Jika `doctor->room[0] == '\0'` (dokter tidak ditugaskan ke ruangan), mencetak kesalahan dan mengembalikan `false`.
4.  **Cari Ruangan Dokter & Periksa Pasien**:
    *   Mencari `Room *doctorRoom` yang sesuai dengan `doctor->room`.
    *   Jika `doctorRoom == NULL` atau `doctorRoom->patientInRoom.nEff == 0` (tidak ada pasien di slot langsung ruangan), mencetak kesalahan dan mengembalikan `false`.
5.  **Ambil Pasien Pertama di Ruangan**:
    *   `patientId = doctorRoom->patientInRoom.patientId[0];`
    *   Mencari `patientIdx` dan `Patient *patient` menggunakan `patientId`. Mencetak kesalahan dan mengembalikan `false` jika tidak ditemukan.
6.  **Pemeriksaan Status Pasien**:
    *   Jika `strcmp(patient->disease, "Tidak terdeteksi") == 0` (pasien tidak memiliki penyakit), mencetak sukses, mengatur `patient->treatedStatus = true;` (karena tidak ada perawatan yang diperlukan), dan mengembalikan `true`.
    *   Jika `!patient->diagnosedStatus`, mencetak kesalahan ("Pasien belum mendapat diagnosa...") dan mengembalikan `false`.
    *   Jika `patient->treatedStatus` (sudah dirawat/diresepkan), mencetak kesalahan ("Pasien sudah diberikan resep obat!") dan mengembalikan `false`.
    *   Jika `strcmp(patient->queueRoom, doctor->room) != 0` (pasien tidak berada di antrian ruangan dokter ini), mencetak kesalahan dan mengembalikan `false`.
    *   Jika `patient->id != patientId` (ID pasien tidak cocok dengan yang ada di depan slot ruangan), mencetak kesalahan dan mengembalikan `false`.
7.  **Cari ID Penyakit**:
    *   Mencari `diseaseId` dari `hospital->diseases.elements` yang cocok dengan nama `patient->disease`.
    *   Jika `diseaseId == -1`, mencetak kesalahan ("Penyakit pasien tidak ditemukan dalam database!") dan mengembalikan `false`.
8.  **Kumpulkan Resep untuk Penyakit**:
    *   Menghitung `medicationCount` untuk `diseaseId` dari `hospital->prescriptions.elements`.
    *   Jika `medicationCount > 0`:
        *   Mengalokasikan array `tempPrescriptions` dari `MedicationPrescription`. Jika gagal, mencetak kesalahan dan mengembalikan `false`.
        *   Menyalin semua entri `MedicationPrescription` untuk `diseaseId` ke `tempPrescriptions`.
        *   **Urutkan `tempPrescriptions` berdasarkan `doseOrder`** menggunakan bubble sort.
        *   Mengalokasikan `prescribedMedications.elements` (array struct `Medication`, tetapi hanya field `id` yang digunakan). Jika gagal, membebaskan `tempPrescriptions`, mencetak kesalahan, mengembalikan `false`.
        *   Menyalin `medicationId` dari `tempPrescriptions` yang sudah diurutkan ke `prescribedMedications.elements[i].id`. Mengatur `prescribedMedications.nEff`.
        *   Membebaskan `tempPrescriptions`.
9.  **Tampilkan Obat yang Diresepkan (Jika Ada)**:
    *   Jika `prescribedMedications.nEff > 0`:
        *   Mencetak pesan konfirmasi.
        *   Mencetak header tabel: "No.", "Nama Obat".
        *   Melakukan iterasi melalui `prescribedMedications.elements`:
            *   Mencari nama obat dari `hospital->medications.elements` menggunakan ID.
            *   Mencetak nomor urut dan nama obat dalam tabel.
            *   Jika ID obat tidak ditemukan di daftar utama, mencetak kesalahan.
        *   Mencetak batas bawah tabel.
10. **Perbarui Daftar Resep Pasien**:
    *   `patient->medicationsPrescribed.nEff = 0;` (bersihkan resep lama untuk keamanan, meskipun secara logis seharusnya tidak diperlukan jika pemeriksaan `treatedStatus` efektif).
    *   Menyalin ID obat dari `prescribedMedications.elements` ke `patient->medicationsPrescribed.medicationId`. Memperbarui `patient->medicationsPrescribed.nEff`.
11. **Pembersihan dan Finalisasi**:
    *   `free(prescribedMedications.elements);` (membebaskan daftar sementara yang digunakan untuk tampilan).
    *   `patient->treatedStatus = true;`
    *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika perawatan (penetapan resep) berhasil atau jika pasien tidak memiliki penyakit. `false` untuk kondisi kesalahan atau prasyarat yang tidak terpenuhi.
