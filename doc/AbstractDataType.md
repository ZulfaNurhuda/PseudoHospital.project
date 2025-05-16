# Rekomendasi ADT untuk Sistem Manajemen Rumah Sakit

## 1. Pengelolaan Pengguna

### 1.1 ADT User
```c
/* ADT Sederhana - untuk representasi data pengguna dasar */
typedef struct {
    char id[10];
    char username[50];
    char password[100];   // Enkripsi Enigma
    char role[10];        // "MANAJER", "DOKTER", atau "PASIEN"
} User;

/* ADT List - untuk penyimpanan kumpulan user */
typedef struct {
    User* elements;
    int capacity;
    int nEff;
} UserList;
```

### 1.2 ADT Pasien
```c
/* ADT Sederhana - untuk representasi data pasien */
typedef struct {
    char id[10];
    char username[50];
    char penyakit[50];
    int nyawa;           // Untuk fitur B04 - Dead or Alive (mulai dengan 3)
    float bananaRich;    // Untuk fitur B03 - BananaRich
    
    // Data kesehatan
    float suhuTubuh;
    int tekananDarahSistolik;
    int tekananDarahDiastolik;
    int detakJantung;
    int saturasiOksigen;
    float kadarGulaDarah;
    float beratBadan;
    float tinggiBadan;
    float kadarKolesterol;
    float kadarKolesterolLDL;
    int trombosit;
    
    // Untuk tracking obat yang diminum
    int* obatDiminum;      // List ID obat yang sudah diminum
    int jumlahObatDiminum;
} Pasien;

/* ADT Linked List - untuk daftar pasien yang dinamis */
typedef struct nodePasien {
    Pasien info;
    struct nodePasien* next;
} NodePasien;

typedef struct {
    NodePasien* first;
} ListPasien;
```

### 1.3 ADT Dokter
```c
/* ADT Sederhana - untuk representasi data dokter */
typedef struct {
    char id[10];
    char username[50];
    char spesialis[50];
    float aura;
    float bananaRich;    // Untuk fitur B03 - BananaRich
    char ruangan[5];     // Contoh: "A1", "B2", dll.
} Dokter;

/* ADT Linked List - untuk daftar dokter yang dinamis */
typedef struct nodeDokter {
    Dokter info;
    struct nodeDokter* next;
} NodeDokter;

typedef struct {
    NodeDokter* first;
} ListDokter;
```

## 2. Pengelolaan Ruangan dan Denah

### 2.1 ADT Ruangan
```c
/* ADT Sederhana - untuk representasi data ruangan */
typedef struct {
    char kode[5];        // Contoh: "A1", "B2", dll.
    int kapasitas;
    char dokterID[10];   // ID dokter yang menempati ruangan ini
    int jumlahPasienDalamRuangan;
} Ruangan;
```

### 2.2 ADT Matrix untuk Denah
```c
/* ADT Matrix - untuk representasi denah rumah sakit 2D */
typedef struct {
    Ruangan** elements;  // Matrix dari pointer ke Ruangan
    int rowEff;          // Jumlah baris efektif
    int colEff;          // Jumlah kolom efektif
} Denah;
```

## 3. Pengelolaan Penyakit dan Diagnosis

### 3.1 ADT Penyakit
```c
/* ADT Sederhana - untuk representasi data penyakit */
typedef struct {
    int id;
    char nama[50];
    float suhuTubuhMin;
    float suhuTubuhMax;
    int tekananDarahSistolikMin;
    int tekananDarahSistolikMax;
    int tekananDarahDiastolikMin;
    int tekananDarahDiastolikMax;
    int detakJantungMin;
    int detakJantungMax;
    float kadarGulaDarahMin;
    float kadarGulaDarahMax;
    float beratBadanMin;
    float beratBadanMax;
    float tinggiBadanMin;
    float tinggiBadanMax;
    float kadarKolesterolMin;
    float kadarKolesterolMax;
    int trombositMin;
    int trombositMax;
} Penyakit;

/* ADT List - untuk penyimpanan daftar penyakit */
typedef struct {
    Penyakit* elements;
    int capacity;
    int nEff;
} ListPenyakit;
```

## 4. Pengelolaan Obat dan Resep

### 4.1 ADT Obat
```c
/* ADT Sederhana - untuk representasi data obat */
typedef struct {
    int id;
    char nama[50];
} Obat;

/* ADT List - untuk penyimpanan daftar obat */
typedef struct {
    Obat* elements;
    int capacity;
    int nEff;
} ListObat;
```

### 4.2 ADT Resep
```c
/* ADT Sederhana - untuk representasi data resep obat */
typedef struct {
    int obatID;
    int penyakitID;
    int urutanMinum;
} ResepObat;

/* ADT List - untuk penyimpanan daftar resep */
typedef struct {
    ResepObat* elements;
    int capacity;
    int nEff;
} ListResep;
```

## 5. Pengelolaan Antrian Pasien

### 5.1 ADT Queue untuk Antrian per Ruangan
```c
/* ADT Sederhana - untuk informasi pasien dalam antrian */
typedef struct {
    char pasienID[10];
} AntrianInfo;

/* ADT Queue - untuk implementasi antrian pasien */
typedef struct {
    AntrianInfo* buffer;
    int idxHead;
    int idxTail;
    int capacity;
} Queue;

/* ADT List - untuk kumpulan antrian di rumah sakit */
typedef struct {
    Queue* queues;        // Array of Queues
    int nRuangan;         // Jumlah ruangan yang memiliki antrian
} AntrianRumahSakit;
```

## 6. Pengelolaan BananaRich (B03)

### 6.1 ADT untuk Manajemen Finansial
```c
/* ADT Sederhana - untuk pengelolaan data keuangan rumah sakit */
typedef struct {
    float totalPendapatan;
    float totalPengeluaran;
    float saldoRumahSakit;
} FinansialRS;
```

## 7. Integrasi Data (Struktur Utama)

### 7.1 ADT RumahSakit (Master)
```c
/* ADT Sederhana - untuk integrasi seluruh komponen dalam satu struktur */
typedef struct {
    // Pengguna
    UserList users;
    ListPasien pasien;
    ListDokter dokter;
    
    // Ruangan
    Denah denah;
    
    // Antrian
    AntrianRumahSakit antrian;
    
    // Penyakit dan obat
    ListPenyakit penyakit;
    ListObat obat;
    ListResep resep;
    
    // Finansial
    FinansialRS finansial;
} RumahSakit;
```

## 8. ADT EnigmaCipher (E01)

```c
/* ADT Sederhana - untuk konfigurasi mesin Enigma */
typedef struct {
    char* rotorTypes;     // Array of rotor types
    int* rotorPositions;  // Array of rotor positions
    char* reflectionType;
    char** plugboardPairs;
} EnigmaConfig;

/* ADT Sederhana - untuk penyimpanan password terenkripsi */
typedef struct {
    char* encryptedContent;
    EnigmaConfig config;
} PasswordData;
```

## 9. Pengelolaan Sesi Login

### 9.1 ADT Session
```c
/* ADT Sederhana - untuk mengelola sesi login pengguna */

typedef struct {
    char userID[10];        // ID pengguna yang sedang login
    char username[50];      // Username pengguna
    char role[10];          // Role: "MANAJER", "DOKTER", atau "PASIEN"
    int isLoggedIn;         // Status login: 1 (login) atau 0 (logout)
    time_t loginTime;       // Waktu login
    time_t lastActivity;    // Waktu aktivitas terakhir
} Session;
```

## Penggunaan ADT sesuai Fitur-Fitur Utama

### F01-F04: Login, Register, Logout, Lupa Password
- Menggunakan `ADT User` dan `PasswordData` untuk enkripsi

### F05: Menu & Help
- Tidak memerlukan ADT khusus

### F06: Denah Rumah Sakit
- Menggunakan `ADT Matrix` (Denah)
- Menggunakan `ADT Ruangan`

### F07-F08: Lihat User & Cari User
- Menggunakan `ADT User`, `ADT Pasien`, dan `ADT Dokter`

### F09: Lihat Antrian
- Menggunakan `ADT Queue` (AntrianRumahSakit)
- Menggunakan `ADT Matrix` (Denah)

### F10: Tambah Dokter
- Menggunakan `ADT Dokter`

### F11: Diagnosis
- Menggunakan `ADT Penyakit`
- Menggunakan `ADT Pasien`

### F12: Ngobatin
- Menggunakan `ADT Resep`
- Menggunakan `ADT Obat`

### F13-F17: Pulang Dok, Daftar Checkup, Antrian Saya, Minum Obat, Minum Penawar
- Menggunakan kombinasi ADT yang sudah ada

### B02: Denah Dinamis
- Menggunakan `ADT Matrix` (Denah) dengan fungsi perubahan ukuran

### B03: BananaRich
- Menggunakan `ADT Pasien`, `ADT Dokter`, dan `FinansialRS`

### B04: Dead or Alive
- Menggunakan `ADT Pasien` dengan atribut nyawa

### B05: Mainin Antrian
- Menggunakan `ADT Queue` (AntrianRumahSakit) dengan operasi tambahan

### E01: Encrypt & Decrypt Password
- Menggunakan `ADT EnigmaCipher`