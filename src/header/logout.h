#ifndef LOGOUT_H
#define LOGOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `logout`
 * 
 * Deskripsi: Mengakhiri sesi pengguna saat ini dengan mengatur ulang data sesi (isLoggedIn ke false, userID ke -1, username ke string kosong, dan role ke -1). Memeriksa apakah pengguna sudah login dan sesi valid.
 * 
 * Parameter:
 *   - `session (Session*)`: Pointer ke struktur sesi yang akan diatur ulang.
 * 
 * Return: `boolean` - True jika logout berhasil, false jika pengguna belum login atau sesi tidak valid.
 */
boolean logout(Session *session);

#endif // LOGOUT_H