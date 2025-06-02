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

    // Check if patient has taken any medication
    if (patient->medicationsTaken.top < 0)
    {
        printError("Anda belum mengonsumsi obat apapun!");
        return false;
    }

    // Check if patient needs antidote by finding the last wrongly taken medication
    boolean isNeedAntidote = false;
    int wrongMedicationId = -1;

    // We need to check if the last medication taken was wrong
    // by comparing the sequence of taken medications with prescribed ones
    int lastMedicationId = patient->medicationsTaken.medicationId[patient->medicationsTaken.top];

    // Find what should have been the next medication in order
    int expectedMedicationId = -1;
    int takenCount = patient->medicationsTaken.top + 1; // +1 because top is 0-indexed

    if (takenCount <= patient->medicationsPrescribed.nEff)
    {
        expectedMedicationId = patient->medicationsPrescribed.medicationId[takenCount - 1];
    }

    // Check if the last taken medication was wrong
    if (expectedMedicationId != -1 && lastMedicationId != expectedMedicationId)
    {
        isNeedAntidote = true;
        wrongMedicationId = lastMedicationId;
    }
    else
    {
        // Alternative check: look for any medication taken out of order
        // This handles cases where multiple wrong medications were taken
        for (int i = 0; i <= patient->medicationsTaken.top; i++)
        {
            if (i < patient->medicationsPrescribed.nEff)
            {
                if (patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i])
                {
                    // Found a wrong medication, but we can only undo the last one
                    if (i == patient->medicationsTaken.top)
                    {
                        isNeedAntidote = true;
                        wrongMedicationId = patient->medicationsTaken.medicationId[i];
                        break;
                    }
                }
            }
        }
    }

    if (isNeedAntidote && wrongMedicationId != -1)
    {
        // Find the name of the wrong medication
        char medicationName[50] = "Tidak dikenal";
        for (int i = 0; i < hospital->medications.nEff; i++)
        {
            if (hospital->medications.elements[i].id == wrongMedicationId)
            {
                strcpy(medicationName, hospital->medications.elements[i].name);
                break;
            }
        }

        // Remove the last (wrong) medication from taken list
        patient->medicationsTaken.top--;

        // Display antidote consumption result
        printHeader("Konsumsi Penawar");
        int widths[] = {20, 20, 20};
        const char *headers[] = {"Obat Dibatalkan", "Nyawa Tersisa", "Status"};
        printTableBorder(widths, 3, 1);
        printTableRow(headers, widths, 3);
        printTableBorder(widths, 3, 2);

        // Build life display string
        char lifeStr[15] = "";
        for (int i = 0; i < 3; i++)
        {
            if (i < patient->life)
            {
                strcat(lifeStr, "● ");
            }
            else
            {
                strcat(lifeStr, "○ ");
            }
        }

        const char *row[] = {medicationName, lifeStr, "Penawar berhasil"};
        printTableRow(row, widths, 3);
        printTableBorder(widths, 3, 3);

        // Create and display success message
        char successMsg[100] = "Penawar berhasil! Obat ";
        strcat(successMsg, medicationName);
        strcat(successMsg, " dibatalkan.");
        printSuccess(successMsg);

        return true;
    }
    else
    {
        printError("Anda tidak perlu mengonsumsi penawar! Urutan obat yang diminum sudah benar.");
        return false; // Changed from true to false since no antidote was needed
    }
}