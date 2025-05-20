#ifndef ENIGMA_H
#define ENIGMA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "abstractDataTypes.h"
#include "utils.h"

/**
 * Nama Fungsi: `enigmaEncrypt`
 * 
 * Deskripsi: Mengenkripsi teks menggunakan simulasi mesin Enigma dengan konfigurasi rotor, reflektor, dan plugboard. Konfigurasi dienkripsi dengan Caesar Cipher. Teks dienkripsi menjadi teks terenkripsi.
 * 
 * Parameter:
 *   - `plaintext (const char*)`: Teks asli yang akan dienkripsi.
 *   - `ciphertext (char*)`: Buffer untuk menyimpan teks terenkripsi.
 *   - `maxLength (int)`: Panjang maksimum buffer ciphertext.
 * 
 * Return: `boolean` - True jika enkripsi berhasil, false jika input tidak valid, buffer terlalu kecil, atau karakter tidak didukung.
 */
boolean enigmaEncrypt(const char *plaintext, char *ciphertext, int maxLength);

/**
 * Nama Fungsi: `enigmaDecrypt`
 * 
 * Deskripsi: Mendekripsi teks terenkripsi menggunakan simulasi mesin Enigma dengan konfigurasi yang sama seperti saat enkripsi. Teks terenkripsi didekripsi kembali ke teks asli.
 * 
 * Parameter:
 *   - `ciphertext (const char*)`: Teks terenkripsi yang akan didekripsi.
 *   - `plaintext (char*)`: Buffer untuk menyimpan teks asli yang didekripsi.
 *   - `maxLength (int)`: Panjang maksimum buffer plaintext.
 * 
 * Return: `boolean` - True jika dekripsi berhasil, false jika input tidak valid, buffer terlalu kecil, atau karakter tidak didukung.
 */
boolean enigmaDecrypt(const char *ciphertext, char *plaintext, int maxLength);

#endif // ENIGMA_H