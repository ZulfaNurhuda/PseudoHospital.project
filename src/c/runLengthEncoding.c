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

        while (i < usernameLen && username[i] == currentChar)
        {
            count++;
            i++;
        }

        if (count > 1)
        {

            char countStr[12];
            int numDigits = 0;
            int tempCount = count;

            if (tempCount == 0)
            {
                countStr[numDigits++] = '0';
            }
            else
            {

                while (tempCount > 0)
                {

                    if (numDigits >= 11)
                    {
                        printError("Error: Hitungan RLE terlalu besar untuk buffer sementara.");
                        return false;
                    }
                    countStr[numDigits++] = (tempCount % 10) + '0';
                    tempCount /= 10;
                }
            }

            for (int j = 0; j < numDigits / 2; j++)
            {
                char temp = countStr[j];
                countStr[j] = countStr[numDigits - 1 - j];
                countStr[numDigits - 1 - j] = temp;
            }
            countStr[numDigits] = '\0';

            if (outIndex + numDigits + 1 >= maxLen)
            {
                printError("Buffer kode RLE tidak cukup untuk menyimpan hasil (angka).");
                return false;
            }

            for (int k = 0; k < numDigits; ++k)
            {
                output[outIndex++] = countStr[k];
            }
        }

        if (outIndex + 1 >= maxLen)
        {
            printError("Buffer kode RLE tidak cukup untuk menyimpan hasil (karakter).");
            return false;
        }
        output[outIndex++] = currentChar;
    }

    output[outIndex] = '\0';

    printSuccess("Kode RLE berhasil dihasilkan!");

    return true;
}