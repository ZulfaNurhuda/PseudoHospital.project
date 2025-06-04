# Berkas: `src/c/queuePrimitive.c`

Berkas ini menyediakan operasi primitif untuk struktur data antrian (queue), yang diimplementasikan sebagai senarai berantai ganda (doubly linked list). Antrian ini digunakan untuk mengelola pasien yang menunggu ruangan/dokter. Setiap node dalam antrian menyimpan `patientId`.

## Fungsi: `initializeQueue`

### Tanda Tangan (Signature)
```c
void initializeQueue(Queue *q, const char *roomCode)
```

### Ringkasan
Menginisialisasi struktur `Queue`. Mengatur pointer head dan tail ke `NULL`, ukuran ke `0`, dan mengaitkan antrian dengan `roomCode` yang diberikan.

### Parameter
*   `Queue *q`: Pointer ke struktur `Queue` yang akan diinisialisasi.
*   `const char *roomCode`: String yang merepresentasikan kode ruangan tempat antrian ini terkait. Jika `NULL`, kode ruangan antrian diatur menjadi string kosong.

### Algoritma
1.  **Pemeriksaan Null**: Jika `q` adalah `NULL`, fungsi segera kembali.
2.  **Inisialisasi Pointer dan Ukuran**:
    *   `q->head = NULL;`
    *   `q->tail = NULL;`
    *   `q->size = 0;`
3.  **Atur Kode Ruangan**:
    *   Jika `roomCode` bukan `NULL`, salin `roomCode` ke `q->roomCode` menggunakan `strcpy`.
    *   Lainnya (jika `roomCode` adalah `NULL`), atur `q->roomCode[0] = '\0';` (string kosong).

### Nilai Kembali
*   `void`.

## Fungsi: `isQueueEmpty`

### Tanda Tangan (Signature)
```c
boolean isQueueEmpty(const Queue *q)
```

### Ringkasan
Memeriksa apakah antrian yang diberikan kosong.

### Parameter
*   `const Queue *q`: Pointer ke `Queue` untuk diperiksa.

### Algoritma
1.  **Pemeriksaan Null**: Jika `q` adalah `NULL`, mengembalikan `true` (dianggap kosong atau tidak valid).
2.  **Periksa Ukuran**: Mengembalikan `true` jika `q->size == 0`, `false` jika tidak.

### Nilai Kembali
*   `boolean`: `true` jika antrian kosong atau `NULL`, `false` jika tidak.

## Fungsi: `queueSize`

### Tanda Tangan (Signature)
```c
int queueSize(const Queue *q)
```

### Ringkasan
Mengembalikan jumlah elemen yang saat ini ada dalam antrian.

### Parameter
*   `const Queue *q`: Pointer ke `Queue`.

### Algoritma
1.  **Pemeriksaan Null**: Jika `q` adalah `NULL`, mengembalikan `0`.
2.  **Kembalikan Ukuran**: Mengembalikan `q->size`.

### Nilai Kembali
*   `int`: Jumlah elemen dalam antrian, atau `0` jika antrian adalah `NULL`.

## Fungsi: `enqueue`

### Tanda Tangan (Signature)
```c
boolean enqueue(Queue *q, int patientId)
```

### Ringkasan
Menambahkan elemen baru (berisi `patientId`) ke tail (akhir) antrian.

### Parameter
*   `Queue *q`: Pointer ke `Queue` tempat elemen akan ditambahkan.
*   `int patientId`: ID pasien yang akan ditambahkan ke antrian.

### Algoritma
1.  **Pemeriksaan Null (Antrian)**: Jika `q` adalah `NULL`, mengembalikan `false`.
2.  **Alokasi Node Baru**:
    *   `QueueNode *newNode = (QueueNode *)safeMalloc(sizeof(QueueNode));`
    *   Jika `newNode` adalah `NULL` (alokasi gagal), mencetak kesalahan ("Gagal alokasi memori untuk node antrian baru!") dan mengembalikan `false`.
3.  **Inisialisasi Node Baru**:
    *   `newNode->info.patientId = patientId;`
    *   `newNode->next = NULL;` (karena akan menjadi tail baru).
    *   `newNode->prev = q->tail;` (tautkan ke tail saat ini).
4.  **Tambahkan Node ke Antrian**:
    *   Jika `isQueueEmpty(q)`:
        *   `q->head = newNode;`
        *   `q->tail = newNode;`
    *   Lainnya (antrian tidak kosong):
        *   `q->tail->next = newNode;` (tautkan tail sebelumnya ke node baru).
        *   `q->tail = newNode;` (perbarui pointer tail).
5.  **Tambah Ukuran**: `q->size++;`
6.  **Kembalikan Sukses**: Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika elemen berhasil ditambahkan, `false` jika tidak (misalnya, antrian `NULL` atau alokasi memori gagal).

## Fungsi: `dequeue`

### Tanda Tangan (Signature)
```c
boolean dequeue(Queue *q, int *patientId)
```

### Ringkasan
Menghapus elemen dari head (depan) antrian dan mengembalikan `patientId` dari elemen yang dihapus.

### Parameter
*   `Queue *q`: Pointer ke `Queue` tempat elemen akan dihapus.
*   `int *patientId`: Pointer ke integer tempat `patientId` dari elemen yang di-dequeue akan disimpan. Bisa `NULL` jika pemanggil tidak memerlukan ID.

### Algoritma
1.  **Pemeriksaan Null atau Kosong**:
    *   Jika `q` adalah `NULL` atau `isQueueEmpty(q)` adalah true:
        *   Jika `patientId` bukan `NULL`, atur `*patientId = -1;` (atau indikator kegagalan/kosong lainnya).
        *   Mengembalikan `false`.
2.  **Simpan Node Head**: `QueueNode *temp = q->head;`
3.  **Kembalikan ID Pasien (Opsional)**:
    *   Jika `patientId` bukan `NULL`, atur `*patientId = temp->info.patientId;`.
4.  **Perbarui Pointer Head**:
    *   `q->head = q->head->next;`
5.  **Perbarui Tail jika Antrian Menjadi Kosong**:
    *   Jika `q->head == NULL` (antrian sekarang kosong):
        *   `q->tail = NULL;`
    *   Lainnya (antrian masih memiliki elemen):
        *   `q->head->prev = NULL;` (head baru tidak memiliki node sebelumnya).
6.  **Bebaskan Node Head Lama**: `free(temp);`
7.  **Kurangi Ukuran**: `q->size--;`
8.  **Kembalikan Sukses**: Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika elemen berhasil dihapus, `false` jika tidak (misalnya, antrian kosong atau `NULL`).

## Fungsi: `peekQueue`

### Tanda Tangan (Signature)
```c
boolean peekQueue(const Queue *q, int *patientId)
```

### Ringkasan
Mengambil `patientId` dari head (depan) antrian tanpa menghapus elemen tersebut.

### Parameter
*   `const Queue *q`: Pointer ke `Queue`.
*   `int *patientId`: Pointer ke integer tempat `patientId` dari elemen head akan disimpan. Bisa `NULL`.

### Algoritma
1.  **Pemeriksaan Null atau Kosong**:
    *   Jika `q` adalah `NULL` atau `isQueueEmpty(q)` adalah true:
        *   Jika `patientId` bukan `NULL`, atur `*patientId = -1;`.
        *   Mengembalikan `false`.
2.  **Kembalikan ID Pasien (Opsional)**:
    *   Jika `patientId` bukan `NULL`, atur `*patientId = q->head->info.patientId;`.
3.  **Kembalikan Sukses**: Mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika ada elemen di head antrian, `false` jika tidak.
