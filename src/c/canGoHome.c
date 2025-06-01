#include "canGoHome.h"

boolean canGoHome(Hospital *hospital, Session *session)
{
    // ========== VALIDASI INPUT AWAL ==========
    // Memeriksa apakah pointer ke struktur Hospital dan Session tidak NULL.
    // Ini penting untuk mencegah dereferensi NULL pointer.
    if (!hospital || !session) // Cara singkat untuk memeriksa apakah pointer NULL.
    {
        printError("Struktur rumah sakit atau sesi tidak valid!"); // Pesan error jika input tidak valid.
        return false;                                              // Mengembalikan false menandakan kegagalan.
    }

    // ========== VALIDASI SESI LOGIN DAN PERAN PENGGUNA ==========
    // Memastikan bahwa pengguna yang memanggil fungsi ini sudah login (`session->isLoggedIn`)
    // dan memiliki peran sebagai Pasien (`session->role == PATIENT`).
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat memeriksa status pulang."); // Pesan error jika hak akses tidak sesuai.
        return false;                                                                  // Mengembalikan false.
    }

    // ========== PEMERIKSAAN DATA PASIEN DI RUMAH SAKIT ==========
    // Memeriksa apakah ada data pasien yang terdaftar di rumah sakit.
    if (hospital->patients.nEff == 0)
    {
        printError("Tidak ada pasien terdaftar!"); // Pesan error jika tidak ada pasien.
        return false;                              // Mengembalikan false.
    }

    // ========== MENCARI DATA PASIEN YANG SEDANG LOGIN ==========
    // Mencari entri pasien yang sesuai berdasarkan `session->userId`.
    // Ini adalah optimasi dari pencarian berdasarkan username jika ID sesi sudah benar.
    Patient *patient = NULL; // Pointer ke struktur Patient, diinisialisasi NULL.
    // Melakukan iterasi melalui daftar pasien yang efektif (`hospital->patients.nEff`).
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        // Membandingkan ID pasien di daftar dengan ID pengguna dari sesi.
        if (hospital->patients.elements[i].id == session->userId)
        {
            patient = &hospital->patients.elements[i]; // Jika ditemukan, simpan pointer ke pasien tersebut.
            break;                                     // Keluar dari loop karena pasien sudah ditemukan.
        }
    }

    // Jika setelah iterasi, pasien tidak ditemukan (pointer `patient` masih NULL).
    if (!patient) // Cara singkat untuk memeriksa `patient == NULL`.
    {
        printError("Pasien tidak ditemukan!"); // Pesan error.
        return false;                          // Mengembalikan false.
    }

    // ========== MENENTUKAN STATUS DAN KETERANGAN BOLEH PULANG ==========
    // Variabel untuk menyimpan string status dan deskripsi.
    const char *statusStr, *descriptionStr;
    // Flag boolean untuk menandakan apakah pasien boleh pulang. Diinisialisasi false.
    boolean canGoHomeStatus = false;

    // Logika untuk menentukan status berdasarkan kondisi pasien.
    // Kondisi 1: Pasien belum didiagnosa.
    if (!patient->diagnosedStatus) // Jika status diagnosis pasien adalah false.
    {
        statusStr = "Belum Diagnosa";
        descriptionStr = "Anda belum didiagnosa oleh dokter.";
    }
    // Kondisi 2: Pasien sudah didiagnosa, tetapi belum diberi resep obat/perawatan.
    else if (!patient->treatedStatus) // Jika status perawatan/pemberian resep adalah false.
    {
        statusStr = "Belum Diberi Obat";
        descriptionStr = "Anda belum diberikan resep obat.";
    }
    // Kondisi 3: Pasien sudah didiagnosa dan sudah diberi resep. Sekarang periksa konsumsi obat.
    else
    {
        // Memeriksa jumlah obat yang diresepkan (`prescribedCount`)
        // dengan jumlah obat yang sudah diminum (`takenCount`).
        // `patient->medicationsPrescribed.nEff` adalah jumlah obat yang diresepkan.
        // `patient->medicationsTaken.top + 1` adalah jumlah obat yang diminum (jika `medicationsTaken` adalah stack).
        int prescribedCount = patient->medicationsPrescribed.nEff;
        int takenCount = patient->medicationsTaken.top + 1; // `top` -1 berarti kosong, jadi `top+1` adalah ukuran.

        // Jika jumlah obat yang diminum tidak sama dengan yang diresepkan.
        if (takenCount != prescribedCount)
        {
            statusStr = "Belum Minum Semua Obat";
            descriptionStr = "Anda belum mengonsumsi semua obat yang telah diresepkan.";
        }
        // Jika jumlah obat sudah sesuai, periksa urutan konsumsi obat.
        else
        {
            // Asumsikan urutan benar pada awalnya.
            boolean correctOrder = true;
            // Melakukan iterasi untuk membandingkan ID setiap obat yang diminum dengan yang diresepkan.
            // Loop berhenti jika semua obat sudah dicek atau jika ditemukan urutan yang salah (`!correctOrder`).
            // Ini adalah optimasi "early exit".
            for (int i = 0; i < takenCount && correctOrder; i++)
            {
                // Membandingkan ID obat ke-i yang diminum dengan ID obat ke-i yang diresepkan.
                if (patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i])
                {
                    correctOrder = false; // Jika ada yang tidak cocok, urutan salah.
                    // `break` tidak digunakan di sini, tapi `correctOrder` akan menghentikan loop.
                }
            }

            // Jika urutan konsumsi obat salah.
            if (!correctOrder)
            {
                statusStr = "Urutan Obat Salah";
                descriptionStr = "Anda mengonsumsi obat dengan urutan yang salah.";
            }
            // Jika semua kondisi terpenuhi (didiagnosa, diobati, semua obat diminum dengan urutan benar).
            else
            {
                statusStr = "Boleh Pulang";
                descriptionStr = "Anda telah selesai menjalani perawatan.";
                canGoHomeStatus = true; // Set status boleh pulang menjadi true.
            }
        }
    }

    // ========== MENAMPILKAN STATUS BOLEH PULANG ==========
    // Mencetak header untuk status pulang.
    printHeader("Status Pulang");
    // Mendefinisikan lebar kolom dan header untuk tabel output.
    int widths[] = {15, 40}; // Lebar untuk kolom "Status" dan "Keterangan".
    const char *headers[] = {"Status", "Keterangan"};
    // Data untuk baris tabel: string status dan deskripsi yang sudah ditentukan.
    const char *row[] = {statusStr, descriptionStr};

    // Mencetak tabel menggunakan fungsi utilitas.
    printTableBorder(widths, 2, 1);    // Border atas tabel.
    printTableRow(headers, widths, 2); // Baris header.
    // Baris di bawah header tidak menggunakan printTableBorder tipe 2, langsung mencetak baris data.
    // Ini mungkin disengaja untuk format tertentu atau oversight.
    printTableRow(row, widths, 2);  // Baris data status dan keterangan.
    printTableBorder(widths, 2, 3); // Border bawah tabel.

    // ========== PROSES JIKA PASIEN BOLEH PULANG ==========
    // Jika `canGoHomeStatus` adalah true.
    if (canGoHomeStatus)
    {
        // Tingkatkan aura dokter yang merawat pasien.
        // Mencari riwayat pengobatan pasien untuk menemukan ID dokter.
        // Loop ini akan berhenti pada entri riwayat pertama yang cocok dengan ID pasien.
        // Diasumsikan riwayat terakhir atau yang relevan akan ditemukan pertama, atau hanya ada satu.
        for (int i = 0; i < hospital->treatmentHistory.nEff; i++)
        {
            if (hospital->treatmentHistory.elements[i].patientId == patient->id)
            {
                int doctorId = hospital->treatmentHistory.elements[i].doctorId; // Dapatkan ID dokter dari riwayat.
                // Cari dokter di daftar dokter rumah sakit berdasarkan ID.
                for (int j = 0; j < hospital->doctors.nEff; j++)
                {
                    if (hospital->doctors.elements[j].id == doctorId)
                    {
                        hospital->doctors.elements[j].aura++; // Tingkatkan aura dokter.
                        break;                                // Keluar dari loop pencarian dokter (inner loop).
                    }
                }
                break; // Keluar dari loop pencarian riwayat (outer loop) setelah dokter ditemukan dan aura diupdate.
            }
        }

        // Hapus pasien dari antrian jika pasien masih terdaftar dalam antrian.
        // `patient->queueRoom[0] != '\0'` menandakan pasien masih terasosiasi dengan sebuah antrian/ruangan.
        if (patient->queueRoom[0] != '\0')
        {
            Queue *targetQueue = NULL; // Pointer ke antrian target.

            // Cari antrian yang sesuai dengan `patient->queueRoom`.
            // Loop melalui semua slot antrian yang dialokasikan (`hospital->queues.capacity`).
            for (int i = 0; i < hospital->queues.capacity; i++)
            {
                // Periksa apakah antrian aktif (`roomCode` tidak kosong) dan cocok.
                if (hospital->queues.queues[i].roomCode[0] != '\0' &&
                    strcmp(hospital->queues.queues[i].roomCode, patient->queueRoom) == 0)
                {
                    targetQueue = &hospital->queues.queues[i]; // Antrian ditemukan.
                    break;                                     // Keluar dari loop.
                }
            }

            // Jika antrian target ditemukan dan tidak kosong.
            if (targetQueue && !isQueueEmpty(targetQueue)) // `isQueueEmpty` adalah fungsi utilitas.
            {
                QueueNode *current = targetQueue->head; // Mulai dari kepala antrian.
                QueueNode *prev = NULL;                 // Pointer ke node sebelumnya.

                // Cari dan hapus node pasien dari linked list antrian.
                while (current) // Selama `current` tidak NULL.
                {
                    if (current->info.patientId == patient->id) // Jika ID pasien di node cocok.
                    {
                        // Logika penghapusan node dari doubly linked list.
                        if (!prev) // Jika node yang dihapus adalah kepala (head).
                        {
                            targetQueue->head = current->next; // Pindahkan head ke node berikutnya.
                            if (targetQueue->head)             // Jika antrian tidak menjadi kosong.
                            {
                                targetQueue->head->prev = NULL; // Set prev dari head baru ke NULL.
                            }
                            else // Jika antrian menjadi kosong.
                            {
                                targetQueue->tail = NULL; // Set tail juga ke NULL.
                            }
                        }
                        else // Jika node yang dihapus bukan head.
                        {
                            prev->next = current->next; // Sambungkan prev ke next dari current.
                            if (current->next)          // Jika current bukan node terakhir (tail).
                            {
                                current->next->prev = prev; // Set prev dari next node ke prev.
                            }
                            else // Jika current adalah node terakhir (tail).
                            {
                                targetQueue->tail = prev; // Set tail ke prev.
                            }
                        }

                        free(current);       // Bebaskan memori node yang dihapus.
                        targetQueue->size--; // Kurangi ukuran antrian.
                        break;               // Keluar dari loop while karena pasien sudah dihapus.
                    }
                    prev = current;          // Simpan current sebagai prev untuk iterasi berikutnya.
                    current = current->next; // Pindah ke node berikutnya.
                }
            }

            // Setelah dihapus dari antrian (atau jika tidak ditemukan di antrian aktif),
            // bersihkan informasi antrian pada data pasien.
            patient->queueRoom[0] = '\0'; // Kosongkan kode ruangan antrian.
            patient->queuePosition = 0;   // Reset posisi antrian.
        }

        // Pesan sukses bahwa pasien boleh pulang.
        printSuccess("Selamat, Anda boleh pulang!");
        return true; // Fungsi berhasil, pasien boleh pulang.
    }
    else // Jika `canGoHomeStatus` adalah false.
    {
        // Pesan bahwa pasien belum boleh pulang.
        printError("Anda belum boleh pulang!");
        return false; // Fungsi mengembalikan false.
    }
}
