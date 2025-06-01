#include "treat.h"

boolean treatPatient(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != DOCTOR)
    {
        printError("Akses ditolak! Hanya Dokter yang dapat meresepkan obat.");
        return false;
    }

    int doctorIdx = -1;
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        if (strcmp(hospital->doctors.elements[i].username, session->username) == 0)
        {
            doctorIdx = i;
            break;
        }
    }
    if (doctorIdx == -1)
    {
        printError("Dokter tidak ditemukan dalam daftar!");
        return false;
    }

    Doctor *doctor = &hospital->doctors.elements[doctorIdx];

    if (doctor->room[0] == '\0')
    {
        printError("Dokter tidak ditugaskan ke ruangan manapun!");
        return false;
    }

    Room *doctorRoom = NULL;
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            if (strcmp(hospital->layout.elements[i][j].code, doctor->room) == 0)
            {
                doctorRoom = &hospital->layout.elements[i][j];
                break;
            }
        }
        if (doctorRoom != NULL)
            break;
    }

    if (doctorRoom == NULL || doctorRoom->patientInRoom.nEff == 0)
    {
        printError("Tidak ada pasien yang berada didalam ruangan dokter ini.");
        return false;
    }

    int patientId = doctorRoom->patientInRoom.patientId[0];
    int patientIdx = -1;
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (hospital->patients.elements[i].id == patientId)
        {
            patientIdx = i;
            break;
        }
    }

    if (patientIdx == -1)
    {
        printError("Pasien tidak ditemukan!");
        return false;
    }

    Patient *patient = &hospital->patients.elements[patientIdx];

    if (strcmp(patient->disease, "Tidak terdeteksi") == 0)
    {
        printSuccess("Pasien tidak menderita penyakit apapun!");
        patient->treatedStatus = true;
        return true;
    }

    if (!patient->diagnosedStatus)
    {
        printError("Pasien belom mendapat diagnosa dari dokter!");
        return false;
    }

    if (patient->treatedStatus)
    {
        printError("Pasien sudah diberikan resep obat!");
        return false;
    }

    if (strcmp(patient->queueRoom, doctor->room) != 0)
    {
        printError("Pasien tidak berada di antrian ruangan dokter!");
        return false;
    }

    Queue *roomQueue = NULL;
    if (hospital->queues.nRooms > 0)
    {
        for (int i = 0; i < hospital->queues.nRooms; i++)
        {
            if (strcmp(hospital->queues.queues[i].roomCode, doctor->room) == 0)
            {
                roomQueue = &hospital->queues.queues[i];
                break;
            }
        }
    }

    if (roomQueue == NULL || isQueueEmpty(roomQueue))
    {
        printError("Antrian untuk ruangan dokter ini kosong atau tidak ditemukan.");
        return false;
    }

    int firstPatientId = -1;
    if (!peekQueue(roomQueue, &firstPatientId))
    {
        printError("Tidak dapat melihat pasien di depan antrian.");
        return false;
    }

    if (firstPatientId != patient->id)
    {
        printError("Pasien ini tidak berada di depan antrian untuk ruangan dokter ini.");
        return false;
    }

    int diseaseId = -1;
    for (int i = 0; i < hospital->diseases.nEff; i++)
    {
        if (strcmp(hospital->diseases.elements[i].name, patient->disease) == 0)
        {
            diseaseId = hospital->diseases.elements[i].id;
            break;
        }
    }

    if (diseaseId == -1)
    {
        printError("Penyakit pasien tidak ditemukan dalam database!");
        return false;
    }

    int medicationCount = 0;
    for (int i = 0; i < hospital->prescriptions.nEff; i++)
    {
        if (hospital->prescriptions.elements[i].diseaseId == diseaseId)
        {
            medicationCount++;
        }
    }

    MedicationPrescription *tempPrescriptions = NULL;
    MedicationList prescribedMedications = {NULL, 0, 0};

    if (medicationCount > 0)
    {
        tempPrescriptions = (MedicationPrescription *)malloc(medicationCount * sizeof(MedicationPrescription));
        if (tempPrescriptions == NULL)
        {
            printError("Gagal mengalokasi memori untuk resep sementara!");
            return false;
        }

        int tempCount = 0;
        for (int i = 0; i < hospital->prescriptions.nEff; i++)
        {
            if (hospital->prescriptions.elements[i].diseaseId == diseaseId)
            {
                tempPrescriptions[tempCount] = hospital->prescriptions.elements[i];
                tempCount++;
            }
        }

        for (int i = 0; i < medicationCount - 1; i++)
        {
            for (int j = 0; j < medicationCount - i - 1; j++)
            {
                if (tempPrescriptions[j].doseOrder > tempPrescriptions[j + 1].doseOrder)
                {
                    MedicationPrescription temp = tempPrescriptions[j];
                    tempPrescriptions[j] = tempPrescriptions[j + 1];
                    tempPrescriptions[j + 1] = temp;
                }
            }
        }

        prescribedMedications.elements = malloc(medicationCount * sizeof(Medication));
        if (prescribedMedications.elements == NULL)
        {
            free(tempPrescriptions);
            printError("Gagal mengalokasi memori untuk daftar obat!");
            return false;
        }

        for (int i = 0; i < medicationCount; i++)
        {
            prescribedMedications.elements[i].id = tempPrescriptions[i].medicationId;
            prescribedMedications.nEff++;
        }

        free(tempPrescriptions);
    }

    if (prescribedMedications.nEff > 0)
    {
        printf(COLOR_GREEN "Dokter sedang mengobati pasien!\n" COLOR_RESET);
        printf("Pasien memiliki penyakit %s%s%s\n", COLOR_YELLOW, patient->disease, COLOR_RESET);
        printf("\nObat yang harus diberikan:\n");

        int widths[] = {5, 30};

        const char *header[] = {"No.", "Nama Obat"};
        printTableBorder(widths, 2, 1);
        printTableRow(header, widths, 2);
        printTableBorder(widths, 2, 2);

        for (int i = 0; i < prescribedMedications.nEff; i++)
        {
            Medication *med = NULL;
            for (int j = 0; j < hospital->medications.nEff; j++)
            {
                if (hospital->medications.elements[j].id == prescribedMedications.elements[i].id)
                {
                    med = &hospital->medications.elements[j];
                    break;
                }
            }

            if (med == NULL)
            {
                char errorMessage[100] = "";
                strcat(errorMessage, "Obat dengan ID ");
                char idStr[10];
                integerToString(prescribedMedications.elements[i].id, idStr, sizeof(idStr));
                strcat(errorMessage, idStr);
                strcat(errorMessage, " tidak ditemukan!");
                printError(errorMessage);

                free(prescribedMedications.elements);
                return false;
            }

            char no[10] = "";
            integerToString(i + 1, no, sizeof(no));
            strcat(no, ".");
            const char *row[] = {no, med->name};
            printTableRow(row, widths, 2);
        }

        printTableBorder(widths, 2, 3);

        if (patient->medicationsPrescribed.capacity < medicationCount)
        {
            int *newMedicationIds = realloc(patient->medicationsPrescribed.medicationId,
                                            medicationCount * sizeof(int));
            if (newMedicationIds == NULL)
            {
                free(prescribedMedications.elements);
                printError("Gagal mengalokasi memori untuk daftar obat pasien!");
                return false;
            }
            patient->medicationsPrescribed.medicationId = newMedicationIds;
            patient->medicationsPrescribed.capacity = medicationCount;
        }

        patient->medicationsPrescribed.nEff = 0;
        for (int i = 0; i < prescribedMedications.nEff; i++)
        {
            patient->medicationsPrescribed.medicationId[patient->medicationsPrescribed.nEff] = prescribedMedications.elements[i].id;
            patient->medicationsPrescribed.nEff++;
        }

        patient->treatedStatus = true;
        free(prescribedMedications.elements);

        printSuccess("Obat berhasil diberikan!");
        return true;
    }
    else
    {
        printError("Tidak ada obat yang ditemukan untuk penyakit ini!");
        return false;
    }
}