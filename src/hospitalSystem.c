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
    if (!initHospital(&hospital, 100, 100, 100, hospitalRows, hospitalCols)) {
        // initHospital already prints a specific error message
        printError("Inisialisasi rumah sakit gagal. Program dihentikan.");
        return 1; // Exit main with an error code
    }

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
            char username[51], password[100]; // username buffer 50 + 1 for null
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username: ") ||
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
            char username[51], password[100];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username: ") ||
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
            char query[51]; // Ensure buffer is adequate for readStringWithSpaces
            boolean by_id = false;
            printf("Cari berdasarkan?\n1. ID\n2. Nama\n");
            int choice;
            if (!readValidInt(&choice, ">>> Pilihan: ")) {
                printError("Pilihan tidak valid!");
                continue;
            }
            
            if (choice == 1) {
                by_id = true;
                if (!readValidString(query, sizeof(query), ">>> Masukkan nomor ID user: ", false)) {
                    printError("Input ID tidak valid!");
                    continue;
                }
            } else if (choice == 2) {
                by_id = false;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama user: ")) {
                    printError("Input Nama tidak valid!");
                    continue;
                }
            } else {
                printError("Pilihan tidak valid!");
                continue;
            }
            findUser(&hospital, &session, query, by_id);
        }
        else if (strcmp(command, "CARIPASIEN") == 0)
        {
            char query[51]; // Increased size for potential spaces + null terminator
            boolean by_id = false;
            boolean by_disease = false; // New flag

            printf("Cari berdasarkan?\n1. ID\n2. Nama\n3. Penyakit\n");
            int choice;
            if (!readValidInt(&choice, ">>> Pilihan: ")) {
                continue;
            }

            if (choice == 1) { // ID
                by_id = true;
                by_disease = false;
                if (!readValidString(query, sizeof(query), ">>> Masukkan nomor ID user: ", false)) { // Allow non-alphanumeric for ID if it's just numbers
                    printError("Input ID tidak valid!");
                    continue;
                }
            } else if (choice == 2) { // Nama
                by_id = false;
                by_disease = false;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama user: ")) { 
                    printError("Input Nama tidak valid!");
                    continue;
                }
            } else if (choice == 3) { // Penyakit
                by_id = false;
                by_disease = true;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama penyakit: ")) {
                    printError("Input Penyakit tidak valid!");
                    continue;
                }
            } else {
                printError("Pilihan tidak valid!");
                continue;
            }
            // Temporarily modify findPatient call signature, actual function will be changed later.
            findPatient(&hospital, &session, query, by_id, by_disease); 
        }
        else if (strcmp(command, "CARIDOKTER") == 0)
        {
            char query[51]; // Ensure buffer is adequate
            boolean by_id = false;
            printf("Cari berdasarkan?\n1. ID\n2. Nama\n");
            int choice;
            if (!readValidInt(&choice, ">>> Pilihan: ")) {
                printError("Pilihan tidak valid!");
                continue;
            }

            if (choice == 1) {
                by_id = true;
                if (!readValidString(query, sizeof(query), ">>> Masukkan nomor ID dokter: ", false)) {
                    printError("Input ID tidak valid!");
                    continue;
                }
            } else if (choice == 2) {
                by_id = false;
                if (!readStringWithSpaces(query, sizeof(query), ">>> Masukkan nama dokter: ")) {
                    printError("Input Nama tidak valid!");
                    continue;
                }
            } else {
                printError("Pilihan tidak valid!");
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
            char username[51], password[100], specialization[50];
            // Specialization can also use readStringWithSpaces if it can contain spaces
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username dokter: ") ||
                !readValidString(password, 100, "Masukkan password dokter: ", false) ||
                !readStringWithSpaces(specialization, sizeof(specialization), "Masukkan spesialisasi dokter: ")) // Assuming specialization can have spaces
            {
                printError("Input tidak valid!");
                continue;
            }
            addDoctor(&hospital, &session, username, password, specialization);
        }
        else if (strcmp(command, "ASSIGNDOKTER") == 0)
        {
            char username[51], room_code[5];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username dokter: ") ||
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
            char patient_username[51];
            if (!readUsernameWithTrim(patient_username, sizeof(patient_username), "Masukkan username pasien: "))
            {
                printError("Input username pasien tidak valid!"); // More generic error as trim handles format
                continue;
            }
            diagnosePatient(&hospital, &session, patient_username);
        }
        // F12 - Ngobatin
        else if (strcmp(command, "NGOBATIN") == 0)
        {
            char patient_username[51];
            if (!readUsernameWithTrim(patient_username, sizeof(patient_username), "Masukkan username pasien: "))
            {
                printError("Input username pasien tidak valid!");
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
            char doctor_username[51];
            float health_data[11];
            if (!readUsernameWithTrim(doctor_username, sizeof(doctor_username), "Masukkan username dokter: "))
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
            viewMySpecificQueueStatus(&hospital, &session);
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
            return 0; // Terminate main loop and program
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
            char username[51], room_code[5];
            if (!readUsernameWithTrim(username, sizeof(username), "Masukkan username dokter: ") ||
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
            char roomCode[5]; // Assuming room codes are like "A1", "B12" etc. Max 4 chars + null.
            if (!readValidString(roomCode, sizeof(roomCode), "Masukkan kode ruangan antrian yang akan di-skip: ", false)) {
                // readValidString prints its own error
                continue;
            }
            skipPatientInQueue(&hospital, &session, roomCode);
        }
        else if (strcmp(command, "CANCELANTRIAN") == 0)
        {
            char patientUsernameToCancel[51]; // Max username length 50 + null.
            // Use readUsernameWithTrim as usernames can have spaces and need trimming.
            if (!readUsernameWithTrim(patientUsernameToCancel, sizeof(patientUsernameToCancel), "Masukkan username pasien yang antriannya akan dibatalkan (atau 'saya' untuk diri sendiri): ")) {
                // readUsernameWithTrim prints its own error or handles empty input based on its impl.
                continue;
            }
            // If user types 'saya', use their own session username
            // Note: customCaseInsensitiveStrcmp should be available from utils.h
            if (customCaseInsensitiveStrcmp(patientUsernameToCancel, "saya") == 0) {
                 if (session.role == PATIENT) { // Make sure it's a patient session if 'saya' is used
                    strcpy(patientUsernameToCancel, session.username);
                 } else {
                    printError("Perintah 'saya' hanya valid untuk Pasien. Manajer harus memasukkan username pasien secara spesifik.");
                    continue;
                 }
            }
            cancelPatientFromQueue(&hospital, &session, patientUsernameToCancel);
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