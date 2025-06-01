#include "viewPatientQueue.h"

void viewMySpecificQueueStatus(Hospital *hospital, Session *session)
{
    // Validasi Sesi: Memastikan pengguna sudah login dan berperan sebagai Pasien.
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat melihat status antrian."); // Pesan error jika akses tidak sesuai.
        return;                                                                       // Keluar dari fungsi.
    }

    // Mencari Data Pasien yang Sedang Login.
    // Menggunakan username dari sesi untuk menemukan entri pasien yang sesuai.
    Patient *patient = NULL; // Pointer ke struktur Patient, diinisialisasi NULL.
    // Melakukan iterasi melalui daftar pasien di rumah sakit.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        // Membandingkan username pasien di daftar dengan username dari sesi.
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
        {
            patient = &hospital->patients.elements[i]; // Jika ditemukan, simpan pointer ke pasien tersebut.
            break;                                     // Keluar dari loop.
        }
    }

    // Jika data pasien tidak ditemukan di daftar pasien rumah sakit (seharusnya tidak terjadi jika sesi valid).
    if (patient == NULL)
    {
        printError("Pasien tidak ditemukan dalam daftar!"); // Pesan error.
        return;                                             // Keluar dari fungsi.
    }

    // Kasus 1: Memeriksa apakah pasien tercatat sedang berada di sebuah ruangan (berdasarkan `patient->queueRoom`).
    // `patient->queueRoom[0] != '\0'` berarti field `queueRoom` pada struktur pasien tidak kosong,
    // yang mengindikasikan pasien terasosiasi dengan suatu ruangan (entah di dalam atau di antrian).
    if (patient->queueRoom[0] != '\0')
    {
        // Verifikasi tambahan: apakah pasien benar-benar ada di dalam daftar `patientInRoom` pada ruangan tersebut.
        boolean roomFound = false;     // Flag apakah ruangan yang tercatat di `patient->queueRoom` ditemukan di denah.
        boolean patientInRoom = false; // Flag apakah ID pasien ditemukan dalam daftar `patientInRoom` di ruangan tersebut.

        // Mencari ruangan berdasarkan `patient->queueRoom` di denah rumah sakit.
        for (int r = 0; r < hospital->layout.rowEff; r++) // Loop baris denah.
        {
            for (int c = 0; c < hospital->layout.colEff; c++) // Loop kolom denah.
            {
                Room *room = &hospital->layout.elements[r][c]; // Pointer ke ruangan saat ini di denah.
                // Membandingkan kode ruangan di denah dengan kode ruangan yang tercatat pada pasien.
                if (strcmp(room->code, patient->queueRoom) == 0)
                {
                    roomFound = true; // Ruangan ditemukan di denah.

                    // Periksa apakah ID pasien ada dalam daftar `patientInRoom` di ruangan ini.
                    // `room->patientInRoom.patientId` adalah array ID pasien di dalam ruangan.
                    // `room->patientInRoom.nEff` adalah jumlah pasien di dalam ruangan.
                    for (int i = 0; i < room->patientInRoom.nEff; i++)
                    {
                        if (room->patientInRoom.patientId[i] == patient->id)
                        {
                            patientInRoom = true; // Pasien ditemukan di dalam ruangan.
                            break;                // Keluar dari loop pencarian pasien di ruangan.
                        }
                    }

                    // Jika pasien ditemukan di dalam ruangan.
                    if (patientInRoom)
                    {
                        printf(COLOR_YELLOW "Anda sedang berada di dalam ruangan dokter! Silakan menyelesaikan pemeriksaan.\n" COLOR_RESET);
                        return; // Keluar dari fungsi karena status sudah jelas.
                    }
                    // Jika ruangan ditemukan tapi pasien tidak ada di daftar `patientInRoom` ruangan tersebut,
                    // loop akan berlanjut untuk memastikan tidak ada duplikasi room code (seharusnya tidak ada),
                    // atau logika akan jatuh ke pengecekan antrian di bawah.
                    // Namun, jika `patientInRoom` adalah satu-satunya kondisi "di dalam ruangan", dan pasien tidak di sana,
                    // maka pasien seharusnya ada di antrian untuk ruangan `patient->queueRoom`.
                    break; // Keluar dari loop kolom setelah ruangan ditemukan.
                }
            }
            if (roomFound)
                break; // Keluar dari loop baris jika ruangan sudah ditemukan.
        }

        // Jika ruangan yang tercatat di `patient->queueRoom` tidak ditemukan di denah (inkonsistensi data).
        if (!roomFound) // Ini hanya akan tercapai jika loop di atas selesai tanpa `roomFound = true`.
        {
            printError("Ruangan yang terdaftar untuk Anda tidak ditemukan!");
            return;
        }
        // Jika `roomFound` true TAPI `patientInRoom` false, berarti pasien terdaftar di `patient->queueRoom`
        // tapi tidak secara eksplisit di `room->patientInRoom`. Ini mengarah ke pengecekan antrian.
    }

    // Kasus 2: Pasien terdaftar dalam antrian untuk sebuah ruangan (dicek lagi `patient->queueRoom`).
    // Kondisi ini juga mencakup kasus di atas jika `patientInRoom` adalah false.
    if (patient->queueRoom[0] != '\0')
    {
        // Temukan dokter yang bertugas di ruangan yang tercatat pada `patient->queueRoom`.
        Doctor *doctor = NULL; // Pointer ke dokter, diinisialisasi NULL.
        // Loop melalui daftar dokter.
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            // Membandingkan kode ruangan dokter dengan kode ruangan antrian pasien.
            if (strcmp(hospital->doctors.elements[i].room, patient->queueRoom) == 0)
            {
                doctor = &hospital->doctors.elements[i]; // Dokter ditemukan.
                break;                                   // Keluar dari loop.
            }
        }

        // Jika dokter yang bertugas di ruangan tersebut ditemukan.
        if (doctor != NULL)
        {
            // Cek apakah ID pasien ada dalam data antrian (struktur Queue) untuk ruangan tersebut.
            boolean patientFoundInQueue = false; // Flag apakah pasien ditemukan di struktur antrian.
            int queueIdx = -1;                   // Indeks antrian ruangan di `hospital->queues.queues`.
            // Cari antrian yang sesuai dengan `patient->queueRoom`.
            for (int i = 0; i < hospital->queues.nRooms; i++) // `nRooms` adalah jumlah antrian aktif.
            {
                if (strcmp(hospital->queues.queues[i].roomCode, patient->queueRoom) == 0)
                {
                    queueIdx = i; // Indeks antrian ditemukan.
                    break;
                }
            }

            // Jika tidak ada struktur antrian yang cocok dengan `patient->queueRoom` (inkonsistensi data).
            if (queueIdx == -1)
            {
                // Ini berarti `patient->queueRoom` terisi tapi tidak ada antrian formal yang terdaftar untuk ruangan itu,
                // atau pasien tidak ada di antrian tersebut. Pesan error bisa lebih spesifik.
                printError("Antrian untuk ruangan Anda tidak ditemukan atau Anda tidak ada di dalamnya!");
                return;
            }

            // Dapatkan pointer ke struktur antrian yang relevan.
            Queue *queueToCheck = &hospital->queues.queues[queueIdx];

            // Lakukan iterasi (traverse) pada linked list antrian untuk menemukan ID pasien.
            QueueNode *currentNode = queueToCheck->head; // Mulai dari kepala antrian.
            while (currentNode != NULL)                  // Selama masih ada node.
            {
                if (currentNode->info.patientId == patient->id) // Jika ID pasien di node cocok.
                {
                    patientFoundInQueue = true; // Pasien ditemukan di struktur antrian.
                    break;                      // Keluar dari loop.
                }
                currentNode = currentNode->next; // Pindah ke node berikutnya.
            }

            // Jika pasien tidak ditemukan di struktur antrian setelah iterasi.
            // Ini bisa berarti `patient->queueRoom` terisi, tapi pasien sudah keluar dari antrian atau ada kesalahan data.
            if (!patientFoundInQueue)
            {
                printError("Anda belum terdaftar dalam antrian check-up! Silakan daftar terlebih dahulu dengan command DAFTAR_CHECKUP.");
                // Reset status antrian pasien supaya data singkron.
                patient->queueRoom[0] = '\0';
                patient->queuePosition = 0;
                return;
            }

            // Jika pasien ditemukan di antrian, siapkan informasi untuk ditampilkan.
            // Membuat format string untuk posisi antrian: "Posisi / Total".
            char queuePositionStr[5], queueSizeStr[5], finalQueueFormat[10] = "";
            // Konversi posisi antrian pasien (integer) ke string.
            integerToString(patient->queuePosition, queuePositionStr, sizeof(queuePositionStr));
            // Konversi ukuran total antrian (integer) ke string.
            integerToString(queueSize(queueToCheck), queueSizeStr, sizeof(queueSizeStr));
            // Gabungkan string-string tersebut.
            strcat(finalQueueFormat, queuePositionStr);
            strcat(finalQueueFormat, "/");
            strcat(finalQueueFormat, queueSizeStr);

            // Tampilkan informasi antrian dalam format tabel.
            printHeader("Status antrian Anda:"); // Header.

            // Definisi lebar kolom dan data untuk tabel.
            int widths[] = {20, 20}; // Lebar untuk kolom "Dokter" dan "Ruangan"/"Posisi antrian".
            const char *row1[] = {"Dokter", doctor->username};
            const char *row2[] = {"Ruangan", doctor->room};
            const char *row3[] = {"Posisi antrian", finalQueueFormat};

            // Mencetak tabel.
            printTableBorder(widths, 2, 1); // Border atas.
            printTableRow(row1, widths, 2); // Baris info dokter.
            printTableBorder(widths, 2, 2); // Pemisah.
            printTableRow(row2, widths, 2); // Baris info ruangan.
            printTableBorder(widths, 2, 2); // Pemisah.
            printTableRow(row3, widths, 2); // Baris info posisi antrian.
            printTableBorder(widths, 2, 3); // Border bawah.

            return; // Keluar dari fungsi setelah menampilkan status.
        }
        // Jika dokter untuk `patient->queueRoom` tidak ditemukan (inkonsistensi data).
        else
        {
            printError("Dokter untuk ruangan antrian Anda tidak ditemukan!");
            // Mungkin perlu mereset `patient->queueRoom` di sini.
            // patient->queueRoom[0] = '\0'; patient->queuePosition = 0; (Tidak ada di skrip asli)
            return;
        }
    }

    // Kasus 3: Pasien belum terdaftar dalam antrian manapun (`patient->queueRoom` kosong).
    // Ini akan tercapai jika kondisi `if (patient->queueRoom[0] != '\0')` di awal adalah false.
    printError("Anda belum terdaftar dalam antrian check-up! Silakan daftar terlebih dahulu dengan command DAFTAR_CHECKUP.");
}
