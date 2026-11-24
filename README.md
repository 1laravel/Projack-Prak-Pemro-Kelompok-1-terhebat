# Sistem Manajemen Perpustakaan Berbasis CLI

Project Praktikum Pemrograman — **Kelompok 1**  
Informatika FMIPA Universitas Syiah Kuala, 2026.

Aplikasi manajemen perpustakaan berbasis **Command Line Interface (CLI)** yang dibangun secara modular menggunakan bahasa C. Mendukung manajemen hak akses berbasis *role* (Admin dan Member), katalog buku dinamis, pencarian cepat, pengurutan multi-algoritma, sirkulasi peminjaman, penyimpanan berbasis file teks, serta pencatatan *activity log*.

---

## Anggota Kelompok

| No | Nama Lengkap | NPM |
| :-: | :--- | :--- |
| 1 | Muhammad Aufa Zaki | `260810701100046` |
| 2 | Niswatul Afifah Kluet | `260810701100004` |
| 3 | Faiza Muzkia | `260810701100010` |
| 4 | Diah Aliefa | `260810701100011` |

---

## Fitur Utama

### Autentikasi & Hak Akses
* Login berbasis file (`data/user.txt`) dengan batas maksimal **3 kali percobaan**.
* Pembagian menu otomatis sesuai *role* akun:
  * **Admin (Pustakawan)**: Tambah buku, lihat katalog (+ sorting), update data buku (tekan `ENTER` untuk mempertahankan nilai lama), hapus buku (terproteksi jika buku masih dipinjam), dan lihat laporan seluruh transaksi peminjaman.
  * **Member (Anggota)**: Lihat katalog buku yang tersedia (`stok > 0`, + sorting), cari buku berdasarkan ID (`ocaid`) atau kata kunci judul (*case-insensitive*), pinjam buku, kembalikan buku, dan lihat daftar peminjaman pribadi.

### Pengurutan Katalog (Sorting)
* Mengurutkan katalog secara **Ascending** maupun **Descending** berdasarkan 5 atribut:
  1. ID Buku (`ocaid`)
  2. Judul Buku (`title`)
  3. Tahun Terbit (`year`)
  4. Jumlah Halaman (`page_count`)
  5. Stok Ketersediaan (`stok`)
* Mendukung 3 algoritma yang diimplementasikan secara manual: **Insertion Sort**, **Selection Sort**, dan **Bubble Sort**.
* Hasil pengurutan langsung disimpan secara permanen ke `data/buku.txt`.

### Sirkulasi & Activity Log
* Pembaruan stok otomatis secara *real-time* setiap kali terjadi peminjaman atau pengembalian buku.
* Normalisasi otomatis format tanggal peminjaman (`DD-MM-YYYY`) agar struktur kolom file tetap aman.
* Pencatatan otomatis setiap aktivitas penting (`LOGIN`, `LOGOUT`, `LOGIN_FAILED`, `TAMBAH_BUKU`, `UPDATE_BUKU`, `HAPUS_BUKU`, `PINJAM_BUKU`, `KEMBALI_BUKU`) ke dalam `data/log_activity.txt`.

---

## Arsitektur Program

### Alur Eksekusi (`src/main.c` → `src/menu.c`)
1. Pengguna melakukan autentikasi melalui `promptLogin()` (`src/auth.c`).
2. Sistem memuat data katalog dari `data/buku.txt` (jika belum ada/kosong, otomatis melakukan *fallback* membaca dataset mentah `data/Books.txt` dengan stok awal default `5`) serta memuat riwayat transaksi dari `data/peminjaman.txt`.
3. Program mengarahkan pengguna ke `menuAdmin()` atau `menuUser()` berdasarkan `role`.
4. Setiap transaksi dan perubahan data langsung ditulis ke file dan dicatat ke `data/log_activity.txt`.
5. Saat pengguna logout, seluruh memori dinamis katalog dibebaskan melalui `katalog_free()`.

### Struktur Data Utama
* `User` (`include/auth.h`): `username`, `password`, `role`
* `Buku` (`include/buku.h`): `ocaid`, `title`, `author`, `year`, `page_count`, `stok`
* `Katalog` (`include/buku.h`): Array dinamis `Buku *data` beserta `jumlah` dan `kapasitas` yang membesar otomatis menggunakan `realloc`
* `Peminjaman` (`include/peminjaman.h`): `id_peminjaman`, `username`, `ocaid`, `tanggal_pinjam`, `status` (`DIPINJAM` / `KEMBALI`)

---

## Struktur Direktori

```text
.
├── Makefile
├── README.md
├── data/
│   ├── Books.txt          # Dataset buku mentah awal
│   ├── buku.txt           # Katalog buku rapi + stok (hasil load & sorting)
│   ├── user.txt           # Data akun (format: username|password|role)
│   ├── peminjaman.txt     # Riwayat transaksi peminjaman & pengembalian
│   └── log_activity.txt   # Catatan aktivitas pengguna
├── include/
│   ├── auth.h
│   ├── buku.h
│   ├── menu.h
│   ├── peminjaman.h
│   ├── sorting.h
│   ├── storage.h
│   └── utils.h
└── src/
    ├── main.c
    ├── auth.c
    ├── buku.c
    ├── menu.c
    ├── peminjaman.c
    ├── sorting.c
    ├── storage.c
    └── utils.c
```

---

## Cara Compile & Menjalankan

> **Penting:** Wajib dijalankan dari **root direktori project** (folder yang sejajar dengan `Makefile`, `src/`, `include/`, dan `data/`) agar seluruh file di dalam `data/` dapat terbaca.

### 1. Menggunakan Makefile

**Windows (MinGW):**
```powershell
mingw32-make
.\perpustakaan.exe
```

**Linux / macOS:**
```bash
make
./perpustakaan
```

### 2. Compile Manual (Tanpa Makefile)

**Windows (PowerShell / CMD dengan MinGW GCC):**
```powershell
gcc -Wall -Wextra -Iinclude src/main.c src/auth.c src/buku.c src/menu.c src/peminjaman.c src/sorting.c src/storage.c src/utils.c -o perpustakaan.exe
.\perpustakaan.exe
```

**Linux / macOS:**
```bash
gcc -Wall -Wextra -Iinclude src/*.c -o perpustakaan
./perpustakaan
```

---

## Format File Data

| File | Format Baris | Contoh |
| :--- | :--- | :--- |
| `data/user.txt` | `username\|password\|role` | `admin\|admin123\|admin` |
| `data/buku.txt` | `ocaid "title" "author" year page_count stok` | `worksofcharlesle01leve "The works..." "Lever..." 1872 692 5` |
| `data/peminjaman.txt` | `id username ocaid tanggal status` | `1 diah goldenwestpoems00crabrich 19-11-2026 DIPINJAM` |
| `data/log_activity.txt` | `username \| action \| detail` | `admin \| LOGIN \| admin` |