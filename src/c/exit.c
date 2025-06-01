#include "exit.h"

void exitProgram(Hospital *hospital, Session *session)
{
    if (hospital == NULL || session == NULL)
    {

        printError("Struktur rumah sakit atau sesi tidak valid!");

        return;
    }

    printHeader("Keluar Program");

    printSuccess("Terima kasih telah menggunakan Sistem Manajemen Rumah Sakit Nimons!\n");

    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                 Sampai Jumpa!                ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");

    freeHospital(hospital);
}