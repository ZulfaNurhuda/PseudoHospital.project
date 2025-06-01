#include "treat.h"
#include "utils.h"

boolean treatPatient(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit, sesi, atau username tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != DOCTOR)
    {
        printError("Akses ditolak! Hanya Dokter yang dapat meresepkan obat.");
        return false;
    }

    // Mencari dokter berdasarkan session username
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

    // Memastikan dokter sudah ditugaskan ke ruangan
    if (doctor->room[0] == '\0')
    {
        printError("Dokter tidak ditugaskan ke ruangan manapun!");
        return false;
    }

    // Memeriksa apakah ruangan dokter memiliki pasien yang antri
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
        printError("Tidak ada pasien yang berada didalam ruangan dokter ini.");
        return false;
    }

    // Ambil pasien yang berada di depan ruangan
    int patientId = doctorRoom->patientInRoom.patientId[0]; // Pasien pertama di dalam ruangan
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

    if (strcmp(patient->disease, "Tidak terdeteksi") == 0)
    {
        printError("Pasien tidak menderita penyakit apapun!");
        return false;
    }

    // Pastikan pasien belum terdiagnosis
    if (patient->diagnosedStatus)
    {
        printError("Pasien sudah didiagnosa sebelumnya!");
        return false;
    }

    // Memastikan pasien belum diberikan resep obat
    if (patient->treatedStatus)
    {
        printError("Pasien sudah diberikan resep obat!");
        return false;
    }

    // Memastikan pasien berada di antrian ruangan dokter
    if (strcmp(patient->queueRoom, doctor->room) != 0)
    {
        printError("Pasien tidak berada di antrian ruangan dokter!");
        return false;
    }

    // Mencari penyakit pasien
    int diseaseId = -1;
    for (int i = 0; i < hospital->diseases.nEff; i++)
    {
        if (strcmp(hospital->diseases.elements[i].name, patient->disease) == 0)
        {
            diseaseId = hospital->diseases.elements[i].id;
            break;
        }
    }

    if (diseaseId == -1)
    {
        printError("Penyakit pasien tidak ditemukan dalam database!");
        return false;
    }

    // Hitung dulu berapa banyak obat yang diperlukan
    int medicationCount = 0;
    for (int i = 0; i < hospital->prescriptions.nEff; i++)
    {
        if (hospital->prescriptions.elements[i].diseaseId == diseaseId)
        {
            medicationCount++;
        }
    }

    // Alokasi memori untuk menyimpan resep sementara
    MedicationPrescription *tempPrescriptions = NULL;
    MedicationList prescribedMedications = {NULL, 0, 0};

    if (medicationCount > 0)
    {
        tempPrescriptions = malloc(medicationCount * sizeof(MedicationPrescription));
        if (tempPrescriptions == NULL)
        {
            printError("Gagal mengalokasi memori untuk resep sementara!");
            return false;
        }

        // Mengisi daftar resep sementara
        int tempCount = 0;
        for (int i = 0; i < hospital->prescriptions.nEff; i++)
        {
            if (hospital->prescriptions.elements[i].diseaseId == diseaseId)
            {
                tempPrescriptions[tempCount] = hospital->prescriptions.elements[i];
                tempCount++;
            }
        }

        // Mengurutkan berdasarkan doseOrder (bubble sort)
        for (int i = 0; i < medicationCount - 1; i++)
        {
            for (int j = 0; j < medicationCount - i - 1; j++)
            {
                if (tempPrescriptions[j].doseOrder > tempPrescriptions[j + 1].doseOrder)
                {
                    // Tukar posisi
                    MedicationPrescription temp = tempPrescriptions[j];
                    tempPrescriptions[j] = tempPrescriptions[j + 1];
                    tempPrescriptions[j + 1] = temp;
                }
            }
        }

        // Alokasi memori untuk daftar obat final
        prescribedMedications.elements = malloc(medicationCount * sizeof(Medication));
        if (prescribedMedications.elements == NULL)
        {
            free(tempPrescriptions);
            printError("Gagal mengalokasi memori untuk daftar obat!");
            return false;
        }

        // Mengisi daftar obat final berdasarkan urutan yang sudah diurutkan
        for (int i = 0; i < medicationCount; i++)
        {
            prescribedMedications.elements[i].id = tempPrescriptions[i].medicationId;
            prescribedMedications.nEff++;
        }

        // Bebaskan memori sementara
        free(tempPrescriptions);
    }

    // Menampilkan daftar obat yang harus diberikan
    if (prescribedMedications.nEff > 0)
    {
        printf(COLOR_GREEN "Dokter sedang mengobati pasien!\n" COLOR_RESET);
        printf("Pasien memiliki penyakit %s%s%s\n", COLOR_YELLOW, patient->disease, COLOR_RESET);
        printf("\nObat yang harus diberikan:\n");

        // Tentukan lebar kolom untuk tabel
        int widths[] = {5, 30}; // No, Nama Obat, Keterangan

        // Header Tabel
        const char *header[] = {"No.", "Nama Obat"};
        printTableBorder(widths, 2, 1);
        printTableRow(header, widths, 2);
        printTableBorder(widths, 2, 2);

        // Isi tabel dengan data obat yang telah diberikan kepada pasien
        for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
        {
            // Mendapatkan obat berdasarkan ID
            Medication *med = &hospital->medications.elements[patient->medicationsPrescribed.medicationId[i]];

            // Menyiapkan data untuk baris tabel
            char no[10] = "", medicationName[50] = "";
            integerToString(i + 1, no, sizeof(no));
            strcat(no, ".");
            strcat(medicationName, med->name);
            const char *row[] = {no, medicationName};

            // Menampilkan baris tabel
            printTableRow(row, widths, 2);
        }

        printTableBorder(widths, 2, 3);

        // Menyimpan resep obat ke dalam ADT pasien
        // Pastikan kapasitas mencukupi
        if (patient->medicationsPrescribed.capacity < medicationCount)
        {
            // Realokasi jika kapasitas tidak mencukupi
            int *newMedicationIds = realloc(patient->medicationsPrescribed.medicationId,
                                            medicationCount * sizeof(int));
            if (newMedicationIds == NULL)
            {
                free(prescribedMedications.elements);
                printError("Gagal mengalokasi memori untuk daftar obat pasien!");
                return false;
            }
            patient->medicationsPrescribed.medicationId = newMedicationIds;
            patient->medicationsPrescribed.capacity = medicationCount;
        }

        // Menyimpan medication ID ke dalam daftar obat pasien berdasarkan urutan
        patient->medicationsPrescribed.nEff = 0;
        for (int i = 0; i < prescribedMedications.nEff; i++)
        {
            patient->medicationsPrescribed.medicationId[patient->medicationsPrescribed.nEff] =
                prescribedMedications.elements[i].id;
            patient->medicationsPrescribed.nEff++;
        }

        // Update status pengobatan pasien
        patient->treatedStatus = true;

        // Bebaskan memori yang dialokasi
        free(prescribedMedications.elements);

        printSuccess("Obat berhasil diberikan!");
        return true;
    }
    else
    {
        printError("Tidak ada obat yang ditemukan untuk penyakit ini!");
        return false;
    }
}