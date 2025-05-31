#include "hospital.h"

boolean initHospital(Hospital *hospital, int userCapacity, int patientCapacity, int doctorCapacity, int roomRows, int roomCols)
{
    // Validasi input: pointer hospital tidak boleh NULL.
    if (hospital == NULL)
    {
        printError("Pointer hospital tidak valid!");
        return false; // Mengembalikan false jika pointer tidak valid.
    }
    // Validasi input: semua kapasitas dan dimensi denah harus positif.
    if (userCapacity <= 0 || patientCapacity <= 0 || doctorCapacity <= 0 || roomRows <= 0 || roomCols <= 0)
    {
        printError("Kapasitas atau ukuran denah tidak valid!");
        return false; // Mengembalikan false jika ada nilai yang tidak valid.
    }

    // Inisialisasi semua pointer elemen dalam struktur Hospital ke NULL.
    // Ini adalah praktik yang baik untuk memudahkan logika pembersihan (cleanup) jika terjadi kegagalan alokasi di tengah jalan.
    // Jika pointer NULL, free() tidak akan melakukan apa-apa, mencegah double free atau free pada memori yang tidak dialokasikan.
    hospital->users.elements = NULL;
    hospital->patients.elements = NULL;
    hospital->doctors.elements = NULL;
    hospital->layout.elements = NULL;
    hospital->queues.queues = NULL;
    hospital->diseases.elements = NULL;
    hospital->medications.elements = NULL;
    hospital->prescriptions.elements = NULL;
    hospital->treatmentHistory.elements = NULL;

    // Inisialisasi UserSet (daftar semua pengguna sistem).
    // Mengalokasikan memori untuk array pengguna. `safeMalloc` adalah pembungkus malloc yang mungkin menangani error.
    hospital->users.elements = (User *)safeMalloc(userCapacity * sizeof(User));
    // Jika alokasi memori gagal.
    if (hospital->users.elements == NULL)
    {
        printError("Gagal alokasi memori untuk UserSet!");
        return false; // Tidak ada alokasi sebelumnya dalam fungsi ini yang perlu dibebaskan pada titik ini.
    }
    hospital->users.capacity = userCapacity; // Set kapasitas.
    hospital->users.nEff = 0;                // Jumlah pengguna efektif awal adalah 0.

    // Inisialisasi PatientList (daftar semua pasien).
    hospital->patients.elements = (Patient *)safeMalloc(patientCapacity * sizeof(Patient));
    if (hospital->patients.elements == NULL)
    {
        printError("Gagal alokasi memori untuk PatientList!");
        // Jika gagal, bebaskan memori yang sudah dialokasikan untuk users.elements.
        free(hospital->users.elements);
        hospital->users.elements = NULL; // Set ke NULL setelah free.
        return false;
    }
    hospital->patients.capacity = patientCapacity; // Set kapasitas.
    hospital->patients.nEff = 0;                   // Jumlah pasien efektif awal adalah 0.

    // Inisialisasi DoctorList (daftar semua dokter).
    hospital->doctors.elements = (Doctor *)safeMalloc(doctorCapacity * sizeof(Doctor));
    if (hospital->doctors.elements == NULL)
    {
        printError("Gagal alokasi memori untuk DoctorList!");
        // Jika gagal, bebaskan memori yang sudah dialokasikan untuk users dan patients.
        free(hospital->users.elements);
        hospital->users.elements = NULL;
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
        return false;
    }
    hospital->doctors.capacity = doctorCapacity; // Set kapasitas.
    hospital->doctors.nEff = 0;                  // Jumlah dokter efektif awal adalah 0.

    // Inisialisasi Layout (denah ruangan rumah sakit).
    // `hospital->layout.elements` adalah array pointer ke Room (merepresentasikan array 2D).
    // Pertama, alokasikan memori untuk array baris (setiap elemen adalah pointer ke array Room untuk kolom).
    hospital->layout.elements = (Room **)safeMalloc(roomRows * sizeof(Room *));
    if (hospital->layout.elements == NULL)
    {
        printError("Gagal alokasi memori untuk Layout (rows)!");
        // Jika gagal, bebaskan semua memori yang sudah dialokasikan sebelumnya untuk list lain.
        free(hospital->users.elements);
        hospital->users.elements = NULL;
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
        return false;
    }
    // Inisialisasi setiap pointer baris dalam `layout.elements` ke NULL.
    // Ini penting untuk logika cleanup jika alokasi untuk kolom-kolom nanti gagal sebagian.
    for (int i = 0; i < roomRows; i++)
    {
        hospital->layout.elements[i] = NULL;
    }

    // Alokasikan memori untuk setiap baris (yaitu, array kolom yang berisi struktur Room).
    for (int i = 0; i < roomRows; i++) // Loop untuk setiap baris.
    {
        hospital->layout.elements[i] = (Room *)safeMalloc(roomCols * sizeof(Room));
        // Jika alokasi untuk satu baris (array kolom) gagal.
        if (hospital->layout.elements[i] == NULL)
        {
            printError("Gagal alokasi memori untuk Layout (cols)!");
            // Lakukan cleanup untuk baris-baris yang sudah berhasil dialokasikan sebelumnya.
            for (int k = 0; k < i; k++)
            { // Loop hingga baris sebelum baris yang gagal (indeks i).
              // Di dalam setiap kolom dari baris k yang sudah dialokasikan,
              // perlu membebaskan `patientInRoom.patientId` jika sudah dialokasikan.
              // Ini karena `patientInRoom.patientId` dialokasikan per ruangan.
                for (int l = 0; l < roomCols; l++)
                {
                    // Memeriksa apakah elemen baris k tidak NULL (sudah dialokasikan)
                    // dan patientId di kolom l juga tidak NULL (sudah dialokasikan).
                    if (hospital->layout.elements[k] != NULL && hospital->layout.elements[k][l].patientInRoom.patientId != NULL)
                    {
                        free(hospital->layout.elements[k][l].patientInRoom.patientId);
                        // Sebaiknya set ke NULL setelah free: hospital->layout.elements[k][l].patientInRoom.patientId = NULL; (tidak ada di skrip asli)
                    }
                }
                free(hospital->layout.elements[k]); // Bebaskan memori untuk array kolom pada baris k.
                                                    // Sebaiknya set ke NULL: hospital->layout.elements[k] = NULL; (tidak ada di skrip asli)
            }
            // Bebaskan array pointer baris itu sendiri.
            free(hospital->layout.elements);
            hospital->layout.elements = NULL;
            // Bebaskan semua alokasi tingkat atas sebelumnya.
            free(hospital->users.elements);
            hospital->users.elements = NULL;
            free(hospital->patients.elements);
            hospital->patients.elements = NULL;
            free(hospital->doctors.elements);
            hospital->doctors.elements = NULL;
            return false; // Inisialisasi gagal.
        }
        // Inisialisasi pointer `patientInRoom.patientId` ke NULL untuk setiap ruangan di baris yang baru dialokasikan.
        // Ini penting sebagai penanda sebelum `patientInRoom.patientId` benar-benar dialokasikan di loop berikutnya.
        for (int j = 0; j < roomCols; j++)
        {
            hospital->layout.elements[i][j].patientInRoom.patientId = NULL;
        }

        // Inisialisasi setiap struktur Room dalam baris saat ini.
        for (int j = 0; j < roomCols; j++) // Loop untuk setiap kolom dalam baris i.
        {
            // Membuat kode ruangan (misalnya, A1, A2, B1, dst.).
            char code[5] = "";   // Buffer untuk kode ruangan, diinisialisasi string kosong.
            code[0] = 'A' + i;   // Karakter pertama adalah huruf berdasarkan indeks baris (A untuk i=0, B untuk i=1, ...).
            char colStr[4] = ""; // Buffer untuk representasi string nomor kolom.
            int col = j + 1;     // Nomor kolom dimulai dari 1 (bukan 0).
            int index = 0;       // Indeks untuk membangun string nomor kolom (colStr).
            // Mengkonversi nomor kolom (integer) menjadi string, digit per digit, dalam urutan terbalik.
            // Contoh: jika col = 12, colStr akan menjadi "21" (sementara).
            if (col == 0)
            { // Menangani kasus jika col adalah 0 (seharusnya tidak terjadi karena j+1).
                colStr[index++] = '0';
            }
            else
            {
                while (col > 0)
                {
                    colStr[index++] = (col % 10) + '0'; // Ambil digit terakhir, konversi ke char, tambahkan ke colStr.
                    col /= 10;                          // Hapus digit terakhir dari col.
                }
            }
            // colStr[index] = '\0'; // Null terminator untuk colStr (tidak ada di skrip asli, tapi implisit dari loop berikutnya yang menggunakan strlen pada `code`).

            // Membalik string nomor kolom (colStr) dan menggabungkannya ke `code`.
            // Loop dari digit terakhir colStr (index-1) ke digit pertama (0).
            for (int k = index - 1; k >= 0; k--)
            {
                // Baris `strncat(code, &colStr[k], 1);` yang mungkin ada sebelumnya diganti dengan implementasi manual ini
                // untuk memastikan tidak ada buffer overflow pada `code`.
                int currentLen = strlen(code); // Dapatkan panjang `code` saat ini.
                // Pastikan ada cukup ruang di buffer `code` (ukuran 5, jadi maksimal 4 karakter + null terminator).
                if (currentLen < sizeof(code) - 1)
                {                                 // sizeof(code) - 1 adalah ruang maksimal untuk karakter.
                    code[currentLen] = colStr[k]; // Tambahkan digit dari colStr.
                    code[currentLen + 1] = '\0';  // Tambahkan null terminator baru setelah digit ditambahkan.
                }
            }
            // Menyalin kode ruangan yang sudah jadi ke struktur Room.
            // Diasumsikan `hospital->layout.elements[i][j].code` memiliki ukuran yang cukup (misalnya, 5).
            strcpy(hospital->layout.elements[i][j].code, code);
            hospital->layout.elements[i][j].capacity = 3;  // Set kapasitas default ruangan menjadi 3.
            hospital->layout.elements[i][j].doctorId = -1; // Awalnya tidak ada dokter di ruangan (-1 menandakan kosong).

            // Alokasi memori untuk array ID pasien di dalam ruangan (`patientInRoom.patientId`).
            // Kapasitas default 3 pasien per ruangan.
            hospital->layout.elements[i][j].patientInRoom.patientId = (int *)safeMalloc(3 * sizeof(int));
            // Jika alokasi memori untuk `patientId` gagal.
            if (hospital->layout.elements[i][j].patientInRoom.patientId == NULL)
            {
                printError("Gagal alokasi memori untuk patientInRoom.patientId!");
                // Cleanup yang rumit karena kegagalan terjadi di tengah inisialisasi layout:
                // 1. Bebaskan `patientId` yang sudah dialokasikan di kolom-kolom sebelumnya pada baris saat ini (baris i).
                for (int m = 0; m < j; m++)
                { // Loop hingga kolom sebelum kolom yang gagal (indeks j).
                    if (hospital->layout.elements[i][m].patientInRoom.patientId != NULL)
                    {
                        free(hospital->layout.elements[i][m].patientInRoom.patientId);
                        // hospital->layout.elements[i][m].patientInRoom.patientId = NULL; // Sebaiknya ditambahkan
                    }
                }
                // 2. Bebaskan memori untuk baris saat ini (baris i) yang alokasi kolomnya gagal sebagian.
                free(hospital->layout.elements[i]);
                // hospital->layout.elements[i] = NULL; // Sebaiknya ditambahkan

                // 3. Bebaskan semua baris yang sudah dialokasikan penuh sebelumnya (dari baris 0 hingga i-1).
                for (int k = 0; k < i; k++)
                { // Loop untuk setiap baris yang sudah dialokasikan penuh.
                  // Untuk setiap baris k, bebaskan `patientId` di setiap kolomnya.
                    for (int l = 0; l < roomCols; l++)
                    {
                        if (hospital->layout.elements[k] != NULL && hospital->layout.elements[k][l].patientInRoom.patientId != NULL)
                        {
                            free(hospital->layout.elements[k][l].patientInRoom.patientId);
                            // hospital->layout.elements[k][l].patientInRoom.patientId = NULL; // Sebaiknya
                        }
                    }
                    free(hospital->layout.elements[k]); // Bebaskan memori untuk array kolom pada baris k.
                                                        // hospital->layout.elements[k] = NULL; // Sebaiknya
                }
                // 4. Bebaskan array pointer baris utama (`layout.elements`) dan semua alokasi tingkat atas lainnya.
                free(hospital->layout.elements);
                hospital->layout.elements = NULL;
                free(hospital->users.elements);
                hospital->users.elements = NULL;
                free(hospital->patients.elements);
                hospital->patients.elements = NULL;
                free(hospital->doctors.elements);
                hospital->doctors.elements = NULL;
                return false; // Inisialisasi gagal.
            }
            hospital->layout.elements[i][j].patientInRoom.capacity = 3; // Set kapasitas pasien di ruangan.
            hospital->layout.elements[i][j].patientInRoom.nEff = 0;     // Jumlah pasien efektif awal di ruangan adalah 0.
        }
    }
    // Set kapasitas dan jumlah efektif baris/kolom untuk keseluruhan denah.
    hospital->layout.rowCapacity = roomRows;
    hospital->layout.colCapacity = roomCols;
    hospital->layout.rowEff = roomRows;
    hospital->layout.colEff = roomCols;

    // Inisialisasi HospitalQueueList (daftar antrian untuk setiap ruangan).
    // Mengalokasikan memori untuk array struktur Queue. Jumlah maksimum antrian = jumlah total ruangan.
    hospital->queues.queues = (Queue *)safeMalloc(roomRows * roomCols * sizeof(Queue));
    if (hospital->queues.queues == NULL)
    {
        printError("Gagal alokasi memori untuk HospitalQueueList!");
        // Cleanup penuh untuk semua yang sudah dialokasikan, dimulai dari layout yang paling dalam.
        for (int i = 0; i < roomRows; i++)
        { // Loop melalui setiap baris yang mungkin sudah dialokasikan.
            if (hospital->layout.elements[i] != NULL)
            { // Periksa apakah pointer baris i valid.
                for (int j = 0; j < roomCols; j++)
                { // Loop melalui setiap kolom.
                  // Periksa apakah `patientId` di dalam `Room [i][j]` sudah dialokasikan.
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]); // Bebaskan array kolom untuk baris i.
            }
        }
        free(hospital->layout.elements);
        hospital->layout.elements = NULL; // Bebaskan array pointer baris.
        // Bebaskan list-list lainnya.
        free(hospital->users.elements);
        hospital->users.elements = NULL;
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
        return false; // Inisialisasi gagal.
    }
    hospital->queues.capacity = roomRows * roomCols; // Kapasitas total slot antrian yang dialokasikan.
    hospital->queues.nRooms = 0;                     // Awalnya, jumlah ruangan yang memiliki antrian aktif (dan terdaftar di sini) adalah 0.
    // Inisialisasi setiap antrian dalam array `hospital->queues.queues`.
    // Perubahan: Menggunakan fungsi `initializeQueue` untuk inisialisasi setiap antrian.
    for (int i = 0; i < hospital->queues.capacity; i++)
    { // Loop hingga kapasitas yang dialokasikan.
        // Memanggil fungsi `initializeQueue` untuk menginisialisasi antrian ke-i.
        // Argumen kedua "" mungkin adalah roomCode default atau placeholder.
        initializeQueue(&hospital->queues.queues[i], "");
    }

    // Inisialisasi DiseaseList (daftar penyakit yang diketahui sistem).
    // Mengalokasikan memori untuk 5 penyakit (jumlah ini di-hardcode).
    hospital->diseases.elements = (Disease *)safeMalloc(5 * sizeof(Disease));
    if (hospital->diseases.elements == NULL)
    {
        printError("Gagal alokasi memori untuk DiseaseList!");
        // Cleanup penuh untuk semua yang sudah dialokasikan sebelumnya.
        free(hospital->queues.queues); // Bebaskan array antrian yang baru saja mungkin berhasil dialokasikan.
        hospital->queues.queues = NULL;
        // Cleanup untuk layout (disalin dari atas, idealnya menggunakan fungsi helper untuk menghindari duplikasi kode).
        for (int i = 0; i < roomRows; i++)
        {
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL)
            {
                for (int j = 0; j < roomCols; j++)
                {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        if (hospital->layout.elements != NULL)
        {
            free(hospital->layout.elements);
            hospital->layout.elements = NULL;
        }
        free(hospital->users.elements);
        hospital->users.elements = NULL;
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
        return false; // Inisialisasi gagal.
    }
    hospital->diseases.capacity = 5; // Kapasitas daftar penyakit.
    hospital->diseases.nEff = 5;     // Jumlah penyakit efektif (semua 5 penyakit akan diisi).
    // Data penyakit yang di-hardcode. Setiap baris adalah satu struktur Disease.
    Disease diseases[] = {
        {1, "Influenza", 36.0, 38.5, 90, 120, 60, 80, 60, 100, 95.0, 100.0, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {5, "Anemia", 36.0, 37.0, 90, 120, 60, 80, 60, 100, 95.0, 100.0, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450}, // ID 5 untuk Anemia, perhatikan urutan ID tidak harus berurutan di array ini.
        {2, "COVID-19", 36.0, 39.0, 90, 130, 60, 85, 60, 120, 90.0, 100.0, 70.0, 180.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {3, "Hipertensi", 36.0, 37.5, 140, 180, 90, 120, 60, 100, 90.0, 100.0, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {4, "Diabetes Mellitus", 36.0, 37.5, 90, 140, 60, 90, 60, 100, 90.0, 100.0, 126.0, 200.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450}};
    // Menyalin data penyakit dari array lokal `diseases` ke `hospital->diseases.elements`.
    for (int i = 0; i < hospital->diseases.nEff; i++)
    {
        hospital->diseases.elements[i] = diseases[i]; // Menyalin seluruh isi struktur Disease.
        // `strcpy` di sini penting untuk menyalin string `name` dengan benar,
        // karena `diseases[i].name` adalah pointer ke string literal (konstan),
        // sedangkan `hospital->diseases.elements[i].name` adalah array char yang bisa dimodifikasi.
        strcpy(hospital->diseases.elements[i].name, diseases[i].name);
    }

    // Inisialisasi MedicationList (daftar obat yang tersedia).
    // Mengalokasikan memori untuk 5 obat (jumlah di-hardcode).
    hospital->medications.elements = (Medication *)safeMalloc(5 * sizeof(Medication));
    if (hospital->medications.elements == NULL)
    {
        printError("Gagal alokasi memori untuk MedicationList!");
        // Cleanup penuh.
        free(hospital->diseases.elements);
        hospital->diseases.elements = NULL;
        free(hospital->queues.queues);
        hospital->queues.queues = NULL;
        for (int i = 0; i < roomRows; i++)
        { /* ... cleanup layout ... */
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL)
            {
                for (int j = 0; j < roomCols; j++)
                {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        if (hospital->layout.elements != NULL)
        {
            free(hospital->layout.elements);
            hospital->layout.elements = NULL;
        }
        free(hospital->users.elements);
        hospital->users.elements = NULL;
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
        return false; // Inisialisasi gagal.
    }
    hospital->medications.capacity = 5; // Kapasitas daftar obat.
    hospital->medications.nEff = 5;     // Jumlah obat efektif.
    // Data obat yang di-hardcode (ID obat, Nama obat).
    Medication medications[] = {
        {2, "Vitamin C"}, {1, "Oseltamivir"}, {4, "Lisinopril"}, {5, "Metformin"}, {3, "Remdesivir"}};
    // Menyalin data obat.
    for (int i = 0; i < hospital->medications.nEff; i++)
    {
        hospital->medications.elements[i] = medications[i];                  // Salin struktur.
        strcpy(hospital->medications.elements[i].name, medications[i].name); // Salin nama obat.
    }

    // Inisialisasi PrescriptionList (daftar resep standar: pemetaan penyakit ke obat dan kuantitas).
    // Mengalokasikan memori untuk 10 entri resep (di-hardcode).
    hospital->prescriptions.elements = (MedicationPrescription *)safeMalloc(10 * sizeof(MedicationPrescription));
    if (hospital->prescriptions.elements == NULL)
    {
        printError("Gagal alokasi memori untuk PrescriptionList!");
        // Cleanup penuh.
        free(hospital->medications.elements);
        hospital->medications.elements = NULL;
        free(hospital->diseases.elements);
        hospital->diseases.elements = NULL;
        free(hospital->queues.queues);
        hospital->queues.queues = NULL;
        for (int i = 0; i < roomRows; i++)
        { /* ... cleanup layout ... */
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL)
            {
                for (int j = 0; j < roomCols; j++)
                {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        if (hospital->layout.elements != NULL)
        {
            free(hospital->layout.elements);
            hospital->layout.elements = NULL;
        }
        free(hospital->users.elements);
        hospital->users.elements = NULL;
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
        return false; // Inisialisasi gagal.
    }
    hospital->prescriptions.capacity = 10; // Kapasitas daftar resep.
    hospital->prescriptions.nEff = 10;     // Jumlah resep efektif.
    // Data resep yang di-hardcode (diseaseId, medicationId, quantity).
    MedicationPrescription prescriptions[] = {
        {1, 1, 1}, {3, 2, 1}, {2, 1, 2}, {4, 4, 2}, {4, 3, 1}, {5, 4, 1}, {2, 5, 2}, {3, 4, 3}, {2, 4, 4}, {3, 5, 1}};
    // Menyalin data resep.
    for (int i = 0; i < hospital->prescriptions.nEff; i++)
    {
        hospital->prescriptions.elements[i] = prescriptions[i]; // Salin struktur.
    }

    // Inisialisasi HospitalFinance (data keuangan rumah sakit).
    hospital->finance.hospitalBalance = 10000.0; // Saldo awal rumah sakit di-hardcode.

    // Inisialisasi TreatmentHistoryList (daftar riwayat pengobatan pasien).
    // Mengalokasikan memori untuk 100 entri riwayat (di-hardcode).
    hospital->treatmentHistory.elements = (TreatmentHistory *)safeMalloc(100 * sizeof(TreatmentHistory));
    if (hospital->treatmentHistory.elements == NULL)
    {
        printError("Gagal alokasi memori untuk TreatmentHistoryList!");
        // Cleanup penuh.
        free(hospital->prescriptions.elements);
        hospital->prescriptions.elements = NULL;
        free(hospital->medications.elements);
        hospital->medications.elements = NULL;
        free(hospital->diseases.elements);
        hospital->diseases.elements = NULL;
        free(hospital->queues.queues);
        hospital->queues.queues = NULL;
        for (int i = 0; i < roomRows; i++)
        { /* ... cleanup layout ... */
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL)
            {
                for (int j = 0; j < roomCols; j++)
                {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        if (hospital->layout.elements != NULL)
        {
            free(hospital->layout.elements);
            hospital->layout.elements = NULL;
        }
        free(hospital->users.elements);
        hospital->users.elements = NULL;
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
        return false; // Inisialisasi gagal.
    }
    hospital->treatmentHistory.capacity = 100; // Kapasitas daftar riwayat.
    hospital->treatmentHistory.nEff = 0;       // Awalnya tidak ada riwayat pengobatan.

    // Menambahkan pengguna default (Manajer) secara manual setelah semua inisialisasi dasar selesai.
    // Ini adalah perubahan dari versi sebelumnya dimana user default di-init di main.
    // Pastikan `hospital->users.elements` sudah dialokasikan dan `hospital->users.nEff` masih < `hospital->users.capacity`.
    // Karena `userCapacity` bisa saja kecil, penambahan ini harus aman.
    // Jika `userCapacity` adalah 0 (dicegah di awal), atau jika `nEff` sudah sama dengan `capacity` sebelum ini,
    // maka ini bisa menjadi masalah. Namun, `nEff` diinisialisasi 0.
    hospital->users.elements[0].id = 1;                         // Menetapkan ID pengguna default.
    strcpy(hospital->users.elements[0].username, "NimonsDawg"); // Menetapkan username.
                                                                // Mengenkripsi password pengguna default. `enigmaEncrypt` adalah fungsi kustom.
                                                                // 100 adalah panjang buffer untuk password terenkripsi.
    if (!enigmaEncrypt("admoontothemoon", hospital->users.elements[0].password.encryptedContent, 100))
    {
        printError("Gagal mengenkripsi password manager default!"); // Pesan error spesifik.
        // PERBAIKAN: Jika enkripsi gagal, panggil freeHospital untuk membersihkan semua alokasi
        // dan kembalikan false untuk menandakan kegagalan inisialisasi.
        freeHospital(hospital); // Memanggil fungsi cleanup.
        return false;           // Mengembalikan false.
    }
    hospital->users.elements[0].role = MANAGER; // Menetapkan peran sebagai Manajer.
    hospital->users.nEff++;                     // Menambah jumlah pengguna efektif menjadi 1.

    // Jika semua inisialisasi (termasuk user default) berhasil.
    printSuccess("Rumah sakit berhasil diinisialisasi!");
    return true; // Mengembalikan true menandakan inisialisasi berhasil.
}

void freeHospital(Hospital *hospital)
{
    // Membebaskan memori untuk array pengguna (UserSet).
    if (hospital->users.elements) // Periksa apakah pointer tidak NULL sebelum memanggil free.
    {
        free(hospital->users.elements);
        hospital->users.elements = NULL; // Set pointer ke NULL setelah free untuk mencegah dangling pointer dan double free.
    }

    // Membebaskan memori untuk array pasien (PatientList).
    if (hospital->patients.elements)
    {
        // Sebelum membebaskan array `hospital->patients.elements` itu sendiri,
        // perlu membebaskan memori yang dialokasikan secara dinamis di dalam setiap elemen Patient,
        // yaitu `medicationsPrescribed.medicationId` dan `medicationsTaken.medicationId`.
        for (int i = 0; i < hospital->patients.nEff; i++)
        { // Loop melalui pasien yang efektif.
          // Bebaskan daftar obat yang diresepkan untuk pasien i.
            if (hospital->patients.elements[i].medicationsPrescribed.medicationId != NULL)
            {
                free(hospital->patients.elements[i].medicationsPrescribed.medicationId);
                hospital->patients.elements[i].medicationsPrescribed.medicationId = NULL;
            }
            // Bebaskan daftar obat yang sudah diminum oleh pasien i.
            if (hospital->patients.elements[i].medicationsTaken.medicationId != NULL)
            {
                free(hospital->patients.elements[i].medicationsTaken.medicationId);
                hospital->patients.elements[i].medicationsTaken.medicationId = NULL;
            }
        }
        // Setelah semua alokasi internal dalam setiap Patient dibebaskan, bebaskan array Patient itu sendiri.
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
    }

    // Membebaskan memori untuk array dokter (DoctorList).
    if (hospital->doctors.elements)
    {
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
    }

    // Membebaskan memori untuk denah layout (array 2D Room).
    if (hospital->layout.elements) // Periksa apakah array pointer baris dialokasikan.
    {
        // Loop melalui setiap baris denah berdasarkan kapasitas alokasi (`rowCapacity`).
        for (int i = 0; i < hospital->layout.rowCapacity; i++)
        {
            // Periksa apakah pointer untuk baris ke-i tidak NULL (artinya baris ini dialokasikan).
            if (hospital->layout.elements[i])
            {
                // Loop melalui setiap kolom dalam baris ke-i untuk membebaskan `patientInRoom.patientId`.
                for (int j = 0; j < hospital->layout.colCapacity; j++) // Gunakan `colCapacity`.
                {
                    // Periksa apakah pointer `patientId` di dalam `Room [i][j]` tidak NULL.
                    if (hospital->layout.elements[i][j].patientInRoom.patientId)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                        hospital->layout.elements[i][j].patientInRoom.patientId = NULL;
                    }
                }
                // Bebaskan memori untuk array kolom (yaitu, baris ke-i dari denah).
                free(hospital->layout.elements[i]);
                hospital->layout.elements[i] = NULL;
            }
        }
        // Bebaskan memori untuk array pointer baris itu sendiri.
        free(hospital->layout.elements);
        hospital->layout.elements = NULL;
    }

    // Membebaskan memori untuk daftar antrian (HospitalQueueList).
    if (hospital->queues.queues) // Periksa apakah array struktur Queue dialokasikan.
    {
        // Loop melalui semua slot antrian yang dialokasikan (berdasarkan `hospital->queues.capacity`).
        for (int i = 0; i < hospital->queues.capacity; i++)
        {
            // Menggunakan `head` untuk iterasi node dalam antrian (perubahan dari `front`).
            QueueNode *current = hospital->queues.queues[i].head; // Mulai dari node kepala.
            QueueNode *nextNode;
            while (current != NULL)
            {                             // Selama masih ada node dalam antrian.
                nextNode = current->next; // Simpan pointer ke node berikutnya.
                free(current);            // Bebaskan memori untuk node saat ini.
                current = nextNode;       // Pindah ke node berikutnya.
            }
            // Setelah semua node dibebaskan, set pointer head dan tail ke NULL, dan size ke 0.
            hospital->queues.queues[i].head = NULL;
            hospital->queues.queues[i].tail = NULL;
            hospital->queues.queues[i].size = 0; // Reset ukuran antrian.
        }
        // Bebaskan memori untuk array struktur Queue itu sendiri.
        free(hospital->queues.queues);
        hospital->queues.queues = NULL;
    }

    // Membebaskan memori untuk daftar penyakit (DiseaseList).
    if (hospital->diseases.elements)
    {
        free(hospital->diseases.elements);
        hospital->diseases.elements = NULL;
    }

    // Membebaskan memori untuk daftar obat (MedicationList).
    if (hospital->medications.elements)
    {
        free(hospital->medications.elements);
        hospital->medications.elements = NULL;
    }

    // Membebaskan memori untuk daftar resep (PrescriptionList).
    if (hospital->prescriptions.elements)
    {
        free(hospital->prescriptions.elements);
        hospital->prescriptions.elements = NULL;
    }

    // Membebaskan memori untuk daftar riwayat pengobatan (TreatmentHistoryList).
    if (hospital->treatmentHistory.elements)
    {
        free(hospital->treatmentHistory.elements);
        hospital->treatmentHistory.elements = NULL;
    }

    // Pesan sukses setelah semua memori berhasil dibebaskan.
    printSuccess("Memori rumah sakit berhasil dibebaskan!");
}

boolean deletePatient(Hospital *hospital, int patientId)
{
    // Langkah 1: Menghapus pasien dari PatientList.
    int patientIndex = -1;         // Indeks pasien dalam array `hospital->patients.elements`, diinisialisasi -1 (tidak ditemukan).
    char patientUsername[50] = ""; // Buffer untuk menyimpan username pasien yang dihapus (digunakan untuk pesan sukses).
                                   // Mencari pasien dalam `PatientList` berdasarkan ID.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (hospital->patients.elements[i].id == patientId)
        {
            patientIndex = i;                                                 // Simpan indeks pasien yang ditemukan.
            strcpy(patientUsername, hospital->patients.elements[i].username); // Salin username pasien.
            break;                                                            // Keluar dari loop karena pasien sudah ditemukan.
        }
    }
    // Jika pasien tidak ditemukan di `PatientList` setelah iterasi.
    if (patientIndex == -1)
    {
        printError("Pasien tidak ditemukan!"); // Cetak pesan error.
        return false;                          // Hentikan fungsi, kembalikan false.
    }

    // Bebaskan memori yang dialokasikan untuk daftar obat (resep dan yang diminum) milik pasien yang akan dihapus.
    // Ini harus dilakukan SEBELUM elemen array digeser, agar pointer ke memori yang benar yang dibebaskan.
    if (hospital->patients.elements[patientIndex].medicationsPrescribed.medicationId != NULL)
    {
        free(hospital->patients.elements[patientIndex].medicationsPrescribed.medicationId);
        hospital->patients.elements[patientIndex].medicationsPrescribed.medicationId = NULL; // Set NULL.
    }
    if (hospital->patients.elements[patientIndex].medicationsTaken.medicationId != NULL)
    {
        free(hospital->patients.elements[patientIndex].medicationsTaken.medicationId);
        hospital->patients.elements[patientIndex].medicationsTaken.medicationId = NULL; // Set NULL.
    }

    // Geser elemen-elemen berikutnya dalam PatientList untuk mengisi celah yang ditinggalkan oleh pasien yang dihapus.
    // Ini adalah operasi penghapusan standar untuk array (jika urutan tidak penting atau akan diurutkan lagi nanti).
    for (int i = patientIndex; i < hospital->patients.nEff - 1; i++)
    {
        // Timpa elemen saat ini dengan elemen berikutnya.
        hospital->patients.elements[i] = hospital->patients.elements[i + 1];
    }
    hospital->patients.nEff--; // Kurangi jumlah pasien efektif dalam daftar.

    // Langkah 2: Menghapus entri pengguna (yang berkorespondensi dengan pasien tersebut) dari UserSet.
    int userIndex = -1; // Indeks pengguna dalam array `hospital->users.elements`.
                        // Cari pengguna berdasarkan ID (yang seharusnya sama dengan patientId).
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (hospital->users.elements[i].id == patientId)
        {
            userIndex = i; // Simpan indeks pengguna.
            break;         // Keluar dari loop.
        }
    }
    // Jika pengguna ditemukan di UserSet.
    if (userIndex != -1)
    {
        // Geser elemen-elemen berikutnya dalam UserSet.
        for (int i = userIndex; i < hospital->users.nEff - 1; i++)
        {
            hospital->users.elements[i] = hospital->users.elements[i + 1];
        }
        hospital->users.nEff--; // Kurangi jumlah pengguna efektif.
    }

    // Langkah 3: Menghapus pasien dari semua antrian (jika pasien tersebut ada dalam antrian).
    // Melakukan iterasi melalui semua slot antrian yang mungkin ada (berdasarkan `hospital->queues.capacity`).
    for (int i = 0; i < hospital->queues.capacity; i++)
    {
        Queue *q = &hospital->queues.queues[i]; // Pointer ke struktur antrian saat ini.
                                                // Lewati antrian yang tidak aktif (belum memiliki `roomCode`) atau antrian yang kosong.
                                                // `isQueueEmpty(q)` adalah fungsi utilitas yang diasumsikan ada.
        if (q->roomCode[0] == '\0' || isQueueEmpty(q))
        {
            continue; // Lanjut ke slot antrian berikutnya.
        }

        boolean removed = false;      // Flag untuk menandakan apakah pasien sudah dihapus dari antrian ini.
        QueueNode *current = q->head; // Mulai dari node kepala antrian (perubahan dari `front`).
        QueueNode *prev = NULL;       // Pointer ke node sebelumnya (digunakan untuk penghapusan dari tengah/belakang list).

        // Iterasi melalui node-node dalam antrian saat ini.
        while (current != NULL)
        {
            // Jika ID pasien di node saat ini (`current->info.patientId`) cocok dengan `patientId` yang akan dihapus.
            if (current->info.patientId == patientId)
            {
                // Logika penghapusan node dari linked list (kemungkinan doubly linked list).
                if (prev == NULL)
                {                            // Jika pasien ada di node kepala antrian (head).
                    q->head = current->next; // Pindahkan pointer `head` ke node berikutnya.
                }
                else
                {                               // Jika pasien ada di tengah atau belakang antrian.
                    prev->next = current->next; // Sambungkan node `prev` ke node setelah `current`.
                }

                // Penyesuaian untuk doubly linked list atau tail pointer.
                if (current->next == NULL) // Jika node yang dihapus adalah node terakhir (tail).
                {
                    q->tail = prev; // Pointer `tail` menunjuk ke `prev` (bisa NULL jika hanya ada 1 elemen).
                }
                else // Jika node yang dihapus bukan node terakhir.
                {
                    // Jika ini adalah doubly linked list, pointer `prev` dari node `current->next` perlu diupdate.
                    current->next->prev = prev;
                }
                free(current);  // Bebaskan memori yang dialokasikan untuk node `current`.
                q->size--;      // Kurangi ukuran (jumlah elemen) antrian.
                removed = true; // Tandai bahwa pasien sudah berhasil dihapus dari antrian ini.
                break;          // Keluar dari loop `while` karena pasien sudah ditemukan dan dihapus dari antrian ini.
            }
            prev = current;          // Simpan node saat ini sebagai `prev` untuk iterasi berikutnya.
            current = current->next; // Pindah ke node berikutnya dalam antrian.
        }
        // Jika setelah penghapusan, antrian menjadi kosong (head menjadi NULL), pastikan tail juga NULL.
        if (q->head == NULL)
        {
            q->tail = NULL;
        }
        // Jika pasien sudah dihapus dari satu antrian, kita bisa berasumsi pasien hanya bisa ada dalam satu antrian pada satu waktu.
        if (removed)
        {
            // Komentar di skrip asli menjelaskan bahwa posisi pasien lain di antrian ini
            // tidak perlu diupdate di sini karena ini adalah proses penghapusan pasien dari sistem secara keseluruhan.
            // Informasi `queueRoom` dan `queuePosition` pada data pasien yang dihapus menjadi tidak relevan.
            break; // Keluar dari loop `for` antrian (asumsi pasien hanya bisa ada di satu antrian).
        }
    }

    // Langkah 4: Menghapus pasien dari daftar pasien di dalam setiap ruangan (`patientInRoom`).
    // Iterasi melalui semua ruangan di denah layout.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            Room *room = &hospital->layout.elements[i][j]; // Pointer ke ruangan saat ini.
                                                           // Iterasi melalui daftar ID pasien yang ada di dalam ruangan tersebut.
            for (int k = 0; k < room->patientInRoom.nEff; k++)
            {
                // Jika ID pasien di dalam ruangan (`room->patientInRoom.patientId[k]`) cocok dengan `patientId` yang akan dihapus.
                if (room->patientInRoom.patientId[k] == patientId)
                {
                    // Geser elemen-elemen berikutnya dalam array `patientId` untuk mengisi celah.
                    for (int l = k; l < room->patientInRoom.nEff - 1; l++)
                    {
                        room->patientInRoom.patientId[l] = room->patientInRoom.patientId[l + 1];
                    }
                    room->patientInRoom.nEff--; // Kurangi jumlah pasien efektif di dalam ruangan.
                    break;                      // Keluar dari loop `k` karena pasien sudah dihapus dari ruangan ini.
                }
            }
        }
    }

    // Langkah 5: Menghapus pasien dari TreatmentHistory (riwayat pengobatan).
    // Implementasi ini hanya akan menghapus entri riwayat PERTAMA yang ditemukan cocok dengan patientId.
    // Jika seorang pasien mungkin memiliki beberapa entri riwayat, logika ini perlu diubah
    // untuk menghapus semua entri yang relevan (misalnya, dengan loop `while` atau menggeser berulang kali).
    int historyIndex = -1; // Indeks entri riwayat yang akan dihapus.
                           // Cari entri riwayat berdasarkan patientId.
    for (int i = 0; i < hospital->treatmentHistory.nEff; i++)
    {
        if (hospital->treatmentHistory.elements[i].patientId == patientId)
        {
            historyIndex = i; // Simpan indeks entri pertama yang cocok.
            break;            // Keluar dari loop.
        }
    }
    // Jika entri riwayat ditemukan.
    if (historyIndex != -1)
    {
        // Geser elemen-elemen berikutnya dalam array TreatmentHistory.
        for (int i = historyIndex; i < hospital->treatmentHistory.nEff - 1; i++)
        {
            hospital->treatmentHistory.elements[i] = hospital->treatmentHistory.elements[i + 1];
        }
        hospital->treatmentHistory.nEff--; // Kurangi jumlah riwayat efektif.
    }

    // Membuat dan mencetak pesan sukses setelah semua langkah penghapusan selesai.
    char successMessage[100] = "";
    strcat(successMessage, "Pasien dengan username '");
    strcat(successMessage, patientUsername); // Menggunakan username pasien yang sudah disimpan sebelumnya.
    strcat(successMessage, "' berhasil dihapus dari sistem!");
    printSuccess(successMessage);
    return true; // Mengembalikan true menandakan penghapusan berhasil.
}
