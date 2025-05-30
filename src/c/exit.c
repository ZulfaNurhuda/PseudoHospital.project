#include <stdio.h>
#include <stdlib.h>

#include "exit.h"
#include "hospital.h"
#include "utils.h"

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
    // exit(0); // Removed: Control flow will return to main
}