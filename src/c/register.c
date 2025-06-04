#include "../header/register.h"
#include "../header/login.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void registerPatient(Hospital *hospital, Session *session)
{
    
    if (session->isLoggedIn)
    {
        printf("Anda sudah login sebagai %s! Anda tidak dapat mendaftar pasien.\n", session->username);
        return;
    }
    
    char username[50];
    char password[100];

    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (strcmp(hospital->users.elements[i].username, username) == 0)
        {
            printf("Registrasi gagal! Pasien dengan nama %s sudah terdaftar.\n", username);
            return;
        }
    }

    if (hospital->users.nEff >= hospital->users.capacity ||
        hospital->patients.nEff >= hospital->patients.capacity)
    {
        printf("Kapasitas pengguna atau pasien penuh!\n");
        return;
    }

    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = hospital->users.nEff + 1;
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->role = PATIENT;
    hospital->users.nEff++;

    Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];
    newPatient->id = newUser->id;
    strcpy(newPatient->username, username);
    strcpy(newPatient->disease, "");
    newPatient->life = 3;
    newPatient->bananaRich = 100.0f;

    newPatient->bodyTemperature = 36.5f;
    newPatient->systolicBloodPressure = 120;
    newPatient->diastolicBloodPressure = 80;
    newPatient->heartRate = 75;
    newPatient->oxygenSaturation = 98;
    newPatient->bloodSugarLevel = 100.0f;
    newPatient->weight = 70.0f;
    newPatient->height = 170.0f;
    newPatient->cholesterolLevel = 180.0f;
    newPatient->ldlCholesterolLevel = 100.0f;
    newPatient->platelets = 250;

    newPatient->medicationsTaken.capacity = 10;
    newPatient->medicationsTaken.medicationID = (int *)malloc(10 * sizeof(int));
    newPatient->medicationsTaken.top = -1;

    newPatient->medicationsPrescribed.capacity = 10;
    newPatient->medicationsPrescribed.medicationID = (int *)malloc(10 * sizeof(int));
    newPatient->medicationsPrescribed.nEff = 0;

    newPatient->queueRoom[0] = '\0';
    newPatient->queuePosition = -1;

    newPatient->diagnosedStatus = false;
    newPatient->treatedStatus = false;

    hospital->patients.nEff++;

    printf("Pasien %s berhasil ditambahkan!\n\n", username);

    login(hospital, session);
}