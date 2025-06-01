// Menyertakan file header "viewQueue.h".
// File ini diasumsikan berisi deklarasi fungsi displayQueue,
// definisi struktur Hospital (menyimpan data rumah sakit), Session (informasi sesi pengguna),
// Room (detail ruangan), Queue (struktur data antrian), QueueNode (node dalam antrian),
// serta makro (seperti COLOR_YELLOW, COLOR_RESET untuk pewarnaan teks) dan
// deklarasi fungsi utilitas lainnya yang relevan seperti
// printError (mencetak pesan kesalahan), printHeader (mencetak judul),
// displayLayout (menampilkan denah), integerToString (mengkonversi integer ke string),
// printTableBorder (mencetak batas tabel), printTableRow (mencetak baris tabel),
// isQueueEmpty (memeriksa apakah antrian kosong), dan queueSize (mendapatkan ukuran antrian).
#include "viewQueue.h"
#include <stdio.h>  // Diperlukan untuk fungsi input/output standar (misalnya, printf).
#include <string.h> // Diperlukan untuk fungsi manipulasi string seperti strcat, strcmp.

// Fungsi untuk menampilkan status antrian di setiap ruangan yang memiliki dokter.
// Hanya Manajer yang dapat mengakses fungsi ini.
// Fungsi ini akan menampilkan denah rumah sakit, kemudian untuk setiap ruangan yang memiliki dokter,
// akan ditampilkan detail kapasitas, nama dokter, pasien di dalam ruangan, dan pasien dalam antrian.
// Parameter:
//   hospital: pointer ke struktur Hospital yang menyimpan semua data rumah sakit.
//   session: pointer ke struktur Session yang menyimpan informasi sesi pengguna saat ini.
void displayQueue(Hospital *hospital, Session *session)
{
    // Pemeriksaan Awal: Validasi input pointer.
    // Memastikan bahwa pointer ke struktur Hospital dan Session tidak NULL untuk menghindari dereferensi NULL.
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
    // Jika jumlah baris (`hospital->layout.rowEff`) atau kolom (`hospital->layout.colEff`) efektif adalah 0 atau kurang,
    // maka tidak ada ruangan untuk ditampilkan antriannya.
    if (hospital->layout.rowEff <= 0 || hospital->layout.colEff <= 0)
    {
        printError("Denah rumah sakit kosong!"); // Pesan error jika denah kosong.
        return;                                  // Mengembalikan dari fungsi.
    }

    // Mencetak header utama untuk halaman status antrian.
    printHeader("Status Antrian");
    // Menampilkan denah rumah sakit. Argumen `false` untuk `printHeaderFlag` di `displayLayout`
    // berarti `displayLayout` tidak akan mencetak headernya sendiri, karena header sudah dicetak di sini.
    displayLayout(hospital, session, false);

    // Flag untuk melacak apakah ada setidaknya satu ruangan yang memiliki dokter yang bertugas.
    // Digunakan untuk menampilkan pesan jika tidak ada dokter sama sekali.
    boolean hasRoomsWithDoctor = false;

    // Melakukan iterasi melalui setiap sel (ruangan) dalam denah rumah sakit.
    // Loop untuk baris denah.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        // Loop untuk kolom denah.
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            // Mendapatkan pointer ke struktur Room saat ini pada posisi (i, j) di denah.
            Room *room = &hospital->layout.elements[i][j];
            // Memeriksa apakah ada dokter yang ditugaskan ke ruangan ini.
            // `room->doctorId != -1` menandakan ada dokter (diasumsikan ID dokter -1 berarti tidak ada dokter).
            if (room->doctorId != -1)
            {
                // Jika ada dokter di ruangan, set flag `hasRoomsWithDoctor` menjadi true.
                hasRoomsWithDoctor = true;
                // Mencetak header spesifik untuk antrian ruangan ini.
                char header[100] = "Antrian Ruangan "; // Buffer untuk judul header.
                strcat(header, room->code);            // Menambahkan kode ruangan (misal "A1") ke judul.
                printHeader(header);                   // Mencetak header antrian ruangan.

                // Menampilkan Kapasitas Ruangan.
                char capacityStr[20];          // Buffer untuk menyimpan string kapasitas.
                int capacity = room->capacity; // Mengambil nilai kapasitas ruangan.
                // Mengkonversi nilai integer kapasitas menjadi string menggunakan fungsi `integerToString`.
                // `sizeof(capacityStr)` adalah ukuran buffer untuk mencegah overflow.
                if (!integerToString(capacity, capacityStr, sizeof(capacityStr)))
                {
                    // Jika konversi gagal, cetak error dan hentikan seluruh fungsi `displayQueue`.
                    // Ini adalah perilaku `return` di tengah fungsi.
                    return printError("Kapasitas ruangan tidak valid!");
                }

                // Menambahkan teks " orang" ke string kapasitas untuk tampilan yang lebih jelas.
                strcat(capacityStr, " orang");
                // Data untuk tabel kapasitas (label dan nilai).
                const char *row1[] = {"Kapasitas", capacityStr};
                int widths[] = {15, 10}; // Lebar kolom untuk tabel (15 untuk label "Kapasitas", 10 untuk nilainya).
                // Mencetak tabel kapasitas menggunakan fungsi utilitas.
                printTableBorder(widths, 2, 1); // Border atas tabel (2 kolom, tipe border 1).
                printTableRow(row1, widths, 2); // Baris data kapasitas.

                // Menampilkan Dokter yang Bertugas di Ruangan.
                printf("Dokter:\n");     // Label untuk bagian informasi dokter.
                char *doctorName = NULL; // Pointer untuk menyimpan nama dokter, diinisialisasi NULL.
                // Mencari nama dokter berdasarkan `room->doctorId` di daftar dokter rumah sakit (`hospital->doctors`).
                for (int k = 0; k < hospital->doctors.nEff; k++) // Loop melalui dokter yang efektif.
                {
                    // Jika ID dokter di daftar cocok dengan ID dokter di ruangan.
                    if (hospital->doctors.elements[k].id == room->doctorId)
                    {
                        doctorName = hospital->doctors.elements[k].username; // Simpan username dokter.
                        break;                                               // Keluar dari loop setelah dokter ditemukan.
                    }
                }
                // Jika nama dokter tidak ditemukan (pointer masih NULL) atau tidak ada dokter terdaftar di rumah sakit.
                if (doctorName == NULL || hospital->doctors.nEff <= 0)
                {
                    printf(COLOR_YELLOW "  Tidak ada dokter\n" COLOR_RESET); // Cetak pesan bahwa tidak ada dokter.
                }
                else
                {
                    // Data untuk tabel kapasitas (dokter dan nama).
                    const char *row1[] = {"Dokter", doctorName};
                    int widths[] = {15, 10}; // Lebar kolom untuk tabel (15 untuk label "Dokter", 10 untuk nama dokternya).
                    // Mencetak tabel kapasitas menggunakan fungsi utilitas.
                    printTableBorder(widths, 2, 2);                        // Border atas tabel (2 kolom, tipe border 2).
                    printTableRow(row1, widths, 2);                        // Baris data kapasitas.
                    printTableBorder(widths, 2, 3);                        // Border bawah tabel (tipe border 3).
                }

                // Menampilkan Pasien yang Saat Ini Berada di Dalam Ruangan.
                // Ini adalah pasien yang sudah masuk dan sedang/selesai diperiksa, bukan yang di antrian.
                printf("Pasien di dalam ruangan:\n"); // Label.
                // Memeriksa apakah ada pasien di dalam ruangan (`room->patientInRoom.nEff > 0`)
                // atau tidak ada pasien terdaftar di rumah sakit secara umum (`hospital->patients.nEff <= 0`).
                if (room->patientInRoom.nEff == 0 || hospital->patients.nEff <= 0)
                {
                    printf(COLOR_YELLOW "  Tidak ada pasien\n" COLOR_RESET); // Pesan jika tidak ada pasien di ruangan.
                }
                else
                {
                    // Melakukan iterasi melalui daftar ID pasien yang tercatat ada di dalam ruangan ini.
                    for (int k = 0; k < room->patientInRoom.nEff; k++)
                    {
                        boolean found = false; // Flag untuk menandakan apakah detail pasien (username) ditemukan.
                        // Mencari detail pasien (username) di daftar pasien rumah sakit berdasarkan ID.
                        for (int l = 0; l < hospital->patients.nEff; l++)
                        {
                            // Jika ID pasien di daftar cocok dengan ID pasien yang ada di ruangan.
                            if (hospital->patients.elements[l].id == room->patientInRoom.patientId[k])
                            {
                                // Cetak nomor urut (k+1) dan username pasien.
                                printf(COLOR_YELLOW "  %d. %s\n" COLOR_RESET, k + 1, hospital->patients.elements[l].username);
                                found = true; // Set flag bahwa detail pasien ditemukan.
                                break;        // Keluar dari loop pencarian pasien karena sudah ditemukan.
                            }
                        }
                        if (!found) // Jika pasien dengan ID tersebut tidak ditemukan di daftar pasien utama (inkonsistensi data).
                        {
                            printf(COLOR_YELLOW "  %d. Pasien tidak ditemukan\n" COLOR_RESET, k + 1);
                        }
                    }
                }

                // Menampilkan Pasien yang Berada dalam Antrian untuk Ruangan Ini.
                printf("Pasien di antrian:\n"); // Label.
                boolean foundQueue = false;     // Flag untuk menandakan apakah antrian untuk ruangan ini ditemukan dalam `hospital->queues`.
                // Mencari antrian yang sesuai dengan kode ruangan (`room->code`) saat ini.
                // `hospital->queues.nRooms` adalah jumlah antrian yang aktif dan terdaftar.
                for (int k = 0; k < hospital->queues.nRooms; k++)
                {
                    // Membandingkan kode ruangan dari antrian yang tersimpan dengan kode ruangan saat ini.
                    if (strcmp(hospital->queues.queues[k].roomCode, room->code) == 0)
                    {
                        foundQueue = true; // Antrian untuk ruangan ini ditemukan.
                        // Mendapatkan pointer ke struktur Queue yang sesuai.
                        Queue *currentQueue = &hospital->queues.queues[k];
                        // Memeriksa apakah antrian kosong menggunakan `isQueueEmpty` (fungsi utilitas)
                        // atau jika tidak ada pasien terdaftar di rumah sakit.
                        if (isQueueEmpty(currentQueue) || hospital->patients.nEff <= 0)
                        {
                            printf(COLOR_YELLOW "  Tidak ada pasien di antrian\n" COLOR_RESET); // Pesan jika antrian kosong.
                        }
                        else
                        {
                            // Jika antrian tidak kosong, lakukan iterasi melalui node-node antrian (linked list).
                            QueueNode *currentNode = currentQueue->head; // Mulai dari kepala (head) antrian. (Sebelumnya `front`)
                            int position = 1;                            // Nomor urut pasien dalam antrian, dimulai dari 1.
                            while (currentNode != NULL)                  // Selama masih ada node dalam antrian.
                            {
                                boolean patientFoundInList = false; // Flag apakah pasien di antrian ini ditemukan di daftar pasien utama.
                                // Mencari detail pasien (username) berdasarkan ID pasien yang ada di node antrian.
                                for (int m = 0; m < hospital->patients.nEff; m++)
                                {
                                    if (hospital->patients.elements[m].id == currentNode->info.patientId)
                                    {
                                        // Cetak nomor antrian dan username pasien.
                                        printf(COLOR_YELLOW "  %d. %s\n" COLOR_RESET, position, hospital->patients.elements[m].username);
                                        patientFoundInList = true; // Pasien ditemukan di daftar utama.
                                        break;                     // Keluar dari loop pencarian pasien.
                                    }
                                }
                                if (!patientFoundInList) // Jika pasien dengan ID di antrian tidak ditemukan di daftar pasien utama (inkonsistensi data).
                                {
                                    printf(COLOR_YELLOW "  %d. Pasien ID %d tidak ditemukan dalam daftar pasien utama\n" COLOR_RESET, position, currentNode->info.patientId);
                                }
                                currentNode = currentNode->next; // Pindah ke node antrian berikutnya.
                                position++;                      // Increment nomor urut antrian.
                            }
                        }
                        break; // Keluar dari loop pencarian antrian (`for k`) karena antrian untuk ruangan ini sudah diproses.
                    }
                }
                // Jika setelah loop `for k`, antrian untuk ruangan ini tidak ditemukan di `hospital->queues`.
                // Ini bisa terjadi jika ruangan memiliki dokter tapi belum ada pasien yang mendaftar antrian ke sana,
                // sehingga entri antriannya belum dibuat atau `nRooms` belum mencakupnya.
                if (!foundQueue)
                {
                    printf(COLOR_YELLOW "  Tidak ada pasien di antrian\n" COLOR_RESET); // Atau bisa juga pesan "Antrian untuk ruangan ini belum aktif."
                }
            }
        }
    }

    // Jika setelah memeriksa semua ruangan, tidak ada satupun ruangan yang memiliki dokter yang bertugas.
    if (!hasRoomsWithDoctor)
    {
        printf(COLOR_YELLOW "Tidak ada ruangan dengan dokter saat ini.\n" COLOR_RESET); // Cetak pesan.
    }
}