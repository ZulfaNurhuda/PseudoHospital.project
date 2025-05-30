#include "searchUser.h"
#include "utils.h" // For integerToString, floatToString, printError, etc.
#include <stdlib.h> 
#include <string.h> 

// Static helper: Convert char to lowercase
static char localCharToLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

// Static helper: Case-insensitive string comparison
static int localCaseInsensitiveStrcmp(const char *s1, const char *s2) {
    if (s1 == NULL && s2 == NULL) return 0;
    if (s1 == NULL) return -1;
    if (s2 == NULL) return 1;
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        char c1_lower = localCharToLower(s1[i]);
        char c2_lower = localCharToLower(s2[i]);
        if (c1_lower != c2_lower) {
            return c1_lower - c2_lower;
        }
        i++;
    }
    return localCharToLower(s1[i]) - localCharToLower(s2[i]);
}

// Static helper: Case-insensitive substring check
static boolean localContainsCaseInsensitiveSubstring(const char *text, const char *pattern) {
    if (text == NULL || pattern == NULL) return false;
    int textLen = 0; while(text[textLen] != '\0') textLen++;
    int patternLen = 0; while(pattern[patternLen] != '\0') patternLen++;
    if (patternLen == 0) return true;
    if (patternLen > textLen) return false;
    for (int i = 0; i <= textLen - patternLen; i++) {
        boolean match = true;
        for (int j = 0; j < patternLen; j++) {
            if (localCharToLower(text[i + j]) != localCharToLower(pattern[j])) {
                match = false; break;
            }
        }
        if (match) return true;
    }
    return false;
}

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

    boolean overallFound = false; 
    boolean printedTableHeaders = false;
    int widths[] = {5, 20, 10, 20}; // ID, Username, Role, Penyakit
    const char *headers[] = {"ID", "Username", "Role", "Penyakit"};
    // Table headers are now printed conditionally when the first result is found.

    char idStr[12];      // Buffer for ID string
    char roleStr[20];    // Buffer for Role string
    char diseaseStr[50]; // Buffer for Disease string (max 50 as per Patient struct)

    if (byId) {
        int targetId = stringToInt(query);
        // Simplified error check for ID format, actual "not found" is handled by overallFound
        if (targetId == -1 && strcmp(query, "-1") != 0 && strcmp(query, "0") != 0) {
             if (strcmp(query,"-1")!=0) { // if query is not "-1" or "0" but stringToInt returned -1
                printError("ID pencarian tidak valid (harus berupa angka).");
                printf(COLOR_YELLOW "Pengguna dengan ID '%s' tidak ditemukan.\n" COLOR_RESET, query);
                return;
             }
        }
        
        User keyUser; keyUser.id = targetId;
        qsort(hospital->users.elements, hospital->users.nEff, sizeof(User), compareUsersByID);
        User *foundUser = customBinarySearchUsers(&keyUser, hospital->users.elements, hospital->users.nEff, compareUsersByID);

        if (foundUser != NULL) {
            if (!printedTableHeaders) {
                printTableBorder(widths, 4, 1); printTableRow(headers, widths, 4); printTableBorder(widths, 4, 2);
                printedTableHeaders = true;
            }
            overallFound = true;
            if (!integerToString(foundUser->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
            
            switch (foundUser->role) {
                case MANAGER: strcpy(roleStr, "Manajer"); break;
                case DOCTOR: strcpy(roleStr, "Dokter"); break;
                case PATIENT: strcpy(roleStr, "Pasien"); break;
                default: strcpy(roleStr, "N/A"); break;
            }
            strcpy(diseaseStr, "-");
            if (foundUser->role == PATIENT) {
                for (int j = 0; j < hospital->patients.nEff; j++) {
                    if (hospital->patients.elements[j].id == foundUser->id && hospital->patients.elements[j].disease[0] != '\0') {
                        strcpy(diseaseStr, hospital->patients.elements[j].disease);
                        break;
                    }
                }
            }
            const char *row[] = {idStr, foundUser->username, roleStr, diseaseStr};
            printTableRow(row, widths, 4);
        }
    } else { // Search by name (two-tier)
        boolean exactMatchFound = false;
        for (int i = 0; i < hospital->users.nEff; i++) {
            User *user = &hospital->users.elements[i];
            if (customCaseInsensitiveStrcmp(user->username, query) == 0) {
                if (!printedTableHeaders) {
                    printTableBorder(widths, 4, 1); printTableRow(headers, widths, 4); printTableBorder(widths, 4, 2);
                    printedTableHeaders = true;
                }
                overallFound = true; exactMatchFound = true;
                if (!integerToString(user->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
                switch (user->role) { 
                    case MANAGER: strcpy(roleStr, "Manajer"); break;
                    case DOCTOR: strcpy(roleStr, "Dokter"); break;
                    case PATIENT: strcpy(roleStr, "Pasien"); break;
                    default: strcpy(roleStr, "N/A"); break;
                }
                strcpy(diseaseStr, "-");
                if (user->role == PATIENT) { 
                    for (int j = 0; j < hospital->patients.nEff; j++) {
                        if (hospital->patients.elements[j].id == user->id && hospital->patients.elements[j].disease[0] != '\0') {
                             strcpy(diseaseStr, hospital->patients.elements[j].disease); break;
                        }
                    }
                }
                const char *row[] = {idStr, user->username, roleStr, diseaseStr};
                printTableRow(row, widths, 4);
            }
        }

        if (!exactMatchFound && query[0] != '\0') { // Only search for suggestions if no exact match and query is not empty
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->users.nEff; i++) {
                User *user = &hospital->users.elements[i];
                if (containsCaseInsensitiveSubstring(user->username, query)) {
                    if (!printedTableHeaders) {
                        printTableBorder(widths, 4, 1); printTableRow(headers, widths, 4); printTableBorder(widths, 4, 2);
                        printedTableHeaders = true;
                    }
                    if (!suggestionsFound) {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    overallFound = true;
                    if (!integerToString(user->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
                    switch (user->role) { 
                        case MANAGER: strcpy(roleStr, "Manajer"); break;
                        case DOCTOR: strcpy(roleStr, "Dokter"); break;
                        case PATIENT: strcpy(roleStr, "Pasien"); break;
                        default: strcpy(roleStr, "N/A"); break;
                    }
                     strcpy(diseaseStr, "-");
                    if (user->role == PATIENT) { 
                        for (int j = 0; j < hospital->patients.nEff; j++) {
                            if (hospital->patients.elements[j].id == user->id && hospital->patients.elements[j].disease[0] != '\0') {
                                strcpy(diseaseStr, hospital->patients.elements[j].disease); break;
                            }
                        }
                    }
                    const char *row[] = {idStr, user->username, roleStr, diseaseStr};
                    printTableRow(row, widths, 4);
                }
            }
        }
    }

    if (overallFound) {
        printTableBorder(widths, 4, 3);
    } else {
        // If headers were printed, it implies an ID search for a validly formatted ID that wasn't found.
        // So, close the table. Otherwise, just print the message.
        if (printedTableHeaders) { 
             printTableBorder(widths, 4, 3);
        }
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

    boolean overallFound = false; 
    boolean printedTableHeaders = false;
    int widths[] = {5, 20, 50}; // ID, Username, Penyakit (diseaseStr width increased)
    const char *headers[] = {"ID", "Username", "Penyakit"};
    char idStr[12];
    char diseaseStr[51]; // Max disease name 50 + null

    if (byDisease) {
        boolean exactMatchFound = false;
        for (int i = 0; i < hospital->patients.nEff; i++) {
            Patient *patient = &hospital->patients.elements[i];
            if (customCaseInsensitiveStrcmp(patient->disease, query) == 0) {
                if (!printedTableHeaders) {
                    printTableBorder(widths, 3, 1); printTableRow(headers, widths, 3); printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true; exactMatchFound = true;
                if (!integerToString(patient->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
                strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
        if (!exactMatchFound && query[0] != '\0') {
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->patients.nEff; i++) {
                Patient *patient = &hospital->patients.elements[i];
                if (containsCaseInsensitiveSubstring(patient->disease, query)) {
                    if (!printedTableHeaders) {
                        printTableBorder(widths, 3, 1); printTableRow(headers, widths, 3); printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }
                    if (!suggestionsFound) {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk penyakit. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    overallFound = true;
                    if (!integerToString(patient->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
                    strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                    const char *row[] = {idStr, patient->username, diseaseStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    } else if (byId) {
        int targetId = stringToInt(query);
        if (targetId == -1 && strcmp(query,"-1")!=0 && strcmp(query, "0") !=0) { 
            printError("ID pencarian tidak valid."); 
            printf(COLOR_YELLOW "Pasien dengan ID '%s' tidak ditemukan.\n" COLOR_RESET, query); return;
        }
        Patient keyPatient; keyPatient.id = targetId;
        qsort(hospital->patients.elements, hospital->patients.nEff, sizeof(Patient), comparePatientsByID);
        Patient *foundPatient = customBinarySearchPatients(&keyPatient, hospital->patients.elements, hospital->patients.nEff, comparePatientsByID);
        if (foundPatient != NULL) {
            if (!printedTableHeaders) {
                printTableBorder(widths, 3, 1); printTableRow(headers, widths, 3); printTableBorder(widths, 3, 2);
                printedTableHeaders = true;
            }
            overallFound = true;
            if (!integerToString(foundPatient->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
            strcpy(diseaseStr, foundPatient->disease[0] != '\0' ? foundPatient->disease : "-");
            const char *row[] = {idStr, foundPatient->username, diseaseStr};
            printTableRow(row, widths, 3);
        }
    } else { // Search by name
        boolean exactMatchFound = false;
        for (int i = 0; i < hospital->patients.nEff; i++) {
            Patient *patient = &hospital->patients.elements[i];
            if (customCaseInsensitiveStrcmp(patient->username, query) == 0) {
                if (!printedTableHeaders) {
                    printTableBorder(widths, 3, 1); printTableRow(headers, widths, 3); printTableBorder(widths, 3, 2);
                    printedTableHeaders = true;
                }
                overallFound = true; exactMatchFound = true;
                if (!integerToString(patient->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
                strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                const char *row[] = {idStr, patient->username, diseaseStr};
                printTableRow(row, widths, 3);
            }
        }
        if (!exactMatchFound && query[0] != '\0') {
            boolean suggestionsFound = false;
            for (int i = 0; i < hospital->patients.nEff; i++) {
                Patient *patient = &hospital->patients.elements[i];
                if (containsCaseInsensitiveSubstring(patient->username, query)) {
                     if (!printedTableHeaders) {
                        printTableBorder(widths, 3, 1); printTableRow(headers, widths, 3); printTableBorder(widths, 3, 2);
                        printedTableHeaders = true;
                    }
                    if (!suggestionsFound) {
                        printf(COLOR_YELLOW "Tidak ada hasil pencocokan pasti untuk nama. Mungkin maksud Anda:\n" COLOR_RESET);
                        suggestionsFound = true;
                    }
                    overallFound = true;
                    if (!integerToString(patient->id, idStr, sizeof(idStr))) { strcpy(idStr, "ERR"); }
                    strcpy(diseaseStr, patient->disease[0] != '\0' ? patient->disease : "-");
                    const char *row[] = {idStr, patient->username, diseaseStr};
                    printTableRow(row, widths, 3);
                }
            }
        }
    }
    
    if (overallFound) {
        printTableBorder(widths, 3, 3);
    } else {
        if (printedTableHeaders) { printTableBorder(widths, 3, 3); }
        char searchTypeStr[20];
        if (byDisease) strcpy(searchTypeStr, "penyakit");
        else if (byId) strcpy(searchTypeStr, "ID");
        else strcpy(searchTypeStr, "nama");
        char notFoundMsg[100];
        strcpy(notFoundMsg, "Pasien dengan "); strcat(notFoundMsg, searchTypeStr); strcat(notFoundMsg, " '");
        strcat(notFoundMsg, query); strcat(notFoundMsg, "' tidak ditemukan.");
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