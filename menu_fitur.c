/* ============================================================
 *  BRANCH: FITUR MENU  (Sistem Manajemen Perpustakaan CLI)
 *  Isi: navigasi menu Admin/User + submenu pilihan sorting.
 *
 *  CATATAN INTEGRASI:
 *  - Bagian "TIPE DATA" & "PROTOTIPE" di bawah: hapus yang sudah
 *    ada di main/branch lain agar tidak double definisi.
 *  - Panggil dari main setelah login:  menuUtama(&akun);
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------- TIPE DATA (samakan dengan tim) ---------------- */
/* ====== Struct bersama (kontrak antar branch). Jangan diubah sepihak ====== */
typedef struct {
    char ocaid[64];
    char title[256];
    char author[256];
    char publisher[128];
    int  year;
    int  page_count;
    int  stock;
} Book;

typedef struct {
    char username[32];
    char password[32];
    char role[12];          /* "admin" / "user" */
} Account;

typedef struct {
    unsigned int id_peminjaman;
    char username[32];
    char ocaid[64];
    char tanggal_pinjam[20]; /* DD-MM-YYYY */
    char status[20];         /* DIPINJAM / KEMBALI */
} Loan;

/* ====== Opsi sorting ====== */
typedef enum {
    SORT_ID = 1,
    SORT_TITLE,
    SORT_YEAR,
    SORT_PAGE_COUNT,
    SORT_STOCK
} SortKey;

typedef enum { ORDER_ASC = 1, ORDER_DESC = 2 } SortOrder;

/* ---------------- PROTOTIPE ---------------- */
/* ===== API milik branch MENU ===== */
/* Dipanggil dari main() setelah login berhasil. Return saat user logout. */
void menuUtama(const Account *akun);

void menuAdmin(const char *username);
void menuUser(const char *username);

/* Submenu pemilihan algoritma sorting. Return 1 jika dipilih, 0 jika batal. */
int  menuSorting(SortKey *key, SortOrder *order);

/* ===== Fungsi yang HARUS disediakan branch lain (dipanggil oleh menu) ===== */
/* -- Admin (CRUD & laporan) -- */
void tambahBuku(void);
void tampilkanSemuaBuku(SortKey key, SortOrder order);
void updateBuku(void);
void hapusBuku(void);
void laporanTransaksi(void);

/* -- User -- */
void tampilkanKatalogTersedia(SortKey key, SortOrder order);
void cariBuku(void);
void pinjamBuku(const char *username);
void kembalikanBuku(const char *username);
void daftarPinjamanPribadi(const char *username);

/* ---------------- IMPLEMENTASI MENU ---------------- */

/* ---------- Helper input aman (fgets + strtol) ---------- */
static int bacaInt(const char *prompt, int min, int max)
{
    char buf[64], *end;
    long v;
    for (;;) {
        printf("%s", prompt);
        if (!fgets(buf, sizeof buf, stdin)) return min;   /* EOF -> pilihan terendah */
        v = strtol(buf, &end, 10);
        if (end != buf && (*end == '\n' || *end == '\0') && v >= min && v <= max)
            return (int)v;
        printf("  [!] Input tidak valid. Masukkan angka %d-%d.\n", min, max);
    }
}

static void tungguEnter(void)
{
    char buf[8];
    printf("\nTekan ENTER untuk kembali ke menu...");
    fgets(buf, sizeof buf, stdin);
}

static void garis(void)
{
    printf("========================================\n");
}

/* ---------- Submenu sorting ---------- */
int menuSorting(SortKey *key, SortOrder *order)
{
    int k, o;
    const char *lblAsc[]  = {"", "ID Kecil -> Besar", "Judul A-Z",
                             "Tahun Terlama -> Terbaru",
                             "Halaman Tipis -> Tebal", "Stok Sedikit -> Banyak"};
    const char *lblDesc[] = {"", "ID Besar -> Kecil", "Judul Z-A",
                             "Tahun Terbaru -> Terlama",
                             "Halaman Tebal -> Tipis", "Stok Banyak -> Sedikit"};

    printf("\n--- PILIH URUTAN KATALOG ---\n");
    printf("1. ID Buku (ocaid)\n");
    printf("2. Judul Buku (title)\n");
    printf("3. Tahun Terbit (year)\n");
    printf("4. Ketebalan Halaman (page_count)\n");
    printf("5. Stok Ketersediaan\n");
    printf("0. Batal\n");
    k = bacaInt("Pilih atribut: ", 0, 5);
    if (k == 0) return 0;

    printf("\n1. %s\n2. %s\n", lblAsc[k], lblDesc[k]);
    o = bacaInt("Pilih arah: ", 1, 2);

    *key   = (SortKey)k;
    *order = (SortOrder)o;
    return 1;
}

/* ---------- Menu Admin ---------- */
void menuAdmin(const char *username)
{
    int pilih;
    SortKey key;
    SortOrder order;

    do {
        printf("\n");
        garis();
        printf("   MENU ADMIN (Pustakawan) - %s\n", username);
        garis();
        printf("1. Tambah Buku\n");
        printf("2. Lihat Katalog Buku (+ sorting)\n");
        printf("3. Update Data Buku\n");
        printf("4. Hapus Buku\n");
        printf("5. Laporan Transaksi Peminjaman\n");
        printf("0. Logout\n");
        garis();
        pilih = bacaInt("Pilihan: ", 0, 5);

        switch (pilih) {
        case 1: tambahBuku();       tungguEnter(); break;
        case 2:
            if (menuSorting(&key, &order)) {
                tampilkanSemuaBuku(key, order);
                tungguEnter();
            }
            break;
        case 3: updateBuku();       tungguEnter(); break;
        case 4: hapusBuku();        tungguEnter(); break;
        case 5: laporanTransaksi(); tungguEnter(); break;
        case 0: printf("\nLogout berhasil.\n"); break;
        }
    } while (pilih != 0);
}

/* ---------- Menu User ---------- */
void menuUser(const char *username)
{
    int pilih;
    SortKey key;
    SortOrder order;

    do {
        printf("\n");
        garis();
        printf("   MENU USER (Anggota) - %s\n", username);
        garis();
        printf("1. Lihat Katalog Tersedia (+ sorting)\n");
        printf("2. Cari Buku (ID / Judul)\n");
        printf("3. Pinjam Buku\n");
        printf("4. Kembalikan Buku\n");
        printf("5. Daftar Peminjaman Saya\n");
        printf("0. Logout\n");
        garis();
        pilih = bacaInt("Pilihan: ", 0, 5);

        switch (pilih) {
        case 1:
            if (menuSorting(&key, &order)) {
                tampilkanKatalogTersedia(key, order);
                tungguEnter();
            }
            break;
        case 2: cariBuku();                     tungguEnter(); break;
        case 3: pinjamBuku(username);           tungguEnter(); break;
        case 4: kembalikanBuku(username);       tungguEnter(); break;
        case 5: daftarPinjamanPribadi(username);tungguEnter(); break;
        case 0: printf("\nLogout berhasil.\n"); break;
        }
    } while (pilih != 0);
}

/* ---------- Navigasi berdasarkan role ---------- */
void menuUtama(const Account *akun)
{
    if (akun == NULL) return;

    if (strcmp(akun->role, "admin") == 0)
        menuAdmin(akun->username);
    else if (strcmp(akun->role, "user") == 0)
        menuUser(akun->username);
    else
        printf("[!] Role '%s' tidak dikenali. Akses ditolak.\n", akun->role);
}
