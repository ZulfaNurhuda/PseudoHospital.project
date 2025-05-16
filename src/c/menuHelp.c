#include "menuHelp.h"

void displayMenu(Session *session)
{
    if (session == NULL)
    {
        printError("Sesi tidak valid!");
        return;
    }

    printHeader("Daftar Perintah");
    const char *menu[20];
    int count = 0;
    int widths[] = {20};

    if (!session->isLoggedIn)
    {
        printf(COLOR_YELLOW "Anda belum login. Perintah yang tersedia:\n" COLOR_RESET);
        menu[count++] = "LOGIN";
        menu[count++] = "REGISTER_PASIEN";
        menu[count++] = "LUPA_PASSWORD";
        menu[count++] = "MENU";
        menu[count++] = "HELP";
        menu[count++] = "EXIT";
    }
    else
    {
        printf(COLOR_YELLOW "Perintah untuk %s:\n" COLOR_RESET, session->username);
        switch (session->role)
        {
        case MANAGER:
            menu[count++] = "LOGOUT";
            menu[count++] = "MENU";
            menu[count++] = "HELP";
            menu[count++] = "LIHAT_DENAH";
            menu[count++] = "VIEWUSERS";
            menu[count++] = "SEARCHUSER";
            menu[count++] = "VIEWQUEUE";
            menu[count++] = "ADDDOCTOR";
            menu[count++] = "CHANGELAYOUT";
            menu[count++] = "MOVEDOCTOR";
            menu[count++] = "VIEWFINANCIAL";
            menu[count++] = "EXIT";
            break;
        case DOCTOR:
            menu[count++] = "LOGOUT";
            menu[count++] = "MENU";
            menu[count++] = "HELP";
            menu[count++] = "LIHAT_DENAH";
            menu[count++] = "DIAGNOSIS";
            menu[count++] = "TREAT";
            menu[count++] = "VIEWWALLET";
            menu[count++] = "EXIT";
            break;
        case PATIENT:
            menu[count++] = "LOGOUT";
            menu[count++] = "MENU";
            menu[count++] = "HELP";
            menu[count++] = "LIHAT_DENAH";
            menu[count++] = "CANGOHOME";
            menu[count++] = "REGISTERCHECKUP";
            menu[count++] = "MYQUEUE";
            menu[count++] = "TAKEMEDICATION";
            menu[count++] = "TAKEANTIDOTE";
            menu[count++] = "VIEWWALLET";
            menu[count++] = "GACHA";
            menu[count++] = "SKIPQUEUE";
            menu[count++] = "CANCELQUEUE";
            menu[count++] = "EXIT";
            break;
        }
    }

    printTableBorder(widths, 1, 1);
    for (int i = 0; i < count; i++)
    {
        printTableRow(&menu[i], widths, 1);
    }
    printTableBorder(widths, 1, 3);
}

void displayHelp(Session *session, const char *command)
{
    if (session == NULL)
    {
        printError("Sesi tidak valid!");
        return;
    }
    if (command == NULL || command[0] == '\0')
    {
        printError("Perintah tidak valid!");
        return;
    }

    printHeader("Bantuan Perintah");

    if (!session->isLoggedIn)
    {
        if (strcmp(command, "LOGIN") == 0)
        {
            printf("LOGIN: Masuk ke sistem dengan username dan password.\n");
        }
        else if (strcmp(command, "REGISTERPATIENT") == 0)
        {
            printf("REGISTERPATIENT: Daftar sebagai pasien baru dengan username dan password.\n");
        }
        else if (strcmp(command, "FORGOTPASSWORD") == 0)
        {
            printf("FORGOTPASSWORD: Atur ulang kata sandi menggunakan username dan kode verifikasi.\n");
        }
        else if (strcmp(command, "MENU") == 0)
        {
            printf("MENU: Tampilkan daftar perintah yang tersedia.\n");
        }
        else if (strcmp(command, "HELP") == 0)
        {
            printf("HELP: Tampilkan bantuan untuk perintah tertentu.\n");
        }
        else if (strcmp(command, "EXIT") == 0)
        {
            printf("EXIT: Keluar dari sistem.\n");
        }
        else
        {
            char errorMessage[100] = "Perintah ";
            strcat(errorMessage, command);
            strcat(errorMessage, " tidak dikenal!");
            printError(errorMessage);
        }
    }
    else
    {
        switch (session->role)
        {
        case MANAGER:
            if (strcmp(command, "LOGOUT") == 0)
            {
                printf("LOGOUT: Keluar dari sesi saat ini.\n");
            }
            else if (strcmp(command, "MENU") == 0)
            {
                printf("MENU: Tampilkan daftar perintah yang tersedia.\n");
            }
            else if (strcmp(command, "HELP") == 0)
            {
                printf("HELP: Tampilkan bantuan untuk perintah tertentu.\n");
            }
            else if (strcmp(command, "LIHAT_DENAH") == 0)
            {
                printf("LIHAT_DENAH: Tampilkan denah rumah sakit atau detail ruangan.\n");
            }
            else if (strcmp(command, "VIEWUSERS") == 0)
            {
                printf("VIEWUSERS: Tampilkan daftar pengguna (pasien atau dokter).\n");
            }
            else if (strcmp(command, "SEARCHUSER") == 0)
            {
                printf("SEARCHUSER: Cari pengguna berdasarkan ID atau username.\n");
            }
            else if (strcmp(command, "VIEWQUEUE") == 0)
            {
                printf("VIEWQUEUE: Tampilkan antrian pasien di setiap ruangan.\n");
            }
            else if (strcmp(command, "ADDDOCTOR") == 0)
            {
                printf("ADDDOCTOR: Tambah dokter baru atau tetapkan dokter ke ruangan.\n");
            }
            else if (strcmp(command, "CHANGELAYOUT") == 0)
            {
                printf("CHANGELAYOUT: Ubah ukuran denah rumah sakit.\n");
            }
            else if (strcmp(command, "MOVEDOCTOR") == 0)
            {
                printf("MOVEDOCTOR: Pindahkan dokter ke ruangan lain.\n");
            }
            else if (strcmp(command, "VIEWFINANCIAL") == 0)
            {
                printf("VIEWFINANCIAL: Tampilkan laporan keuangan rumah sakit.\n");
            }
            else if (strcmp(command, "EXIT") == 0)
            {
                printf("EXIT: Keluar dari sistem.\n");
            }
            else
            {
                char errorMessage[100] = "Perintah ";
                strcat(errorMessage, command);
                strcat(errorMessage, " tidak dikenal!");
                printError(errorMessage);
            }
            break;
        case DOCTOR:
            if (strcmp(command, "LOGOUT") == 0)
            {
                printf("LOGOUT: Keluar dari sesi saat ini.\n");
            }
            else if (strcmp(command, "MENU") == 0)
            {
                printf("MENU: Tampilkan daftar perintah yang tersedia.\n");
            }
            else if (strcmp(command, "HELP") == 0)
            {
                printf("HELP: Tampilkan bantuan untuk perintah tertentu.\n");
            }
            else if (strcmp(command, "LIHAT_DENAH") == 0)
            {
                printf("LIHAT_DENAH: Tampilkan denah rumah sakit atau detail ruangan.\n");
            }
            else if (strcmp(command, "DIAGNOSIS") == 0)
            {
                printf("DIAGNOSIS: Lakukan diagnosis otomatis untuk pasien.\n");
            }
            else if (strcmp(command, "TREAT") == 0)
            {
                printf("TREAT: Berikan resep obat otomatis berdasarkan penyakit pasien.\n");
            }
            else if (strcmp(command, "VIEWWALLET") == 0)
            {
                printf("VIEWWALLET: Tampilkan saldo BananaRich dokter.\n");
            }
            else if (strcmp(command, "EXIT") == 0)
            {
                printf("EXIT: Keluar dari sistem.\n");
            }
            else
            {
                char errorMessage[100] = "Perintah ";
                strcat(errorMessage, command);
                strcat(errorMessage, " tidak dikenal!");
                printError(errorMessage);
            }
            break;
        case PATIENT:
            if (strcmp(command, "LOGOUT") == 0)
            {
                printf("LOGOUT: Keluar dari sesi saat ini.\n");
            }
            else if (strcmp(command, "MENU") == 0)
            {
                printf("MENU: Tampilkan daftar perintah yang tersedia.\n");
            }
            else if (strcmp(command, "HELP") == 0)
            {
                printf("HELP: Tampilkan bantuan untuk perintah tertentu.\n");
            }
            else if (strcmp(command, "LIHAT_DENAH") == 0)
            {
                printf("LIHAT_DENAH: Tampilkan denah rumah sakit atau detail ruangan.\n");
            }
            else if (strcmp(command, "CANGOHOME") == 0)
            {
                printf("CANGOHOME: Periksa apakah pasien boleh pulang berdasarkan status pengobatan.\n");
            }
            else if (strcmp(command, "REGISTERCHECKUP") == 0)
            {
                printf("REGISTERCHECKUP: Daftar untuk pemeriksaan dengan dokter tertentu.\n");
            }
            else if (strcmp(command, "MYQUEUE") == 0)
            {
                printf("MYQUEUE: Tampilkan status antrian pasien saat ini.\n");
            }
            else if (strcmp(command, "TAKEMEDICATION") == 0)
            {
                printf("TAKEMEDICATION: Minum obat sesuai urutan yang diresepkan.\n");
            }
            else if (strcmp(command, "TAKEANTIDOTE") == 0)
            {
                printf("TAKEANTIDOTE: Minum penawar untuk membatalkan obat terakhir.\n");
            }
            else if (strcmp(command, "VIEWWALLET") == 0)
            {
                printf("VIEWWALLET: Tampilkan saldo BananaRich pasien.\n");
            }
            else if (strcmp(command, "GACHA") == 0)
            {
                printf("GACHA: Gunakan BananaRich untuk bermain gacha.\n");
            }
            else if (strcmp(command, "SKIPQUEUE") == 0)
            {
                printf("SKIPQUEUE: Lewati antrian untuk pemeriksaan.\n");
            }
            else if (strcmp(command, "CANCELQUEUE") == 0)
            {
                printf("CANCELQUEUE: Batalkan antrian pemeriksaan saat ini.\n");
            }
            else if (strcmp(command, "EXIT") == 0)
            {
                printf("EXIT: Keluar dari sistem.\n");
            }
            else
            {
                char errorMessage[100] = "Perintah ";
                strcat(errorMessage, command);
                strcat(errorMessage, " tidak dikenal!");
                printError(errorMessage);
            }
            break;
        }
    }
}