#include "registerCheckup.h"

boolean registerCheckup(Hospital *hospital, Session *session, float healthData[])
{

    if (hospital == NULL || session == NULL || healthData == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau data kesehatan tidak valid!");
        return false;
    }

    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mendaftar checkup.");
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

    int availableDoctorCount = 0;

    Doctor *availableDoctors[hospital->doctors.nEff];

    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        Doctor *doctor = &hospital->doctors.elements[i];

        if (doctor->room[0] != '\0')
        {

            availableDoctors[availableDoctorCount] = doctor;
            availableDoctorCount++;
        }
    }

    if (availableDoctorCount == 0)
    {
        printError("Tidak ada dokter yang tersedia saat ini.");
        return false;
    }

    printf("\nDaftar Dokter yang Tersedia:\n");

    int widths[] = {5, 20, 15, 10, 10, 10, 15};
    const char *headers[] = {"No", "Dokter", "Spesialisasi", "Ruangan", "Antrian", "Aura", "Biaya Checkup"};

    printTableBorder(widths, 7, 1);
    printTableRow(headers, widths, 7);
    printTableBorder(widths, 7, 2);

    for (int i = 0; i < availableDoctorCount; i++)
    {
        Doctor *doctor = availableDoctors[i];
        int queueCount = 0;

        for (int j = 0; j < hospital->queues.nRooms; j++)
        {

            if (strcmp(hospital->queues.queues[j].roomCode, doctor->room) == 0)
            {

                queueCount = queueSize(&hospital->queues.queues[j]);
                break;
            }
        }

        char numberStr[10], queueCountStr[10], auraStr[10], checkupCostStr[20];

        integerToString(i + 1, numberStr, sizeof(numberStr));
        strcat(numberStr, ". ");

        integerToString(queueCount, queueCountStr, sizeof(queueCountStr));

        integerToString(doctor->aura, auraStr, sizeof(auraStr));

        floatToString(doctor->checkupCost, checkupCostStr, sizeof(checkupCostStr), 2);

        const char *row[] = {
            numberStr,
            doctor->username,
            doctor->specialization,
            doctor->room,
            queueCountStr,
            auraStr,
            checkupCostStr};
        printTableRow(row, widths, 7);
    }

    printTableBorder(widths, 7, 3);

    int doctorChoice = 0;

    while (doctorChoice < 1 || doctorChoice > availableDoctorCount)
    {

        char promptSelectDoctor[100] = "";
        strcat(promptSelectDoctor, "Pilih dokter (1 - ");
        char availableCountStr[10];
        integerToString(availableDoctorCount, availableCountStr, sizeof(availableCountStr));
        strcat(promptSelectDoctor, availableCountStr);
        strcat(promptSelectDoctor, "): ");

        if (!readValidInt(&doctorChoice, promptSelectDoctor))
        {
            printError("Pilihan tidak valid!, harap masukkan angka yang sesuai.");

            return false;
        }
    }

    Doctor *doctor = availableDoctors[doctorChoice - 1];

    if (patient->bananaRich < doctor->checkupCost)
    {
        printError("Saldo BananaRich tidak cukup untuk checkup!");
        return false;
    }

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

        initializeQueue(newQueue, doctor->room);
    }

    Queue *queueToCheck = &hospital->queues.queues[queueIdx];

    Room *selectedRoom = NULL;

    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            Room *room = &hospital->layout.elements[i][j];
            if (strcmp(room->code, doctor->room) == 0)
            {
                selectedRoom = room;
                break;
            }
        }
        if (selectedRoom != NULL)
            break;
    }

    patient->bodyTemperature = healthData[0];
    patient->systolicBloodPressure = (int)healthData[1];
    patient->diastolicBloodPressure = (int)healthData[2];
    patient->heartRate = (int)healthData[3];
    patient->oxygenSaturation = (int)healthData[4];
    patient->bloodSugarLevel = healthData[5];
    patient->weight = healthData[6];
    patient->height = healthData[7];
    patient->cholesterolLevel = healthData[8];
    patient->platelets = (int)healthData[9];

    patient->bananaRich -= doctor->checkupCost;

    hospital->finance.hospitalBalance += 0.2 * doctor->checkupCost;

    doctor->bananaRich += 0.8 * doctor->checkupCost;

    if (hospital->treatmentHistory.nEff >= hospital->treatmentHistory.capacity)
    {

        TreatmentHistory *newElements = (TreatmentHistory *)realloc(
            hospital->treatmentHistory.elements,
            sizeof(TreatmentHistory) * hospital->treatmentHistory.capacity * 2);

        if (!newElements)
        {
            printError("Gagal mengalokasikan ulang memori untuk riwayat pengobatan!");

            patient->bananaRich += doctor->checkupCost;
            hospital->finance.hospitalBalance -= 0.2 * doctor->checkupCost;
            doctor->bananaRich -= 0.8 * doctor->checkupCost;

            return false;
        }

        hospital->treatmentHistory.capacity *= 2;
        hospital->treatmentHistory.elements = newElements;
    }

    TreatmentHistory *history = &hospital->treatmentHistory.elements[hospital->treatmentHistory.nEff++];
    history->patientId = patient->id;
    history->doctorId = doctor->id;
    strcpy(history->room, doctor->room);
    strcpy(history->disease, "Belum Didiagnosis");
    history->treatmentStatus = false;

    if (selectedRoom != NULL && selectedRoom->patientInRoom.nEff < selectedRoom->capacity)
    {

        strcpy(patient->queueRoom, doctor->room);
        patient->queuePosition = 0;

        selectedRoom->patientInRoom.patientId[selectedRoom->patientInRoom.nEff++] = patient->id;

        printf("\n");
        printHeader("Pendaftaran Checkup");

        int widths2[] = {20, 20};
        const char *headers2[] = {"Dokter", "Ruangan"};
        printTableBorder(widths2, 2, 1);
        printTableRow(headers2, widths2, 2);
        printTableBorder(widths2, 2, 2);
        const char *row2[] = {doctor->username, doctor->room};
        printTableRow(row2, widths2, 2);
        printTableBorder(widths2, 2, 3);

        printf(FORMAT_BOLD COLOR_YELLOW "[ℹ️  | Info]: Anda telah dipindahkan langsung ke ruangan dokter!\n" FORMAT_RESET);

        char successMsg[100] = "";
        strcat(successMsg, "Pendaftaran checkup untuk ");
        strcat(successMsg, session->username);
        strcat(successMsg, " pada dr.");
        strcat(successMsg, doctor->username);
        strcat(successMsg, " di ruangan ");
        strcat(successMsg, doctor->room);
        strcat(successMsg, " berhasil!");
        printSuccess(successMsg);

        return true;
    }

    if (!enqueue(queueToCheck, patient->id))
    {
        printError("Gagal menambahkan pasien ke antrian!");

        patient->bananaRich += doctor->checkupCost;
        hospital->finance.hospitalBalance -= 0.2 * doctor->checkupCost;
        doctor->bananaRich -= 0.8 * doctor->checkupCost;

        hospital->treatmentHistory.nEff--;
        return false;
    }

    strcpy(patient->queueRoom, doctor->room);
    patient->queuePosition = queueSize(queueToCheck);

    printf("\n");
    printHeader("Pendaftaran Checkup");

    int widths2[] = {20, 20};
    const char *headers2[] = {"Dokter", "Ruangan"};
    printTableBorder(widths2, 2, 1);
    printTableRow(headers2, widths2, 2);
    printTableBorder(widths2, 2, 2);
    const char *row2[] = {doctor->username, doctor->room};
    printTableRow(row2, widths2, 2);
    printTableBorder(widths2, 2, 3);

    printf(FORMAT_BOLD COLOR_YELLOW "[ℹ️  | Info]: Posisi antrian: %d\n" FORMAT_RESET,
           patient->queuePosition);

    char successMsg[100] = "";
    strcat(successMsg, "Pendaftaran checkup untuk ");
    strcat(successMsg, session->username);
    strcat(successMsg, " pada dr.");
    strcat(successMsg, doctor->username);
    strcat(successMsg, " di ruangan ");
    strcat(successMsg, doctor->room);
    strcat(successMsg, " berhasil!");
    printSuccess(successMsg);

    return true;
}