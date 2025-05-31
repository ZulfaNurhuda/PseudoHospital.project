#include "enigma.h"

// --- Konfigurasi Statis Mesin Enigma (Simulasi Sederhana) ---
// Variabel-variabel statis global ini mendefinisikan komponen-komponen tetap dari mesin Enigma yang disimulasikan.
// 'static' berarti variabel ini hanya memiliki lingkup file (file scope).

// `rotor`: Mendefinisikan pemetaan substitusi karakter oleh rotor. Ini adalah string 26 huruf unik.
// Contoh: jika input 'A', akan dipetakan ke rotor[0] ('B'), 'B' ke rotor[1] ('D'), dst., setelah penyesuaian posisi.
static const char *rotor = "BDFHJLCPRTXVZNYEIWGAKMUSQO";

// `reflector`: Mendefinisikan pemetaan oleh reflektor. Reflektor memetakan satu huruf ke huruf lain secara simetris.
// Contoh: 'A' dipetakan ke reflector[0] ('Z'), 'B' ke reflector[1] ('Y'). Jika 'A' -> 'Z', maka 'Z' -> 'A'.
static const char *reflector = "ZYXWVUTSRQPONMLKJIHGFEDCBA";

// `plugboard`: Mendefinisikan pasangan huruf yang ditukar sebelum dan sesudah proses rotor/reflektor.
// Ini adalah array 2D yang menyimpan 5 pasangan huruf.
// Contoh: {'A', 'B'} berarti 'A' akan menjadi 'B' dan 'B' akan menjadi 'A' saat melewati plugboard.
static const char plugboard[5][2] = {{'A', 'B'}, {'C', 'D'}, {'E', 'F'}, {'G', 'H'}, {'I', 'J'}};

// `rotorPosition`: Posisi awal atau offset dari rotor. Ini mempengaruhi bagaimana karakter dipetakan melalui rotor.
// Nilai 3 berarti rotor "bergeser" 3 posisi.
static const int rotorPosition = 3;

// `caesarShift`: Nilai pergeseran yang digunakan untuk mengenkripsi string konfigurasi Enigma itu sendiri (rotor, reflector, dll.)
// menggunakan Caesar cipher. Ini adalah lapisan enkripsi tambahan pada metadata konfigurasi, bukan pada data utama.
static const int caesarShift = 3;

// Fungsi statis untuk menghitung nilai pergeseran dinamis berdasarkan input string.
// Parameter input: string yang akan digunakan untuk menghitung shift.
// Mengembalikan nilai integer antara 0-9 yang merupakan hasil modulo 10 dari jumlah nilai ASCII karakter input.
// Shift ini digunakan khusus untuk enkripsi/dekripsi angka.
static int calculateDynamicShift(const char *input)
{
    int sum = 0; // Variabel untuk mengakumulasi jumlah nilai ASCII.
    // Melakukan iterasi melalui setiap karakter dalam string input.
    for (int i = 0; input[i] != '\0'; i++)
    {
        sum += (int)input[i]; // Menambahkan nilai ASCII karakter ke sum.
    }
    // Mengembalikan sisa bagi dari sum dengan 10, sehingga hasilnya selalu antara 0 dan 9.
    return sum % 10;
}

// Fungsi statis untuk mengenkripsi sebuah string konfigurasi menggunakan Caesar Cipher.
// Parameter config: pointer ke string konfigurasi yang akan dienkripsi (dimodifikasi langsung).
// Parameter shift: nilai pergeseran untuk Caesar cipher.
// Fungsi ini hanya mengenkripsi huruf (A-Z, a-z).
static void caesarEncryptConfig(char *config, int shift)
{
    int len = strlen(config); // Mendapatkan panjang string konfigurasi.
    // Melakukan iterasi melalui setiap karakter dalam string.
    for (int i = 0; i < len; i++)
    {
        // Memeriksa apakah karakter saat ini adalah huruf (besar atau kecil).
        if (('A' <= config[i] && config[i] <= 'Z') || ('a' <= config[i] && config[i] <= 'z'))
        {
            // Menentukan basis ASCII ('A' untuk huruf besar, 'a' untuk huruf kecil).
            char base = ('A' <= config[i] && config[i] <= 'Z') ? 'A' : 'a';
            // Melakukan enkripsi Caesar: (karakter - basis + shift) mod 26 + basis.
            config[i] = base + ((config[i] - base + shift) % 26);
        }
    }
}

// Fungsi statis untuk mendekripsi sebuah string konfigurasi yang telah dienkripsi dengan Caesar Cipher.
// Parameter config: pointer ke string konfigurasi terenkripsi yang akan didekripsi (dimodifikasi langsung).
// Parameter shift: nilai pergeseran yang sama yang digunakan saat enkripsi.
// Fungsi ini hanya mendekripsi huruf.
static void caesarDecryptConfig(char *config, int shift)
{
    int len = strlen(config); // Mendapatkan panjang string konfigurasi.
    // Melakukan iterasi melalui setiap karakter.
    for (int i = 0; i < len; i++)
    {
        // Memeriksa apakah karakter adalah huruf.
        if (('A' <= config[i] && config[i] <= 'Z') || ('a' <= config[i] && config[i] <= 'z'))
        {
            char base = ('A' <= config[i] && config[i] <= 'Z') ? 'A' : 'a';
            // Melakukan dekripsi Caesar: (karakter - basis - shift + 26) mod 26 + basis.
            // Penambahan 26 sebelum modulo adalah untuk menangani hasil negatif.
            config[i] = base + ((config[i] - base - shift + 26) % 26);
        }
    }
}

// Fungsi statis untuk mengenkripsi sebuah karakter angka ('0'-'9') menggunakan Caesar-like cipher.
// Parameter c: karakter angka yang akan dienkripsi.
// Parameter shift: nilai pergeseran dinamis (dari calculateDynamicShift).
// Mengembalikan karakter angka terenkripsi, atau karakter asli jika bukan angka.
static char encryptNumber(char c, int shift)
{
    // Memeriksa apakah karakter adalah digit angka.
    if ('0' <= c && c <= '9')
    {
        // Melakukan enkripsi: (angka - '0' + shift) mod 10 + '0'.
        return '0' + ((c - '0' + shift) % 10);
    }
    // Jika bukan angka, kembalikan karakter asli.
    return c;
}

// Fungsi statis untuk mendekripsi sebuah karakter angka ('0'-'9') yang dienkripsi dengan Caesar-like cipher.
// Parameter c: karakter angka terenkripsi.
// Parameter shift: nilai pergeseran dinamis yang sama yang digunakan saat enkripsi.
// Mengembalikan karakter angka terdekripsi, atau karakter asli jika bukan angka.
static char decryptNumber(char c, int shift)
{
    // Memeriksa apakah karakter adalah digit angka.
    if ('0' <= c && c <= '9')
    {
        // Melakukan dekripsi: (angka - '0' - shift + 10) mod 10 + '0'.
        // Penambahan 10 sebelum modulo untuk menangani hasil negatif.
        return '0' + ((c - '0' - shift + 10) % 10);
    }
    // Jika bukan angka, kembalikan karakter asli.
    return c;
}

// Fungsi statis untuk memproses sebuah karakter melalui plugboard.
// Jika karakter adalah bagian dari pasangan di plugboard, karakter tersebut ditukar dengan pasangannya.
// Parameter c: karakter input (diasumsikan sudah huruf besar).
// Mengembalikan karakter hasil pemrosesan plugboard.
static char processPlugboard(char c)
{
    // Melakukan iterasi melalui 5 pasangan yang didefinisikan di `plugboard`.
    for (int i = 0; i < 5; i++) // Angka 5 merujuk pada jumlah pasangan di `plugboard`.
    {
        // Jika karakter input cocok dengan elemen pertama pasangan.
        if (c == plugboard[i][0])
            return plugboard[i][1]; // Kembalikan elemen kedua.
        // Jika karakter input cocok dengan elemen kedua pasangan.
        if (c == plugboard[i][1])
            return plugboard[i][0]; // Kembalikan elemen pertama.
    }
    // Jika karakter tidak ada dalam pasangan plugboard, kembalikan karakter asli.
    return c;
}

// Fungsi utama untuk mengenkripsi plaintext menggunakan simulasi Enigma.
// Parameter plaintext: string input yang akan dienkripsi.
// Parameter ciphertext: buffer untuk menyimpan hasil enkripsi.
// Parameter maxLength: ukuran maksimum buffer ciphertext (termasuk null terminator).
// Mengembalikan true jika enkripsi berhasil, false jika gagal.
boolean enigmaEncrypt(const char *plaintext, char *ciphertext, int maxLength)
{
    // Validasi parameter input.
    if (!plaintext || !ciphertext || maxLength <= 0)
    {
        printError("Parameter enkripsi tidak valid!"); // Pesan error jika parameter tidak valid.
        return false;                                  // Enkripsi gagal.
    }

    int len = strlen(plaintext); // Mendapatkan panjang plaintext.
    // Memeriksa apakah panjang plaintext melebihi kapasitas buffer ciphertext.
    if (len >= maxLength) // Harus `len >= maxLength` karena perlu ruang untuk null terminator.
    {
        printError("Panjang input melebihi kapasitas output!"); // Pesan error.
        return false;                                           // Enkripsi gagal.
    }

    // Validasi karakter dalam plaintext: hanya memperbolehkan huruf A-Z (case-insensitive) dan angka 0-9.
    for (int i = 0; i < len; i++)
    {
        char c = plaintext[i];
        // Jika karakter bukan huruf besar, huruf kecil, atau angka.
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')))
        {
            printError("Input mengandung karakter tidak valid! Hanya huruf A-Z dan angka 0-9 diperbolehkan.");
            return false; // Enkripsi gagal.
        }
    }

    // Menghitung nilai pergeseran dinamis untuk enkripsi angka, berdasarkan seluruh plaintext.
    int numberShift = calculateDynamicShift(plaintext);

    // Bagian ini membangun string 'config' yang berisi representasi dari konfigurasi Enigma (rotor, posisi, reflektor)
    // dan kemudian mengenkripsinya menggunakan Caesar cipher.
    // Namun, string 'config' terenkripsi ini TIDAK digunakan secara langsung dalam proses enkripsi karakter per karakter di bawah.
    // Ini mungkin untuk tujuan logging, fitur lain, atau hanya demonstrasi enkripsi konfigurasi.
    char config[100] = "";    // Buffer untuk string konfigurasi.
    strcat(config, rotor);    // Menambahkan string rotor.
    strcat(config, "|");      // Pemisah.
    char posStr[10] = "";     // Buffer untuk posisi rotor sebagai string.
    int temp = rotorPosition; // Mengambil posisi rotor statis.
    int index = 0;
    // Mengkonversi rotorPosition (integer) menjadi string (dalam urutan terbalik).
    // Jika rotorPosition adalah 0, loop ini tidak akan berjalan, posStr akan kosong.
    // Jika rotorPosition > 0:
    if (temp == 0)
    { // Menangani kasus jika rotorPosition adalah 0.
        posStr[index++] = '0';
    }
    else
    {
        while (temp > 0)
        {
            posStr[index++] = (temp % 10) + '0'; // Ambil digit terakhir, konversi ke char, simpan.
            temp /= 10;                          // Hapus digit terakhir.
        }
    }
    posStr[index] = '\0'; // Tambahkan null terminator untuk posStr yang digitnya terbalik atau "0".
    // Membalik string posStr agar urutan digitnya benar.
    for (int i = 0; i < index / 2; i++)
    {
        char swap = posStr[i];
        posStr[i] = posStr[index - 1 - i];
        posStr[index - 1 - i] = swap;
    }
    // Jika rotorPosition awalnya 0 dan posStr menjadi "0", ini akan tetap "0".
    // Jika rotorPosition misal 3, posStr akan menjadi "3".
    strcat(config, posStr);    // Menambahkan string posisi rotor.
    strcat(config, "|");       // Pemisah.
    strcat(config, reflector); // Menambahkan string reflektor.
    // Mengenkripsi string 'config' yang baru dibuat menggunakan Caesar cipher dengan `caesarShift`.
    caesarEncryptConfig(config, caesarShift);
    // Catatan: `config` terenkripsi ini tidak digunakan lebih lanjut dalam fungsi `enigmaEncrypt` ini.

    // Proses enkripsi karakter per karakter dari plaintext.
    for (int i = 0; i < len; i++)
    {
        char c = plaintext[i]; // Mengambil karakter plaintext saat ini.

        // Jika karakter adalah angka ('0'-'9').
        if ('0' <= c && c <= '9')
        {
            // Enkripsi angka menggunakan fungsi encryptNumber dengan numberShift dinamis.
            ciphertext[i] = encryptNumber(c, numberShift);
            continue; // Lanjutkan ke karakter berikutnya dalam plaintext.
        }

        // Jika karakter adalah huruf, normalisasi ke huruf besar.
        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A'; // Konversi huruf kecil ke huruf besar.

        // Tahap 1: Proses melalui Plugboard (pertama kali).
        c = processPlugboard(c);

        // Tahap 2: Proses melalui Rotor (jalur maju/forward).
        // Posisi efektif pada rotor dihitung dengan menambahkan rotorPosition (offset) dan melakukan modulo 26.
        int pos = (c - 'A' + rotorPosition) % 26; // c - 'A' menghasilkan indeks 0-25.
        c = rotor[pos];                           // Substitusi karakter berdasarkan pemetaan rotor pada posisi efektif.

        // Tahap 3: Proses melalui Reflektor.
        // Reflektor memetakan karakter ke karakter lain berdasarkan posisinya.
        c = reflector[c - 'A']; // c - 'A' menghasilkan indeks untuk array reflektor.

        // Tahap 4: Proses melalui Rotor (jalur balik/backward).
        // Ini adalah invers dari pemetaan rotor maju. Kita mencari karakter input asli
        // yang akan menghasilkan karakter 'c' saat ini setelah melalui rotor maju.
        // Loop mencari indeks 'j' pada `rotor` yang nilainya sama dengan `c`.
        for (int j = 0; j < 26; j++)
        {
            if (rotor[j] == c) // Jika rotor[j] adalah karakter 'c' saat ini.
            {
                // Karakter sebelum rotor adalah 'A' + ((indeks j - rotorPosition + 26) % 26).
                // Penambahan 26 untuk menangani hasil negatif dari (j - rotorPosition).
                c = 'A' + ((j - rotorPosition + 26) % 26);
                break; // Keluar dari loop setelah pemetaan balik ditemukan.
            }
        }

        // Tahap 5: Proses melalui Plugboard lagi (kedua kali).
        c = processPlugboard(c);

        // Menyimpan karakter terenkripsi ke buffer ciphertext.
        ciphertext[i] = c;
    }
    ciphertext[len] = '\0'; // Menambahkan null terminator pada akhir ciphertext.

    printSuccess("Teks berhasil dienkripsi!"); // Pesan sukses.
    return true;                               // Enkripsi berhasil.
}

// Fungsi utama untuk mendekripsi ciphertext yang dienkripsi dengan simulasi Enigma.
// Parameter ciphertext: string input terenkripsi.
// Parameter plaintext: buffer untuk menyimpan hasil dekripsi.
// Parameter maxLength: ukuran maksimum buffer plaintext.
// Mengembalikan true jika dekripsi berhasil, false jika gagal.
// Proses dekripsi Enigma pada dasarnya sama dengan enkripsi karena sifat simetris dari komponennya
// (jika A dienkripsi menjadi X, maka X dienkripsi menjadi A dengan konfigurasi yang sama).
// Namun, implementasi rotor balik di sini mungkin perlu diperhatikan apakah benar-benar simetris
// dengan implementasi rotor maju di enkripsi.
boolean enigmaDecrypt(const char *ciphertext, char *plaintext, int maxLength)
{
    // Validasi parameter input (sama seperti enkripsi).
    if (!ciphertext || !plaintext || maxLength <= 0)
    {
        printError("Parameter dekripsi tidak valid!");
        return false;
    }

    int len = strlen(ciphertext); // Panjang ciphertext.
    // Validasi panjang output buffer.
    if (len >= maxLength)
    {
        printError("Panjang input melebihi kapasitas output!");
        return false;
    }

    // Validasi karakter dalam ciphertext (sama seperti enkripsi).
    for (int i = 0; i < len; i++)
    {
        char c = ciphertext[i];
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')))
        {
            printError("Input mengandung karakter tidak valid! Hanya huruf A-Z dan angka 0-9 diperbolehkan.");
            return false;
        }
    }

    // Menghitung nilai pergeseran dinamis untuk dekripsi angka.
    // Penting: `calculateDynamicShift` di sini menggunakan `ciphertext` sebagai input.
    // Agar dekripsi angka berhasil, `calculateDynamicShift(ciphertext)` harus menghasilkan nilai
    // yang sama dengan `calculateDynamicShift(plaintext)` yang digunakan saat enkripsi.
    // Ini akan terjadi jika proses enkripsi/dekripsi huruf tidak mengubah jumlah total nilai ASCII
    // atau jika perubahannya konsisten sehingga modulo 10 tetap sama, atau jika angka dienkripsi/didekripsi
    // secara terpisah dan tidak mempengaruhi shift untuk huruf.
    // Dalam implementasi ini, `numberShift` hanya untuk angka, jadi `calculateDynamicShift` dipanggil pada `plaintext` saat enkripsi
    // dan pada `ciphertext` saat dekripsi. Jika enkripsi angka bersifat reversibel dengan shift yang sama,
    // maka `calculateDynamicShift(ciphertext)` harus menghasilkan shift yang sama dengan `calculateDynamicShift(plaintext asli)`.
    // Namun, karena enkripsi angka mengubah angkanya, `calculateDynamicShift(ciphertext)` mungkin berbeda dari `calculateDynamicShift(plaintext asli)`.
    // Agar dekripsi angka berhasil, `decryptNumber` harus menggunakan shift yang sama persis dengan `encryptNumber`.
    // Jadi, `calculateDynamicShift` harus dipanggil pada `plaintext asli` untuk mendapatkan `numberShift` yang benar untuk dekripsi,
    // atau `numberShift` harus disimpan/diturunkan dari proses enkripsi.
    // Kode saat ini menghitungnya dari `ciphertext`, yang mungkin tidak benar untuk membalikkan enkripsi angka.
    // **Komentar ini berdasarkan analisis kode, tanpa mengubah kode.**
    int numberShift = calculateDynamicShift(ciphertext); // Seharusnya menggunakan shift yang sama dengan saat enkripsi.

    // Bagian "Simpan konfigurasi sebagai string" (sama seperti di enkripsi).
    // String 'config' terenkripsi ini juga tidak digunakan dalam proses dekripsi karakter per karakter.
    char config[100] = "";
    strcat(config, rotor);
    strcat(config, "|");
    char posStr[10] = "";
    int temp = rotorPosition;
    int index = 0;
    if (temp == 0)
    {
        posStr[index++] = '0';
    }
    else
    {
        while (temp > 0)
        {
            posStr[index++] = (temp % 10) + '0';
            temp /= 10;
        }
    }
    posStr[index] = '\0';
    for (int i = 0; i < index / 2; i++)
    {
        char swap = posStr[i];
        posStr[i] = posStr[index - 1 - i];
        posStr[index - 1 - i] = swap;
    }
    strcat(config, posStr);
    strcat(config, "|");
    strcat(config, reflector);
    caesarEncryptConfig(config, caesarShift);

    // Proses dekripsi karakter per karakter dari ciphertext.
    for (int i = 0; i < len; i++)
    {
        char c = ciphertext[i]; // Mengambil karakter ciphertext saat ini.

        // Jika karakter adalah angka.
        if ('0' <= c && c <= '9')
        {
            // Dekripsi angka menggunakan fungsi decryptNumber dengan numberShift.
            // Keberhasilan ini bergantung pada apakah `numberShift` yang dihitung di atas adalah kebalikan yang benar.
            plaintext[i] = decryptNumber(c, numberShift);
            continue; // Lanjutkan ke karakter berikutnya.
        }

        // Jika karakter adalah huruf, normalisasi ke huruf besar.
        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A';

        // Tahap 1: Proses melalui Plugboard (pertama kali, sama seperti enkripsi).
        c = processPlugboard(c);

        // Tahap 2: Proses melalui Rotor (jalur balik, ini adalah langkah pertama rotor dalam dekripsi,
        //          yang seharusnya merupakan kebalikan dari langkah terakhir rotor dalam enkripsi yaitu rotor[pos]).
        // Logika rotor balik yang digunakan di enkripsi adalah: cari j s.t. rotor[j] == c, lalu c = 'A' + ((j - rotorPosition + 26) % 26)
        // Logika di sini:
        int pos = (c - 'A' + rotorPosition) % 26; // Ini mirip dengan rotor maju di enkripsi.
        // Loop mencari j dimana rotor[j] adalah karakter target ('A' + pos).
        // Ini mencari input ke rotor maju yang akan menghasilkan 'A' + pos.
        // Ini adalah invers dari: output = rotor[(input_index + rotorPosition) % 26]
        // Kita punya output (yaitu 'c' setelah plugboard), kita ingin input.
        // Seharusnya, jika enkripsi adalah c_out = rotor[(c_in_idx + shift) % 26],
        // maka dekripsi adalah c_in_idx = (rotor_inverse[c_out_idx] - shift + 26) % 26.
        // Kode ini melakukan:
        // 1. `pos = (c - 'A' + rotorPosition) % 26;`  Ini seperti menghitung output dari rotor maju jika inputnya `c` (setelah plugboard)
        //    dan kemudian menggesernya. Ini tidak terlihat sebagai langkah rotor balik yang standar.
        //    Mari kita telusuri: `c` adalah output dari reflektor (setelah plugboard & rotor maju & reflektor).
        //    Langkah ini seharusnya membalikkan `c = rotor[pos]` dari enkripsi (langkah 2 enkripsi).
        //    Di enkripsi (maju): `c_rotor_out = rotor[(c_plugboard_out_idx + rotorPosition) % 26]`
        //    Di dekripsi (balik, langkah ini): kita punya `c_reflector_out`. Ini harusnya masuk ke rotor balik.
        //    Logika yang benar untuk rotor balik (setelah reflektor): cari `j` sehingga `rotor[j]` menghasilkan `c_reflector_out` (setelah penyesuaian `rotorPosition`).
        //    `int target_char_idx = c - 'A';`
        //    `int rotor_output_idx_no_shift;`
        //    `for (int j=0; j<26; ++j) { if (rotor[(j + rotorPosition) % 26] == c) { rotor_output_idx_no_shift = j; break; } }`
        //    `c = 'A' + rotor_output_idx_no_shift;`
        // Kode yang ada: `pos = (c - 'A' + rotorPosition) % 26;` lalu `for (j if rotor[j] == 'A' + pos) c = 'A' + j;`
        // Ini mencari `j` sehingga `rotor[j]` sama dengan `('A' + ( (c-'A') + rotorPosition) % 26 )`.
        // Ini adalah langkah yang berbeda dari rotor balik di enkripsi.
        // **Komentar ini berdasarkan analisis, tanpa mengubah kode.**
        for (int j = 0; j < 26; j++)
        {
            // Mencari indeks 'j' pada `rotor` yang nilainya sama dengan karakter target ('A' + pos).
            // 'A' + pos adalah karakter yang dihasilkan jika karakter (c-'A') digeser oleh rotorPosition dan kemudian dipetakan.
            if (rotor[j] == 'A' + pos) // Ini adalah bagian yang perlu dicermati untuk kebenaran logika Enigma.
            {
                c = 'A' + j; // Hasilnya adalah indeks 'j' tersebut sebagai karakter.
                break;
            }
        }

        // Tahap 3: Proses melalui Reflektor (sama seperti enkripsi, karena reflektor simetris).
        c = reflector[c - 'A'];

        // Tahap 4: Proses melalui Rotor (jalur maju, ini adalah langkah terakhir rotor dalam dekripsi,
        //          yang seharusnya merupakan kebalikan dari langkah pertama rotor dalam enkripsi).
        // Logika rotor maju di enkripsi: `pos = (c - 'A' + rotorPosition) % 26; c = rotor[pos];`
        // Logika di sini: `pos = (c - 'A' - rotorPosition + 26) % 26; c = rotor[pos];`
        // Pengurangan `rotorPosition` di sini adalah upaya untuk membalikkan penambahan di enkripsi.
        // Ini adalah pemetaan rotor maju dengan shift terbalik.
        pos = (c - 'A' - rotorPosition + 26) % 26; // Penambahan 26 untuk menangani hasil negatif.
        c = rotor[pos];                            // Substitusi karakter.

        // Tahap 5: Proses melalui Plugboard lagi (sama seperti enkripsi).
        c = processPlugboard(c);

        // Menyimpan karakter terdekripsi ke buffer plaintext.
        plaintext[i] = c;
    }
    plaintext[len] = '\0'; // Menambahkan null terminator.

    printSuccess("Teks berhasil didekripsi!"); // Pesan sukses.
    return true;                               // Dekripsi berhasil.
}
