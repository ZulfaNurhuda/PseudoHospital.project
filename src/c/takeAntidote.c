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

    boolean isNeedAntidote = false;
    int lastMedicationId = patient->medicationsTaken.medicationId[patient->medicationsTaken.top];
    for (int i = 0; i < patient->medicationsTaken.top; i++)
    {
        if (patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i])
        {
            isNeedAntidote = true;
            break;
        }
    }

    if (isNeedAntidote)
    {
        char medicationName[50] = "Tidak dikenal";
        for (int i = 0; i < hospital->medications.nEff; i++)
        {
            if (hospital->medications.elements[i].id == lastMedicationId)
            {
                strcpy(medicationName, hospital->medications.elements[i].name);
                break;
            }
        }

        patient->medicationsTaken.top--;

        printHeader("Konsumsi Penawar");
        int widths[] = {15, 20, 10};
        const char *headers[] = {"Obat Dibatalkan", "Nyawa Tersisa", "Status"};
        printTableBorder(widths, 3, 1);
        printTableRow(headers, widths, 3);
        printTableBorder(widths, 3, 2);
        char lifeStr[15] = "";
        for (int i = 0; i < 3; i++)
        {
            if (i < patient->life)
            {
                strcat(lifeStr, "💙");
            }
            else
            {
                strcat(lifeStr, "🖤");
            }
        }
        const char *row[] = {medicationName, lifeStr, "Penawar berhasil"};
        printTableRow(row, widths, 3);
        printTableBorder(widths, 3, 3);

        char successMsg[100] = "";
        strcat(successMsg, "Penawar berhasil! Obat ");
        strcat(successMsg, medicationName);
        strcat(successMsg, " dibatalkan.");
        printSuccess(successMsg);
        return true;
    }
    else
    {
        printError("Anda tidak perlu mengonsumsi penawar! Urutan obat yang diminum sudah benar.");
        return true;
    }
}
