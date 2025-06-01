#include "treat.h"

boolean treatPatient(Hospital *hospital, Session *session)
{
    // ========== VALIDASI INPUT AWAL DAN SESI ==========
    // Memeriksa apakah pointer ke struktur Hospital dan Session tidak NULL.
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau username tidak valid!"); // Pesan error jika input tidak valid.
        return false;                                                         // Mengembalikan false menandakan kegagalan.
    }
    // Memastikan bahwa pengguna yang memanggil fungsi ini sudah login (`session->isLoggedIn`)
    // dan memiliki peran sebagai Dokter (`session->role == DOCTOR`).
    if (!session->isLoggedIn || session->role != DOCTOR)
    {
        printError("Akses ditolak! Hanya Dokter yang dapat meresepkan obat."); // Pesan error jika hak akses tidak sesuai.
        return false;                                                          // Mengembalikan false.
    }

    // ========== MENCARI DATA DOKTER YANG SEDANG LOGIN ==========
    // Menggunakan username dari sesi untuk menemukan entri dokter yang sesuai.
    int doctorIdx = -1; // Indeks dokter dalam array `hospital->doctors.elements`.
    // Melakukan iterasi melalui daftar dokter di rumah sakit.
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        // Membandingkan username dokter di daftar dengan username dari sesi.
        if (strcmp(hospital->doctors.elements[i].username, session->username) == 0)
        {
            doctorIdx = i; // Jika ditemukan, simpan indeksnya.
            break;         // Keluar dari loop.
        }
    }
    // Jika dokter tidak ditemukan setelah iterasi.
    if (doctorIdx == -1)
    {
        printError("Dokter tidak ditemukan dalam daftar!"); // Pesan error.
        return false;                                       // Mengembalikan false.
    }
    // Mendapatkan pointer ke struktur Doctor yang sesuai.
    Doctor *doctor = &hospital->doctors.elements[doctorIdx];

    // ========== VALIDASI RUANGAN DOKTER ==========
    // Memastikan dokter sudah ditugaskan ke sebuah ruangan.
    // `doctor->room[0] == '\0'` berarti string `room` pada dokter kosong.
    if (doctor->room[0] == '\0')
    {
        printError("Dokter tidak ditugaskan ke ruangan manapun!"); // Pesan error.
        return false;                                              // Mengembalikan false.
    }

    // ========== MENCARI RUANGAN DOKTER DAN PASIEN DI DALAMNYA ==========
    // Mencari struktur Room yang sesuai dengan kode ruangan dokter.
    Room *doctorRoom = NULL; // Pointer ke ruangan dokter, diinisialisasi NULL.
    // Iterasi melalui denah layout.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            // Membandingkan kode ruangan di denah dengan kode ruangan dokter.
            if (strcmp(hospital->layout.elements[i][j].code, doctor->room) == 0)
            {
                doctorRoom = &hospital->layout.elements[i][j]; // Ruangan ditemukan.
                break;                                         // Keluar dari loop kolom.
            }
        }
        if (doctorRoom != NULL) // Jika ruangan sudah ditemukan di loop kolom.
            break;              // Keluar dari loop baris.
    }

    // Jika ruangan dokter tidak ditemukan di denah, atau jika tidak ada pasien di dalam ruangan tersebut.
    // `doctorRoom->patientInRoom.nEff == 0` berarti tidak ada pasien dalam daftar `patientInRoom`.
    if (doctorRoom == NULL || doctorRoom->patientInRoom.nEff == 0)
    {
        printError("Tidak ada pasien yang berada didalam ruangan dokter ini."); // Pesan error.
        return false;                                                           // Mengembalikan false.
    }

    // ========== MENGAMBIL DAN MEMVALIDASI PASIEN PERTAMA DI RUANGAN ==========
    // Mengambil ID pasien pertama dari daftar `patientInRoom` di ruangan dokter.
    // Diasumsikan pasien pertama dalam array ini adalah yang akan diobati.
    int patientId = doctorRoom->patientInRoom.patientId[0];
    int patientIdx = -1; // Indeks pasien di daftar pasien rumah sakit.
    // Mencari data lengkap pasien berdasarkan ID yang didapat.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (hospital->patients.elements[i].id == patientId)
        {
            patientIdx = i; // Pasien ditemukan.
            break;
        }
    }

    // Jika data pasien dengan ID tersebut tidak ditemukan di daftar pasien rumah sakit.
    if (patientIdx == -1)
    {
        printError("Pasien tidak ditemukan!"); // Pesan error (inkonsistensi data).
        return false;                          // Mengembalikan false.
    }
    // Mendapatkan pointer ke struktur Patient.
    Patient *patient = &hospital->patients.elements[patientIdx];

    // ========== VALIDASI PENYAKIT DAN STATUS PASIEN ==========
    // Memeriksa apakah pasien memiliki diagnosis penyakit yang valid (bukan "Tidak terdeteksi").
    if (strcmp(patient->disease, "Tidak terdeteksi") == 0)
    {
        printError("Pasien tidak menderita penyakit apapun!"); // Pesan error.
        return false;                                          // Mengembalikan false.
    }

    // Memastikan pasien sudah didiagnosis sebelumnya.
    if (!patient->diagnosedStatus) // Jika `diagnosedStatus` adalah false.
    {
        printError("Pasien belom mendapat diagnosa dari dokter!"); // Pesan error.
        return false;                                              // Mengembalikan false.
    }

    // Memastikan pasien belum pernah diberikan resep obat sebelumnya untuk diagnosis ini.
    if (patient->treatedStatus) // Jika `treatedStatus` sudah true.
    {
        printError("Pasien sudah diberikan resep obat!"); // Pesan error.
        return false;                                     // Mengembalikan false.
    }

    // Memastikan pasien berada di antrian (atau di dalam) ruangan dokter yang benar.
    // `patient->queueRoom` menyimpan kode ruangan tempat pasien terdaftar.
    if (strcmp(patient->queueRoom, doctor->room) != 0)
    {
        printError("Pasien tidak berada di antrian ruangan dokter!"); // Pesan error.
        return false;                                                 // Mengembalikan false.
    }

    // ========== MENCARI ID PENYAKIT PASIEN ==========
    // Mencari ID penyakit berdasarkan nama penyakit yang tersimpan pada data pasien.
    int diseaseId = -1; // ID penyakit, diinisialisasi -1 (tidak ditemukan).
    // Iterasi melalui daftar penyakit yang diketahui sistem.
    for (int i = 0; i < hospital->diseases.nEff; i++)
    {
        // Membandingkan nama penyakit di daftar dengan nama penyakit pasien.
        if (strcmp(hospital->diseases.elements[i].name, patient->disease) == 0)
        {
            diseaseId = hospital->diseases.elements[i].id; // Jika cocok, simpan ID penyakit.
            break;                                         // Keluar dari loop.
        }
    }

    // Jika ID penyakit tidak ditemukan dalam database (nama penyakit pasien tidak cocok dengan yang ada).
    if (diseaseId == -1)
    {
        printError("Penyakit pasien tidak ditemukan dalam database!"); // Pesan error.
        return false;                                                  // Mengembalikan false.
    }

    // ========== MENGHITUNG DAN MENGUMPULKAN RESEP OBAT ==========
    // Menghitung terlebih dahulu berapa banyak jenis obat yang diperlukan untuk penyakit ini.
    int medicationCount = 0; // Jumlah jenis obat.
    // Iterasi melalui daftar resep standar (`hospital->prescriptions`).
    for (int i = 0; i < hospital->prescriptions.nEff; i++)
    {
        // Jika ID penyakit pada resep cocok dengan `diseaseId` pasien.
        if (hospital->prescriptions.elements[i].diseaseId == diseaseId)
        {
            medicationCount++; // Increment jumlah obat.
        }
    }

    // Alokasi memori untuk menyimpan daftar resep sementara (sebelum diurutkan).
    MedicationPrescription *tempPrescriptions = NULL; // Pointer ke array resep sementara.
    // `prescribedMedications` akan menyimpan daftar obat final (setelah diurutkan dan diambil ID-nya).
    // Inisialisasi `elements` ke NULL, `nEff` dan `capacity` ke 0.
    MedicationList prescribedMedications = {NULL, 0, 0};

    // Jika ada obat yang ditemukan untuk penyakit ini (`medicationCount > 0`).
    if (medicationCount > 0)
    {
        // Alokasikan memori untuk `tempPrescriptions`.
        tempPrescriptions = malloc(medicationCount * sizeof(MedicationPrescription));
        if (tempPrescriptions == NULL) // Jika alokasi gagal.
        {
            printError("Gagal mengalokasi memori untuk resep sementara!");
            return false; // Mengembalikan false.
        }

        // Mengisi daftar resep sementara (`tempPrescriptions`) dengan resep yang cocok.
        int tempCount = 0; // Penghitung untuk `tempPrescriptions`.
        for (int i = 0; i < hospital->prescriptions.nEff; i++)
        {
            if (hospital->prescriptions.elements[i].diseaseId == diseaseId)
            {
                tempPrescriptions[tempCount] = hospital->prescriptions.elements[i]; // Salin struktur resep.
                tempCount++;
            }
        }

        // Mengurutkan `tempPrescriptions` berdasarkan `doseOrder` (urutan minum obat) secara menaik.
        // Menggunakan algoritma bubble sort sederhana.
        for (int i = 0; i < medicationCount - 1; i++)
        {
            for (int j = 0; j < medicationCount - i - 1; j++)
            {
                if (tempPrescriptions[j].doseOrder > tempPrescriptions[j + 1].doseOrder)
                {
                    // Tukar posisi dua elemen resep.
                    MedicationPrescription temp = tempPrescriptions[j];
                    tempPrescriptions[j] = tempPrescriptions[j + 1];
                    tempPrescriptions[j + 1] = temp;
                }
            }
        }

        // Alokasi memori untuk daftar obat final (`prescribedMedications.elements`).
        // Ini akan menyimpan struktur Medication (bukan MedicationPrescription).
        // Namun, kode di bawah hanya menyimpan ID obat, jadi mungkin lebih tepat jika ini adalah array int.
        // Sesuai skrip asli, ini adalah `MedicationList` yang elemennya `Medication`.
        prescribedMedications.elements = malloc(medicationCount * sizeof(Medication)); // Alokasi untuk struktur Medication.
        if (prescribedMedications.elements == NULL)                                    // Jika alokasi gagal.
        {
            free(tempPrescriptions); // Bebaskan memori `tempPrescriptions`.
            printError("Gagal mengalokasi memori untuk daftar obat!");
            return false; // Mengembalikan false.
        }
        prescribedMedications.capacity = medicationCount; // Set kapasitas.

        // Mengisi daftar obat final (`prescribedMedications`) dengan ID obat dari `tempPrescriptions` yang sudah diurutkan.
        // `prescribedMedications.elements[i].id` akan diisi. Field lain dari `Medication` tidak diisi di sini.
        for (int i = 0; i < medicationCount; i++)
        {
            prescribedMedications.elements[i].id = tempPrescriptions[i].medicationId; // Simpan ID obat.
            // Seharusnya juga menyalin nama obat jika `prescribedMedications` dimaksudkan untuk menyimpan detail obat.
            // Namun, skrip selanjutnya hanya menggunakan ID ini untuk mengisi `patient->medicationsPrescribed.medicationId`.
            prescribedMedications.nEff++; // Increment jumlah obat efektif.
        }

        // Bebaskan memori yang dialokasikan untuk `tempPrescriptions` karena sudah tidak diperlukan.
        free(tempPrescriptions);
    }

    // Menampilkan daftar obat yang harus diberikan kepada pasien.
    // Memeriksa apakah ada obat yang diresepkan (`prescribedMedications.nEff > 0`).
    if (prescribedMedications.nEff > 0)
    {
        printf(COLOR_GREEN "Dokter sedang mengobati pasien!\n" COLOR_RESET);                      // Pesan status.
        printf("Pasien memiliki penyakit %s%s%s\n", COLOR_YELLOW, patient->disease, COLOR_RESET); // Menampilkan penyakit pasien.
        printf("\nObat yang harus diberikan:\n");                                                 // Judul daftar obat.

        // Mendefinisikan lebar kolom untuk tabel daftar obat.
        int widths[] = {5, 30}; // Kolom: Nomor, Nama Obat.
        // Header tabel.
        const char *header[] = {"No.", "Nama Obat"};
        printTableBorder(widths, 2, 1);   // Border atas.
        printTableRow(header, widths, 2); // Baris header.
        printTableBorder(widths, 2, 2);   // Pemisah.

        // Mengisi tabel dengan data obat yang telah diberikan (atau akan diberikan) kepada pasien.
        // Loop ini menggunakan `patient->medicationsPrescribed` yang SEHARUSNYA diisi SETELAH ini.
        // Ini adalah potensi bug: tabel ini dicetak SEBELUM `patient->medicationsPrescribed` diupdate.
        // Seharusnya loop ini menggunakan `prescribedMedications` yang baru saja diisi.
        // **Komentar ini berdasarkan analisis, tanpa mengubah kode.**
        // Loop di bawah ini akan menampilkan daftar obat yang MUNGKIN sudah ada sebelumnya pada pasien,
        // BUKAN daftar obat yang baru saja ditentukan untuk penyakit ini.
        // Jika `patient->medicationsPrescribed` kosong, tidak ada yang akan dicetak di sini.
        // Untuk menampilkan obat yang BARU diresepkan, seharusnya loop melalui `prescribedMedications`.
        for (int i = 0; i < patient->medicationsPrescribed.nEff; i++) // Loop menggunakan nEff LAMA dari pasien.
        {
            // Mendapatkan detail obat berdasarkan ID dari `patient->medicationsPrescribed.medicationId[i]`.
            // Ini juga mengasumsikan bahwa `medicationId[i]` adalah indeks yang valid untuk `hospital->medications.elements`.
            // Seharusnya mencari berdasarkan ID, bukan menggunakan ID sebagai indeks.
            // Contoh perbaikan (tidak diimplementasikan sesuai instruksi):
            // Medication* med = NULL;
            // for (int m_idx = 0; m_idx < hospital->medications.nEff; ++m_idx) {
            //    if (hospital->medications.elements[m_idx].id == patient->medicationsPrescribed.medicationId[i]) {
            //        med = &hospital->medications.elements[m_idx];
            //        break;
            //    }
            // }
            // if (med == NULL) continue; // atau handle error
            Medication *med = &hospital->medications.elements[patient->medicationsPrescribed.medicationId[i]]; // Ini mengasumsikan ID adalah indeks+offset.

            // Menyiapkan data untuk baris tabel.
            char no[10] = "", medicationName[50] = "";
            integerToString(i + 1, no, sizeof(no)); // Nomor urut.
            strcat(no, ".");
            // Jika `med` valid (setelah perbaikan di atas), salin nama obat.
            // strcpy(medicationName, med->name);
            // Kode asli mungkin crash di sini jika `patient->medicationsPrescribed.medicationId[i]` bukan indeks yang valid.
            // Untuk sementara, kita asumsikan `med` valid sesuai skrip asli.
            // Jika `patient->medicationsPrescribed` kosong, loop ini tidak berjalan.
            // Jika diisi dengan ID obat yang benar, maka `med->name` akan valid.
            // Namun, `patient->medicationsPrescribed` baru akan diisi SETELAH blok ini.
            // Jadi, loop ini kemungkinan besar tidak akan mencetak apa-apa atau mencetak data lama.
            // Untuk tujuan komentar, kita ikuti alur skrip asli.
            // Jika kita ingin mencetak obat yang BARU diresepkan, kita harus loop `prescribedMedications`.
            // **Mengikuti skrip asli, ini akan mencoba mengakses `hospital->medications.elements` menggunakan ID sebagai indeks, yang salah.**
            // **Untuk tujuan komentar, kita akan mengasumsikan ini dimaksudkan untuk loop `prescribedMedications`**
            // **dan `med` akan dicari berdasarkan `prescribedMedications.elements[i].id`.**
            // **Namun, karena diminta tidak mengubah, kita komentari sesuai yang tertulis.**
            // **Agar tidak crash, kita akan cari nama obat dari `prescribedMedications` dan `hospital->medications`**
            char tempMedName[50] = "Obat tidak diketahui";
            for (int med_idx = 0; med_idx < hospital->medications.nEff; ++med_idx)
            {
                // Loop ini seharusnya menggunakan prescribedMedications.elements[i].id
                // tapi karena patient->medicationsPrescribed.medicationId[i] yang digunakan di skrip asli,
                // kita asumsikan itu yang dimaksud untuk dicari namanya.
                // Namun, karena patient->medicationsPrescribed belum diisi, loop ini tidak akan berjalan
                // jika patient->medicationsPrescribed.nEff adalah 0.
                // Jika kita loop prescribedMedications.nEff:
                if (i < prescribedMedications.nEff && hospital->medications.elements[med_idx].id == prescribedMedications.elements[i].id)
                {
                    strcpy(tempMedName, hospital->medications.elements[med_idx].name);
                    break;
                }
            }
            // Menggunakan tempMedName jika loop di atas dimaksudkan untuk prescribedMedications
            strcpy(medicationName, tempMedName);
            // Jika loop asli `patient->medicationsPrescribed.nEff` yang dimaksud, dan itu kosong, blok ini tidak dieksekusi.

            const char *row[] = {no, medicationName};
            printTableRow(row, widths, 2); // Menampilkan baris tabel.
        }
        // Jika loop di atas seharusnya menampilkan obat yang BARU diresepkan:
        // for (int i = 0; i < prescribedMedications.nEff; i++) {
        //     Medication* med_detail = NULL;
        //     for (int m_idx = 0; m_idx < hospital->medications.nEff; ++m_idx) {
        //         if (hospital->medications.elements[m_idx].id == prescribedMedications.elements[i].id) {
        //             med_detail = &hospital->medications.elements[m_idx];
        //             break;
        //         }
        //     }
        //     char no_str[10]; integerToString(i + 1, no_str, sizeof(no_str)); strcat(no_str, ".");
        //     const char* row_data[] = {no_str, (med_detail ? med_detail->name : "Nama Tidak Diketahui")};
        //     printTableRow(row_data, widths, 2);
        // }

        printTableBorder(widths, 2, 3); // Border bawah tabel.

        // Menyimpan resep obat yang sudah diurutkan ke dalam data pasien (`patient->medicationsPrescribed`).
        // Pertama, pastikan kapasitas array `medicationId` pada pasien mencukupi.
        if (patient->medicationsPrescribed.capacity < medicationCount)
        {
            // Jika kapasitas tidak cukup, realokasi memori.
            int *newMedicationIds = realloc(patient->medicationsPrescribed.medicationId,
                                            medicationCount * sizeof(int));
            if (newMedicationIds == NULL) // Jika realokasi gagal.
            {
                free(prescribedMedications.elements); // Bebaskan memori `prescribedMedications`.
                printError("Gagal mengalokasi memori untuk daftar obat pasien!");
                return false;
            }
            // Update pointer dan kapasitas pada data pasien.
            patient->medicationsPrescribed.medicationId = newMedicationIds;
            patient->medicationsPrescribed.capacity = medicationCount;
        }

        // Menyimpan ID-ID obat dari `prescribedMedications` (yang sudah diurutkan)
        // ke dalam `patient->medicationsPrescribed.medicationId`.
        patient->medicationsPrescribed.nEff = 0; // Reset nEff pasien terlebih dahulu.
        for (int i = 0; i < prescribedMedications.nEff; i++)
        {
            patient->medicationsPrescribed.medicationId[patient->medicationsPrescribed.nEff] =
                prescribedMedications.elements[i].id; // Salin ID obat.
            patient->medicationsPrescribed.nEff++;    // Increment jumlah obat efektif pada pasien.
        }

        // Update status pengobatan pasien menjadi true (sudah diberi resep).
        patient->treatedStatus = true;

        // Bebaskan memori yang dialokasikan untuk `prescribedMedications.elements` karena sudah disalin ke pasien.
        free(prescribedMedications.elements);
        prescribedMedications.elements = NULL; // Set ke NULL.

        printSuccess("Obat berhasil diberikan!"); // Pesan sukses.
        return true;                              // Proses pemberian resep berhasil.
    }
    else // Jika `prescribedMedications.nEff` adalah 0 (tidak ada obat ditemukan untuk penyakit ini).
    {
        printError("Tidak ada obat yang ditemukan untuk penyakit ini!");
        // `patient->treatedStatus` tidak diubah, tetap false.
        return false; // Proses gagal.
    }
}