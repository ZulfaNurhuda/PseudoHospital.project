#include "takeAntidote.h"

boolean takeAntidote(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }

    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mengonsumsi penawar.");
        return false;
    }

    // Find patient
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

    if (patient->medicationsTaken.top < 0)
    {
        printError("Anda belum mengonsumsi obat apapun!");
        return false;
    }

    // Menampilkan obat yang terakhir diminum
    int lastMedicationId = patient->medicationsTaken.medicationId[patient->medicationsTaken.top];
    char medicationName[50] = "Tidak dikenal";
    for (int i = 0; i < hospital->medications.nEff; i++)
    {
        if (hospital->medications.elements[i].id == lastMedicationId)
        {
            strcpy(medicationName, hospital->medications.elements[i].name);
            break;
        }
    }

    // Hapus obat terakhir dari stack dan kembalikan ke inventory
    patient->medicationsPrescribed.medicationId[patient->medicationsPrescribed.nEff++]
    patient->medicationsTaken.top--;

    // Tambahkan nyawa pasien
    if (patient->life < 3)
    {
        patient->life++;
    }

    printSuccess("Penawar berhasil diminum! Obat %s dibatalkan, nyawa tersisa: %d", medicationName, patient->life);
    return true;
}
