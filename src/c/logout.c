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
        printError("Anda belum login!");
        return false;
    }

    session->userId = -1;
    session->username[0] = '\0';
    session->role = -1;
    session->isLoggedIn = false;

    printSuccess("Logout berhasil! Sampai jumpa!");
    return true;
}