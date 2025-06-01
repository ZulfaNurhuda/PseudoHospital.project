#include "enigma.h"

static const char *rotor = "BDFHJLCPRTXVZNYEIWGAKMUSQO";

static const char *reflector = "ZYXWVUTSRQPONMLKJIHGFEDCBA";

static const char plugboard[5][2] = {{'A', 'B'}, {'C', 'D'}, {'E', 'F'}, {'G', 'H'}, {'I', 'J'}};

static const int rotorPosition = 3;

static const int caesarShift = 3;

static int calculateDynamicShift(const char *input)
{
    int sum = 0;

    for (int i = 0; input[i] != '\0'; i++)
    {
        sum += (int)input[i];
    }

    return sum % 10;
}

static void caesarEncryptConfig(char *config, int shift)
{
    int len = strlen(config);

    for (int i = 0; i < len; i++)
    {

        if (('A' <= config[i] && config[i] <= 'Z') || ('a' <= config[i] && config[i] <= 'z'))
        {

            char base = ('A' <= config[i] && config[i] <= 'Z') ? 'A' : 'a';

            config[i] = base + ((config[i] - base + shift) % 26);
        }
    }
}

static void caesarDecryptConfig(char *config, int shift)
{
    int len = strlen(config);

    for (int i = 0; i < len; i++)
    {

        if (('A' <= config[i] && config[i] <= 'Z') || ('a' <= config[i] && config[i] <= 'z'))
        {
            char base = ('A' <= config[i] && config[i] <= 'Z') ? 'A' : 'a';

            config[i] = base + ((config[i] - base - shift + 26) % 26);
        }
    }
}

static char encryptNumber(char c, int shift)
{

    if ('0' <= c && c <= '9')
    {

        return '0' + ((c - '0' + shift) % 10);
    }

    return c;
}

static char decryptNumber(char c, int shift)
{

    if ('0' <= c && c <= '9')
    {

        return '0' + ((c - '0' - shift + 10) % 10);
    }

    return c;
}

static char processPlugboard(char c)
{

    for (int i = 0; i < 5; i++)
    {

        if (c == plugboard[i][0])
            return plugboard[i][1];

        if (c == plugboard[i][1])
            return plugboard[i][0];
    }

    return c;
}

boolean enigmaEncrypt(const char *plaintext, char *ciphertext, int maxLength)
{

    if (!plaintext || !ciphertext || maxLength <= 0)
    {
        printError("Parameter enkripsi tidak valid!");
        return false;
    }

    int len = strlen(plaintext);

    if (len >= maxLength)
    {
        printError("Panjang input melebihi kapasitas output!");
        return false;
    }

    for (int i = 0; i < len; i++)
    {
        char c = plaintext[i];

        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')))
        {
            printError("Input mengandung karakter tidak valid! Hanya huruf A-Z dan angka 0-9 diperbolehkan.");
            return false;
        }
    }

    int numberShift = calculateDynamicShift(plaintext);

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

    for (int i = 0; i < len; i++)
    {
        char c = plaintext[i];

        if ('0' <= c && c <= '9')
        {

            ciphertext[i] = encryptNumber(c, numberShift);
            continue;
        }

        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A';

        c = processPlugboard(c);

        int pos = (c - 'A' + rotorPosition) % 26;
        c = rotor[pos];

        c = reflector[c - 'A'];

        for (int j = 0; j < 26; j++)
        {
            if (rotor[j] == c)
            {

                c = 'A' + ((j - rotorPosition + 26) % 26);
                break;
            }
        }

        c = processPlugboard(c);

        ciphertext[i] = c;
    }
    ciphertext[len] = '\0';

    printSuccess("Teks berhasil dienkripsi!");
    return true;
}

boolean enigmaDecrypt(const char *ciphertext, char *plaintext, int maxLength)
{

    if (!ciphertext || !plaintext || maxLength <= 0)
    {
        printError("Parameter dekripsi tidak valid!");
        return false;
    }

    int len = strlen(ciphertext);

    if (len >= maxLength)
    {
        printError("Panjang input melebihi kapasitas output!");
        return false;
    }

    for (int i = 0; i < len; i++)
    {
        char c = ciphertext[i];
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')))
        {
            printError("Input mengandung karakter tidak valid! Hanya huruf A-Z dan angka 0-9 diperbolehkan.");
            return false;
        }
    }

    int numberShift = calculateDynamicShift(ciphertext);

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

    for (int i = 0; i < len; i++)
    {
        char c = ciphertext[i];

        if ('0' <= c && c <= '9')
        {

            plaintext[i] = decryptNumber(c, numberShift);
            continue;
        }

        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A';

        c = processPlugboard(c);

        int pos = (c - 'A' + rotorPosition) % 26;

        for (int j = 0; j < 26; j++)
        {

            if (rotor[j] == 'A' + pos)
            {
                c = 'A' + j;
                break;
            }
        }

        c = reflector[c - 'A'];

        pos = (c - 'A' - rotorPosition + 26) % 26;
        c = rotor[pos];

        c = processPlugboard(c);

        plaintext[i] = c;
    }
    plaintext[len] = '\0';

    printSuccess("Teks berhasil didekripsi!");
    return true;
}
