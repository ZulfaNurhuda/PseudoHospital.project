#include "addDoctor.h"

boolean addDoctor(Hospital *hospital, Session *session, const char *inputUsername, const char *password, const char *specialization, const float checkupCost)
{
    // Pemeriksaan Awal: Validasi input pointer.
    // Memastikan bahwa pointer ke struktur Hospital, Session, dan string input tidak NULL.
    if (hospital == NULL || session == NULL || inputUsername == NULL || password == NULL || specialization == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau input tidak valid!"); // Pesan error jika ada input yang NULL.
        return false;                                                      // Mengembalikan false menandakan kegagalan.
    }

    // Pemeriksaan Hak Akses: Hanya Manajer yang dapat menambah dokter.
    // Memeriksa apakah pengguna saat ini sudah login (`session->isLoggedIn`)
    // dan apakah perannya adalah MANAGER (`session->role == MANAGER`).
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat menambah dokter."); // Pesan error jika hak akses tidak sesuai.
        return false;                                                           // Mengembalikan false.
    }

    // Validasi Username Dokter Baru.
    // Menggunakan fungsi `isValidUsername` (diasumsikan ada) untuk memeriksa format username.
    if (!isValidUsername(inputUsername))
    {
        printError("Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore."); // Pesan error jika username tidak valid.
        return false;                                                                                 // Mengembalikan false.
    }

    // Pemeriksaan Keunikan Username.
    // Menggunakan fungsi `isUsernameTaken` (diasumsikan ada) untuk memeriksa apakah username sudah digunakan.
    if (isUsernameTaken(hospital, inputUsername))
    {
        printError("Username sudah terdaftar!"); // Pesan error jika username sudah ada.
        return false;                            // Mengembalikan false.
    }

    // Validasi Panjang Password.
    // Password harus memiliki minimal 6 karakter.
    if (strlen(password) < 6)
    {
        printError("Password harus minimal 6 karakter!"); // Pesan error jika password terlalu pendek.
        return false;                                     // Mengembalikan false.
    }

    // Validasi Spesialisasi.
    // Menggunakan kembali fungsi `isValidUsername` untuk validasi spesialisasi,
    // karena diasumsikan memiliki batasan karakter yang sama.
    if (!isValidUsername(specialization))
    {
        printError("Spesialisasi tidak valid! Gunakan huruf, angka, atau underscore."); // Pesan error jika spesialisasi tidak valid.
        return false;                                                                   // Mengembalikan false.
    }

    // Pemeriksaan dan Penanganan Kapasitas Array Pengguna dan Dokter.
    // Jika jumlah pengguna efektif (`hospital->users.nEff`) atau jumlah dokter efektif (`hospital->doctors.nEff`)
    // sudah mencapai kapasitasnya masing-masing.
    if (hospital->users.nEff >= hospital->users.capacity || hospital->doctors.nEff >= hospital->doctors.capacity)
    {
        // Menggandakan kapasitas array pengguna.
        int newCapacity = hospital->users.capacity * 2;
        // Mencoba merealokasi memori untuk array pengguna dengan kapasitas baru.
        User *tempUsers = realloc(hospital->users.elements, newCapacity * sizeof(User));
        // Jika realokasi gagal (tempUsers adalah NULL).
        if (tempUsers == NULL)
        {
            printError("Gagal mengalokasi memori untuk ekspansi pengguna!"); // Pesan error.
            return false;                                                    // Mengembalikan false.
        }
        // Jika berhasil, update pointer elemen pengguna dan kapasitasnya.
        hospital->users.elements = tempUsers;
        hospital->users.capacity = newCapacity;

        // Menggandakan kapasitas array dokter.
        int newDoctorCapacity = hospital->doctors.capacity * 2;
        // Mencoba merealokasi memori untuk array dokter dengan kapasitas baru.
        Doctor *tempDoctors = realloc(hospital->doctors.elements, newDoctorCapacity * sizeof(Doctor));
        // Jika realokasi gagal.
        if (tempDoctors == NULL)
        {
            printError("Gagal mengalokasi memori untuk ekspansi dokter!"); // Pesan error.
            // Catatan: Sebaiknya, jika alokasi dokter gagal, alokasi pengguna yang mungkin sudah berhasil
            // tidak di-rollback di sini. Ini bisa menjadi potensi masalah jika salah satu berhasil dan lainnya gagal.
            return false; // Mengembalikan false.
        }
        // Jika berhasil, update pointer elemen dokter dan kapasitasnya.
        hospital->doctors.elements = tempDoctors;
        hospital->doctors.capacity = newDoctorCapacity;
    }

    // Menghasilkan ID unik untuk dokter baru.
    // Mencari ID maksimum yang sudah ada di antara semua pengguna.
    int maxId = 0;
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (hospital->users.elements[i].id > maxId)
        {
            maxId = hospital->users.elements[i].id;
        }
    }
    // ID dokter baru adalah ID maksimum yang ada ditambah satu.
    int newDoctorId = maxId + 1;

    // Menambahkan entri pengguna baru untuk dokter.
    // Mendapatkan pointer ke elemen berikutnya yang tersedia di array `hospital->users.elements`.
    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = newDoctorId;                // Menetapkan ID.
    strcpy(newUser->username, inputUsername); // Menyalin username.
    // Mengenkripsi password menggunakan fungsi `enigmaEncrypt` (diasumsikan ada).
    // `newUser->password.encryptedContent` adalah tempat menyimpan password terenkripsi.
    // 100 mungkin adalah panjang buffer maksimum untuk password terenkripsi.
    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password!"); // Pesan error jika enkripsi gagal.
        return false;                               // Mengembalikan false.
    }
    newUser->role = DOCTOR; // Menetapkan peran sebagai DOCTOR.

    // Menambahkan entri dokter baru.
    // Mendapatkan pointer ke elemen berikutnya yang tersedia di array `hospital->doctors.elements`.
    Doctor *newDoctor = &hospital->doctors.elements[hospital->doctors.nEff];
    newDoctor->id = newDoctorId;                       // Menetapkan ID (sama dengan ID pengguna).
    strcpy(newDoctor->username, inputUsername);        // Menyalin username.
    strcpy(newDoctor->specialization, specialization); // Menyalin spesialisasi.
    newDoctor->aura = 0;                               // Inisialisasi nilai aura dokter (misalnya, 0).
    newDoctor->bananaRich = 100.0f;                    // Inisialisasi nilai BananaRich dokter (misalnya, 100.0).
    newDoctor->room[0] = '\0';                         // Inisialisasi kode ruangan dokter sebagai string kosong (belum ditugaskan).
    newDoctor->checkupCost = checkupCost;              // Menetapkan biaya check

    // Meningkatkan jumlah pengguna efektif dan dokter efektif.
    hospital->users.nEff++;
    hospital->doctors.nEff++;

    // Membuat dan mencetak pesan sukses.
    char successMsg[100] = ""; // Buffer untuk pesan sukses, diinisialisasi string kosong.
    strcat(successMsg, "Dokter ");
    strcat(successMsg, inputUsername);
    strcat(successMsg, " berhasil ditambahkan!");
    printSuccess(successMsg); // Mencetak pesan sukses.
    return true;              // Mengembalikan true menandakan dokter berhasil ditambahkan.
}

boolean assignDoctor(Hospital *hospital, Session *session, const char *username, const char *roomCode)
{
    // Pemeriksaan Awal: Validasi input pointer.
    if (hospital == NULL || session == NULL || username == NULL || roomCode == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau input tidak valid!");
        return false;
    }

    // Pemeriksaan Hak Akses: Hanya Manajer yang dapat menugaskan dokter.
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat menugaskan dokter.");
        return false;
    }

    // Validasi Username Dokter.
    if (!isValidUsername(username))
    {
        printError("Username dokter tidak valid!");
        return false;
    }

    // Validasi Kode Ruangan.
    // Menggunakan fungsi `isValidRoomCode` (diasumsikan ada) untuk memeriksa format dan keberadaan kode ruangan.
    if (!isValidRoomCode(hospital, roomCode))
    {
        printError("Kode ruangan tidak valid! Contoh: A1");
        return false;
    }

    // Mencari Dokter berdasarkan Username.
    int doctorIdx = -1; // Indeks dokter dalam array `hospital->doctors.elements`. Diinisialisasi -1 (tidak ditemukan).
    // Melakukan iterasi melalui daftar dokter.
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        // Membandingkan username input dengan username dokter (case-insensitive).
        // `customCaseInsensitiveStrcmp` adalah fungsi perbandingan string kustom yang tidak membedakan huruf besar/kecil.
        if (customCaseInsensitiveStrcmp(hospital->doctors.elements[i].username, username) == 0)
        {
            doctorIdx = i; // Jika ditemukan, simpan indeksnya.
            break;         // Keluar dari loop.
        }
    }
    // Jika dokter tidak ditemukan setelah iterasi.
    if (doctorIdx == -1)
    {
        printError("Dokter tidak ditemukan!");
        return false;
    }

    // Mencari Ruangan berdasarkan Kode Ruangan.
    Room *room = NULL; // Pointer ke struktur Room. Diinisialisasi NULL.
    // Memastikan denah memiliki dimensi yang valid sebelum mencari.
    if (hospital->layout.rowEff > 0 && hospital->layout.colEff > 0)
    {
        // Melakukan iterasi melalui denah.
        for (int i = 0; i < hospital->layout.rowEff; i++)
        {
            for (int j = 0; j < hospital->layout.colEff; j++)
            {
                // Membandingkan kode ruangan di denah dengan kode ruangan input.
                if (strcmp(hospital->layout.elements[i][j].code, roomCode) == 0)
                {
                    room = &hospital->layout.elements[i][j]; // Jika ditemukan, simpan alamatnya.
                    break;                                   // Keluar dari loop kolom.
                }
            }
            if (room != NULL)
                break; // Jika ditemukan di loop kolom, keluar juga dari loop baris.
        }
    }

    // Jika ruangan tidak ditemukan setelah iterasi.
    if (room == NULL)
    {
        printError("Ruangan tidak ditemukan!");
        return false;
    }

    // Mendapatkan pointer ke struktur dokter yang akan ditugaskan.
    Doctor *doctor = &hospital->doctors.elements[doctorIdx];

    // Pemeriksaan Kondisi Penugasan.
    // Cek apakah dokter yang akan ditugaskan sudah memiliki ruangan (sudah di-assign ke ruangan lain).
    // `doctor->room[0] != '\0'` berarti string `doctor->room` tidak kosong.
    boolean doctorAlreadyAssigned = (doctor->room[0] != '\0');

    // Cek apakah ruangan target sudah ditempati oleh dokter lain.
    // `room->doctorId != -1` berarti ada ID dokter yang valid di ruangan tersebut (-1 menandakan kosong).
    boolean roomOccupied = (room->doctorId != -1);

    // Skenario Konflik 1 (Test Case 4 dari deskripsi): Dokter sudah di-assign DAN ruangan target sudah ditempati.
    // Ini adalah kasus paling restriktif.
    if (doctorAlreadyAssigned && roomOccupied)
    {
        // Pesan error pertama: memberitahu bahwa dokter X sudah di ruangan Y.
        char errorMsg1[100] = "";
        strcat(errorMsg1, "Dokter ");
        strcat(errorMsg1, doctor->username);
        strcat(errorMsg1, " sudah menempati ruangan ");
        strcat(errorMsg1, doctor->room); // Menampilkan kode ruangan dokter saat ini.
        strcat(errorMsg1, "!");
        printError(errorMsg1);

        // Mencari nama dokter yang saat ini menempati ruangan target.
        char occupyingDoctorName[50] = ""; // Buffer untuk nama dokter yang menempati.
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            if (hospital->doctors.elements[i].id == room->doctorId) // `room->doctorId` adalah ID dokter di ruangan target.
            {
                strcpy(occupyingDoctorName, hospital->doctors.elements[i].username);
                break;
            }
        }

        // Pesan error kedua: memberitahu bahwa ruangan target Z sudah ditempati dokter A.
        char errorMsg2[100] = "";
        strcat(errorMsg2, "Ruangan ");
        strcat(errorMsg2, roomCode); // Kode ruangan target.
        strcat(errorMsg2, " juga sudah ditempati dokter ");
        strcat(errorMsg2, occupyingDoctorName); // Nama dokter yang menempati ruangan target.
        strcat(errorMsg2, "!");
        printError(errorMsg2);
        return false; // Penugasan gagal.
    }

    // Skenario Konflik 2 (Test Case 2 dari deskripsi): Dokter sudah di-assign ke ruangan lain (tapi ruangan target mungkin kosong).
    // Kondisi ini diperiksa setelah kondisi gabungan di atas.
    if (doctorAlreadyAssigned)
    {
        char errorMsg[100] = "";
        strcat(errorMsg, "Dokter ");
        strcat(errorMsg, doctor->username);
        strcat(errorMsg, " sudah diassign ke ruangan ");
        strcat(errorMsg, doctor->room);
        strcat(errorMsg, "!");
        printError(errorMsg);
        return false; // Penugasan gagal.
    }

    // Skenario Konflik 3 (Test Case 3 dari deskripsi): Ruangan target sudah ditempati dokter lain (tapi dokter yang akan di-assign mungkin belum punya ruangan).
    // Kondisi ini diperiksa setelah kondisi gabungan di atas.
    if (roomOccupied)
    {
        // Mencari nama dokter yang saat ini menempati ruangan target.
        char occupyingDoctorName[50] = "";
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            if (hospital->doctors.elements[i].id == room->doctorId)
            {
                strcpy(occupyingDoctorName, hospital->doctors.elements[i].username);
                break;
            }
        }

        // Pesan error pertama: memberitahu bahwa dokter A sudah di ruangan Z.
        char errorMsg1[100] = "";
        strcat(errorMsg1, "Dokter ");
        strcat(errorMsg1, occupyingDoctorName);
        strcat(errorMsg1, " sudah menempati ruangan ");
        strcat(errorMsg1, roomCode);
        strcat(errorMsg1, "!");
        printError(errorMsg1);

        // Pesan error kedua: saran untuk mencari ruangan lain.
        char errorMsg2[100] = "";
        strcat(errorMsg2, "Silakan cari ruangan lain untuk dokter ");
        strcat(errorMsg2, doctor->username); // Dokter yang ingin ditugaskan.
        strcat(errorMsg2, ".");
        printError(errorMsg2);
        return false; // Penugasan gagal.
    }

    // Skenario Sukses (Test Case 1 dari deskripsi): Dokter belum di-assign DAN ruangan target kosong.
    // Jika semua pemeriksaan konflik di atas tidak terpenuhi, maka penugasan dapat dilakukan.
    room->doctorId = doctor->id;    // Menetapkan ID dokter ke ruangan.
    strcpy(doctor->room, roomCode); // Menetapkan kode ruangan ke data dokter.

    // Membuat dan mencetak pesan sukses.
    char successMsg[100] = "";
    strcat(successMsg, "Dokter ");
    strcat(successMsg, doctor->username);
    strcat(successMsg, " berhasil diassign ke ruangan ");
    strcat(successMsg, roomCode);
    strcat(successMsg, "!");
    printSuccess(successMsg);
    return true; // Mengembalikan true menandakan penugasan berhasil.
}