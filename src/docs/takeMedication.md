# Berkas: `src/c/takeMedication.c`

Berkas ini berisi logika bagi pasien untuk meminum obat yang telah diresepkan untuknya. Fungsi ini memeriksa apakah obat tersebut adalah yang benar untuk diminum berikutnya secara berurutan dan menangani konsekuensi jika obat diminum tidak sesuai urutan, termasuk kehilangan "nyawa" dan potensi penghapusan dari sistem jika nyawa habis.

## Fungsi: `takeMedication`

### Tanda Tangan (Signature)
```c
boolean takeMedication(Hospital *hospital, Session *session)
```

### Ringkasan
Memungkinkan `PATIENT` yang login untuk meminum obat. Fungsi pertama-tama memeriksa apakah pasien telah didiagnosis dan diresepkan obat. Kemudian, fungsi ini menampilkan daftar obat yang diresepkan yang tersedia (belum diminum). Jika pasien memilih obat yang tidak sesuai urutan resep, mereka kehilangan "nyawa". Jika semua nyawa habis, pasien dihapus dari sistem.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke `Session` pengguna saat ini (harus seorang PATIENT).

### Algoritma
1.  **Validasi Input & Otorisasi**:
    *   Memeriksa apakah `hospital` atau `session` adalah `NULL`. Mencetak kesalahan dan mengembalikan `false`.
    *   Memverifikasi `session->isLoggedIn` dan `session->role == PATIENT`. Mencetak kesalahan dan mengembalikan `false` jika tidak.
2.  **Cari Catatan Pasien**:
    *   Menemukan `patientIdx` dan `Patient *patient` menggunakan `session->username`. Mencetak kesalahan dan mengembalikan `false` jika tidak ditemukan.
3.  **Pemeriksaan Pra-Kondisi**:
    *   Jika `strcmp(patient->disease, "Tidak terdeteksi") == 0` (tidak ada penyakit), mencetak pesan dan mengembalikan `false`.
    *   Jika `!patient->treatedStatus` (belum diresepkan obat), mencetak kesalahan ("Anda belum diberikan resep obat!") dan mengembalikan `false`.
    *   Jika `patient->medicationsPrescribed.nEff == 0` (tidak ada obat yang diresepkan), mencetak kesalahan dan mengembalikan `false`.
4.  **Periksa Obat Salah Sebelumnya**:
    *   Jika `patient->medicationsTaken.top >= 0` (setidaknya satu obat sudah diminum):
        *   Menentukan apakah obat *terakhir* yang diminum salah dengan membandingkan `patient->medicationsTaken.medicationId[patient->medicationsTaken.top]` dengan apa yang seharusnya menjadi `patient->medicationsPrescribed.medicationId[takenCount - 1]`.
        *   Jika salah (`needsAntidote == true`), mencetak kesalahan ("Urutan obat salah! Silakan minum penawar terlebih dahulu.") dan mengembalikan `false`.
5.  **Periksa Apakah Semua Obat Resep Sudah Diminum**:
    *   Melakukan iterasi melalui `patient->medicationsPrescribed`. Jika semua obat yang diresepkan ditemukan di `patient->medicationsTaken`, atur `isMedicationAvailable = false`.
    *   Jika `!isMedicationAvailable`, mencetak kesalahan ("Semua obat yang diresepkan sudah dikonsumsi!") dan mengembalikan `false`.
6.  **Tampilkan Obat yang Tersedia**:
    *   Mencetak header "Daftar Obat yang Diresepkan" dan pesan info.
    *   Mencetak header tabel: "Urutan Minum", "Nama Obat".
    *   Melakukan iterasi melalui `patient->medicationsPrescribed`:
        *   Jika obat belum diminum:
            *   Menandainya sebagai `validOptions[i] = true`.
            *   Menemukan namanya dari `hospital->medications.elements`.
            *   Mencetak obat (nomor urut `i+1`, nama) dalam tabel.
        *   Lainnya, `validOptions[i] = false`.
    *   Mencetak batas bawah tabel.
7.  **Pilihan Pengguna**:
    *   Meminta pengguna ">>> Pilih obat untuk diminum: " menggunakan `readValidInt(&choice, ...)`. Loop hingga input integer valid.
    *   Jika `choice < 1` atau `choice > patient->medicationsPrescribed.nEff` (nomor tidak valid), mencetak kesalahan dan mengembalikan `false`.
    *   Jika `!validOptions[choice - 1]` (obat yang dipilih sudah diminum atau tidak valid), mencetak kesalahan dan mengembalikan `false`.
8.  **Tentukan Obat Berikutnya yang Diharapkan**:
    *   Mencari ID obat pertama di `patient->medicationsPrescribed` yang belum ada di `patient->medicationsTaken`. Ini adalah `expectedMedicationId`.
9.  **Proses Obat yang Diminum**:
    *   `selectedMedicationId = patient->medicationsPrescribed.medicationId[choice - 1];`
    *   Menambahkan `selectedMedicationId` ke tumpukan `patient->medicationsTaken.medicationId` dan menaikkan `patient->medicationsTaken.top`.
10. **Periksa Apakah Obat yang Diminum Benar**:
    *   **Jika `selectedMedicationId != expectedMedicationId` (Obat Salah)**:
        *   `patient->life--;`
        *   Mencetak "[💀 | Info ] - Obat Salah! Nyawa berkurang satu.".
        *   **Jika `patient->life <= 0` (Game Over)**:
            *   Mencetak kesalahan ("Pasien kehabisan nyawa! Pasien akan dihapus. 🥀").
            *   **Hapus dari Ruangan/Antrian**: Jika pasien berada di `patient->queueRoom`, temukan ruangan dan antrian terkait. Hapus pasien dari `room->patientInRoom`. Jika antrian tidak kosong, dequeue pasien berikutnya ke slot ruangan jika tersedia. (Ini mirip dengan pembersihan `canGoHome`).
            *   Memanggil `deletePatient(hospital, patient->id)` untuk menghapus pasien dari sistem.
            *   Mencetak info: "[⭕ | Info ] - Sesi tidak valid setelah pasien kehabisan nyawa.".
            *   Membatalkan sesi saat ini: `session->isLoggedIn = false; session->userId = -1; strcpy(session->username, "");`.
            *   Mengembalikan `false`.
        *   **Lainnya (Masih memiliki nyawa)**:
            *   Mencetak sisa nyawa (misalnya, "→ Sisa nyawa: OOX").
            *   Mengembalikan `false` (menunjukkan obat diminum, tetapi salah).
    *   **Lainnya (`selectedMedicationId == expectedMedicationId`) (Obat Benar)**:
        *   Mencetak sukses ("Obat berhasil diminum!").
        *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika obat yang benar berhasil diminum. `false` jika terjadi kesalahan, obat yang salah diminum (meskipun nyawa tersisa), atau jika pasien kehabisan nyawa.
