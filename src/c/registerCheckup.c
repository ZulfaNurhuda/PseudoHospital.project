#include "registerCheckup.h"

boolean registerCheckup(Hospital *hospital, Session *session, const char *doctorUsername, float healthData[])
{
    if (hospital == NULL || session == NULL || doctorUsername == NULL || healthData == NULL)
    {
        printError("Struktur rumah sakit, sesi, username dokter, atau data kesehatan tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mendaftar checkup.");
        return false;
    }

    if (!isValidUsername(doctorUsername))
    {
        printError("Username dokter tidak valid!");
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
        if (strcmp(hospital->patients.elements[i].username, session->username) == 0)
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
    if (patient->queueRoom[0] != '\0')
    {
        printError("Anda sudah terdaftar dalam antrian!");
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
        if (strcmp(hospital->doctors.elements[i].username, doctorUsername) == 0)
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

    // Validasi data kesehatan
    const float ranges[11][2] = {
        {30.0, 45.0},   // bodyTemperature
        {50.0, 200.0},  // systolicBloodPressure
        {30.0, 120.0},  // diastolicBloodPressure
        {40.0, 200.0},  // heartRate
        {50.0, 100.0},  // oxygenSaturation
        {50.0, 300.0},  // bloodSugarLevel
        {20.0, 200.0},  // weight
        {100.0, 250.0}, // height
        {100.0, 400.0}, // cholesterolLevel
        {50.0, 200.0},  // ldlCholesterolLevel
        {100.0, 1000.0} // platelets
    };
    for (int i = 0; i < 11; i++)
    {
        if (healthData[i] < ranges[i][0] || healthData[i] > ranges[i][1])
        {
            printError("Data kesehatan tidak valid!");
            return false;
        }
    }

    // Validasi biaya checkup (misalnya, 100 BananaRich)
    const float checkupCost = 100.0;
    if (patient->bananaRich < checkupCost)
    {
        printError("Saldo BananaRich tidak cukup untuk checkup!");
        return false;
    }

    // Menambahkan ke antrian
    int queueIdx = -1;
    for (int i = 0; i < hospital->queues.nRooms; i++)
    {
        if (strcmp(hospital->queues.queues[i].roomCode, doctor->room) == 0)
        {
            queueIdx = i;
            break;
        }
    }
    if (queueIdx == -1)
    {
        if (hospital->queues.nRooms >= hospital->queues.capacity)
        {
            printError("Kapasitas antrian penuh!");
            return false;
        }
        queueIdx = hospital->queues.nRooms++;
        Queue *newQueue = &hospital->queues.queues[queueIdx];
        strcpy(newQueue->roomCode, doctor->room);
        newQueue->idxHead = -1;
        newQueue->idxTail = -1;
        newQueue->capacity = 100; // Asumsi kapasitas default
    }

    Queue *queue = &hospital->queues.queues[queueIdx];
    if (queue->idxTail - queue->idxHead + 1 >= queue->capacity)
    {
        printError("Antrian ruangan penuh!");
        return false;
    }

    if (queue->idxHead == -1)
    {
        queue->idxHead = 0;
        queue->idxTail = 0;
    }
    else
    {
        queue->idxTail++;
    }
    queue->buffer[queue->idxTail].patientID = patient->id;
    strcpy(patient->queueRoom, doctor->room);
    patient->queuePosition = queue->idxTail - queue->idxHead + 1;

    // Menyimpan data kesehatan
    patient->bodyTemperature = healthData[0];
    patient->systolicBloodPressure = (int)healthData[1];
    patient->diastolicBloodPressure = (int)healthData[2];
    patient->heartRate = (int)healthData[3];
    patient->oxygenSaturation = (int)healthData[4];
    patient->bloodSugarLevel = healthData[5];
    patient->weight = healthData[6];
    patient->height = healthData[7];
    patient->cholesterolLevel = healthData[8];
    patient->ldlCholesterolLevel = healthData[9];
    patient->platelets = (int)healthData[10];

    // Mengelola biaya
    patient->bananaRich -= checkupCost;
    hospital->finance.hospitalBalance += checkupCost;

    // Menambahkan ke riwayat perawatan
    if (hospital->treatmentHistory.nEff < hospital->treatmentHistory.capacity)
    {
        TreatmentHistory *history = &hospital->treatmentHistory.elements[hospital->treatmentHistory.nEff++];
        history->patientID = patient->id;
        history->doctorID = doctor->id;
        strcpy(history->room, doctor->room);
        history->examinationDate = 20250509; // Asumsi tanggal saat ini (YYYYMMDD)
        history->treatmentStatus = false;    // Belum selesai
    }
    else
    {
        printError("Kapasitas riwayat pengobatan penuh!");
        return false;
    }

    // Tampilkan hasil
    printHeader("Pendaftaran Checkup");
    int widths[] = {15, 20};
    const char *headers[] = {"Dokter", "Ruangan"};
    printTableRow(headers, widths, 4);
    printTableBorder(widths, 2, 1);
    const char *row[] = {doctor->username, doctor->room};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    char queueMsg[50] = "Posisi antrian: ";
    char numStr[10];
    int num = patient->queuePosition;
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
        queueMsg[16 + k - 1 - m] = numStr[m];
    queueMsg[16 + k] = '\0';
    printSuccess(queueMsg);

    // Pesan sukses
    char successMsg[100] = "Pendaftaran checkup untuk ";
    strcat(successMsg, session->username);
    strcat(successMsg, " berhasil!");
    printSuccess(successMsg);
    return true;
}