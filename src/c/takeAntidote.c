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

    boolean isNeedAntidote = false;
    int wrongMedicationId = -1;

    int lastMedicationId = patient->medicationsTaken.medicationId[patient->medicationsTaken.top];

    int expectedMedicationId = -1;
    int takenCount = patient->medicationsTaken.top + 1;

    if (takenCount <= patient->medicationsPrescribed.nEff)
    {
        expectedMedicationId = patient->medicationsPrescribed.medicationId[takenCount - 1];
    }

    if (expectedMedicationId != -1 && lastMedicationId != expectedMedicationId)
    {
        isNeedAntidote = true;
        wrongMedicationId = lastMedicationId;
    }
    else
    {
        for (int i = 0; i <= patient->medicationsTaken.top; i++)
        {
            if (i < patient->medicationsPrescribed.nEff)
            {
                if (patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i])
                {
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
        char medicationName[50] = "Tidak dikenal";
        for (int i = 0; i < hospital->medications.nEff; i++)
        {
            if (hospital->medications.elements[i].id == wrongMedicationId)
            {
                strcpy(medicationName, hospital->medications.elements[i].name);
                break;
            }
        }

        patient->medicationsTaken.top--;

        printHeader("Konsumsi Penawar");
        int widths[] = {20, 20, 20};
        const char *headers[] = {"Obat Dibatalkan", "Nyawa Tersisa", "Status"};
        printTableBorder(widths, 3, 1);
        printTableRow(headers, widths, 3);
        printTableBorder(widths, 3, 2);

        char lifeStr[10] = "";
        strcat(lifeStr, COLOR_YELLOW);
        for (int i = 0; i < 3; i++)
        {
            if (i < patient->life)
            {
                strcat(lifeStr, "O");
            }
            else
            {
                strcat(lifeStr, "X");
            }
        }
        strcat(lifeStr, COLOR_RESET);

        const char *row[] = {medicationName, lifeStr, "Penawar berhasil"};
        printTableRow(row, widths, 3);
        printTableBorder(widths, 3, 3);

        char successMsg[100] = "Penawar berhasil! Obat ";
        strcat(successMsg, medicationName);
        strcat(successMsg, " dibatalkan.");
        printSuccess(successMsg);

        return true;
    }
    else
    {
        printError("Anda tidak perlu mengonsumsi penawar! Urutan obat yang diminum sudah benar.");
        return false;
    }
}