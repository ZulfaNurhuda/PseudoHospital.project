# **Data yang Dibutuhkan**

File ini digunakan untuk merinci / listing data yang dibutuhkan untuk masing masing fitur.

## F00 - Pengelolaan Sesi

| Data yang diperlukan |
|-----------|
| ID |
| username |
| role |
| isLoggedIn |

## F01 - Login

> Akeses: Belum login.

| Data yang diperlukan |
|-----------|
| Username  |
| Password  |

## F02 -  Register Pasien

> Akses: Belum login (Khusus Pasien)

| Data yang diperlukan |
|-----------|
| Username  |
| Password  |

## F03 - Logout

> Akses: Manajer, Dokter, Pasien

| Data yang diperlukan |
|-----------|
| Username  |
| Password  |

## F04 - Lupa Password

> Akses: Manajer, Dokter, Pasien

| Data yang diperlukan |
|-----------|
| Username  |
| Password  |

## F05 - Menu & Help

> Akses: Manajer, Dokter, Pasien, Belum Login

| Data yang diperlukan  |
|------------|
| List menu  |

**Note**: List menu yang ditampilkan sesuai dengan user yang menggunakan command.

## F06 - Denah Rumah Sakit

> Akses: Manajer, Dokter, Pasien

### `Sub:` Lihat Denah

| Data yang diperlukan  |
|------------|
| Ukuran denah (matriks 2D) |

**Contoh**
```js
      1     2     3     4
   +-----+-----+-----+-----+
 A | A1  | A2  | A3  | A4  |
   +-----+-----+-----+-----+
 B | B1  | B2  | B3  | B4  |
   +-----+-----+-----+-----+
 C | C1  | C2  | C3  | C4  |
   +-----+-----+-----+-----+
 D | D1  | D2  | D3  | D4  |
   +-----+-----+-----+-----+
```

### `Sub:` Lihat Ruangan ${ruangan}

| Data yang diperlukan  |
|------------|
| Kapasitas ruangan  |
| Dokter yang berada dalam ruangan |
| List pasien dalam ruangan |

## F07 - Lihat User

> Akses: Manajer

### `Sub:` Lihat User

| Data yang diperlukan  |
|------------|
| ID |
| Username |
| Role |
| Penyakit |

**Note**: Penyakit hanya ditampilkan untuk pasien, untuk role  lain akan ditampilkan dengan (`-`).

### `Sub:` Lihat Pasien

| Data yang diperlukan  |
|------------|
| ID |
| Username |
| Penyakit |

### `Sub:` Lihat Dokter

| Data yang diperlukan  |
|------------|
| ID |
| Username |
| Aura |

## F08 - Cari User

> Akses: Manajer

| Data yang diperlukan  |
|------------|
| ID |
| Username |
| Role |
| Penyakit |

**Note**: Penyakit hanya ditampilkan untuk pasien, untuk role  lain akan ditampilkan dengan (`-`).

### `Sub:` Cari Pasien

| Data yang diperlukan  |
|------------|
| ID |
| Username |
| Penyakit |

### `Sub:` Cari Dokter

| Data yang diperlukan  |
|------------|
| ID |
| Username |

## F09 - Lihat Antrian

> Akses: Manajer

| Data yang diperlukan  |
|------------|
| Ukuran denah (matriks 2D) |
| Data Per Ruangan (Kapasitas, Dokter, Pasien dalam Ruangan, Pasien dalam Antrian) |

**Contoh**
```js
      1     2     3     4
   +-----+-----+-----+-----+
 A | A1  | A2  | A3  | A4  |
   +-----+-----+-----+-----+
 B | B1  | B2  | B3  | B4  |
   +-----+-----+-----+-----+
 C | C1  | C2  | C3  | C4  |
   +-----+-----+-----+-----+
 D | D1  | D2  | D3  | D4  |
   +-----+-----+-----+-----+
        
============ A1 ============
Kapasitas  : 3
Dokter     : Dr. Strange
Pasien di dalam ruangan:
  1. John Smith
  2. John Lennon
  3. John Lagi
Pasien di antrian:
  1. John Terus
  2. John Bosen

# Ruangan Kosong tidak perlu ditampilkan
============ B1 ============
Kapasitas  : 3
Dokter     : Dr. Oz
Pasien di dalam ruangan:
  Tidak ada pasien di dalam ruangan saat ini.
Pasien di antrian:
  Tidak ada pasien di antrian saat ini.
```

## F10 - Tambah Dokter

> Akses: Manajer

### `Sub:` Tambah Dokter

| Data yang diperlukan  |
|------------|
| Username |
| Password |
| Spesialis |

### `Sub:` Assign Dokter

| Data yang diperlukan  |
|------------|
| Username |
| Ruangan |

## F11 - Diagnosis

> Akses: Dokter

| Data yang diperlukan |
|------------|
| Data Pasien |

**Note**: Pada fitur ini, akan melakukan diagnosis otomatis berdasarkan data seperti dibawah ini.


**Sampel Data**
| id  | nama_penyakit     | suhu_tubuh_min | suhu_tubuh_max | tekanan_darah_sistolik_min | tekanan_darah_sistolik_max | tekanan_darah_diastolik_min | tekanan_darah_diastolik_max | detak_jantung_min | detak_jantung_max | kadar_gula_darah_min | kadar_gula_darah_max | berat_badan_min | berat_badan_max | tinggi_badan_min | tinggi_badan_max | kadar_kolesterol_min | kadar_kolesterol_max | trombosit_min | trombosit_max |
|-----|-------------------|----------------|----------------|----------------------------|----------------------------|----------------------------|----------------------------|-------------------|-------------------|----------------------|----------------------|-----------------|-----------------|------------------|------------------|----------------------|----------------------|----------------|----------------|
| 1   | Influenza          | 36.0           | 38.5           | 90                         | 120                        | 60                         | 80                         | 60                | 100               | 70.0                 | 140.0                | 45.0            | 90.0            | 150              | 185              | 150                  | 240                  | 150            | 450            |
| 2   | COVID-19           | 36.0           | 39.0           | 90                         | 130                        | 60                         | 85                         | 60                | 120               | 70.0                 | 180.0                | 45.0            | 90.0            | 150              | 185              | 150                  | 240                  | 150            | 450            |
| 3   | Hipertensi         | 36.0           | 37.5           | 140                        | 180                        | 90                         | 120                        | 60                | 100               | 70.0                 | 140.0                | 45.0            | 90.0            | 150              | 185              | 150                  | 240                  | 150            | 450            |
| 4   | Diabetes Mellitus  | 36.0           | 37.5           | 90                         | 140                        | 60                         | 90                         | 60                | 100               | 126.0                | 200.0                | 45.0            | 90.0            | 150              | 185              | 150                  | 240                  | 150            | 450            |
| 5   | Anemia             | 36.0           | 37.0           | 90                         | 120                        | 60                         | 80                         | 60                | 100               | 70.0                 | 140.0                | 45.0            | 90.0            | 150              | 185              | 150                  | 240                  | 150            | 450            |

## F12 - Ngobatin

> Akses: Dokter

| Data yang diperlukan |
|------------|
| Penyakit |

**Note**: Pada fitur ini, akan melakukan peresepan obat otomatis berdasarkan data seperti dibawah ini.

**Sampel Data**
| obat_id | penyakit_id | urutan_minum |
|---------|-------------|--------------|
| 1       | 1           | 1            |
| 2       | 1           | 2            |
| 3       | 2           | 1            |
| 4       | 3           | 1            |
| 5       | 4           | 1            |

## F13 - Pulang Dok

> Akses: Pasien

| Data yang diperlukan |
|------------|
| Data dokter (Aura) |
| Status Pasien (Belum di diagnosa, Belum diberikan obar, Salah urutan konsumsi obat, Sudah boleh pulang) |

**Note**: Aura diberikan setiap dokter berhasil mengobati pasien.

## F14 - Daftar Checkup

> Akses: Pasien

| Data yang diperlukan |
|------------|
| Data pasien (BananaRich, Suhu Tubuh, Tekanan Darah, Detak Jantung, Saturasi Oksigen, Kadar Gula Darah, Berat Badan, Tinggi Badan, Kadar Kolestrol, Kadar Kolestrol LDL, Trombosit) |
| Daftar dokter tersedia (Nama dokter, Spesialisasi, Ruangan, Jumlah antrian, Aura, Biaya *[dalam BananaRich]*) |

## F15 - Antrian Saya

> Akses: Pasien

| Data yang diperlukan |
|------------|
| Status Antrian (Nama Dokter, Ruangan, antrian ke `...` dari `...` antrian) |

## F16 - Minum Obat

> Akses: Pasien

| Data yang diperlukan  |
|------------|
| Daftar Obat sesuai urutan |
| Sisa nyawa |

## F17 - Minum Penawar

> Akses: Pasien

| Data yang diperlukan  |
|------------|
| Daftar Obat sesuai urutan |

**Note**: Setelah menjalankan command ini, obat yang terakhir diminum akan kembali ke inventory

## F18 - Exit

| Data yang diperlukan |
|------------|
| *Tidak ada data yang diperlukan* |

## B02 - Denah Dinamis

> Akses: Manajer

### `Sub:` Ubah Denah

| Data yang diperlukan |
|------------|
| Ukuran denah (matriks 2D) |
| Dokter dalam ruangan |

**Contoh**
```
# Denah awal berukuran 2x3

>>> LIHAT_DENAH

      1     2     3     
   +-----+-----+-----+
 A | A1  | A2  | A3  |
   +-----+-----+-----+
 B | B1  | B2  | B3  |
   +-----+-----+-----+

# Manajer ingin mengubah ukuran denah

>>> UBAH_DENAH 3 4
Denah rumah sakit berhasil diubah menjadi 3 baris dan 4 kolom.

>>> LIHAT_DENAH

      1     2     3     4
   +-----+-----+-----+-----+
 A | A1  | A2  | A3  | A4  |
   +-----+-----+-----+-----+
 B | B1  | B2  | B3  | B4  |
   +-----+-----+-----+-----+
 C | C1  | C2  | C3  | C4  |
   +-----+-----+-----+-----+

# Manager mencoba mengecilkan denah ke 1x1, tapi masih terdapat dokter di A2 dan A3

>>> UBAH_DENAH 1 1
Tidak dapat mengubah ukuran denah. Ruangan A2 masih ditempati oleh Dr. Strange. Silakan pindahkan dokter terlebih dahulu.

>>> LIHAT_DENAH

      1     2     3     4
   +-----+-----+-----+-----+
 A | A1  | A2  | A3  | A4  |
   +-----+-----+-----+-----+
 B | B1  | B2  | B3  | B4  |
   +-----+-----+-----+-----+
 C | C1  | C2  | C3  | C4  |
   +-----+-----+-----+-----+
```

### `Sub:` Pindahkan Dokter

| Data yang diperlukan |
|------------|
| Dokter dalam ruangan |

## B03 - BananaRich

### `Sub:` Lihat Dompet

> Akses: Pasien dan Dokter

| Data yang diperlukan |
|------------|
| Jumlah uang dalam dompet (dalam BananaRich) |

### `Sub:` Lihat Finansial

> Akses: Manajer

| Data yang diperlukan |
|------------|
| Jumlah uang dalam dompet (dalam BananaRich) |

### `Sub:` Gacha

| Data yang diperlukan |
|------------|
| Jumlah uang dalam dompet (dalam BananaRich) |

## B04 - Dead or Alive

**📝 Aturan Singkat:**
- Setiap pasien memulai dengan `3` nyawa (🔴🔴🔴).
- Setiap salah minum obat, nyawa pasien berkurang `1`.
- Jika nyawa habis `(0)`, pasien dinyatakan "Ded" dan harus dikeluarkan dari rumah sakit.
- Pasien bisa sembuh dengan meminum obat dalam urutan yang benar atau menggunakan penawar sebelum mencapai `3` kesalahan.
- Tiap kali fitur Penawar digunakan, sisa nyawa wajib ditampilkan.

**📌 Contoh Kasus:**
- Stewart salah minum obat(minta Penawar): 🔴🔴⚫ *(sisa 2 nyawa)*
- Stewart salah minum obat lagi( minta Penawar lagi ): 🔴⚫⚫ *(sisa 1 nyawa)*
- Pasien melakukan kesalahan minum obat(minta Penawar) sebanyak 3 kali: ⚫⚫⚫ *(Kritis!)* Kondisi pasien menjadi fatal, sehingga pasien tersebut dinyatakan “ded” dan otomatis tidak bisa login kembali (data pasien dihapus dari sistem).

## B05 - Mainin Antrian

> Akses: Pasien

### `Sub:` Skip Antrian

| Data yang diperlukan |
|------------|
| Status Antrian (Nama Dokter, Ruangan, antrian ke `...` dari `...` antrian) |

### `Sub:` Cancel Antrian
| Data yang diperlukan |
|------------|
| Status Antrian (Nama Dokter, Ruangan) |

## E01 - Encrypt & Decrypt Password

**Note** Fitur ini akan mengenkripsi password dengan metode Enkripsi Mesin Enigma. Nah yang nanti di simpan adalah `Password` dan `Konfigurasi Enigma`. Nah, konfigurasi enigma ini yang akan kembali di enkripsi dengan metode Caesar Chiper.

**Contoh Data Password**
```py
# Konfigurasi Caesar Chiper Tetap.

"password": {
    "content": "...",
    "caesarChiperShift": 0,
    "enigma_congfig": {
        "rotor_types": [...],
        "rotor_positions": [...],
        "reflection_type": "...",
        "plugboard_pairs": {...},
    }
}
```