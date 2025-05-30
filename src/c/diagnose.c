#include "diagnose.h"
#include "myQueue.h" // Added for new queue functions

boolean diagnosePatient(Hospital *hospital, Session *session, const char *patientUsername)
{
    if (hospital == NULL || session == NULL || patientUsername == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau username tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != DOCTOR)
    {
        printError("Akses ditolak! Hanya Dokter yang dapat mendiagnosis.");
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
    if (patient->diagnosedStatus)
    {
        printError("Pasien sudah didiagnosa sebelumnya!");
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
    if (hospital->queues.nRooms > 0) {
        for (int i = 0; i < hospital->queues.nRooms; i++) {
            if (strcmp(hospital->queues.queues[i].roomCode, doctor->room) == 0) {
                roomQueue = &hospital->queues.queues[i];
                break;
            }
        }
    }

    if (roomQueue == NULL || isQueueEmpty(roomQueue)) {
        printError("Antrian untuk ruangan dokter ini kosong atau tidak ditemukan.");
        return false;
    }

    int firstPatientID = -1;
    if (!peekQueue(roomQueue, &firstPatientID)) {
        // This case should ideally be covered by isQueueEmpty, but as a safeguard:
        printError("Tidak dapat melihat pasien di depan antrian.");
        return false;
    }

    if (firstPatientID != patient->id) {
        printError("Pasien ini tidak berada di depan antrian untuk ruangan dokter ini.");
        // Optionally, find the username of the patient who IS at the front for a more informative message.
        // char frontPatientUsername[50] = "Unknown";
        // for(int i=0; i < hospital->patients.nEff; ++i){
        //    if(hospital->patients.elements[i].id == firstPatientID){
        //        strcpy(frontPatientUsername, hospital->patients.elements[i].username);
        //        break;
        //    }
        // }
        // char errMsg[150];
        // sprintf(errMsg, "Pasien %s berada di depan antrian.", frontPatientUsername);
        // printError(errMsg);
        return false;
    }

    // Diagnosis berdasarkan data kesehatan
    char diseaseStr[50] = "Tidak terdeteksi";
    if (hospital->diseases.nEff > 0)
    {
        for (int i = 0; i < hospital->diseases.nEff; i++)
        {
            Disease *d = &hospital->diseases.elements[i];
            if (patient->bodyTemperature >= d->bodyTemperatureMin && patient->bodyTemperature <= d->bodyTemperatureMax &&
                patient->systolicBloodPressure >= d->systolicBloodPressureMin && patient->systolicBloodPressure <= d->systolicBloodPressureMax &&
                patient->diastolicBloodPressure >= d->diastolicBloodPressureMin && patient->diastolicBloodPressure <= d->diastolicBloodPressureMax &&
                patient->heartRate >= d->heartRateMin && patient->heartRate <= d->heartRateMax &&
                patient->oxygenSaturation >= d->oxygenSaturationMin && patient->oxygenSaturation <= d->oxygenSaturationMax && // <-- New check
                patient->bloodSugarLevel >= d->bloodSugarLevelMin && patient->bloodSugarLevel <= d->bloodSugarLevelMax &&
                patient->weight >= d->weightMin && patient->weight <= d->weightMax &&
                patient->height >= d->heightMin && patient->height <= d->heightMax &&
                patient->cholesterolLevel >= d->cholesterolLevelMin && patient->cholesterolLevel <= d->cholesterolLevelMax &&
                patient->platelets >= d->plateletsMin && patient->platelets <= d->plateletsMax)
            {
                strcpy(diseaseStr, d->name);
                break;
            }
        }
    }

    // Simpan hasil diagnosis
    strcpy(patient->disease, diseaseStr);
    patient->diagnosedStatus = true;

    // Tampilkan hasil
    printHeader("Hasil Diagnosis");
    int widths[] = {15, 20};
    const char *headers[] = {"Pasien", "Penyakit"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    const char *row[] = {patient->username, diseaseStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    // Pesan sukses
    char successMsg[100] = "Diagnosis untuk ";
    strcat(successMsg, patientUsername);
    strcat(successMsg, " berhasil!");
    printSuccess(successMsg);
    return true;
}