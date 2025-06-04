# Berkas: `src/c/dynamicLayout.c`

Berkas ini mengelola tata letak dinamis rumah sakit, termasuk mengubah dimensi tata letak ruangan secara keseluruhan dan memindahkan dokter antar ruangan.

## Fungsi: `changeLayout`

### Tanda Tangan (Signature)
```c
boolean changeLayout(Hospital *hospital, Session *session, int newRowCount, int newColCount)
```

### Ringkasan
Memungkinkan seorang `MANAGER` untuk mengubah dimensi (baris dan kolom) dari tata letak rumah sakit. Fungsi ini mengalokasikan ulang memori untuk tata letak baru, menginisialisasi ruangan baru, dan mencoba mempertahankan penugasan dokter dan penempatan pasien yang ada jika mereka berada dalam dimensi baru. Fungsi ini mencegah pengecilan ukuran jika ruangan yang akan dihapus saat ini ditempati oleh dokter.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`. Ini akan diubah untuk mencerminkan tata letak baru.
*   `Session *session`: Pointer ke `Session` pengguna saat ini untuk memverifikasi hak istimewa manajer.
*   `int newRowCount`: Jumlah baris yang diinginkan untuk tata letak baru.
*   `int newColCount`: Jumlah kolom yang diinginkan untuk tata letak baru.

### Algoritma
1.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan `session->role == MANAGER`.
    *   Jika tidak diotorisasi, mencetak kesalahan ("Akses ditolak! Hanya Manajer yang dapat mengubah denah.") dan mengembalikan `false`.
2.  **Validasi Dimensi Baru**:
    *   Memeriksa apakah `newRowCount <= 0` atau `newColCount <= 0`.
    *   Jika tidak valid, mencetak kesalahan ("Ukuran denah tidak valid!") dan mengembalikan `false`.
3.  **Periksa Ruangan yang Ditempati di Area yang Menyusut**:
    *   Melakukan iterasi melalui tata letak saat ini (`hospital->layout.elements`).
    *   Untuk setiap ruangan di `(i, j)` yang akan berada di luar dimensi baru (`i >= newRowCount` atau `j >= newColCount`):
        *   Jika `room->doctorId != -1` (ruangan ditempati oleh dokter):
            *   Menemukan nama pengguna dokter.
            *   Mencetak pesan kesalahan (misalnya, "Ruangan [room->code] masih ditempati oleh Dr.[nama pengguna dokter]").
            *   Mengembalikan `false` (tidak dapat mengecilkan tata letak jika itu menghapus ruangan yang ditempati).
4.  **Alokasikan Memori Tata Letak Baru (Baris)**:
    *   `Room **newLayout = (Room **)safeMalloc(newRowCount * sizeof(Room *));`
    *   Jika alokasi gagal, mencetak kesalahan ("Gagal alokasi memori untuk layout baru (rows)!") dan mengembalikan `false`.
5.  **Alokasikan Memori Tata Letak Baru (Kolom) dan Inisialisasi Ruangan**:
    *   Melakukan iterasi `i` dari `0` hingga `newRowCount - 1`:
        *   `newLayout[i] = (Room *)safeMalloc(newColCount * sizeof(Room));`
        *   Jika alokasi gagal:
            *   Mencetak kesalahan ("Gagal alokasi memori untuk layout baru (cols)!").
            *   Membebaskan baris yang dialokasikan sebelumnya (`newLayout[k]` untuk `k < i`).
            *   Membebaskan `newLayout` itu sendiri.
            *   Mengembalikan `false`.
        *   Melakukan iterasi `j` dari `0` hingga `newColCount - 1` (menginisialisasi setiap ruangan baru):
            *   Menghasilkan `code` (misalnya, "A1", "A2", ... "B1", ...): `code[0] = 'A' + i; integerToString(j + 1, ...); strcat(code, ...);`.
            *   `strcpy(newLayout[i][j].code, code);`
            *   `newLayout[i][j].capacity = 3;` (kapasitas default).
            *   `newLayout[i][j].doctorId = -1;` (awalnya tidak ada dokter).
            *   `newLayout[i][j].patientInRoom.patientId = (int *)safeMalloc(3 * sizeof(int));` (mengalokasikan ruang untuk pasien di ruangan).
            *   (Catatan: `patientInRoom.nEff` secara implisit adalah 0).
6.  **Salin Data yang Ada ke Tata Letak Baru**:
    *   Melakukan iterasi `i` dari `0` hingga `min(hospital->layout.rowEff, newRowCount) - 1`.
    *   Melakukan iterasi `j` dari `0` hingga `min(hospital->layout.colEff, newColCount) - 1`.
        *   `newLayout[i][j].doctorId = hospital->layout.elements[i][j].doctorId;`
        *   `newLayout[i][j].patientInRoom.nEff = hospital->layout.elements[i][j].patientInRoom.nEff;`
        *   Menyalin `patientId` dari ruangan tata letak lama ke ruangan tata letak baru.
7.  **Bebaskan Memori Tata Letak Lama**:
    *   Melakukan iterasi melalui `hospital->layout.elements`:
        *   Untuk setiap ruangan, membebaskan `hospital->layout.elements[i][j].patientInRoom.patientId` jika bukan `NULL`.
        *   Membebaskan setiap baris `hospital->layout.elements[i]` jika bukan `NULL`.
    *   Membebaskan `hospital->layout.elements` itu sendiri jika bukan `NULL`.
8.  **Perbarui Struktur Rumah Sakit dengan Tata Letak Baru**:
    *   `hospital->layout.elements = newLayout;`
    *   `hospital->layout.rowEff = newRowCount;`
    *   `hospital->layout.colEff = newColCount;`
    *   `hospital->layout.rowCapacity = newRowCount;`
    *   `hospital->layout.colCapacity = newColCount;`
9.  **Sukses**:
    *   Mencetak pesan sukses ("Denah rumah sakit berhasil diubah!").
    *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika perubahan tata letak berhasil, `false` jika tidak.

---

## Fungsi: `moveDoctor`

### Tanda Tangan (Signature)
```c
boolean moveDoctor(Hospital *hospital, Session *session, const char *username, const char *newRoomCode)
```

### Ringkasan
Memungkinkan seorang `MANAGER` untuk memindahkan dokter tertentu dari ruangannya saat ini ke ruangan baru yang tidak ditempati. Pasien yang saat ini berada di ruangan asli dokter (slot langsung, belum tentu antrian yang lebih luas untuk ruangan itu) dipindahkan ke ruangan baru jika ruang memungkinkan. Antrian umum yang terkait dengan ruangan lama juga diperbarui untuk menunjuk ke ruangan baru.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Session *session`: Pointer ke `Session` pengguna saat ini untuk otorisasi.
*   `const char *username`: Nama pengguna dokter yang akan dipindahkan.
*   `const char *newRoomCode`: Kode ruangan tujuan (misalnya, "A1").

### Algoritma
1.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn` dan `session->role == MANAGER`.
    *   Jika tidak diotorisasi, mencetak kesalahan ("Akses ditolak! Hanya Manajer yang dapat memindahkan dokter.") dan mengembalikan `false`.
2.  **Cari Dokter**:
    *   Menginisialisasi `Doctor *doctorToMove = NULL;`.
    *   Melakukan iterasi melalui `hospital->doctors.elements` untuk menemukan dokter berdasarkan `username`.
    *   Jika tidak ditemukan, mencetak kesalahan ("Dokter tidak ditemukan!") dan mengembalikan `false`.
3.  **Validasi Kode Ruangan Target**:
    *   Mengurai `newRoomCode` menjadi `targetRow` (misalnya, 'A' -> 0) dan `targetCol` (misalnya, "1" -> 0).
    *   Memeriksa apakah `targetRow` dan `targetCol` berada dalam batas `hospital->layout`.
    *   Jika tidak valid, mencetak kesalahan ("Kode ruangan tidak valid!") dan mengembalikan `false`.
4.  **Periksa Ketersediaan Ruangan Target**:
    *   `Room *targetRoom = &hospital->layout.elements[targetRow][targetCol];`
    *   Jika `targetRoom->doctorId != -1` (ruangan target ditempati), mencetak kesalahan ("Pemindahan gagal. Ruangan tersebut sudah ditempati.") dan mengembalikan `false`.
5.  **Cari Ruangan Dokter Saat Ini**:
    *   Menginisialisasi `Room *currentRoom = NULL;`.
    *   Melakukan iterasi melalui `hospital->layout.elements` untuk menemukan ruangan tempat `doctorId == doctorToMove->id`.
    *   Jika `currentRoom` tidak ditemukan (dokter tidak ditugaskan ke ruangan), mencetak kesalahan ("Dokter tidak memiliki ruangan."). (Catatan: Ini mungkin berlebihan jika dokter selalu memiliki ruangan atau string kosong jika tidak ditugaskan, yang seharusnya ditangani oleh `doctorToMove->room`).
6.  **Perbarui Penugasan Ruangan**:
    *   `currentRoom->doctorId = -1;` (kosongkan ruangan lama).
    *   `targetRoom->doctorId = doctorToMove->id;` (tugaskan dokter ke ruangan baru).
    *   `strcpy(doctorToMove->room, newRoomCode);` (perbarui catatan dokter).
7.  **Pindahkan Pasien dari Slot Langsung Ruangan Lama ke Ruangan Baru**:
    *   Melakukan iterasi melalui pasien di `currentRoom->patientInRoom.patientId` (hingga `currentRoom->patientInRoom.nEff`).
    *   Untuk setiap `patientId`:
        *   Menemukan `Patient *patientToMove` dari `hospital->patients.elements`.
        *   Jika `patientToMove` ditemukan:
            *   Mencoba menambahkan `patientId` ke slot kosong di `targetRoom->patientInRoom.patientId` (hingga `targetRoom->patientInRoom.capacity`).
            *   Jika ditambahkan, menaikkan `targetRoom->patientInRoom.nEff`.
    *   `currentRoom->patientInRoom.nEff = 0;` (kosongkan pasien dari slot langsung ruangan lama).
8.  **Perbarui Antrian Umum**:
    *   Melakukan iterasi melalui `hospital->queues.queues` (antrian umum untuk ruangan).
    *   Jika `queue->roomCode` cocok dengan `currentRoom->code`:
        *   `strcpy(queue->roomCode, newRoomCode);` (mengaitkan kembali antrian dengan ruangan baru).
9.  **Sukses**:
    *   Mencetak pesan sukses ("Dokter dan pasien berhasil dipindahkan!").
    *   Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika pemindahan dokter berhasil, `false` jika tidak.
