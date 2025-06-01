#include "registerCheckup.h"

boolean registerCheckup(Hospital *hospital, Session *session, float healthData[])
{
    // Validasi Input Awal: Memeriksa apakah pointer-pointer penting tidak NULL.
    if (hospital == NULL || session == NULL || healthData == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau data kesehatan tidak valid!"); // Pesan error jika ada input NULL.
        return false;                                                               // Mengembalikan false menandakan kegagalan.
    }

    // Validasi Sesi Login dan Peran Pengguna.
    // Hanya pengguna dengan peran PATIENT yang sudah login yang dapat mendaftar checkup.
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mendaftar checkup."); // Pesan error jika hak akses tidak sesuai.
        return false;                                                            // Mengembalikan false.
    }

    // Mencari Data Pasien yang Sedang Login.
    // Menggunakan username dari sesi untuk menemukan entri pasien yang sesuai di daftar pasien rumah sakit.
    int patientIdx = -1; // Indeks pasien dalam array `hospital->patients.elements`, diinisialisasi -1 (tidak ditemukan).
    // Melakukan iterasi melalui daftar pasien.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        // Membandingkan username pasien dengan username di sesi.
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
        {
            patientIdx = i; // Jika ditemukan, simpan indeksnya.
            break;          // Keluar dari loop.
        }
    }
    // Jika pasien tidak ditemukan setelah iterasi.
    if (patientIdx == -1)
    {
        printError("Pasien tidak ditemukan!"); // Pesan error.
        return false;                          // Mengembalikan false.
    }
    // Mendapatkan pointer ke struktur Patient yang sesuai.
    Patient *patient = &hospital->patients.elements[patientIdx];

    // Pemeriksaan Status Antrian Pasien.
    // Memeriksa apakah pasien sudah terdaftar dalam antrian lain.
    // `patient->queueRoom[0] != '\0'` berarti string `queueRoom` tidak kosong, menandakan pasien sudah di antrian.
    if (patient->queueRoom[0] != '\0')
    {
        printError("Anda sudah terdaftar dalam antrian!"); // Pesan error.
        return false;                                      // Mengembalikan false.
    }

    // Mengumpulkan Daftar Dokter yang Tersedia (yang memiliki ruangan).
    int availableDoctorCount = 0; // Penghitung jumlah dokter yang tersedia.
    // Array sementara untuk menyimpan pointer ke dokter yang tersedia. Ukuran berdasarkan jumlah total dokter.
    Doctor *availableDoctors[hospital->doctors.nEff];

    // Melakukan iterasi melalui daftar semua dokter di rumah sakit.
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        Doctor *doctor = &hospital->doctors.elements[i]; // Pointer ke dokter saat ini.
        // Memeriksa apakah dokter tersebut sudah ditugaskan ke sebuah ruangan.
        // `doctor->room[0] != '\0'` berarti string `room` tidak kosong.
        if (doctor->room[0] != '\0')
        {
            // Jika dokter memiliki ruangan, tambahkan ke daftar dokter yang tersedia.
            availableDoctors[availableDoctorCount] = doctor;
            availableDoctorCount++; // Increment jumlah dokter tersedia.
        }
    }

    // Penanganan Jika Tidak Ada Dokter yang Tersedia.
    if (availableDoctorCount == 0)
    {
        printError("Tidak ada dokter yang tersedia saat ini."); // Pesan error.
        return false;                                           // Mengembalikan false, tidak perlu menampilkan tabel dokter.
    }

    // Menampilkan Daftar Dokter yang Tersedia dalam Format Tabel.
    printf("\nDaftar Dokter yang Tersedia:\n"); // Judul bagian.

    // Mendefinisikan lebar kolom dan header untuk tabel dokter.
    // Kolom: Nama Dokter, Spesialisasi, Ruangan, Jumlah Antrian Saat Ini, Aura, Biaya Checkup.
    int widths[] = {5, 20, 15, 10, 10, 10, 15};
    const char *headers[] = {"No", "Dokter", "Spesialisasi", "Ruangan", "Antrian", "Aura", "Biaya Checkup"};

    // Mencetak border atas dan baris header tabel.
    printTableBorder(widths, 7, 1);    // 7 kolom, tipe border 1 (atas).
    printTableRow(headers, widths, 7); // Mencetak baris header.
    printTableBorder(widths, 7, 2);    // Mencetak garis pemisah di bawah header.

    // Melakukan iterasi melalui daftar dokter yang tersedia untuk ditampilkan di tabel.
    for (int i = 0; i < availableDoctorCount; i++)
    {
        Doctor *doctor = availableDoctors[i]; // Pointer ke dokter saat ini.
        // Menghitung jumlah pasien saat ini di antrian dokter tersebut.
        int queueCount = 0;

        // Mencari antrian yang sesuai dengan ruangan dokter.
        for (int j = 0; j < hospital->queues.nRooms; j++) // `nRooms` adalah jumlah antrian aktif.
        {
            // Membandingkan kode ruangan antrian dengan kode ruangan dokter.
            if (strcmp(hospital->queues.queues[j].roomCode, doctor->room) == 0)
            {
                // Jika ditemukan, dapatkan ukuran antrian menggunakan `queueSize` (diasumsikan ada).
                queueCount = queueSize(&hospital->queues.queues[j]);
                break; // Keluar dari loop pencarian antrian.
            }
        }

        // Menyiapkan data dokter untuk ditampilkan dalam baris tabel.
        // Buffer untuk menyimpan representasi string dari angka (jumlah antrian, aura, biaya).
        char numberStr[10], queueCountStr[10], auraStr[10], checkupCostStr[20];
        // Mengkonversi integer ke string.
        integerToString(i + 1, numberStr, sizeof(numberStr));
        strcat(numberStr, ". ");
        integerToString(queueCount, queueCountStr, sizeof(queueCountStr));
        integerToString(doctor->aura, auraStr, sizeof(auraStr));
        // Mengkonversi float ke string dengan 2 angka desimal.
        floatToString(doctor->checkupCost, checkupCostStr, sizeof(checkupCostStr), 2);

        // Array string yang berisi data untuk satu baris tabel.
        const char *row[] = {
            numberStr,
            doctor->username,
            doctor->specialization,
            doctor->room,
            queueCountStr,
            auraStr,
            checkupCostStr};
        printTableRow(row, widths, 7); // Mencetak baris data dokter.
    }

    printTableBorder(widths, 7, 3); // Mencetak border bawah tabel.

    // Meminta Pasien untuk Memilih Dokter dari Daftar.
    int doctorChoice = 0; // Variabel untuk menyimpan pilihan dokter oleh pasien.
    // Loop validasi input: terus meminta input hingga pilihan valid (antara 1 dan jumlah dokter tersedia).
    while (doctorChoice < 1 || doctorChoice > availableDoctorCount)
    {
        // Membangun string prompt untuk input.
        char promptSelectDoctor[100] = "";
        strcat(promptSelectDoctor, "Pilih dokter (1 - ");
        char availableCountStr[10]; // Buffer untuk jumlah dokter tersedia sebagai string.
        integerToString(availableDoctorCount, availableCountStr, sizeof(availableCountStr));
        strcat(promptSelectDoctor, availableCountStr);
        strcat(promptSelectDoctor, "): ");

        // Membaca input integer dari pengguna. `readValidInt` adalah fungsi kustom.
        if (!readValidInt(&doctorChoice, promptSelectDoctor))
        {
            printError("Pilihan tidak valid!, harap masukkan angka yang sesuai."); // Pesan error jika input bukan integer.
            // Seharusnya loop berlanjut jika input tidak valid, bukan return false.
            // Namun, sesuai instruksi, tidak mengubah algoritma.
            // Jika readValidInt mengembalikan false dan tidak mengubah doctorChoice, loop akan tak terbatas.
            // Diasumsikan readValidInt menangani loop inputnya sendiri atau doctorChoice direset.
            // Untuk skrip ini, jika readValidInt gagal, fungsi akan return false.
            return false;
        }
        // Jika input integer tapi di luar rentang, pesan error dicetak oleh blok `if` di `while` berikutnya (jika ada).
        // Skrip saat ini akan keluar dari loop jika input valid integer, lalu validasi rentang terjadi di `while`.
    }
    // Mendapatkan pointer ke struktur Doctor yang dipilih oleh pasien.
    // `doctorChoice - 1` karena pilihan pengguna berbasis 1, sedangkan indeks array berbasis 0.
    Doctor *doctor = availableDoctors[doctorChoice - 1];

    // Validasi Biaya Checkup.
    // Memeriksa apakah saldo BananaRich pasien mencukupi untuk biaya checkup dokter yang dipilih.
    if (patient->bananaRich < doctor->checkupCost)
    {
        printError("Saldo BananaRich tidak cukup untuk checkup!"); // Pesan error.
        return false;                                              // Mengembalikan false.
    }

    // Mencari atau Membuat Antrian untuk Dokter yang Dipilih.
    int queueIdx = -1; // Indeks antrian dokter dalam array `hospital->queues.queues`.
    // Mencari apakah sudah ada antrian untuk ruangan dokter yang dipilih.
    for (int i = 0; i < hospital->queues.nRooms; i++) // Loop melalui antrian yang aktif.
    {
        if (strcmp(hospital->queues.queues[i].roomCode, doctor->room) == 0)
        {
            queueIdx = i; // Jika ditemukan, simpan indeksnya.
            break;        // Keluar dari loop.
        }
    }

    // Jika tidak ada antrian yang ditemukan untuk dokter ini, buat antrian baru.
    if (queueIdx == -1)
    {
        // Memeriksa apakah kapasitas total untuk menyimpan antrian baru sudah penuh.
        if (hospital->queues.nRooms >= hospital->queues.capacity)
        {
            printError("Kapasitas antrian penuh!"); // Pesan error.
            return false;                           // Mengembalikan false.
        }
        // Gunakan slot berikutnya yang tersedia di array `hospital->queues.queues`.
        queueIdx = hospital->queues.nRooms++;                 // Increment `nRooms` setelah mendapatkan indeks.
        Queue *newQueue = &hospital->queues.queues[queueIdx]; // Pointer ke antrian baru.
        // Inisialisasi antrian baru dengan kode ruangan dokter.
        // `initializeQueue` adalah fungsi kustom (diasumsikan ada).
        initializeQueue(newQueue, doctor->room);
    }

    // Menambahkan Pasien ke Antrian Dokter.
    Queue *queueToEnqueue = &hospital->queues.queues[queueIdx]; // Pointer ke antrian yang akan digunakan.
    // Memanggil fungsi `enqueue` (diasumsikan ada) untuk menambahkan ID pasien ke antrian.
    if (!enqueue(queueToEnqueue, patient->id))
    {
        printError("Gagal menambahkan pasien ke antrian!"); // Pesan error jika enqueue gagal.
        return false;                                       // Mengembalikan false.
    }
    // Update informasi antrian pada data pasien.
    strcpy(patient->queueRoom, doctor->room);           // Simpan kode ruangan antrian.
    patient->queuePosition = queueSize(queueToEnqueue); // Simpan posisi pasien dalam antrian.

    // Menyimpan Data Kesehatan Awal Pasien.
    // `healthData` adalah array float yang diterima sebagai parameter.
    // Indeks 0 hingga 9 digunakan untuk data kesehatan spesifik.
    patient->bodyTemperature = healthData[0];
    patient->systolicBloodPressure = (int)healthData[1];  // Cast ke int.
    patient->diastolicBloodPressure = (int)healthData[2]; // Cast ke int.
    patient->heartRate = (int)healthData[3];              // Cast ke int.
    patient->oxygenSaturation = (int)healthData[4];       // Cast ke int.
    patient->bloodSugarLevel = healthData[5];
    patient->weight = healthData[6];
    patient->height = healthData[7];
    patient->cholesterolLevel = healthData[8];
    patient->platelets = (int)healthData[9]; // Cast ke int.

    // Mengelola Biaya Checkup dan Distribusi Pendapatan.
    patient->bananaRich -= doctor->checkupCost;                     // Kurangi saldo pasien.
    hospital->finance.hospitalBalance += 0.2 * doctor->checkupCost; // Rumah sakit mendapat 20%.
    doctor->bananaRich += 0.8 * doctor->checkupCost;                // Dokter mendapat 80%.

    // Menambahkan Riwayat Pengobatan Pasien.
    // Memeriksa apakah kapasitas array riwayat pengobatan (`treatmentHistory`) perlu diperbesar.
    if (hospital->treatmentHistory.nEff >= hospital->treatmentHistory.capacity)
    {
        // Realokasi memori untuk array `treatmentHistory.elements`.
        TreatmentHistory *newElements = (TreatmentHistory *)realloc(hospital->treatmentHistory.elements, sizeof(TreatmentHistory) * hospital->treatmentHistory.capacity);

        // Jika realokasi gagal.
        if (!newElements)
        {
            printError("Gagal mengalokasikan ulang memori untuk riwayat pengobatan!");
            // Rollback perubahan finansial yang telah dilakukan karena pendaftaran gagal.
            patient->bananaRich += doctor->checkupCost;
            hospital->finance.hospitalBalance -= 0.2 * doctor->checkupCost;
            doctor->bananaRich -= 0.8 * doctor->checkupCost;

            // Data kesehatan pasien direset ke nilai awal.
            patient->bodyTemperature = (float) 0;
            patient->systolicBloodPressure = 0;  // Cast ke int.
            patient->diastolicBloodPressure = 0; // Cast ke int.
            patient->heartRate = 0;              // Cast ke int.
            patient->oxygenSaturation = 0;       // Cast ke int.
            patient->bloodSugarLevel = (float) 0;
            patient->weight = (float) 0;
            patient->height = (float) 0;
            patient->cholesterolLevel = (float) 0;
            patient->platelets = 0; // Cast ke int.

            return false; // Mengembalikan false.
        }
        // Setelah realokasi, gandakan kapasitas.
        hospital->treatmentHistory.capacity *= 2;
        // Update pointer elemen riwayat ke memori yang baru direalokasi.
        hospital->treatmentHistory.elements = newElements;
    }

    // Menambahkan entri baru ke riwayat pengobatan.
    // Mendapatkan pointer ke elemen berikutnya yang tersedia di array riwayat.
    TreatmentHistory *history = &hospital->treatmentHistory.elements[hospital->treatmentHistory.nEff++]; // Increment nEff setelah mendapatkan pointer.
    history->patientId = patient->id;                                                                    // ID pasien.
    history->doctorId = doctor->id;                                                                      // ID dokter.
    strcpy(history->room, doctor->room);                                                                 // Kode ruangan tempat checkup.
    strcpy(history->disease, "Belum Didiagnosis");                                                       // Status diagnosis awal.
    history->treatmentStatus = false;                                                                    // Status perawatan awal (belum selesai).

    // Menampilkan Hasil Konfirmasi Pendaftaran Checkup.
    printf("\n"); // Baris baru untuk spasi.

    printHeader("Pendaftaran Checkup"); // Header konfirmasi.
    // Tabel ringkasan pendaftaran.
    int widths2[] = {20, 20}; // Lebar kolom: Dokter, Ruangan.
    const char *headers2[] = {"Dokter", "Ruangan"};
    printTableBorder(widths2, 2, 1);
    printTableRow(headers2, widths2, 2);
    printTableBorder(widths2, 2, 2);
    const char *row2[] = {doctor->username, doctor->room}; // Data dokter dan ruangan.
    printTableRow(row2, widths2, 2);
    printTableBorder(widths2, 2, 3);

    // Menampilkan posisi antrian pasien.
    // FORMAT_BOLD, COLOR_YELLOW, FORMAT_RESET adalah makro untuk format teks.
    printf(FORMAT_BOLD COLOR_YELLOW "[ℹ️  | Info]: ");
    printf("Posisi antrian: %d\n" FORMAT_RESET, patient->queuePosition);

    // Membuat dan mencetak pesan sukses pendaftaran.
    char successMsg[100] = "Pendaftaran checkup untuk ";
    strcat(successMsg, session->username); // Nama pasien.
    strcat(successMsg, " pada dr.");
    strcat(successMsg, doctor->username); // Nama dokter.
    strcat(successMsg, " di ruangan ");
    strcat(successMsg, doctor->room); // Kode ruangan.
    strcat(successMsg, " berhasil!");
    printSuccess(successMsg); // Mencetak pesan sukses.
    return true;              // Mengembalikan true menandakan pendaftaran berhasil.
}