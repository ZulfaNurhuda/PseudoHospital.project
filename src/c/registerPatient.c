#include "registerPatient.h"

boolean registerPatient(Hospital *hospital, Session *session, const char *inputUsername, const char *password)
{
    // Memeriksa apakah pointer hospital atau session adalah NULL.
    // Ini adalah pemeriksaan keamanan dasar untuk mencegah dereferensi pointer NULL.
    if (hospital == NULL || session == NULL)
    {
        // Mencetak pesan error jika salah satu pointer tidak valid.
        printError("Struktur rumah sakit atau sesi tidak valid!");
        // Mengembalikan false karena registrasi tidak dapat dilanjutkan.
        return false;
    }

    // Memeriksa apakah username input atau password adalah NULL atau string kosong.
    // Username dan password yang valid diperlukan untuk registrasi.
    if (inputUsername == NULL || inputUsername[0] == '\0' || password == NULL || password[0] == '\0')
    {
        // Mencetak pesan error jika username (setelah trim) atau password tidak valid.
        printError("Username (setelah trim) atau password tidak valid!");
        // Mengembalikan false karena input tidak valid.
        return false;
    }

    // Memeriksa apakah sesi saat ini sudah dalam keadaan login.
    // Mencegah pengguna yang sudah login untuk melakukan registrasi.
    if (session->isLoggedIn)
    {
        // Mencetak pesan error jika pengguna sudah login.
        printError("Anda sudah login! Silakan logout terlebih dahulu.");
        // Mengembalikan false karena pengguna sudah login.
        return false;
    }

    // Memeriksa apakah format username input valid menggunakan fungsi isValidUsername.
    // Fungsi ini diasumsikan ada dan melakukan validasi format username.
    if (!isValidUsername(inputUsername))
    {
        // Mencetak pesan error jika username tidak memenuhi kriteria validasi.
        printError("Username tidak valid! Hanya boleh berisi huruf, angka, spasi, atau underscore.");
        // Mengembalikan false karena username tidak valid.
        return false;
    }

    // Memeriksa apakah username input sudah terdaftar menggunakan fungsi isUsernameTaken.
    // Fungsi ini diasumsikan ada dan memeriksa keberadaan username di daftar pengguna.
    if (isUsernameTaken(hospital, inputUsername))
    {
        // Mencetak pesan error jika username sudah terdaftar.
        printError("Registrasi gagal! Pasien dengan nama tersebut sudah terdaftar."); // Pesan sesuai spesifikasi F02
        // Mengembalikan false karena username sudah ada.
        return false;
    }

    // Memeriksa apakah panjang password kurang dari 6 karakter.
    // Ini adalah kebijakan keamanan umum untuk memastikan password cukup kuat.
    if (strlen(password) < 6)
    {
        // Mencetak pesan error jika password terlalu pendek.
        printError("Password harus minimal 6 karakter!");
        // Mengembalikan false karena password tidak memenuhi syarat panjang minimum.
        return false;
    }

    // Memeriksa apakah kapasitas untuk pengguna baru atau pasien baru sudah penuh.
    // nEff adalah jumlah elemen efektif, capacity adalah kapasitas maksimum.
    if (hospital->users.nEff >= hospital->users.capacity || hospital->patients.nEff >= hospital->patients.capacity)
    {
        // Mencetak pesan error jika kapasitas penuh.
        printError("Kapasitas pengguna atau pasien penuh!");
        // Mengembalikan false karena tidak ada ruang untuk pengguna/pasien baru.
        return false;
    }

    // Validasi ukuran buffer untuk enkripsi password.
    // Mendapatkan panjang password yang dimasukkan pengguna.
    int passwordLength = strlen(password);
    // Memeriksa apakah panjang password melebihi batas buffer (misalnya, 99 karakter + null terminator).
    if (passwordLength >= 100) // Asumsi buffer enkripsi adalah 100
    {
        // Mencetak pesan error jika password terlalu panjang untuk dienkripsi dengan aman.
        printError("Panjang password terlalu besar untuk dienkripsi!");
        // Mengembalikan false karena potensi buffer overflow.
        return false;
    }

    // Mencari ID maksimum yang sudah ada untuk menghasilkan ID baru yang unik.
    int maxId = 0;
    for (int i = 0; i < hospital->users.nEff; i++)
    {
        if (hospital->users.elements[i].id > maxId)
        {
            maxId = hospital->users.elements[i].id;
        }
    }
    // ID pasien baru adalah ID maksimum yang ada ditambah satu.
    int newPatientId = maxId + 1;

    // Menambahkan pengguna baru ke dalam daftar pengguna rumah sakit.
    // Mengambil pointer ke elemen berikutnya yang tersedia di array users.
    User *newUser = &hospital->users.elements[hospital->users.nEff];
    newUser->id = newPatientId;               // Menetapkan ID pengguna baru.
    strcpy(newUser->username, inputUsername); // Menyalin username input ke pengguna baru.

    // Mengenkripsi password pengguna baru menggunakan fungsi enigmaEncrypt.
    // Fungsi ini diasumsikan ada dan melakukan enkripsi.
    if (!enigmaEncrypt(password, newUser->password.encryptedContent, 100)) // Ukuran buffer 100
    {
        // Mencetak pesan error jika proses enkripsi gagal.
        printError("Gagal mengenkripsi password!");
        // Mengembalikan false karena kegagalan enkripsi.
        return false;
    }
    // Menetapkan peran pengguna baru sebagai PATIENT.
    // PATIENT diasumsikan sebagai nilai enum atau define.
    newUser->role = PATIENT; // Asumsi PATIENT adalah enum atau define

    // Menambahkan pasien baru ke dalam daftar pasien rumah sakit.
    // Mengambil pointer ke elemen berikutnya yang tersedia di array patients.
    Patient *newPatient = &hospital->patients.elements[hospital->patients.nEff];
    newPatient->id = newPatientId;               // Menetapkan ID pasien baru (sama dengan ID pengguna).
    strcpy(newPatient->username, inputUsername); // Menyalin username input ke pasien baru.
    newPatient->bananaRich = 100.0;              // Inisialisasi nilai bananaRich.
    newPatient->life = 3;                        // Inisialisasi jumlah nyawa pasien.
    newPatient->diagnosedStatus = false;         // Inisialisasi status diagnosis.
    newPatient->treatedStatus = false;           // Inisialisasi status perawatan.

    // Mengalokasikan memori untuk daftar resep obat pasien.
    // safeMalloc diasumsikan sebagai fungsi alokasi memori yang aman.
    newPatient->medicationsPrescribed.medicationId = (int *)safeMalloc(10 * sizeof(int));
    if (newPatient->medicationsPrescribed.medicationId == NULL)
    {
        // Mencetak pesan error jika alokasi memori gagal.
        printError("Gagal alokasi memori untuk resep pasien!");
        // Mengembalikan false karena kegagalan alokasi.
        // Sebaiknya juga membersihkan data pengguna yang mungkin sudah ditambahkan sebagian.
        return false;
    }
    newPatient->medicationsPrescribed.capacity = 10; // Menetapkan kapasitas daftar resep.
    newPatient->medicationsPrescribed.nEff = 0;      // Inisialisasi jumlah resep efektif.

    // Mengalokasikan memori untuk daftar obat yang sudah diminum pasien.
    newPatient->medicationsTaken.medicationId = (int *)safeMalloc(10 * sizeof(int));
    if (newPatient->medicationsTaken.medicationId == NULL)
    {
        // Mencetak pesan error jika alokasi memori gagal.
        printError("Gagal alokasi memori untuk obat yang diminum pasien!");
        // Membebaskan memori yang sudah dialokasikan untuk resep jika terjadi kegagalan di sini.
        free(newPatient->medicationsPrescribed.medicationId);
        newPatient->medicationsPrescribed.medicationId = NULL;
        // Mengembalikan false karena kegagalan alokasi.
        return false;
    }
    newPatient->medicationsTaken.capacity = 10; // Menetapkan kapasitas daftar obat yang diminum.
    newPatient->medicationsTaken.top = -1;      // Inisialisasi top untuk stack (jika digunakan sebagai stack).
    newPatient->queueRoom[0] = '\0';            // Inisialisasi nama ruang antrian.
    newPatient->queuePosition = 0;              // Inisialisasi posisi antrian.

    // Meningkatkan jumlah pengguna dan pasien efektif di rumah sakit.
    hospital->users.nEff++;
    hospital->patients.nEff++;

    // Membuat dan mencetak pesan sukses registrasi.
    char successMessage[100] = ""; // Buffer untuk pesan sukses.
    // Menggabungkan string untuk membentuk pesan sukses.
    strcat(successMessage, "Pasien dengan nama ");
    strcat(successMessage, inputUsername);
    strcat(successMessage, " berhasil terdaftar!");
    printSuccess(successMessage); // Mencetak pesan sukses.

    // Mengembalikan true karena registrasi berhasil.
    return true;
}