# Berkas: `src/c/queueManagement.c`

Berkas ini mengelola operasi antrian pasien dalam sistem rumah sakit. Ini termasuk menemukan antrian, memperbarui posisi pasien dalam antrian, memungkinkan pasien dipindahkan ke depan antrian (dilewati), dan menghapus pasien dari antrian (membatalkan).

## Fungsi Pembantu: `findQueueByRoomCode`

### Tanda Tangan (Signature)
```c
static Queue *findQueueByRoomCode(Hospital *hospital, const char *roomCode)
```

### Ringkasan
Fungsi pembantu statis yang mencari struktur `Queue` yang terkait dengan `roomCode` tertentu dalam daftar `hospital->queues`.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `const char *roomCode`: Kode ruangan yang akan dicari.

### Algoritma
1.  **Validasi Input**:
    *   Jika `hospital` adalah `NULL`, `roomCode` adalah `NULL`, atau `roomCode` adalah string kosong, mengembalikan `NULL`.
2.  **Cari Antrian**:
    *   Melakukan iterasi melalui `hospital->queues.queues` (hingga `hospital->queues.capacity`).
    *   Untuk setiap antrian, memeriksa apakah `roomCode`-nya tidak kosong dan cocok dengan `roomCode` input menggunakan `strcmp`.
    *   Jika kecocokan ditemukan, mengembalikan pointer ke struktur `Queue` tersebut.
3.  **Tidak Ditemukan**: Jika loop selesai tanpa kecocokan, mengembalikan `NULL`.

### Nilai Kembali
*   `Queue *`: Pointer ke `Queue` yang ditemukan jika berhasil, jika tidak `NULL`.

## Fungsi Pembantu: `updatePatientPositionsInQueue`

### Tanda Tangan (Signature)
```c
static void updatePatientPositionsInQueue(Hospital *hospital, Queue *q)
```

### Ringkasan
Fungsi pembantu statis yang memperbarui field `queuePosition` untuk semua pasien. Untuk pasien yang saat ini terdaftar dalam antrian `q` yang diberikan, `queuePosition` mereka diatur sesuai dengan urutan aktual mereka dalam antrian (ke-1, ke-2, dst.). Untuk pasien yang sebelumnya terkait dengan ruangan antrian ini tetapi tidak lagi ditemukan dalam struktur antrian itu sendiri, `queuePosition` mereka direset menjadi 0.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`.
*   `Queue *q`: Pointer ke `Queue` spesifik yang posisi pasiennya perlu diperbarui.

### Algoritma
1.  **Validasi Input**:
    *   Jika `q` adalah `NULL` atau `hospital` adalah `NULL`, segera kembali.
2.  **Reset Posisi untuk Pasien yang Tidak Lagi dalam Antrian**:
    *   Melakukan iterasi melalui semua pasien di `hospital->patients.elements`.
    *   Jika `queueRoom` pasien cocok dengan `q->roomCode`:
        *   Memeriksa apakah ID pasien ini masih ada di antrian aktual `q` dengan memindai `q->head` melalui daftar tertautnya.
        *   Jika pasien *tidak* ditemukan dalam struktur antrian (`stillInQueue == false`), `hospital->patients.elements[i].queuePosition` mereka diatur menjadi `0`.
3.  **Perbarui Posisi untuk Pasien yang Saat Ini dalam Antrian**:
    *   Jika `isQueueEmpty(q)` adalah true, kembali.
    *   Menginisialisasi `QueueNode *currentNode = q->head;` dan `int pos = 1;`.
    *   Melakukan iterasi melalui daftar tertaut antrian (`currentNode != NULL`):
        *   Untuk setiap `currentNode->info.patientId`, melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien yang cocok.
        *   Jika pasien ditemukan dan `queueRoom` mereka cocok dengan `q->roomCode`, mengatur `hospital->patients.elements[i].queuePosition = pos;`.
        *   Pindah ke `currentNode = currentNode->next;` dan menaikkan `pos`.

### Nilai Kembali
*   `void`.

## Fungsi Utama: `skipPatientInQueue`

### Tanda Tangan (Signature)
```c
boolean skipPatientInQueue(Hospital *hospital, Session *session, const char *patientUsername)
```

### Ringkasan
Memungkinkan `MANAGER` yang login atau `PATIENT` tertentu itu sendiri untuk memindahkan pasien tersebut ke depan antrian mereka saat ini.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data `Hospital`.
*   `Session *session`: Pointer ke `Session` pengguna saat ini untuk otorisasi.
*   `const char *patientUsername`: Nama pengguna pasien yang akan dipindahkan ke depan antrian.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah `hospital`, `session`, atau `patientUsername` adalah `NULL`. Mencetak kesalahan dan mengembalikan `false` jika ada.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn`.
    *   Memeriksa apakah `session->role` adalah `MANAGER` atau `PATIENT`.
    *   Jika tidak diotorisasi, mencetak kesalahan dan mengembalikan `false`.
3.  **Cari Pasien dan Antrian Mereka**:
    *   Menginisialisasi `patientId = -1` dan `roomCode[0] = '\0'`.
    *   Melakukan iterasi melalui `hospital->patients.elements` untuk menemukan pasien berdasarkan `patientUsername`.
        *   Jika ditemukan:
            *   Jika `patient->queuePosition == 0` (sudah di dalam ruangan), mencetak kesalahan ("Anda sudah berada didalam ruangan dokter...") dan mengembalikan `false`.
            *   Mengatur `patientId` dan menyalin `patient->queueRoom` ke `roomCode`. Hentikan loop.
    *   Jika `patientId == -1` (pasien tidak ditemukan), mencetak kesalahan dan mengembalikan `false`.
    *   Jika `roomCode[0] == '\0'` (pasien tidak dalam antrian apa pun), mencetak kesalahan dan mengembalikan `false`.
4.  **Cari Struktur Antrian**:
    *   `Queue *q = findQueueByRoomCode(hospital, roomCode);`.
    *   Jika `q == NULL`, mencetak kesalahan (antrian untuk ruangan terdaftar pasien tidak ditemukan) dan mengembalikan `false`.
    *   Jika `isQueueEmpty(q)`, mencetak kesalahan (antrian kosong) dan mengembalikan `false`.
5.  **Temukan Pasien dalam Daftar Tertaut Antrian**:
    *   Menginisialisasi `targetNode = NULL`, `prevNode = NULL`.
    *   Melintasi antrian `q` untuk menemukan `QueueNode` di mana `current->info.patientId == patientId`. `prevNode` melacak node sebelum `targetNode`.
    *   Jika `targetNode == NULL` setelah traversal (pasien terdaftar ke antrian tetapi tidak ditemukan dalam strukturnya), mencetak kesalahan (data mungkin rusak) dan mengembalikan `false`.
6.  **Pindahkan Pasien ke Depan**:
    *   Jika `targetNode == q->head` (sudah di depan), mencetak pesan sukses dan mengembalikan `true`.
    *   **Sambungkan Ulang Node**:
        *   Jika `prevNode != NULL`, `prevNode->next = targetNode->next;` (lepaskan target dari posisi saat ini).
        *   Jika `targetNode->next != NULL`, `targetNode->next->prev = prevNode;`.
        *   Lainnya (target adalah tail), `q->tail = prevNode;`.
    *   **Atur Target sebagai Head Baru**:
        *   `targetNode->next = q->head;`
        *   `targetNode->prev = NULL;`
        *   Jika `q->head != NULL`, `q->head->prev = targetNode;`.
        *   `q->head = targetNode;`.
    *   Jika `q->tail == NULL` (antrian kosong sebelumnya, atau menjadi kosong lalu head ditambahkan kembali), `q->tail = targetNode;`.
7.  **Perbarui Posisi Pasien**:
    *   Memanggil `updatePatientPositionsInQueue(hospital, q)`.
8.  **Pesan Sukses**: Mencetak pesan yang mengonfirmasi pasien dipindahkan ke depan. Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika berhasil, `false` jika tidak.

## Fungsi Utama: `cancelPatientFromQueue`

### Tanda Tangan (Signature)
```c
boolean cancelPatientFromQueue(Hospital *hospital, Session *session, const char *patientUsernameToCancel)
```

### Ringkasan
Memungkinkan `MANAGER` yang login atau `PATIENT` tertentu itu sendiri untuk menghapus pasien tersebut dari antrian mereka saat ini.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data `Hospital`.
*   `Session *session`: Pointer ke `Session` pengguna saat ini untuk otorisasi.
*   `const char *patientUsernameToCancel`: Nama pengguna pasien yang entri antriannya akan dibatalkan.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah `hospital`, `session`, atau `patientUsernameToCancel` adalah `NULL`. Mencetak kesalahan dan mengembalikan `false` jika ada.
2.  **Pemeriksaan Otorisasi**:
    *   Memverifikasi `session->isLoggedIn`.
    *   Memeriksa apakah `session->role` adalah `MANAGER` atau `PATIENT`.
    *   Jika tidak diotorisasi, mencetak kesalahan dan mengembalikan `false`.
3.  **Cari Pasien untuk Dibatalkan**:
    *   Mencari `patientToCancelIdx` di `hospital->patients.elements` berdasarkan `patientUsernameToCancel`.
        *   Jika ditemukan dan `patient->queuePosition == 0` (di dalam ruangan), mencetak kesalahan dan mengembalikan `false`.
    *   Jika `patientToCancelIdx == -1`, mencetak kesalahan dan mengembalikan `false`.
    *   Mendapatkan `Patient *patientToCancel`.
4.  **Pemeriksaan Pembatalan Mandiri Pasien**:
    *   Jika `session->role == PATIENT` dan `session->username` tidak cocok dengan `patientUsernameToCancel`, mencetak kesalahan (pasien hanya dapat membatalkan antriannya sendiri) dan mengembalikan `false`.
5.  **Periksa Apakah Pasien dalam Antrian**:
    *   Jika `patientToCancel->queueRoom[0] == '\0'`, mencetak kesalahan (pasien tidak dalam antrian apa pun) dan mengembalikan `false`.
    *   Menyalin `patientToCancel->queueRoom` ke `originalRoomCode`.
6.  **Cari Struktur Antrian**:
    *   `Queue *q = findQueueByRoomCode(hospital, originalRoomCode);`.
    *   Jika `q == NULL`:
        *   Mencetak kesalahan (antrian untuk ruangan pasien tidak ditemukan).
        *   Mereset `patientToCancel->queueRoom` dan `queuePosition`.
        *   Mengembalikan `false`.
7.  **Hapus Pasien dari Struktur Antrian**:
    *   `boolean removed = false; int patientIdToCancel = patientToCancel->id;`.
    *   **Jika pasien ada di head**: Menggunakan `dequeue(q, &firstPatientId)`. Mengatur `removed = true` jika berhasil.
    *   **Lainnya (pasien ada di tengah/tail)**: Melintasi antrian untuk menemukan node dengan `patientIdToCancel`.
        *   Menyambungkan ulang `prev->next` dan `current->next->prev` untuk menghapus node `current`.
        *   Menangani pembaruan `q->tail` jika `current` adalah tail.
        *   `free(current); q->size--; removed = true;`.
    *   Jika `q->head == NULL`, mengatur `q->tail = NULL`.
8.  **Tangani Kegagalan Penghapusan/Kerusakan Data**:
    *   Jika `!removed`, mencetak kesalahan (pasien tidak ditemukan dalam struktur antrian meskipun terdaftar - data mungkin rusak).
9.  **Perbarui Catatan Pasien**:
    *   `patientToCancel->queueRoom[0] = '\0';`
    *   `patientToCancel->queuePosition = 0;`
10. **Perbarui Posisi Pasien yang Tersisa**:
    *   Jika `q != NULL`, memanggil `updatePatientPositionsInQueue(hospital, q)`.
11. **Pesan Sukses**: Mencetak pesan yang mengonfirmasi pembatalan atau pembersihan data. Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika berhasil atau jika inkonsistensi data diselesaikan dengan membersihkan status antrian pasien, `false` untuk kesalahan awal.
