#include "hospital.h"

void initHospital(Hospital *hospital, int userCapacity, int patientCapacity, int doctorCapacity, int roomRows, int roomCols)
{
    if (userCapacity <= 0 || patientCapacity <= 0 || doctorCapacity <= 0 || roomRows <= 0 || roomCols <= 0)
    {
        printError("Kapasitas atau ukuran denah tidak valid!");
        return;
    }

    // Inisialisasi UserList
    hospital->users.elements = (User *)safeMalloc(userCapacity * sizeof(User));
    hospital->users.capacity = userCapacity;
    hospital->users.nEff = 0;

    // Inisialisasi PatientList
    hospital->patients.elements = (Patient *)safeMalloc(patientCapacity * sizeof(Patient));
    hospital->patients.capacity = patientCapacity;
    hospital->patients.nEff = 0;

    // Inisialisasi DoctorList
    hospital->doctors.elements = (Doctor *)safeMalloc(doctorCapacity * sizeof(Doctor));
    hospital->doctors.capacity = doctorCapacity;
    hospital->doctors.nEff = 0;

    // Inisialisasi Layout
    hospital->layout.elements = (Room **)safeMalloc(roomRows * sizeof(Room *));
    for (int i = 0; i < roomRows; i++)
    {
        hospital->layout.elements[i] = (Room *)safeMalloc(roomCols * sizeof(Room));
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
            for (int k = index - 1; k >= 0; k--)
                strncat(code, &colStr[k], 1);
            strcpy(hospital->layout.elements[i][j].code, code);
            hospital->layout.elements[i][j].capacity = 3;
            hospital->layout.elements[i][j].doctorID = -1;
            hospital->layout.elements[i][j].patientInRoom.patientID = (int *)safeMalloc(3 * sizeof(int));
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
    hospital->queues.capacity = roomRows * roomCols;
    hospital->queues.nRooms = 0;
    for (int i = 0; i < roomRows * roomCols; i++)
    {
        hospital->queues.queues[i].buffer = (QueueInfo *)safeMalloc(10 * sizeof(QueueInfo));
        hospital->queues.queues[i].capacity = 10;
        hospital->queues.queues[i].idxHead = -1;
        hospital->queues.queues[i].idxTail = -1;
        hospital->queues.queues[i].roomCode[0] = '\0';
    }

    // Inisialisasi DiseaseList
    hospital->diseases.elements = (Disease *)safeMalloc(5 * sizeof(Disease));
    hospital->diseases.capacity = 5;
    hospital->diseases.nEff = 5;
    Disease diseases[] = {
        {1, "Influenza", 36.0, 38.5, 90, 120, 60, 80, 60, 100, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {2, "COVID-19", 36.0, 39.0, 90, 130, 60, 85, 60, 120, 70.0, 180.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {3, "Hipertensi", 36.0, 37.5, 140, 180, 90, 120, 60, 100, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {4, "Diabetes Mellitus", 36.0, 37.5, 90, 140, 60, 90, 60, 100, 126.0, 200.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450},
        {5, "Anemia", 36.0, 37.0, 90, 120, 60, 80, 60, 100, 70.0, 140.0, 45.0, 90.0, 150, 185, 150, 240, 150, 450}};
    for (int i = 0; i < 5; i++)
    {
        hospital->diseases.elements[i] = diseases[i];
    }

    // Inisialisasi MedicationList
    hospital->medications.elements = (Medication *)safeMalloc(5 * sizeof(Medication));
    hospital->medications.capacity = 5;
    hospital->medications.nEff = 5;
    Medication medications[] = {
        {1, "Paracetamol"}, {2, "Antibiotik"}, {3, "Antiviral"}, {4, "Amlodipine"}, {5, "Metformin"}};
    for (int i = 0; i < 5; i++)
    {
        hospital->medications.elements[i] = medications[i];
    }

    // Inisialisasi PrescriptionList
    hospital->prescriptions.elements = (MedicationPrescription *)safeMalloc(5 * sizeof(MedicationPrescription));
    hospital->prescriptions.capacity = 5;
    hospital->prescriptions.nEff = 5;
    MedicationPrescription prescriptions[] = {
        {1, 1, 1}, {2, 1, 2}, {3, 2, 1}, {4, 3, 1}, {5, 4, 1}};
    for (int i = 0; i < 5; i++)
    {
        hospital->prescriptions.elements[i] = prescriptions[i];
    }

    // Inisialisasi HospitalFinance
    hospital->finance.hospitalBalance = 10000.0;

    // Inisialisasi TreatmentHistoryList
    hospital->treatmentHistory.elements = (TreatmentHistory *)safeMalloc(100 * sizeof(TreatmentHistory));
    hospital->treatmentHistory.capacity = 100;
    hospital->treatmentHistory.nEff = 0;

    printSuccess("Rumah sakit berhasil diinisialisasi!");
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
                    if (hospital->layout.elements[i][j].patientInRoom.patientID)
                    {
                        free(hospital->layout.elements[i][j].patientInRoom.patientID);
                        hospital->layout.elements[i][j].patientInRoom.patientID = NULL;
                    }
                }
                free(hospital->layout.elements[i]);
                hospital->layout.elements[i] = NULL;
            }
        }
        free(hospital->layout.elements);
        hospital->layout.elements = NULL;
    }
    if (hospital->queues.queues)
    {
        for (int i = 0; i < hospital->queues.capacity; i++)
        {
            if (hospital->queues.queues[i].buffer)
            {
                free(hospital->queues.queues[i].buffer);
                hospital->queues.queues[i].buffer = NULL;
            }
        }
        free(hospital->queues.queues);
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

    // Menghapus dari antrian
    for (int i = 0; i < hospital->queues.nRooms; i++)
    {
        Queue *queue = &hospital->queues.queues[i];
        for (int j = queue->idxHead; j <= queue->idxTail; j++)
        {
            if (queue->buffer[j].patientID == patientId)
            {
                for (int k = j; k < queue->idxTail; k++)
                {
                    queue->buffer[k] = queue->buffer[k + 1];
                }
                queue->idxTail--;
                if (queue->idxTail < queue->idxHead)
                {
                    queue->idxHead = -1;
                    queue->idxTail = -1;
                }
                break;
            }
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
                if (room->patientInRoom.patientID[k] == patientId)
                {
                    for (int l = k; l < room->patientInRoom.nEff - 1; l++)
                    {
                        room->patientInRoom.patientID[l] = room->patientInRoom.patientID[l + 1];
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
        if (hospital->treatmentHistory.elements[i].patientID == patientId)
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