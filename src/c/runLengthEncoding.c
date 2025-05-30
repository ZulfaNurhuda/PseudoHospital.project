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
        char currentChar = username[i];
        int count = 0;

        // Hitung jumlah karakter berurutan
        while (i < usernameLen && username[i] == currentChar)
        {
            count++;
            i++;
        }

        // Tambahkan angka (jika count > 1) SEBELUM karakter
        if (count > 1)
        {
            // Konversi count ke string secara manual
            char countStr[12]; // Buffer untuk menampung angka (e.g., up to 10 digits for count)
            int numDigits = 0;
            int tempCount = count;

            if (tempCount == 0) { // Should not happen if count > 1, but as safeguard
                countStr[numDigits++] = '0';
            } else {
                while (tempCount > 0)
                {
                    if (numDigits >= 11) { // Safety for countStr buffer
                         printError("Error: Hitungan RLE terlalu besar untuk buffer sementara.");
                         return false;
                    }
                    countStr[numDigits++] = (tempCount % 10) + '0';
                    tempCount /= 10;
                }
            }

            // Balik urutan digit agar sesuai (misal '3', '2', '1' -> '1', '2', '3')
            for (int j = 0; j < numDigits / 2; j++)
            {
                char temp = countStr[j];
                countStr[j] = countStr[numDigits - 1 - j];
                countStr[numDigits - 1 - j] = temp;
            }
            countStr[numDigits] = '\0'; // Tambahkan null terminator

            // Cek apakah buffer output cukup untuk angka
            // outIndex already points to where new data should start
            // We need space for numDigits + 1 (for char) + 1 (for null terminator at the very end)
            if (outIndex + numDigits + 1 >= maxLen) // +1 for char, assuming null term is checked at end
            {
                printError("Buffer kode RLE tidak cukup untuk menyimpan hasil (angka).");
                return false;
            }

            // Salin angka ke output
            // strcpy(output + outIndex, countStr); // strcpy is allowed
            for(int k=0; k < numDigits; ++k) { // Manual strcpy
                output[outIndex++] = countStr[k];
            }
            // outIndex += numDigits; // Already incremented in loop
        }
        
        // Tambahkan karakter ke output SETELAH angka (jika ada)
        // Cek sisa ruang untuk karakter saat ini dan null terminator
        if (outIndex + 1 >= maxLen) // +1 for the character itself, null term check is after loop
        {
            printError("Buffer kode RLE tidak cukup untuk menyimpan hasil (karakter).");
            return false;
        }
        output[outIndex++] = currentChar;
    }

    output[outIndex] = '\0'; // Akhiri string

    printSuccess("Kode RLE berhasil dihasilkan!");
    return true;
}