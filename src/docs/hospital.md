# Berkas: `src/c/hospital.c`

Berkas ini merupakan pusat dari sistem manajemen rumah sakit, berisi fungsi-fungsi untuk menginisialisasi struktur data utama `Hospital`, membebaskan memori yang dialokasikannya, dan mengelola entitas tingkat tinggi seperti pasien (misalnya, penghapusan).

## Fungsi: `initHospital`

### Tanda Tangan (Signature)
```c
boolean initHospital(Hospital *hospital, int userCapacity, int patientCapacity, int doctorCapacity, int roomRows, int roomCols)
```

### Ringkasan
Menginisialisasi struktur data utama `Hospital` dengan kapasitas yang ditentukan untuk pengguna, pasien, dokter, dan dimensi tata letak ruangan. Fungsi ini mengalokasikan memori untuk semua array dan daftar data primer, menginisialisasi ruangan dengan kode dan kapasitas default, menyiapkan penyakit, obat-obatan, resep yang telah ditentukan sebelumnya, dan membuat pengguna `MANAGER` default. Pemeriksaan kesalahan ekstensif dilakukan pada setiap langkah alokasi memori, dengan pembersihan memori yang dialokasikan sebelumnya jika alokasi gagal.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur `Hospital` yang akan diinisialisasi.
*   `int userCapacity`: Kapasitas awal untuk array pengguna.
*   `int patientCapacity`: Kapasitas awal untuk array pasien.
*   `int doctorCapacity`: Kapasitas awal untuk array dokter.
*   `int roomRows`: Jumlah baris untuk tata letak ruangan rumah sakit.
*   `int roomCols`: Jumlah kolom untuk tata letak ruangan rumah sakit.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah pointer `hospital` adalah `NULL`. Jika ya, mencetak kesalahan dan mengembalikan `false`.
    *   Memeriksa apakah ada kapasitas/dimensi (`userCapacity`, `patientCapacity`, `doctorCapacity`, `roomRows`, `roomCols`) yang `<= 0`. Jika ya, mencetak kesalahan dan mengembalikan `false`.
2.  **Inisialisasi Pointer ke NULL**: Mengatur semua pointer array dinamis dalam `hospital` (misalnya, `hospital->users.elements`, `hospital->patients.elements`, dll.) ke `NULL` sebagai tindakan pencegahan.
3.  **Alokasi dan Inisialisasi `UserSet` (`hospital->users`)**:
    *   Mengalokasikan memori untuk `userCapacity` pengguna menggunakan `safeMalloc`.
    *   Jika alokasi gagal, mencetak kesalahan dan mengembalikan `false`.
    *   Mengatur `hospital->users.capacity = userCapacity` dan `hospital->users.nEff = 0`.
4.  **Alokasi dan Inisialisasi `PatientList` (`hospital->patients`)**:
    *   Mengalokasikan memori untuk `patientCapacity` pasien.
    *   Jika alokasi gagal, mencetak kesalahan, membebaskan `hospital->users.elements`, dan mengembalikan `false`.
    *   Mengatur `hospital->patients.capacity = patientCapacity` dan `hospital->patients.nEff = 0`.
5.  **Alokasi dan Inisialisasi `DoctorList` (`hospital->doctors`)**:
    *   Mengalokasikan memori untuk `doctorCapacity` dokter.
    *   Jika alokasi gagal, mencetak kesalahan, membebaskan memori pengguna dan pasien yang dialokasikan sebelumnya, dan mengembalikan `false`.
    *   Mengatur `hospital->doctors.capacity = doctorCapacity` dan `hospital->doctors.nEff = 0`.
6.  **Alokasi dan Inisialisasi `Layout` (`hospital->layout`)**:
    *   Mengalokasikan memori untuk `roomRows` pointer ke `Room` (yaitu, `Room **`).
    *   Jika alokasi gagal, mencetak kesalahan, membebaskan memori yang dialokasikan sebelumnya, dan mengembalikan `false`.
    *   Menginisialisasi setiap pointer baris ke `NULL`.
    *   Untuk setiap baris `i` dari `0` hingga `roomRows - 1`:
        *   Mengalokasikan memori untuk `roomCols` struktur `Room` (`hospital->layout.elements[i]`).
        *   Jika alokasi gagal:
            *   Mencetak kesalahan.
            *   Melakukan pembersihan cermat: membebaskan `patientInRoom.patientId` untuk ruangan yang sudah diinisialisasi di baris sebelumnya, membebaskan baris yang dialokasikan sebelumnya, kemudian membebaskan `hospital->layout.elements` itu sendiri, dan semua daftar lain yang dialokasikan di tingkat atas (pengguna, pasien, dokter). Mengembalikan `false`.
        *   Menginisialisasi `patientInRoom.patientId` untuk setiap ruangan di baris saat ini ke `NULL`.
        *   Untuk setiap kolom `j` dari `0` hingga `roomCols - 1` (inisialisasi ruangan individu):
            *   Menghasilkan `code` ruangan (misalnya, "A1", "B12") berdasarkan baris `i` dan kolom `j`.
            *   `strcpy(hospital->layout.elements[i][j].code, code);`
            *   `hospital->layout.elements[i][j].capacity = 3;` (kapasitas pasien default di ruangan).
            *   `hospital->layout.elements[i][j].doctorId = -1;` (tidak ada dokter yang ditugaskan).
            *   Mengalokasikan memori untuk array `patientInRoom.patientId` (kapasitas 3).
            *   Jika alokasi ini gagal, melakukan pembersihan ekstensif serupa dengan kegagalan alokasi baris (membebaskan array `patientId` individu di baris saat ini, baris saat ini itu sendiri, baris sebelumnya, array tata letak utama, dan semua daftar lain di tingkat atas). Mengembalikan `false`.
            *   Mengatur `patientInRoom.capacity = 3` dan `patientInRoom.nEff = 0`.
    *   Mengatur `hospital->layout.rowCapacity`, `colCapacity`, `rowEff`, `colEff` berdasarkan `roomRows` dan `roomCols`.
7.  **Alokasi dan Inisialisasi `HospitalQueueList` (`hospital->queues`)**:
    *   Mengalokasikan memori untuk `roomRows * roomCols` struktur `Queue`.
    *   Jika alokasi gagal, mencetak kesalahan, membersihkan `layout` dan daftar tingkat atas lainnya, dan mengembalikan `false`.
    *   Mengatur `hospital->queues.capacity = roomRows * roomCols` dan `hospital->queues.nRooms = 0`.
    *   Untuk setiap antrian, memanggil `initializeQueue(&hospital->queues.queues[i], "");` (menginisialisasi dengan kode ruangan kosong).
8.  **Alokasi dan Inisialisasi `DiseaseList` (`hospital->diseases`)**:
    *   Mengalokasikan memori untuk kapasitas awal tetap (5) struktur `Disease`.
    *   Jika alokasi gagal, mencetak kesalahan, membersihkan antrian, tata letak, dan daftar lainnya, dan mengembalikan `false`.
    *   Mengatur kapasitas dan `nEff` menjadi 5.
    *   Menyalin data penyakit yang telah ditentukan sebelumnya (ID, nama, dan berbagai rentang metrik kesehatan) ke dalam `hospital->diseases.elements`.
9.  **Alokasi dan Inisialisasi `MedicationList` (`hospital->medications`)**:
    *   Mengalokasikan memori untuk kapasitas awal tetap (5) struktur `Medication`.
    *   Jika alokasi gagal, membersihkan, mencetak kesalahan, dan mengembalikan `false`.
    *   Mengatur kapasitas dan `nEff` menjadi 5.
    *   Menyalin data obat yang telah ditentukan sebelumnya (ID, nama) ke dalam `hospital->medications.elements`.
10. **Alokasi dan Inisialisasi `PrescriptionList` (`hospital->prescriptions`)**:
    *   Mengalokasikan memori untuk kapasitas awal tetap (10) struktur `MedicationPrescription`.
    *   Jika alokasi gagal, membersihkan, mencetak kesalahan, dan mengembalikan `false`.
    *   Mengatur kapasitas dan `nEff` menjadi 10.
    *   Menyalin data resep yang telah ditentukan sebelumnya (ID penyakit, ID obat, kuantitas) ke dalam `hospital->prescriptions.elements`.
11. **Inisialisasi Keuangan Rumah Sakit**:
    *   `hospital->finance.hospitalBalance = 10000.0;`
12. **Alokasi dan Inisialisasi `TreatmentHistoryList` (`hospital->treatmentHistory`)**:
    *   Mengalokasikan memori untuk kapasitas awal (100) struktur `TreatmentHistory`.
    *   Jika alokasi gagal, membersihkan, mencetak kesalahan, dan mengembalikan `false`.
    *   Mengatur kapasitas menjadi 100 dan `nEff = 0`.
13. **Buat Pengguna Manajer Default**:
    *   Mengatur ID pengguna 1: `hospital->users.elements[0].id = 1;`.
    *   Mengatur nama pengguna: `strcpy(hospital->users.elements[0].username, "NimonsDawg");`.
    *   Mengenkripsi kata sandi "admoontothemoon" menggunakan `enigmaEncrypt` dan menyimpannya.
    *   Jika enkripsi gagal, mencetak kesalahan, memanggil `freeHospital(hospital)` untuk pembersihan, dan mengembalikan `false`.
    *   Mengatur peran: `hospital->users.elements[0].role = MANAGER;`.
    *   Menaikkan `hospital->users.nEff`.
14. **Sukses**:
    *   Mencetak "Rumah sakit berhasil diinisialisasi!".
    *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika inisialisasi berhasil, `false` jika tidak (biasanya karena kegagalan alokasi memori).

---

## Fungsi: `freeHospital`

### Tanda Tangan (Signature)
```c
void freeHospital(Hospital *hospital)
```

### Ringkasan
Membatalkan alokasi semua memori yang dialokasikan secara dinamis dalam struktur `Hospital`. Ini termasuk array untuk pengguna, pasien (dan daftar obat mereka), dokter, tata letak ruangan (dan array ID pasien di setiap ruangan), antrian (dan node mereka), penyakit, obat-obatan, resep, dan riwayat perawatan.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur `Hospital` yang memorinya akan dibebaskan.

### Algoritma
1.  **Bebaskan Pengguna**: Jika `hospital->users.elements` bukan `NULL`, bebaskan dan atur ke `NULL`.
2.  **Bebaskan Pasien**: Jika `hospital->patients.elements` bukan `NULL`:
    *   Melakukan iterasi melalui setiap pasien (`i` dari `0` hingga `hospital->patients.nEff - 1`):
        *   Membebaskan `hospital->patients.elements[i].medicationsPrescribed.medicationId` jika bukan `NULL`.
        *   Membebaskan `hospital->patients.elements[i].medicationsTaken.medicationId` jika bukan `NULL`.
    *   Membebaskan `hospital->patients.elements` itu sendiri dan mengatur ke `NULL`.
3.  **Bebaskan Dokter**: Jika `hospital->doctors.elements` bukan `NULL`, bebaskan dan atur ke `NULL`.
4.  **Bebaskan Tata Letak**: Jika `hospital->layout.elements` bukan `NULL`:
    *   Melakukan iterasi melalui setiap baris `i` dari `0` hingga `hospital->layout.rowCapacity - 1`:
        *   Jika `hospital->layout.elements[i]` (pointer baris) bukan `NULL`:
            *   Melakukan iterasi melalui setiap kolom `j` dari `0` hingga `hospital->layout.colCapacity - 1`:
                *   Membebaskan `hospital->layout.elements[i][j].patientInRoom.patientId` jika bukan `NULL`.
            *   Membebaskan `hospital->layout.elements[i]` (baris itu sendiri) dan mengatur ke `NULL`.
    *   Membebaskan `hospital->layout.elements` (array pointer baris) dan mengatur ke `NULL`.
5.  **Bebaskan Antrian**: Jika `hospital->queues.queues` bukan `NULL`:
    *   Melakukan iterasi melalui setiap antrian `i` dari `0` hingga `hospital->queues.capacity - 1`:
        *   Melintasi daftar tertaut `QueueNode` untuk antrian saat ini (`hospital->queues.queues[i].head`).
        *   Membebaskan setiap `QueueNode` secara individual.
        *   Mengatur `head`, `tail` ke `NULL` dan `size` menjadi `0` untuk antrian saat ini.
    *   Membebaskan `hospital->queues.queues` (array antrian) dan mengatur ke `NULL`.
6.  **Bebaskan Penyakit**: Jika `hospital->diseases.elements` bukan `NULL`, bebaskan dan atur ke `NULL`.
7.  **Bebaskan Obat-obatan**: Jika `hospital->medications.elements` bukan `NULL`, bebaskan dan atur ke `NULL`.
8.  **Bebaskan Resep**: Jika `hospital->prescriptions.elements` bukan `NULL`, bebaskan dan atur ke `NULL`.
9.  **Bebaskan Riwayat Perawatan**: Jika `hospital->treatmentHistory.elements` bukan `NULL`, bebaskan dan atur ke `NULL`.
10. **Pesan Sukses**: Mencetak "Memori rumah sakit berhasil dibebaskan!".

### Nilai Kembali
*   `void`: Fungsi ini tidak mengembalikan nilai.

---

## Fungsi: `deletePatient`

### Tanda Tangan (Signature)
```c
boolean deletePatient(Hospital *hospital, int patientId)
```

### Ringkasan
Menghapus pasien dari sistem rumah sakit berdasarkan `patientId` mereka. Ini melibatkan penghapusan catatan pasien dari daftar pasien utama, catatan pengguna yang sesuai dari daftar pengguna, mengeluarkan mereka dari antrian ruangan mana pun, menghapus mereka dari slot ruangan fisik mana pun, dan membersihkan entri riwayat perawatan mereka.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `int patientId`: ID pasien yang akan dihapus.

### Algoritma
1.  **Cari Pasien dalam Daftar Pasien**:
    *   Menginisialisasi `patientIndex = -1` dan `char patientUsername[50] = "";`.
    *   Melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien dengan `id == patientId`.
    *   Jika ditemukan, mengatur `patientIndex` dan menyalin `patientUsername`.
    *   Jika tidak ditemukan (`patientIndex == -1`), mencetak kesalahan ("Pasien tidak ditemukan!") dan mengembalikan `false`.
2.  **Bebaskan Daftar Obat Pasien**:
    *   Membebaskan `hospital->patients.elements[patientIndex].medicationsPrescribed.medicationId` jika bukan `NULL`.
    *   Membebaskan `hospital->patients.elements[patientIndex].medicationsTaken.medicationId` jika bukan `NULL`.
3.  **Hapus Pasien dari Daftar Pasien**:
    *   Menggeser elemen dalam `hospital->patients.elements` dari `patientIndex` dan seterusnya untuk mengisi celah.
    *   Mengurangi `hospital->patients.nEff`.
4.  **Cari dan Hapus Pengguna yang Sesuai dari Daftar Pengguna**:
    *   Menginisialisasi `userIndex = -1`.
    *   Melakukan iterasi melalui `hospital->users.elements` untuk menemukan pengguna dengan `id == patientId`.
    *   Jika ditemukan (`userIndex != -1`):
        *   Menggeser elemen dalam `hospital->users.elements` dari `userIndex` dan seterusnya.
        *   Mengurangi `hospital->users.nEff`.
5.  **Hapus Pasien dari Antrian**:
    *   Melakukan iterasi melalui semua antrian di `hospital->queues.queues`.
    *   Untuk setiap antrian `q`:
        *   Lewati jika `q->roomCode` kosong atau antrian kosong.
        *   Melintasi daftar tertaut antrian:
            *   Jika `info.patientId` dari `QueueNode` adalah `patientId`, hapus node dari daftar tertaut (menangani kasus head, tail, tengah).
            *   Bebaskan node yang dihapus.
            *   Kurangi `q->size`.
            *   Atur `removed = true` dan hentikan dari traversal antrian saat ini.
        *   Jika `removed` adalah true, hentikan dari loop luar (mengasumsikan pasien paling banyak berada dalam satu antrian).
6.  **Hapus Pasien dari Slot Ruangan (Hunian Fisik)**:
    *   Melakukan iterasi melalui semua ruangan di `hospital->layout.elements`.
    *   Untuk setiap `Room *room`:
        *   Melakukan iterasi melalui `room->patientInRoom.patientId` (hingga `room->patientInRoom.nEff`).
        *   Jika `patientId` ditemukan pada indeks `k`:
            *   Menggeser elemen dalam `room->patientInRoom.patientId` dari `k` dan seterusnya.
            *   Mengurangi `room->patientInRoom.nEff`.
            *   Hentikan dari iterasi slot pasien ruangan ini.
7.  **Hapus Pasien dari Riwayat Perawatan**:
    *   Menginisialisasi `historyIndex = -1`.
    *   Melakukan iterasi melalui `hospital->treatmentHistory.elements` untuk menemukan entri dengan `patientId`.
    *   Jika ditemukan pada `historyIndex`:
        *   Menggeser elemen dalam `hospital->treatmentHistory.elements` dari `historyIndex` dan seterusnya.
        *   Mengurangi `hospital->treatmentHistory.nEff`.
        *   (Catatan: Ini hanya menghapus entri riwayat *pertama* yang ditemukan. Jika beberapa entri untuk pasien yang sama dapat ada, ini mungkin tidak cukup.)
8.  **Pesan Sukses**:
    *   Membuat dan mencetak pesan sukses: "Pasien dengan username '[patientUsername]' berhasil dihapus dari sistem!".
    *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika penghapusan pasien berhasil, `false` jika pasien tidak ditemukan.
