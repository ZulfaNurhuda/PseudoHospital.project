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
        if (hospital->patients.elements[i].id == session->userId)
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
        boolean correctOrder = true;
        if (patient->medicationsTaken.top + 1 != patient->medicationsPrescribed.nEff)
        {
            correctOrder = false;
        }

        else
        {
            for (int i = 0; i <= patient->medicationsTaken.top; i++)
            {
                if (patient->medicationsTaken.medicationId[i] != patient->medicationsPrescribed.medicationId[i])
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
                    if (hospital->treatmentHistory.elements[i].patientId == patient->id)
                    {
                        for (int j = 0; j < hospital->doctors.nEff; j++)
                        {
                            if (hospital->doctors.elements[j].id == hospital->treatmentHistory.elements[i].doctorId)
                            {
                                hospital->doctors.elements[j].aura += 1;
                                break;
                            }
                        }
                        break;
                    }
                }
            }

            // Menghapus pasien dari antrian (if they are still in one, which is an inconsistency)
            // This is a cleanup step. Normal dequeueing should happen in treatPatient.
            // In canGoHome function, replace the queue cleanup logic
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
                if (targetQueue != NULL && !isQueueEmpty(targetQueue))
                {
                    QueueNode *current = targetQueue->head;
                    QueueNode *prev = NULL;
                    boolean foundAndRemoved = false;
                    while (current != NULL)
                    {
                        if (current->info.patientId == patient->id)
                        {
                            foundAndRemoved = true;
                            if (prev == NULL)
                            { // Patient is at the head
                                targetQueue->head = current->next;
                                if (targetQueue->head == NULL)
                                {
                                    targetQueue->tail = NULL;
                                }
                                else
                                {
                                    targetQueue->head->prev = NULL; // Update prev for new head
                                }
                            }
                            else
                            { // Patient is in the middle or at the end
                                prev->next = current->next;
                                if (current->next == NULL)
                                { // Patient was at the tail
                                    targetQueue->tail = prev;
                                }
                                else
                                {
                                    current->next->prev = prev; // Update prev for next node
                                }
                            }
                            free(current);
                            targetQueue->size--;
                            break;
                        }
                        prev = current;
                        current = current->next;
                    }
                    if (foundAndRemoved)
                    {
                        patient->queueRoom[0] = '\0';
                        patient->queuePosition = 0;
                    }
                }
                else
                {
                    patient->queueRoom[0] = '\0';
                    patient->queuePosition = 0;
                }
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