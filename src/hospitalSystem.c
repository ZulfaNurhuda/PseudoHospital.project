#include "hospitalSystem.h"

static void normalizeCommand(char *command)
{
    int i = 0, j = 0;
    while (command[i])
    {
        if (command[i] != '_')
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

    int hospitalRows, hospitalCols;
    if (argc >= 2 && argc <= 3)
    {
        hospitalRows = stringToInt(argv[1]);
        hospitalCols = (argc == 3) ? stringToInt(argv[2]) : hospitalRows;

        if (hospitalRows <= 0 || hospitalCols <= 0 || hospitalRows > 10 || hospitalCols > 10)
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

    if (!initHospital(&hospital, 100, 100, 100, hospitalRows, hospitalCols))
    {
        printError("Inisialisasi rumah sakit gagal. Program dihentikan.");
        return 1;
    }

    session.isLoggedIn = false;
    session.userId = -1;
    session.username[0] = '\0';
    session.role = -1;

    char command[50];
    printHeader("Sistem Manajemen Rumah Sakit Nimons");
    printf("Ketik 'MENU' untuk melihat daftar perintah.\n");

    while (1)
    {
        if (!readValidString(command, 50, "\n>>> Masukkan perintah: ", false))
        {
            printError("Perintah tidak valid!");
            continue;
        }
        normalizeCommand(command);

        if (strcmp(command, "LOGIN") == 0)
        {
            char username[50], password[100];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username: ") ||
                !readValidString(password, 100, "Masukkan password: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            login(&hospital, &session, username, password);
        }

        else if (strcmp(command, "REGISTERPASIEN") == 0)
        {
            char username[50], password[100];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username: ") ||
                !readValidString(password, 100, "Masukkan password: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            registerPatient(&hospital, &session, username, password);
        }

        else if (strcmp(command, "LOGOUT") == 0)
        {
            logout(&session);
        }

        else if (strcmp(command, "LUPAPASSWORD") == 0)
        {
            char username[51], new_password[100], rleCode[100];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username: ") ||
                !readValidString(rleCode, 100, "Masukkan kode RLE: ", false) ||
                !readValidString(new_password, 100, "Masukkan password baru: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            forgotPassword(&hospital, username, rleCode, new_password);
        }
        else if (strcmp(command, "MENU") == 0)
        {
            displayMenu(&session);
        }
        else if (strcmp(command, "HELP") == 0)
        {
            char helpCommand[50];
            if (!readValidString(helpCommand, 50, "Masukkan nama perintah untuk bantuan: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            displayHelp(&session, helpCommand);
        }
        else if (strcmp(command, "LIHATDENAH") == 0)
        {
            displayLayout(&hospital, &session, true);
        }
        else if (strcmp(command, "LIHATRUANGAN") == 0)
        {
            char roomCode[5];
            if (!readValidString(roomCode, 5, "Masukkan kode ruangan: ", false))
            {
                printError("Kode ruangan tidak valid!");
                continue;
            }
            displayRoomDetails(&hospital, &session, roomCode);
        }

        else if (strcmp(command, "LIHATUSER") == 0)
        {
            int choiceSortBy;
            int choiceSortOrder;

            int widths[] = {5, 20};
            const char *headers[] = {"No.", "Urutan berdasarkan"};
            printTableBorder(widths, 2, 1);
            printTableRow(headers, widths, 2);
            printTableBorder(widths, 2, 2);
            const char *row1[] = {"1.", "ID"};
            const char *row2[] = {"2.", "Nama"};
            printTableRow(row1, widths, 2);
            printTableRow(row2, widths, 2);
            printTableBorder(widths, 2, 3);

            if (!readValidInt(&choiceSortBy, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            if (choiceSortBy < 1 || choiceSortBy > 2)
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            // Menampilkan menu urutan sorting dalam format tabel
            int _widths[] = {5, 20};
            const char *_headers[] = {"No.", "Urutan Sorting"};
            printTableBorder(_widths, 2, 1);
            printTableRow(_headers, _widths, 2);
            printTableBorder(_widths, 2, 2);
            const char *_row1[] = {"1.", "ASC (A-Z)"};
            const char *_row2[] = {"2.", "DESC (Z-A)"};
            printTableRow(_row1, _widths, 2);
            printTableRow(_row2, _widths, 2);
            printTableBorder(_widths, 2, 3);

            if (!readValidInt(&choiceSortOrder, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            if (choiceSortOrder < 1 || choiceSortOrder > 2)
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            displayUsers(&hospital, &session, choiceSortBy, choiceSortOrder);
        }

        // Untuk command "LIHATPASIEN"
        else if (strcmp(command, "LIHATPASIEN") == 0)
        {
            int choiceSortBy;
            int choiceSortOrder;

            int widths[] = {5, 20};
            const char *headers[] = {"No.", "Urutan berdasarkan"};
            printTableBorder(widths, 2, 1);
            printTableRow(headers, widths, 2);
            printTableBorder(widths, 2, 2);
            const char *row1[] = {"1.", "ID"};
            const char *row2[] = {"2.", "Nama"};
            printTableRow(row1, widths, 2);
            printTableRow(row2, widths, 2);
            printTableBorder(widths, 2, 3);

            if (!readValidInt(&choiceSortBy, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            if (choiceSortBy < 1 || choiceSortBy > 2)
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            int _widths[] = {5, 20};
            const char *_headers[] = {"No.", "Urutan Sorting"};
            printTableBorder(_widths, 2, 1);
            printTableRow(_headers, _widths, 2);
            printTableBorder(_widths, 2, 2);
            const char *_row1[] = {"1.", "ASC (A-Z)"};
            const char *_row2[] = {"2.", "DESC (Z-A)"};
            printTableRow(_row1, _widths, 2);
            printTableRow(_row2, _widths, 2);
            printTableBorder(_widths, 2, 3);

            if (!readValidInt(&choiceSortOrder, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            if (choiceSortOrder < 1 || choiceSortOrder > 2)
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            displayPatients(&hospital, &session, choiceSortBy, choiceSortOrder);
        }

        // Untuk command "LIHATDOKTER"
        else if (strcmp(command, "LIHATDOKTER") == 0)
        {
            int choiceSortBy;
            int choiceSortOrder;

            int widths[] = {5, 20};
            const char *headers[] = {"No.", "Urutan berdasarkan"};
            printTableBorder(widths, 2, 1);
            printTableRow(headers, widths, 2);
            printTableBorder(widths, 2, 2);
            const char *row1[] = {"1.", "ID"};
            const char *row2[] = {"2.", "Nama"};
            const char *row3[] = {"3.", "Aura"};
            printTableRow(row1, widths, 2);
            printTableRow(row2, widths, 2);
            printTableRow(row3, widths, 2);
            printTableBorder(widths, 2, 3);

            if (!readValidInt(&choiceSortBy, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            if (choiceSortBy < 1 || choiceSortBy > 3)
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            int _widths[] = {5, 20};
            const char *_headers[] = {"No.", "Urutan Sorting"};
            printTableBorder(_widths, 2, 1);
            printTableRow(_headers, _widths, 2);
            printTableBorder(_widths, 2, 2);
            const char *_row1[] = {"1.", "ASC (A-Z)"};
            const char *_row2[] = {"2.", "DESC (Z-A)"};
            printTableRow(_row1, _widths, 2);
            printTableRow(_row2, _widths, 2);
            printTableBorder(_widths, 2, 3);

            if (!readValidInt(&choiceSortOrder, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            if (choiceSortOrder < 1 || choiceSortOrder > 2)
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            displayDoctors(&hospital, &session, choiceSortBy, choiceSortOrder);
        }

        // Untuk command "CARIUSER"
        else if (strcmp(command, "CARIUSER") == 0)
        {
            char query[50];
            boolean byId = false;

            int widths[] = {5, 20};
            const char *headers[] = {"No.", "Cari berdasarkan"};
            printTableBorder(widths, 2, 1);
            printTableRow(headers, widths, 2);
            printTableBorder(widths, 2, 2);
            const char *row1[] = {"1.", "ID"};
            const char *row2[] = {"2.", "Nama"};
            printTableRow(row1, widths, 2);
            printTableRow(row2, widths, 2);
            printTableBorder(widths, 2, 3);

            int choice;
            if (!readValidInt(&choice, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            if (choice == 1)
            {
                byId = true;
                if (!readValidString(query, sizeof(query), ">>> Masukkan nomor ID user: ", false))
                {
                    printError("Input ID tidak valid!");
                    continue;
                }

                printf("\n");
            }
            else if (choice == 2)
            {
                byId = false;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama user: "))
                {
                    printError("Input Nama tidak valid!");
                    continue;
                }

                printf("\n");
            }
            else
            {
                printError("Pilihan tidak valid!");
                continue;
            }
            findUser(&hospital, &session, query, byId);
        }

        // Untuk command "CARIPASIEN"
        else if (strcmp(command, "CARIPASIEN") == 0)
        {
            char query[50];
            boolean byId = false;
            boolean byDisease = false;

            int widths[] = {5, 20};
            const char *headers[] = {"No.", "Cari berdasarkan"};
            printTableBorder(widths, 2, 1);
            printTableRow(headers, widths, 2);
            printTableBorder(widths, 2, 2);
            const char *row1[] = {"1.", "ID"};
            const char *row2[] = {"2.", "Nama"};
            const char *row3[] = {"3.", "Penyakit"};
            printTableRow(row1, widths, 2);
            printTableRow(row2, widths, 2);
            printTableRow(row3, widths, 2);
            printTableBorder(widths, 2, 3);

            int choice;
            if (!readValidInt(&choice, ">>> Pilihan: "))
            {
                continue;
            }

            printf("\n");

            if (choice == 1)
            {
                byId = true;
                byDisease = false;
                if (!readValidString(query, sizeof(query), ">>> Masukkan nomor ID user: ", false))
                {
                    printError("Input ID tidak valid!");
                    continue;
                }

                printf("\n");
            }
            else if (choice == 2)
            {
                byId = false;
                byDisease = false;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama user: "))
                {
                    printError("Input Nama tidak valid!");
                    continue;
                }

                printf("\n");
            }
            else if (choice == 3)
            {
                byId = false;
                byDisease = true;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama penyakit: "))
                {
                    printError("Input Penyakit tidak valid!");
                    continue;
                }

                printf("\n");
            }
            else
            {
                printError("Pilihan tidak valid!");
                continue;
            }
            findPatient(&hospital, &session, query, byId, byDisease);
        }

        // Untuk command "CARIDOKTER"
        else if (strcmp(command, "CARIDOKTER") == 0)
        {
            char query[51];
            boolean byId = false;

            int widths[] = {5, 20};
            const char *headers[] = {"No.", "Cari berdasarkan"};
            printTableBorder(widths, 2, 1);
            printTableRow(headers, widths, 2);
            printTableBorder(widths, 2, 2);
            const char *row1[] = {"1.", "ID"};
            const char *row2[] = {"2.", "Nama"};
            printTableRow(row1, widths, 2);
            printTableRow(row2, widths, 2);
            printTableBorder(widths, 2, 3);

            int choice;
            if (!readValidInt(&choice, ">>> Pilihan: "))
            {
                printError("Pilihan tidak valid!");
                continue;
            }

            printf("\n");

            if (choice == 1)
            {
                byId = true;
                if (!readValidString(query, sizeof(query), ">>> Masukkan nomor ID dokter: ", false))
                {
                    printError("Input ID tidak valid!");
                    continue;
                }

                printf("\n");
            }
            else if (choice == 2)
            {
                byId = false;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama dokter: "))
                {
                    printError("Input Nama tidak valid!");
                    continue;
                }

                printf("\n");
            }
            else
            {
                printError("Pilihan tidak valid!");
                continue;
            }
            findDoctor(&hospital, &session, query, byId);
        }

        else if (strcmp(command, "LIHATANTRIAN") == 0)
        {
            displayQueue(&hospital, &session);
        }

        else if (strcmp(command, "TAMBAHDOKTER") == 0)
        {
            char username[51], password[100], specialization[50];
            float checkupCost;

            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username dokter: ") ||
                !readValidString(password, 100, "Masukkan password dokter: ", false) ||
                !readStringWithSpaces(specialization, sizeof(specialization), "Masukkan spesialisasi dokter: ") ||
                !readValidFloat(&checkupCost, "Masukkan biaya checkup dokter: "))
            {
                printError("Input tidak valid!");
                continue;
            }
            addDoctor(&hospital, &session, username, password, specialization, checkupCost);
        }
        else if (strcmp(command, "ASSIGNDOKTER") == 0)
        {
            char username[51], roomCode[5];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username dokter: ") ||
                !readValidString(roomCode, 5, "Masukkan kode ruangan: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            assignDoctor(&hospital, &session, username, roomCode);
        }

        else if (strcmp(command, "DIAGNOSIS") == 0)
        {
            diagnosePatient(&hospital, &session);
        }

        else if (strcmp(command, "NGOBATIN") == 0)
        {
            treatPatient(&hospital, &session);
        }

        else if (strcmp(command, "PULANGDOK") == 0)
        {
            canGoHome(&hospital, &session);
        }

        else if (strcmp(command, "DAFTARCHECKUP") == 0)
        {
            float healthData[11]; // Misalnya data yang dimasukkan oleh pasien

            // Array prompt untuk masing-masing data kesehatan
            const char *prompts[] = {
                "Suhu Tubuh (Celcius)",
                "Tekanan Darah Sistol (mmHg)",
                "Tekanan Darah Diastol (mmHg)",
                "Detak Jantung (bpm)",
                "Saturasi Oksigen (%)",
                "Kadar Gula Darah (mg/dL)",
                "Berat Badan (kg)",
                "Tinggi Badan (cm)",
                "Kadar Kolestrol (mg/dL)",
                "Trombosit (ribu/µL)"};

            for (int i = 0; i < 10; i++)
            {
                while (true)
                {
                    char promp[100] = "";
                    strcat(promp, prompts[i]);
                    strcat(promp, ": ");

                    // Meminta input untuk masing-masing data kesehatan
                    if (!readValidFloat(&healthData[i], promp))
                    {
                        printError("Input tidak valid, silakan coba lagi!");
                        continue;
                    }

                    // Validasi jika input sesuai dengan rentang yang diinginkan
                    const float ranges[10][2] = {
                        {30.0, 45.0},   // bodyTemperature
                        {50.0, 200.0},  // systolicBloodPressure
                        {30.0, 120.0},  // diastolicBloodPressure
                        {40.0, 200.0},  // heartRate
                        {50.0, 100.0},  // oxygenSaturation
                        {50.0, 300.0},  // bloodSugarLevel
                        {20.0, 200.0},  // weight
                        {100.0, 250.0}, // height
                        {100.0, 400.0}, // cholesterolLevel
                        {100.0, 1000.0} // platelets
                    };

                    // Cek apakah data berada dalam rentang yang valid
                    if (healthData[i] < ranges[i][0] || healthData[i] > ranges[i][1])
                    {
                        char errorMsg[100] = "";
                        strcat(errorMsg, "Data kesehatan '");
                        strcat(errorMsg, prompts[i]);
                        strcat(errorMsg, "' tidak valid! Harap masukkan nilai dalam rentang yang benar.");
                        printError(errorMsg);
                        continue; // Jika data tidak valid, ulangi input
                    }
                    break; // Jika input valid, keluar dari loop
                }
            }

            registerCheckup(&hospital, &session, healthData);
        }

        else if (strcmp(command, "ANTRIANSAYA") == 0)
        {
            viewMySpecificQueueStatus(&hospital, &session);
        }

        else if (strcmp(command, "MINUMOBAT") == 0)
        {
            takeMedication(&hospital, &session);
        }

        else if (strcmp(command, "MINUMPENAWAR") == 0)
        {
            takeAntidote(&hospital, &session);
        }

        else if (strcmp(command, "EXIT") == 0)
        {
            exitProgram(&hospital, &session);
            return 0;
        }

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
            char username[51], roomCode[5];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username dokter: ") ||
                !readValidString(roomCode, 5, "Masukkan kode ruangan: ", false))
            {
                printError("Input tidak valid!");
                continue;
            }
            moveDoctor(&hospital, &session, username, roomCode);
        }

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

        else if (strcmp(command, "LEWATIANTRIAN") == 0)
        {
            char patientUsernameToCancel[51];
            if (session.role == PATIENT)
            {
                strcpy(patientUsernameToCancel, session.username);
            }
            else
            {
                while (!readUsernameWithTrim(patientUsernameToCancel, sizeof(patientUsernameToCancel), "Masukkan username pasien yang antriannya akan dilewati: "))
                {
                    printError("Input username tidak valid!");
                }
            }
            skipPatientInQueue(&hospital, &session, patientUsernameToCancel);
        }
        else if (strcmp(command, "BATALKANANTRIAN") == 0)
        {
            char patientUsernameToCancel[51];
            if (session.role == PATIENT)
            {
                strcpy(patientUsernameToCancel, session.username);
            }
            else
            {
                while (!readUsernameWithTrim(patientUsernameToCancel, sizeof(patientUsernameToCancel), "Masukkan username pasien yang antriannya akan dibatalkan: "))
                {
                    printError("Input username tidak valid!");
                }
            }

            cancelPatientFromQueue(&hospital, &session, patientUsernameToCancel);
        }

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