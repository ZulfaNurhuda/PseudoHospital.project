#include "searchUser.h"
#include <stdlib.h> // For qsort and bsearch

// Static comparison function for Users by ID
static int compareUsersByID(const void *a, const void *b) {
    User *userA = (User *)a;
    User *userB = (User *)b;
    if (userA->id < userB->id) return -1;
    if (userA->id > userB->id) return 1;
    return 0;
}

// Static comparison function for Patients by ID
static int comparePatientsByID(const void *a, const void *b) {
    Patient *patientA = (Patient *)a;
    Patient *patientB = (Patient *)b;
    if (patientA->id < patientB->id) return -1;
    if (patientA->id > patientB->id) return 1;
    return 0;
}

// Static comparison function for Doctors by ID
static int compareDoctorsByID(const void *a, const void *b) {
    Doctor *doctorA = (Doctor *)a;
    Doctor *doctorB = (Doctor *)b;
    if (doctorA->id < doctorB->id) return -1;
    if (doctorA->id > doctorB->id) return 1;
    return 0;
}

// Custom binary search for UserList
static User* customBinarySearchUsers(const User *key, User *base, int num, int (*compare)(const void *, const void *)) {
    if (num == 0) return NULL;
    // qsort(base, num, sizeof(User), compare); // qsort should already have been called before this function

    int low = 0;
    int high = num - 1;
    User *foundUser = NULL;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = compare(key, &base[mid]);

        if (cmp == 0) {
            foundUser = &base[mid];
            break; 
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return foundUser;
}

// Custom binary search for PatientList
static Patient* customBinarySearchPatients(const Patient *key, Patient *base, int num, int (*compare)(const void *, const void *)) {
    if (num == 0) return NULL;
    // qsort(base, num, sizeof(Patient), compare); // qsort should already have been called

    int low = 0;
    int high = num - 1;
    Patient *foundPatient = NULL;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = compare(key, &base[mid]);

        if (cmp == 0) {
            foundPatient = &base[mid];
            break;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return foundPatient;
}

// Custom binary search for DoctorList
static Doctor* customBinarySearchDoctors(const Doctor *key, Doctor *base, int num, int (*compare)(const void *, const void *)) {
    if (num == 0) return NULL;
    // qsort(base, num, sizeof(Doctor), compare); // qsort should already have been called

    int low = 0;
    int high = num - 1;
    Doctor *foundDoctor = NULL;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        int cmp = compare(key, &base[mid]);

        if (cmp == 0) {
            foundDoctor = &base[mid];
            break;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return foundDoctor;
}

/**
 * `[Add-ons]` - Nama Fungsi: `containsSubstring`
 * 
 * Deskripsi: Memeriksa apakah `str` mengandung substring `substr`.
 *
 * Parameter:
 *   - `str (const char*)`: String yang akan dicari.
 *   - `substr (const char*)`: Substring yang akan dicari.
 *
 * Return: `boolean` - True jika `str` mengandung `substr`, false jika tidak.
 */
static boolean containsSubstring(const char *str, const char *substr)
{
    if (str == NULL || substr == NULL || str[0] == '\0' || substr[0] == '\0')
    {
        return false;
    }
    int strLen = strlen(str);
    int substrLen = strlen(substr);
    if (substrLen > strLen)
    {
        return false;
    }
    for (int i = 0; i <= strLen - substrLen; i++)
    {
        boolean match = true;
        for (int j = 0; j < substrLen; j++)
        {
            if (str[i + j] != substr[j])
            {
                match = false;
                break;
            }
        }
        if (match)
        {
            return true;
        }
    }
    return false;
}

void findUser(Hospital *hospital, Session *session, const char *query, boolean byId)
{
    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari pengguna.");
        return;
    }

    printHeader("Hasil Pencarian Pengguna");
    if (hospital->users.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pengguna terdaftar.\n" COLOR_RESET);
        return;
    }

    boolean found = false;
    int widths[] = {5, 20, 10, 20};
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"};
    printTableBorder(widths, 4, 1);
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 4, 2);

    if (byId) {
        int targetId = stringToInt(query);
        if (targetId == -1 && strcmp(query, "-1") != 0) { // stringToInt returns -1 on error
             // Check if query was genuinely "-1" or an error.
             // A simple check: if query is not "-1" but result is -1, it's an error.
             // Or if query is "0" and result is -1 (assuming 0 is not a valid ID for stringToInt error)
             // For this code, assuming -1 is not a searchable ID and indicates error from stringToInt
             // if the original query string wasn't also "-1".
            if (strcmp(query, "0") == 0 && targetId == -1) { // stringToInt might return -1 for "0" depending on impl.
                 // If "0" is a valid ID to search, this logic needs refinement.
                 // Let's assume stringToInt returns 0 for "0" if valid, and -1 for actual errors.
                 // For now, if stringToInt returns -1, and query wasn't "-1", assume error.
            } else if (strcmp(query,"-1") != 0) {
                 printError("ID pencarian tidak valid (harus berupa angka).");
                 printTableBorder(widths, 4, 3); // Close table
                 printf(COLOR_YELLOW "Pengguna dengan ID '%s' tidak ditemukan.\n" COLOR_RESET, query);
                 return;
            }
             // If query was "-1" and targetId is -1, proceed if -1 is a possible ID.
             // If -1 is not a possible ID, then this is an error.
             // Simplified: if targetId is -1 from non "-1" string, it's an error.
        }
        
        // Special handling if targetId is 0 and query was "0"
        if (targetId == 0 && strcmp(query, "0") != 0) {
             // This means stringToInt converted non-"0" string to 0, which might be an error or valid if "0" is not allowed as ID
        }


        User keyUser;
        keyUser.id = targetId;

        qsort(hospital->users.elements, hospital->users.nEff, sizeof(User), compareUsersByID);
        // User *foundUser = (User *)bsearch(&keyUser, hospital->users.elements, hospital->users.nEff, sizeof(User), compareUsersByID);
        User *foundUser = customBinarySearchUsers(&keyUser, hospital->users.elements, hospital->users.nEff, compareUsersByID);

        if (foundUser != NULL) {
            found = true;
            char idStr[12]; 
            // Manual integerToString for foundUser->id
            char tempIdConv[20];
            int k_id = 0;
            if (foundUser->id == 0) { tempIdConv[k_id++] = '0'; }
            else {
                long long tempVal_id = foundUser->id;
                boolean isNeg_id = false;
                if (tempVal_id < 0) { isNeg_id = true; tempVal_id = -tempVal_id; }
                int numDigits_id = 0; long long valCopy_id = tempVal_id; if(valCopy_id == 0 && foundUser->id != 0) numDigits_id=0; else if (valCopy_id == 0) numDigits_id=1; else while (valCopy_id > 0) { valCopy_id /= 10; numDigits_id++; }
                k_id = numDigits_id;
                if (tempVal_id == 0 && foundUser->id != 0) {} // Handles if id was 0 and became empty str
                else if (tempVal_id == 0) tempIdConv[0] = '0';
                else while (tempVal_id > 0) { tempIdConv[--k_id] = (tempVal_id % 10) + '0'; tempVal_id /= 10; }
                k_id = numDigits_id;
                if (isNeg_id) { for (int m = k_id; m > 0; m--) tempIdConv[m] = tempIdConv[m-1]; tempIdConv[0] = '-'; k_id++; }
            }
            tempIdConv[k_id] = '\0';
            if (k_id < sizeof(idStr)) strcpy(idStr, tempIdConv); else strcpy(idStr, "BIG");

            char roleStr[20], diseaseStr[20] = "-";
            switch (foundUser->role)
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
            // Corrected: use foundUser->id for checking patient details
            if (foundUser->role == PATIENT && hospital->patients.nEff > 0)
            {
                for (int j = 0; j < hospital->patients.nEff; j++)
                {
                    if (hospital->patients.elements[j].id == foundUser->id)
                    {
                        if (strlen(hospital->patients.elements[j].disease) > 0 && hospital->patients.elements[j].disease[0] != '\0')
                        {
                            strcpy(diseaseStr, hospital->patients.elements[j].disease);
                        }
                        break;
                    }
                }
            }
            const char *row[] = {idStr, foundUser->username, roleStr, diseaseStr};
            printTableRow(row, widths, 4);
        }
    } else { // Search by name (existing logic)
        for (int i = 0; i < hospital->users.nEff; i++) {
            User *user = &hospital->users.elements[i];
            if (containsSubstring(user->username, query)) {
                found = true;
                char idStr[12];
                // Manual integerToString for user->id
                char tempIdConv_name[20];
                int k_id_name = 0;
                if (user->id == 0) { tempIdConv_name[k_id_name++] = '0'; }
                else {
                    long long tempVal_id_name = user->id;
                    boolean isNeg_id_name = false;
                    if (tempVal_id_name < 0) { isNeg_id_name = true; tempVal_id_name = -tempVal_id_name; }
                    int numDigits_id_name = 0; long long valCopy_id_name = tempVal_id_name; if(valCopy_id_name == 0 && user->id != 0) numDigits_id_name=0; else if (valCopy_id_name == 0) numDigits_id_name=1; else while (valCopy_id_name > 0) { valCopy_id_name /= 10; numDigits_id_name++; }
                    k_id_name = numDigits_id_name;
                    if (tempVal_id_name == 0 && user->id != 0) {}
                    else if (tempVal_id_name == 0) tempIdConv_name[0] = '0';
                    else while (tempVal_id_name > 0) { tempIdConv_name[--k_id_name] = (tempVal_id_name % 10) + '0'; tempVal_id_name /= 10; }
                    k_id_name = numDigits_id_name;
                    if (isNeg_id_name) { for (int m = k_id_name; m > 0; m--) tempIdConv_name[m] = tempIdConv_name[m-1]; tempIdConv_name[0] = '-'; k_id_name++; }
                }
                tempIdConv_name[k_id_name] = '\0';
                if (k_id_name < sizeof(idStr)) strcpy(idStr, tempIdConv_name); else strcpy(idStr, "BIG");

                char roleStr[20], diseaseStr[20] = "-";
                switch (user->role) {
                case MANAGER: strcpy(roleStr, "Manajer"); break;
                case DOCTOR: strcpy(roleStr, "Dokter"); break;
                case PATIENT: strcpy(roleStr, "Pasien"); break;
                default: strcpy(roleStr, "Tidak diketahui");
                }
                if (user->role == PATIENT && hospital->patients.nEff > 0) {
                    for (int j = 0; j < hospital->patients.nEff; j++) {
                        if (hospital->patients.elements[j].id == user->id) {
                            if (strlen(hospital->patients.elements[j].disease) > 0 && hospital->patients.elements[j].disease[0] != '\0') {
                                strcpy(diseaseStr, hospital->patients.elements[j].disease);
                            }
                            break;
                        }
                    }
                }
                const char *row[] = {idStr, user->username, roleStr, diseaseStr};
                printTableRow(row, widths, 4);
            }
        }
    }
    printTableBorder(widths, 4, 3);

    if (!found)
    {
        printf(COLOR_YELLOW "Pengguna dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}

// Updated function signature
void findPatient(Hospital *hospital, Session *session, const char *query, boolean byId, boolean byDisease)
{
    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari pasien.");
        return;
    }

    printHeader("Hasil Pencarian Pasien");
    if (hospital->patients.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada pasien terdaftar.\n" COLOR_RESET);
        return;
    }

    boolean found_overall = false; // Use a single flag for overall result
    int widths[] = {5, 20, 20}; // ID, Username, Penyakit
    const char *headers[] = {"ID", "Username", "Penyakit"};
    
    // Print table headers once
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    if (byDisease) {
        for (int i = 0; i < hospital->patients.nEff; i++) {
            Patient *patient = &hospital->patients.elements[i];
            if (strcmp(patient->disease, query) == 0) { // Exact match for disease name
                found_overall = true;
                char idStr[20];
                // Manual int-to-string for patient->id
                char tempIntStr_id[20]; 
                int k_id = 0;
                long long val_id = patient->id;
                if (val_id == 0) { tempIntStr_id[k_id++] = '0'; } 
                else {
                    if (val_id < 0) { /* Should not happen for IDs, but good practice */ val_id = -val_id; /* then add sign handling if needed */ }
                    int numDigits_id = 0; long long cpy_id = val_id; if(cpy_id == 0) numDigits_id=1; else while(cpy_id > 0){ cpy_id /= 10; numDigits_id++; }
                    k_id = numDigits_id; 
                    while (val_id > 0) { tempIntStr_id[--k_id] = (val_id % 10) + '0'; val_id /= 10; }
                    k_id = numDigits_id;
                }
                tempIntStr_id[k_id] = '\0';
                // strcpy(idStr, tempIntStr_id); // Assuming idStr is large enough
                if(k_id < sizeof(idStr)) strcpy(idStr, tempIntStr_id); else strcpy(idStr, "BIG_ID");


                char diseaseStr[50] = "-"; 
                if (patient->disease[0] != '\0') {
                    // strncpy(diseaseStr, patient->disease, sizeof(diseaseStr) - 1); // strncpy not allowed
                    // diseaseStr[sizeof(diseaseStr)-1] = '\0';
                    // Use customSafeStrcpy or manual loop if available/needed, for now direct strcpy assuming size
                    strcpy(diseaseStr, patient->disease); // Assuming diseaseStr is large enough
                }
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
    } else if (byId) {
        int targetId = stringToInt(query);
        if (targetId == -1 && strcmp(query, "-1") != 0 ) {
             if (strcmp(query,"-1") !=0) {
                // Error message will be printed by the common "not found" logic if found_overall is false
             }
        } else { // Valid integer or query was "-1"
            Patient keyPatient;
            keyPatient.id = targetId;

            qsort(hospital->patients.elements, hospital->patients.nEff, sizeof(Patient), comparePatientsByID);
            Patient *foundPatient = customBinarySearchPatients(&keyPatient, hospital->patients.elements, hospital->patients.nEff, comparePatientsByID);
            
            if (foundPatient != NULL) {
                found_overall = true;
                char idStr[12];
                // Manual integerToString for foundPatient->id (copied from previous step)
                char tempIdConv_pat[20]; int k_id_pat = 0;
                if (foundPatient->id == 0) { tempIdConv_pat[k_id_pat++] = '0'; } else {
                    long long tempVal_id_pat = foundPatient->id; boolean isNeg_id_pat = false;
                    if (tempVal_id_pat < 0) { isNeg_id_pat = true; tempVal_id_pat = -tempVal_id_pat; }
                    int numDigits_id_pat = 0; long long valCopy_id_pat = tempVal_id_pat; if(valCopy_id_pat == 0 && foundPatient->id !=0) numDigits_id_pat=0; else if(valCopy_id_pat == 0) numDigits_id_pat=1; else while (valCopy_id_pat > 0) { valCopy_id_pat /= 10; numDigits_id_pat++; }
                    k_id_pat = numDigits_id_pat;
                    if (tempVal_id_pat == 0 && foundPatient->id != 0) {} else if (tempVal_id_pat == 0) tempIdConv_pat[0] = '0'; else while (tempVal_id_pat > 0) { tempIdConv_pat[--k_id_pat] = (tempVal_id_pat % 10) + '0'; tempVal_id_pat /= 10; }
                    k_id_pat = numDigits_id_pat; if (isNeg_id_pat) { for (int m = k_id_pat; m > 0; m--) tempIdConv_pat[m] = tempIdConv_pat[m-1]; tempIdConv_pat[0] = '-'; k_id_pat++; }
                } tempIdConv_pat[k_id_pat] = '\0';
                if (k_id_pat < sizeof(idStr)) strcpy(idStr, tempIdConv_pat); else strcpy(idStr, "BIG");
                
                char diseaseStr[50] = "-"; 
                if (strlen(foundPatient->disease) > 0 && foundPatient->disease[0] != '\0') {
                    // strncpy(diseaseStr, foundPatient->disease, sizeof(diseaseStr) - 1);
                    // diseaseStr[sizeof(diseaseStr)-1] = '\0';
                    strcpy(diseaseStr, foundPatient->disease); // Assuming diseaseStr is large enough
                }
                const char *row[] = {idStr, foundPatient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
    } else { // Search by name
        for (int i = 0; i < hospital->patients.nEff; i++) {
            Patient *patient = &hospital->patients.elements[i];
            if (containsSubstring(patient->username, query)) {
                found_overall = true;
                char idStr[12];
                // Manual integerToString for patient->id (copied from previous step)
                char tempIdConv_pat_name[20]; int k_id_pat_name = 0;
                if (patient->id == 0) { tempIdConv_pat_name[k_id_pat_name++] = '0'; } else {
                    long long tempVal_id_pat_name = patient->id; boolean isNeg_id_pat_name = false;
                    if (tempVal_id_pat_name < 0) { isNeg_id_pat_name = true; tempVal_id_pat_name = -tempVal_id_pat_name; }
                    int numDigits_id_pat_name = 0; long long valCopy_id_pat_name = tempVal_id_pat_name; if(valCopy_id_pat_name == 0 && patient->id != 0) numDigits_id_pat_name=0; else if (valCopy_id_pat_name == 0) numDigits_id_pat_name=1; else while (valCopy_id_pat_name > 0) { valCopy_id_pat_name /= 10; numDigits_id_pat_name++; }
                    k_id_pat_name = numDigits_id_pat_name;
                    if (tempVal_id_pat_name == 0 && patient->id != 0) {} else if (tempVal_id_pat_name == 0) tempIdConv_pat_name[0] = '0'; else while (tempVal_id_pat_name > 0) { tempIdConv_pat_name[--k_id_pat_name] = (tempVal_id_pat_name % 10) + '0'; tempVal_id_pat_name /= 10; }
                    k_id_pat_name = numDigits_id_pat_name; if (isNeg_id_pat_name) { for (int m = k_id_pat_name; m > 0; m--) tempIdConv_pat_name[m] = tempIdConv_pat_name[m-1]; tempIdConv_pat_name[0] = '-'; k_id_pat_name++; }
                } tempIdConv_pat_name[k_id_pat_name] = '\0';
                if (k_id_pat_name < sizeof(idStr)) strcpy(idStr, tempIdConv_pat_name); else strcpy(idStr, "BIG");

                char diseaseStr[50] = "-";
                if (strlen(patient->disease) > 0 && patient->disease[0] != '\0') {
                     // strncpy(diseaseStr, patient->disease, sizeof(diseaseStr) - 1);
                     // diseaseStr[sizeof(diseaseStr)-1] = '\0';
                     strcpy(diseaseStr, patient->disease); // Assuming diseaseStr is large enough
                }
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
    }
    
    // Print table bottom border once if any patient was found, or print not found message.
    if (found_overall) {
        printTableBorder(widths, 3, 3);
    } else {
        // Close the header part of the table if nothing was found before printing message
        printTableBorder(widths, 3, 3); // This might look odd if no rows, but closes the header box
        char notFoundMsg[100];
        char searchTypeStr[20];
        if (byDisease) strcpy(searchTypeStr, "penyakit");
        else if (byId) strcpy(searchTypeStr, "ID");
        else strcpy(searchTypeStr, "nama");
        
        // Manual construction for notFoundMsg
        strcpy(notFoundMsg, "Pasien dengan ");
        strcat(notFoundMsg, searchTypeStr);
        strcat(notFoundMsg, " '");
        strcat(notFoundMsg, query);
        strcat(notFoundMsg, "' tidak ditemukan.");
        printf(COLOR_YELLOW "%s\n" COLOR_RESET, notFoundMsg);
    }
}

void findDoctor(Hospital *hospital, Session *session, const char *query, boolean byId)
{
    if (hospital == NULL || session == NULL || query == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau kueri tidak valid!");
        return;
    }
    if (!session->isLoggedIn || session->role != MANAGER)
    {
        printError("Akses ditolak! Hanya Manajer yang dapat mencari dokter.");
        return;
    }

    printHeader("Hasil Pencarian Dokter");
    if (hospital->doctors.nEff == 0)
    {
        printf(COLOR_YELLOW "Tidak ada dokter terdaftar.\n" COLOR_RESET);
        return;
    }

    boolean found = false;
    int widths[] = {5, 20, 10};
    const char *headers[] = {"ID", "Username", "Aura"};
    printTableBorder(widths, 3, 1);
    printTableRow(headers, widths, 3);
    printTableBorder(widths, 3, 2);

    if (byId) {
        int targetId = stringToInt(query);
        // Similar error checking for targetId as in findUser
        if (targetId == -1 && strcmp(query, "-1") != 0) { // Assuming -1 is error from stringToInt
            if (strcmp(query,"-1") !=0) {
                printError("ID pencarian tidak valid (harus berupa angka).");
                printTableBorder(widths, 3, 3);
                printf(COLOR_YELLOW "Dokter dengan ID '%s' tidak ditemukan.\n" COLOR_RESET, query);
                return;
            }
        }
        
        Doctor keyDoctor;
        keyDoctor.id = targetId;

        qsort(hospital->doctors.elements, hospital->doctors.nEff, sizeof(Doctor), compareDoctorsByID);
        // Doctor *foundDoctor = (Doctor *)bsearch(&keyDoctor, hospital->doctors.elements, hospital->doctors.nEff, sizeof(Doctor), compareDoctorsByID);
        Doctor *foundDoctor = customBinarySearchDoctors(&keyDoctor, hospital->doctors.elements, hospital->doctors.nEff, compareDoctorsByID);

        if (foundDoctor != NULL) {
            found = true;
            char idStr[12], auraStr[16];
            // Manual integerToString for foundDoctor->id
            char tempIdConv_doc[20];
            int k_id_doc = 0;
            if (foundDoctor->id == 0) { tempIdConv_doc[k_id_doc++] = '0'; }
            else {
                long long tempVal_id_doc = foundDoctor->id;
                boolean isNeg_id_doc = false;
                if (tempVal_id_doc < 0) { isNeg_id_doc = true; tempVal_id_doc = -tempVal_id_doc; }
                int numDigits_id_doc = 0; long long valCopy_id_doc = tempVal_id_doc; if(valCopy_id_doc == 0 && foundDoctor->id != 0) numDigits_id_doc=0; else if (valCopy_id_doc == 0) numDigits_id_doc=1; else while (valCopy_id_doc > 0) { valCopy_id_doc /= 10; numDigits_id_doc++; }
                k_id_doc = numDigits_id_doc;
                if (tempVal_id_doc == 0 && foundDoctor->id != 0) {}
                else if (tempVal_id_doc == 0) tempIdConv_doc[0] = '0';
                else while (tempVal_id_doc > 0) { tempIdConv_doc[--k_id_doc] = (tempVal_id_doc % 10) + '0'; tempVal_id_doc /= 10; }
                k_id_doc = numDigits_id_doc;
                if (isNeg_id_doc) { for (int m = k_id_doc; m > 0; m--) tempIdConv_doc[m] = tempIdConv_doc[m-1]; tempIdConv_doc[0] = '-'; k_id_doc++; }
            }
            tempIdConv_doc[k_id_doc] = '\0';
            if (k_id_doc < sizeof(idStr)) strcpy(idStr, tempIdConv_doc); else strcpy(idStr, "BIG");

            // Manual floatToString for foundDoctor->aura (precision 1)
            float floatValue_aura = foundDoctor->aura;
            int precision_aura = 1;
            auraStr[0] = '\0'; // Initialize auraStr
            char tempFloatStr_aura[30]; tempFloatStr_aura[0] = '\0';
            boolean isNeg_aura = false;
            if (floatValue_aura < 0.0f) { isNeg_aura = true; floatValue_aura = -floatValue_aura; strcpy(tempFloatStr_aura, "-");}
            
            long long intPart_aura = (long long)floatValue_aura;
            char intStr_aura[20];
            // Manual int-to-string for intPart_aura
            char tempIntConv_aura[20]; int k_int_aura = 0;
            if (intPart_aura == 0) { tempIntConv_aura[k_int_aura++] = '0'; }
            else {
                long long tempVal_int_aura = intPart_aura;
                int numDigits_int_aura = 0; long long valCopy_int_aura = tempVal_int_aura; if(valCopy_int_aura == 0) numDigits_int_aura=1; else while (valCopy_int_aura > 0) { valCopy_int_aura /= 10; numDigits_int_aura++; }
                k_int_aura = numDigits_int_aura;
                if (tempVal_int_aura == 0) tempIntConv_aura[0] = '0'; else while (tempVal_int_aura > 0) { tempIntConv_aura[--k_int_aura] = (tempVal_int_aura % 10) + '0'; tempVal_int_aura /= 10; }
                k_int_aura = numDigits_int_aura;
            }
            tempIntConv_aura[k_int_aura] = '\0';
            strcpy(intStr_aura, tempIntConv_aura);
            strcat(tempFloatStr_aura, intStr_aura);

            if (precision_aura > 0) {
                strcat(tempFloatStr_aura, ".");
                float fracPart_aura = floatValue_aura - (float)intPart_aura;
                for (int p_aura = 0; p_aura < precision_aura; p_aura++) {
                    fracPart_aura *= 10;
                    int digit_aura = (int)fracPart_aura;
                    char digitChar_aura[2] = { (char)(digit_aura + '0'), '\0' };
                    strcat(tempFloatStr_aura, digitChar_aura);
                    fracPart_aura -= digit_aura;
                }
            }
            if (strlen(tempFloatStr_aura) < sizeof(auraStr)) strcpy(auraStr, tempFloatStr_aura); else strcpy(auraStr, "BIG_F");
            
            const char *row[] = {idStr, foundDoctor->username, auraStr};
            printTableRow(row, widths, 3);
        }
    } else { // Search by name
        for (int i = 0; i < hospital->doctors.nEff; i++) {
            Doctor *doctor = &hospital->doctors.elements[i];
            if (containsSubstring(doctor->username, query)) {
                found = true;
                char idStr[12], auraStr[16];
                // Manual integerToString for doctor->id
                char tempIdConv_doc_name[20];
                int k_id_doc_name = 0;
                if (doctor->id == 0) { tempIdConv_doc_name[k_id_doc_name++] = '0'; }
                else {
                    long long tempVal_id_doc_name = doctor->id;
                    boolean isNeg_id_doc_name = false;
                    if (tempVal_id_doc_name < 0) { isNeg_id_doc_name = true; tempVal_id_doc_name = -tempVal_id_doc_name; }
                    int numDigits_id_doc_name = 0; long long valCopy_id_doc_name = tempVal_id_doc_name; if(valCopy_id_doc_name == 0 && doctor->id !=0) numDigits_id_doc_name=0; else if (valCopy_id_doc_name == 0)numDigits_id_doc_name=1; else while (valCopy_id_doc_name > 0) { valCopy_id_doc_name /= 10; numDigits_id_doc_name++; }
                    k_id_doc_name = numDigits_id_doc_name;
                    if (tempVal_id_doc_name == 0 && doctor->id !=0) {}
                    else if (tempVal_id_doc_name == 0) tempIdConv_doc_name[0] = '0';
                    else while (tempVal_id_doc_name > 0) { tempIdConv_doc_name[--k_id_doc_name] = (tempVal_id_doc_name % 10) + '0'; tempVal_id_doc_name /= 10; }
                    k_id_doc_name = numDigits_id_doc_name;
                    if (isNeg_id_doc_name) { for (int m = k_id_doc_name; m > 0; m--) tempIdConv_doc_name[m] = tempIdConv_doc_name[m-1]; tempIdConv_doc_name[0] = '-'; k_id_doc_name++; }
                }
                tempIdConv_doc_name[k_id_doc_name] = '\0';
                if (k_id_doc_name < sizeof(idStr)) strcpy(idStr, tempIdConv_doc_name); else strcpy(idStr, "BIG");

                // Manual floatToString for doctor->aura (precision 1)
                float floatValue_aura_name = doctor->aura;
                int precision_aura_name = 1;
                auraStr[0] = '\0';
                char tempFloatStr_aura_name[30]; tempFloatStr_aura_name[0] = '\0';
                boolean isNeg_aura_name = false;
                if (floatValue_aura_name < 0.0f) { isNeg_aura_name = true; floatValue_aura_name = -floatValue_aura_name; strcpy(tempFloatStr_aura_name, "-");}

                long long intPart_aura_name = (long long)floatValue_aura_name;
                char intStr_aura_name[20];
                // Manual int-to-string for intPart_aura_name
                char tempIntConv_aura_name[20]; int k_int_aura_name = 0;
                if (intPart_aura_name == 0) { tempIntConv_aura_name[k_int_aura_name++] = '0'; }
                else {
                    long long tempVal_int_aura_name = intPart_aura_name;
                    int numDigits_int_aura_name = 0; long long valCopy_int_aura_name = tempVal_int_aura_name; if(valCopy_int_aura_name == 0) numDigits_int_aura_name=1; else while (valCopy_int_aura_name > 0) { valCopy_int_aura_name /= 10; numDigits_int_aura_name++; }
                    k_int_aura_name = numDigits_int_aura_name;
                    if (tempVal_int_aura_name == 0) tempIntConv_aura_name[0] = '0'; else while (tempVal_int_aura_name > 0) { tempIntConv_aura_name[--k_int_aura_name] = (tempVal_int_aura_name % 10) + '0'; tempVal_int_aura_name /= 10; }
                    k_int_aura_name = numDigits_int_aura_name;
                }
                tempIntConv_aura_name[k_int_aura_name] = '\0';
                strcpy(intStr_aura_name, tempIntConv_aura_name);
                strcat(tempFloatStr_aura_name, intStr_aura_name);

                if (precision_aura_name > 0) {
                    strcat(tempFloatStr_aura_name, ".");
                    float fracPart_aura_name = floatValue_aura_name - (float)intPart_aura_name;
                    for (int p_aura_name = 0; p_aura_name < precision_aura_name; p_aura_name++) {
                        fracPart_aura_name *= 10;
                        int digit_aura_name = (int)fracPart_aura_name;
                        char digitChar_aura_name[2] = { (char)(digit_aura_name + '0'), '\0' };
                        strcat(tempFloatStr_aura_name, digitChar_aura_name);
                        fracPart_aura_name -= digit_aura_name;
                    }
                }
                if (strlen(tempFloatStr_aura_name) < sizeof(auraStr)) strcpy(auraStr, tempFloatStr_aura_name); else strcpy(auraStr, "BIG_F");

                const char *row[] = {idStr, doctor->username, auraStr};
                printTableRow(row, widths, 3);
            }
        }
    }
    printTableBorder(widths, 3, 3);

    if (!found)
    {
        printf(COLOR_YELLOW "Dokter dengan %s '%s' tidak ditemukan.\n" COLOR_RESET, byId ? "ID" : "nama", query);
    }
}