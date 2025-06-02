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

        if (takenCount != prescribedCount)
        {
            statusStr = "Belum Minum Semua Obat";
            descriptionStr = "Anda belum mengonsumsi semua obat yang telah diresepkan.";
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

    int widths[] = {15, 40};
    const char *headers[] = {"Status", "Keterangan"};

    const char *row[] = {statusStr, descriptionStr};

    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);

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
                QueueNode *current = targetQueue->head;
                QueueNode *prev = NULL;

                while (current)
                {
                    if (current->info.patientId == patient->id)
                    {

                        if (!prev)
                        {
                            targetQueue->head = current->next;
                            if (targetQueue->head)
                            {
                                targetQueue->head->prev = NULL;
                            }
                            else
                            {
                                targetQueue->tail = NULL;
                            }
                        }
                        else
                        {
                            prev->next = current->next;
                            if (current->next)
                            {
                                current->next->prev = prev;
                            }
                            else
                            {
                                targetQueue->tail = prev;
                            }
                        }

                        free(current);
                        targetQueue->size--;
                        break;
                    }
                    prev = current;
                    current = current->next;
                }
            }

            patient->queueRoom[0] = '\0';
            patient->queuePosition = 0;
            patient->diagnosedStatus = false;
            patient->treatedStatus = false;
            patient->life = 3;
            strcpy(patient->disease, "");

            patient->medicationsTaken.top = -1;
            free(patient->medicationsTaken.medicationId);
            patient->medicationsPrescribed.nEff = 0;
            free(patient->medicationsPrescribed.medicationId);
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
