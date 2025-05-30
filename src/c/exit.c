#include "exit.h"

void exitProgram(Hospital *hospital, Session *session)
{
    // Memeriksa apakah pointer hospital atau session adalah NULL.
    // Ini adalah pemeriksaan untuk memastikan struktur data utama valid sebelum melanjutkan.
    if (hospital == NULL || session == NULL)
    {
        // Mencetak pesan error jika salah satu struktur tidak valid.
        printError("Struktur rumah sakit atau sesi tidak valid!");
        // Keluar dari fungsi karena operasi tidak dapat dilanjutkan dengan aman.
        return;
    }

    // Mencetak header untuk bagian keluar program.
    printHeader("Keluar Program");
    // Mencetak pesan terima kasih kepada pengguna.
    printSuccess("Terima kasih telah menggunakan Sistem Manajemen Rumah Sakit Nimons!\n");
    // Mencetak pesan perpisahan dengan format kotak.
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                 Sampai Jumpa!                ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    // Memanggil fungsi freeHospital untuk membebaskan memori yang dialokasikan untuk struktur hospital.
    // Ini adalah langkah penting untuk mencegah kebocoran memori.
    freeHospital(hospital);
    // Catatan: Sesi (session) mungkin tidak perlu dibebaskan secara eksplisit di sini jika
    // itu adalah variabel lokal di 'main' atau dikelola secara berbeda (tidak ada alokasi dinamis).
    // Namun, jika sesi juga dialokasikan secara dinamis dan perlu dibebaskan,
    // pemanggilan freeSession(session); akan [HARUS!] ditambahkan di sini.
}