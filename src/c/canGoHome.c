#include "canGoHome.h"

boolean canGoHome(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat memeriksa status pulang.");
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
    printHeader("Status Pulang");

    int widths[] = {15, 30};
    const char *headers[] = {"Status", "Keterangan"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);

    const char *statusStr, *descriptionStr;
    boolean canGoHomeStatus = false;

    if (!patient->diagnosedStatus)
    {
        statusStr = "Belum Diagnosa";
        descriptionStr = "Anda belum didiagnosa oleh dokter.";
    }
    else if (!patient->treatedStatus)
    {
        statusStr = "Belum Diberi Obat";
        descriptionStr = "Anda belum diberikan resep obat.";
    }
    else
    {
        // Memeriksa urutan konsumsi obat
        boolean correctOrder = true;
        if (patient->medicationsTaken.top + 1 != patient->medicationsPrescribed.nEff)
        {
            correctOrder = false;
        }
        else
        {
            for (int i = 0; i <= patient->medicationsTaken.top; i++)
            {
                if (patient->medicationsTaken.medicationID[i] != patient->medicationsPrescribed.medicationID[i])
                {
                    correctOrder = false;
                    break;
                }
            }
        }

        if (!correctOrder)
        {
            statusStr = "Urutan Obat Salah";
            descriptionStr = "Anda belum mengonsumsi semua obat dengan urutan yang benar.";
        }
        else
        {
            statusStr = "Boleh Pulang";
            descriptionStr = "Anda telah selesai menjalani perawatan.";
            canGoHomeStatus = true;

            // Meningkatkan aura dokter
            if (hospital->treatmentHistory.nEff > 0)
            {
                for (int i = 0; i < hospital->treatmentHistory.nEff; i++)
                {
                    if (hospital->treatmentHistory.elements[i].patientID == patient->id)
                    {
                        for (int j = 0; j < hospital->doctors.nEff; j++)
                        {
                            if (hospital->doctors.elements[j].id == hospital->treatmentHistory.elements[i].doctorID)
                            {
                                hospital->doctors.elements[j].aura += 1.0;
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            // Menghapus pasien dari antrian
            if (hospital->queues.nRooms > 0 && patient->queueRoom[0] != '\0')
            {
                for (int i = 0; i < hospital->queues.nRooms; i++)
                {
                    Queue *queue = &hospital->queues.queues[i];
                    if (strcmp(queue->roomCode, patient->queueRoom) == 0)
                    {
                        for (int j = queue->idxHead; j <= queue->idxTail; j++)
                        {
                            if (queue->buffer[j].patientID == patient->id)
                            {
                                // Geser elemen setelah pasien
                                for (int k = j; k < queue->idxTail; k++)
                                {
                                    queue->buffer[k] = queue->buffer[k + 1];
                                }
                                queue->idxTail--;
                                break;
                            }
                        }
                        break;
                    }
                }
                // Reset queueRoom pasien
                patient->queueRoom[0] = '\0';
            }
        }
    }

    const char *row[] = {statusStr, descriptionStr};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    if (canGoHomeStatus)
    {
        printSuccess("Selamat, Anda boleh pulang!");
        return true;
    }
    else
    {
        printError("Anda belum boleh pulang!");
        return false;
    }
}