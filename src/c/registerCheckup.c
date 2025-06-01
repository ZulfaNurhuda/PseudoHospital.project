#include "registerCheckup.h"

boolean registerCheckup(Hospital *hospital, Session *session, float healthData[])
{
    // ========== VALIDASI INPUT AWAL ==========
    // Memeriksa apakah pointer-pointer penting (hospital, session, healthData) tidak NULL.
    // Ini adalah langkah pencegahan dasar untuk menghindari dereferensi NULL pointer.
    if (hospital == NULL || session == NULL || healthData == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau data kesehatan tidak valid!"); // Pesan error jika ada input yang NULL.
        return false;                                                               // Mengembalikan false menandakan kegagalan fungsi.
    }

    // ========== VALIDASI SESI LOGIN DAN PERAN PENGGUNA ==========
    // Memastikan bahwa pengguna yang mencoba mendaftar checkup sudah login (`session->isLoggedIn`)
    // dan memiliki peran sebagai Pasien (`session->role == PATIENT`).
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mendaftar checkup."); // Pesan error jika hak akses tidak sesuai.
        return false;                                                            // Mengembalikan false.
    }

    // ========== MENCARI DATA PASIEN YANG SEDANG LOGIN ==========
    // Menggunakan username dari sesi untuk menemukan entri pasien yang sesuai di daftar pasien rumah sakit.
    int patientIdx = -1; // Indeks pasien dalam array `hospital->patients.elements`. Diinisialisasi -1 (tidak ditemukan).
    // Melakukan iterasi melalui daftar pasien yang ada di rumah sakit.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        // Membandingkan username pasien di daftar dengan username dari sesi saat ini.
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
        {
            patientIdx = i; // Jika username cocok, simpan indeks pasien tersebut.
            break;          // Keluar dari loop karena pasien sudah ditemukan.
        }
    }

    // Jika setelah iterasi, pasien tidak ditemukan dalam daftar (patientIdx masih -1).
    // Ini seharusnya tidak terjadi jika data sesi dan pasien konsisten.
    if (patientIdx == -1)
    {
        printError("Pasien tidak ditemukan!"); // Pesan error.
        return false;                          // Mengembalikan false.
    }

    // Mendapatkan pointer ke struktur Patient yang sesuai berdasarkan indeks yang ditemukan.
    Patient *patient = &hospital->patients.elements[patientIdx];

    // ========== PEMERIKSAAN STATUS ANTRIAN PASIEN ==========
    // Memeriksa apakah pasien saat ini sudah terdaftar dalam antrian untuk ruangan lain.
    // `patient->queueRoom[0] != '\0'` berarti field `queueRoom` pada struktur pasien tidak kosong,
    // yang mengindikasikan pasien sudah terasosiasi dengan suatu antrian/ruangan.
    if (patient->queueRoom[0] != '\0')
    {
        printError("Anda sudah terdaftar dalam antrian!"); // Pesan error jika pasien sudah ada di antrian lain.
        return false;                                      // Mengembalikan false.
    }

    // ========== MENGUMPULKAN DAFTAR DOKTER YANG TERSEDIA ==========
    // Dokter dianggap tersedia jika mereka sudah ditugaskan ke sebuah ruangan.
    int availableDoctorCount = 0; // Penghitung jumlah dokter yang tersedia.
    // Array sementara untuk menyimpan pointer ke dokter-dokter yang tersedia.
    // Ukuran array ini adalah jumlah total dokter, meskipun mungkin tidak semua slot terisi.
    Doctor *availableDoctors[hospital->doctors.nEff];

    // Melakukan iterasi melalui daftar semua dokter di rumah sakit.
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        Doctor *doctor = &hospital->doctors.elements[i]; // Pointer ke dokter saat ini.
        // Memeriksa apakah dokter tersebut sudah ditugaskan ke sebuah ruangan.
        // `doctor->room[0] != '\0'` berarti string `room` pada struktur dokter tidak kosong.
        if (doctor->room[0] != '\0')
        {
            // Jika dokter memiliki ruangan, tambahkan pointer dokter tersebut ke array `availableDoctors`.
            availableDoctors[availableDoctorCount] = doctor;
            availableDoctorCount++; // Increment jumlah dokter yang tersedia.
        }
    }

    // Jika tidak ada dokter yang tersedia (misalnya, semua dokter belum ditugaskan ke ruangan).
    if (availableDoctorCount == 0)
    {
        printError("Tidak ada dokter yang tersedia saat ini."); // Pesan error.
        return false;                                           // Mengembalikan false, tidak perlu menampilkan tabel dokter.
    }

    // ========== MENAMPILKAN TABEL DOKTER YANG TERSEDIA ==========
    printf("\nDaftar Dokter yang Tersedia:\n"); // Judul bagian.

    // Mendefinisikan lebar kolom dan header untuk tabel daftar dokter.
    // Kolom: Nomor, Nama Dokter, Spesialisasi, Ruangan, Jumlah Antrian Saat Ini, Aura, Biaya Checkup.
    int widths[] = {5, 20, 15, 10, 10, 10, 15};
    const char *headers[] = {"No", "Dokter", "Spesialisasi", "Ruangan", "Antrian", "Aura", "Biaya Checkup"};

    // Mencetak border atas dan baris header tabel menggunakan fungsi utilitas.
    printTableBorder(widths, 7, 1);    // 7 kolom, tipe border 1 (atas).
    printTableRow(headers, widths, 7); // Mencetak baris header.
    printTableBorder(widths, 7, 2);    // Mencetak garis pemisah di bawah header.

    // Melakukan iterasi melalui daftar dokter yang tersedia untuk ditampilkan di tabel.
    for (int i = 0; i < availableDoctorCount; i++)
    {
        Doctor *doctor = availableDoctors[i]; // Pointer ke dokter saat ini dalam daftar tersedia.
        int queueCount = 0;                   // Variabel untuk menyimpan jumlah pasien di antrian dokter ini.

        // Menghitung jumlah pasien saat ini di antrian untuk ruangan dokter tersebut.
        // Loop melalui daftar antrian aktif di rumah sakit.
        for (int j = 0; j < hospital->queues.nRooms; j++)
        {
            // Membandingkan kode ruangan antrian dengan kode ruangan dokter.
            if (strcmp(hospital->queues.queues[j].roomCode, doctor->room) == 0)
            {
                // Jika ditemukan antrian yang cocok, dapatkan ukurannya menggunakan `queueSize` (diasumsikan ada).
                queueCount = queueSize(&hospital->queues.queues[j]);
                break; // Keluar dari loop pencarian antrian.
            }
        }

        // Menyiapkan data dokter untuk ditampilkan dalam baris tabel.
        // Buffer untuk menyimpan representasi string dari nomor urut, jumlah antrian, aura, dan biaya.
        char numberStr[10], queueCountStr[10], auraStr[10], checkupCostStr[20];
        // Konversi nomor urut (i+1) ke string.
        integerToString(i + 1, numberStr, sizeof(numberStr));
        strcat(numberStr, ". "); // Tambahkan titik dan spasi setelah nomor.
        // Konversi jumlah antrian ke string.
        integerToString(queueCount, queueCountStr, sizeof(queueCountStr));
        // Konversi aura dokter ke string.
        integerToString(doctor->aura, auraStr, sizeof(auraStr));
        // Konversi biaya checkup (float) ke string dengan 2 angka desimal.
        floatToString(doctor->checkupCost, checkupCostStr, sizeof(checkupCostStr), 2);

        // Array string yang berisi data untuk satu baris tabel.
        const char *row[] = {
            numberStr,
            doctor->username,
            doctor->specialization,
            doctor->room,
            queueCountStr,
            auraStr,
            checkupCostStr
        };
        printTableRow(row, widths, 7); // Mencetak baris data dokter.
    }

    printTableBorder(widths, 7, 3); // Mencetak border bawah tabel.

    // ========== INPUT PILIHAN DOKTER ==========
    int doctorChoice = 0; // Variabel untuk menyimpan pilihan dokter oleh pasien (berbasis 1).
    // Loop validasi input: terus meminta input hingga pilihan valid (antara 1 dan `availableDoctorCount`).
    while (doctorChoice < 1 || doctorChoice > availableDoctorCount)
    {
        // Membangun string prompt untuk input, menampilkan rentang pilihan yang valid.
        char promptSelectDoctor[100] = "";
        strcat(promptSelectDoctor, "Pilih dokter (1 - ");
        char availableCountStr[10]; // Buffer untuk jumlah dokter tersedia sebagai string.
        integerToString(availableDoctorCount, availableCountStr, sizeof(availableCountStr));
        strcat(promptSelectDoctor, availableCountStr);
        strcat(promptSelectDoctor, "): ");

        // Membaca input integer dari pengguna. `readValidInt` adalah fungsi kustom yang menangani validasi.
        if (!readValidInt(&doctorChoice, promptSelectDoctor))
        {
            printError("Pilihan tidak valid!, harap masukkan angka yang sesuai."); // Pesan error jika input bukan integer yang valid.
            // Jika `readValidInt` gagal, fungsi ini akan return false, menghentikan proses pendaftaran.
            return false;
        }
        // Jika input adalah integer tapi di luar rentang, loop `while` akan berlanjut.
    }
    // Mendapatkan pointer ke struktur Doctor yang dipilih oleh pasien.
    // `doctorChoice - 1` karena pilihan pengguna berbasis 1, sedangkan indeks array `availableDoctors` berbasis 0.
    Doctor *doctor = availableDoctors[doctorChoice - 1];

    // ========== VALIDASI SALDO BANANARICH ==========
    // Memeriksa apakah saldo BananaRich pasien (`patient->bananaRich`) mencukupi untuk biaya checkup dokter yang dipilih.
    if (patient->bananaRich < doctor->checkupCost)
    {
        printError("Saldo BananaRich tidak cukup untuk checkup!"); // Pesan error.
        return false;                                              // Mengembalikan false.
    }

    // ========== MENCARI ATAU MEMBUAT ANTRIAN DOKTER ==========
    int queueIdx = -1; // Indeks antrian dokter dalam array `hospital->queues.queues`.
    // Mencari apakah sudah ada antrian yang terdaftar untuk ruangan dokter yang dipilih.
    for (int i = 0; i < hospital->queues.nRooms; i++) // Loop melalui antrian yang sudah aktif/terdaftar.
    {
        if (strcmp(hospital->queues.queues[i].roomCode, doctor->room) == 0)
        {
            queueIdx = i; // Jika ditemukan, simpan indeksnya.
            break;        // Keluar dari loop.
        }
    }

    // Jika tidak ada antrian yang ditemukan untuk ruangan dokter ini, maka buat antrian baru.
    if (queueIdx == -1)
    {
        // Memeriksa apakah kapasitas total untuk menyimpan antrian baru (`hospital->queues.nRooms`) sudah penuh.
        if (hospital->queues.nRooms >= hospital->queues.capacity)
        {
            printError("Kapasitas antrian penuh!"); // Pesan error.
            return false;                           // Mengembalikan false.
        }
        // Gunakan slot berikutnya yang tersedia di array `hospital->queues.queues` untuk antrian baru.
        queueIdx = hospital->queues.nRooms++;                 // Dapatkan indeks baru dan increment `nRooms`.
        Queue *newQueue = &hospital->queues.queues[queueIdx]; // Pointer ke struktur antrian baru.
        // Inisialisasi antrian baru dengan kode ruangan dokter.
        // `initializeQueue` adalah fungsi kustom (diasumsikan ada dan mengset `head`, `tail`, `size`, `roomCode`).
        initializeQueue(newQueue, doctor->room);
    }

    // Dapatkan pointer ke antrian yang akan digunakan (baik yang sudah ada maupun yang baru dibuat).
    Queue *queueToCheck = &hospital->queues.queues[queueIdx];

    // ========== MENCARI RUANGAN YANG DIPILIH ==========
    // Bagian ini mencari pointer ke struktur Room dari dokter yang dipilih.
    // Ini diperlukan untuk memeriksa kapasitas ruangan nanti.
    Room *selectedRoom = NULL; // Pointer ke ruangan yang dipilih, diinisialisasi NULL.
    // Iterasi melalui denah layout untuk menemukan ruangan yang cocok dengan `doctor->room`.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            Room *room = &hospital->layout.elements[i][j]; // Pointer ke ruangan saat ini di denah.
            if (strcmp(room->code, doctor->room) == 0)     // Jika kode ruangan cocok.
            {
                selectedRoom = room; // Simpan pointer ke ruangan yang dipilih.
                break;               // Keluar dari loop kolom.
            }
        }
        if (selectedRoom != NULL) // Jika ruangan sudah ditemukan di loop kolom.
            break;                // Keluar dari loop baris.
    }
    // Jika `selectedRoom` masih NULL setelah loop, berarti ada inkonsistensi data (dokter punya room code tapi room tidak ada di layout).
    // Namun, kode tidak secara eksplisit menangani kasus ini di sini, diasumsikan `selectedRoom` akan ditemukan jika `doctor->room` valid.

    // ========== MENYIMPAN DATA KESEHATAN PASIEN ==========
    // Menyalin data kesehatan dari array `healthData` ke field-field yang sesuai dalam struktur `Patient`.
    // Indeks 0 hingga 9 dari `healthData` digunakan untuk data kesehatan spesifik.
    patient->bodyTemperature = healthData[0];
    patient->systolicBloodPressure = (int)healthData[1];  // Cast ke int jika perlu.
    patient->diastolicBloodPressure = (int)healthData[2]; // Cast ke int.
    patient->heartRate = (int)healthData[3];              // Cast ke int.
    patient->oxygenSaturation = (int)healthData[4];       // Cast ke int.
    patient->bloodSugarLevel = healthData[5];
    patient->weight = healthData[6];
    patient->height = healthData[7];
    patient->cholesterolLevel = healthData[8];
    patient->platelets = (int)healthData[9]; // Cast ke int.

    // ========== PROSES PEMBAYARAN ==========
    // Mengurangi saldo BananaRich pasien sejumlah biaya checkup.
    patient->bananaRich -= doctor->checkupCost;
    // Menambahkan sebagian biaya (20%) ke saldo rumah sakit.
    hospital->finance.hospitalBalance += 0.2 * doctor->checkupCost;
    // Dokter mendapatkan sisa biaya (80%).
    doctor->bananaRich += 0.8 * doctor->checkupCost;

    // ========== MENAMBAHKAN RIWAYAT PENGOBATAN ==========
    // Memeriksa apakah kapasitas array riwayat pengobatan (`hospital->treatmentHistory`) perlu diperbesar.
    if (hospital->treatmentHistory.nEff >= hospital->treatmentHistory.capacity)
    {
        // Jika penuh, gandakan kapasitas array riwayat.
        // Alokasi ulang memori untuk `hospital->treatmentHistory.elements`.
        TreatmentHistory *newElements = (TreatmentHistory *)realloc(
            hospital->treatmentHistory.elements,
            sizeof(TreatmentHistory) * hospital->treatmentHistory.capacity * 2); // Kapasitas baru.

        // Jika realokasi memori gagal.
        if (!newElements)
        {
            printError("Gagal mengalokasikan ulang memori untuk riwayat pengobatan!");
            // Rollback perubahan finansial yang telah dilakukan karena pendaftaran gagal pada tahap ini.
            patient->bananaRich += doctor->checkupCost;
            hospital->finance.hospitalBalance -= 0.2 * doctor->checkupCost;
            doctor->bananaRich -= 0.8 * doctor->checkupCost;
            // Kapasitas array riwayat tidak diubah kembali di sini, namun `newElements` tidak diassign.
            return false; // Mengembalikan false.
        }
        // Jika realokasi berhasil, update kapasitas dan pointer elemen.
        hospital->treatmentHistory.capacity *= 2;          // Update kapasitas.
        hospital->treatmentHistory.elements = newElements; // Update pointer.
    }

    // Menambahkan entri baru ke riwayat pengobatan.
    // Mendapatkan pointer ke elemen berikutnya yang tersedia di array riwayat dan increment `nEff`.
    TreatmentHistory *history = &hospital->treatmentHistory.elements[hospital->treatmentHistory.nEff++];
    history->patientId = patient->id;              // Menyimpan ID pasien.
    history->doctorId = doctor->id;                // Menyimpan ID dokter.
    strcpy(history->room, doctor->room);           // Menyimpan kode ruangan tempat checkup.
    strcpy(history->disease, "Belum Didiagnosis"); // Status diagnosis awal.
    history->treatmentStatus = false;              // Status perawatan awal (belum selesai/diobati).

    // ========== PENGECEKAN RUANGAN KOSONG DAN PENUGASAN LANGSUNG ATAU ANTRIAN ==========
    // Memeriksa apakah ruangan dokter yang dipilih (`selectedRoom`) ditemukan dan memiliki kapasitas kosong.
    if (selectedRoom != NULL && selectedRoom->patientInRoom.nEff < selectedRoom->capacity)
    {
        // Jika ruangan ditemukan dan ada slot kosong, pasien langsung ditugaskan ke ruangan.
        strcpy(patient->queueRoom, doctor->room); // Set ruangan pasien.
        patient->queuePosition = 0;               // Posisi 0 menandakan di dalam ruangan, bukan antrian.
        // Tambahkan ID pasien ke daftar pasien di dalam ruangan.
        selectedRoom->patientInRoom.patientId[selectedRoom->patientInRoom.nEff++] = patient->id;

        // Menampilkan hasil pendaftaran (pasien langsung masuk ruangan).
        printf("\n");                       // Spasi.
        printHeader("Pendaftaran Checkup"); // Header.

        // Tabel konfirmasi.
        int widths2[] = {20, 20}; // Lebar kolom: Dokter, Ruangan.
        const char *headers2[] = {"Dokter", "Ruangan"};
        printTableBorder(widths2, 2, 1);
        printTableRow(headers2, widths2, 2);
        printTableBorder(widths2, 2, 2);
        const char *row2[] = {doctor->username, doctor->room}; // Data dokter dan ruangan.
        printTableRow(row2, widths2, 2);
        printTableBorder(widths2, 2, 3);

        // Pesan informasi bahwa pasien langsung masuk ruangan.
        printf(FORMAT_BOLD COLOR_YELLOW "[ℹ️  | Info]: Anda telah dipindahkan langsung ke ruangan dokter!\n" FORMAT_RESET);

        // Pesan sukses pendaftaran.
        char successMsg[100] = "";
        strcat(successMsg, "Pendaftaran checkup untuk ");
        strcat(successMsg, session->username);
        strcat(successMsg, " pada dr.");
        strcat(successMsg, doctor->username);
        strcat(successMsg, " di ruangan ");
        strcat(successMsg, doctor->room);
        strcat(successMsg, " berhasil!");
        printSuccess(successMsg);

        return true; // Pendaftaran berhasil, pasien masuk ruangan.
    }

    // ========== JIKA RUANGAN PENUH (atau selectedRoom NULL, meskipun seharusnya tidak), MASUKKAN KE ANTRIAN ==========
    // Jika kondisi di atas tidak terpenuhi (ruangan penuh atau `selectedRoom` NULL), pasien dimasukkan ke antrian.
    // Memanggil fungsi `enqueue` untuk menambahkan ID pasien ke antrian dokter yang dipilih (`queueToCheck`).
    if (!enqueue(queueToCheck, patient->id))
    {
        printError("Gagal menambahkan pasien ke antrian!"); // Pesan error jika enqueue gagal.
        // Rollback pembayaran karena gagal masuk antrian setelah bayar.
        patient->bananaRich += doctor->checkupCost;
        hospital->finance.hospitalBalance -= 0.2 * doctor->checkupCost;
        doctor->bananaRich -= 0.8 * doctor->checkupCost;
        // Rollback riwayat pengobatan yang mungkin sudah ditambahkan.
        hospital->treatmentHistory.nEff--;
        return false; // Mengembalikan false.
    }

    // Update informasi antrian pada data pasien.
    strcpy(patient->queueRoom, doctor->room);         // Simpan kode ruangan antrian.
    patient->queuePosition = queueSize(queueToCheck); // Simpan posisi pasien saat ini dalam antrian.

    // Menampilkan hasil pendaftaran (pasien masuk antrian).
    printf("\n");                       // Spasi.
    printHeader("Pendaftaran Checkup"); // Header.

    // Tabel konfirmasi.
    int widths2[] = {20, 20};
    const char *headers2[] = {"Dokter", "Ruangan"};
    printTableBorder(widths2, 2, 1);
    printTableRow(headers2, widths2, 2);
    printTableBorder(widths2, 2, 2);
    const char *row2[] = {doctor->username, doctor->room};
    printTableRow(row2, widths2, 2);
    printTableBorder(widths2, 2, 3);

    // Menampilkan posisi antrian pasien.
    printf(FORMAT_BOLD COLOR_YELLOW "[ℹ️  | Info]: Posisi antrian: %d\n" FORMAT_RESET,
           patient->queuePosition);

    // Pesan sukses pendaftaran.
    char successMsg[100] = "";
    strcat(successMsg, "Pendaftaran checkup untuk ");
    strcat(successMsg, session->username);
    strcat(successMsg, " pada dr.");
    strcat(successMsg, doctor->username);
    strcat(successMsg, " di ruangan ");
    strcat(successMsg, doctor->room);
    strcat(successMsg, " berhasil!");
    printSuccess(successMsg);

    return true; // Pendaftaran berhasil, pasien masuk antrian.
}