#include "viewUsers.h"

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan ID secara menaik (Ascending).
// Digunakan oleh qsort untuk mengurutkan array User.
// Mengembalikan nilai negatif jika ID ua < ID ub, nol jika sama, positif jika ID ua > ID ub.
int compareUserByIdAsc(const void *a, const void *b)
{
    // Meng-cast pointer void ke pointer const User.
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // Mengembalikan selisih ID untuk urutan menaik.
    return ua->id - ub->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan ID secara menurun (Descending).
// Mengembalikan nilai positif jika ID ua < ID ub, nol jika sama, negatif jika ID ua > ID ub.
int compareUserByIdDesc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // Mengembalikan selisih ID terbalik untuk urutan menurun.
    return ub->id - ua->id;
}

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan username secara menaik (Ascending Alfabetis).
// Menggunakan strcmp untuk perbandingan string.
int compareUserByNameAsc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // Mengembalikan hasil strcmp untuk urutan alfabetis menaik.
    return strcmp(ua->username, ub->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua User berdasarkan username secara menurun (Descending Alfabetis).
int compareUserByNameDesc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    // Mengembalikan hasil strcmp terbalik untuk urutan alfabetis menurun.
    return strcmp(ub->username, ua->username);
}

// Fungsi Pembanding untuk qsort: Membandingkan dua Patient berdasarkan ID secara menaik (Ascending).
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

// Fungsi untuk mengurutkan daftar pengguna (UserSet).
// Menerima pointer ke UserSet, kriteria pengurutan (sortBy: 1 untuk ID, lainnya untuk Nama),
// dan urutan pengurutan (sortOrder: 1 untuk Ascending, lainnya untuk Descending).
void sortUser(UserSet *userList, int sortBy, int sortOrder)
{
    // Memeriksa kriteria pengurutan.
    if (sortBy == 1) // Jika diurutkan berdasarkan ID.
    {
        // Memeriksa urutan pengurutan.
        if (sortOrder == 1) // Jika Ascending.
        {
            // Menggunakan qsort dengan fungsi pembanding compareUserByIdAsc.
            // `userList->elements`: array yang akan diurutkan.
            // `userList->nEff`: jumlah elemen dalam array.
            // `sizeof(User)`: ukuran satu elemen.
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByIdAsc);
        }
        else // Jika Descending.
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByIdDesc);
        }
    }
    else // Jika diurutkan berdasarkan Nama.
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
// Logikanya mirip dengan sortUser, tetapi untuk struktur Patient.
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
// Logikanya mirip dengan sortUser, tetapi untuk struktur Doctor.
void sortDoctors(DoctorList *doctorList, int sortBy, int sortOrder)
{
    if (sortBy == 1) // Sort by ID
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
    else // Sort by Name
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
}

void displayUsers(Hospital *hospital, Session *session, int sortBy, int sortOrder)
{
    // Pemeriksaan Awal: Validasi pointer hospital dan session.
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    // Pemeriksaan Hak Akses: Hanya Manajer yang dapat melihat daftar semua pengguna.
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar pengguna.");
        return;
    }

    // Mencetak header untuk daftar pengguna.
    printHeader("Daftar Pengguna");
    // Jika tidak ada pengguna terdaftar.
    if (hospital->users.nEff == 0)
    {
        // Mencetak pesan bahwa tidak ada pengguna dan keluar dari fungsi.
        // COLOR_YELLOW dan COLOR_RESET diasumsikan makro untuk pewarnaan teks.
        printf(COLOR_YELLOW "Tidak ada pengguna terdaftar.\n" COLOR_RESET);
        return;
    }

    // Membuat daftar sementara (tempList) untuk pengurutan agar tidak mengubah urutan asli.
    UserSet tempList;
    tempList.capacity = hospital->users.capacity; // Salin kapasitas.
    tempList.nEff = hospital->users.nEff;         // Salin jumlah elemen efektif.
    // Alokasi memori untuk elemen-elemen dalam tempList.
    tempList.elements = (User *)malloc(tempList.capacity * sizeof(User));
    // Jika alokasi memori gagal.
    if (tempList.elements == NULL)
    {
        printError("Gagal alokasi memori untuk daftar pengguna sementara!");
        return;
    }

    // Menyalin semua pengguna dari daftar asli (hospital->users) ke tempList.
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        tempList.elements[i] = hospital->users.elements[i]; // Menyalin struktur User.
    }

    // Mengurutkan daftar sementara (tempList) berdasarkan kriteria yang diberikan.
    sortUser(&tempList, sortBy, sortOrder);

    // Mencetak informasi mengenai kriteria pengurutan yang digunakan.
    printf("Menampilkan semua pengguna dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",                  // Jika sortBy 1, tampilkan "ID", jika tidak, "nama".
           (sortOrder == 1) ? "ascending" : "descending"); // Jika sortOrder 1, tampilkan "ascending", jika tidak, "descending".

    // Mendefinisikan lebar kolom dan header untuk tabel output.
    int widths[] = {5, 20, 10, 20};                                 // Lebar untuk kolom ID, Username, Role, Penyakit.
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"}; // Nama header kolom.
    // Mencetak border atas dan baris header tabel.
    printTableBorder(widths, 4, 1);    // 4 kolom, tipe border 1 (atas).
    printTableRow(headers, widths, 4); // Mencetak baris header.
    printTableBorder(widths, 4, 2);    // Mencetak garis pemisah di bawah header.

    // Melakukan iterasi melalui daftar pengguna yang sudah diurutkan (tempList).
    for (int i = 0; i < tempList.nEff; i++)
    {
        // Mendapatkan pointer ke pengguna saat ini.
        User *user = &tempList.elements[i];
        // Buffer untuk menyimpan representasi string dari ID, Role, dan Penyakit.
        char idStr[10], roleStr[20], diseaseStr[20] = "-"; // Inisialisasi diseaseStr dengan "-" (strip).

        // Mengkonversi ID pengguna (integer) menjadi string.
        // `integerToString` adalah fungsi kustom yang diasumsikan ada.
        if (!integerToString(user->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements); // Bebaskan memori yang dialokasikan sebelum keluar.
            return;
        }

        // Menentukan string untuk peran (Role) berdasarkan nilai enum/define.
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
        default:
            strcpy(roleStr, "Tidak diketahui"); // Untuk peran yang tidak terdefinisi.
        }

        // Mengambil informasi penyakit jika pengguna adalah Pasien.
        // Memeriksa apakah peran adalah PATIENT dan apakah ada data pasien di rumah sakit.
        if (user->role == PATIENT && hospital->patients.nEff > 0)
        {
            // Mencari data pasien yang sesuai berdasarkan ID pengguna.
            for (int j = 0; j < hospital->patients.nEff; j++)
            {
                if (hospital->patients.elements[j].id == user->id)
                {
                    // Jika pasien ditemukan dan memiliki data penyakit yang valid.
                    // `strlen > 0` dan `[0] != '\0'` untuk memastikan string penyakit tidak kosong.
                    if (strlen(hospital->patients.elements[j].disease) > 0 && hospital->patients.elements[j].disease[0] != '\0')
                    {
                        strcpy(diseaseStr, hospital->patients.elements[j].disease); // Salin nama penyakit.
                    }
                    break; // Keluar dari loop pencarian pasien.
                }
            }
        }

        // Data untuk baris tabel saat ini.
        const char *row[] = {idStr, user->username, roleStr, diseaseStr};
        // Mencetak baris tabel.
        printTableRow(row, widths, 4);
    }
    // Mencetak border bawah tabel.
    printTableBorder(widths, 4, 3); // Tipe border 3 (bawah).

    // Membebaskan memori yang dialokasikan untuk daftar sementara.
    free(tempList.elements);
}

void displayPatients(Hospital *hospital, Session *session, int sortBy, int sortOrder)
{
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

    printHeader("Daftar Pasien");
    if (hospital->patients.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pasien terdaftar.\n" COLOR_RESET);
        return;
    }

    // Membuat daftar sementara untuk pengurutan.
    PatientList tempList;
    tempList.capacity = hospital->patients.capacity;
    tempList.nEff = hospital->patients.nEff;
    tempList.elements = (Patient *)malloc(tempList.capacity * sizeof(Patient));
    if (tempList.elements == NULL)
    {
        printError("Gagal alokasi memori untuk daftar pasien sementara!");
        return;
    }

    // Menyalin semua pasien ke daftar sementara.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        tempList.elements[i] = hospital->patients.elements[i];
    }

    // Mengurutkan daftar sementara.
    sortPatients(&tempList, sortBy, sortOrder);

    printf("Menampilkan pasien dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",
           (sortOrder == 1) ? "ascending" : "descending");

    // Mendefinisikan lebar kolom dan header untuk tabel pasien.
    int widths[] = {5, 20, 20}; // ID, Username, Penyakit.
    const char *headers[] = {"ID", "Username", "Penyakit"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    // Melakukan iterasi melalui daftar pasien yang sudah diurutkan.
    for (int i = 0; i < tempList.nEff; i++)
    {
        Patient *patient = &tempList.elements[i];
        char idStr[10], diseaseStr[20] = "-"; // Inisialisasi diseaseStr dengan "-".

        // Konversi ID ke string.
        if (!integerToString(patient->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

        // Mengambil data penyakit.
        if (strlen(patient->disease) > 0 && patient->disease[0] != '\0')
        {
            strcpy(diseaseStr, patient->disease);
        }

        const char *row[] = {idStr, patient->username, diseaseStr};
        printTableRow(row, widths, 3);
    }
    printTableBorder(widths, 3, 3);

    // Membebaskan memori daftar sementara.
    free(tempList.elements);
}

void displayDoctors(Hospital *hospital, Session *session, int sortBy, int sortOrder)
{
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

    printHeader("Daftar Dokter");
    if (hospital->doctors.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada dokter terdaftar.\n" COLOR_RESET);
        return;
    }

    // Membuat daftar sementara untuk pengurutan.
    DoctorList tempList;
    tempList.capacity = hospital->doctors.capacity;
    tempList.nEff = hospital->doctors.nEff;
    tempList.elements = (Doctor *)malloc(tempList.capacity * sizeof(Doctor));
    if (tempList.elements == NULL)
    {
        printError("Gagal alokasi memori untuk daftar dokter sementara!");
        return;
    }

    // Menyalin semua dokter ke daftar sementara.
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        tempList.elements[i] = hospital->doctors.elements[i];
    }

    // Mengurutkan daftar sementara.
    sortDoctors(&tempList, sortBy, sortOrder);

    printf("Menampilkan dokter dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",
           (sortOrder == 1) ? "ascending" : "descending");

    // Mendefinisikan lebar kolom dan header untuk tabel dokter.
    int widths[] = {5, 20, 10}; // ID, Username, Aura.
    const char *headers[] = {"ID", "Username", "Aura"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    // Melakukan iterasi melalui daftar dokter yang sudah diurutkan.
    for (int i = 0; i < tempList.nEff; i++)
    {
        Doctor *doctor = &tempList.elements[i];
        char idStr[10], auraStr[10]; // Buffer untuk ID dan Aura sebagai string.

        // Konversi ID ke string.
        if (!integerToString(doctor->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

        // Konversi Aura (integer) ke string.
        if (!integerToString(doctor->aura, auraStr, sizeof(auraStr)))
        {
            printError("Gagal mengonversi aura ke string!");
            free(tempList.elements);
            return;
        }

        const char *row[] = {idStr, doctor->username, auraStr};
        printTableRow(row, widths, 3);
    }
    printTableBorder(widths, 3, 3);

    // Membebaskan memori daftar sementara.
    free(tempList.elements);
}