#include "hospitalSystem.h"

void normalizeCommand(char *command)
{
    int i = 0, j = 0;
    while (command[i])
    {
        if (command[i] != ' ' && command[i] != '_')
        {
            command[j] = (command[i] >= 'a' && command[i] <= 'z') ? command[i] - 32 : command[i];
            j++;
        }
        i++;
    }
    command[j] = '\0';
}

int main(int argc, char *argv[])
{
    Hospital hospital;
    Session session;

    // Inisialisasi ukuran denah sesuai argument
    int hospitalRows, hospitalCols;
    if (argc >= 2 && argc <= 3)
    {
        hospitalRows = stringToInt(argv[1]);
        hospitalCols = (argc == 3) ? stringToInt(argv[2]) : hospitalRows;

        // Validasi input
        if (hospitalRows <= 0 || hospitalCols <= 0)
        {
            printError("Argumen ukuran denah tidak valid!");
            return 1;
        }
    }
    else
    {
        hospitalRows = 5;
        hospitalCols = 5;
    }

    // Inisialisasi hospital
    initHospital(&hospital, 100, 100, 100, hospitalRows, hospitalCols);

    // Inisialisasi Seorang Manajer
    hospital.users.elements[0].id = 1;
    strcpy(hospital.users.elements[0].username, "NimonsDawg");
    if (!enigmaEncrypt("admoontothemoon", hospital.users.elements[0].password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password!");
        return false;
    }
    hospital.users.elements[0].role = MANAGER;
    hospital.users.nEff++;

    // Inisialisasi sesi
    session.isLoggedIn = false;
    session.userID = -1;
    session.username[0] = '\0';
    session.role = -1;

    char command[50];
    printHeader("Sistem Manajemen Rumah Sakit Nimons");
    printf("Ketik 'MENU' untuk melihat daftar perintah.\n");

    while (1)
    {
        printf("\n> ");
        if (!readValidString(command, 50, "Masukkan perintah: ", false))
        {
            printError("Perintah tidak valid!");
            continue;
        }
        normalizeCommand(command);

        // F01 - Login
        if (strcmp(command, "LOGIN") == 0)
        {
            char username[50], password[100];
            if (!readValidString(username, 50, "Masukkan username: ", true) ||
                !readValidString(password, 100, "Masukkan password: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            login(&hospital, &session, username, password);
        }
        // F02 - Register Pasien
        else if (strcmp(command, "REGISTERPASIEN") == 0)
        {
            char username[50], password[100];
            if (!readValidString(username, 50, "Masukkan username: ", true) ||
                !readValidString(password, 100, "Masukkan password: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            registerPatient(&hospital, &session, username, password);
        }
        // F03 - Logout
        else if (strcmp(command, "LOGOUT") == 0)
        {
            logout(&session);
        }
        // F04 - Forgot Password
        else if (strcmp(command, "LUPAPASSWORD") == 0)
        {
            char username[50], new_password[100], rleCode[100];
            if (!readValidString(username, 50, "Masukkan username: ", true) ||
                !readValidString(rleCode, 100, "Masukkan kode RLE: ", false) ||
                !readValidString(new_password, 100, "Masukkan password baru: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            forgotPassword(&hospital, username, rleCode, new_password);
        }
        // F05 - Menu & Help
        else if (strcmp(command, "MENU") == 0)
        {
            displayMenu(&session);
        }
        // F06 - Denah Rumah Sakit
        else if (strcmp(command, "LIHATDENAH") == 0)
        {
            displayLayout(&hospital, &session);
        }
        else if (strcmp(command, "LIHATRUANGAN") == 0)
        {
            char room_code[5];
            if (!readValidString(room_code, 5, "Masukkan kode ruangan: ", false))
            {
                printError("Kode ruangan tidak valid!");
                continue;
            }
            displayRoomDetails(&hospital, &session, room_code);
        }
        // F07 - Lihat User
        else if (strcmp(command, "LIHATUSER") == 0)
        {
            displayUsers(&hospital, &session);
        }
        else if (strcmp(command, "LIHATPASIEN") == 0)
        {
            displayPatients(&hospital, &session);
        }
        else if (strcmp(command, "LIHATDOKTER") == 0)
        {
            displayDoctors(&hospital, &session);
        }
        // F08 - Cari User
        else if (strcmp(command, "CARIUSER") == 0)
        {
            char query[50];
            boolean by_id = false;
            printf("Cari berdasarkan ID? (1=Ya, 0=Tidak): ");
            int choice;
            if (!readValidInt(&choice, "Masukkan pilihan: "))
                continue;
            by_id = choice == 1;
            if (!readValidString(query, 50, "Masukkan query: ", false))
            {
                printError("Query tidak valid!");
                continue;
            }
            findUser(&hospital, &session, query, by_id);
        }
        else if (strcmp(command, "CARIPASIEN") == 0)
        {
            char query[50];
            boolean by_id = false;
            printf("Cari berdasarkan ID? (1=Ya, 0=Tidak): ");
            int choice;
            if (!readValidInt(&choice, "Masukkan pilihan: "))
                continue;
            by_id = choice == 1;
            if (!readValidString(query, 50, "Masukkan query: ", false))
            {
                printError("Query tidak valid!");
                continue;
            }
            findPatient(&hospital, &session, query, by_id);
        }
        else if (strcmp(command, "CARIDOKTER") == 0)
        {
            char query[50];
            boolean by_id = false;
            printf("Cari berdasarkan ID? (1 - Ya, 0 - Tidak): ");
            int choice;
            if (!readValidInt(&choice, "Masukkan pilihan: "))
                continue;
            by_id = choice == 1;
            if (!readValidString(query, 50, "Masukkan query: ", false))
            {
                printError("Query tidak valid!");
                continue;
            }
            findDoctor(&hospital, &session, query, by_id);
        }
        // F09 - Lihat Antrian
        else if (strcmp(command, "LIHATANTRIAN") == 0)
        {
            displayQueue(&hospital, &session);
        }
        // F10 - Tambah Dokter
        else if (strcmp(command, "TAMBAHDOKTER") == 0)
        {
            char username[50], password[100], specialization[50];
            if (!readValidString(username, 50, "Masukkan username dokter: ", true) ||
                !readValidString(password, 100, "Masukkan password dokter: ", false) ||
                !readValidString(specialization, 50, "Masukkan spesialisasi dokter: ", true))
            {
                printError("Input tidak valid!");
                continue;
            }
            addDoctor(&hospital, &session, username, password, specialization);
        }
        else if (strcmp(command, "ASSIGNDOKTER") == 0)
        {
            char username[50], room_code[5];
            if (!readValidString(username, 50, "Masukkan username dokter: ", true) ||
                !readValidString(room_code, 5, "Masukkan kode ruangan: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            assignDoctor(&hospital, &session, username, room_code);
        }
        // F11 - Diagnosis
        else if (strcmp(command, "DIAGNOSIS") == 0)
        {
            char patient_username[50];
            if (!readValidString(patient_username, 50, "Masukkan username pasien: ", true))
            {
                printError("Username pasien tidak valid!");
                continue;
            }
            diagnosePatient(&hospital, &session, patient_username);
        }
        // F12 - Ngobatin
        else if (strcmp(command, "NGOBATIN") == 0)
        {
            char patient_username[50];
            if (!readValidString(patient_username, 50, "Masukkan username pasien: ", true))
            {
                printError("Username pasien tidak valid!");
                continue;
            }
            treatPatient(&hospital, &session, patient_username);
        }
        // F13 - Pulang Dok
        else if (strcmp(command, "PULANGDOK") == 0)
        {
            canGoHome(&hospital, &session);
        }
        // F14 - Daftar Checkup
        else if (strcmp(command, "DAFTARCHECKUP") == 0)
        {
            char doctor_username[50];
            float health_data[11];
            if (!readValidString(doctor_username, 50, "Masukkan username dokter: ", true))
            {
                printError("Username dokter tidak valid!");
                continue;
            }
            printf("Masukkan data kesehatan (11 nilai):\n");
            for (int i = 0; i < 11; i++)
            {
                if (!readValidFloat(&health_data[i], "Masukkan data kesehatan: "))
                {
                    printError("Data kesehatan tidak valid!");
                    break;
                }
            }
            registerCheckup(&hospital, &session, doctor_username, health_data);
        }
        // F15 - Antrian Saya
        else if (strcmp(command, "ANTRIANSAYA") == 0)
        {
            myQueue(&hospital, &session);
        }
        // F16 - Minum Obat
        else if (strcmp(command, "MINUMOBAT") == 0)
        {
            int medication_id;
            if (!readValidInt(&medication_id, "Masukkan ID obat: "))
            {
                printError("ID obat tidak valid!");
                continue;
            }
            takeMedication(&hospital, &session, medication_id);
        }
        // F17 - Minum Penawar
        else if (strcmp(command, "MINUMPENAWAR") == 0)
        {
            takeAntidote(&hospital, &session);
        }
        // F18 - Exit
        else if (strcmp(command, "EXIT") == 0)
        {
            exitProgram(&hospital, &session);
        }
        // B02 - Denah Dinamis
        else if (strcmp(command, "UBAHDENAH") == 0)
        {
            int rows, cols;
            if (!readValidInt(&rows, "Masukkan jumlah baris: ") ||
                !readValidInt(&cols, "Masukkan jumlah kolom: "))
            {
                printError("Ukuran denah tidak valid!");
                continue;
            }
            changeLayout(&hospital, &session, rows, cols);
        }
        else if (strcmp(command, "PINDAHKANDOKTER") == 0)
        {
            char username[50], room_code[5];
            if (!readValidString(username, 50, "Masukkan username dokter: ", true) ||
                !readValidString(room_code, 5, "Masukkan kode ruangan: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            moveDoctor(&hospital, &session, username, room_code);
        }
        // B03 - BananaRich
        else if (strcmp(command, "LIHATDOMPET") == 0)
        {
            viewWallet(&hospital, &session);
        }
        else if (strcmp(command, "LIHATFINANSIAL") == 0)
        {
            viewFinancial(&hospital, &session);
        }
        else if (strcmp(command, "GACHA") == 0)
        {
            gacha(&hospital, &session);
        }
        // B05 - Mainin Antrian
        else if (strcmp(command, "SKIPANTRIAN") == 0)
        {
            skipQueue(&hospital, &session);
        }
        else if (strcmp(command, "CANCELANTRIAN") == 0)
        {
            cancelQueue(&hospital, &session);
        }
        // New Feature: View Life Status
        else if (strcmp(command, "LIHATNYAWA") == 0)
        {
            viewLifeStatus(&hospital, &session);
        }
        else
        {
            printError("Perintah tidak dikenal! Ketik 'MENU' untuk bantuan.");
        }
    }

    return 0;
}