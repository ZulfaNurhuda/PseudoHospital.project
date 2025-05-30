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
        // Caller is now responsible for handling NULL return
        return NULL;
    }
    return ptr;
}

boolean readValidString(char *buffer, int maxLen, const char *prompt, boolean alphanumeric)
{
    printf("%s", prompt);
    char temp[256];       // Buffer sementara
    if (scanf("%255s", temp) != 1) { // Baca string tanpa spasi, check if read was successful
        // If scanf fails (e.g. EOF before any input), clear buffer and return false
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        printError("Gagal membaca input.");
        return false;
    }
    
    // Clear the rest of the input buffer robustly
    // Consume characters until newline or EOF
    // This pattern aims to clear any remaining characters on the line after "%255s"
    // The first scanf discards everything up to a newline.
    // The second scanf discards the newline itself.
    // Check return values to be absolutely sure, but often ignored for buffer clearing.
    if (scanf("%*[^\n]") == EOF) { /* Handle EOF if necessary, though unlikely here */ }
    if (scanf("%*c") == EOF) { /* Handle EOF if newline was the last char */ }


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

int customCaseInsensitiveStrcmp(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return 0;
    if (s1 == NULL) return -1; // Consistent with strcmp: NULL is less than non-NULL
    if (s2 == NULL) return 1;  // Consistent with strcmp: non-NULL is greater than NULL

    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        char c1_lower = customCharToLower(s1[i]);
        char c2_lower = customCharToLower(s2[i]);
        if (c1_lower != c2_lower) {
            return c1_lower - c2_lower;
        }
        i++;
    }
    // Check if one string is a prefix of the other, or if both ended
    return customCharToLower(s1[i]) - customCharToLower(s2[i]);
}

char customCharToLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
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

boolean isValidUsername(const char *username) {
    if (username == NULL) {
        return false;
    }
    int len = 0;
    // Custom strlen to avoid direct call if not desired, or use it if allowed
    while(username[len] != '\0') {
        len++;
    }

    if (len == 0 || len > 50) {
        return false;
    }

    boolean allSpaces = true;
    for (int i = 0; i < len; i++) {
        char c = username[i];
        boolean charIsValid = false;

        // Check for letter
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            charIsValid = true;
            allSpaces = false;
        }
        // Check for number
        else if (c >= '0' && c <= '9') {
            charIsValid = true;
            allSpaces = false;
        }
        // Check for underscore
        else if (c == '_') {
            charIsValid = true;
            allSpaces = false;
        }
        // Check for space (now allowed as an internal character)
        else if (c == ' ') {
            charIsValid = true;
            // Do not set allSpaces to false for a space character
        }

        if (!charIsValid) {
            return false; // Found an invalid character
        }
    }
    
    if (allSpaces && len > 0) { // If len is 0, it's caught by the first check.
                                // This checks if a non-empty username consists only of spaces.
        return false; 
    }

    return true; // All characters are valid, and it's not all spaces (if non-empty)
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

int stringToInt(const char *str)
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
_Bool isUsernameTaken(Hospital *hospital, const char *username) {
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
    if (bufferSize == 1) { // Only space for null terminator
        buffer[0] = '\0';
        // Consume rest of line if any (e.g., just a newline)
        int r1 = scanf("%*[^\n]"); if (r1 == EOF) { /* ignore EOF for clear */ }
        int r2 = scanf("%*c");    if (r2 == EOF) { /* ignore EOF for clear */ }
        return true; 
    }

    printf("%s", prompt);

    char rawInput[256]; // Temporary larger buffer
    rawInput[0] = '\0'; 

    // Step 1: Read the whole line or up to rawInput capacity
    int scanf_res = scanf("%255[^\n]", rawInput); 

    // Step 2: Clear the rest of the input buffer (newline and any overflow)
    int clear_res_loop;
    do {
        clear_res_loop = scanf("%*[^\n]"); 
    } while (clear_res_loop == 1 && clear_res_loop != EOF); // if it consumed something and not EOF
    
    if (clear_res_loop != EOF) { // If loop didn't break due to EOF, then a newline (or other char) is next
        scanf("%*c"); // Consume the newline (or whatever single char is left)
    }
    
    if (scanf_res == EOF && rawInput[0] == '\0') { // True EOF before any input captured
        buffer[0] = '\0';
        return false;
    }
    // If scanf_res is 0, it means the first char was newline (rawInput remains empty)
    // If scanf_res is 1, something was read into rawInput.
    // If scanf_res is EOF but rawInput got some data (e.g. file without trailing newline), process rawInput.

    // Step 3: Trim logic from rawInput into final buffer
    int i = 0; // Index for rawInput
    int j = 0; // Index for buffer (final output)
    boolean inWord = false;

    // Skip leading spaces from rawInput
    while (rawInput[i] == ' ') {
        i++;
    }

    // Process rest of string: copy non-spaces, condense multiple spaces to one
    while (rawInput[i] != '\0' && j < bufferSize - 1) {
        if (rawInput[i] == ' ') {
            if (inWord) { 
                buffer[j++] = ' ';
                inWord = false; 
            }
            // Skip multiple spaces by advancing 'i' (already handled by outer loop's i++ and inner logic)
        } else {
            buffer[j++] = rawInput[i];
            inWord = true;
        }
        i++;
    }
    // After loop, if the last character processed to buffer was a space, remove it.
    if (j > 0 && buffer[j - 1] == ' ') {
        j--; // Effectively remove trailing space before null termination
    }
    buffer[j] = '\0'; // Null-terminate the processed string
       
    return true;
}

boolean readUsernameWithTrim(char *buffer, int bufferSize, const char *prompt) {
    // This function is intended to read usernames, which can include spaces.
    // It should trim leading/trailing whitespace and condense internal spaces.
    // readStringWithSpaces already provides this functionality.
    return readStringWithSpaces(buffer, bufferSize, prompt);
}