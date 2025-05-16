#include "bananarich.h"

void viewWallet(Hospital *hospital, Session *session)
{
    if (!session->isLoggedIn || (session->role != DOCTOR && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Dokter dan Pasien yang dapat melihat dompet.");
        return;
    }

    printHeader("Saldo Dompet");
    float balance = 0.0;
    char *userType = session->role == PATIENT ? "Pasien" : "Dokter";

    if (session->role == PATIENT)
    {
        for (int i = 0; i < hospital->patients.nEff; i++)
        {
            if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
            {
                balance = hospital->patients.elements[i].bananaRich;
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            if (strcmp(hospital->doctors.elements[i].username, session->username) == 0)
            {
                balance = hospital->doctors.elements[i].bananaRich;
                break;
            }
        }
    }

    // Format saldo ke string secara manual
    char balanceStr[50] = "";
    int intPart = (int)balance;
    int decPart = (int)((balance - intPart) * 100);
    char tempStr[20] = "";

    // Konversi bagian integer
    if (intPart == 0)
        strcat(tempStr, "0");
    else
    {
        char digits[20] = "";
        int index = 0;
        while (intPart > 0)
        {
            digits[index++] = (intPart % 10) + '0';
            intPart /= 10;
        }
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &digits[i], 1);
    }

    // Tambahkan titik desimal dan bagian desimal
    strcat(tempStr, ".");
    if (decPart < 10)
        strcat(tempStr, "0");
    char decStr[10] = "";
    int index = 0;
    while (decPart > 0)
    {
        decStr[index++] = (decPart % 10) + '0';
        decPart /= 10;
    }
    if (index == 0)
        strcat(tempStr, "00");
    else
    {
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &decStr[i], 1);
        if (index == 1)
            strcat(tempStr, "0");
    }

    // Tambahkan warna kuning dan unit
    strcat(balanceStr, COLOR_YELLOW);
    strcat(balanceStr, tempStr);
    strcat(balanceStr, " BananaRich");
    strcat(balanceStr, COLOR_RESET);

    int widths[] = {15, 20};
    const char *headers[] = {userType, "Saldo"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    const char *row[] = {session->username, balanceStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);
}

void viewFinancial(Hospital *hospital, Session *session)
{
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat finansial.");
        return;
    }

    printHeader("Finansial Rumah Sakit");
    float balance = hospital->finance.hospitalBalance;

    // Format saldo ke string secara manual
    char balanceStr[50] = "";
    int intPart = (int)balance;
    int decPart = (int)((balance - intPart) * 100);
    char tempStr[20] = "";

    // Konversi bagian integer
    if (intPart == 0)
        strcat(tempStr, "0");
    else
    {
        char digits[20] = "";
        int index = 0;
        while (intPart > 0)
        {
            digits[index++] = (intPart % 10) + '0';
            intPart /= 10;
        }
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &digits[i], 1);
    }

    // Tambahkan titik desimal dan bagian desimal
    strcat(tempStr, ".");
    if (decPart < 10)
        strcat(tempStr, "0");
    char decStr[10] = "";
    int index = 0;
    while (decPart > 0)
    {
        decStr[index++] = (decPart % 10) + '0';
        decPart /= 10;
    }
    if (index == 0)
        strcat(tempStr, "00");
    else
    {
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &decStr[i], 1);
        if (index == 1)
            strcat(tempStr, "0");
    }

    // Tambahkan unit
    strcat(balanceStr, tempStr);
    strcat(balanceStr, " BananaRich");

    int widths[] = {15, 20};
    const char *headers[] = {"Entitas", "Saldo"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    const char *row[] = {"Rumah Sakit", balanceStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);
}

boolean gacha(Hospital *hospital, Session *session)
{
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat melakukan gacha.");
        return false;
    }

    int patientIndex = -1;
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
        {
            patientIndex = i;
            break;
        }
    }
    if (patientIndex == -1)
    {
        printError("Pasien tidak ditemukan!");
        return false;
    }

    Patient *patient = &hospital->patients.elements[patientIndex];
    const float gachaCost = 10.0;

    if (patient->bananaRich < gachaCost)
    {
        printError("Saldo BananaRich tidak cukup! Dibutuhkan 10.00 BananaRich.");
        return false;
    }

    patient->bananaRich -= gachaCost;
    hospital->finance.hospitalBalance += gachaCost;

    // Simulasi gacha
    int seed = (int)(patient->bananaRich * 100);
    int result = seed % 3;
    float reward = 0.0;
    char rewardMessage[100] = "";

    switch (result)
    {
    case 0:
        reward = 5.0;
        strcpy(rewardMessage, "Selamat, Anda mendapatkan 5 BananaRich!");
        break;
    case 1:
        reward = 20.0;
        strcpy(rewardMessage, "Jackpot! Anda mendapatkan 20 BananaRich!");
        break;
    default:
        reward = 0.0;
        strcpy(rewardMessage, "Maaf, Anda tidak mendapatkan hadiah.");
        break;
    }

    patient->bananaRich += reward;

    printHeader("Hasil Gacha");
    printf(COLOR_YELLOW "%s\n" COLOR_RESET, rewardMessage);

    // Format saldo sebelum dan sesudah
    char beforeStr[50] = "";
    char afterStr[50] = "";
    float beforeBalance = patient->bananaRich - reward;
    float afterBalance = patient->bananaRich;

    // Format saldo sebelum
    int intPart = (int)beforeBalance;
    int decPart = (int)((beforeBalance - intPart) * 100);
    char tempStr[20] = "";
    if (intPart == 0)
        strcat(tempStr, "0");
    else
    {
        char digits[20] = "";
        int index = 0;
        while (intPart > 0)
        {
            digits[index++] = (intPart % 10) + '0';
            intPart /= 10;
        }
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &digits[i], 1);
    }
    strcat(tempStr, ".");
    if (decPart < 10)
        strcat(tempStr, "0");
    char decStr[10] = "";
    int index = 0;
    while (decPart > 0)
    {
        decStr[index++] = (decPart % 10) + '0';
        decPart /= 10;
    }
    if (index == 0)
        strcat(tempStr, "00");
    else
    {
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &decStr[i], 1);
        if (index == 1)
            strcat(tempStr, "0");
    }
    strcat(beforeStr, tempStr);
    strcat(beforeStr, " BananaRich");

    // Format saldo sesudah
    intPart = (int)afterBalance;
    decPart = (int)((afterBalance - intPart) * 100);
    tempStr[0] = '\0';
    if (intPart == 0)
        strcat(tempStr, "0");
    else
    {
        char digits[20] = "";
        index = 0;
        while (intPart > 0)
        {
            digits[index++] = (intPart % 10) + '0';
            intPart /= 10;
        }
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &digits[i], 1);
    }
    strcat(tempStr, ".");
    if (decPart < 10)
        strcat(tempStr, "0");
    decStr[0] = '\0';
    index = 0;
    while (decPart > 0)
    {
        decStr[index++] = (decPart % 10) + '0';
        decPart /= 10;
    }
    if (index == 0)
        strcat(tempStr, "00");
    else
    {
        for (int i = index - 1; i >= 0; i--)
            strncat(tempStr, &decStr[i], 1);
        if (index == 1)
            strcat(tempStr, "0");
    }
    strcat(afterStr, tempStr);
    strcat(afterStr, " BananaRich");

    int widths[] = {15, 20};
    const char *headers[] = {"Saldo Sebelum", "Saldo Sekarang"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    const char *row[] = {beforeStr, afterStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    printSuccess("Gacha selesai!");
    return true;
}