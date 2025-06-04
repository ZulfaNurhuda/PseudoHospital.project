#include "utils.h"

void *safeMalloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
    {

        return NULL;
    }

    return ptr;
}

char charToLower(char c)
{

    if (c >= 'A' && c <= 'Z')
    {

        return c + ('a' - 'A');
    }

    return c;
}

int caseInsensitiveStrcmp(const char *s1, const char *s2)
{

    if (s1 == NULL && s2 == NULL)
        return 0;

    if (s1 == NULL)
        return -1;

    if (s2 == NULL)
        return 1;

    int i = 0;

    while (s1[i] != '\0' && s2[i] != '\0')
    {

        char c1Lower = charToLower(s1[i]);
        char c2Lower = charToLower(s2[i]);

        if (c1Lower != c2Lower)
        {

            return c1Lower - c2Lower;
        }
        i++;
    }

    return charToLower(s1[i]) - charToLower(s2[i]);
}

boolean readValidString(char *buffer, int maxLen, const char *prompt, boolean alphanumeric)
{

    if (buffer == NULL || maxLen < 2 || prompt == NULL)
    {
        return false;
    }

    printf("%s", prompt);

    char temp[256];

    if (scanf("%255s", temp) != 1)
    {

        int cClear;

        while ((cClear = getchar()) != '\n' && cClear != EOF)
            ;

        printError("Gagal membaca input.");
        return false;
    }

    int cClearAfter;
    while ((cClearAfter = getchar()) != '\n' && cClearAfter != EOF)
        ;

    size_t len = strlen(temp);

    if (len >= maxLen)
    {
        printError("Input terlalu panjang!");
        return false;
    }

    if (len == 0)
    {
        printError("Input tidak boleh kosong!");
        return false;
    }

    if (alphanumeric)
    {

        for (int i = 0; temp[i] != '\0'; i++)
        {
            char c_char_val = temp[i];

            if (!((c_char_val >= 'A' && c_char_val <= 'Z') || (c_char_val >= 'a' && c_char_val <= 'z') ||
                  (c_char_val >= '0' && c_char_val <= '9') || c_char_val == '_'))
            {
                printError("Input hanya boleh berisi huruf, angka, atau underscore!");
                return false;
            }
        }
    }

    strcpy(buffer, temp);
    return true;
}

boolean readStringWithSpaces(char *buffer, int bufferSize, const char *prompt)
{

    if (buffer == NULL || bufferSize <= 0)
    {
        return false;
    }

    if (bufferSize == 1)
    {
        buffer[0] = '\0';

        int R1ClearSingle;

        while ((R1ClearSingle = getchar()) != '\n' && R1ClearSingle != EOF)
            ;
        return true;
    }

    printf("%s", prompt);

    char rawInput[256];
    rawInput[0] = '\0';

    int scanfRes = scanf("%255[^\n]", rawInput);

    int clearResLoop;

    do
    {
        clearResLoop = scanf("%*[^\n]");
    } while (clearResLoop == 1 && clearResLoop != EOF);

    if (clearResLoop != EOF)
    {
        scanf("%*c");
    }

    if (scanfRes == EOF && rawInput[0] == '\0')
    {
        buffer[0] = '\0';
        return false;
    }

    int i = 0;
    int j = 0;
    boolean inWord = false;

    while (rawInput[i] == ' ')
    {
        i++;
    }

    while (rawInput[i] != '\0' && j < bufferSize - 1)
    {
        if (rawInput[i] == ' ')
        {
            if (inWord)
            {
                buffer[j++] = ' ';
                inWord = false;
            }
        }
        else
        {
            buffer[j++] = rawInput[i];
            inWord = true;
        }
        i++;
    }

    if (j > 0 && buffer[j - 1] == ' ')
    {
        j--;
    }
    buffer[j] = '\0';

    return true;
}

boolean readUsernameWithTrim(char *buffer, int bufferSize, const char *prompt)
{

    return readStringWithSpaces(buffer, bufferSize, prompt);
}

boolean readValidInt(int *value, const char *prompt)
{

    if (value == NULL || prompt == NULL)
    {
        return false;
    }

    char buffer[32];

    if (!readValidString(buffer, sizeof(buffer), prompt, false))
    {
        return false;
    }

    if (buffer[0] == '\0')
    {
        printError("Input tidak boleh kosong!");
        return false;
    }

    long long result = 0;
    int i = 0;
    boolean isNegative = false;

    const long long MAX_LONG_LONG = 9223372036854775807LL;
    const int MAX_INT = 2147483647;

    if (buffer[0] == '-')
    {
        isNegative = true;
        i++;

        if (buffer[i] == '\0')
        {
            printError("Input harus berupa angka!");
            return false;
        }
    }
    else if (buffer[0] == '+')
    {
        i++;

        if (buffer[i] == '\0')
        {
            printError("Input harus berupa angka!");
            return false;
        }
    }

    while (buffer[i] != '\0')
    {

        if (buffer[i] < '0' || buffer[i] > '9')
        {
            printError("Input harus berupa angka!");
            return false;
        }

        if (result > (MAX_LONG_LONG - (buffer[i] - '0')) / 10)
        {
            printError("Angka terlalu besar!");
            return false;
        }

        result = result * 10 + (buffer[i] - '0');
        i++;
    }

    if (isNegative)
    {

        if (result > (long long)MAX_INT + 1)
        {
            printError("Angka terlalu kecil!");
            return false;
        }
        *value = -(int)result;
    }
    else
    {

        if (result > MAX_INT)
        {
            printError("Angka terlalu besar!");
            return false;
        }
        *value = (int)result;
    }

    return true;
}

boolean readValidFloat(float *value, const char *prompt)
{

    if (value == NULL || prompt == NULL)
    {
        return false;
    }

    char buffer[32];

    if (!readValidString(buffer, sizeof(buffer), prompt, false))
    {
        return false;
    }

    if (buffer[0] == '\0')
    {
        printError("Input tidak boleh kosong!");
        return false;
    }

    double result = 0.0;
    double fraction = 0.1;
    boolean isNegative = false;
    boolean hasDecimal = false;
    boolean hasDigits = false;
    int i = 0;

    if (buffer[0] == '-')
    {
        isNegative = true;
        i++;
    }
    else if (buffer[0] == '+')
    {
        i++;
    }

    if (buffer[i] == '\0' && (isNegative || (i > 0 && buffer[0] == '+')))
    {
        printError("Input harus berupa angka desimal!");
        return false;
    }

    if (buffer[i] == '\0' && i == 0 && buffer[0] != '\0' && (buffer[0] < '0' || buffer[0] > '9') && buffer[0] != '.')
    {
        printError("Input harus berupa angka desimal!");
        return false;
    }

    while (buffer[i] != '\0')
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

            if (buffer[i] == '\0' && !hasDigits)
            {
                printError("Input harus berupa angka desimal!");
                return false;
            }
            continue;
        }

        if (buffer[i] < '0' || buffer[i] > '9')
        {
            printError("Input harus berupa angka desimal!");
            return false;
        }

        hasDigits = true;

        if (hasDecimal)
        {
            result += (buffer[i] - '0') * fraction;
            fraction *= 0.1;
        }
        else
        {
            result = result * 10.0 + (buffer[i] - '0');
        }
        i++;
    }

    if (!hasDigits)
    {
        printError("Input harus berupa angka desimal!");
        return false;
    }

    if (isNegative)
    {
        result = -result;
    }

    if (result > 3.4e38 || result < -3.4e38)
    {
        printError("Angka terlalu besar atau terlalu kecil untuk float!");
        return false;
    }

    *value = (float)result;
    return true;
}

int stringToInt(const char *str)
{
    int result = 0;
    int i = 0;

    while (str[i] != '\0')
    {

        if (str[i] < '0' || str[i] > '9')
            return -1;

        result = result * 10 + (str[i] - '0');
        i++;
    }
    return result;
}

boolean integerToString(int value, char *buffer, int bufferSize)
{

    if (buffer == NULL || bufferSize < 2)
    {

        if (buffer != NULL && bufferSize > 0)
            buffer[0] = '\0';
        return false;
    }

    char tempReverse[12];

    int i = 0;
    long long longValue = value;

    boolean isNegative = false;

    if (longValue == 0)
    {
        if (bufferSize < 2)
        {
            buffer[0] = '\0';
            return false;
        }
        buffer[0] = '0';
        buffer[1] = '\0';
        return true;
    }

    if (longValue < 0)
    {
        isNegative = true;
        longValue = -longValue;
    }

    while (longValue > 0)
    {

        if (i >= 11)
        {
            buffer[0] = '\0';
            return false;
        }
        tempReverse[i++] = (longValue % 10) + '0';
        longValue /= 10;
    }

    int requiredSize = i + (isNegative ? 1 : 0) + 1;
    if (requiredSize > bufferSize)
    {
        buffer[0] = '\0';
        return false;
    }

    int k = 0;

    if (isNegative)
    {
        buffer[k++] = '-';
    }

    while (i > 0)
    {
        buffer[k++] = tempReverse[--i];
    }
    buffer[k] = '\0';

    return true;
}

boolean floatToString(float value, char *buffer, int bufferSize, int precision)
{

    if (buffer == NULL || bufferSize < 2 || precision < 0)
    {
        if (buffer != NULL && bufferSize > 0)
            buffer[0] = '\0';
        return false;
    }

    buffer[0] = '\0';

    if (value != value)
    {
        if (bufferSize >= 4)
        {
            strcpy(buffer, "nan");
            return true;
        }
        return false;
    }

    if (value == 1.0f / 0.0f || value == -1.0f / 0.0f)
    {
        const char *infStr = (value < 0) ? "-inf" : "inf";
        if (bufferSize >= (int)strlen(infStr) + 1)
        {
            strcpy(buffer, infStr);
            return true;
        }
        return false;
    }

    if (value == 0.0f || value == -0.0f)
    {
        int len = 1;
        if (precision > 0)
            len += 1 + precision;

        if (bufferSize < len + 1)
            return false;

        strcpy(buffer, "0");
        if (precision > 0)
        {
            strcat(buffer, ".");
            for (int i = 0; i < precision; i++)
            {
                strcat(buffer, "0");
            }
        }
        return true;
    }

    float absValue = value;
    boolean isNegative = false;
    if (value < 0.0f)
    {
        absValue = -value;
        isNegative = true;
    }

    unsigned long intPart = (unsigned long)absValue;
    char intStr[32];

    if (!integerToString(intPart, intStr, sizeof(intStr)))
    {
        buffer[0] = '\0';
        return false;
    }

    int requiredSize = strlen(intStr);
    if (isNegative)
        requiredSize++;
    if (precision > 0)
        requiredSize += 1 + precision;

    if (bufferSize < requiredSize + 1)
    {
        buffer[0] = '\0';
        return false;
    }

    int currentLen = 0;

    if (isNegative)
    {
        buffer[currentLen++] = '-';
    }

    for (int j = 0; intStr[j] != '\0'; ++j)
    {
        buffer[currentLen++] = intStr[j];
    }

    if (precision > 0)
    {
        buffer[currentLen++] = '.';

        float fracPart = absValue - (float)intPart;

        float roundingFactor = 0.5f;
        for (int p = 0; p < precision; ++p)
        {
            roundingFactor /= 10.0f;
        }
        fracPart += roundingFactor;

        if (fracPart >= 1.0f)
        {
            intPart++;

            fracPart = 0.0f;

            if (!integerToString(intPart, intStr, sizeof(intStr)))
            {
                buffer[0] = '\0';
                return false;
            }

            requiredSize = strlen(intStr);
            if (isNegative)
                requiredSize++;
            if (precision > 0)
                requiredSize += 1 + precision;
            if (bufferSize < requiredSize + 1)
            {
                buffer[0] = '\0';
                return false;
            }

            currentLen = 0;
            if (isNegative)
                buffer[currentLen++] = '-';
            for (int j = 0; intStr[j] != '\0'; ++j)
                buffer[currentLen++] = intStr[j];
            buffer[currentLen++] = '.';
        }

        for (int p = 0; p < precision; p++)
        {
            fracPart *= 10.0f;
            int digit = (int)fracPart;

            if (digit < 0)
                digit = 0;
            if (digit > 9)
                digit = 9;

            buffer[currentLen++] = (char)(digit + '0');
            fracPart -= (float)digit;
        }
    }

    buffer[currentLen] = '\0';
    return true;
}

boolean isValidUsername(const char *username)
{

    if (username == NULL)
    {
        return false;
    }
    int len = 0;

    while (username[len] != '\0')
    {
        len++;
    }

    if (len == 0 || len > 50)
    {
        return false;
    }

    boolean allSpaces = true;

    for (int i = 0; i < len; i++)
    {
        char c = username[i];
        boolean charIsValid = false;

        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        {
            charIsValid = true;
            allSpaces = false;
        }

        else if (c >= '0' && c <= '9')
        {
            charIsValid = true;
            allSpaces = false;
        }

        else if (c == '_')
        {
            charIsValid = true;
            allSpaces = false;
        }

        else if (c == ' ')
        {
            charIsValid = true;
        }

        if (!charIsValid)
        {
            return false;
        }
    }

    if (allSpaces && len > 0)
    {
        return false;
    }

    return true;
}

boolean isUsernameTaken(Hospital *hospital, const char *username)
{

    if (hospital == NULL || username == NULL)
    {
        return true;
    }

    char lowerUsername[51];

    int usernameLen = strlen(username);

    if (usernameLen > 50)
        usernameLen = 50;

    for (int i = 0; i < usernameLen; i++)
    {
        char c = username[i];
        if (c >= 'A' && c <= 'Z')
        {
            lowerUsername[i] = c + ('a' - 'A');
        }
        else
        {
            lowerUsername[i] = c;
        }
    }
    lowerUsername[usernameLen] = '\0';

    for (int i = 0; i < hospital->users.nEff; i++)
    {
        char lowerElementUsername[51];
        const char *existingUsername = hospital->users.elements[i].username;
        int elementLen = strlen(existingUsername);

        if (elementLen > 50)
            elementLen = 50;

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
        lowerElementUsername[elementLen] = '\0';

        if (strcmp(lowerUsername, lowerElementUsername) == 0)
        {
            return true;
        }
    }
    return false;
}

boolean isValidRoomCode(Hospital *hospital, const char *roomCode)
{

    if (strlen(roomCode) < 2 || strlen(roomCode) > 4)
        return false;

    char row = roomCode[0];
    char colStr[4];
    strcpy(colStr, roomCode + 1);

    if (!(row >= 'A' && row <= 'Z'))
        return false;

    for (int i = 0; colStr[i] != '\0'; i++)
    {
        if (!(colStr[i] >= '0' && colStr[i] <= '9'))
            return false;
    }

    int col = 0;
    for (int i = 0; colStr[i] != '\0'; i++)
    {
        col = col * 10 + (colStr[i] - '0');
    }

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
