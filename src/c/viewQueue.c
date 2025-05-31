#include "viewQueue.h"

void displayQueue(Hospital *hospital, Session *session)
{
    // Pemeriksaan Awal: Validasi input pointer.
    // Memastikan bahwa pointer ke struktur Hospital dan Session tidak NULL.
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!"); // Pesan error jika ada input yang NULL.
        return;                                                    // Mengembalikan dari fungsi karena operasi tidak dapat dilanjutkan.
    }

    // Pemeriksaan Hak Akses: Hanya Manajer yang dapat melihat semua antrian.
    // Memeriksa apakah pengguna saat ini sudah login (`session->isLoggedIn`)
    // dan apakah perannya adalah MANAGER (`session->role == MANAGER`).
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat antrian."); // Pesan error jika hak akses tidak sesuai.
        return;                                                                 // Mengembalikan dari fungsi.
    }

    // Pemeriksaan Denah Rumah Sakit: Memastikan denah tidak kosong.
    // Jika jumlah baris atau kolom efektif adalah 0 atau kurang, maka tidak ada ruangan untuk ditampilkan.
    if (hospital->layout.rowEff <= 0 || hospital->layout.colEff <= 0)
    {
        printError("Denah rumah sakit kosong!"); // Pesan error jika denah kosong.
        return;                                  // Mengembalikan dari fungsi.
    }

    // Mencetak header utama untuk halaman status antrian.
    printHeader("Status Antrian");
    // Menampilkan denah rumah sakit. Argumen `false` mungkin berarti tidak mencetak header dari fungsi `displayLayout`.
    displayLayout(hospital, session, false);

    // Flag untuk melacak apakah ada setidaknya satu ruangan dengan dokter yang bertugas.
    boolean hasRoomsWithDoctor = false;

    // Melakukan iterasi melalui setiap sel (ruangan) dalam denah rumah sakit.
    // Loop untuk baris denah.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        // Loop untuk kolom denah.
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            // Mendapatkan pointer ke struktur Room saat ini.
            Room *room = &hospital->layout.elements[i][j];
            // Memeriksa apakah ada dokter yang ditugaskan ke ruangan ini.
            // `room->doctorId != -1` menandakan ada dokter (diasumsikan -1 berarti tidak ada dokter).
            if (room->doctorId != -1)
            {
                // Jika ada dokter di ruangan, set flag menjadi true.
                hasRoomsWithDoctor = true;
                // Mencetak header spesifik untuk antrian ruangan ini.
                char header[100] = "Antrian Ruangan "; // Buffer untuk judul header.
                strcat(header, room->code);            // Menambahkan kode ruangan ke judul.
                printHeader(header);                   // Mencetak header antrian ruangan.

                // Menampilkan Kapasitas Ruangan.
                char capacityStr[20];          // Buffer untuk menyimpan string kapasitas.
                int capacity = room->capacity; // Mengambil nilai kapasitas ruangan.
                // Mengkonversi nilai integer kapasitas menjadi string menggunakan fungsi `integerToString`.
                // `sizeof(capacityStr)` adalah ukuran buffer.
                if (!integerToString(capacity, capacityStr, sizeof(capacityStr)))
                {
                    // Jika konversi gagal, cetak error dan hentikan untuk ruangan ini (atau seluruh fungsi).
                    // Perilaku return di sini akan menghentikan seluruh fungsi displayQueue.
                    return printError("Kapasitas ruangan tidak valid!");
                }

                // Menambahkan " orang" ke string kapasitas.
                strcat(capacityStr, " orang");
                // Data untuk tabel kapasitas.
                const char *row1[] = {"Kapasitas", capacityStr};
                int widths[] = {15, 10}; // Lebar kolom untuk tabel (15 untuk label, 10 untuk nilai).
                // Mencetak tabel kapasitas.
                printTableBorder(widths, 2, 1); // Border atas.
                printTableRow(row1, widths, 2); // Baris data.
                printTableBorder(widths, 2, 3); // Border bawah.

                // Menampilkan Dokter yang Bertugas di Ruangan.
                printf("Dokter:\n");     // Label untuk bagian dokter.
                char *doctorName = NULL; // Pointer untuk menyimpan nama dokter, diinisialisasi NULL.
                // Mencari nama dokter berdasarkan `room->doctorId` di daftar dokter rumah sakit.
                for (int k = 0; k < hospital->doctors.nEff; k++)
                {
                    if (hospital->doctors.elements[k].id == room->doctorId)
                    {
                        doctorName = hospital->doctors.elements[k].username; // Simpan username dokter.
                        break;                                               // Keluar dari loop setelah dokter ditemukan.
                    }
                }
                // Jika nama dokter tidak ditemukan atau tidak ada dokter terdaftar.
                if (doctorName == NULL || hospital->doctors.nEff <= 0)
                {
                    printf(COLOR_YELLOW "  Tidak ada dokter\n" COLOR_RESET); // Cetak pesan.
                }
                else
                {
                    printf(COLOR_YELLOW "  %s\n" COLOR_RESET, doctorName); // Cetak nama dokter.
                }

                // Menampilkan Pasien yang Saat Ini Berada di Dalam Ruangan.
                printf("Pasien di dalam ruangan:\n"); // Label.
                // Memeriksa apakah ada pasien di dalam ruangan atau tidak ada pasien terdaftar di rumah sakit.
                if (room->patientInRoom.nEff == 0 || hospital->patients.nEff <= 0)
                {
                    printf(COLOR_YELLOW "  Tidak ada pasien\n" COLOR_RESET); // Pesan jika tidak ada.
                }
                else
                {
                    // Melakukan iterasi melalui daftar ID pasien yang ada di dalam ruangan.
                    for (int k = 0; k < room->patientInRoom.nEff; k++)
                    {
                        boolean found = false; // Flag untuk menandakan apakah detail pasien ditemukan.
                        // Mencari detail pasien (username) berdasarkan ID.
                        for (int l = 0; l < hospital->patients.nEff; l++)
                        {
                            if (hospital->patients.elements[l].id == room->patientInRoom.patientId[k])
                            {
                                // Cetak nomor urut dan username pasien.
                                printf(COLOR_YELLOW "  %d. %s\n" COLOR_RESET, k + 1, hospital->patients.elements[l].username);
                                found = true; // Set flag ditemukan.
                                break;        // Keluar dari loop pencarian pasien.
                            }
                        }
                        if (!found) // Jika pasien dengan ID tersebut tidak ditemukan di daftar pasien utama.
                        {
                            printf(COLOR_YELLOW "  %d. Pasien tidak ditemukan\n" COLOR_RESET, k + 1);
                        }
                    }
                }

                // Menampilkan Pasien yang Berada dalam Antrian untuk Ruangan Ini.
                printf("Pasien di antrian:\n"); // Label.
                boolean foundQueue = false;     // Flag untuk menandakan apakah antrian untuk ruangan ini ditemukan.
                // Mencari antrian yang sesuai dengan kode ruangan saat ini.
                for (int k = 0; k < hospital->queues.nRooms; k++)
                {
                    if (strcmp(hospital->queues.queues[k].roomCode, room->code) == 0)
                    {
                        foundQueue = true;
                        Queue *currentQueue = &hospital->queues.queues[k];
                        if (isQueueEmpty(currentQueue) || hospital->patients.nEff <= 0)
                        {
                            printf(COLOR_YELLOW "  Tidak ada pasien di antrian\n" COLOR_RESET);
                        }
                        else
                        {
                            QueueNode *currentNode = currentQueue->head; // Updated from front
                            int position = 1;
                            while (currentNode != NULL)
                            {
                                boolean patientFoundInList = false;
                                for (int m = 0; m < hospital->patients.nEff; m++)
                                {
                                    if (hospital->patients.elements[m].id == currentNode->info.patientId)
                                    {
                                        printf(COLOR_YELLOW "  %d. %s\n" COLOR_RESET, position, hospital->patients.elements[m].username);
                                        patientFoundInList = true;
                                        break;
                                    }
                                }
                                if (!patientFoundInList)
                                {
                                    printf(COLOR_YELLOW "  %d. Pasien ID %d tidak ditemukan dalam daftar pasien utama\n" COLOR_RESET, position, currentNode->info.patientId);
                                }
                                currentNode = currentNode->next;
                                position++;
                            }
                        }
                        break;
                    }
                }
                
                if (!foundQueue) // Jika setelah loop, antrian untuk ruangan ini tidak ditemukan (seharusnya ada jika dokter ada).
                {
                    printf(COLOR_YELLOW "  Tidak ada pasien di antrian\n" COLOR_RESET); // Atau "Antrian untuk ruangan ini tidak ditemukan."
                }
            } // Akhir dari `if (room->doctorId != -1)`
        } // Akhir dari loop kolom (j)
    } // Akhir dari loop baris (i)

    // Jika setelah memeriksa semua ruangan, tidak ada satupun ruangan yang memiliki dokter.
    if (!hasRoomsWithDoctor)
    {
        printf(COLOR_YELLOW "Tidak ada ruangan dengan dokter saat ini.\n" COLOR_RESET); // Cetak pesan.
    }
}