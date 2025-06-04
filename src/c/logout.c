#include "logout.h"

boolean logout(Session *session)
{

    if (session == NULL)
    {

        printError("Sesi tidak valid!");

        return false;
    }

    if (!session->isLoggedIn)
    {

        printError("Anda belum login! Silakan login terlebih dahulu.");

        return false;
    }

    char tempUsername[50];

    strcpy(tempUsername, session->username);

    session->userId = -1;

    session->username[0] = '\0';

    session->role = -1;

    session->isLoggedIn = false;

    char successMessage[100];

    successMessage[0] = '\0';

    strcat(successMessage, "Logout berhasil! Sampai jumpa, ");
    strcat(successMessage, tempUsername);
    strcat(successMessage, "!");

    printSuccess(successMessage);

    return true;
}
