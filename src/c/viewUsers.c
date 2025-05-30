#include "viewUsers.h"

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan ID secara menaik (Ascending).
// Fungsi ini akan dipanggil berulang kali oleh qsort untuk menentukan urutan elemen.
// Parameter a dan b adalah pointer ke elemen yang akan dibandingkan.
// Mengembalikan:
//   - nilai negatif jika elemen pertama (a) lebih kecil dari elemen kedua (b).
//   - nol jika kedua elemen sama.
//   - nilai positif jika elemen pertama (a) lebih besar dari elemen kedua (b).
int compareUserByIdAsc(const void *a, const void *b)
{
    // Meng-cast pointer void ke pointer const User agar bisa mengakses field 'id'.
    // Penggunaan 'const' menandakan bahwa fungsi ini tidak akan mengubah data yang ditunjuk.
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // Mengembalikan selisih ID. Jika ua->id < ub->id, hasilnya negatif (sesuai untuk ascending).
    return ua->id - ub->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan ID secara menurun (Descending).
// Logikanya mirip dengan ascending, tetapi urutan pengembalian dibalik.
int compareUserByIdDesc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // Mengembalikan selisih ID terbalik. Jika ub->id < ua->id, hasilnya negatif (artinya ua->id > ub->id, sesuai untuk descending).
    return ub->id - ua->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan username secara menaik (Ascending Alfabetis).
// Menggunakan fungsi standar strcmp untuk perbandingan string.
int compareUserByNameAsc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // strcmp mengembalikan <0 jika ua->username < ub->username, 0 jika sama, >0 jika ua->username > ub->username.
    return strcmp(ua->username, ub->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan username secara menurun (Descending Alfabetis).
int compareUserByNameDesc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // Membalik urutan argumen strcmp untuk mendapatkan urutan menurun.
    return strcmp(ub->username, ua->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Patient berdasarkan ID secara menaik (Ascending).
// Struktur dan logika sama dengan compareUserByIdAsc, tetapi untuk tipe Patient.
int comparePatientByIdAsc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return pa->id - pb->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Patient berdasarkan ID secara menurun (Descending).
int comparePatientByIdDesc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return pb->id - pa->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Patient berdasarkan username secara menaik (Ascending Alfabetis).
int comparePatientByNameAsc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return strcmp(pa->username, pb->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Patient berdasarkan username secara menurun (Descending Alfabetis).
int comparePatientByNameDesc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return strcmp(pb->username, pa->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Doctor berdasarkan ID secara menaik (Ascending).
int compareDoctorByIdAsc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return da->id - db->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Doctor berdasarkan ID secara menurun (Descending).
int compareDoctorByIdDesc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return db->id - da->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Doctor berdasarkan username secara menaik (Ascending Alfabetis).
int compareDoctorByNameAsc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return strcmp(da->username, db->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Doctor berdasarkan username secara menurun (Descending Alfabetis).
int compareDoctorByNameDesc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return strcmp(db->username, da->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Doctor berdasarkan aura secara menaik (Ascending).
// Aura diasumsikan sebagai nilai integer.
int compareDoctorByAuraAsc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return da->aura - db->aura; // Mengembalikan selisih aura.
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Doctor berdasarkan aura secara menurun (Descending).
int compareDoctorByAuraDesc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return db->aura - da->aura; // Mengembalikan selisih aura terbalik.
}

// Fungsi untuk mengurutkan daftar pengguna (UserSet).
// Menerima pointer ke UserSet yang akan diurutkan,
// kriteria pengurutan (sortBy: 1 untuk ID, lainnya (misal 2) untuk Nama),
// dan urutan pengurutan (sortOrder: 1 untuk Ascending, lainnya (misal 2) untuk Descending).
void sortUser(UserSet *userList, int sortBy, int sortOrder)
{
    // Memeriksa kriteria pengurutan.
    if (sortBy == 1) // Jika diurutkan berdasarkan ID.
    {
        // Memeriksa urutan pengurutan.
        if (sortOrder == 1) // Jika Ascending.
        {
            // Menggunakan fungsi standar qsort untuk mengurutkan array.
            // Parameter:
            //   1. userList->elements: pointer ke elemen pertama array yang akan diurutkan.
            //   2. userList->nEff: jumlah elemen dalam array.
            //   3. sizeof(User): ukuran (dalam byte) dari satu elemen array.
            //   4. compareUserByIdAsc: pointer ke fungsi pembanding yang akan digunakan.
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByIdAsc);
        }
        else // Jika Descending (sortOrder bukan 1).
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByIdDesc);
        }
    }
    else // Jika diurutkan berdasarkan Nama (sortBy bukan 1).
    {
        if (sortOrder == 1) // Jika Ascending.
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByNameAsc);
        }
        else // Jika Descending.
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByNameDesc);
        }
    }
}

// Fungsi untuk mengurutkan daftar pasien (PatientList).
// Logikanya identik dengan sortUser, tetapi beroperasi pada struktur Patient dan menggunakan fungsi pembanding Patient.
void sortPatients(PatientList *patientList, int sortBy, int sortOrder)
{
    if (sortBy == 1) // Sort by ID
    {
        if (sortOrder == 1) // ASC
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByIdAsc);
        }
        else // DESC
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByIdDesc);
        }
    }
    else // Sort by Name
    {
        if (sortOrder == 1) // ASC
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByNameAsc);
        }
        else // DESC
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByNameDesc);
        }
    }
}

// Fungsi untuk mengurutkan daftar dokter (DoctorList).
// Menambahkan kriteria pengurutan ketiga yaitu berdasarkan Aura.
// sortBy: 1 untuk ID, 2 untuk Nama, lainnya (misal 3) untuk Aura.
void sortDoctors(DoctorList *doctorList, int sortBy, int sortOrder)
{
    if (sortBy == 1) // Jika diurutkan berdasarkan ID.
    {
        if (sortOrder == 1) // ASC
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByIdAsc);
        }
        else // DESC
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByIdDesc);
        }
    }
    else if (sortBy == 2) // Jika diurutkan berdasarkan Nama.
    {
        if (sortOrder == 1) // ASC
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByNameAsc);
        }
        else // DESC
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByNameDesc);
        }
    }
    else // Jika diurutkan berdasarkan kriteria lain (diasumsikan Aura, sortBy bukan 1 atau 2).
    {
        if (sortOrder == 1) // ASC
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByAuraAsc);
        }
        else // DESC
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByAuraDesc);
        }
    }
}

void displayUsers(Hospital *hospital, Session *session, int sortBy, int sortOrder)
{
    // Pemeriksaan Awal: Validasi pointer hospital dan session.
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return; // Keluar jika data dasar tidak valid.
    }
    // Pemeriksaan Hak Akses: Hanya Manajer yang dapat melihat daftar semua pengguna.
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar pengguna.");
        return; // Keluar jika tidak ada hak akses.
    }

    // Mencetak header untuk daftar pengguna.
    printHeader("Daftar Pengguna");
    // Jika tidak ada pengguna terdaftar dalam sistem.
    if (hospital->users.nEff == 0)
    {
        // Mencetak pesan bahwa tidak ada pengguna dan keluar dari fungsi.
        // COLOR_YELLOW dan COLOR_RESET diasumsikan makro untuk pewarnaan teks.
        printf(COLOR_YELLOW "Tidak ada pengguna terdaftar.\n" COLOR_RESET);
        return;
    }

    // Membuat daftar sementara (tempList) untuk pengurutan.
    // Ini dilakukan agar daftar pengguna asli (hospital->users) tidak diubah urutannya.
    UserSet tempList;
    tempList.capacity = hospital->users.capacity; // Salin kapasitas dari daftar asli.
    tempList.nEff = hospital->users.nEff;         // Salin jumlah elemen efektif dari daftar asli.
    // Alokasi memori dinamis untuk array elemen dalam tempList.
    tempList.elements = (User *)malloc(tempList.capacity * sizeof(User));
    // Pemeriksaan kegagalan alokasi memori.
    if (tempList.elements == NULL)
    {
        printError("Gagal alokasi memori untuk daftar pengguna sementara!");
        return; // Keluar jika alokasi gagal.
    }

    // Menyalin semua pengguna dari daftar asli (hospital->users.elements) ke tempList.elements.
    // Ini adalah penyalinan dangkal jika User mengandung pointer, namun di sini User adalah struktur biasa.
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        tempList.elements[i] = hospital->users.elements[i]; // Menyalin seluruh isi struktur User.
    }

    // Mengurutkan daftar sementara (tempList) menggunakan fungsi sortUser yang telah dibuat.
    sortUser(&tempList, sortBy, sortOrder);

    // Mencetak informasi mengenai kriteria pengurutan yang sedang diterapkan.
    printf("Menampilkan semua pengguna dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",                  // Jika sortBy adalah 1, tampilkan "ID", jika tidak (misal 2), tampilkan "nama".
           (sortOrder == 1) ? "ascending" : "descending"); // Jika sortOrder adalah 1, tampilkan "ascending", jika tidak, tampilkan "descending".

    // Mendefinisikan lebar kolom dan nama header untuk tabel output.
    int widths[] = {5, 20, 10, 20};                                 // Lebar untuk kolom: ID, Username, Role, Penyakit.
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"}; // Nama header untuk setiap kolom.
    // Mencetak border atas dan baris header tabel menggunakan fungsi utilitas.
    printTableBorder(widths, 4, 1);    // 4 kolom, tipe border 1 (mungkin garis atas).
    printTableRow(headers, widths, 4); // Mencetak baris header.
    printTableBorder(widths, 4, 2);    // Mencetak garis pemisah di bawah header (tipe border 2).

    // Melakukan iterasi melalui daftar pengguna yang sudah diurutkan dalam tempList.
    for (int i = 0; i < tempList.nEff; i++)
    {
        // Mendapatkan pointer ke pengguna saat ini dalam iterasi.
        User *user = &tempList.elements[i];
        // Buffer untuk menyimpan representasi string dari ID, Role, dan Penyakit.
        // diseaseStr diinisialisasi dengan "-" sebagai nilai default jika pengguna bukan pasien atau tidak ada info penyakit.
        char idStr[10], roleStr[20], diseaseStr[20] = "-";

        // Mengkonversi ID pengguna (integer) menjadi string.
        // `integerToString` adalah fungsi kustom yang diasumsikan ada untuk konversi ini.
        // `sizeof(idStr)` memberikan ukuran buffer untuk mencegah overflow.
        if (!integerToString(user->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements); // Penting untuk membebaskan memori yang dialokasikan sebelum keluar jika terjadi error.
            return;
        }

        // Menentukan string untuk peran (Role) berdasarkan nilai enum/define dari `user->role`.
        switch (user->role)
        {
        case MANAGER:
            strcpy(roleStr, "Manajer");
            break;
        case DOCTOR:
            strcpy(roleStr, "Dokter");
            break;
        case PATIENT:
            strcpy(roleStr, "Pasien");
            break;
        default: // Jika nilai peran tidak dikenali.
            strcpy(roleStr, "Tidak diketahui");
        }

        // Mengambil informasi penyakit jika pengguna adalah seorang Pasien.
        // Memeriksa apakah peran pengguna adalah PATIENT dan apakah ada data pasien di rumah sakit (`hospital->patients.nEff > 0`).
        if (user->role == PATIENT && hospital->patients.nEff > 0)
        {
            // Mencari data pasien yang sesuai dalam `hospital->patients.elements` berdasarkan ID pengguna.
            for (int j = 0; j < hospital->patients.nEff; j++)
            {
                if (hospital->patients.elements[j].id == user->id) // Jika ID cocok.
                {
                    // Jika pasien ditemukan dan memiliki data penyakit yang valid (string tidak kosong).
                    // `strlen > 0` dan `[0] != '\0'` adalah pemeriksaan untuk memastikan string penyakit tidak kosong.
                    if (strlen(hospital->patients.elements[j].disease) > 0 && hospital->patients.elements[j].disease[0] != '\0')
                    {
                        strcpy(diseaseStr, hospital->patients.elements[j].disease); // Salin nama penyakit.
                    }
                    break; // Keluar dari loop pencarian pasien karena data sudah ditemukan.
                }
            }
        }

        // Data untuk baris tabel saat ini: ID, username, peran, dan penyakit (jika ada).
        const char *row[] = {idStr, user->username, roleStr, diseaseStr};
        // Mencetak baris tabel menggunakan fungsi utilitas.
        printTableRow(row, widths, 4); // 4 kolom.
    }
    // Mencetak border bawah tabel.
    printTableBorder(widths, 4, 3); // Tipe border 3 (mungkin garis bawah).

    // Membebaskan memori yang dialokasikan untuk `tempList.elements` setelah selesai digunakan.
    // Ini penting untuk mencegah kebocoran memori (memory leak).
    free(tempList.elements);
}

void displayPatients(Hospital *hospital, Session *session, int sortBy, int sortOrder)
{
    // Validasi input dan hak akses (hanya Manajer).
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar pasien.");
        return;
    }

    // Header dan penanganan jika tidak ada pasien.
    printHeader("Daftar Pasien");
    if (hospital->patients.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pasien terdaftar.\n" COLOR_RESET);
        return;
    }

    // Membuat daftar sementara untuk pasien.
    PatientList tempList;
    tempList.capacity = hospital->patients.capacity;
    tempList.nEff = hospital->patients.nEff;
    tempList.elements = (Patient *)malloc(tempList.capacity * sizeof(Patient));
    if (tempList.elements == NULL) // Pemeriksaan kegagalan malloc.
    {
        printError("Gagal alokasi memori untuk daftar pasien sementara!");
        return;
    }

    // Menyalin semua pasien ke daftar sementara.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        tempList.elements[i] = hospital->patients.elements[i];
    }

    // Mengurutkan daftar pasien sementara.
    sortPatients(&tempList, sortBy, sortOrder);

    // Informasi pengurutan.
    printf("Menampilkan pasien dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",
           (sortOrder == 1) ? "ascending" : "descending");

    // Mendefinisikan lebar kolom dan header untuk tabel pasien.
    int widths[] = {5, 20, 20}; // Kolom: ID, Username, Penyakit.
    const char *headers[] = {"ID", "Username", "Penyakit"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    // Melakukan iterasi dan menampilkan setiap pasien dalam daftar yang sudah diurutkan.
    for (int i = 0; i < tempList.nEff; i++)
    {
        Patient *patient = &tempList.elements[i]; // Pointer ke pasien saat ini.
        char idStr[10], diseaseStr[20] = "-";     // Buffer untuk ID dan Penyakit (default "-").

        // Konversi ID pasien ke string.
        if (!integerToString(patient->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements); // Bebaskan memori sebelum keluar.
            return;
        }

        // Mengambil data penyakit jika ada.
        if (strlen(patient->disease) > 0 && patient->disease[0] != '\0')
        {
            strcpy(diseaseStr, patient->disease);
        }

        // Data untuk baris tabel pasien.
        const char *row[] = {idStr, patient->username, diseaseStr};
        printTableRow(row, widths, 3); // Mencetak baris.
    }
    printTableBorder(widths, 3, 3); // Border bawah tabel.

    // Membebaskan memori yang dialokasikan untuk daftar sementara.
    free(tempList.elements);
}

void displayDoctors(Hospital *hospital, Session *session, int sortBy, int sortOrder)
{
    // Validasi input dan hak akses (hanya Manajer).
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar dokter.");
        return;
    }

    // Header dan penanganan jika tidak ada dokter.
    printHeader("Daftar Dokter");
    if (hospital->doctors.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada dokter terdaftar.\n" COLOR_RESET);
        return;
    }

    // Membuat daftar sementara untuk dokter.
    DoctorList tempList;
    tempList.capacity = hospital->doctors.capacity;
    tempList.nEff = hospital->doctors.nEff;
    tempList.elements = (Doctor *)malloc(tempList.capacity * sizeof(Doctor));
    if (tempList.elements == NULL) // Pemeriksaan kegagalan malloc.
    {
        printError("Gagal alokasi memori untuk daftar dokter sementara!");
        return;
    }

    // Menyalin semua dokter ke daftar sementara.
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        tempList.elements[i] = hospital->doctors.elements[i];
    }

    // Mengurutkan daftar dokter sementara.
    sortDoctors(&tempList, sortBy, sortOrder);

    // String untuk menampilkan kriteria pengurutan yang lebih deskriptif.
    char sortByStr[10], sortOrderStr[10];

    // Menentukan string untuk kriteria pengurutan (sortBy).
    if (sortBy == 1)
        strcpy(sortByStr, "ID");
    else if (sortBy == 2)
        strcpy(sortByStr, "nama");
    else // Diasumsikan kriteria lain (misal sortBy == 3) adalah "aura".
        strcpy(sortByStr, "aura");

    // Menentukan string untuk urutan pengurutan (sortOrder).
    if (sortOrder == 1)
        strcpy(sortOrderStr, "ascending");
    else
        strcpy(sortOrderStr, "descending");

    // Mencetak informasi pengurutan.
    printf("Menampilkan dokter dengan %s terurut %s...\n\n", sortByStr, sortOrderStr);

    // Mendefinisikan lebar kolom dan header untuk tabel dokter.
    int widths[] = {5, 20, 10}; // Kolom: ID, Username, Aura.
    const char *headers[] = {"ID", "Username", "Aura"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    // Melakukan iterasi dan menampilkan setiap dokter dalam daftar yang sudah diurutkan.
    for (int i = 0; i < tempList.nEff; i++)
    {
        Doctor *doctor = &tempList.elements[i]; // Pointer ke dokter saat ini.
        char idStr[10], auraStr[10];            // Buffer untuk ID dan Aura sebagai string.

        // Konversi ID dokter ke string.
        if (!integerToString(doctor->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements); // Bebaskan memori.
            return;
        }

        // Konversi Aura dokter (integer) ke string.
        if (!integerToString(doctor->aura, auraStr, sizeof(auraStr)))
        {
            printError("Gagal mengonversi aura ke string!");
            free(tempList.elements); // Bebaskan memori.
            return;
        }

        // Data untuk baris tabel dokter.
        const char *row[] = {idStr, doctor->username, auraStr};
        printTableRow(row, widths, 3); // Mencetak baris.
    }
    printTableBorder(widths, 3, 3); // Border bawah tabel.

    // Membebaskan memori yang dialokasikan untuk daftar sementara.
    free(tempList.elements);
}