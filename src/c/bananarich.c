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

    // Use floatToString to handle the float conversion
    char balanceStr[50] = "";
    if (!floatToString(balance, balanceStr, sizeof(balanceStr), 2))
    {
        printError("Gagal mengonversi float ke string!");
        return;
    }

    // Add the color and unit
    strcat(balanceStr, " BananaRich");

    int widths[] = {15, 20};
    const char *headers[] = {userType, "Saldo"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    printTableBorder(widths, 2, 2);
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

    // Use floatToString to handle the float conversion
    char balanceStr[50] = "";
    if (!floatToString(balance, balanceStr, sizeof(balanceStr), 2))
    {
        printError("Gagal mengonversi float ke string!");
        return;
    }

    // Add the unit
    strcat(balanceStr, " BananaRich");

    int widths[] = {15, 20};
    const char *headers[] = {"Entitas", "Saldo"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    printTableBorder(widths, 2, 2);
    const char *row[] = {"Rumah Sakit", balanceStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);
}

// Counter untuk tracking pemanggilan gacha (persistent)
static int gachaCallCounter = 0;

// Fungsi hash sederhana untuk string
unsigned long hashString(const char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// Fungsi untuk generate pseudo-random number tanpa time.h
unsigned long generateSeed(Patient *patient)
{
    gachaCallCounter++;

    unsigned long seed = 0;

    // Kombinasi berbagai sumber entropy yang tersedia
    seed ^= hashString(patient->username);               // Username hash
    seed ^= (unsigned long)(patient->bananaRich * 100);  // Balance (tapi dikontrol)
    seed ^= (unsigned long)gachaCallCounter * 1664525UL; // Call counter
    seed ^= (unsigned long)&patient;                     // Memory address
    seed ^= (unsigned long)&seed;                        // Stack address

    // Mixing step untuk distribusi yang lebih baik
    seed = (seed ^ (seed >> 16)) * 0x45d9f3bUL;
    seed = (seed ^ (seed >> 16)) * 0x45d9f3bUL;
    seed = seed ^ (seed >> 16);

    return seed;
}

// LCG dengan seed yang dinamis
unsigned long lcgRandom(unsigned long seed)
{
    return (seed * 1664525UL + 1013904223UL) & 0xFFFFFFFFUL;
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

    // Generate seed dinamis
    unsigned long seed = generateSeed(patient);

    // Generate beberapa random number untuk variasi
    unsigned long rand1 = lcgRandom(seed);
    unsigned long rand2 = lcgRandom(rand1);
    unsigned long rand3 = lcgRandom(rand2);

    float reward = 0.0;
    char rewardMessage[100] = "";

    // Gunakan rand2 untuk reward type (0-99)
    int rewardType = rand2 % 100;

    // Gunakan rand3 untuk memilih item dalam kategori
    int itemChoice = rand3 % 3;

    // Distribusi baru:
    // 40% - Tidak dapat apa-apa
    // 35% - Small rewards (2, 5, 10)
    // 15% - Medium rewards (15, 20, 25)
    // 8% - Large rewards (50, 100)
    // 2% - Jackpot (200)

    if (rewardType < 40)
    {
        // 40% chance untuk tidak mendapat apa-apa
        printf("Selamat datang %s, di Mesin Gacha %sXXeon06%s!\n", patient->username, FORMAT_BOLD COLOR_BLUE, FORMAT_RESET);
        strcpy(rewardMessage, "[👀 Yahhh] - Kamu kurang beruntung, coba lagi nanti!");
        printf(COLOR_YELLOW FORMAT_BOLD "%s\n" FORMAT_RESET, rewardMessage);
        return true;
    }
    else if (rewardType < 75)
    {
        // 35% chance for small rewards (2, 5, 10 BananaRich)
        int smallRewards[] = {2, 5, 10};
        reward = smallRewards[itemChoice];
        strcat(rewardMessage, "[😲 WOW] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];
        floatToString(reward, rewardStr, sizeof(rewardStr), 2);
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }
    else if (rewardType < 90)
    {
        // 15% chance for medium rewards (15, 20, 25 BananaRich)
        int mediumRewards[] = {15, 20, 25};
        reward = mediumRewards[itemChoice];
        strcat(rewardMessage, "[🎉 YAY] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];
        floatToString(reward, rewardStr, sizeof(rewardStr), 2);
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }
    else if (rewardType < 98)
    {
        // 8% chance for large rewards (50, 100 BananaRich)
        int largeRewards[] = {50, 100};
        reward = largeRewards[itemChoice % 2]; // Hanya 2 pilihan
        strcat(rewardMessage, "[🥳 HOREE] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];
        floatToString(reward, rewardStr, sizeof(rewardStr), 2);
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }
    else
    {
        // 2% chance for Jackpot reward (200 BananaRich)
        reward = 200;
        strcat(rewardMessage, "[🎊 JACKPOT] - ");
        strcat(rewardMessage, "Selamat! ");
        strcat(rewardMessage, "Anda mendapatkan ");
        char rewardStr[10];
        floatToString(reward, rewardStr, sizeof(rewardStr), 2);
        strcat(rewardMessage, rewardStr);
        strcat(rewardMessage, " BananaRich!");
    }

    patient->bananaRich += reward;

    printHeader("Hasil Gacha");
    printf("Selamat datang %s, di Mesin Gacha %sXXeon06%s!\n", patient->username, FORMAT_BOLD COLOR_BLUE, FORMAT_RESET);
    printf(COLOR_YELLOW FORMAT_BOLD "%s\n" FORMAT_RESET, rewardMessage);

    // Format saldo sebelum dan sesudah
    char beforeStr[50] = "";
    char afterStr[50] = "";
    float beforeBalance = patient->bananaRich - reward;
    float afterBalance = patient->bananaRich;

    if (!floatToString(beforeBalance, beforeStr, sizeof(beforeStr), 2) ||
        !floatToString(afterBalance, afterStr, sizeof(afterStr), 2))
    {
        printError("Gagal mengonversi saldo ke string!");
        return false;
    }

    printf("\n[💰 | Info]: Saldo sebelum: %s\n", beforeStr);
    printf("[🤑 | Info]: Saldo sesudah: %s\n", afterStr);

    return true;
}