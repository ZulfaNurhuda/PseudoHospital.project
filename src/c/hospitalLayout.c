// Menyertakan file header "hospitalLayout.h".
// File ini diasumsikan berisi deklarasi fungsi-fungsi dalam file ini (displayLayout, displayRoomDetails),
// definisi struktur Hospital (menyimpan data rumah sakit), Session (data sesi pengguna),
// Room (detail ruangan), Layout (struktur denah), dan enum Role (peran pengguna),
// serta makro seperti COLOR_YELLOW, COLOR_RESET (untuk pewarnaan teks di konsol),
// dan deklarasi fungsi utilitas seperti printError (mencetak pesan kesalahan),
// printHeader (mencetak judul), isValidRoomCode (memvalidasi format kode ruangan),
// printTableBorder (mencetak batas tabel), dan printTableRow (mencetak baris tabel).
#include "hospitalLayout.h"
#include <stdio.h>  // Diperlukan untuk fungsi input/output standar seperti printf.
#include <string.h> // Diperlukan untuk fungsi manipulasi string seperti strcmp (membandingkan string), strcpy (menyalin string), dan strcat (menggabungkan string).

// Fungsi untuk menampilkan denah rumah sakit secara visual di konsol.
// Menerima pointer ke struktur Hospital (data rumah sakit),
// pointer ke struktur Session (informasi sesi pengguna saat ini),
// dan flag boolean printHeaderFlag untuk menentukan apakah header "Denah Rumah Sakit" perlu dicetak.
void displayLayout(Hospital *hospital, Session *session, boolean printHeaderFlag)
{
    // Pemeriksaan Awal: Memastikan pointer hospital dan session tidak NULL.
    // Ini penting untuk mencegah dereferensi NULL pointer yang bisa menyebabkan crash program.
    if (hospital == NULL || session == NULL)
    {
        // Jika salah satu atau kedua pointer adalah NULL, cetak pesan error.
        printError("Struktur rumah sakit atau sesi tidak valid!"); // Menampilkan pesan kesalahan jika ada pointer yang NULL.
        return;                                                    // Menghentikan eksekusi fungsi jika data tidak valid karena operasi selanjutnya bergantung pada struktur ini.
    }

    // Pemeriksaan Hak Akses: Memastikan pengguna sudah login dan memiliki peran yang diizinkan.
    // Hanya Manajer, Dokter, atau Pasien yang dapat melihat denah. Peran lain tidak diizinkan.
    // `session->isLoggedIn` memeriksa status login.
    // `session->role` memeriksa peran pengguna. MANAGER, DOCTOR, PATIENT adalah konstanta enum/define.
    if (!session->isLoggedIn || (session->role != MANAGER && session->role != DOCTOR && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Manajer, Dokter, atau Pasien yang dapat melihat denah."); // Pesan jika akses tidak diizinkan.
        return;                                                                                    // Menghentikan eksekusi fungsi karena pengguna tidak memiliki hak akses.
    }

    // Pemeriksaan Denah Kosong: Memastikan denah memiliki dimensi yang valid (baris dan kolom efektif > 0).
    // `hospital->layout.rowEff` adalah jumlah baris efektif denah.
    // `hospital->layout.colEff` adalah jumlah kolom efektif denah.
    // Jika rowEff atau colEff adalah 0 atau negatif, denah tidak dapat ditampilkan.
    if (hospital->layout.rowEff <= 0 || hospital->layout.colEff <= 0)
    {
        printError("Denah rumah sakit kosong!"); // Pesan jika denah tidak memiliki isi atau dimensi tidak valid.
        return;                                  // Menghentikan eksekusi fungsi.
    }

    // Mencetak Header: Jika printHeaderFlag bernilai true, cetak judul "Denah Rumah Sakit".
    // Ini berguna jika fungsi dipanggil dari konteks berbeda yang mungkin sudah memiliki header sendiri,
    // sehingga pencetakan header bisa bersifat opsional.
    if (printHeaderFlag)
        printHeader("Denah Rumah Sakit");

    // Mencetak Garis Batas Atas Tabel Denah.
    // Karakter '┌' (Unicode U+250C) adalah sudut kiri atas untuk box drawing.
    printf("┌");
    // Loop untuk setiap kolom dalam denah untuk mencetak bagian atas dari setiap sel.
    // `hospital->layout.colEff` adalah jumlah total kolom.
    for (int j = 0; j < hospital->layout.colEff; j++)
    {
        // Mencetak segmen garis horizontal "─────" (5 tanda hubung)
        // dan pemisah kolom atas "┬" (Unicode U+252C).
        printf("─────┬");
    }
    // Karakter '\b' (backspace) digunakan untuk menghapus pemisah kolom terakhir "┬" yang berlebih,
    // karena pemisah hanya ada di antara kolom, bukan di ujung kanan.
    // Karakter '┐' (Unicode U+2510) adalah sudut kanan atas. '\n' untuk pindah ke baris baru.
    printf("\b┐\n");

    // Melakukan iterasi untuk setiap baris dalam denah (dari atas ke bawah, i adalah indeks baris).
    // `hospital->layout.rowEff` adalah jumlah total baris.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        // Mencetak Baris yang Berisi Kode Ruangan.
        // Karakter '│' (Unicode U+2502) adalah batas vertikal kiri untuk setiap baris.
        printf("│");
        // Loop untuk setiap kolom dalam baris saat ini (j adalah indeks kolom).
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            // Mencetak kode ruangan (string) yang tersimpan di `hospital->layout.elements[i][j].code`.
            // `hospital->layout.elements` adalah array 2D yang menyimpan data setiap sel/ruangan.
            // COLOR_YELLOW dan COLOR_RESET adalah makro untuk pewarnaan teks (diasumsikan menggunakan ANSI escape codes).
            // " %-3s " : format specifier untuk string.
            //   ' ' (spasi) di awal dan akhir untuk padding di dalam sel agar kode tidak menempel ke border.
            //   '-' untuk rata kiri (left-justify).
            //   '3' untuk lebar minimal 3 karakter. Jika kode lebih pendek, akan diisi spasi. Jika lebih panjang, akan tetap dicetak penuh.
            //   's' untuk tipe data string.
            // Karakter '│' dicetak setelah setiap kode ruangan sebagai batas vertikal kanan sel.
            printf(COLOR_YELLOW " %-3s " COLOR_RESET "│", hospital->layout.elements[i][j].code);
        }
        printf("\n"); // Pindah ke baris baru setelah semua kode ruangan dalam satu baris tercetak.

        // Mencetak Garis Pemisah Horizontal Antar Baris.
        // Garis ini tidak dicetak setelah baris terakhir dari denah untuk membentuk batas bawah yang benar.
        if (i != hospital->layout.rowEff - 1) // Cek apakah ini bukan baris terakhir.
        {
            // Karakter '├' (Unicode U+251C) adalah pemisah T kiri (junction), menandakan persimpangan garis.
            printf("├");
            // Loop untuk setiap kolom untuk mencetak garis pemisah horizontal.
            for (int j = 0; j < hospital->layout.colEff; j++)
            {
                // Mencetak segmen garis horizontal "─────" dan pemisah persimpangan "┼" (Unicode U+253C).
                printf("─────┼");
            }
            // Karakter '\b' (backspace) untuk menghapus pemisah persimpangan terakhir "┼" yang berlebih.
            // Karakter '┤' (Unicode U+2524) adalah pemisah T kanan. '\n' untuk pindah ke baris baru.
            printf("\b┤\n");
        }
    }

    // Mencetak Garis Batas Bawah Tabel Denah.
    // Karakter '└' (Unicode U+2514) adalah sudut kiri bawah.
    printf("└");
    // Loop untuk setiap kolom untuk mencetak bagian bawah dari setiap sel.
    for (int j = 0; j < hospital->layout.colEff; j++)
    {
        // Mencetak segmen garis horizontal "─────" dan pemisah bawah "┴" (Unicode U+2534).
        printf("─────┴");
    }
    // Karakter '\b' (backspace) untuk menghapus pemisah bawah terakhir "┴" yang berlebih.
    // Karakter '┘' (Unicode U+2518) adalah sudut kanan bawah. '\n' untuk pindah ke baris baru.
    printf("\b┘\n");
}

// Fungsi untuk menampilkan detail sebuah ruangan tertentu, termasuk kapasitas, dokter yang bertugas, dan pasien yang ada di dalamnya.
// Menerima pointer ke struktur Hospital, pointer ke struktur Session, dan string kode ruangan yang ingin dilihat detailnya.
void displayRoomDetails(Hospital *hospital, Session *session, const char *roomCode)
{
    // Pemeriksaan Awal: Validitas pointer hospital dan session.
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return; // Menghentikan fungsi jika data dasar tidak ada.
    }
    // Pemeriksaan Hak Akses: Pengguna harus login dan memiliki peran yang sesuai (Manajer, Dokter, atau Pasien).
    if (!session->isLoggedIn || (session->role != MANAGER && session->role != DOCTOR && session->role != PATIENT))
    {
        printError("Akses ditolak! Hanya Manajer, Dokter, atau Pasien yang dapat melihat detail ruangan.");
        return; // Menghentikan fungsi jika tidak ada hak akses.
    }
    // Pemeriksaan Kode Ruangan: Kode ruangan tidak boleh NULL atau string kosong.
    if (roomCode == NULL || roomCode[0] == '\0')
    {
        printError("Kode ruangan tidak valid!"); // Pesan jika kode ruangan tidak diberikan.
        return;                                  // Menghentikan fungsi.
    }
    // Validasi Format Kode Ruangan: Menggunakan fungsi isValidRoomCode (diasumsikan ada dan terdefinisi di tempat lain).
    // Fungsi ini mungkin memeriksa apakah format kode sesuai (misalnya, satu huruf diikuti satu angka) dan apakah kode tersebut ada di denah.
    if (!isValidRoomCode(hospital, roomCode))
    {
        printError("Kode ruangan tidak valid! Contoh: A1"); // Memberikan contoh format yang benar.
        return;                                             // Menghentikan fungsi jika format kode tidak valid atau kode tidak ditemukan.
    }
    // Pemeriksaan Denah Kosong: Denah harus memiliki dimensi yang valid untuk bisa mencari ruangan.
    if (hospital->layout.rowEff <= 0 || hospital->layout.colEff <= 0)
    {
        printError("Denah rumah sakit kosong!");
        return; // Menghentikan fungsi jika denah kosong.
    }

    // Mencari Ruangan: Melakukan iterasi pada matriks denah untuk menemukan ruangan dengan kode yang sesuai.
    Room *room = NULL; // Pointer ke struktur Room. Diinisialisasi NULL, akan diisi jika ruangan ditemukan.
    // Loop melalui setiap baris denah.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        // Loop melalui setiap kolom dalam baris saat ini.
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            // Membandingkan kode ruangan pada elemen denah (hospital->layout.elements[i][j].code)
            // dengan roomCode yang dicari menggunakan strcmp. strcmp mengembalikan 0 jika string sama.
            if (strcmp(hospital->layout.elements[i][j].code, roomCode) == 0)
            {
                // Jika kode cocok, simpan alamat (pointer) dari elemen denah (struktur Room) tersebut ke variabel 'room'.
                room = &hospital->layout.elements[i][j];
                break; // Keluar dari loop kolom (inner loop) karena ruangan sudah ditemukan.
            }
        }
        if (room != NULL) // Jika 'room' tidak lagi NULL (artinya ruangan sudah ditemukan di loop kolom).
            break;        // Keluar dari loop baris (outer loop) untuk efisiensi.
    }

    // Penanganan Ruangan Tidak Ditemukan: Jika setelah iterasi 'room' masih bernilai NULL.
    // Ini bisa terjadi jika isValidRoomCode hanya memeriksa format, bukan keberadaan, atau jika ada bug.
    if (room == NULL)
    {
        char errorMessage[100] = "Ruangan "; // Buffer untuk membangun pesan kesalahan.
        strcat(errorMessage, roomCode);      // Menambahkan kode ruangan yang dicari ke pesan.
        strcat(errorMessage, " tidak ditemukan!");
        printError(errorMessage); // Mencetak pesan kesalahan yang informatif.
        return;                   // Menghentikan eksekusi fungsi.
    }

    // Mencetak Header untuk Detail Ruangan.
    char header[100] = "Detail Ruangan "; // Buffer untuk judul header.
    strcat(header, roomCode);             // Menambahkan kode ruangan ke judul.
    printHeader(header);                  // Memanggil fungsi untuk mencetak header yang diformat.

    // Menampilkan Informasi Kapasitas Ruangan dalam Format Tabel.
    char capacityStr[20];          // Buffer untuk menyimpan representasi string dari nilai kapasitas.
    int capacity = room->capacity; // Mengambil nilai kapasitas integer dari struktur Room yang ditemukan.
    // Mengkonversi nilai integer kapasitas menjadi string secara manual.
    // Ini adalah alternatif dari penggunaan fungsi seperti sprintf untuk kasus sederhana dan terbatas.
    if (capacity < 10) // Jika kapasitas adalah satu digit (0-9).
    {
        capacityStr[0] = '0' + capacity; // Konversi digit ke karakter ASCII-nya (misal, 5 (int) -> '5' (char)).
        capacityStr[1] = '\0';           // Menambahkan null terminator untuk menandai akhir string.
    }
    else if (capacity < 100) // Jika kapasitas adalah dua digit (10-99).
    {
        capacityStr[0] = '0' + (capacity / 10); // Mendapatkan digit puluhan dan mengkonversinya ke char.
        capacityStr[1] = '0' + (capacity % 10); // Mendapatkan digit satuan dan mengkonversinya ke char.
        capacityStr[2] = '\0';                  // Null terminator.
    }
    else // Jika kapasitas adalah tiga digit atau lebih (contoh ini spesifik untuk "100").
    {
        // Untuk kapasitas >= 100, kode ini secara spesifik menangani kasus "100".
        // Jika kapasitas bisa lebih dari 100 dan perlu ditampilkan dengan benar, logika ini perlu diperluas
        // atau lebih baik menggunakan fungsi standar seperti sprintf(capacityStr, "%d", capacity);
        capacityStr[0] = '1';
        capacityStr[1] = '0';
        capacityStr[2] = '0';
        capacityStr[3] = '\0'; // Null terminator untuk string "100".
    }
    // Data untuk baris pertama tabel: label "Kapasitas" dan nilai kapasitas dalam bentuk string.
    const char *row1[] = {"Kapasitas", capacityStr};
    int widths1[] = {15, 20};        // Mendefinisikan lebar kolom untuk tabel ini: 15 untuk label, 20 untuk nilai.
    printTableBorder(widths1, 2, 1); // Mencetak border atas tabel (2 kolom, tipe border 1 - mungkin garis atas solid).
    printTableRow(row1, widths1, 2); // Mencetak baris data kapasitas ke dalam tabel.
    printTableBorder(widths1, 2, 2); // Mencetak garis pemisah di bawah baris kapasitas (tipe border 2 - mungkin garis tengah).

    // Menampilkan Informasi Dokter yang Bertugas di Ruangan.
    char doctorStr[50]; // Buffer untuk menyimpan nama dokter atau pesan status "Tidak ada dokter".
    // Memeriksa apakah ada ID dokter yang valid (room->doctorId != -1, dimana -1 biasanya menandakan tidak ada) di ruangan
    // atau jika daftar dokter di rumah sakit (hospital->doctors.nEff) kosong.
    if (room->doctorId == -1 || hospital->doctors.nEff <= 0)
    {
        strcpy(doctorStr, "Tidak ada dokter"); // Jika tidak ada dokter yang ditugaskan atau tidak ada dokter terdaftar, set pesan ini.
    }
    else
    {
        boolean found = false; // Flag untuk menandakan apakah dokter dengan ID tersebut ditemukan di daftar dokter rumah sakit.
        // Mencari dokter di daftar dokter rumah sakit (hospital->doctors.elements)
        // berdasarkan doctorId yang tersimpan di struktur Room.
        for (int i = 0; i < hospital->doctors.nEff; i++)
        {
            // Membandingkan ID dokter di daftar dengan ID dokter yang ditugaskan ke ruangan.
            if (hospital->doctors.elements[i].id == room->doctorId)
            {
                strcpy(doctorStr, hospital->doctors.elements[i].username); // Salin username dokter yang ditemukan.
                found = true;                                              // Set flag bahwa dokter telah ditemukan.
                break;                                                     // Keluar dari loop karena dokter sudah ditemukan.
            }
        }
        if (!found) // Jika setelah loop dokter dengan ID tersebut tidak ditemukan di daftar dokter (kemungkinan inkonsistensi data).
        {
            strcpy(doctorStr, "Dokter tidak ditemukan"); // Set pesan ini, menandakan ada ID tapi tidak ada entri dokter yang cocok.
        }
    }
    // Data untuk baris kedua tabel: label "Dokter" dan nama dokter (atau pesan status).
    const char *row2[] = {"Dokter", doctorStr};
    int widths2[] = {15, 20};        // Lebar kolom sama seperti tabel kapasitas.
    printTableRow(row2, widths2, 2); // Mencetak baris data dokter.
    printTableBorder(widths2, 2, 3); // Mencetak border bawah tabel untuk bagian dokter (tipe border 3 - mungkin garis bawah solid).

    // Menampilkan Daftar Pasien yang Ada di Dalam Ruangan.
    const char *row3[] = {"No.", "Pasien dalam Ruangan"}; // Header untuk tabel daftar pasien.
    int widths3[] = {5, 30};                              // Lebar kolom: 5 untuk nomor urut, 30 untuk nama pasien.
    printTableBorder(widths3, 2, 1);                      // Mencetak border atas tabel pasien.
    printTableRow(row3, widths3, 2);                      // Mencetak baris header tabel pasien.
    printTableBorder(widths3, 2, 2);                      // Mencetak garis pemisah di bawah header.
    // Memeriksa apakah ada pasien yang tercatat di ruangan (room->patientInRoom.nEff > 0)
    // dan apakah ada pasien terdaftar di data rumah sakit secara keseluruhan (hospital->patients.nEff > 0).
    // Kondisi hospital->patients.nEff <= 0 juga dicek untuk memastikan ada data pasien global untuk dicocokkan.
    if (room->patientInRoom.nEff == 0 || hospital->patients.nEff <= 0)
    {
        // Jika tidak ada pasien di ruangan atau tidak ada data pasien global, tampilkan pesan ini.
        // Nomor urut "0." digunakan sebagai placeholder, mungkin lebih baik "-" atau dikosongkan.
        const char *row4[] = {"0.", "Tidak ada pasien"};
        printTableRow(row4, widths3, 2);
    }
    else
    {
        // Melakukan iterasi melalui daftar ID pasien yang tercatat berada di ruangan ini (room->patientInRoom.patientId).
        // `room->patientInRoom.nEff` adalah jumlah pasien di ruangan tersebut.
        for (int i = 0; i < room->patientInRoom.nEff; i++)
        {
            boolean found = false; // Flag untuk menandakan apakah detail pasien dengan ID tersebut ditemukan di daftar pasien rumah sakit.
            // Mencari detail pasien di daftar pasien rumah sakit (hospital->patients.elements)
            // berdasarkan ID pasien dari ruangan (room->patientInRoom.patientId[i]).
            for (int j = 0; j < hospital->patients.nEff; j++)
            {
                // Membandingkan ID pasien di daftar global dengan ID pasien yang ada di ruangan.
                if (hospital->patients.elements[j].id == room->patientInRoom.patientId[i])
                {
                    char idStr[20]; // Buffer untuk menyimpan nomor urut pasien sebagai string.
                    // Logika pembuatan string nomor urut: (i + 1) untuk nomor mulai dari 1.
                    // `i + 1 + '0'` akan menghasilkan karakter ASCII berdasarkan nilai numerik, BUKAN string angka.
                    // Contoh: jika i+1 adalah 1, maka 1 + '0' (ASCII 48) = 49 (ASCII untuk '1'). Ini benar untuk 1 digit.
                    // Namun, untuk 2 digit atau lebih, ini akan salah.
                    // Misal, jika i+1 adalah 10, 10 + '0' adalah 58 (ASCII untuk ':').
                    // Kode ini akan menghasilkan string yang salah untuk nomor > 9.
                    // Seharusnya menggunakan sprintf(idStr, "%d", i + 1); atau logika konversi multi-digit yang benar.
                    // Karena diminta TIDAK MENGUBAH, komentar ini hanya menjelaskan perilaku kode ASLI.
                    strcpy(idStr, i + 1 + "0"); // Ini akan menghasilkan karakter, bukan string angka untuk i+1 >= 10
                    strcat(idStr, ".");         // Menambahkan titik setelah nomor.

                    // Data untuk baris pasien: nomor urut (yang mungkin salah formatnya) dan username pasien.
                    const char *row4[] = {idStr, hospital->patients.elements[j].username};
                    printTableRow(row4, widths3, 2); // Mencetak baris data pasien.

                    found = true; // Set flag bahwa detail pasien telah ditemukan dan dicetak.
                    break;        // Keluar dari loop pencarian pasien karena detail sudah ditemukan.
                }
            }
            if (!found) // Jika setelah loop, detail pasien dengan ID tersebut tidak ditemukan di daftar pasien rumah sakit (kemungkinan inkonsistensi data).
            {
                // Menampilkan pesan bahwa data pasien spesifik tidak ditemukan, beserta nomor urut (yang juga mungkin salah formatnya).
                // "0." digunakan lagi sebagai placeholder jika pasien tidak ditemukan.
                const char *row4[] = {"0.", "Tidak ada pasien"}; // Seharusnya menampilkan nomor urut i+1 dan pesan "Data tidak ditemukan".
                printTableRow(row4, widths3, 2);
            }
        }
    }
    printTableBorder(widths3, 2, 3); // Mencetak border bawah tabel daftar pasien.
}