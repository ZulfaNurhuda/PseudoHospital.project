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
    if (!patient->treatedStatus)
    {
        if (strcmp(patient->disease, "Tidak terdeteksi") == 0)
        {
            printSuccess("Pasien tidak diresepkan obat karena tidak ada penyakit terdeteksi.");
            return true; // Tidak ada penawar yang perlu dikonsumsi
        }
        else
        {
            printError("Anda belum diberikan resep obat!");
            return false;
        }
    }

    // Pengecekan top < 0 sudah ada, memastikan stack tidak kosong.
    if (patient->medicationsTaken.top < 0)
    {
        printError("Anda belum mengonsumsi obat apapun!");
        return false;
    }

    // Ambil ID obat terakhir terlebih dahulu, lalu baru decrement 'top'.
    // Ini memastikan kita mengakses indeks yang valid sebelum mengubah 'top'.
    int lastMedicationId = patient->medicationsTaken.medicationId[patient->medicationsTaken.top];
    patient->medicationsTaken.top--;
    // --- KOREKSI SELESAI DI SINI ---

    if (patient->life < 0)
    {
        printError("Data nyawa tidak valid!");
        return false;
    }

    if (patient->life < 3)
    {
        patient->life++;
    }

    char medicationName[50] = "Tidak dikenal";
    if (hospital->medications.nEff > 0)
    {
        for (int i = 0; i < hospital->medications.nEff; i++)
        {
            if (hospital->medications.elements[i].id == lastMedicationId)
            {
                strcpy(medicationName, hospital->medications.elements[i].name);
                break;
            }
        }
    }

    printHeader("Konsumsi Penawar");
    int widths[] = {15, 20, 10};
    const char *headers[] = {"Obat Dibatalkan", "Nyawa Tersisa", "Status"};
    // Perbaiki jumlah kolom untuk printTableRow dan printTableBorder agar sesuai dengan 'widths' (3 kolom)
    printTableRow(headers, widths, 3); // Ubah 4 menjadi 3
    printTableBorder(widths, 3, 1);

    char lifeStr[10] = "";
    int k = 0;
    int life = patient->life;
    if (life == 0)
        lifeStr[k++] = '0';
    else
        while (life > 0)
        {
            lifeStr[k++] = (life % 10) + '0';
            life /= 10;
        }
    lifeStr[k] = '\0'; // Pastikan string diakhiri dengan null terminator

    const char *row[] = {medicationName, lifeStr, "Penawar berhasil"};
    printTableRow(row, widths, 3); // Ubah 3 menjadi 3
    printTableBorder(widths, 3, 3);

    char successMsg[100] = "Penawar berhasil! Obat ";
    strcat(successMsg, medicationName);
    strcat(successMsg, " dibatalkan.");
    printSuccess(successMsg);
    return true;
}