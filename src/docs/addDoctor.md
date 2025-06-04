# Berkas: `src/c/addDoctor.c`

Berkas ini berisi fungsi-fungsi yang berkaitan dengan pengelolaan dokter dalam sistem rumah sakit, khususnya menambahkan dokter baru dan menugaskan mereka ke ruangan.

## Fungsi: `addDoctor`

### Tanda Tangan (Signature)
```c
boolean addDoctor(Hospital *hospital, Session *session, const char *inputUsername, const char *password, const char *specialization, const float checkupCost)
```

### Ringkasan
Menambahkan pengguna dokter baru ke sistem rumah sakit. Fungsi ini terbatas untuk pengguna dengan peran `MANAGER` yang saat ini sedang login. Fungsi ini menangani validasi input pengguna, pengubahan ukuran dinamis array pengguna dan dokter jika perlu, enkripsi kata sandi, dan pembaruan struktur data rumah sakit.

### Parameter
*   `Hospital *hospital`: Sebuah pointer ke struktur data utama `Hospital`. Struktur ini berisi daftar pengguna dan dokter, di antara data terkait rumah sakit lainnya. Fungsi ini akan mengubah daftar-daftar ini.
*   `Session *session`: Sebuah pointer ke struktur `Session`, yang menyimpan informasi tentang sesi pengguna saat ini, termasuk status login dan peran.
*   `const char *inputUsername`: Sebuah string yang merepresentasikan nama pengguna yang diinginkan untuk dokter baru. Nama pengguna ini akan divalidasi format dan keunikannya.
*   `const char *password`: Sebuah string yang merepresentasikan kata sandi untuk dokter baru. Kata sandi ini akan divalidasi panjang minimumnya dan kemudian dienkripsi sebelum disimpan.
*   `const char *specialization`: Sebuah string yang merepresentasikan spesialisasi medis dokter baru (misalnya, "Kardiologi", "Pediatri"). Ini akan divalidasi formatnya.
*   `const float checkupCost`: Angka floating-point yang merepresentasikan biaya pemeriksaan yang dilakukan oleh dokter ini.

### Algoritma
1.  **Validasi Input (Pointer)**: Memeriksa apakah `hospital`, `session`, `inputUsername`, `password`, atau `specialization` adalah `NULL`. Jika ada yang `NULL`, mencetak pesan kesalahan ("Struktur rumah sakit, sesi, atau input tidak valid!") dan mengembalikan `false`.
2.  **Pemeriksaan Otorisasi**: Memverifikasi apakah pengguna yang melakukan tindakan sedang login (`session->isLoggedIn`) dan memiliki peran `MANAGER` (`session->role != MANAGER`). Jika tidak, mencetak pesan kesalahan ("Akses ditolak! Hanya Manajer yang dapat menambah dokter.") dan mengembalikan `false`.
3.  **Validasi Nama Pengguna**: Memanggil `isValidUsername(inputUsername)` untuk memeriksa apakah nama pengguna yang diberikan sesuai dengan aturan karakter yang diizinkan (huruf, angka, spasi, garis bawah). Jika tidak valid, mencetak pesan kesalahan ("Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore.") dan mengembalikan `false`.
4.  **Pemeriksaan Keunikan Nama Pengguna**: Memanggil `isUsernameTaken(hospital, inputUsername)` untuk memastikan nama pengguna belum terdaftar di sistem. Jika sudah terdaftar, mencetak pesan kesalahan ("Username sudah terdaftar!") dan mengembalikan `false`.
5.  **Validasi Panjang Kata Sandi**: Memeriksa apakah panjang `password` kurang dari 6 karakter menggunakan `strlen(password) < 6`. Jika terlalu pendek, mencetak pesan kesalahan ("Password harus minimal 6 karakter!") dan mengembalikan `false`.
6.  **Validasi Spesialisasi**: Memanggil `isValidUsername(specialization)` (menggunakan kembali logika validasi nama pengguna) untuk memeriksa format string spesialisasi. Jika tidak valid, mencetak pesan kesalahan ("Spesialisasi tidak valid! Gunakan huruf, angka, atau underscore.") dan mengembalikan `false`.
7.  **Pemeriksaan Kapasitas Array dan Ekspansi**:
    *   Memeriksa apakah jumlah pengguna (`hospital->users.nEff`) telah mencapai kapasitas (`hospital->users.capacity`) atau jika jumlah dokter (`hospital->doctors.nEff`) telah mencapai kapasitas (`hospital->doctors.capacity`).
    *   Jika salah satu array penuh:
        *   Menghitung `newCapacity` untuk pengguna (kapasitas saat ini \* 2).
        *   Mencoba `realloc` `hospital->users.elements` ke `newCapacity` ini. Jika `realloc` gagal, mencetak kesalahan ("Gagal mengalokasi memori untuk ekspansi pengguna!") dan mengembalikan `false`. Memperbarui `hospital->users.capacity`.
        *   Menghitung `newDoctorCapacity` untuk dokter (kapasitas saat ini \* 2).
        *   Mencoba `realloc` `hospital->doctors.elements` ke `newDoctorCapacity`. Jika `realloc` gagal, mencetak kesalahan ("Gagal mengalokasi memori untuk ekspansi dokter!") dan mengembalikan `false`. Memperbarui `hospital->doctors.capacity`.
8.  **Menentukan ID Dokter Baru**: Melakukan iterasi melalui semua pengguna yang ada (`hospital->users.elements`) untuk menemukan `id` maksimum saat ini. `newDoctorId` diatur ke `maxId + 1`.
9.  **Mengisi Data Pengguna Baru**:
    *   Mendapatkan pointer ke slot berikutnya yang tersedia di array pengguna: `User *newUser = &hospital->users.elements[hospital->users.nEff];`.
    *   Mengatur `newUser->id = newDoctorId;`.
    *   Menyalin `inputUsername` ke `newUser->username` menggunakan `strcpy`.
    *   Mengenkripsi `password` menggunakan `enigmaEncrypt(password, newUser->password.encryptedContent, 100)`. Jika enkripsi gagal (`!enigmaEncrypt(...)`), mencetak kesalahan ("Gagal mengenkripsi password!") dan mengembalikan `false`.
    *   Mengatur `newUser->role = DOCTOR;`.
10. **Mengisi Data Dokter Baru**:
    *   Mendapatkan pointer ke slot berikutnya yang tersedia di array dokter: `Doctor *newDoctor = &hospital->doctors.elements[hospital->doctors.nEff];`.
    *   Mengatur `newDoctor->id = newDoctorId;`.
    *   Menyalin `inputUsername` ke `newDoctor->username` menggunakan `strcpy`.
    *   Menyalin `specialization` ke `newDoctor->specialization` menggunakan `strcpy`.
    *   Menginisialisasi `newDoctor->aura = 0;`.
    *   Menginisialisasi `newDoctor->bananaRich = 100.0f;`.
    *   Menginisialisasi `newDoctor->room[0] = '\0';` (menunjukkan belum ada ruangan yang ditugaskan).
    *   Mengatur `newDoctor->checkupCost = checkupCost;`.
11. **Menambah Jumlah**: Meningkatkan jumlah efektif pengguna dan dokter: `hospital->users.nEff++;` dan `hospital->doctors.nEff++;`.
12. **Pesan Sukses**: Membuat dan mencetak pesan sukses (misalnya, "Dokter [inputUsername] berhasil ditambahkan!").
13. **Nilai Kembali**: Mengembalikan `true` yang menunjukkan penambahan dokter berhasil.

---

## Fungsi: `assignDoctor`

### Tanda Tangan (Signature)
```c
boolean assignDoctor(Hospital *hospital, Session *session, const char *username, const char *roomCode)
```

### Ringkasan
Menugaskan dokter yang sudah ada ke ruangan tertentu dalam tata letak rumah sakit. Operasi ini terbatas untuk pengguna yang login dengan peran `MANAGER`. Fungsi ini memvalidasi input, memeriksa keberadaan dokter dan ruangan, serta menangani berbagai skenario konflik seperti dokter sudah ditugaskan atau ruangan sudah ditempati.

### Parameter
*   `Hospital *hospital`: Sebuah pointer ke struktur data utama `Hospital`. Ini digunakan untuk mengakses daftar dokter dan tata letak ruangan rumah sakit.
*   `Session *session`: Sebuah pointer ke struktur `Session`, digunakan untuk memverifikasi status login dan peran pengguna saat ini.
*   `const char *username`: Sebuah string yang merepresentasikan nama pengguna dokter yang akan ditugaskan ke sebuah ruangan.
*   `const char *roomCode`: Sebuah string yang merepresentasikan kode ruangan tempat dokter akan ditugaskan (misalnya, "A1", "B3").

### Algoritma
1.  **Validasi Input (Pointer)**: Memeriksa apakah `hospital`, `session`, `username`, atau `roomCode` adalah `NULL`. Jika ada yang `NULL`, mencetak pesan kesalahan ("Struktur rumah sakit, sesi, atau input tidak valid!") dan mengembalikan `false`.
2.  **Pemeriksaan Otorisasi**: Memverifikasi apakah pengguna sedang login (`session->isLoggedIn`) dan memiliki peran `MANAGER` (`session->role != MANAGER`). Jika tidak, mencetak pesan kesalahan ("Akses ditolak! Hanya Manajer yang dapat menugaskan dokter.") dan mengembalikan `false`.
3.  **Validasi Nama Pengguna Dokter**: Memanggil `isValidUsername(username)` untuk memeriksa format nama pengguna dokter. Jika tidak valid, mencetak pesan kesalahan ("Username dokter tidak valid!") dan mengembalikan `false`.
4.  **Validasi Kode Ruangan**: Memanggil `isValidRoomCode(hospital, roomCode)` untuk memeriksa apakah `roomCode` valid (misalnya, ada dalam tata letak rumah sakit dan mengikuti aturan format). Jika tidak valid, mencetak pesan kesalahan ("Kode ruangan tidak valid! Contoh: A1") dan mengembalikan `false`.
5.  **Cari Dokter**:
    *   Menginisialisasi `doctorIdx = -1`.
    *   Melakukan iterasi melalui `hospital->doctors.elements` hingga `hospital->doctors.nEff`.
    *   Menggunakan `caseInsensitiveStrcmp` untuk membandingkan `hospital->doctors.elements[i].username` dengan input `username`.
    *   Jika ditemukan kecocokan, mengatur `doctorIdx = i` dan menghentikan loop.
    *   Jika `doctorIdx` tetap -1 setelah loop, dokter tidak ditemukan. Mencetak pesan kesalahan ("Dokter tidak ditemukan!") dan mengembalikan `false`.
6.  **Cari Ruangan**:
    *   Menginisialisasi `Room *room = NULL`.
    *   Memeriksa apakah tata letak rumah sakit telah ditentukan (`hospital->layout.rowEff > 0 && hospital->layout.colEff > 0`).
    *   Melakukan iterasi melalui `hospital->layout.elements` (array 2D dari struktur `Room`).
    *   Membandingkan `hospital->layout.elements[i][j].code` dengan input `roomCode` menggunakan `strcmp`.
    *   Jika ditemukan kecocokan, mengatur `room` untuk menunjuk ke `&hospital->layout.elements[i][j]` dan menghentikan loop dalam dan luar.
    *   Jika `room` masih `NULL` setelah loop, ruangan tidak ditemukan. Mencetak pesan kesalahan ("Ruangan tidak ditemukan!") dan mengembalikan `false`.
7.  **Pengecekan Konflik**:
    *   Mendapatkan pointer ke dokter yang ditemukan: `Doctor *doctor = &hospital->doctors.elements[doctorIdx];`.
    *   Menentukan apakah dokter sudah ditugaskan: `boolean doctorAlreadyAssigned = (doctor->room[0] != '\0');`.
    *   Menentukan apakah ruangan target sudah ditempati: `boolean roomOccupied = (room->doctorId != -1);`.
8.  **Tangani Konflik**:
    *   **Skenario 1: Dokter sudah ditugaskan DAN ruangan sudah ditempati.**
        *   Mencetak dua pesan kesalahan:
            1.  "Dokter [doctor->username] sudah menempati ruangan [doctor->room]!"
            2.  Menemukan nama dokter yang menempati ruangan target (`occupyingDoctorName`).
            3.  "Ruangan [roomCode] juga sudah ditempati dokter [occupyingDoctorName]!"
        *   Mengembalikan `false`.
    *   **Skenario 2: Dokter sudah ditugaskan (tetapi ruangan target kosong).**
        *   Mencetak pesan kesalahan: "Dokter [doctor->username] sudah diassign ke ruangan [doctor->room]!"
        *   Mengembalikan `false`.
    *   **Skenario 3: Ruangan sudah ditempati (oleh dokter lain, dan dokter saat ini belum ditugaskan di tempat lain).**
        *   Menemukan nama dokter yang menempati ruangan target (`occupyingDoctorName`).
        *   Mencetak dua pesan kesalahan:
            1.  "Dokter [occupyingDoctorName] sudah menempati ruangan [roomCode]!"
            2.  "Silakan cari ruangan lain untuk dokter [doctor->username]."
        *   Mengembalikan `false`.
9.  **Tugaskan Dokter ke Ruangan (Tidak Adgia Konflik)**:
    *   Mengatur `room->doctorId = doctor->id;`.
    *   Menyalin `roomCode` ke `doctor->room` menggunakan `strcpy`.
10. **Pesan Sukses**: Membuat dan mencetak pesan sukses (misalnya, "Dokter [doctor->username] berhasil diassign ke ruangan [roomCode]!").
11. **Nilai Kembali**: Mengembalikan `true` yang menunjukkan penugasan berhasil.
