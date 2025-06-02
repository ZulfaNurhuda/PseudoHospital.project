#include "canGoHome.h"

boolean canGoHome(Hospital *hospital, Session *session)
{
    if (!hospital || !session)
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

    Patient *patient = NULL;

    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (hospital->patients.elements[i].id == session->userId)
        {
            patient = &hospital->patients.elements[i];
            break;
        }
    }

    if (!patient)
    {
        printError("Pasien tidak ditemukan!");
        return false;
    }

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
        int prescribedCount = patient->medicationsPrescribed.nEff;
        int takenCount = patient->medicationsTaken.top + 1;

        if (takenCount < prescribedCount)
        {
            statusStr = "Belum Minum Semua Obat";
            descriptionStr = "Anda belum mengonsumsi semua obat yang telah diresepkan.";
        }
        else if (takenCount > prescribedCount)
        {
            statusStr = "Urutan Obat Salah";
            descriptionStr = "Anda mengonsumsi lebih banyak obat dari yang diresepkan.";
        }
        else
        {
            boolean correctOrder = true;
            for (int i = 0; i < takenCount && correctOrder; i++)
            {
                if (patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i])
                {
                    correctOrder = false;
                }
            }

            if (!correctOrder)
            {
                statusStr = "Urutan Obat Salah";
                descriptionStr = "Anda mengonsumsi obat dengan urutan yang salah.";
            }
            else
            {
                statusStr = "Boleh Pulang";
                descriptionStr = "Anda telah selesai menjalani perawatan.";
                canGoHomeStatus = true;
            }
        }
    }

    printHeader("Status Pulang");
    int widths[] = {20, 40};
    const char *headers[] = {"Status", "Keterangan"};
    const char *row[] = {statusStr, descriptionStr};

    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    printTableBorder(widths, 2, 2);
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    if (canGoHomeStatus)
    {
        for (int i = 0; i < hospital->treatmentHistory.nEff; i++)
        {
            if (hospital->treatmentHistory.elements[i].patientId == patient->id)
            {
                int doctorId = hospital->treatmentHistory.elements[i].doctorId;
                for (int j = 0; j < hospital->doctors.nEff; j++)
                {
                    if (hospital->doctors.elements[j].id == doctorId)
                    {
                        hospital->doctors.elements[j].aura++;
                        break;
                    }
                }
                break;
            }
        }

        if (patient->queueRoom[0] != '\0')
        {
            Room *room = NULL;
            boolean roomFound = false;

            for (int i = 0; i < hospital->layout.rowEff && !roomFound; i++)
            {
                for (int j = 0; j < hospital->layout.colEff && !roomFound; j++)
                {
                    if (strcmp(hospital->layout.elements[i][j].code, patient->queueRoom) == 0)
                    {
                        room = &hospital->layout.elements[i][j];
                        roomFound = true;
                    }
                }
            }

            if (room)
            {
                for (int i = 0; i < room->patientInRoom.nEff; i++)
                {
                    if (room->patientInRoom.patientId[i] == patient->id)
                    {
                        for (int j = i; j < room->patientInRoom.nEff - 1; j++)
                        {
                            room->patientInRoom.patientId[j] = room->patientInRoom.patientId[j + 1];
                        }
                        room->patientInRoom.nEff--;
                        break;
                    }
                }

                Queue *targetQueue = NULL;
                for (int i = 0; i < hospital->queues.capacity; i++)
                {
                    if (hospital->queues.queues[i].roomCode[0] != '\0' &&
                        strcmp(hospital->queues.queues[i].roomCode, patient->queueRoom) == 0)
                    {
                        targetQueue = &hospital->queues.queues[i];
                        break;
                    }
                }

                if (targetQueue && !isQueueEmpty(targetQueue))
                {
                    QueueInfo nextPatientInfo;
                    if (dequeue(targetQueue, &nextPatientInfo.patientId))
                    {
                        if (room->patientInRoom.nEff < room->patientInRoom.capacity)
                        {
                            room->patientInRoom.patientId[room->patientInRoom.nEff] = nextPatientInfo.patientId;
                            room->patientInRoom.nEff++;
                        }
                    }
                }
            }
        }

        patient->queueRoom[0] = '\0';
        patient->queuePosition = 0;
        patient->diagnosedStatus = false;
        patient->treatedStatus = false;
        patient->life = 3;
        strcpy(patient->disease, "");

        patient->medicationsTaken.top = -1;
        if (patient->medicationsTaken.medicationId != NULL)
        {
            free(patient->medicationsTaken.medicationId);
            patient->medicationsTaken.medicationId = NULL;
        }

        patient->medicationsPrescribed.nEff = 0;
        if (patient->medicationsPrescribed.medicationId != NULL)
        {
            free(patient->medicationsPrescribed.medicationId);
            patient->medicationsPrescribed.medicationId = NULL;
        }

        printSuccess("Selamat, Anda boleh pulang!");
        return true;
    }
    else
    {
        printError("Anda belum boleh pulang!");
        return false;
    }
}