# Berkas: `src/c/takeAntidote.c`

Berkas ini berisi logika bagi pasien untuk meminum penawar. Meminum penawar dimaksudkan untuk membalikkan efek obat terakhir yang dikonsumsi jika diminum secara tidak benar (yaitu, tidak sesuai urutan resep).

## Fungsi: `takeAntidote`

### Tanda Tangan (Signature)
```c
boolean takeAntidote(Hospital *hospital, Session *session)
```

### Ringkasan
Memungkinkan `PATIENT` yang login untuk mengonsumsi penawar. Penawar efektif jika obat terakhir yang diminum pasien bukanlah obat berikutnya yang benar menurut resep mereka saat ini. Jika berhasil, obat terakhir dihapus dari tumpukan `medicationsTaken` mereka, dan status nyawa mereka ditampilkan.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`. Digunakan untuk mengakses catatan pasien, daftar obat, dan resep.
*   `Session *session`: Pointer ke `Session` pengguna saat ini (harus seorang PATIENT).

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah `hospital` atau `session` adalah `NULL`. Jika ya, mencetak kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan mengembalikan `false`.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan `session->role == PATIENT`. Jika tidak, mencetak kesalahan ("Akses ditolak! Hanya Pasien yang dapat mengonsumsi penawar.") dan mengembalikan `false`.
3.  **Cari Catatan Pasien**:
    *   Menemukan indeks pasien saat ini (`patientIdx`) di `hospital->patients.elements` menggunakan `session->username`.
    *   Jika tidak ditemukan, mencetak kesalahan ("Pasien tidak ditemukan!") dan mengembalikan `false`.
    *   Mendapatkan `Patient *patient = &hospital->patients.elements[patientIdx];`.
4.  **Periksa Apakah Ada Obat yang Telah Diminum**:
    *   Jika `patient->medicationsTaken.top < 0` (tumpukan kosong), mencetak kesalahan ("Anda belum mengonsumsi obat apapun!") dan mengembalikan `false`.
5.  **Tentukan Apakah Penawar Dibutuhkan**:
    *   Menginisialisasi `isNeedAntidote = false` dan `wrongMedicationId = -1`.
    *   `lastMedicationId = patient->medicationsTaken.medicationId[patient->medicationsTaken.top];` (mendapatkan ID obat yang terakhir diminum).
    *   `takenCount = patient->medicationsTaken.top + 1;` (jumlah obat yang telah diminum sejauh ini).
    *   `expectedMedicationId = -1;`
    *   Jika `takenCount <= patient->medicationsPrescribed.nEff` (pasien belum minum obat lebih banyak dari yang diresepkan):
        *   `expectedMedicationId = patient->medicationsPrescribed.medicationId[takenCount - 1];` (mendapatkan ID obat yang *seharusnya* menjadi obat terakhir yang diminum sesuai urutan resep).
    *   **Pemeriksaan Utama**: Jika `expectedMedicationId != -1` (artinya ada obat yang diharapkan pada langkah ini) DAN `lastMedicationId != expectedMedicationId`:
        *   Mengatur `isNeedAntidote = true`.
        *   Mengatur `wrongMedicationId = lastMedicationId`.
    *   **Pemeriksaan Sekunder (untuk ketidakcocokan urutan umum jika pemeriksaan utama lolos atau tidak berlaku, misalnya, jika lebih banyak obat diminum daripada yang diresepkan awalnya)**:
        *   Lainnya (jika obat terakhir yang diminum *adalah* yang diharapkan, atau jika lebih banyak obat diminum daripada yang diresepkan awalnya, loop ini memeriksa seluruh urutan hingga obat terakhir yang diminum):
            *   Melakukan iterasi `i` dari `0` hingga `patient->medicationsTaken.top`.
            *   Jika `i < patient->medicationsPrescribed.nEff` (masih dalam batas obat yang diresepkan):
                *   Jika `patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i]`:
                    *   Jika ketidakcocokan ini terjadi pada obat terakhir yang diminum (`i == patient->medicationsTaken.top`):
                        *   Mengatur `isNeedAntidote = true`.
                        *   Mengatur `wrongMedicationId = patient->medicationsTaken.medicationId[i]`.
                        *   Hentikan loop.
6.  **Proses Penawar jika Dibutuhkan**:
    *   Jika `isNeedAntidote` adalah `true` DAN `wrongMedicationId != -1`:
        *   Menemukan nama (`medicationName`) dari `wrongMedicationId` dari `hospital->medications.elements`. Default ke "Tidak dikenal" jika tidak ditemukan.
        *   "Menghapus" obat terakhir dengan mengurangi pointer tumpukan: `patient->medicationsTaken.top--;`.
        *   **Tampilkan Tabel Hasil Penawar**:
            *   Mencetak header "Konsumsi Penawar".
            *   Header tabel: "Obat Dibatalkan", "Nyawa Tersisa", "Status".
            *   Memformat `lifeStr` untuk menunjukkan 'O' untuk nyawa yang tersisa dan 'X' untuk nyawa yang hilang (misalnya, "OOX").
            *   Mencetak baris tabel dengan `medicationName`, `lifeStr`, dan "Penawar berhasil".
        *   Mencetak pesan sukses: "Penawar berhasil! Obat [medicationName] dibatalkan.".
        *   Mengembalikan `true`.
7.  **Penawar Tidak Dibutuhkan**:
    *   Lainnya (jika `isNeedAntidote` adalah `false` atau `wrongMedicationId` tidak diatur):
        *   Mencetak kesalahan ("Anda tidak perlu mengonsumsi penawar! Urutan obat yang diminum sudah benar.").
        *   Mengembalikan `false`.

### Nilai Kembali
*   `boolean`: `true` jika penawar berhasil diminum (artinya obat yang salah dibatalkan), `false` jika tidak (misalnya, tidak ada obat yang diminum, tidak ada kesalahan ditemukan, atau kesalahan lainnya).
