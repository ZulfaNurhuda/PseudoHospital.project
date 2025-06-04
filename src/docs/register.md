# Berkas: `src/c/register.c`

Berkas ini menangani pendaftaran pasien baru ke dalam sistem rumah sakit.

## Fungsi: `registerPatient`

### Tanda Tangan (Signature)
```c
void registerPatient(Hospital *hospital, Session *session)
```

### Ringkasan
Mendaftarkan pasien baru ke dalam sistem. Fungsi ini meminta nama pengguna dan kata sandi, memeriksa keunikan nama pengguna, dan memastikan ada kapasitas untuk pengguna dan pasien baru. Jika pendaftaran berhasil, fungsi ini menginisialisasi data pasien dengan nilai default (termasuk tanda vital dan kapasitas daftar obat) dan kemudian mencoba untuk login pasien baru secara otomatis dengan memanggil fungsi `login`.

### Parameter
*   `Hospital *hospital`: Pointer ke struktur data utama `Hospital`. Ini akan diperbarui dengan informasi pengguna dan pasien baru.
*   `Session *session`: Pointer ke struktur `Session`. Digunakan untuk memeriksa apakah pengguna sudah login dan untuk diteruskan ke fungsi `login`.

### Algoritma
1.  **Pemeriksaan Sesi yang Ada**:
    *   Jika `session->isLoggedIn` adalah `true`, mencetak pesan ("Anda sudah login sebagai [session->username]! Anda tidak dapat mendaftar pasien.") dan kembali.
2.  **Input Kredensial Pengguna**:
    *   Mendeklarasikan `char username[50];` dan `char password[100];`.
    *   Meminta pengguna untuk "Username: " dan membaca input ke `username` menggunakan `scanf("%s", username)`.
    *   Meminta pengguna untuk "Password: " dan membaca input ke `password` menggunakan `scanf("%s", password)`.
    *   (Catatan: Menggunakan `scanf` untuk kata sandi tidak aman karena biasanya tidak disamarkan dan dapat berisiko buffer overflow jika input tidak divalidasi panjangnya sebelum `scanf`).
3.  **Pemeriksaan Keunikan Nama Pengguna**:
    *   Melakukan iterasi melalui `hospital->users.elements` (`i` dari `0` hingga `hospital->users.nEff - 1`).
    *   Jika `strcmp(hospital->users.elements[i].username, username) == 0` (nama pengguna sudah ada), mencetak kesalahan ("Registrasi gagal! Pasien dengan nama [username] sudah terdaftar.") dan kembali.
4.  **Pemeriksaan Kapasitas**:
    *   Jika `hospital->users.nEff >= hospital->users.capacity` ATAU `hospital->patients.nEff >= hospital->patients.capacity` (daftar pengguna atau pasien penuh), mencetak kesalahan ("Kapasitas pengguna atau pasien penuh!") dan kembali.
5.  **Buat Catatan Pengguna Baru**:
    *   `User *newUser = &hospital->users.elements[hospital->users.nEff];`
    *   `newUser->id = hospital->users.nEff + 1;` (Menetapkan ID baru, kemungkinan berurutan).
    *   `strcpy(newUser->username, username);`
    *   `strcpy(newUser->password, password);`
        *   (Catatan: Ini menyimpan kata sandi secara langsung sebagai teks biasa atau ke dalam struktur yang tidak didefinisikan dengan jelas di sini sebagai `Password`. Jika `User.password` adalah struct yang ditujukan untuk konten terenkripsi, penyalinan langsung teks biasa ini bermasalah untuk penyimpanan yang aman dan untuk panggilan `login` berikutnya jika mengharapkan kata sandi terenkripsi di sana. Bagian lain dari sistem menggunakan `enigmaEncrypt` untuk menyimpan kata sandi.)
    *   `newUser->role = PATIENT;`
    *   `hospital->users.nEff++;`
6.  **Buat Catatan Pasien Baru**:
    *   `Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];`
    *   `newPatient->id = newUser->id;` (Menghubungkan ID pasien ke ID pengguna).
    *   `strcpy(newPatient->username, username);`
    *   `strcpy(newPatient->disease, "");` (Tidak ada penyakit awal).
    *   `newPatient->life = 3;` (Poin nyawa default).
    *   `newPatient->bananaRich = 100.0f;` (Mata uang default).
    *   **Inisialisasi Tanda Vital (Nilai Default)**:
        *   `bodyTemperature = 36.5f;`
        *   `systolicBloodPressure = 120;`
        *   `diastolicBloodPressure = 80;`
        *   `heartRate = 75;`
        *   `oxygenSaturation = 98;`
        *   `bloodSugarLevel = 100.0f;`
        *   `weight = 70.0f;`
        *   `height = 170.0f;`
        *   `cholesterolLevel = 180.0f;`
        *   `ldlCholesterolLevel = 100.0f;` (Kolesterol LDL)
        *   `platelets = 250;`
    *   **Inisialisasi Daftar Obat**:
        *   `medicationsTaken`: kapasitas 10, mengalokasikan memori untuk array `medicationID`, `top = -1`.
        *   `medicationsPrescribed`: kapasitas 10, mengalokasikan memori untuk array `medicationID`, `nEff = 0`.
        *   (Catatan: `medicationID` kemungkinan seharusnya `medicationId` agar cocok dengan konvensi penamaan struct lain yang terlihat, misalnya di `canGoHome.c`.)
    *   `newPatient->queueRoom[0] = '\0';` (Awalnya tidak dalam antrian apa pun).
    *   `newPatient->queuePosition = -1;` (Tidak ada posisi antrian).
    *   `newPatient->diagnosedStatus = false;`
    *   `newPatient->treatedStatus = false;`
    *   `hospital->patients.nEff++;`
7.  **Cetak Pesan Sukses**:
    *   Mencetak "Pasien [username] berhasil ditambahkan!\n\n".
8.  **Upaya Login Otomatis**:
    *   Memanggil `login(hospital, session);`
        *   (Catatan: Ini mencoba untuk login pasien yang baru terdaftar menggunakan variabel `username` dan `password` yang baru saja dibaca melalui `scanf`. Keberhasilan login ini tergantung pada perilaku fungsi `login`, terutama bagaimana ia menangani kata sandi yang baru saja disimpan oleh `registerPatient` (yang disimpan sebagai teks biasa atau langsung ke field struct `Password`) versus kata sandi teks biasa yang diteruskan ke `login` untuk enkripsi dan perbandingan.)

### Nilai Kembali
*   `void`: Fungsi ini tidak mengembalikan nilai. Ia berinteraksi langsung dengan pengguna dan mengubah status rumah sakit.
