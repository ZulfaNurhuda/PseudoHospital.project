#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "boolean.h"
#include "abstractDataTypes.h"

void *safeMalloc(size_t size)
{
    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        printf(COLOR_RED "Alokasi memori gagal! Program dihentikan.\n" COLOR_RESET);
        exit(1);
    }
    return ptr;
}

boolean readValidString(char *buffer, int maxLen, const char *prompt, boolean alphanumeric)
{
    printf("%s", prompt);
    char temp[256];       // Buffer sementara
    scanf("%255s", temp); // Baca string tanpa spasi
    while (getchar() != '\n')
        ; // Bersihkan buffer

    if (strlen(temp) >= maxLen)
    {
        printError("Input terlalu panjang!");
        return false;
    }
    if (strlen(temp) == 0)
    {
        printError("Input tidak boleh kosong!");
        return false;
    }
    if (alphanumeric)
    {
        for (int i = 0; temp[i]; i++)
        {
            char c = temp[i];
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'))
            {
                printError("Input hanya boleh berisi huruf, angka, atau underscore!");
                return false;
            }
        }
    }
    strcpy(buffer, temp); // Salin ke buffer
    return true;
}

boolean readValidInt(int *value, const char *prompt)
{
    char buffer[32];
    if (!readValidString(buffer, 32, prompt, false))
        return false;

    int result = 0;
    int i = 0;
    boolean isNegative = false;

    if (buffer[0] == '-')
    {
        isNegative = true;
        i++;
    }

    while (buffer[i])
    {
        if (buffer[i] < '0' || buffer[i] > '9')
        {
            printError("Input harus berupa angka!");
            return false;
        }
        result = result * 10 + (buffer[i] - '0');
        i++;
    }

    *value = isNegative ? -result : result;
    return true;
}

boolean readValidFloat(float *value, const char *prompt)
{
    char buffer[32];
    if (!readValidString(buffer, 32, prompt, false))
        return false;

    float result = 0.0f;
    float fraction = 0.1f;
    boolean isNegative = false;
    boolean hasDecimal = false;
    int i = 0;

    if (buffer[0] == '-')
    {
        isNegative = true;
        i++;
    }

    while (buffer[i])
    {
        if (buffer[i] == '.')
        {
            if (hasDecimal)
            {
                printError("Input hanya boleh memiliki satu titik desimal!");
                return false;
            }
            hasDecimal = true;
            i++;
            continue;
        }
        if (buffer[i] < '0' || buffer[i] > '9')
        {
            printError("Input harus berupa angka desimal!");
            return false;
        }
        if (hasDecimal)
        {
            result += (buffer[i] - '0') * fraction;
            fraction *= 0.1f;
        }
        else
        {
            result = result * 10 + (buffer[i] - '0');
        }
        i++;
    }

    *value = isNegative ? -result : result;
    return true;
}

boolean isValidUsername(const char *username)
{
    if (strlen(username) == 0 || strlen(username) > 50)
        return false;
    for (int i = 0; username[i]; i++)
    {
        char c = username[i];
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_'))
            return false;
    }
    return true;
}

boolean isValidRoomCode(Hospital *hospital, const char *roomCode)
{
    if (strlen(roomCode) < 2 || strlen(roomCode) > 4)
        return false;

    char row = roomCode[0];
    char colStr[4];
    strcpy(colStr, roomCode + 1);

    // Validasi baris (huruf A-Z)
    if (!(row >= 'A' && row <= 'Z'))
        return false;

    // Validasi kolom (angka)
    for (int i = 0; colStr[i]; i++)
    {
        if (!(colStr[i] >= '0' && colStr[i] <= '9'))
            return false;
    }

    // Konversi kolom ke integer
    int col = 0;
    for (int i = 0; colStr[i]; i++)
    {
        col = col * 10 + (colStr[i] - '0');
    }

    // Validasi sesuai ukuran denah rumah sakit
    if (row - 'A' >= hospital->layout.rowEff || col > hospital->layout.colEff || col <= 0)
        return false;

    return true;
}

void printHeader(const char *title)
{
    printf(COLOR_BLUE);
    printf("╔══════════════════════════════════════╗\n");
    printf("║ %-36s ║\n", title);
    printf("╚══════════════════════════════════════╝\n" COLOR_RESET);
}

void printError(const char *message)
{
    printf(COLOR_RED "ERROR: %s\n" COLOR_RESET, message);
}

void printSuccess(const char *message)
{
    printf(COLOR_GREEN "SUKSES: %s\n" COLOR_RESET, message);
}

void printTableBorder(int *widths, int n, int type)
{
    const char *left, *mid, *sep, *right;

    switch (type)
    {
    case 1:
        left = "┌";
        mid = "─";
        sep = "┬";
        right = "┐";
        break;
    case 2:
        left = "├";
        mid = "─";
        sep = "┼";
        right = "┤";
        break;
    case 3:
        left = "└";
        mid = "─";
        sep = "┴";
        right = "┘";
        break;
    default:
        left = "┌";
        mid = "─";
        sep = "┬";
        right = "┐";
    }

    printf("%s", left);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < widths[i] + 2; j++)
            printf("%s", mid);
        printf("%s", (i < n - 1 ? sep : right));
    }
    printf("\n");
}

void printTableRow(const char **row, int *widths, int n)
{
    printf("│");
    for (int i = 0; i < n; i++)
    {
        printf(" %-*s │", widths[i], row[i]);
    }
    printf("\n");
}

int stringToInt(char *str)
{
    int result = 0;
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return -1; // deteksi karakter non-digit
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}