# Berkas: `src/c/enigma.c`

Berkas ini mengimplementasikan sistem enkripsi dan dekripsi mirip Enigma. Sistem ini menggunakan kombinasi rotor, reflektor, papan colok (plugboard), sandi Caesar, dan pergeseran dinamis untuk angka. Sistem dirancang untuk mengenkripsi dan mendekripsi string alfanumerik (A-Z, a-z, 0-9).

## Konfigurasi Statis Global
Konfigurasi ini mendefinisikan perilaku simulasi mesin Enigma:
*   `static const char *rotor = "BDFHJLCPRTXVZNYEIWGAKMUSQO";`: Pengkabelan rotor utama.
*   `static const char *reflector = "ZYXWVUTSRQPONMLKJIHGFEDCBA";`: Pengkabelan reflektor (alfabet terbalik sederhana).
*   `static const char plugboard[5][2] = {{'A', 'B'}, {'C', 'D'}, {'E', 'F'}, {'G', 'H'}, {'I', 'J'}};`: Mendefinisikan pertukaran karakter sebelum dan sesudah pemrosesan rotor. Hanya 5 pasangan pertama yang didefinisikan.
*   `static const int rotorPosition = 3;`: Ofset awal tetap untuk rotor.
*   `static const int caesarShift = 3;`: Nilai pergeseran tetap yang digunakan untuk sandi Caesar yang diterapkan pada string konfigurasi internal (tidak secara langsung ke plaintext/ciphertext selama langkah enkripsi utama).

## Fungsi Pembantu (Statis)

### `static int calculateDynamicShift(const char *input)`
*   **Ringkasan**: Menghitung nilai pergeseran dinamis (0-9) untuk enkripsi/dekripsi angka berdasarkan jumlah nilai ASCII karakter dalam string input.
*   **Parameter**: `const char *input` (plaintext untuk enkripsi, ciphertext untuk dekripsi).
*   **Mengembalikan**: Integer `sum % 10`.

### `static void caesarEncryptConfig(char *config, int shift)`
*   **Ringkasan**: Mengenkripsi string konfigurasi yang diberikan menggunakan sandi Caesar. Hanya menggeser karakter alfabet.
*   **Parameter**: `char *config` (string yang akan dienkripsi), `int shift` (nilai pergeseran Caesar).

### `static void caesarDecryptConfig(char *config, int shift)`
*   **Ringkasan**: Mendekripsi string konfigurasi yang diberikan menggunakan sandi Caesar. Hanya menggeser karakter alfabet. (Catatan: Fungsi ini ada tetapi tidak digunakan secara langsung oleh `enigmaDecrypt` untuk string konfigurasinya dalam kode yang diberikan; `enigmaDecrypt` menggunakan kembali `caesarEncryptConfig`.)
*   **Parameter**: `char *config` (string yang akan didekripsi), `int shift` (nilai pergeseran Caesar).

### `static char encryptNumber(char c, int shift)`
*   **Ringkasan**: Mengenkripsi satu karakter digit ('0'-'9') dengan menggesernya sejumlah tertentu, modulo 10.
*   **Parameter**: `char c` (karakter digit), `int shift` (nilai pergeseran).
*   **Mengembalikan**: Karakter digit yang dienkripsi.

### `static char decryptNumber(char c, int shift)`
*   **Ringkasan**: Mendekripsi satu karakter digit ('0'-'9') dengan pergeseran terbalik, modulo 10.
*   **Parameter**: `char c` (karakter digit), `int shift` (nilai pergeseran).
*   **Mengembalikan**: Karakter digit yang didekripsi.

### `static char processPlugboard(char c)`
*   **Ringkasan**: Memproses karakter melalui papan colok. Jika karakter adalah bagian dari pasangan yang ditentukan, karakter tersebut ditukar dengan pasangannya. Jika tidak, karakter dikembalikan tanpa perubahan.
*   **Parameter**: `char c` (karakter input).
*   **Mengembalikan**: Karakter setelah pemrosesan papan colok.

## Fungsi Utama

## Fungsi: `enigmaEncrypt`

### Tanda Tangan (Signature)
```c
boolean enigmaEncrypt(const char *plaintext, char *ciphertext, int maxLength)
```

### Ringkasan
Mengenkripsi string plaintext yang diberikan menggunakan simulasi Enigma. Menangani karakter alfanumerik (A-Z, a-z, 0-9). Karakter non-alfanumerik dianggap input tidak valid.

### Parameter
*   `const char *plaintext`: String yang akan dienkripsi.
*   `char *ciphertext`: Buffer untuk menyimpan string terenkripsi yang dihasilkan.
*   `int maxLength`: Kapasitas maksimum buffer `ciphertext` (termasuk terminator null).

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah `plaintext`, `ciphertext` adalah `NULL` atau `maxLength <= 0`. Jika ya, mencetak kesalahan ("Parameter enkripsi tidak valid!") dan mengembalikan `false`.
    *   Memeriksa apakah `strlen(plaintext) >= maxLength`. Jika ya, mencetak kesalahan ("Panjang input melebihi kapasitas output!") dan mengembalikan `false`.
    *   Memvalidasi karakter `plaintext`: harus 'A'-'Z', 'a'-'z', atau '0'-'9'. Jika ditemukan karakter tidak valid, mencetak kesalahan ("Input mengandung karakter tidak valid! ...") dan mengembalikan `false`.
2.  **Hitung Pergeseran Dinamis untuk Angka**:
    *   `int numberShift = calculateDynamicShift(plaintext);`
3.  **Siapkan dan Enkripsi String Konfigurasi (Penggunaan Internal)**:
    *   Membuat string `config` dengan menggabungkan `rotor`, "|", `rotorPosition` (sebagai string), "|", dan `reflector`.
    *   `caesarEncryptConfig(config, caesarShift);` (String config terenkripsi ini tidak digunakan secara langsung dalam logika enkripsi per karakter yang mengikuti, menunjukkan bahwa ini mungkin untuk logging, debugging, atau fitur yang tidak sepenuhnya diimplementasikan untuk pengaruh langsung pada jalur enkripsi per karakter).
4.  **Enkripsi Per Karakter**:
    *   Melakukan iterasi melalui setiap karakter `c` dari `plaintext`:
        *   **Enkripsi Angka**: Jika `c` adalah digit ('0'-'9'):
            *   `ciphertext[i] = encryptNumber(c, numberShift);`
            *   Lanjut ke karakter berikutnya.
        *   **Enkripsi Huruf**:
            *   Mengonversi `c` huruf kecil menjadi huruf besar.
            *   `c = processPlugboard(c);` (Lintasan papan colok pertama).
            *   **Rotor (Lintasan Maju)**: `int pos = (c - 'A' + rotorPosition) % 26; c = rotor[pos];`
            *   **Reflektor**: `c = reflector[c - 'A'];`
            *   **Rotor (Lintasan Mundur)**: Menemukan `j` sehingga `rotor[j] == c`. Kemudian `c = 'A' + ((j - rotorPosition + 26) % 26);`.
            *   `c = processPlugboard(c);` (Lintasan papan colok kedua).
            *   `ciphertext[i] = c;`
5.  **Terminasi Null pada Ciphertext**: `ciphertext[len] = '\0';`
6.  **Sukses**: Mencetak "Teks berhasil dienkripsi!" dan mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika enkripsi berhasil, `false` jika tidak.

---

## Fungsi: `enigmaDecrypt`

### Tanda Tangan (Signature)
```c
boolean enigmaDecrypt(const char *ciphertext, char *plaintext, int maxLength)
```

### Ringkasan
Mendekripsi string ciphertext yang diberikan yang dienkripsi oleh `enigmaEncrypt`. Menangani karakter alfanumerik.

### Parameter
*   `const char *ciphertext`: String yang akan didekripsi.
*   `char *plaintext`: Buffer untuk menyimpan string terdekripsi yang dihasilkan.
*   `int maxLength`: Kapasitas maksimum buffer `plaintext`.

### Algoritma
1.  **Validasi Input**:
    *   Memeriksa apakah `ciphertext`, `plaintext` adalah `NULL` atau `maxLength <= 0`. Jika ya, mencetak kesalahan ("Parameter dekripsi tidak valid!") dan mengembalikan `false`.
    *   Memeriksa apakah `strlen(ciphertext) >= maxLength`. Jika ya, mencetak kesalahan ("Panjang input melebihi kapasitas output!") dan mengembalikan `false`.
    *   Memvalidasi karakter `ciphertext`: harus 'A'-'Z', 'a'-'z', atau '0'-'9'. Jika ditemukan karakter tidak valid, mencetak kesalahan ("Input mengandung karakter tidak valid! ...") dan mengembalikan `false`.
2.  **Hitung Pergeseran Dinamis untuk Angka**:
    *   `int numberShift = calculateDynamicShift(ciphertext);` (Catatan: Menggunakan ciphertext untuk perhitungan pergeseran dinamis, yang simetris jika ciphertext hanya berisi angka atau jika properti set karakter dipertahankan melalui enkripsi untuk huruf).
3.  **Siapkan dan Enkripsi String Konfigurasi (Penggunaan Internal)**:
    *   Membuat string `config` mirip dengan `enigmaEncrypt` (menggabungkan `rotor`, "|", `rotorPosition`, "|", `reflector`).
    *   `caesarEncryptConfig(config, caesarShift);` (Sama seperti dalam enkripsi; string config itu sendiri tidak "didekripsi" menggunakan `caesarDecryptConfig`).
4.  **Dekripsi Per Karakter**:
    *   Melakukan iterasi melalui setiap karakter `c` dari `ciphertext`:
        *   **Dekripsi Angka**: Jika `c` adalah digit ('0'-'9'):
            *   `plaintext[i] = decryptNumber(c, numberShift);`
            *   Lanjut ke karakter berikutnya.
        *   **Dekripsi Huruf** (mengikuti jalur terbalik dari enkripsi):
            *   Mengonversi `c` huruf kecil menjadi huruf besar.
            *   `c = processPlugboard(c);` (Lintasan papan colok pertama - kebalikan dari lintasan kedua dalam enkripsi).
            *   **Rotor Terbalik (Lintasan Maju - menyimulasikan efek reflektor pada jalur terbalik)**:
                *   `int pos = (c - 'A' + rotorPosition) % 26;` (Posisi ini relatif terhadap keadaan rotor saat ini)
                *   Logika dekripsi aktual untuk lintasan mundur rotor di Enigma adalah menemukan input mana ke rotor yang akan menghasilkan `c` setelah melalui reflektor. Kode yang diberikan adalah:
                    ```c
                    // Bagian ini tampaknya merupakan upaya untuk membalikkan lintasan maju rotor sebelum reflektor
                    // tetapi ini bukan langkah dekripsi Enigma standar untuk tahap ini.
                    // Enigma Standar: Plugboard -> Rotor (terbalik) -> Reflektor (terbalik) -> Rotor (terbalik) -> Plugboard
                    // Kode di sini: Plugboard -> Cari input ke rotor yang menghasilkan (c - 'A' + rotorPosition) -> Reflektor -> Rotor (seperti maju) -> Plugboard
                    // Mari dokumentasikan apa yang dilakukan kode:
                    int target_char_after_first_rotor_pass_in_encryption = (c - 'A' + rotorPosition) % 26; // Ini adalah 'pos'
                    // Cari karakter `j` mana di rotor yang memetakan ke `target_char_after_first_rotor_pass_in_encryption` ini
                    // Bukan, ini adalah `rotor[j] == 'A' + pos`. Ini berarti `rotor[j]` adalah karakter yang dipetakan oleh `pos`.
                    // Ini mencari `j` sehingga `rotor[j]` adalah karakter `('A' + pos)`.
                    // `c` pada titik ini adalah setelah papan colok pertama.
                    // `pos` adalah `(c - 'A' + rotorPosition) % 26`.
                    // Ini mencari `j` di mana `rotor[j] == 'A' + pos`. Ini tampaknya kesalahan atau kesalahpahaman logika Enigma.
                    // Mari kita asumsikan `c` adalah karakter setelah papan colok. Langkah selanjutnya dalam dekripsi seharusnya adalah kebalikan dari lintasan rotor terakhir enkripsi.
                    // Lintasan rotor terakhir enkripsi adalah: temukan `j` sehingga `rotor[j] == c_after_reflector`, kemudian `c = 'A' + ((j - rotorPosition + 26) % 26)`.
                    // Jadi, kebalikannya adalah: `rotor_input_char_code = (c_from_plugboard - 'A' + rotorPosition) % 26`. `c = rotor[rotor_input_char_code]`. Ini adalah lintasan maju rotor.
                    // Bagian kode ini membingungkan dan mungkin tidak mengimplementasikan dekripsi Enigma dengan benar.
                    // Mendokumentasikan seperti yang tertulis:
                    for (int j = 0; j < 26; j++) {
                        if (rotor[j] == 'A' + pos) { // 'A' + pos adalah karakter yang akan dipetakan oleh `c` (setelah papan colok) jika melewati rotor maju dengan posisi saat ini
                            c = 'A' + j; // `c` menjadi *indeks* karakter tersebut dalam alfabet
                            break;
                        }
                    }
                    ```
            *   **Reflektor (Terbalik)**: `c = reflector[c - 'A'];` (Reflektor bersifat resiprokal sendiri, jadi ini sama dengan enkripsi).
            *   **Rotor Terbalik (Lintasan Mundur - menyimulasikan efek rotor pada jalur terbalik)**:
                *   `pos = (c - 'A' - rotorPosition + 26) % 26;`
                *   `c = rotor[pos];` (Ini adalah lintasan maju rotor dengan posisi yang dimodifikasi, bukan pemetaan terbalik sejati berdasarkan pengkabelan rotor).
            *   `c = processPlugboard(c);` (Lintasan papan colok kedua - kebalikan dari lintasan pertama dalam enkripsi).
            *   `plaintext[i] = c;`
5.  **Terminasi Null pada Plaintext**: `plaintext[len] = '\0';`
6.  **Sukses**: Mencetak "Teks berhasil didekripsi!" dan mengembalikan `true`.

### Nilai Kembali
*   `boolean`: `true` jika dekripsi berhasil, `false` jika tidak.
*   **Catatan tentang Kebenaran Dekripsi**: Logika dekripsi untuk huruf, khususnya lintasan rotor, mungkin tidak membalikkan proses enkripsi dengan sempurna karena detail implementasinya. Dekripsi Enigma standar memerlukan lintasan rotor terbalik yang sejati. Algoritma yang didokumentasikan mencerminkan kode seperti yang tertulis.
