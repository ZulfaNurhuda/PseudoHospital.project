#ifndef RUN_LENGTH_ENCODING_H
#define RUN_LENGTH_ENCODING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "boolean.h"
#include "utils.h"

/**
 * Nama Fungsi: `generateRleCode`
 * 
 * Deskripsi: Menghasilkan kode Run-Length Encoding (RLE) dari username, misalnya, "aaabb" menjadi "a3b2". Kode ditulis ke buffer output dengan panjang maksimum maxLen.
 * 
 * Parameter:
 *   - `username (const char*)`: String username yang akan dikodekan.
 *   - `output (char*)`: Buffer untuk menyimpan kode RLE yang dihasilkan.
 *   - `maxLen (int)`: Panjang maksimum buffer output.
 * 
 * Return: `boolean` - True jika kode RLE berhasil dihasilkan, false jika username tidak valid, buffer penuh, atau maxLen tidak valid.
 */
boolean generateRleCode(const char *username, char *output, int maxLen);

#endif // RUN_LENGTH_ENCODING_H