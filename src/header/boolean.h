#ifndef BOOLEAN_H // Mengecek apakah header BOOLEAN_H sudah didefinisikan
#define BOOLEAN_H // Mendefinisikan BOOLEAN_H untuk menghindari multiple inclusion

// Bagian ini hanya dijalankan jika bukan di dalam konteks C++ (misalnya, di C)
#ifndef __cplusplus

// Mengecek apakah versi standar C yang digunakan lebih besar dari C17 (ISO C18)
#if defined __STDC_VERSION__ && __STDC_VERSION__ > 201710L

// Jika menggunakan C18 atau lebih baru, tidak perlu mendefinisikan bool karena sudah ada di dalam standar
#else

// Untuk versi C yang lebih lama (misalnya C99 atau sebelumnya)
// Mendefinisikan tipe data bool dan konstanta true/false untuk kompatibilitas
#define boolean _Bool // Menggunakan _Bool untuk mendefinisikan boolean
#define true 1        // Mendefinisikan nilai true sebagai 1
#define false 0       // Mendefinisikan nilai false sebagai 0

#endif

// Bagian ini hanya dijalankan jika di dalam konteks C++ (misalnya, di C++)
#else

// Di C++, kita juga mendefinisikan bool sebagai _Bool untuk memastikan kompatibilitas
#define boolean _Bool // Menggunakan _Bool untuk mendefinisikan boolean

#endif

// Mendefinisikan makro __bool_true_false_are_defined untuk memberi tahu compiler bahwa
// tipe bool dan nilai true/false sudah didefinisikan.
#define __boolean_true_false_are_defined 1

#endif // BOOLEAN_H
