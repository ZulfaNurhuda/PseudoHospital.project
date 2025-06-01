#include "takeMedication.h"

boolean takeMedication(Hospital *hospital, Session *session, int medicationId)
{
    // --- Validasi Input Awal ---
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!");
        return false;
    }
    if (!session->isLoggedIn || session->role != PATIENT)
    {
        printError("Akses ditolak! Hanya Pasien yang dapat mengonsumsi obat.");
        return false;
    }
    if (medicationId <= 0)
    {
        printError("ID obat tidak valid!");
        return false;
    }

    // --- Mencari Data Pasien ---
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

    // --- Validasi Resep dan Ketersediaan Obat di RS ---
    if (strcmp(patient->disease, "Tidak terdeteksi") == 0)
    {
        printSuccess("Pasien tidak diresepkan obat karena tidak ada penyakit terdeteksi.");
        return true; // Tidak ada obat yang perlu dikonsumsi
    }
    if (!patient->treatedStatus)
    {
        printError("Anda belum diberikan resep obat!");
        return false;
    }
    if (patient->medicationsPrescribed.nEff == 0)
    {
        printError("Tidak ada obat yang diresepkan untuk pasien ini!");
        return false;
    }
    if (hospital->medications.nEff == 0)
    {
        printError("Tidak ada obat terdaftar di rumah sakit!");
        return false;
    }

    // **PERBAIKAN KRUSIAL:**
    // --- Memeriksa apakah semua obat sudah dikonsumsi (dipindahkan ke sini) ---
    // Jika jumlah obat yang sudah dikonsumsi (top + 1) sudah mencapai
    // atau melebihi jumlah total obat yang diresepkan (nEff), berarti semua sudah selesai.
    if (patient->medicationsTaken.top + 1 >= patient->medicationsPrescribed.nEff)
    {
        printError("Semua obat yang diresepkan sudah dikonsumsi!");
        return false;
    }
    // END PERBAIKAN KRUSIAL

    // --- Validasi ID Obat yang Dimasukkan (Apakah ada di resep pasien & di daftar RS global) ---
    boolean isMedicationActuallyPrescribedToThisPatient = false;
    char medicationName[50] = "Tidak dikenal";
    boolean isIdInHospitalMedicationList = false;

    // Pertama, cek apakah ID obat ada di resep spesifik pasien ini
    for (int i = 0; i < patient->medicationsPrescribed.nEff; i++)
    {
        if (patient->medicationsPrescribed.medicationId[i] == medicationId)
        {
            isMedicationActuallyPrescribedToThisPatient = true;
            break;
        }
    }

    if (!isMedicationActuallyPrescribedToThisPatient)
    {
        // Jika obat tidak ada di daftar resep pasien sama sekali
        printError("Pilihan tidak tersedia! Obat ini tidak diresepkan untuk Anda.");
        return false;
    }

    // Kedua, dapatkan nama obat dari daftar obat global RS (jika sudah dipastikan diresepkan untuk pasien)
    // Ini penting agar 'medicationName' terisi dengan benar untuk pesan sukses/error.
    for (int i = 0; i < hospital->medications.nEff; i++)
    {
        if (hospital->medications.elements[i].id == medicationId)
        {
            strcpy(medicationName, hospital->medications.elements[i].name);
            isIdInHospitalMedicationList = true; // Menandakan bahwa ID ini juga ada di daftar global RS
            break;
        }
    }
    // Ini adalah pengecekan konsistensi data. Seharusnya obat yang diresepkan selalu ada di daftar RS.
    if (!isIdInHospitalMedicationList)
    {
        printError("Kesalahan sistem: Obat diresepkan tetapi tidak terdaftar di Rumah Sakit!");
        return false;
    }

    // Mendapatkan ID obat yang seharusnya dikonsumsi selanjutnya (urutan yang benar)
    // Baris ini aman karena `patient->medicationsTaken.top + 1 >= patient->medicationsPrescribed.nEff`
    // sudah dicek di awal fungsi.
    int expectedMedicationId = patient->medicationsPrescribed.medicationId[patient->medicationsTaken.top + 1];

    // --- Logika Memeriksa Urutan Obat dan Menangani Kesalahan ---
    if (medicationId == expectedMedicationId)
    {
        // Kasus: Obat yang dimasukkan adalah obat yang diharapkan (urutan benar).
        // Lanjutkan ke proses konsumsi obat yang berhasil.
    }
    else
    {
        // Kasus: Obat yang dimasukkan adalah obat yang diresepkan (sudah dicek oleh isMedicationActuallyPrescribedToThisPatient),
        // TETAPI BUKAN urutan yang diharapkan. Ini berarti salah urutan.
        patient->life--;
        printHeader("Konsumsi Obat");

        char errorMsg[100] = "Obat ";
        strcat(errorMsg, medicationName);
        strcat(errorMsg, " salah urutan! Nyawa tersisa: ");
        char lifeStr[10];
        integerToString(patient->life, lifeStr, sizeof(lifeStr)); // Asumsi integerToString ada dan berfungsi
        strcat(errorMsg, lifeStr);
        printError(errorMsg);

        if (patient->life <= 0)
        {
            printError("Pasien meninggal!");
            deletePatient(hospital, patient->id);
            session->isLoggedIn = false;
            session->userId = -1;
            session->username[0] = '\0';
            session->role = -1;
        }
        return false; // Keluar dari fungsi karena kesalahan urutan
    }

    // --- Menambahkan Obat ke Stack (medicationsTaken) ---
    // Bagian ini hanya dijalankan jika medicationId == expectedMedicationId (konsumsi benar)
    if (patient->medicationsTaken.top + 1 < patient->medicationsTaken.capacity)
    {
        patient->medicationsTaken.medicationId[++patient->medicationsTaken.top] = medicationId;
    }
    else
    {
        // Ini adalah skenario yang jarang terjadi jika kapasitas stack tidak diatur dengan baik
        // (misalnya, lebih kecil dari medicationsPrescribed.nEff).
        printError("Kapasitas penyimpanan obat yang sudah dikonsumsi penuh!");
        return false;
    }

    // --- Tampilan Konfirmasi dan Update Status ---
    printHeader("Konsumsi Obat");
    int widths[] = {15, 20};
    const char *headers[] = {"Obat", "Status"};
    printTableBorder(widths, 2, 1);
    printTableRow(headers, widths, 2);
    printTableBorder(widths, 2, 2);
    const char *row[] = {medicationName, "Berhasil dikonsumsi"};
    printTableRow(row, widths, 2);
    printTableBorder(widths, 2, 3);

    char lifeMsg[50] = "Nyawa tersisa: ";
    char lifeStr[10];
    integerToString(patient->life, lifeStr, sizeof(lifeStr)); // Asumsi integerToString ada
    strcat(lifeMsg, lifeStr);
    printSuccess(lifeMsg);

    char successMsg[100] = "Obat ";
    strcat(successMsg, medicationName);
    strcat(successMsg, " berhasil dikonsumsi!");
    printSuccess(successMsg);

    return true;
}