# Berkas: `src/c/viewLifeStatus.c`

Berkas ini berisi fungsi untuk menampilkan status nyawa saat ini dari pasien yang sedang login.

## Fungsi: `viewLifeStatus`

### Tanda Tangan (Signature)
```c
void viewLifeStatus(Hospital *hospital, Session *session)
```

### Ringkasan
Menampilkan "status nyawa" untuk pasien yang saat ini login. Status nyawa direpresentasikan secara visual (misalnya, "OOX" untuk 2 dari 3 nyawa) dan sebagai status tekstual ("Hidup" atau "Meninggal"). Fungsi ini hanya dapat diakses oleh pengguna dengan peran `PATIENT`.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`, digunakan untuk mengakses daftar pasien.
*   `Session *session`: Pointer ke struktur `Session`, digunakan untuk otorisasi dan untuk mengidentifikasi pasien saat ini berdasarkan nama pengguna.

### Algoritma
1.  **Validasi Input (Pointer)**:
    *   Memeriksa apakah pointer `hospital` atau `session` adalah `NULL`. Jika ya, mencetak kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan kembali.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan `session->role == PATIENT`.
    *   Jika tidak diotorisasi, mencetak kesalahan ("Akses ditolak! Hanya Pasien yang dapat melihat status nyawa.") dan kembali.
3.  **Pemeriksaan Daftar Pasien**:
    *   Jika `hospital->patients.nEff == 0` (tidak ada pasien terdaftar di sistem), mencetak kesalahan ("Tidak ada pasien terdaftar!") dan kembali.
4.  **Cari Catatan Pasien**:
    *   Menginisialisasi `patientIdx = -1`.
    *   Melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien yang `username`-nya cocok dengan `session->username`.
    *   Jika ditemukan, `patientIdx` diatur.
    *   Jika tidak ditemukan (`patientIdx == -1`), mencetak kesalahan ("Pasien tidak ditemukan!") dan kembali.
5.  **Ambil Data Pasien**:
    *   `Patient *patient = &hospital->patients.elements[patientIdx];`
6.  **Validasi Data Nyawa**:
    *   Jika `patient->life < 0` (jumlah nyawa tidak valid), mencetak kesalahan ("Data nyawa tidak valid!") dan kembali. (Mengasumsikan nyawa tidak bisa negatif).
7.  **Tampilkan Status Nyawa**:
    *   Memanggil `printHeader("Status Nyawa")`.
    *   Mendefinisikan `widths` tabel dan `headers` ("Pasien", "Nyawa").
    *   Mencetak batas atas tabel dan baris header menggunakan `printTableBorder` dan `printTableRow`.
    *   **Format String Nyawa (`lifeStr`)**:
        *   Menginisialisasi `char lifeStr[10] = "";`.
        *   Menambahkan `COLOR_YELLOW`.
        *   Loop 3 kali (mengasumsikan maksimal 3 nyawa):
            *   Jika `i < patient->life`, tambahkan "O".
            *   Lainnya, tambahkan "X".
        *   Menambahkan `COLOR_RESET`.
    *   Menyiapkan baris data: `const char *row[] = {patient->username, lifeStr};`.
    *   Mencetak baris data dan batas bawah tabel.
8.  **Cetak Status Tekstual**:
    *   Jika `patient->life > 0`, memanggil `printSuccess("Status: Hidup")`.
    *   Lainnya (`patient->life <= 0`), memanggil `printError("Status: Meninggal")`.

### Nilai Kembali
*   `void`: Fungsi ini langsung mencetak output ke konsol.
