#include "runLengthEncoding.h"

boolean generateRleCode(const char *username, char *output, int maxLen)
{
    if (username == NULL || username[0] == '\0')
    {
        printError("Username kosong atau tidak valid untuk kode RLE!");
        return false;
    }
    if (maxLen <= 0)
    {
        printError("Panjang buffer maksimum tidak valid!");
        return false;
    }
    if (!isValidUsername(username))
    {
        printError("Username tidak valid untuk kode RLE!");
        return false;
    }

    int usernameLen = strlen(username);
    int outIndex = 0;
    int i = 0;

    while (i < usernameLen)
    {
        // Cek sisa ruang untuk karakter saat ini
        if (outIndex >= maxLen - 1)
        {
            printError("Buffer kode RLE tidak cukup untuk menyimpan hasil!");
            return false;
        }

        char currentChar = username[i];
        int count = 0;

        // Hitung jumlah karakter berurutan
        while (i < usernameLen && username[i] == currentChar)
        {
            count++;
            i++;
        }

        // Tambahkan karakter ke output
        output[outIndex++] = currentChar;

        // Tambahkan angka hanya jika count > 1
        if (count > 1)
        {
            // Konversi count ke string secara manual
            char countStr[12]; // Buffer untuk menampung angka
            int numDigits = 0;
            int tempCount = count;

            // Ambil digit secara terbalik (misal 123 -> '3', '2', '1')
            while (tempCount > 0)
            {
                countStr[numDigits++] = (tempCount % 10) + '0';
                tempCount /= 10;
            }

            // Balik urutan digit agar sesuai (misal '3', '2', '1' -> '1', '2', '3')
            for (int j = 0; j < numDigits / 2; j++)
            {
                char temp = countStr[j];
                countStr[j] = countStr[numDigits - 1 - j];
                countStr[numDigits - 1 - j] = temp;
            }
            countStr[numDigits] = '\0'; // Tambahkan null terminator

            // Cek apakah buffer cukup untuk angka
            if (outIndex + numDigits >= maxLen)
            {
                printError("Buffer kode RLE tidak cukup untuk menyimpan hasil!");
                return false;
            }

            // Salin angka ke output
            strcpy(output + outIndex, countStr);
            outIndex += numDigits;
        }
    }

    output[outIndex] = '\0'; // Akhiri string

    printSuccess("Kode RLE berhasil dihasilkan!");
    return true;
}