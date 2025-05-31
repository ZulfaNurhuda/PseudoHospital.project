#include "treat.h"

boolean treatPatient(Hospital *hospital, Session *session, const char *patientUsername)
{
    if (hospital == NULL || session == NULL || patientUsername == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau username tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != DOCTOR)
    {
        printError("Akses ditolak! Hanya Dokter yang dapat meresepkan obat.");
        return false;
    }

    if (!isValidUsername(patientUsername))
    {
        printError("Username pasien tidak valid!");
        return false;
    }

    if (hospital->patients.nEff == 0)
    {
        printError("Tidak ada pasien terdaftar!");
        return false;
    }
    int patientIdx = -1;
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (strcmp(hospital->patients.elements[i].username, patientUsername) == 0)
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
    if (!patient->diagnosedStatus)
    {
        printError("Pasien belum didiagnosa!");
        return false;
    }
    if (patient->treatedStatus)
    {
        printError("Pasien sudah diberikan resep obat!");
        return false;
    }
    if (strcmp(patient->disease, "Tidak terdeteksi") == 0 || patient->disease[0] == '\0')
    {
        printError("Penyakit pasien tidak valid untuk peresepan!");
        return false;
    }

    if (hospital->doctors.nEff == 0)
    {
        printError("Tidak ada dokter terdaftar!");
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
        printError("Dokter tidak ditemukan!");
        return false;
    }

    Doctor *doctor = &hospital->doctors.elements[doctorIdx];
    if (doctor->room[0] == '\0')
    {
        printError("Dokter tidak ditugaskan ke ruangan manapun!");
        return false;
    }

    // Memeriksa apakah pasien mengantri di ruangan dokter
    if (strcmp(patient->queueRoom, doctor->room) != 0)
    {
        printError("Pasien tidak berada di antrian ruangan dokter!");
        return false;
    }

    // Memeriksa antrian di HospitalQueueList
    // Patient must be at the front of the queue for the doctor's room.
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

    // Mencari diseaseId berdasarkan patient->disease
    int diseaseId = -1;
    if (hospital->diseases.nEff > 0)
    {
        for (int i = 0; i < hospital->diseases.nEff; i++)
        {
            if (strcmp(hospital->diseases.elements[i].name, patient->disease) == 0)
            {
                diseaseId = hospital->diseases.elements[i].id;
                break;
            }
        }
    }
    if (diseaseId == -1)
    {
        printError("Penyakit pasien tidak ditemukan dalam database!");
        return false;
    }

    // Meresepkan obat berdasarkan diseaseId, diurutkan berdasarkan doseOrder
    if (hospital->prescriptions.nEff > 0)
    {
        // Mengumpulkan semua resep untuk penyakit
        struct
        {
            int medicationId;
            int doseOrder;
        } prescriptions[100];
        int prescriptionCount = 0;
        for (int i = 0; i < hospital->prescriptions.nEff; i++)
        {
            if (hospital->prescriptions.elements[i].diseaseId == diseaseId)
            {
                prescriptions[prescriptionCount].medicationId = hospital->prescriptions.elements[i].medicationId;
                prescriptions[prescriptionCount].doseOrder = hospital->prescriptions.elements[i].doseOrder;
                prescriptionCount++;
            }
        }

        // Mengurutkan resep berdasarkan doseOrder
        for (int i = 0; i < prescriptionCount - 1; i++)
        {
            for (int j = 0; j < prescriptionCount - i - 1; j++)
            {
                if (prescriptions[j].doseOrder > prescriptions[j + 1].doseOrder)
                {
                    int tempId = prescriptions[j].medicationId;
                    int tempOrder = prescriptions[j].doseOrder;
                    prescriptions[j].medicationId = prescriptions[j + 1].medicationId;
                    prescriptions[j].doseOrder = prescriptions[j + 1].doseOrder;
                    prescriptions[j + 1].medicationId = tempId;
                    prescriptions[j + 1].doseOrder = tempOrder;
                }
            }
        }

        // Menambahkan obat ke medicationsPrescribed
        for (int i = 0; i < prescriptionCount; i++)
        {
            if (patient->medicationsPrescribed.nEff < patient->medicationsPrescribed.capacity)
            {
                patient->medicationsPrescribed.medicationId[patient->medicationsPrescribed.nEff++] =
                    prescriptions[i].medicationId;
            }
            else
            {
                printError("Kapasitas daftar resep pasien penuh!");
                return false;
            }
        }
    }

    patient->treatedStatus = true;

    // Dequeue patient after treatment
    int dequeuedPatientId = -1;
    if (dequeue(roomQueue, &dequeuedPatientId))
    {
        if (dequeuedPatientId != patient->id)
        {
            printError("Pasien yang di-dequeue berbeda dari pasien yang diobati!");
        }
        patient->queueRoom[0] = '\0';
        patient->queuePosition = 0;
    }
    else
    {
        printError("Gagal men-dequeue pasien setelah pengobatan!");
    }

    // Tampilkan hasil
    printHeader("Resep Obat");
    if (patient->medicationsPrescribed.nEff == 0)
    {
        printError("Tidak ada obat yang diresepkan.");
    }
    else
    {
        char msg[100] = "Obat yang diresepkan (urutan konsumsi):";
        printSuccess(msg);
        for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
        {
            for (int j = 0; j < hospital->medications.nEff; j++)
            {
                if (hospital->medications.elements[j].id == patient->medicationsPrescribed.medicationId[i])
                {
                    char medMsg[100] = "";
                    strcat(medMsg, "  ");
                    char numStr[10];
                    int num = i + 1;
                    int k = 0;
                    if (num == 0)
                        numStr[k++] = '0';
                    else
                        while (num > 0)
                        {
                            numStr[k++] = (num % 10) + '0';
                            num /= 10;
                        }
                    for (int m = k - 1; m >= 0; m--)
                        medMsg[2 + k - 1 - m] = numStr[m];
                    medMsg[2 + k] = '.';
                    medMsg[3 + k] = ' ';
                    strcat(medMsg, hospital->medications.elements[j].name);
                    printSuccess(medMsg);
                    break;
                }
            }
        }
    }

    // Pesan sukses
    char successMsg[100] = "Resep obat untuk ";
    strcat(successMsg, patientUsername);
    strcat(successMsg, " berhasil diberikan!");
    printSuccess(successMsg);
    return true;
}