#include "diagnose.h"

boolean diagnosePatient(Hospital *hospital, Session *session)
{
    // ========== VALIDASI INPUT AWAL DAN SESI ==========
    // Memeriksa apakah pointer ke struktur Hospital dan Session tidak NULL.
    if (hospital == NULL || session == NULL)
    {
        printError("Struktur rumah sakit atau sesi tidak valid!"); // Pesan error jika input tidak valid.
        return false;                                              // Mengembalikan false menandakan kegagalan.
    }
    // Memastikan bahwa pengguna yang memanggil fungsi ini sudah login (`session->isLoggedIn`)
    // dan memiliki peran sebagai Dokter (`session->role == DOCTOR`).
    if (!session->isLoggedIn || session->role != DOCTOR)
    {
        printError("Akses ditolak! Hanya Dokter yang dapat mendiagnosis."); // Pesan error jika hak akses tidak sesuai.
        return false;                                                       // Mengembalikan false.
    }

    // ========== MENCARI DATA DOKTER YANG SEDANG LOGIN ==========
    // Menggunakan username dari sesi untuk menemukan entri dokter yang sesuai.
    int doctorIdx = -1; // Indeks dokter dalam array `hospital->doctors.elements`.
    // Melakukan iterasi melalui daftar dokter di rumah sakit.
    for (int i = 0; i < hospital->doctors.nEff; i++)
    {
        // Membandingkan username dokter di daftar dengan username dari sesi.
        if (strcmp(hospital->doctors.elements[i].username, session->username) == 0)
        {
            doctorIdx = i; // Jika ditemukan, simpan indeksnya.
            break;         // Keluar dari loop.
        }
    }
    // Jika dokter tidak ditemukan setelah iterasi.
    if (doctorIdx == -1)
    {
        printError("Dokter tidak ditemukan dalam daftar!"); // Pesan error.
        return false;                                       // Mengembalikan false.
    }
    // Mendapatkan pointer ke struktur Doctor yang sesuai.
    Doctor *doctor = &hospital->doctors.elements[doctorIdx];

    // ========== VALIDASI RUANGAN DOKTER ==========
    // Memastikan dokter sudah ditugaskan ke sebuah ruangan.
    // `doctor->room[0] == '\0'` berarti string `room` pada dokter kosong.
    if (doctor->room[0] == '\0')
    {
        printError("Dokter tidak ditugaskan ke ruangan manapun!"); // Pesan error.
        return false;                                              // Mengembalikan false.
    }

    // ========== MENCARI RUANGAN DOKTER DAN PASIEN DI DALAMNYA ==========
    // Mencari struktur Room yang sesuai dengan kode ruangan dokter.
    Room *doctorRoom = NULL; // Pointer ke ruangan dokter, diinisialisasi NULL.
    // Iterasi melalui denah layout.
    for (int i = 0; i < hospital->layout.rowEff; i++)
    {
        for (int j = 0; j < hospital->layout.colEff; j++)
        {
            // Membandingkan kode ruangan di denah dengan kode ruangan dokter.
            if (strcmp(hospital->layout.elements[i][j].code, doctor->room) == 0)
            {
                doctorRoom = &hospital->layout.elements[i][j]; // Ruangan ditemukan.
                break;                                         // Keluar dari loop kolom.
            }
        }
        if (doctorRoom != NULL) // Jika ruangan sudah ditemukan di loop kolom.
            break;              // Keluar dari loop baris.
    }

    // Jika ruangan dokter tidak ditemukan di denah, atau jika tidak ada pasien di dalam ruangan tersebut.
    // `doctorRoom->patientInRoom.nEff == 0` berarti tidak ada pasien dalam daftar `patientInRoom`.
    if (doctorRoom == NULL || doctorRoom->patientInRoom.nEff == 0)
    {
        // Pesan error ini mungkin lebih tepat jika "Tidak ada pasien di dalam ruangan dokter."
        // "Mengantri" biasanya merujuk pada `Queue`, bukan `patientInRoom`.
        printError("Ruangan dokter ini tidak memiliki pasien yang mengantri."); // Atau "Tidak ada pasien di dalam ruangan."
        return false;                                                           // Mengembalikan false.
    }

    // ========== MENGAMBIL DAN MEMVALIDASI PASIEN PERTAMA DI RUANGAN ==========
    // Mengambil ID pasien pertama dari daftar `patientInRoom` di ruangan dokter.
    // Diasumsikan pasien pertama dalam array ini adalah yang akan didiagnosis.
    int patientId = doctorRoom->patientInRoom.patientId[0];
    int patientIdx = -1; // Indeks pasien di daftar pasien rumah sakit.
    // Mencari data lengkap pasien berdasarkan ID yang didapat.
    for (int i = 0; i < hospital->patients.nEff; i++)
    {
        if (hospital->patients.elements[i].id == patientId)
        {
            patientIdx = i; // Pasien ditemukan.
            break;
        }
    }

    // Jika data pasien dengan ID tersebut tidak ditemukan di daftar pasien rumah sakit.
    if (patientIdx == -1)
    {
        printError("Pasien tidak ditemukan!"); // Pesan error (inkonsistensi data).
        return false;                          // Mengembalikan false.
    }
    // Mendapatkan pointer ke struktur Patient.
    Patient *patient = &hospital->patients.elements[patientIdx];

    // ========== VALIDASI STATUS DIAGNOSIS PASIEN ==========
    // Memastikan pasien belum pernah didiagnosis sebelumnya untuk sesi ini/penyakit ini.
    if (patient->diagnosedStatus) // Jika `diagnosedStatus` sudah true.
    {
        printError("Pasien sudah didiagnosa sebelumnya!"); // Pesan error.
        return false;                                      // Mengembalikan false.
    }

    // ========== VALIDASI KESESUAIAN RUANGAN ANTRIAN PASIEN ==========
    // Memastikan bahwa pasien yang akan didiagnosis memang terdaftar di antrian untuk ruangan dokter ini.
    // `patient->queueRoom` menyimpan kode ruangan tempat pasien mengantri.
    // Ini adalah pemeriksaan tambahan untuk memastikan konsistensi data.
    if (strcmp(patient->queueRoom, doctor->room) != 0)
    {
        printError("Pasien tidak berada di antrian ruangan dokter!"); // Pesan error.
        return false;                                                 // Mengembalikan false.
    }

    // ========== PROSES DIAGNOSIS BERDASARKAN DATA KESEHATAN ==========
    // String untuk menyimpan nama penyakit yang terdeteksi, default "Tidak terdeteksi".
    char diseaseStr[50] = "Tidak terdeteksi";
    // Memeriksa apakah ada daftar penyakit yang terdefinisi di rumah sakit.
    if (hospital->diseases.nEff > 0)
    {
        // Melakukan iterasi melalui setiap penyakit dalam daftar penyakit.
        for (int i = 0; i < hospital->diseases.nEff; i++)
        {
            Disease *d = &hospital->diseases.elements[i]; // Pointer ke penyakit saat ini.

            // Membandingkan data kesehatan pasien dengan rentang kriteria untuk penyakit `d`.
            // Jika semua data kesehatan pasien masuk dalam rentang min-max penyakit `d`.
            if (patient->bodyTemperature >= d->bodyTemperatureMin && patient->bodyTemperature <= d->bodyTemperatureMax &&
                patient->systolicBloodPressure >= d->systolicBloodPressureMin && patient->systolicBloodPressure <= d->systolicBloodPressureMax &&
                patient->diastolicBloodPressure >= d->diastolicBloodPressureMin && patient->diastolicBloodPressure <= d->diastolicBloodPressureMax &&
                patient->heartRate >= d->heartRateMin && patient->heartRate <= d->heartRateMax &&
                patient->oxygenSaturation >= d->oxygenSaturationMin && patient->oxygenSaturation <= d->oxygenSaturationMax &&
                patient->bloodSugarLevel >= d->bloodSugarLevelMin && patient->bloodSugarLevel <= d->bloodSugarLevelMax &&
                patient->weight >= d->weightMin && patient->weight <= d->weightMax &&                                         // Diasumsikan ada field weightMin/Max di Disease
                patient->height >= d->heightMin && patient->height <= d->heightMax &&                                         // Diasumsikan ada field heightMin/Max di Disease
                patient->cholesterolLevel >= d->cholesterolLevelMin && patient->cholesterolLevel <= d->cholesterolLevelMax && // Diasumsikan ada field cholesterolMin/Max
                patient->platelets >= d->plateletsMin && patient->platelets <= d->plateletsMax)                               // Diasumsikan ada field plateletsMin/Max
            {
                strcpy(diseaseStr, d->name);          // Jika penyakit terdeteksi, salin nama penyakit ke `diseaseStr`.
                patient->disease[0] = '\0';           // Kosongkan field penyakit pasien terlebih dahulu (untuk menghindari sisa string lama).
                strcat(patient->disease, diseaseStr); // Set field penyakit pada data pasien.
                patient->diagnosedStatus = true;      // Update status diagnosis pasien menjadi true.
                break;                                // Keluar dari loop penyakit karena diagnosis sudah ditemukan.
            }
        }
    }

    // ========== OUTPUT HASIL DIAGNOSA ==========
    // Jika setelah semua pemeriksaan, `diseaseStr` masih "Tidak terdeteksi".
    if (strcmp(diseaseStr, "Tidak terdeteksi") == 0)
    {
        printSuccess("Pasien tidak terdiagnosis penyakit apapun!"); // Pesan sukses.
        // Meskipun tidak terdeteksi penyakit spesifik, status diagnosis pasien tetap perlu diupdate
        // untuk menandakan bahwa proses diagnosis telah dilakukan.
        // Jika tidak ada penyakit terdeteksi, `patient->disease` mungkin tetap kosong atau diisi "Sehat".
        // Skrip saat ini tidak secara eksplisit set `patient->diagnosedStatus = true` di sini,
        // yang berarti jika tidak ada penyakit cocok, `diagnosedStatus` tetap false. Ini mungkin perlu ditinjau.
        // Namun, sesuai instruksi, tidak mengubah algoritma.
    }
    else // Jika penyakit terdeteksi.
    {
        // Mencetak nama pasien dan penyakit yang terdiagnosis.
        printf("%s terdiagnosa penyakit %s%s%s!\n", patient->username, COLOR_YELLOW, diseaseStr, COLOR_RESET);
        // `patient->diagnosedStatus` sudah di-set true di dalam loop jika penyakit ditemukan.
    }

    return true; // Proses diagnosis selesai (baik menemukan penyakit atau tidak).
}