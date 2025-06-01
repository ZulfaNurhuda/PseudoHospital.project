#include "utils.h"

void *safeMalloc(size_t size)
{
    void *ptr = malloc(size); // Melakukan alokasi memori menggunakan malloc standar.
    // Jika malloc gagal mengalokasikan memori dan mengembalikan NULL.
    if (ptr == NULL)
    {
        // Pemanggil fungsi ini bertanggung jawab untuk menangani kasus NULL.
        // Tidak ada pencetakan error atau penghentian program di sini agar pemanggil dapat memutuskan tindakan selanjutnya.
        return NULL;
    }
    // Jika alokasi berhasil, kembalikan pointer ke memori yang dialokasikan.
    return ptr;
}

char localCharToLower(char c)
{
    // Memeriksa apakah karakter `c` adalah huruf besar (berada dalam rentang ASCII 'A' hingga 'Z').
    if (c >= 'A' && c <= 'Z')
    {
        // Mengkonversi ke huruf kecil dengan menambahkan selisih ASCII antara 'a' dan 'A'.
        // Contoh: 'B' (ASCII 66) + ('a' (97) - 'A' (65)) = 66 + 32 = 98 (ASCII untuk 'b').
        return c + ('a' - 'A');
    }
    // Jika karakter bukan huruf besar, kembalikan karakter tersebut tanpa perubahan.
    return c;
}

int localCaseInsensitiveStrcmp(const char *s1, const char *s2)
{
    // Menangani kasus jika kedua string adalah NULL, dianggap sama.
    if (s1 == NULL && s2 == NULL)
        return 0;
    // Menangani kasus jika hanya s1 yang NULL, s1 dianggap lebih kecil (konsisten dengan perilaku strcmp standar).
    if (s1 == NULL)
        return -1;
    // Menangani kasus jika hanya s2 yang NULL, s1 dianggap lebih besar.
    if (s2 == NULL)
        return 1;

    int i = 0; // Indeks untuk iterasi melalui karakter-karakter string.
    // Melakukan iterasi selama kedua string belum mencapai karakter null terminator ('\0').
    while (s1[i] != '\0' && s2[i] != '\0')
    {
        // Mengubah karakter dari kedua string ke huruf kecil untuk perbandingan menggunakan customCharToLower.
        char c1_lower = customCharToLower(s1[i]);
        char c2_lower = customCharToLower(s2[i]);
        // Jika karakter yang sudah dikecilkan pada posisi saat ini berbeda.
        if (c1_lower != c2_lower)
        {
            // Kembalikan selisih nilai ASCII-nya, yang akan menentukan urutan leksikografis.
            return c1_lower - c2_lower;
        }
        i++; // Pindah ke karakter berikutnya di kedua string.
    }
    // Setelah salah satu string (atau keduanya) mencapai akhir,
    // bandingkan karakter pada posisi `i` (yang mungkin null terminator untuk salah satu atau keduanya).
    // Ini penting untuk menangani kasus di mana satu string adalah awalan dari string lain (misalnya, "apple" vs "applepie").
    return customCharToLower(s1[i]) - customCharToLower(s2[i]);
}

boolean readValidString(char *buffer, int maxLen, const char *prompt, boolean alphanumeric)
{
    // Validasi parameter fungsi: buffer tidak NULL, maxLen minimal 2 (1 char + null), prompt tidak NULL.
    if (buffer == NULL || maxLen < 2 || prompt == NULL)
    {
        return false; // Gagal jika parameter tidak valid.
    }

    printf("%s", prompt); // Menampilkan prompt ke pengguna.

    char temp[256]; // Buffer sementara yang lebih besar (256 char) untuk membaca input awal dari scanf.
                    // Ini membantu mencegah overflow pada `buffer` jika input pengguna sangat panjang.

    // Membaca input string (satu kata, hingga 255 karakter) menggunakan scanf "%255s".
    // Format "%255s" membatasi jumlah karakter yang dibaca untuk mencegah buffer overflow pada `temp`.
    // scanf mengembalikan jumlah item yang berhasil dibaca (seharusnya 1 jika berhasil).
    if (scanf("%255s", temp) != 1)
    {
        // Jika gagal membaca (misalnya, karena EOF atau error input lainnya).
        // Membersihkan sisa buffer input standar (stdin) untuk menghindari masalah pada pembacaan berikutnya.
        int c_clear; // Variabel sementara untuk menampung karakter yang dibuang.
        // Loop untuk membuang semua karakter dari buffer stdin hingga newline ('\n') atau End-Of-File (EOF) ditemui.
        while ((c_clear = getchar()) != '\n' && c_clear != EOF)
            ;

        printError("Gagal membaca input."); // Cetak pesan error.
        return false;                       // Pembacaan gagal.
    }

    // Membersihkan sisa buffer input standar setelah pembacaan berhasil (jika ada sisa karakter di baris yang sama setelah kata pertama).
    // Ini penting agar input berikutnya tidak terpengaruh oleh sisa input dari baris ini.
    int c_clear_after;
    while ((c_clear_after = getchar()) != '\n' && c_clear_after != EOF)
        ;

    // Memeriksa panjang string yang dibaca (disimpan di `temp`).
    size_t len = strlen(temp);
    // Jika panjang string (tidak termasuk null terminator) sama atau lebih besar dari `maxLen` yang diizinkan untuk `buffer` output.
    if (len >= maxLen)
    {
        printError("Input terlalu panjang!"); // Cetak pesan error.
        return false;                         // Input terlalu panjang untuk `buffer`.
    }

    // Memeriksa apakah input kosong. Setelah `scanf %s`, `len` akan 0 jika tidak ada karakter non-whitespace yang dibaca
    // (misalnya, jika pengguna hanya menekan Enter pada baris kosong sebelum input lain, atau jika input adalah EOF).
    if (len == 0)
    {
        printError("Input tidak boleh kosong!"); // Cetak pesan error.
        return false;                            // Input kosong tidak diizinkan.
    }

    // Validasi karakter alfanumerik jika flag `alphanumeric` adalah true.
    if (alphanumeric)
    {
        // Melakukan iterasi melalui setiap karakter dalam string `temp`.
        for (int i = 0; temp[i] != '\0'; i++) // Loop hingga null terminator.
        {
            char c_char_val = temp[i]; // Karakter saat ini.
            // Memeriksa apakah karakter BUKAN huruf besar (A-Z), BUKAN huruf kecil (a-z),
            // BUKAN angka (0-9), dan BUKAN underscore ('_').
            if (!((c_char_val >= 'A' && c_char_val <= 'Z') || (c_char_val >= 'a' && c_char_val <= 'z') ||
                  (c_char_val >= '0' && c_char_val <= '9') || c_char_val == '_'))
            {
                printError("Input hanya boleh berisi huruf, angka, atau underscore!"); // Cetak pesan error.
                return false;                                                          // Karakter tidak valid ditemukan.
            }
        }
    }

    // Jika semua validasi lolos, salin string dari buffer sementara `temp` ke buffer output `buffer`.
    strcpy(buffer, temp);
    return true; // Pembacaan dan validasi berhasil.
}

boolean readStringWithSpaces(char *buffer, int bufferSize, const char *prompt)
{
    // Validasi parameter buffer dan bufferSize.
    if (buffer == NULL || bufferSize <= 0)
    {                 // bufferSize harus positif.
        return false; // Gagal jika buffer tidak valid.
    }
    // Jika bufferSize hanya 1, hanya cukup untuk menampung null terminator.
    if (bufferSize == 1)
    {
        buffer[0] = '\0'; // Set buffer menjadi string kosong.
        // Membersihkan sisa baris input jika ada (misalnya, hanya karakter newline yang tersisa).
        int r1_clear_single;
        // Loop untuk membuang semua karakter dari buffer stdin hingga newline atau EOF.
        while ((r1_clear_single = getchar()) != '\n' && r1_clear_single != EOF)
            ;
        return true; // Dianggap berhasil dengan string kosong karena buffer sangat kecil.
    }

    printf("%s", prompt); // Menampilkan prompt kepada pengguna.

    char rawInput[256]; // Buffer sementara yang lebih besar (256 char) untuk membaca seluruh baris input awal.
    rawInput[0] = '\0'; // Inisialisasi sebagai string kosong untuk keamanan.

    // Langkah 1: Membaca seluruh baris atau hingga kapasitas rawInput (255 karakter + null).
    // "%255[^\n]" membaca semua karakter KECUALI newline ('\n'), hingga 255 karakter.
    // Mengembalikan jumlah item yang berhasil di-assign (1 jika berhasil, 0 jika char pertama newline, EOF jika error/akhir file).
    int scanf_res = scanf("%255[^\n]", rawInput);

    // Langkah 2: Membersihkan sisa buffer input standar (karakter newline dan potensi overflow jika input > 255 char).
    int clear_res_loop;
    // Loop untuk membuang semua karakter hingga newline atau EOF.
    // scanf("%*[^\n]") akan membaca dan membuang semua karakter kecuali newline.
    // Mengembalikan 1 jika berhasil membaca (dan membuang) sesuatu, 0 jika karakter pertama adalah newline, EOF jika error/akhir file.
    do
    {
        clear_res_loop = scanf("%*[^\n]");
    } while (clear_res_loop == 1 && clear_res_loop != EOF); // Lanjutkan jika berhasil membuang sesuatu dan bukan EOF.

    // Jika loop di atas berhenti bukan karena EOF (artinya ada newline atau karakter lain yang tersisa di buffer).
    if (clear_res_loop != EOF)
    {
        scanf("%*c"); // Membaca dan membuang satu karakter tersebut (biasanya newline).
    }

    // Menangani kasus EOF sejati terjadi sebelum input apapun berhasil ditangkap ke rawInput.
    if (scanf_res == EOF && rawInput[0] == '\0')
    {
        buffer[0] = '\0'; // Set buffer output ke string kosong.
        return false;     // Kembalikan false karena tidak ada input yang valid yang bisa diproses.
    }
    // Jika scanf_res adalah 0, berarti karakter pertama yang ditemui adalah newline (rawInput akan tetap kosong).
    // Jika scanf_res adalah 1, berarti sejumlah karakter (selain newline) berhasil dibaca ke rawInput.
    // Jika scanf_res adalah EOF tetapi rawInput memiliki data (misalnya, input dari file tanpa newline di akhir), maka rawInput tetap perlu diproses.

    // Langkah 3: Memangkas spasi di awal/akhir dan mengkondensasi spasi internal dari rawInput ke buffer output.
    int i = 0;              // Indeks untuk iterasi rawInput.
    int j = 0;              // Indeks untuk iterasi buffer (output).
    boolean inWord = false; // Flag untuk menandakan apakah saat ini berada di dalam sebuah kata (digunakan untuk kondensasi spasi).

    // Lewati spasi di awal (leading spaces) dari rawInput.
    while (rawInput[i] == ' ')
    {
        i++;
    }

    // Memproses sisa string: salin karakter non-spasi, ubah beberapa spasi berurutan menjadi satu spasi tunggal.
    // Loop berjalan selama belum mencapai akhir rawInput dan masih ada ruang di buffer output (`j < bufferSize - 1` untuk menyisakan ruang bagi null terminator).
    while (rawInput[i] != '\0' && j < bufferSize - 1)
    {
        if (rawInput[i] == ' ')
        { // Jika karakter saat ini adalah spasi.
            if (inWord)
            {                      // Jika karakter sebelumnya adalah non-spasi (artinya ini adalah spasi pertama setelah sebuah kata).
                buffer[j++] = ' '; // Tambahkan satu spasi ke buffer output.
                inWord = false;    // Set flag bahwa sekarang berada di antara kata (bukan di dalam kata).
            }
            // Spasi berlebih (multiple spaces) akan dilewati secara otomatis karena `i` akan di-increment di akhir loop,
            // dan kondisi `inWord` akan tetap false pada spasi-spasi berikutnya hingga karakter non-spasi ditemui.
        }
        else
        {                              // Jika karakter saat ini bukan spasi.
            buffer[j++] = rawInput[i]; // Salin karakter ke buffer output.
            inWord = true;             // Set flag bahwa sekarang berada di dalam sebuah kata.
        }
        i++; // Pindah ke karakter berikutnya di rawInput.
    }
    // Setelah loop selesai, jika karakter terakhir yang disalin ke buffer output adalah spasi, hapus spasi tersebut (trailing space).
    if (j > 0 && buffer[j - 1] == ' ')
    {
        j--; // Mundurkan indeks j, sehingga null terminator berikutnya akan menimpa spasi terakhir ini.
    }
    buffer[j] = '\0'; // Menambahkan null terminator pada akhir string yang sudah diproses di buffer output.

    return true; // Pembacaan dan pemrosesan berhasil.
}

boolean readUsernameWithTrim(char *buffer, int bufferSize, const char *prompt)
{
    // Fungsi ini dimaksudkan untuk membaca username, yang bisa menyertakan spasi.
    // Seharusnya memangkas spasi di awal/akhir dan mengkondensasi spasi internal.
    // `readStringWithSpaces` sudah menyediakan fungsionalitas ini.
    return readStringWithSpaces(buffer, bufferSize, prompt);
}

boolean readValidInt(int *value, const char *prompt)
{
    // Validasi parameter fungsi: pointer `value` dan `prompt` tidak boleh NULL.
    if (value == NULL || prompt == NULL)
    {
        return false; // Gagal jika parameter tidak valid.
    }

    char buffer[32]; // Buffer sementara untuk membaca input string angka. Ukuran 32 dianggap cukup untuk representasi string dari int.
    // Membaca input sebagai string menggunakan `readValidString`.
    // Argumen `false` untuk `alphanumeric` berarti tidak ada validasi alfanumerik khusus di sini (akan divalidasi sebagai digit nanti).
    if (!readValidString(buffer, sizeof(buffer), prompt, false))
    {
        return false; // Gagal jika `readValidString` gagal (misalnya, input terlalu panjang atau gagal baca).
    }

    // Memeriksa apakah string input kosong setelah dibaca oleh `readValidString`.
    // `readValidString` sendiri sudah mencegah input kosong, tapi ini sebagai lapisan pertahanan tambahan.
    if (buffer[0] == '\0')
    {
        printError("Input tidak boleh kosong!");
        return false;
    }

    long long result = 0;       // Menggunakan `long long` untuk mengakumulasi hasil parsing guna mendeteksi overflow sebelum konversi ke `int`.
    int i = 0;                  // Indeks untuk iterasi melalui karakter-karakter dalam `buffer`.
    boolean isNegative = false; // Flag untuk menandakan apakah angka yang diparsing adalah negatif.

    // Mendefinisikan konstanta batas secara manual karena `limits.h` mungkin tidak diizinkan atau tidak tersedia.
    const long long MAX_LONG_LONG = 9223372036854775807LL; // Batas atas untuk tipe data `long long`.
    const int MAX_INT = 2147483647;                        // Batas atas untuk tipe data `int`.
    // const int MIN_INT = -2147483648; // Batas bawah `int` (sebagai referensi, digunakan dalam logika di bawah).

    // Menangani tanda positif atau negatif di awal string.
    if (buffer[0] == '-') // Jika karakter pertama adalah tanda minus.
    {
        isNegative = true; // Set flag negatif.
        i++;               // Pindah ke karakter berikutnya setelah tanda.
        // Memeriksa apakah ada digit setelah tanda negatif. Jika tidak (misalnya, input hanya "-"), maka tidak valid.
        if (buffer[i] == '\0')
        {
            printError("Input harus berupa angka!");
            return false;
        }
    }
    else if (buffer[0] == '+') // Jika karakter pertama adalah tanda plus.
    {
        i++; // Pindah ke karakter berikutnya setelah tanda.
        // Memeriksa apakah ada digit setelah tanda positif.
        if (buffer[i] == '\0')
        {
            printError("Input harus berupa angka!");
            return false;
        }
    }

    // Melakukan parsing digit-digit angka dan memeriksa kemungkinan overflow.
    while (buffer[i] != '\0') // Loop selama belum mencapai akhir string.
    {
        // Jika karakter saat ini bukan digit (0-9).
        if (buffer[i] < '0' || buffer[i] > '9')
        {
            printError("Input harus berupa angka!");
            return false; // Input tidak valid.
        }

        // Pemeriksaan overflow sebelum melakukan perkalian `result * 10` dan penambahan digit.
        // Jika `result` sudah sangat besar, `result * 10` bisa overflow.
        // Kondisi ini mengecek apakah `result` sudah melebihi `(MAX_LONG_LONG - (digit saat ini)) / 10`.
        // Jika ya, maka `result * 10 + (digit saat ini)` pasti akan melebihi `MAX_LONG_LONG`.
        if (result > (MAX_LONG_LONG - (buffer[i] - '0')) / 10)
        {
            printError("Angka terlalu besar!"); // Overflow `long long` terdeteksi.
            return false;
        }

        // Akumulasi hasil: kalikan hasil sebelumnya dengan 10, lalu tambahkan nilai digit saat ini.
        // `(buffer[i] - '0')` mengkonversi karakter digit (misal '5') menjadi nilai integer-nya (5).
        result = result * 10 + (buffer[i] - '0');
        i++; // Pindah ke digit berikutnya.
    }

    // Menerapkan tanda negatif (jika ada) dan memeriksa apakah hasil (`result`) berada dalam rentang tipe data `int`.
    if (isNegative)
    {
        // Untuk angka negatif, batas bawah `int` adalah `MIN_INT`.
        // `result` saat ini adalah nilai absolut. Jadi, `-result` harus `>= MIN_INT`.
        // Karena `MIN_INT` biasanya `-(MAX_INT + 1)`, maka `result` (absolut) tidak boleh lebih besar dari `(long long)MAX_INT + 1`.
        if (result > (long long)MAX_INT + 1)
        {
            printError("Angka terlalu kecil!"); // Underflow (lebih kecil dari `MIN_INT`).
            return false;
        }
        *value = -(int)result; // Konversi `result` ke `int` dan terapkan tanda negatif.
    }
    else // Untuk angka positif.
    {
        // Memeriksa apakah `result` (yang positif) melebihi `MAX_INT`.
        if (result > MAX_INT)
        {
            printError("Angka terlalu besar!"); // Overflow (lebih besar dari `MAX_INT`).
            return false;
        }
        *value = (int)result; // Konversi `result` ke `int`.
    }

    return true; // Pembacaan dan konversi integer berhasil.
}

boolean readValidFloat(float *value, const char *prompt)
{
    // Validasi parameter fungsi: pointer `value` dan `prompt` tidak boleh NULL.
    if (value == NULL || prompt == NULL)
    {
        return false;
    }

    char buffer[32]; // Buffer sementara untuk membaca input string angka desimal.
    // Membaca input sebagai string menggunakan `readValidString`.
    // Argumen `false` untuk `alphanumeric` berarti tidak ada validasi alfanumerik khusus di sini.
    if (!readValidString(buffer, sizeof(buffer), prompt, false))
    {
        return false; // Gagal jika `readValidString` gagal.
    }

    // Memeriksa apakah string input kosong setelah dibaca.
    if (buffer[0] == '\0')
    {
        printError("Input tidak boleh kosong!");
        return false;
    }

    double result = 0.0;        // Menggunakan `double` untuk mengakumulasi hasil parsing demi presisi yang lebih baik, sebelum dikonversi ke `float`.
    double fraction = 0.1;      // Faktor pengali untuk bagian desimal (0.1, 0.01, 0.001, dst.).
    boolean isNegative = false; // Flag untuk menandakan apakah angka negatif.
    boolean hasDecimal = false; // Flag untuk menandakan apakah titik desimal sudah ditemui.
    boolean hasDigits = false;  // Flag untuk menandakan apakah setidaknya satu digit angka telah diproses.
    int i = 0;                  // Indeks untuk iterasi melalui karakter-karakter dalam `buffer`.

    // Menangani tanda positif atau negatif di awal string.
    if (buffer[0] == '-')
    {
        isNegative = true;
        i++; // Pindah ke karakter setelah tanda.
    }
    else if (buffer[0] == '+')
    {
        i++; // Pindah ke karakter setelah tanda.
    }

    // Memeriksa apakah ada karakter setelah tanda (jika tanda ada).
    // Jika input hanya "-" atau "+", itu tidak valid.
    if (buffer[i] == '\0' && (isNegative || (i > 0 && buffer[0] == '+')))
    {
        printError("Input harus berupa angka desimal!");
        return false;
    }
    // Menangani kasus jika input hanya satu karakter non-digit dan bukan titik (misalnya "a").
    // Kondisi `i==0` berarti belum ada tanda, `buffer[0] != '\0'` berarti ada karakter,
    // `(buffer[0] < '0' || buffer[0] > '9')` berarti bukan digit, `buffer[0] != '.'` berarti bukan titik.
    if (buffer[i] == '\0' && i == 0 && buffer[0] != '\0' && (buffer[0] < '0' || buffer[0] > '9') && buffer[0] != '.')
    {
        printError("Input harus berupa angka desimal!");
        return false;
    }

    // Melakukan parsing digit-digit angka dan titik desimal.
    while (buffer[i] != '\0') // Loop selama belum mencapai akhir string.
    {
        if (buffer[i] == '.') // Jika karakter saat ini adalah titik desimal.
        {
            if (hasDecimal) // Jika titik desimal sudah pernah ditemui sebelumnya.
            {
                printError("Input hanya boleh memiliki satu titik desimal!");
                return false; // Hanya satu titik desimal yang diizinkan.
            }
            hasDecimal = true; // Set flag bahwa titik desimal sudah ditemui.
            i++;               // Pindah ke karakter berikutnya.
            // Jika titik adalah karakter terakhir dan tidak ada digit sebelumnya (misal, input ".")
            // atau setelah tanda (misal, input "-."), maka tidak valid.
            if (buffer[i] == '\0' && !hasDigits)
            {
                printError("Input harus berupa angka desimal!");
                return false;
            }
            continue; // Lanjutkan ke iterasi berikutnya untuk memproses karakter setelah titik.
        }

        // Jika karakter saat ini bukan digit (0-9).
        if (buffer[i] < '0' || buffer[i] > '9')
        {
            printError("Input harus berupa angka desimal!");
            return false; // Karakter tidak valid.
        }

        hasDigits = true; // Setidaknya satu digit angka telah ditemukan dan diproses.

        // Akumulasi hasil berdasarkan apakah sedang memproses bagian integer atau desimal.
        if (hasDecimal) // Jika sedang memproses bagian setelah titik desimal.
        {
            result += (buffer[i] - '0') * fraction; // Tambahkan nilai digit dikalikan faktor fraksional.
            fraction *= 0.1;                        // Update faktor fraksional untuk digit desimal berikutnya (0.1 -> 0.01 -> 0.001).
        }
        else // Jika sedang memproses bagian sebelum titik desimal (bagian integer).
        {
            result = result * 10.0 + (buffer[i] - '0'); // Akumulasi bagian integer.
        }
        i++; // Pindah ke karakter berikutnya.
    }

    // Memeriksa apakah setidaknya satu digit angka telah diproses.
    // Ini penting untuk mencegah input seperti "." atau "-." (tanpa angka) dianggap valid.
    if (!hasDigits)
    {
        printError("Input harus berupa angka desimal!");
        return false;
    }

    // Menerapkan tanda negatif jika angka adalah negatif.
    if (isNegative)
    {
        result = -result;
    }

    // Pemeriksaan rentang dasar untuk tipe data `float`.
    // Nilai literal 3.4e38 adalah perkiraan kasar untuk FLT_MAX.
    // Penggunaan konstanta dari `<float.h>` (FLT_MAX, -FLT_MAX) akan lebih akurat dan portabel.
    if (result > 3.4e38 || result < -3.4e38)
    {
        printError("Angka terlalu besar atau terlalu kecil untuk float!"); // Overflow/underflow float.
        return false;
    }

    *value = (float)result; // Konversi hasil `double` ke `float` dan simpan ke variabel output.
    return true;            // Pembacaan dan konversi float berhasil.
}

int stringToInt(const char *str)
{
    int result = 0; // Variabel untuk menyimpan hasil konversi integer.
    int i = 0;      // Indeks untuk iterasi melalui karakter-karakter string.
    // Loop selama belum mencapai karakter null terminator ('\0') di akhir string.
    while (str[i] != '\0')
    {
        // Jika karakter saat ini bukan digit (berada di luar rentang '0' hingga '9').
        if (str[i] < '0' || str[i] > '9')
            return -1; // Kembalikan -1 sebagai indikasi error (karakter non-digit ditemukan).
        // Akumulasi hasil: kalikan hasil sebelumnya dengan 10, lalu tambahkan nilai integer dari digit saat ini.
        // `(str[i] - '0')` mengkonversi karakter digit (misalnya '5') menjadi nilai integer-nya (5).
        result = result * 10 + (str[i] - '0');
        i++; // Pindah ke karakter berikutnya dalam string.
    }
    return result; // Kembalikan hasil integer yang sudah dikonversi.
}

boolean integerToString(int value, char *buffer, int bufferSize)
{
    // Validasi parameter buffer: tidak NULL dan bufferSize minimal 2 (untuk 1 digit + null, atau tanda + 1 digit + null).
    if (buffer == NULL || bufferSize < 2)
    {
        // Jika buffer valid tapi terlalu kecil, coba kosongkan.
        if (buffer != NULL && bufferSize > 0)
            buffer[0] = '\0';
        return false; // Gagal karena buffer tidak valid atau terlalu kecil.
    }

    char tempReverse[12];        // Buffer sementara untuk menyimpan digit-digit angka dalam urutan terbalik.
                                 // Ukuran 12 cukup untuk menyimpan string dari INT_MIN (-2147483648 -> 10 digit + tanda '-' + null '\0').
    int i = 0;                   // Indeks untuk buffer `tempReverse`.
    long long longValue = value; // Menggunakan `long long` untuk menangani kasus khusus INT_MIN dengan benar saat dinegasikan.
                                 // `abs(INT_MIN)` bisa overflow jika `int` adalah tipe dasarnya.
    boolean isNegative = false;  // Flag untuk menandakan apakah `value` adalah negatif.

    // Menangani kasus khusus jika `value` adalah 0.
    if (longValue == 0)
    {
        if (bufferSize < 2)
        {                     // Perlu ruang minimal untuk string "0" (yaitu '0' dan '\0').
            buffer[0] = '\0'; // Kosongkan buffer jika tidak cukup.
            return false;
        }
        buffer[0] = '0';  // Simpan '0'.
        buffer[1] = '\0'; // Tambahkan null terminator.
        return true;      // Konversi berhasil.
    }

    // Menangani angka negatif.
    if (longValue < 0)
    {
        isNegative = true;      // Set flag negatif.
        longValue = -longValue; // Mengubah `longValue` menjadi positif. Ini aman untuk INT_MIN jika `longValue` adalah `long long`.
    }

    // Mengekstrak digit-digit dari `longValue` dalam urutan terbalik dan menyimpannya di `tempReverse`.
    while (longValue > 0)
    {
        // Pemeriksaan keamanan tambahan untuk mencegah overflow pada `tempReverse` (seharusnya tidak tercapai untuk `int`).
        if (i >= 11)
        {                     // `tempReverse` bisa menampung hingga 11 digit (untuk `long long` yang sangat besar, tapi `int` maks 10 digit).
            buffer[0] = '\0'; // Kosongkan buffer output.
            return false;     // Error.
        }
        tempReverse[i++] = (longValue % 10) + '0'; // Ambil digit terakhir (satuan), konversi ke karakter, simpan.
        longValue /= 10;                           // Hapus digit terakhir dari `longValue`.
    }

    // Memeriksa apakah buffer output (`buffer`) cukup besar untuk menampung string hasil.
    // `i` sekarang adalah jumlah digit. `(isNegative ? 1 : 0)` menambahkan 1 jika ada tanda negatif. `+ 1` untuk null terminator.
    int requiredSize = i + (isNegative ? 1 : 0) + 1;
    if (requiredSize > bufferSize)
    {
        buffer[0] = '\0'; // Kosongkan buffer jika tidak cukup.
        return false;     // Gagal karena buffer terlalu kecil.
    }

    // Membangun string akhir di `buffer` dari digit-digit yang ada di `tempReverse`.
    int k = 0; // Indeks untuk `buffer`.
    // Tambahkan tanda negatif '-' di awal `buffer` jika angka aslinya negatif.
    if (isNegative)
    {
        buffer[k++] = '-';
    }

    // Salin digit-digit dari `tempReverse` (yang urutannya terbalik) ke `buffer` dalam urutan yang benar.
    // Loop dari akhir `tempReverse` (indeks `i-1`) ke awal.
    while (i > 0)
    {
        buffer[k++] = tempReverse[--i]; // Ambil karakter digit dari `tempReverse`, decrement `i`, simpan ke `buffer`.
    }
    buffer[k] = '\0'; // Tambahkan null terminator pada akhir string di `buffer`.

    return true; // Konversi integer ke string berhasil.
}

boolean floatToString(float value, char *buffer, int bufferSize, int precision)
{
    // Validasi parameter: buffer tidak NULL, bufferSize minimal 2, presisi tidak negatif.
    if (buffer == NULL || bufferSize < 2 || precision < 0)
    {
        if (buffer != NULL && bufferSize > 0)
            buffer[0] = '\0'; // Kosongkan buffer jika mungkin.
        return false;
    }

    buffer[0] = '\0'; // Inisialisasi buffer sebagai string kosong untuk memulai.

    // Menangani kasus khusus: NaN (Not a Number).
    // `value != value` adalah cara standar dalam C untuk mengecek apakah sebuah float adalah NaN.
    if (value != value)
    {
        if (bufferSize >= 4)
        {                          // Perlu ruang untuk string "nan" dan null terminator ('\0').
            strcpy(buffer, "nan"); // Salin "nan" ke buffer.
            return true;
        }
        return false; // Gagal karena buffer tidak cukup.
    }

    // Menangani kasus khusus: Infinity (Positif atau Negatif).
    // `1.0f / 0.0f` menghasilkan infinity positif. `-1.0f / 0.0f` menghasilkan infinity negatif.
    if (value == 1.0f / 0.0f || value == -1.0f / 0.0f)
    {
        const char *infStr = (value < 0) ? "-inf" : "inf"; // Tentukan string "inf" atau "-inf".
        if (bufferSize >= (int)strlen(infStr) + 1)
        {                           // Cek apakah buffer cukup untuk string dan null terminator.
            strcpy(buffer, infStr); // Salin string infinity.
            return true;
        }
        return false; // Gagal karena buffer tidak cukup.
    }

    // Menangani kasus khusus jika `value` adalah 0.0 atau -0.0.
    if (value == 0.0f || value == -0.0f) // Perbandingan float dengan 0.0f.
    {
        int len = 1; // Untuk karakter '0'.
        if (precision > 0)
            len += 1 + precision; // Tambah 1 untuk '.' dan `precision` untuk digit desimal.

        if (bufferSize < len + 1)
            return false; // Cek ukuran buffer (+1 untuk null terminator).

        strcpy(buffer, "0"); // Mulai string dengan "0".
        if (precision > 0)
        {
            strcat(buffer, "."); // Tambahkan titik desimal.
            for (int i = 0; i < precision; i++)
            {
                strcat(buffer, "0"); // Tambahkan '0' sejumlah `precision`.
            }
        }
        return true; // Konversi berhasil.
    }

    // Mengambil nilai absolut dari `value` dan menentukan apakah `value` negatif.
    float absValue = value;
    boolean isNegative = false;
    if (value < 0.0f)
    {
        absValue = -value; // `absValue` sekarang positif.
        isNegative = true; // Set flag negatif.
    }

    // Mendapatkan bagian integer dari `absValue`.
    // Cast ke `unsigned long` untuk menangani rentang yang lebih besar dan menghindari masalah dengan `abs()` pada float.
    unsigned long intPart = (unsigned long)absValue;
    char intStr[32]; // Buffer untuk menyimpan representasi string dari bagian integer. Ukuran 32 dianggap aman.
    // Konversi bagian integer menjadi string menggunakan `integerToString`.
    if (!integerToString(intPart, intStr, sizeof(intStr)))
    {
        buffer[0] = '\0'; // Gagal konversi bagian integer.
        return false;
    }

    // Menghitung perkiraan ukuran buffer yang dibutuhkan untuk string hasil.
    int requiredSize = strlen(intStr); // Panjang string bagian integer.
    if (isNegative)
        requiredSize++; // Tambah 1 jika ada tanda '-'.
    if (precision > 0)
        requiredSize += 1 + precision; // Tambah 1 untuk '.' dan `precision` untuk digit desimal.

    // Cek apakah buffer output (`buffer`) cukup besar.
    if (bufferSize < requiredSize + 1) // +1 untuk null terminator.
    {
        buffer[0] = '\0'; // Kosongkan buffer jika tidak cukup.
        return false;
    }

    // Membangun string hasil di `buffer`.
    int currentLen = 0; // Indeks saat ini di `buffer`.

    // Tambahkan tanda negatif '-' di awal `buffer` jika angka aslinya negatif.
    if (isNegative)
    {
        buffer[currentLen++] = '-';
    }

    // Tambahkan bagian integer (yang sudah dalam bentuk string `intStr`) ke `buffer`.
    for (int j = 0; intStr[j] != '\0'; ++j)
    {
        buffer[currentLen++] = intStr[j];
    }

    // Tambahkan bagian desimal jika `precision` lebih besar dari 0.
    if (precision > 0)
    {
        buffer[currentLen++] = '.'; // Tambahkan titik desimal.

        float fracPart = absValue - (float)intPart; // Dapatkan bagian desimal murni dari `absValue`.

        // Melakukan pembulatan (rounding) sederhana pada bagian desimal.
        // Tambahkan 0.5 pada posisi digit terakhir dari presisi yang diinginkan.
        // Contoh: jika presisi 2, tambahkan 0.005. Jika presisi 1, tambahkan 0.05.
        float roundingFactor = 0.5f;
        for (int p = 0; p < precision; ++p)
        {
            roundingFactor /= 10.0f;
        }
        fracPart += roundingFactor; // Tambahkan faktor pembulatan ke bagian desimal.

        // Memeriksa apakah pembulatan menyebabkan "carry-over" ke bagian integer
        // (misalnya, 0.99 dengan presisi 1 dibulatkan menjadi 1.0).
        if (fracPart >= 1.0f)
        {
            intPart++; // Increment bagian integer.
            // Setelah carry-over, bagian desimal yang akan dicetak menjadi 0 semua.
            // fracPart direset untuk mencerminkan ini (atau bisa juga fracPart -= 1.0f jika ingin sisa desimalnya).
            // Untuk kesederhanaan, kita akan menganggapnya 0 dan merekonstruksi bagian integer.
            fracPart = 0.0f;

            // Rekonstruksi string bagian integer (`intStr`) karena nilainya berubah.
            if (!integerToString(intPart, intStr, sizeof(intStr)))
            {
                buffer[0] = '\0';
                return false; // Gagal konversi bagian integer yang baru.
            }

            // Hitung ulang ukuran buffer yang dibutuhkan karena panjang `intStr` mungkin berubah.
            requiredSize = strlen(intStr);
            if (isNegative)
                requiredSize++;
            if (precision > 0)
                requiredSize += 1 + precision;
            if (bufferSize < requiredSize + 1)
            {
                buffer[0] = '\0';
                return false;
            } // Cek ulang ukuran buffer.

            // Bangun ulang string dari awal di `buffer` dengan bagian integer yang sudah diupdate.
            currentLen = 0; // Reset indeks buffer.
            if (isNegative)
                buffer[currentLen++] = '-'; // Tambah tanda jika perlu.
            for (int j = 0; intStr[j] != '\0'; ++j)
                buffer[currentLen++] = intStr[j]; // Salin intStr baru.
            buffer[currentLen++] = '.';           // Tambahkan titik desimal lagi.
        }

        // Mengekstrak digit-digit bagian desimal satu per satu.
        for (int p = 0; p < precision; p++)
        {
            fracPart *= 10.0f;         // Geser digit desimal pertama ke posisi satuan.
            int digit = (int)fracPart; // Ambil bagian integer sebagai digit saat ini.

            // Menjepit (clamp) nilai digit antara 0 dan 9 untuk menghindari masalah dari
            // ketidakakuratan representasi float yang bisa menghasilkan digit < 0 atau > 9.
            if (digit < 0)
                digit = 0;
            if (digit > 9)
                digit = 9;

            buffer[currentLen++] = (char)(digit + '0'); // Tambahkan karakter digit ke `buffer`.
            fracPart -= (float)digit;                   // Kurangi digit yang sudah diambil dari `fracPart` untuk iterasi berikutnya.
        }
    }

    buffer[currentLen] = '\0'; // Tambahkan null terminator pada akhir string hasil.
    return true;               // Konversi float ke string berhasil.
}

boolean isValidUsername(const char *username)
{
    // Periksa apakah pointer username adalah NULL.
    if (username == NULL)
    {
        return false; // Username tidak valid jika NULL.
    }
    int len = 0; // Variabel untuk menyimpan panjang username.
    // Menghitung panjang username secara manual dengan iterasi hingga null terminator.
    // Alternatifnya bisa menggunakan `strlen(username)` jika diizinkan.
    while (username[len] != '\0')
    {
        len++;
    }

    // Periksa panjang username. Harus antara 1 dan 50 karakter.
    if (len == 0 || len > 50)
    {
        return false; // Tidak valid jika kosong atau terlalu panjang.
    }

    boolean allSpaces = true; // Flag untuk melacak apakah semua karakter dalam username adalah spasi.
    // Iterasi melalui setiap karakter dalam username.
    for (int i = 0; i < len; i++)
    {
        char c = username[i];        // Karakter saat ini.
        boolean charIsValid = false; // Flag untuk menandakan apakah karakter `c` valid.

        // Periksa apakah karakter adalah huruf (besar atau kecil).
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            charIsValid = true;
            allSpaces = false; // Jika ada huruf, berarti username tidak seluruhnya spasi.
        }
        // Periksa apakah karakter adalah angka.
        else if (c >= '0' && c <= '9')
        {
            charIsValid = true;
            allSpaces = false; // Jika ada angka, berarti tidak seluruhnya spasi.
        }
        // Periksa apakah karakter adalah underscore.
        else if (c == '_')
        {
            charIsValid = true;
            allSpaces = false; // Jika ada underscore, berarti tidak seluruhnya spasi.
        }
        // Periksa apakah karakter adalah spasi (spasi diizinkan sebagai karakter internal).
        else if (c == ' ')
        {
            charIsValid = true;
            // Jangan set `allSpaces` ke `false` untuk karakter spasi,
            // karena kita ingin mendeteksi jika username *hanya* terdiri dari spasi.
        }

        // Jika karakter `c` tidak memenuhi salah satu kriteria di atas (bukan huruf, angka, underscore, atau spasi).
        if (!charIsValid)
        {
            return false; // Username tidak valid karena mengandung karakter terlarang.
        }
    }

    // Setelah memeriksa semua karakter, jika username tidak kosong (len > 0 sudah dicek di awal)
    // DAN semua karakternya adalah spasi (`allSpaces` masih true).
    if (allSpaces && len > 0)
    {
        return false; // Username tidak valid jika hanya terdiri dari spasi.
    }

    return true; // Semua pemeriksaan lolos, username dianggap valid.
}

boolean isUsernameTaken(Hospital *hospital, const char *username)
{
    // Validasi input: pointer hospital dan username tidak boleh NULL.
    if (hospital == NULL || username == NULL)
    {
        return true; // Kondisi error, lebih aman menganggap username sudah terdaftar untuk mencegah duplikasi akibat error.
    }

    char lowerUsername[51];             // Buffer untuk menyimpan versi huruf kecil dari `username` input.
                                        // Ukuran 51 untuk maksimal 50 karakter username + 1 null terminator.
    int usernameLen = strlen(username); // Dapatkan panjang username input.
    // Mencegah buffer overflow jika `username` input lebih panjang dari 50 karakter
    // (meskipun `isValidUsername` seharusnya sudah menangani ini, ini sebagai lapisan pengaman).
    if (usernameLen > 50)
        usernameLen = 50;

    // Konversi `username` input ke huruf kecil dan simpan di `lowerUsername`.
    for (int i = 0; i < usernameLen; i++)
    {
        char c = username[i];
        if (c >= 'A' && c <= 'Z')
        {                                       // Jika huruf besar.
            lowerUsername[i] = c + ('a' - 'A'); // Konversi ke huruf kecil.
        }
        else
        {                         // Jika sudah huruf kecil, angka, atau simbol.
            lowerUsername[i] = c; // Salin apa adanya.
        }
    }
    lowerUsername[usernameLen] = '\0'; // Tambahkan null terminator.

    // Iterasi melalui daftar pengguna yang sudah ada di `hospital->users.elements`.
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        char lowerElementUsername[51];                                       // Buffer untuk username pengguna yang ada (versi huruf kecil).
        const char *existingUsername = hospital->users.elements[i].username; // Pointer ke username yang sudah ada.
        int elementLen = strlen(existingUsername);                           // Panjang username yang sudah ada.
        // Mencegah buffer overflow.
        if (elementLen > 50)
            elementLen = 50;

        // Konversi `existingUsername` ke huruf kecil dan simpan di `lowerElementUsername`.
        for (int j = 0; j < elementLen; j++)
        {
            char c = existingUsername[j];
            if (c >= 'A' && c <= 'Z')
            {
                lowerElementUsername[j] = c + ('a' - 'A');
            }
            else
            {
                lowerElementUsername[j] = c;
            }
        }
        lowerElementUsername[elementLen] = '\0'; // Null terminator.

        // Bandingkan `lowerUsername` (input) dengan `lowerElementUsername` (dari daftar).
        // `strcmp` bersifat case-sensitive, tetapi karena kedua string sudah diubah ke huruf kecil,
        // perbandingan ini efektif menjadi case-insensitive.
        if (strcmp(lowerUsername, lowerElementUsername) == 0)
        {                // Jika hasilnya 0, berarti string sama.
            return true; // Username sudah terdaftar.
        }
    }
    return false; // Jika loop selesai tanpa menemukan kecocokan, username belum terdaftar (unik).
}

boolean isValidRoomCode(Hospital *hospital, const char *roomCode)
{
    // Periksa panjang kode ruangan. Minimal 2 karakter (misal "A1").
    // Maksimal 4 karakter (misal "Z99" atau "A100" jika kolom bisa 3 digit, tapi colStr[4] jadi batasannya 3 digit kolom).
    // Jika `roomCode` adalah "A10", `strlen` adalah 3. `colStr` akan "10".
    // Jika `roomCode` adalah "A100", `strlen` adalah 4. `colStr` akan "100".
    // Batas `strlen(roomCode) > 4` berarti kode seperti "A1000" tidak valid.
    if (strlen(roomCode) < 2 || strlen(roomCode) > 4)
        return false; // Panjang tidak valid.

    char row = roomCode[0];       // Karakter pertama adalah representasi baris (huruf).
    char colStr[4];               // Buffer untuk menyimpan bagian kolom (angka) dari kode. Ukuran 4 untuk maks 3 digit + null.
    strcpy(colStr, roomCode + 1); // Salin bagian string setelah karakter pertama (bagian angka) ke `colStr`.

    // Validasi karakter baris: harus huruf besar dari 'A' hingga 'Z'.
    if (!(row >= 'A' && row <= 'Z'))
        return false; // Karakter baris tidak valid.

    // Validasi karakter kolom: semua karakter dalam `colStr` harus digit ('0'-'9').
    for (int i = 0; colStr[i] != '\0'; i++) // Loop selama belum null terminator.
    {
        if (!(colStr[i] >= '0' && colStr[i] <= '9'))
            return false; // Jika ada karakter non-digit di bagian kolom.
    }

    // Konversi bagian kolom (string `colStr`) menjadi nilai integer.
    int col = 0; // Variabel untuk menyimpan nilai integer kolom.
    for (int i = 0; colStr[i] != '\0'; i++)
    {
        col = col * 10 + (colStr[i] - '0'); // Akumulasi nilai integer dari karakter digit.
    }

    // Validasi apakah baris dan kolom yang sudah dikonversi berada dalam batas denah rumah sakit.
    // `row - 'A'` mengubah huruf baris (A, B, ...) menjadi indeks 0-based (0, 1, ...).
    // Indeks baris harus lebih kecil dari `hospital->layout.rowEff` (jumlah baris efektif).
    // Nomor kolom (`col`, yang 1-based) harus lebih kecil atau sama dengan `hospital->layout.colEff` dan lebih besar dari 0.
    if (row - 'A' >= hospital->layout.rowEff || col > hospital->layout.colEff || col <= 0)
        return false; // Kode ruangan berada di luar batas denah yang valid.

    // Jika semua pemeriksaan lolos, kode ruangan dianggap valid.
    // Catatan: Fungsi ini memvalidasi format dan apakah kode berada dalam *dimensi* denah.
    // Ini tidak secara eksplisit memeriksa apakah kode tersebut adalah salah satu dari `hospital->layout.elements[i][j].code` yang ada.
    // Namun, jika `hospital->layout.rowEff` dan `colEff` mencerminkan dimensi denah yang benar-benar diisi,
    // maka pemeriksaan batas ini secara implisit juga memvalidasi keberadaan dalam rentang tersebut.
    return true;
}

void printHeader(const char *title)
{
    // COLOR_BLUE dan COLOR_RESET adalah makro yang diasumsikan untuk mengatur warna teks di konsol (misalnya, menggunakan kode escape ANSI).
    printf(COLOR_BLUE);                                   // Memulai pencetakan dengan warna biru.
    printf("╔══════════════════════════════════════╗\n"); // Mencetak garis batas atas header.
    // Mencetak judul. `%-36s` berarti string akan dicetak rata kiri (`-`) dalam field selebar 36 karakter.
    // Jika judul lebih pendek, akan diisi spasi di kanan. Jika lebih panjang, field akan melebar.
    printf("║ %-36s ║\n", title);
    printf("╚══════════════════════════════════════╝\n" COLOR_RESET); // Mencetak garis batas bawah header dan mereset warna teks ke default.
}

void printError(const char *message)
{
    // COLOR_RED dan COLOR_RESET untuk warna merah.
    printf(COLOR_RED "ERROR: %s\n" COLOR_RESET, message); // Mencetak "ERROR: " diikuti pesan, lalu reset warna.
}

void printSuccess(const char *message)
{
    // COLOR_GREEN dan COLOR_RESET untuk warna hijau.
    printf(COLOR_GREEN "SUKSES: %s\n" COLOR_RESET, message); // Mencetak "SUKSES: " diikuti pesan, lalu reset warna.
}

void printTableBorder(int *widths, int n, int type)
{
    // Pointer ke string karakter yang akan digunakan untuk menggambar border.
    // Karakter-karakter ini adalah bagian dari karakter box-drawing Unicode.
    const char *left, *mid, *sep, *right;

    // Menentukan set karakter yang akan digunakan berdasarkan `type` border.
    switch (type)
    {
    case 1: // Tipe 1: Border atas tabel.
        left = "┌";
        mid = "─";
        sep = "┬";
        right = "┐";
        break;
    case 2: // Tipe 2: Border tengah (pemisah antar baris data).
        left = "├";
        mid = "─";
        sep = "┼";
        right = "┤";
        break;
    case 3: // Tipe 3: Border bawah tabel.
        left = "└";
        mid = "─";
        sep = "┴";
        right = "┘";
        break;
    default: // Default ke tipe 1 (border atas) jika `type` tidak dikenal.
        left = "┌";
        mid = "─";
        sep = "┬";
        right = "┐";
    }

    printf("%s", left); // Cetak karakter awal border kiri (misalnya, "┌").
    // Loop untuk setiap kolom (dari 0 hingga n-1).
    for (int i = 0; i < n; i++)
    {
        // Cetak karakter garis horizontal (`mid`, misalnya "─") sebanyak `widths[i] + 2`.
        // `widths[i]` adalah lebar isi kolom. `+ 2` adalah untuk padding spasi di kiri dan kanan isi sel.
        for (int j = 0; j < widths[i] + 2; j++)
            printf("%s", mid);
        // Cetak pemisah kolom (`sep`, misalnya "┬" atau "┼") jika ini bukan kolom terakhir.
        // Jika ini adalah kolom terakhir, cetak karakter akhir border kanan (`right`, misalnya "┐" atau "┤").
        printf("%s", (i < n - 1 ? sep : right));
    }
    printf("\n"); // Pindah ke baris baru setelah selesai mencetak satu garis border.
}

void printTableRow(const char **row, int *widths, int n)
{
    printf("│"); // Cetak karakter batas vertikal kiri baris.
    // Loop untuk setiap kolom.
    for (int i = 0; i < n; i++)
    {
        // Cetak isi sel. `%-*s` adalah format specifier untuk printf:
        //   `-`: rata kiri (left-justify).
        //   `*`: lebar field akan diambil dari argumen berikutnya (yaitu, `widths[i]`).
        //   `s`: tipe data adalah string (yaitu, `row[i]`).
        // Ditambah spasi di awal (" ") untuk padding kiri, dan " │" di akhir untuk padding kanan dan pemisah vertikal.
        printf(" %-*s │", widths[i], row[i]);
    }
    printf("\n"); // Pindah ke baris baru setelah semua sel dalam baris tercetak.
}
