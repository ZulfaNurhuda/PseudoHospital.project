#include "diagnose.h"

boolean diagnosePatient(Hospital *hospital, Session *session)
{

    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }

    if (!session->isLoggedIn || session->role != DOCTOR)
    {
        printError("Akses ditolak! Hanya Dokter yang dapat mendiagnosis.");
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

        printError("Ruangan dokter ini tidak memiliki pasien yang mengantri.");
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

    if (patient->diagnosedStatus)
    {
        printError("Pasien sudah didiagnosa sebelumnya!");
        return false;
    }

    if (strcmp(patient->queueRoom, doctor->room) != 0)
    {
        printError("Pasien tidak berada di antrian ruangan dokter!");
        return false;
    }

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
                patient->oxygenSaturation >= d->oxygenSaturationMin && patient->oxygenSaturation <= d->oxygenSaturationMax &&
                patient->bloodSugarLevel >= d->bloodSugarLevelMin && patient->bloodSugarLevel <= d->bloodSugarLevelMax &&
                patient->weight >= d->weightMin && patient->weight <= d->weightMax &&
                patient->height >= d->heightMin && patient->height <= d->heightMax &&
                patient->cholesterolLevel >= d->cholesterolLevelMin && patient->cholesterolLevel <= d->cholesterolLevelMax &&
                patient->platelets >= d->plateletsMin && patient->platelets <= d->plateletsMax)
            {
                strcpy(diseaseStr, d->name);
                patient->disease[0] = '\0';
                strcat(patient->disease, diseaseStr);
                patient->diagnosedStatus = true;
                break;
            }
        }
    }

    if (strcmp(diseaseStr, "Tidak terdeteksi") == 0)
    {
        patient->diagnosedStatus = true;
        strcpy(patient->disease, "Tidak terdeteksi");
        printSuccess("Pasien tidak terdiagnosis penyakit apapun!");
    }
    else
    {
        printf("%s terdiagnosa penyakit %s%s%s!\n", patient->username, COLOR_YELLOW, diseaseStr, COLOR_RESET);
    }

    return true;
}