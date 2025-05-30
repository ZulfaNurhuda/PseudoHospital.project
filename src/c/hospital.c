#include "hospital.h"
#include "utils.h" // Ensure printError is available
// #include "stringSet.h" // Removed as StringSet ADT is being deleted

boolean initHospital(Hospital *hospital, int userCapacity, int patientCapacity, int doctorCapacity, int roomRows, int roomCols)
{
    if (hospital == NULL) {
        printError("Pointer hospital tidak valid!");
        return false;
    }
    if (userCapacity <= 0 || patientCapacity <= 0 || doctorCapacity <= 0 || roomRows <= 0 || roomCols <= 0)
    {
        printError("Kapasitas atau ukuran denah tidak valid!");
        return false;
    }

    // Initialize all pointers to NULL first, important for cleanup logic
    hospital->users.elements = NULL;
    hospital->patients.elements = NULL;
    hospital->doctors.elements = NULL;
    hospital->layout.elements = NULL;
    hospital->queues.queues = NULL;
    hospital->diseases.elements = NULL;
    hospital->medications.elements = NULL;
    hospital->prescriptions.elements = NULL;
    hospital->treatmentHistory.elements = NULL;

    // Inisialisasi UserSet (formerly UserList)
    hospital->users.elements = (User *)safeMalloc(userCapacity * sizeof(User));
    if (hospital->users.elements == NULL) {
        printError("Gagal alokasi memori untuk UserList!");
        return false; // No prior allocations in this function to free
    }
    hospital->users.capacity = userCapacity;
    hospital->users.nEff = 0;

    // Inisialisasi PatientList
    hospital->patients.elements = (Patient *)safeMalloc(patientCapacity * sizeof(Patient));
    if (hospital->patients.elements == NULL) {
        printError("Gagal alokasi memori untuk PatientList!");
        free(hospital->users.elements); hospital->users.elements = NULL;
        return false;
    }
    hospital->patients.capacity = patientCapacity;
    hospital->patients.nEff = 0;

    // Inisialisasi DoctorList
    hospital->doctors.elements = (Doctor *)safeMalloc(doctorCapacity * sizeof(Doctor));
    if (hospital->doctors.elements == NULL) {
        printError("Gagal alokasi memori untuk DoctorList!");
        free(hospital->users.elements); hospital->users.elements = NULL;
        free(hospital->patients.elements); hospital->patients.elements = NULL;
        return false;
    }
    hospital->doctors.capacity = doctorCapacity;
    hospital->doctors.nEff = 0;

    // Inisialisasi Layout
    hospital->layout.elements = (Room **)safeMalloc(roomRows * sizeof(Room *));
    if (hospital->layout.elements == NULL) {
        printError("Gagal alokasi memori untuk Layout (rows)!");
        free(hospital->users.elements); hospital->users.elements = NULL;
        free(hospital->patients.elements); hospital->patients.elements = NULL;
        free(hospital->doctors.elements); hospital->doctors.elements = NULL;
        return false;
    }
    // Initialize row pointers to NULL for partial allocation cleanup
    for (int i = 0; i < roomRows; i++) {
        hospital->layout.elements[i] = NULL;
    }

    for (int i = 0; i < roomRows; i++)
    {
        hospital->layout.elements[i] = (Room *)safeMalloc(roomCols * sizeof(Room));
        if (hospital->layout.elements[i] == NULL) {
            printError("Gagal alokasi memori untuk Layout (cols)!");
            for (int k = 0; k < i; k++) { // Free previously allocated rows
                // Need to free patientInRoom.patientId within each column of row k first
                for (int l = 0; l < roomCols; l++) {
                    if (hospital->layout.elements[k] != NULL && hospital->layout.elements[k][l].patientInRoom.patientId != NULL) {
                         free(hospital->layout.elements[k][l].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[k]);
            }
            free(hospital->layout.elements); hospital->layout.elements = NULL;
            free(hospital->users.elements); hospital->users.elements = NULL;
            free(hospital->patients.elements); hospital->patients.elements = NULL;
            free(hospital->doctors.elements); hospital->doctors.elements = NULL;
            return false;
        }
        // Initialize patientInRoom.patientId pointers to NULL for this row
        for (int j = 0; j < roomCols; j++) {
            hospital->layout.elements[i][j].patientInRoom.patientId = NULL;
        }

        for (int j = 0; j < roomCols; j++)
        {
            char code[5] = "";
            code[0] = 'A' + i;
            char colStr[4] = "";
            int col = j + 1;
            int index = 0;
            while (col > 0)
            {
                colStr[index++] = (col % 10) + '0';
                col /= 10;
            }
            for (int k = index - 1; k >= 0; k--) {
                // strncat(code, &colStr[k], 1);
                int currentLen = strlen(code);
                if (currentLen < sizeof(code) - 1) {
                    code[currentLen] = colStr[k];
                    code[currentLen+1] = '\0';
                }
            }
            // strcpy(hospital->layout.elements[i][j].code, code); // This strcpy is fine.
            // The previous line was already using customSafeStrcpy implicitly if it was from another change.
            // However, the target here is to ensure 'code' is built safely, then copied.
            // strcpy is fine if 'code' is safely constructed and null-terminated.
            // The problem statement implies replacing strncat, not necessarily all strcpy.
            // The existing strcpy here is fine as 'code' is a local, safely constructed string.
            // customSafeStrcpy(hospital->layout.elements[i][j].code, code, sizeof(hospital->layout.elements[i][j].code));
            strcpy(hospital->layout.elements[i][j].code, code); // Assuming .code is large enough
            hospital->layout.elements[i][j].capacity = 3;
            hospital->layout.elements[i][j].doctorId = -1;
            hospital->layout.elements[i][j].patientInRoom.patientId = (int *)safeMalloc(3 * sizeof(int));
            if (hospital->layout.elements[i][j].patientInRoom.patientId == NULL) {
                printError("Gagal alokasi memori untuk patientInRoom.patientId!");
                // Cleanup: free all patientInRoom.patientId in current row up to column j-1
                for (int m = 0; m < j; m++) {
                    if (hospital->layout.elements[i][m].patientInRoom.patientId != NULL) {
                        free(hospital->layout.elements[i][m].patientInRoom.patientId);
                    }
                }
                // Free current row i
                free(hospital->layout.elements[i]);
                // Free previously allocated full rows (0 to i-1)
                for (int k = 0; k < i; k++) {
                    for (int l = 0; l < roomCols; l++) { // Free patientId arrays in these rows
                        if (hospital->layout.elements[k] != NULL && hospital->layout.elements[k][l].patientInRoom.patientId != NULL) {
                           free(hospital->layout.elements[k][l].patientInRoom.patientId);
                        }
                    }
                    free(hospital->layout.elements[k]);
                }
                free(hospital->layout.elements); hospital->layout.elements = NULL;
                free(hospital->users.elements); hospital->users.elements = NULL;
                free(hospital->patients.elements); hospital->patients.elements = NULL;
                free(hospital->doctors.elements); hospital->doctors.elements = NULL;
                return false;
            }
            hospital->layout.elements[i][j].patientInRoom.capacity = 3;
            hospital->layout.elements[i][j].patientInRoom.nEff = 0;
        }
    }
    hospital->layout.rowCapacity = roomRows;
    hospital->layout.colCapacity = roomCols;
    hospital->layout.rowEff = roomRows;
    hospital->layout.colEff = roomCols;

    // Inisialisasi HospitalQueueList
    hospital->queues.queues = (Queue *)safeMalloc(roomRows * roomCols * sizeof(Queue));
    if (hospital->queues.queues == NULL) {
        printError("Gagal alokasi memori untuk HospitalQueueList!");
        // Full cleanup for layout
        for (int i = 0; i < roomRows; i++) {
            if (hospital->layout.elements[i] != NULL) {
                for (int j = 0; j < roomCols; j++) {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL) {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        free(hospital->layout.elements); hospital->layout.elements = NULL;
        free(hospital->users.elements); hospital->users.elements = NULL;
        free(hospital->patients.elements); hospital->patients.elements = NULL;
        free(hospital->doctors.elements); hospital->doctors.elements = NULL;
        return false;
    }
    hospital->queues.capacity = roomRows * roomCols;
    hospital->queues.nRooms = 0;
    for (int i = 0; i < hospital->queues.capacity; i++) { // Iterate up to the allocated capacity for the list of queues
        hospital->queues.queues[i].front = NULL;
        hospital->queues.queues[i].rear = NULL;
        hospital->queues.queues[i].size = 0;
        hospital->queues.queues[i].roomCode[0] = '\0'; // roomCode might be set when a doctor is assigned or a queue is actively used
    }

    // Inisialisasi DiseaseList
    hospital->diseases.elements = (Disease *)safeMalloc(5 * sizeof(Disease));
    if (hospital->diseases.elements == NULL) {
        printError("Gagal alokasi memori untuk DiseaseList!");
        free(hospital->queues.queues); hospital->queues.queues = NULL;
        // Full cleanup for layout (copied for brevity, ideally a helper)
        for (int i = 0; i < roomRows; i++) {
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL) {
                for (int j = 0; j < roomCols; j++) {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL) {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        if (hospital->layout.elements != NULL) { free(hospital->layout.elements); hospital->layout.elements = NULL; }
        free(hospital->users.elements); hospital->users.elements = NULL;
        free(hospital->patients.elements); hospital->patients.elements = NULL;
        free(hospital->doctors.elements); hospital->doctors.elements = NULL;
        return false;
    }
    hospital->diseases.capacity = 5;
    hospital->diseases.nEff = 5;
    Disease diseases[] = {
        {1, "Influenza", 36.0, 38.5, 90, 120, 60, 80, 60, 100, 95.0, 100.0, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {5, "Anemia", 36.0, 37.0, 90, 120, 60, 80, 60, 100, 95.0, 100.0, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {2, "COVID-19", 36.0, 39.0, 90, 130, 60, 85, 60, 120, 90.0, 100.0, 70.0, 180.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {3, "Hipertensi", 36.0, 37.5, 140, 180, 90, 120, 60, 100, 90.0, 100.0, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {4, "Diabetes Mellitus", 36.0, 37.5, 90, 140, 60, 90, 60, 100, 90.0, 100.0, 126.0, 200.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450}
    };
    for (int i = 0; i < hospital->diseases.nEff; i++)
    {
        hospital->diseases.elements[i] = diseases[i]; 
        strcpy(hospital->diseases.elements[i].name, diseases[i].name); 
    }

    // Inisialisasi MedicationList
    hospital->medications.elements = (Medication *)safeMalloc(5 * sizeof(Medication));
    if (hospital->medications.elements == NULL) {
        printError("Gagal alokasi memori untuk MedicationList!");
        free(hospital->diseases.elements); hospital->diseases.elements = NULL;
        free(hospital->queues.queues); hospital->queues.queues = NULL;
        // Full cleanup for layout
        for (int i = 0; i < roomRows; i++) {
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL) {
                for (int j = 0; j < roomCols; j++) {
                     if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL) {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
       if (hospital->layout.elements != NULL) { free(hospital->layout.elements); hospital->layout.elements = NULL; }
        free(hospital->users.elements); hospital->users.elements = NULL;
        free(hospital->patients.elements); hospital->patients.elements = NULL;
        free(hospital->doctors.elements); hospital->doctors.elements = NULL;
        return false;
    }
    hospital->medications.capacity = 5;
    hospital->medications.nEff = 5;
    Medication medications[] = {
        {2, "Vitamin C"},
        {1, "Oseltamivir"},
        {4, "Lisinopril"},
        {5, "Metformin"},
        {3, "Remdesivir"}
    };
    for (int i = 0; i < hospital->medications.nEff; i++)
    {
        hospital->medications.elements[i] = medications[i]; 
        strcpy(hospital->medications.elements[i].name, medications[i].name); 
    }

    // Inisialisasi PrescriptionList
    hospital->prescriptions.elements = (MedicationPrescription *)safeMalloc(10 * sizeof(MedicationPrescription));
    if (hospital->prescriptions.elements == NULL) {
        printError("Gagal alokasi memori untuk PrescriptionList!");
        free(hospital->medications.elements); hospital->medications.elements = NULL;
        free(hospital->diseases.elements); hospital->diseases.elements = NULL;
        free(hospital->queues.queues); hospital->queues.queues = NULL;
        // Full cleanup for layout
        for (int i = 0; i < roomRows; i++) {
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL) {
                for (int j = 0; j < roomCols; j++) {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL) {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        if (hospital->layout.elements != NULL) { free(hospital->layout.elements); hospital->layout.elements = NULL; }
        free(hospital->users.elements); hospital->users.elements = NULL;
        free(hospital->patients.elements); hospital->patients.elements = NULL;
        free(hospital->doctors.elements); hospital->doctors.elements = NULL;
        return false;
    }
    hospital->prescriptions.capacity = 10;
    hospital->prescriptions.nEff = 10;
    MedicationPrescription prescriptions[] = {
        {1, 1, 1},
        {3, 2, 1},
        {2, 1, 2},
        {4, 4, 2},
        {4, 3, 1},
        {5, 4, 1},
        {2, 5, 2},
        {3, 4, 3},
        {2, 4, 4},
        {3, 5, 1}
    };
    for (int i = 0; i < hospital->prescriptions.nEff; i++)
    {
        hospital->prescriptions.elements[i] = prescriptions[i];
    }

    // Inisialisasi HospitalFinance
    hospital->finance.hospitalBalance = 10000.0;

    // Inisialisasi TreatmentHistoryList
    hospital->treatmentHistory.elements = (TreatmentHistory *)safeMalloc(100 * sizeof(TreatmentHistory));
    if (hospital->treatmentHistory.elements == NULL) {
        printError("Gagal alokasi memori untuk TreatmentHistoryList!");
        free(hospital->prescriptions.elements); hospital->prescriptions.elements = NULL;
        free(hospital->medications.elements); hospital->medications.elements = NULL;
        free(hospital->diseases.elements); hospital->diseases.elements = NULL;
        free(hospital->queues.queues); hospital->queues.queues = NULL;
        // Full cleanup for layout
        for (int i = 0; i < roomRows; i++) {
            if (hospital->layout.elements != NULL && hospital->layout.elements[i] != NULL) {
                for (int j = 0; j < roomCols; j++) {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId != NULL) {
                         free(hospital->layout.elements[i][j].patientInRoom.patientId);
                    }
                }
                free(hospital->layout.elements[i]);
            }
        }
        if (hospital->layout.elements != NULL) { free(hospital->layout.elements); hospital->layout.elements = NULL; }
        free(hospital->users.elements); hospital->users.elements = NULL;
        free(hospital->patients.elements); hospital->patients.elements = NULL;
        free(hospital->doctors.elements); hospital->doctors.elements = NULL;
        return false;
    }
    hospital->treatmentHistory.capacity = 100;
    hospital->treatmentHistory.nEff = 0;

    // Initialize the registeredUsernames set // Removed
    // initializeStringSet(&hospital->registeredUsernames);

    printSuccess("Rumah sakit berhasil diinisialisasi!");
    return true;
}

void freeHospital(Hospital *hospital)
{
    if (hospital->users.elements)
    {
        free(hospital->users.elements);
        hospital->users.elements = NULL;
    }
    if (hospital->patients.elements)
    {
        // Iterate through each patient and free their medication lists FIRST
        for (int i = 0; i < hospital->patients.nEff; i++) {
            if (hospital->patients.elements[i].medicationsPrescribed.medicationId != NULL) {
                free(hospital->patients.elements[i].medicationsPrescribed.medicationId);
                hospital->patients.elements[i].medicationsPrescribed.medicationId = NULL;
            }
            if (hospital->patients.elements[i].medicationsTaken.medicationId != NULL) {
                free(hospital->patients.elements[i].medicationsTaken.medicationId);
                hospital->patients.elements[i].medicationsTaken.medicationId = NULL;
            }
        }
        // Now, it's safe to free the list of patients itself
        free(hospital->patients.elements);
        hospital->patients.elements = NULL;
    }
    if (hospital->doctors.elements)
    {
        free(hospital->doctors.elements);
        hospital->doctors.elements = NULL;
    }
    if (hospital->layout.elements)
    {
        for (int i = 0; i < hospital->layout.rowCapacity; i++)
        {
            if (hospital->layout.elements[i])
            {
                for (int j = 0; j < hospital->layout.colCapacity; j++)
                {
                    if (hospital->layout.elements[i][j].patientInRoom.patientId)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientId);
                        hospital->layout.elements[i][j].patientInRoom.patientId = NULL;
                    }
                }
                free(hospital->layout.elements[i]);
                hospital->layout.elements[i] = NULL;
            }
        }
        free(hospital->layout.elements);
        hospital->layout.elements = NULL;
    }
    if (hospital->queues.queues) {
        for (int i = 0; i < hospital->queues.capacity; i++) { // Iterate up to the allocated capacity for the list of queues
            QueueNode *current = hospital->queues.queues[i].front;
            QueueNode *nextNode;
            while (current != NULL) {
                nextNode = current->next;
                free(current);
                current = nextNode;
            }
            hospital->queues.queues[i].front = NULL;
            hospital->queues.queues[i].rear = NULL;
        }
        free(hospital->queues.queues); // Free the array of Queue structs itself
        hospital->queues.queues = NULL;
    }
    if (hospital->diseases.elements)
    {
        free(hospital->diseases.elements);
        hospital->diseases.elements = NULL;
    }
    if (hospital->medications.elements)
    {
        free(hospital->medications.elements);
        hospital->medications.elements = NULL;
    }
    if (hospital->prescriptions.elements)
    {
        free(hospital->prescriptions.elements);
        hospital->prescriptions.elements = NULL;
    }
    if (hospital->treatmentHistory.elements)
    {
        free(hospital->treatmentHistory.elements);
        hospital->treatmentHistory.elements = NULL;
    }

    // Free the registeredUsernames set // Removed
    // freeStringSet(&hospital->registeredUsernames);

    printSuccess("Memori rumah sakit berhasil dibebaskan!");
}

boolean deletePatient(Hospital *hospital, int patientId)
{
    // Menghapus dari PatientList
    int patientIndex = -1;
    char patientUsername[50] = "";
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (hospital->patients.elements[i].id == patientId)
        {
            patientIndex = i;
            strcpy(patientUsername, hospital->patients.elements[i].username);
            break;
        }
    }
    if (patientIndex == -1)
    {
        printError("Pasien tidak ditemukan!");
        return false;
    }

    // Free medication arrays for the patient being deleted
    if (hospital->patients.elements[patientIndex].medicationsPrescribed.medicationId != NULL) {
        free(hospital->patients.elements[patientIndex].medicationsPrescribed.medicationId);
        hospital->patients.elements[patientIndex].medicationsPrescribed.medicationId = NULL;
    }
    if (hospital->patients.elements[patientIndex].medicationsTaken.medicationId != NULL) {
        free(hospital->patients.elements[patientIndex].medicationsTaken.medicationId);
        hospital->patients.elements[patientIndex].medicationsTaken.medicationId = NULL;
    }

    // Shift elements in PatientList
    for (int i = patientIndex; i < hospital->patients.nEff - 1; i++)
    {
        hospital->patients.elements[i] = hospital->patients.elements[i + 1];
    }
    hospital->patients.nEff--;

    // Menghapus dari UserList
    int userIndex = -1;
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (hospital->users.elements[i].id == patientId)
        {
            userIndex = i;
            break;
        }
    }
    if (userIndex != -1)
    {
        for (int i = userIndex; i < hospital->users.nEff - 1; i++)
        {
            hospital->users.elements[i] = hospital->users.elements[i + 1];
        }
        hospital->users.nEff--;
    }

    // Menghapus dari antrian (using linked-list logic)
    // This logic is similar to parts of cancelPatientFromQueue
    for (int i = 0; i < hospital->queues.capacity; i++) // Iterate all possible queue slots
    {
        Queue *q = &hospital->queues.queues[i];
        if (q->roomCode[0] == '\0' || isQueueEmpty(q)) { // Skip inactive or empty queues
            continue;
        }

        boolean removed = false;
        QueueNode *current = q->front;
        QueueNode *prev = NULL;

        while (current != NULL) {
            if (current->info.patientId == patientId) {
                if (prev == NULL) { // Patient is at the front
                    q->front = current->next;
                } else {
                    prev->next = current->next;
                }
                if (current == q->rear) { // Patient was at the rear
                    q->rear = prev;
                }
                free(current);
                q->size--;
                removed = true;
                break; 
            }
            prev = current;
            current = current->next;
        }

        if (q->front == NULL) { // If list became empty
            q->rear = NULL;
        }
        
        if (removed) {
            // Patient found and removed from this queue.
            // No need to update all patient positions here, as deletePatient
            // is about removing the patient entirely from the system.
            // Their own queueRoom and queuePosition will be implicitly invalid
            // as the patient record itself is removed.
            // If we needed to update other patients' positions in this queue,
            // we would call updatePatientPositionsInQueue(hospital, q);
            break; // Assuming patient can only be in one queue.
        }
    }

    // Menghapus dari ruangan
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            Room *room = &hospital->layout.elements[i][j];
            for (int k = 0; k < room->patientInRoom.nEff; k++)
            {
                if (room->patientInRoom.patientId[k] == patientId)
                {
                    for (int l = k; l < room->patientInRoom.nEff - 1; l++)
                    {
                        room->patientInRoom.patientId[l] = room->patientInRoom.patientId[l + 1];
                    }
                    room->patientInRoom.nEff--;
                    break;
                }
            }
        }
    }

    // Menghapus dari TreatmentHistory
    int historyIndex = -1;
    for (int i = 0; i < hospital->treatmentHistory.nEff; i++)
    {
        if (hospital->treatmentHistory.elements[i].patientId == patientId)
        {
            historyIndex = i;
            break;
        }
    }
    if (historyIndex != -1)
    {
        for (int i = historyIndex; i < hospital->treatmentHistory.nEff - 1; i++)
        {
            hospital->treatmentHistory.elements[i] = hospital->treatmentHistory.elements[i + 1];
        }
        hospital->treatmentHistory.nEff--;
    }

    char successMessage[100] = "Pasien ";
    strcat(successMessage, patientUsername);
    strcat(successMessage, " berhasil dihapus dari sistem!");
    printSuccess(successMessage);
    return true;
}