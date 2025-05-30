#include "viewUsers.h"

// Fungsi sorting dari file pertama
int compareUserByIdAsc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    return ua->id - ub->id;
}

int compareUserByIdDesc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    return ub->id - ua->id;
}

int compareUserByNameAsc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    return strcmp(ua->username, ub->username);
}

int compareUserByNameDesc(const void *a, const void *b)
{
    const User *ua = (const User *)a;
    const User *ub = (const User *)b;
    return strcmp(ub->username, ua->username);
}

int comparePatientByIdAsc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return pa->id - pb->id;
}

int comparePatientByIdDesc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return pb->id - pa->id;
}

int comparePatientByNameAsc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return strcmp(pa->username, pb->username);
}

int comparePatientByNameDesc(const void *a, const void *b)
{
    const Patient *pa = (const Patient *)a;
    const Patient *pb = (const Patient *)b;
    return strcmp(pb->username, pa->username);
}

int compareDoctorByIdAsc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return da->id - db->id;
}

int compareDoctorByIdDesc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return db->id - da->id;
}

int compareDoctorByNameAsc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return strcmp(da->username, db->username);
}

int compareDoctorByNameDesc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return strcmp(db->username, da->username);
}

void sortUser(UserSet *userList, int sortBy, int sortOrder)
{
    if (sortBy == 1) // Sort by ID
    {
        if (sortOrder == 1) // ASC
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByIdAsc);
        }
        else // DESC
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByIdDesc);
        }
    }
    else // Sort by Name
    {
        if (sortOrder == 1) // ASC
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByNameAsc);
        }
        else // DESC
        {
            qsort(userList->elements, userList->nEff, sizeof(User), compareUserByNameDesc);
        }
    }
}

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
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat melihat daftar pengguna.");
        return;
    }

    printHeader("Daftar Pengguna");
    if (hospital->users.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pengguna terdaftar.\n" COLOR_RESET);
        return;
    }

    // Create temporary list for sorting
    UserSet tempList;
    tempList.capacity = hospital->users.capacity;
    tempList.nEff = hospital->users.nEff;
    tempList.elements = (User *)malloc(tempList.capacity * sizeof(User));

    // Copy all users to temp list
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        tempList.elements[i] = hospital->users.elements[i];
    }

    // Sort the temporary list
    sortUser(&tempList, sortBy, sortOrder);

    printf("Menampilkan semua pengguna dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",
           (sortOrder == 1) ? "ascending" : "descending");

    int widths[] = {5, 20, 10, 20};
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"};
    printTableBorder(widths, 4, 1);
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 4, 2);

    for (int i = 0; i < tempList.nEff; i++)
    {
        User *user = &tempList.elements[i];
        char idStr[10], roleStr[20], diseaseStr[20] = "-";

        // Convert ID to string
        if (!integerToString(user->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

        // Set role string
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
            strcpy(roleStr, "Tidak diketahui");
        }

        // Set disease for patients
        if (user->role == PATIENT && hospital->patients.nEff > 0)
        {
            for (int j = 0; j < hospital->patients.nEff; j++)
            {
                if (hospital->patients.elements[j].id == user->id)
                {
                    if (strlen(hospital->patients.elements[j].disease) > 0 && hospital->patients.elements[j].disease[0] != '\0')
                    {
                        strcpy(diseaseStr, hospital->patients.elements[j].disease);
                    }
                    break;
                }
            }
        }

        const char *row[] = {idStr, user->username, roleStr, diseaseStr};
        printTableRow(row, widths, 4);
    }
    printTableBorder(widths, 4, 3);

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

    // Create temporary list for sorting
    PatientList tempList;
    tempList.capacity = hospital->patients.capacity;
    tempList.nEff = hospital->patients.nEff;
    tempList.elements = (Patient *)malloc(tempList.capacity * sizeof(Patient));

    // Copy all patients to temp list
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        tempList.elements[i] = hospital->patients.elements[i];
    }

    // Sort the temporary list
    sortPatients(&tempList, sortBy, sortOrder);

    printf("Menampilkan pasien dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",
           (sortOrder == 1) ? "ascending" : "descending");

    int widths[] = {5, 20, 20};
    const char *headers[] = {"ID", "Username", "Penyakit"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    for (int i = 0; i < tempList.nEff; i++)
    {
        Patient *patient = &tempList.elements[i];
        char idStr[10], diseaseStr[20] = "-";

        // Convert ID to string
        if (!integerToString(patient->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

        // Set disease
        if (strlen(patient->disease) > 0 && patient->disease[0] != '\0')
        {
            strcpy(diseaseStr, patient->disease);
        }

        const char *row[] = {idStr, patient->username, diseaseStr};
        printTableRow(row, widths, 3);
    }
    printTableBorder(widths, 3, 3);

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

    // Create temporary list for sorting
    DoctorList tempList;
    tempList.capacity = hospital->doctors.capacity;
    tempList.nEff = hospital->doctors.nEff;
    tempList.elements = (Doctor *)malloc(tempList.capacity * sizeof(Doctor));

    // Copy all doctors to temp list
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        tempList.elements[i] = hospital->doctors.elements[i];
    }

    // Sort the temporary list
    sortDoctors(&tempList, sortBy, sortOrder);

    printf("Menampilkan dokter dengan %s terurut %s...\n\n",
           (sortBy == 1) ? "ID" : "nama",
           (sortOrder == 1) ? "ascending" : "descending");

    int widths[] = {5, 20, 10};
    const char *headers[] = {"ID", "Username", "Aura"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    for (int i = 0; i < tempList.nEff; i++)
    {
        Doctor *doctor = &tempList.elements[i];
        char idStr[10], auraStr[10];

        // Convert ID to string
        if (!integerToString(doctor->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

        // Convert aura to string
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

    free(tempList.elements);
}