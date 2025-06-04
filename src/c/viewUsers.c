#include "viewUsers.h"

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

int compareDoctorByAuraAsc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return da->aura - db->aura;
}

int compareDoctorByAuraDesc(const void *a, const void *b)
{
    const Doctor *da = (const Doctor *)a;
    const Doctor *db = (const Doctor *)b;
    return db->aura - da->aura;
}

void sortUser(UserSet *userSet, int sortBy, int sortOrder)
{

    if (sortBy == 1)
    {

        if (sortOrder == 1)
        {

            qsort(userSet->elements, userSet->nEff, sizeof(User), compareUserByIdAsc);
        }
        else
        {
            qsort(userSet->elements, userSet->nEff, sizeof(User), compareUserByIdDesc);
        }
    }
    else
    {
        if (sortOrder == 1)
        {
            qsort(userSet->elements, userSet->nEff, sizeof(User), compareUserByNameAsc);
        }
        else
        {
            qsort(userSet->elements, userSet->nEff, sizeof(User), compareUserByNameDesc);
        }
    }
}

void sortPatients(PatientList *patientList, int sortBy, int sortOrder)
{
    if (sortBy == 1)
    {
        if (sortOrder == 1)
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByIdAsc);
        }
        else
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByIdDesc);
        }
    }
    else
    {
        if (sortOrder == 1)
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByNameAsc);
        }
        else
        {
            qsort(patientList->elements, patientList->nEff, sizeof(Patient), comparePatientByNameDesc);
        }
    }
}

void sortDoctors(DoctorList *doctorList, int sortBy, int sortOrder)
{
    if (sortBy == 1)
    {
        if (sortOrder == 1)
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByIdAsc);
        }
        else
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByIdDesc);
        }
    }
    else if (sortBy == 2)
    {
        if (sortOrder == 1)
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByNameAsc);
        }
        else
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByNameDesc);
        }
    }
    else
    {
        if (sortOrder == 1)
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByAuraAsc);
        }
        else
        {
            qsort(doctorList->elements, doctorList->nEff, sizeof(Doctor), compareDoctorByAuraDesc);
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

    UserSet tempList;
    tempList.capacity = hospital->users.capacity;
    tempList.nEff = hospital->users.nEff;

    tempList.elements = (User *)malloc(tempList.capacity * sizeof(User));

    if (tempList.elements == NULL)
    {
        printError("Gagal alokasi memori untuk daftar pengguna sementara!");
        return;
    }

    for (int i = 0; i < hospital->users.nEff; i++)
    {
        tempList.elements[i] = hospital->users.elements[i];
    }

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

        if (!integerToString(user->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

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

    PatientList tempList;
    tempList.capacity = hospital->patients.capacity;
    tempList.nEff = hospital->patients.nEff;
    tempList.elements = (Patient *)malloc(tempList.capacity * sizeof(Patient));
    if (tempList.elements == NULL)
    {
        printError("Gagal alokasi memori untuk daftar pasien sementara!");
        return;
    }

    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        tempList.elements[i] = hospital->patients.elements[i];
    }

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

        if (!integerToString(patient->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

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

    DoctorList tempList;
    tempList.capacity = hospital->doctors.capacity;
    tempList.nEff = hospital->doctors.nEff;
    tempList.elements = (Doctor *)malloc(tempList.capacity * sizeof(Doctor));
    if (tempList.elements == NULL)
    {
        printError("Gagal alokasi memori untuk daftar dokter sementara!");
        return;
    }

    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        tempList.elements[i] = hospital->doctors.elements[i];
    }

    sortDoctors(&tempList, sortBy, sortOrder);

    char sortByStr[10], sortOrderStr[15];

    if (sortBy == 1)
        strcpy(sortByStr, "ID");
    else if (sortBy == 2)
        strcpy(sortByStr, "nama");
    else
        strcpy(sortByStr, "aura");

    if (sortOrder == 1)
        strcpy(sortOrderStr, "ascending");
    else
        strcpy(sortOrderStr, "descending");

    printf("Menampilkan dokter dengan %s terurut %s...\n\n", sortByStr, sortOrderStr);

    int widths[] = {5, 20, 10};
    const char *headers[] = {"ID", "Username", "Aura"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    for (int i = 0; i < tempList.nEff; i++)
    {
        Doctor *doctor = &tempList.elements[i];
        char idStr[10], auraStr[10];

        if (!integerToString(doctor->id, idStr, sizeof(idStr)))
        {
            printError("Gagal mengonversi ID ke string!");
            free(tempList.elements);
            return;
        }

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