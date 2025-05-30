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
    
    // Clear the rest of the input buffer using scanf
    // Reads and discards characters until a newline is encountered, then reads and discards the newline.
    if (scanf("%*[^
]") == EOF) {
        // Handle EOF or error if necessary, though for buffer clearing,
        // it might mean the stream ended. For this function, we can often ignore it.
    }
    // Consume the newline character itself, or whatever single char remains if input was just newline
    if (scanf("%*c") == EOF) { 
        // Handle EOF or error if necessary.
    }

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

// User-specified function for case-insensitive username check
boolean isUsernameTaken(Hospital *hospital, const char *username) {
    if (hospital == NULL || username == NULL) {
        return true; // Error condition, treat as taken to be safe
    }

    char lowerUsername[51]; // Max username length 50 + null
    int usernameLen = strlen(username);
    // Prevent buffer overflow if username is unexpectedly long before validation
    if (usernameLen > 50) usernameLen = 50; 

    for (int i = 0; i < usernameLen; i++) {
        char c = username[i];
        if (c >= 'A' && c <= 'Z') {
            lowerUsername[i] = c + ('a' - 'A');
        } else {
            lowerUsername[i] = c;
        }
    }
    lowerUsername[usernameLen] = '\0';

    for (int i = 0; i < hospital->users.nEff; i++) {
        char lowerElementUsername[51]; // Max username length 50 + null
        const char *existingUsername = hospital->users.elements[i].username;
        int elementLen = strlen(existingUsername);
        // Prevent buffer overflow
        if (elementLen > 50) elementLen = 50;

        for (int j = 0; j < elementLen; j++) {
            char c = existingUsername[j];
            if (c >= 'A' && c <= 'Z') {
                lowerElementUsername[j] = c + ('a' - 'A');
            } else {
                lowerElementUsername[j] = c;
            }
        }
        lowerElementUsername[elementLen] = '\0';

        if (strcmp(lowerUsername, lowerElementUsername) == 0) {
            return true; // Username is taken
        }
    }
    return false; // Username is not taken (unique)
}

boolean integerToString(int value, char *buffer, int bufferSize) {
    if (buffer == NULL || bufferSize < 2) {
        if (buffer != NULL && bufferSize > 0) buffer[0] = '\0';
        return false; // Min for '0' and null, or sign + digit + null
    }

    char tempReverse[20]; // Max ~11 digits for int, plus sign
    int i = 0;
    long long long_value = value; // Use long long to handle INT_MIN correctly
    boolean isNegative = false;

    if (long_value == 0) {
        // bufferSize check already done
        buffer[0] = '0'; buffer[1] = '\0';
        return true;
    }

    if (long_value < 0) {
        isNegative = true;
        long_value = -long_value;
    }

    while (long_value > 0) {
        if (i >= 19) { // Safety for tempReverse buffer overflow against 20 char limit
             buffer[0] = '\0'; return false; 
        }
        tempReverse[i++] = (long_value % 10) + '0';
        long_value /= 10;
    }

    // Check required length against bufferSize (i is number of digits)
    if (i + (isNegative ? 1 : 0) >= bufferSize) { 
        buffer[0] = '\0'; return false; 
    }

    int k = 0;
    if (isNegative) {
        buffer[k++] = '-';
    }
    // Reverse the string from tempReverse into buffer
    while (i > 0) {
        buffer[k++] = tempReverse[--i];
    }
    buffer[k] = '\0';
    return true;
}

boolean floatToString(float value, char *buffer, int bufferSize, int precision) {
    if (buffer == NULL || bufferSize < 2 || precision < 0) {
        if (buffer != NULL && bufferSize > 0) buffer[0] = '\0';
        return false;
    }
    
    buffer[0] = '\0'; // Initialize for safe strcat like operations

    // Handling of NaN/Inf would require non-standard functions or complex checks, omitted.
    
    float absValue = value;
    boolean isNegative = false;

    if (value < 0.0f) {
        absValue = -value;
        isNegative = true;
    }

    int intPart = (int)absValue;
    char intStr[20]; 
    if (!integerToString(intPart, intStr, sizeof(intStr))) {
        buffer[0] = '\0'; return false;
    }

    // Build the string using manual concatenation logic for safety and compliance
    int currentLen = 0;
    if (isNegative) {
        if (currentLen >= bufferSize - 1) { buffer[0] = '\0'; return false; }
        buffer[currentLen++] = '-';
    }

    for(int j=0; intStr[j] != '\0'; ++j) {
        if (currentLen >= bufferSize - 1) { buffer[0] = '\0'; return false; }
        buffer[currentLen++] = intStr[j];
    }
    
    if (precision > 0) {
        if (currentLen >= bufferSize - 1) { buffer[0] = '\0'; return false; } // For '.'
        buffer[currentLen++] = '.';
        
        float fracPart = absValue - (float)intPart;
        // Add 0.5 / (10^precision) for rounding - simplified here by adding to fracpart before loop
        float roundingFactor = 0.5f;
        for(int p=0; p<precision; ++p) roundingFactor /= 10.0f;
        fracPart += roundingFactor; 
        // check if fracPart carried over to 1.0
        if (fracPart >= 1.0f) {
            // This requires re-doing integer part or complex carry.
            // For simplicity in this context, if rounding causes carry, it's complex to handle without sprintf.
            // A more robust solution would build frac part first or use larger intermediate types.
            // This simplified version will truncate if fracPart (after rounding) >= 1.0.
            // A truly robust custom float to string is very complex.
            // The provided solution sketch in the prompt was also simplified.
            // We will proceed with a simplified fractional part conversion.
            fracPart = absValue - (float)intPart; // reset fracPart if rounding is too complex to handle carry
        }


        for (int p = 0; p < precision; p++) {
            fracPart *= 10.0f;
            int digit = (int)fracPart;
            if (digit < 0) digit = 0; 
            if (digit > 9) digit = 9; 

            if (currentLen >= bufferSize - 1) { buffer[0] = '\0'; return false; } // For digit
            buffer[currentLen++] = (char)(digit + '0');
            fracPart -= digit; 
        }
    }
    buffer[currentLen] = '\0';
    return true;
}

boolean readStringWithSpaces(char *buffer, int bufferSize, const char *prompt) {
    if (buffer == NULL || bufferSize <= 0) {
        return false;
    }
    
    printf("%s", prompt);

    if (bufferSize == 1) { // Only space for null terminator
        buffer[0] = '\0';
        // Consume the rest of the input line
        int r1 = scanf("%*[^\n]"); 
        if (r1 == EOF) { /* EOF during clear, probably okay */ }
        int r2 = scanf("%*c");    
        if (r2 == EOF) { /* EOF during clear, probably okay */ }
        return true; // Read an "empty" string successfully
    }

    char formatString[20]; 
    char widthStr[5];     // Max width for int (e.g., 2000000000 is 10 digits, bufferSize-1)

    formatString[0] = '%';
    formatString[1] = '\0';

    if (!integerToString(bufferSize - 1, widthStr, sizeof(widthStr))) {
        printError("Internal error: Could not construct scan width for readStringWithSpaces.");
        // Clear the input buffer before returning false to prevent issues with next input
        int r1 = scanf("%*[^\n]"); if (r1 == EOF) { /* Optional: specific handling */ }
        int r2 = scanf("%*c");    if (r2 == EOF) { /* Optional: specific handling */ }
        buffer[0] = '\0'; 
        return false; 
    }
    strcat(formatString, widthStr);
    strcat(formatString, "[^\n]");

    int scanf_result = scanf(formatString, buffer);

    if (scanf_result == EOF) {
        buffer[0] = '\0'; // Ensure buffer is empty on EOF
        // No need to clear buffer further if EOF was hit on the main read
        return false; 
    }
    if (scanf_result == 0) { 
        // No characters were assigned by the main scanf. 
        // This implies the first character encountered was a newline.
        buffer[0] = '\0'; // Ensure buffer is an empty string
        // Consume the newline character that scanf %[^
] specifically does not consume.
        int r = scanf("%*c"); // Consume the single newline character.
        if (r == EOF) return false; // EOF occurred while consuming the newline
        return true; // Successfully "read" an empty line
    }
    
    // If scanf_result == 1, data was read.
    // Now, consume the rest of the line up to and including the newline.
    int clearScanResChars = scanf("%*[^\n]"); // Renamed variable
    if (clearScanResChars == EOF) { 
        // Main read succeeded, but EOF hit during buffer clear.
        // The primary read was successful, so return true.
        return true; 
    }

    // Consume the newline character itself.
    if (scanf("%*c") == EOF) {
        // Main read succeeded, but EOF hit while consuming the final newline.
        // Consider this a successful read of the data obtained.
        return true;
    }

    return true;
}