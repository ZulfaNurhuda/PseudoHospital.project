# Berkas: `src/c/registerCheckup.c`

Berkas ini berisi logika bagi pasien untuk mendaftar pemeriksaan dengan dokter. Ini melibatkan pemilihan dokter yang tersedia, pembayaran biaya, pembaruan data kesehatan pasien, dan penambahan pasien ke antrian ruangan dokter atau langsung ke dalam ruangan jika ruang tersedia.

## Fungsi: `registerCheckup`

### Tanda Tangan (Signature)
```c
boolean registerCheckup(Hospital *hospital, Session *session, float healthData[])
```

### Ringkasan
Memungkinkan `PATIENT` yang login untuk mendaftar pemeriksaan. Fungsi ini menampilkan daftar dokter yang tersedia, meminta pasien untuk memilih salah satu, memproses pembayaran, memperbarui data kesehatan yang diserahkan pasien, membuat catatan riwayat perawatan, dan kemudian menempatkan pasien langsung ke dalam ruangan dokter (jika kapasitas memungkinkan) atau mengantrikannya.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke `Session` pengguna saat ini (harus seorang PATIENT).
*   `float healthData[]`: Array angka floating-point yang merepresentasikan metrik kesehatan pasien saat ini. Urutan yang diharapkan:
    *   `healthData[0]`: Suhu Tubuh
    *   `healthData[1]`: Tekanan Darah Sistolik
    *   `healthData[2]`: Tekanan Darah Diastolik
    *   `healthData[3]`: Detak Jantung
    *   `healthData[4]`: Saturasi Oksigen
    *   `healthData[5]`: Kadar Gula Darah
    *   `healthData[6]`: Berat Badan
    *   `healthData[7]`: Tinggi Badan
    *   `healthData[8]`: Kadar Kolesterol
    *   `healthData[9]`: Trombosit

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah `hospital`, `session`, atau `healthData` adalah `NULL`. Jika ya, mencetak kesalahan dan mengembalikan `false`.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan `session->role == PATIENT`. Jika tidak, mencetak kesalahan dan mengembalikan `false`.
3.  **Cari Catatan Pasien**:
    *   Menemukan catatan pasien saat ini di `hospital->patients.elements` menggunakan `session->username`.
    *   Jika tidak ditemukan, mencetak kesalahan dan mengembalikan `false`.
    *   Mendapatkan `Patient *patient`.
4.  **Periksa Apakah Sudah Dalam Antrian**:
    *   Jika `patient->queueRoom[0] != '\0'` (pasien sudah dalam antrian atau ruangan), mencetak kesalahan ("Anda sudah terdaftar dalam antrian!") dan mengembalikan `false`.
5.  **Daftar Dokter yang Tersedia**:
    *   Membuat array sementara `availableDoctors` untuk menyimpan pointer ke dokter yang ditugaskan ke sebuah ruangan (`doctor->room[0] != '\0'`).
    *   Jika `availableDoctorCount == 0`, mencetak kesalahan ("Tidak ada dokter yang tersedia saat ini.") dan mengembalikan `false`.
    *   Mencetak header tabel yang diformat: "No", "Dokter", "Spesialisasi", "Ruangan", "Antrian", "Aura", "Biaya Checkup".
    *   Melakukan iterasi melalui `availableDoctors`:
        *   Untuk setiap dokter, menentukan jumlah antrian mereka saat ini dengan mencari di `hospital->queues`.
        *   Mencetak baris dalam tabel dengan detail dokter (nomor, nama pengguna, spesialisasi, ruangan, jumlah antrian, aura, biaya pemeriksaan).
6.  **Pemilihan Dokter**:
    *   Meminta pasien untuk memilih dokter berdasarkan nomor (1 hingga `availableDoctorCount`).
    *   Menggunakan `readValidInt()` untuk mendapatkan pilihan integer yang valid. Jika input tidak valid, mencetak kesalahan dan mengembalikan `false`.
    *   Mendapatkan `Doctor *doctor = availableDoctors[doctorChoice - 1];`.
7.  **Pemeriksaan Pembayaran**:
    *   Jika `patient->bananaRich < doctor->checkupCost`, mencetak kesalahan ("Saldo BananaRich tidak cukup untuk checkup!") dan mengembalikan `false`.
8.  **Cari atau Buat Antrian Dokter**:
    *   Mencari antrian yang ada untuk `doctor->room` di `hospital->queues`.
    *   Jika tidak ada antrian (`queueIdx == -1`):
        *   Memeriksa apakah `hospital->queues.nRooms >= hospital->queues.capacity`. Jika penuh, mencetak kesalahan ("Kapasitas antrian penuh!") dan mengembalikan `false`.
        *   Menggunakan slot berikutnya yang tersedia: `queueIdx = hospital->queues.nRooms++;`.
        *   Menginisialisasi antrian baru ini: `initializeQueue(&hospital->queues.queues[queueIdx], doctor->room);`.
    *   Mendapatkan `Queue *queueToCheck = &hospital->queues.queues[queueIdx];`.
9.  **Cari Ruangan yang Dipilih**:
    *   Mencari `Room *selectedRoom` di `hospital->layout.elements` yang cocok dengan `doctor->room`.
10. **Perbarui Data Kesehatan Pasien**:
    *   Menetapkan nilai dari array `healthData` ke field yang sesuai dalam struktur `patient` (misalnya, `patient->bodyTemperature = healthData[0];`, dll.).
11. **Proses Pembayaran dan Keuangan**:
    *   `patient->bananaRich -= doctor->checkupCost;`
    *   `hospital->finance.hospitalBalance += 0.2 * doctor->checkupCost;` (20% ke rumah sakit).
    *   `doctor->bananaRich += 0.8 * doctor->checkupCost;` (80% ke dokter).
12. **Buat Catatan Riwayat Perawatan**:
    *   Memeriksa apakah `hospital->treatmentHistory.nEff >= hospital->treatmentHistory.capacity`.
        *   Jika penuh, mencoba `realloc` `hospital->treatmentHistory.elements` untuk menggandakan kapasitasnya.
        *   Jika `realloc` gagal, mencetak kesalahan, mengembalikan transaksi pembayaran, dan mengembalikan `false`.
    *   `TreatmentHistory *history = &hospital->treatmentHistory.elements[hospital->treatmentHistory.nEff++];`
    *   Mengisi field `history`: `patientId`, `doctorId`, `room` (ruangan dokter), `disease` ("Belum Didiagnosis"), `treatmentStatus = false`.
13. **Tempatkan Pasien di Ruangan atau Antrian**:
    *   **Jika `selectedRoom != NULL` dan `selectedRoom->patientInRoom.nEff < selectedRoom->capacity` (ruangan ada dan memiliki ruang)**:
        *   `strcpy(patient->queueRoom, doctor->room);`
        *   `patient->queuePosition = 0;` (0 menunjukkan di dalam ruangan, bukan di antrian eksternal).
        *   Menambahkan `patient->id` ke array `selectedRoom->patientInRoom.patientId` dan menaikkan `selectedRoom->patientInRoom.nEff`.
        *   Mencetak header konfirmasi dan tabel untuk "Pendaftaran Checkup" yang menunjukkan Dokter dan Ruangan.
        *   Mencetak pesan info: "[ℹ️  | Info]: Anda telah dipindahkan langsung ke ruangan dokter!".
        *   Mencetak pesan sukses keseluruhan.
        *   Mengembalikan `true`.
    *   **Lainnya (tempatkan di antrian)**:
        *   Memanggil `enqueue(queueToCheck, patient->id)`.
        *   Jika `enqueue` gagal:
            *   Mencetak kesalahan ("Gagal menambahkan pasien ke antrian!").
            *   Mengembalikan transaksi pembayaran dan mengurangi `hospital->treatmentHistory.nEff`.
            *   Mengembalikan `false`.
        *   `strcpy(patient->queueRoom, doctor->room);`
        *   `patient->queuePosition = queueSize(queueToCheck);` (mengatur posisi ke akhir antrian saat ini).
        *   Mencetak header konfirmasi dan tabel untuk "Pendaftaran Checkup".
        *   Mencetak pesan info: "[ℹ️  | Info]: Posisi antrian: [patient->queuePosition]".
        *   Mencetak pesan sukses keseluruhan.
        *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika pendaftaran pemeriksaan berhasil, `false` jika tidak.
