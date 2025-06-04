# Berkas: `src/c/exit.c`

Berkas ini berisi fungsi yang bertanggung jawab untuk keluar dari program manajemen rumah sakit secara baik-baik.

## Fungsi: `exitProgram`

### Tanda Tangan (Signature)
```c
void exitProgram(Hospital *hospital, Session *session)
```

### Ringkasan
Fungsi ini menangani penghentian sistem manajemen rumah sakit. Fungsi ini mencetak pesan perpisahan dan, yang terpenting, memanggil `freeHospital` untuk melepaskan semua memori yang dialokasikan secara dinamis yang terkait dengan struktur `Hospital`, untuk mencegah kebocoran memori.

### Parameter
*   `Hospital *hospital`: Sebuah pointer ke struktur data utama `Hospital`. Struktur ini dan sub-komponennya akan dibebaskan.
*   `Session *session`: Sebuah pointer ke struktur `Session`. Meskipun diteruskan sebagai parameter, penggunaan utamanya di sini adalah untuk pemeriksaan validasi awal, memastikan fungsi dipanggil dalam konteks program yang valid. Ini tidak digunakan secara langsung untuk pembersihan khusus sesi dalam fungsi ini sendiri selain itu.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah pointer `hospital` atau `session` adalah `NULL`.
    *   Jika salah satunya `NULL`, ia mencetak pesan kesalahan ("Struktur rumah sakit atau sesi tidak valid!") dan kembali, karena tidak dapat melanjutkan dengan membebaskan struktur rumah sakit `NULL` atau beroperasi tanpa konteks sesi yang valid.
2.  **Cetak Header**:
    *   Memanggil `printHeader("Keluar Program")` untuk menampilkan header standar untuk operasi keluar.
3.  **Cetak Pesan Perpisahan**:
    *   Mencetak pesan terima kasih: "Terima kasih telah menggunakan Sistem Manajemen Rumah Sakit Nimons!\n".
    *   Mencetak kotak pesan dekoratif "Sampai Jumpa!".
4.  **Bebaskan Sumber Daya Rumah Sakit**:
    *   Memanggil `freeHospital(hospital)`. Ini adalah langkah penting yang membatalkan alokasi semua memori dinamis yang dialokasikan untuk data rumah sakit selama eksekusi program. Ini termasuk memori untuk daftar pasien, daftar dokter, tata letak ruangan, antrian, definisi penyakit, dll.
    *   (Secara implisit, setelah fungsi ini selesai, fungsi `main` atau konteks pemanggilan biasanya akan `exit(0)` atau kembali untuk menghentikan proses.)

### Nilai Kembali
*   `void`: Fungsi ini tidak mengembalikan nilai.
