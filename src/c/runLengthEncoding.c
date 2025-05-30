#include "runLengthEncoding.h"

boolean generateRleCode(const char *username, char *output, int maxLen)
{
    // Memeriksa apakah username NULL atau string kosong.
    if (username == NULL || username[0] == '\0')
    {
        // Mencetak pesan error jika username tidak valid.
        printError("Username kosong atau tidak valid untuk kode RLE!");
        // Mengembalikan false karena input tidak valid.
        return false;
    }
    // Memeriksa apakah panjang buffer maksimum valid (harus lebih besar dari 0).
    if (maxLen <= 0)
    {
        // Mencetak pesan error jika panjang buffer tidak valid.
        printError("Panjang buffer maksimum tidak valid!");
        // Mengembalikan false.
        return false;
    }
    // Memeriksa validitas format username menggunakan fungsi isValidUsername.
    if (!isValidUsername(username))
    {
        // Mencetak pesan error jika username tidak valid.
        printError("Username tidak valid untuk kode RLE!");
        // Mengembalikan false.
        return false;
    }

    // Mendapatkan panjang username.
    int usernameLen = strlen(username);
    // Indeks untuk buffer output.
    int outIndex = 0;
    // Indeks untuk iterasi melalui username.
    int i = 0;

    // Melakukan iterasi melalui username untuk menghasilkan kode RLE.
    while (i < usernameLen)
    {
        // Mengambil karakter saat ini.
        char currentChar = username[i];
        // Menghitung jumlah kemunculan karakter berurutan.
        int count = 0;

        // Menghitung jumlah karakter berurutan yang sama.
        while (i < usernameLen && username[i] == currentChar)
        {
            count++;
            i++;
        }

        // Menambahkan angka hitungan ke output JIKA hitungan lebih dari 1.
        // Angka ditambahkan SEBELUM karakter itu sendiri.
        if (count > 1)
        {
            // Buffer sementara untuk menyimpan representasi string dari angka hitungan.
            char countStr[12];     // Cukup untuk angka hingga 10 digit + null terminator.
            int numDigits = 0;     // Jumlah digit dalam angka hitungan.
            int tempCount = count; // Variabel sementara untuk memproses angka hitungan.

            // Kasus khusus jika hitungan adalah 0 (seharusnya tidak terjadi jika count > 1, tapi sebagai pengaman).
            if (tempCount == 0)
            {
                countStr[numDigits++] = '0';
            }
            else
            {
                // Mengkonversi angka hitungan menjadi string digit secara manual.
                // Digit-digit disimpan dalam urutan terbalik.
                while (tempCount > 0)
                {
                    // Pemeriksaan keamanan untuk mencegah overflow buffer countStr.
                    if (numDigits >= 11)
                    {
                        printError("Error: Hitungan RLE terlalu besar untuk buffer sementara.");
                        return false;
                    }
                    countStr[numDigits++] = (tempCount % 10) + '0'; // Ambil digit terakhir.
                    tempCount /= 10;                                // Hapus digit terakhir.
                }
            }

            // Membalik urutan digit agar benar (misalnya, dari "321" menjadi "123").
            for (int j = 0; j < numDigits / 2; j++)
            {
                char temp = countStr[j];
                countStr[j] = countStr[numDigits - 1 - j];
                countStr[numDigits - 1 - j] = temp;
            }
            countStr[numDigits] = '\0'; // Menambahkan null terminator pada string angka.

            // Memeriksa apakah buffer output cukup untuk menampung angka hitungan.
            // Perlu ruang untuk numDigits + 1 (untuk karakter berikutnya) + 1 (untuk null terminator di akhir).
            // outIndex sudah menunjuk ke posisi awal data baru.
            if (outIndex + numDigits + 1 >= maxLen) // +1 untuk karakter, null terminator diperiksa di akhir.
            {
                printError("Buffer kode RLE tidak cukup untuk menyimpan hasil (angka).");
                return false;
            }

            // Menyalin string angka ke buffer output secara manual.
            for (int k = 0; k < numDigits; ++k)
            {
                output[outIndex++] = countStr[k];
            }
            // outIndex sudah diincrement di dalam loop.
        }

        // Menambahkan karakter saat ini ke buffer output SETELAH angka (jika ada).
        // Memeriksa sisa ruang untuk karakter saat ini dan null terminator di akhir.
        if (outIndex + 1 >= maxLen) // +1 untuk karakter itu sendiri.
        {
            printError("Buffer kode RLE tidak cukup untuk menyimpan hasil (karakter).");
            return false;
        }
        output[outIndex++] = currentChar; // Menambahkan karakter ke output.
    }

    // Mengakhiri string output dengan null terminator.
    output[outIndex] = '\0';

    // Mencetak pesan sukses jika kode RLE berhasil dibuat.
    printSuccess("Kode RLE berhasil dihasilkan!");
    // Mengembalikan true menandakan keberhasilan.
    return true;
}