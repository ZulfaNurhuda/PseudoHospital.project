#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "enigma.h"
#include "utils.h"

// Simulasi konfigurasi Enigma
static const char *rotor = "BDFHJLCPRTXVZNYEIWGAKMUSQO";                                          // Rotor sederhana (substitusi 26 huruf)
static const char *reflector = "ZYXWVUTSRQPONMLKJIHGFEDCBA";                                      // Reflektor (A↔Z, B↔Y, dst.)
static const char plugboard[5][2] = {{'A', 'B'}, {'C', 'D'}, {'E', 'F'}, {'G', 'H'}, {'I', 'J'}}; // 5 pasangan
static const int rotorPosition = 3;                                                               // Shift rotor
static const int caesarShift = 3;                                                                 // Shift untuk konfigurasi

// Fungsi untuk menghitung shift dinamis berdasarkan penjumlahan karakter
static int calculateDynamicShift(const char *input)
{
    int sum = 0;
    for (int i = 0; input[i] != '\0'; i++)
    {
        sum += (int)input[i];
    }
    return sum % 10; // Modulo 10 agar shift berada di rentang 0-9
}

// Fungsi untuk mengenkripsi konfigurasi dengan Caesar Cipher
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

// Fungsi untuk mendekripsi konfigurasi dengan Caesar Cipher
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

// Fungsi untuk mengenkripsi angka dengan Caesar Cipher menggunakan shift dinamis
static char encryptNumber(char c, int shift)
{
    if ('0' <= c && c <= '9')
    {
        return '0' + ((c - '0' + shift) % 10);
    }
    return c;
}

// Fungsi untuk mendekripsi angka dengan Caesar Cipher menggunakan shift dinamis
static char decryptNumber(char c, int shift)
{
    if ('0' <= c && c <= '9')
    {
        return '0' + ((c - '0' - shift + 10) % 10);
    }
    return c;
}

// Fungsi untuk memproses karakter melalui plugboard
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

    // Validasi karakter: hanya huruf A-Z (case-insensitive) dan angka 0-9
    for (int i = 0; i < len; i++)
    {
        char c = plaintext[i];
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')))
        {
            printError("Input mengandung karakter tidak valid! Hanya huruf A-Z dan angka 0-9 diperbolehkan.");
            return false;
        }
    }

    // Hitung shift dinamis untuk angka
    int numberShift = calculateDynamicShift(plaintext);

    // Simpan konfigurasi sebagai string
    char config[100] = "";
    strcat(config, rotor);
    strcat(config, "|");
    char posStr[10] = "";
    int temp = rotorPosition;
    int index = 0;
    while (temp > 0)
    {
        posStr[index++] = (temp % 10) + '0';
        temp /= 10;
    }
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

    // Proses enkripsi
    for (int i = 0; i < len; i++)
    {
        char c = plaintext[i];

        // Jika angka, gunakan Caesar Cipher dengan shift dinamis
        if ('0' <= c && c <= '9')
        {
            ciphertext[i] = encryptNumber(c, numberShift);
            continue;
        }

        // Normalisasi ke huruf besar untuk huruf
        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A';

        // 1. Plugboard
        c = processPlugboard(c);

        // 2. Rotor (maju)
        int pos = (c - 'A' + rotorPosition) % 26;
        c = rotor[pos];

        // 3. Reflektor
        c = reflector[c - 'A'];

        // 4. Rotor (balik)
        for (int j = 0; j < 26; j++)
        {
            if (rotor[j] == c)
            {
                c = 'A' + ((j - rotorPosition + 26) % 26);
                break;
            }
        }

        // 5. Plugboard lagi
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

    // Validasi karakter: hanya huruf A-Z (case-insensitive) dan angka 0-9
    for (int i = 0; i < len; i++)
    {
        char c = ciphertext[i];
        if (!(('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || ('0' <= c && c <= '9')))
        {
            printError("Input mengandung karakter tidak valid! Hanya huruf A-Z dan angka 0-9 diperbolehkan.");
            return false;
        }
    }

    // Hitung shift dinamis untuk angka
    int numberShift = calculateDynamicShift(ciphertext);

    // Simpan konfigurasi sebagai string (sama seperti enkripsi)
    char config[100] = "";
    strcat(config, rotor);
    strcat(config, "|");
    char posStr[10] = "";
    int temp = rotorPosition;
    int index = 0;
    while (temp > 0)
    {
        posStr[index++] = (temp % 10) + '0';
        temp /= 10;
    }
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

    // Proses dekripsi
    for (int i = 0; i < len; i++)
    {
        char c = ciphertext[i];

        // Jika angka, gunakan dekripsi Caesar Cipher dengan shift dinamis
        if ('0' <= c && c <= '9')
        {
            plaintext[i] = decryptNumber(c, numberShift);
            continue;
        }

        // Normalisasi ke huruf besar untuk huruf
        if ('a' <= c && c <= 'z')
            c = c - 'a' + 'A';

        // 1. Plugboard
        c = processPlugboard(c);

        // 2. Rotor (balik)
        int pos = (c - 'A' + rotorPosition) % 26;
        for (int j = 0; j < 26; j++)
        {
            if (rotor[j] == 'A' + pos)
            {
                c = 'A' + j;
                break;
            }
        }

        // 3. Reflektor
        c = reflector[c - 'A'];

        // 4. Rotor (maju)
        pos = (c - 'A' - rotorPosition + 26) % 26;
        c = rotor[pos];

        // 5. Plugboard lagi
        c = processPlugboard(c);

        plaintext[i] = c;
    }
    plaintext[len] = '\0';

    printSuccess("Teks berhasil didekripsi!");
    return true;
}