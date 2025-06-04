#include "../header/help.h"
#include <stdio.h>

static void printFootnote()
{
    printf("\nFootnote:\n");
    printf("Untuk menggunakan aplikasi, silahkan masukkan nama fungsi yang terdaftar\n");
    printf("Jangan lupa untuk memasukkan input yang valid\n");
}

void displayMenu(Session *session)
{
    printf("=========== HELP ===========\n\n");

    if (!session->isLoggedIn)
    {
        printf("Kamu belum login sebagai role apapun. Silahkan login terlebih dahulu.\n\n");
        printf("LOGIN: Masuk ke dalam akun yang sudah terdaftar\n");
        printf("REGISTER: Membuat akun baru\n");
    }
    else
    {
        if (session->role == DOCTOR)
        {
            printf("Halo Dokter %s. Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n", session->username);
            printf("LOGOUT: Keluar dari akun yang sedang digunakan\n");
            printf("DIAGNOSIS: Melakukan diagnosis penyakit pasien berdasarkan kondisi tubuh pasien\n");
        }
        else if (session->role == PATIENT)
        {
            printf("Selamat datang, %s. Kamu memanggil command HELP. Kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n", session->username);
            printf("LOGOUT: Keluar dari akun yang sedang digunakan\n");
            printf("DAFTAR_CHECKUP: Mendaftarkan diri untuk pemeriksaan dokter\n");
        }
        else if (session->role == MANAGER)
        {
            printf("Halo Manager %s. Kenapa kamu memanggil command HELP? Kan kamu manager, tapi yasudahlah kamu pasti sedang kebingungan. Berikut adalah hal-hal yang dapat kamu lakukan sekarang:\n\n", session->username);
            printf("LOGOUT: Keluar dari akun yang sedang digunakan\n");
            printf("TAMBAH_DOKTER: Mendaftarkan dokter baru ke sistem\n");
            printf("ASSIGN_DOKTER: Menugaskan dokter ke ruangan tertentu\n");
            printf("LIHAT_DENAH: Melihat denah rumah sakit\n");
            printf("LIHAT_RUANGAN: Melihat detail ruangan tertentu\n");
            printf("LIHAT_USER: Melihat daftar seluruh pengguna\n");
            printf("LIHAT_PASIEN: Melihat daftar pasien\n");
            printf("LIHAT_DOKTER: Melihat daftar dokter\n");
            printf("CARI_USER: Mencari pengguna berdasarkan kriteria\n");
            printf("CARI_PASIEN: Mencari pasien berdasarkan kriteria\n");
            printf("CARI_DOKTER: Mencari dokter berdasarkan kriteria\n");
        }
    }

    printFootnote();
}