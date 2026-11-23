#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "auth.h"
#include "buku.h"
#include "sorting.h"
#include "peminjaman.h"
#include "storage.h"
#include "utils.h"


static Katalog    katalog;
static Peminjaman daftarPinjam[MAX_PINJAM];
static int        jumlahPinjam = 0;

/* ================= Helper tampilan & input ================= */

static void garis(void)
{
    printf("========================================\n");
}

static void bersihkanBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* Baca satu baris teks. Return 0 jika EOF. */
static int bacaTeks(const char *prompt, char *out, size_t n)
{
    size_t len;
    printf("%s", prompt);
    if (!fgets(out, (int)n, stdin)) { out[0] = '\0'; return 0; }
    len = strcspn(out, "\r\n");
    if (out[len] == '\0') bersihkanBuffer();   /* baris kepanjangan: buang sisanya */
    out[len] = '\0';
    return 1;
}

/* Teks wajib diisi, tanpa tanda petik (format file memakai "..."). */
static void bacaWajib(const char *prompt, char *out, size_t n, int tanpaSpasi)
{
    for (;;) {
        if (!bacaTeks(prompt, out, n)) { out[0] = '\0'; return; }
        if (out[0] != '\0' && strchr(out, '"') == NULL &&
            (!tanpaSpasi || strpbrk(out, " \t") == NULL))
            return;
        printf("  [!] Isi tidak valid (kosong, mengandung tanda petik%s).\n",
               tanpaSpasi ? " atau spasi" : "");
    }
}

static int bacaInt(const char *prompt, int min, int max)
{
    char buf[64], *end;
    long v;
    for (;;) {
        if (!bacaTeks(prompt, buf, sizeof buf)) return min;   /* EOF -> pilihan terendah */
        v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && v >= min && v <= max)
            return (int)v;
        printf("  [!] Input tidak valid. Masukkan angka %d-%d.\n", min, max);
    }
}

/* Untuk update: Enter kosong = pertahankan nilai lama */
static void ubahTeks(const char *label, char *nilai, size_t n)
{
    char prompt[256], buf[256];
    snprintf(prompt, sizeof prompt, "%s [%.100s]: ", label, nilai);
    if (!bacaTeks(prompt, buf, sizeof buf) || buf[0] == '\0') return;
    if (strchr(buf, '"') != NULL) {
        printf("  [!] Tanda petik tidak diizinkan, nilai lama dipertahankan.\n");
        return;
    }
    strncpy(nilai, buf, n - 1);
    nilai[n - 1] = '\0';
}

static int ubahAngka(const char *label, int nilai, int min, int max)
{
    char prompt[128], buf[64], *end;
    long v;
    snprintf(prompt, sizeof prompt, "%s [%d]: ", label, nilai);
    for (;;) {
        if (!bacaTeks(prompt, buf, sizeof buf) || buf[0] == '\0') return nilai;
        v = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && v >= min && v <= max) return (int)v;
        printf("  [!] Masukkan angka %d-%d.\n", min, max);
    }
}

static void tungguEnter(void)
{
    char buf[8];
    printf("\nTekan ENTER untuk kembali ke menu");
    if (!fgets(buf, sizeof buf, stdin)) return;
    if (strchr(buf, '\n') == NULL) bersihkanBuffer();
}

/* ================= Data ================= */

static void muatData(void)
{
    int n = 0, diterima = 0;
    Buku *tmp = malloc(MAX_BUKU * sizeof(Buku));

    katalog_init(&katalog);

    if (tmp == NULL) {
        printf("[!] Memori tidak cukup untuk memuat data buku.\n");
    } else {
        n = load_books(FILE_BUKU, tmp, MAX_BUKU);
        for (int i = 0; i < n; i++)
            diterima += buku_tambah(&katalog, tmp[i]);
        free(tmp);
        if (diterima < n)
            printf("[!] %d baris buku dilewati (ID ganda / data tidak valid).\n", n - diterima);
    }

    jumlahPinjam = load_peminjaman(FILE_PEMINJAMAN, daftarPinjam, MAX_PINJAM);
    printf("Data dimuat: %d buku, %d transaksi.\n", katalog.jumlah, jumlahPinjam);
}

static void simpanBuku(void)
{
    save_books(FILE_BUKU, katalog.data, katalog.jumlah);
}

static int hitungAktif(const char *username)
{
    int n = 0;
    for (int i = 0; i < jumlahPinjam; i++)
        if (strcmp(daftarPinjam[i].username, username) == 0 &&
            strcmp(daftarPinjam[i].status, "DIPINJAM") == 0)
            n++;
    return n;
}

/* ================= Aksi bersama ================= */

/* Pilih atribut, arah, dan algoritma sorting. Return 0 jika dibatalkan. */
static int pilihSorting(AlgoSort *algo, KeySort *key, Urutan *urut)
{
    static const KeySort  keys[]  = {KEY_OCAID, KEY_TITLE, KEY_YEAR, KEY_PAGE, KEY_STOK};
    static const AlgoSort algos[] = {SORT_INSERTION, SORT_SELECTION, SORT_BUBBLE};
    const char *lblAsc[]  = {"", "ID Kecil -> Besar", "Judul A-Z",
                             "Tahun Terlama -> Terbaru",
                             "Halaman Tipis -> Tebal", "Stok Sedikit -> Banyak"};
    const char *lblDesc[] = {"", "ID Besar -> Kecil", "Judul Z-A",
                             "Tahun Terbaru -> Terlama",
                             "Halaman Tebal -> Tipis", "Stok Banyak -> Sedikit"};
    int k, o, a;

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

    printf("\n--- ALGORITMA SORTING ---\n");
    printf("1. Insertion Sort\n2. Selection Sort\n3. Bubble Sort\n");
    a = bacaInt("Pilih algoritma: ", 1, 3);

    *key  = keys[k - 1];
    *urut = (o == 1) ? ASC : DESC;
    *algo = algos[a - 1];
    return 1;
}

static void aksiLihatKatalog(int hanyaTersedia)
{
    AlgoSort algo;
    KeySort  key;
    Urutan   urut;

    if (!pilihSorting(&algo, &key, &urut)) return;
    printf("\n");
    katalog_tampil_terurut(&katalog, algo, key, urut, hanyaTersedia);
    tungguEnter();
}

/* ================= Aksi admin ================= */

static void aksiTambahBuku(const char *username)
{
    Buku b;
    memset(&b, 0, sizeof b);

    printf("\n--- TAMBAH BUKU ---\n");
    bacaWajib("ID buku (ocaid) : ", b.ocaid, sizeof b.ocaid, 1);
    if (b.ocaid[0] != '\0' && buku_cari_index_by_id(&katalog, b.ocaid) != -1) {
        printf("  [!] ID '%s' sudah ada.\n", b.ocaid);
        tungguEnter();
        return;
    }
    bacaWajib("Judul           : ", b.title,  sizeof b.title,  0);
    bacaWajib("Pengarang       : ", b.author, sizeof b.author, 0);
    b.year       = bacaInt("Tahun terbit    : ", 1, 9999);
    b.page_count = bacaInt("Jumlah halaman  : ", 1, 100000);
    b.stok       = bacaInt("Stok            : ", 0, 100000);

    if (b.ocaid[0] == '\0' || b.title[0] == '\0' || b.author[0] == '\0') {
        printf("  [!] Data tidak lengkap, penambahan dibatalkan.\n");
    } else if (buku_tambah(&katalog, b)) {
        simpanBuku();
        logActivity(username, "TAMBAH_BUKU", b.ocaid);
        printf("  [OK] Buku '%s' berhasil ditambahkan.\n", b.title);
    } else {
        printf("  [!] Gagal menambah buku (ID ganda atau data tidak valid).\n");
    }
    tungguEnter();
}

static void aksiUpdateBuku(const char *username)
{
    char id[64];
    int idx;
    Buku baru;

    printf("\n--- UPDATE BUKU ---\n");
    bacaTeks("ID buku yang diubah: ", id, sizeof id);
    idx = buku_cari_index_by_id(&katalog, id);
    if (idx == -1) {
        printf("  [!] Buku dengan ID \"%s\" tidak ditemukan.\n", id);
        tungguEnter();
        return;
    }

    baru = katalog.data[idx];
    printf("Kosongkan input (ENTER) untuk mempertahankan nilai lama.\n");
    ubahTeks("Judul", baru.title,  sizeof baru.title);
    ubahTeks("Pengarang", baru.author, sizeof baru.author);
    baru.year       = ubahAngka("Tahun terbit",   baru.year,       1, 9999);
    baru.page_count = ubahAngka("Jumlah halaman", baru.page_count, 1, 100000);
    baru.stok       = ubahAngka("Stok",           baru.stok,       0, 100000);

    if (buku_update(&katalog, id, baru)) {
        simpanBuku();
        logActivity(username, "UPDATE_BUKU", id);
        printf("  [OK] Data buku berhasil diperbarui.\n");
    } else {
        printf("  [!] Gagal memperbarui buku (data tidak valid).\n");
    }
    tungguEnter();
}

static void aksiHapusBuku(const char *username)
{
    char id[64], yakin[16];
    int idx, aktif = 0;

    printf("\n--- HAPUS BUKU ---\n");
    bacaTeks("ID buku yang dihapus: ", id, sizeof id);
    idx = buku_cari_index_by_id(&katalog, id);
    if (idx == -1) {
        printf("  [!] Buku dengan ID \"%s\" tidak ditemukan.\n", id);
        tungguEnter();
        return;
    }

    for (int i = 0; i < jumlahPinjam; i++)
        if (strcmp(daftarPinjam[i].ocaid, id) == 0 &&
            strcmp(daftarPinjam[i].status, "DIPINJAM") == 0)
            aktif++;
    if (aktif > 0) {
        printf("  [!] Buku masih dipinjam %d orang. Tidak bisa dihapus.\n", aktif);
        tungguEnter();
        return;
    }

    printf("Buku: %s\n", katalog.data[idx].title);
    bacaTeks("Yakin hapus? (y/n): ", yakin, sizeof yakin);
    if (yakin[0] == 'y' || yakin[0] == 'Y') {
        if (buku_hapus(&katalog, id)) {
            simpanBuku();
            logActivity(username, "HAPUS_BUKU", id);
            printf("  [OK] Buku berhasil dihapus.\n");
        }
    } else {
        printf("  Dibatalkan.\n");
    }
    tungguEnter();
}

static void aksiLaporan(void)
{
    laporan_transaksi_global(daftarPinjam, jumlahPinjam);
    tungguEnter();
}

/* ================= Aksi member ================= */

static void aksiCariBuku(void)
{
    char kw[128];
    int p;

    printf("\n--- CARI BUKU ---\n");
    printf("1. Berdasarkan ID (ocaid)\n2. Berdasarkan judul\n0. Batal\n");
    p = bacaInt("Pilih: ", 0, 2);
    if (p == 0) return;

    bacaTeks(p == 1 ? "Masukkan ID: " : "Masukkan kata kunci judul: ", kw, sizeof kw);
    if (kw[0] == '\0') {
        printf("  [!] Kata kunci tidak boleh kosong.\n");
    } else if (p == 1) {
        int idx = buku_cari_index_by_id(&katalog, kw);
        if (idx == -1) {
            printf("Buku dengan ID \"%s\" tidak ditemukan.\n", kw);
        } else {
            const Buku *b = &katalog.data[idx];
            printf("\n  ID        : %s\n  Judul     : %s\n  Pengarang : %s\n"
                   "  Tahun     : %d\n  Halaman   : %d\n  Stok      : %d (%s)\n",
                   b->ocaid, b->title, b->author, b->year, b->page_count,
                   b->stok, b->stok > 0 ? "tersedia" : "habis");
        }
    } else {
        printf("\n");
        buku_cari_by_judul(&katalog, kw);
    }
    tungguEnter();
}

static void aksiPinjam(const char *username)
{
    int sebelum = jumlahPinjam;

    if (jumlahPinjam >= MAX_PINJAM) {
        printf("  [!] Data peminjaman penuh.\n");
        tungguEnter();
        return;
    }
    printf("\n--- PINJAM BUKU ---\n");
    pinjam_buku(daftarPinjam, &jumlahPinjam, katalog.data, katalog.jumlah, username);
    bersihkanBuffer();   /* scanf di peminjaman.c menyisakan '\n' */

    if (jumlahPinjam > sebelum)
        logActivity(username, "PINJAM_BUKU", daftarPinjam[jumlahPinjam - 1].ocaid);
    tungguEnter();
}

static void aksiKembalikan(const char *username)
{
    int sebelum = hitungAktif(username);

    printf("\n--- KEMBALIKAN BUKU ---\n");
    if (sebelum == 0) {
        printf("Anda tidak memiliki buku yang sedang dipinjam.\n");
        tungguEnter();
        return;
    }
    lihat_peminjaman_pribadi(daftarPinjam, jumlahPinjam, username);
    printf("\n");
    kembalikan_buku(daftarPinjam, jumlahPinjam, katalog.data, katalog.jumlah, username);
    bersihkanBuffer();

    if (hitungAktif(username) < sebelum)
        logActivity(username, "KEMBALI_BUKU", "pengembalian buku");
    tungguEnter();
}

static void aksiPinjamanSaya(const char *username)
{
    lihat_peminjaman_pribadi(daftarPinjam, jumlahPinjam, username);
    tungguEnter();
}

/* ================= Menu ================= */

static void menuAdmin(const char *username)
{
    int pilih;

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
        case 1: aksiTambahBuku(username);  break;
        case 2: aksiLihatKatalog(0);       break;
        case 3: aksiUpdateBuku(username);  break;
        case 4: aksiHapusBuku(username);   break;
        case 5: aksiLaporan();             break;
        case 0: printf("\nLogout berhasil.\n"); break;
        }
    } while (pilih != 0);
}

static void menuUser(const char *username)
{
    int pilih;

    do {
        printf("\n");
        garis();
        printf("   MENU MEMBER (Anggota) - %s\n", username);
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
        case 1: aksiLihatKatalog(1);          break;
        case 2: aksiCariBuku();               break;
        case 3: aksiPinjam(username);         break;
        case 4: aksiKembalikan(username);     break;
        case 5: aksiPinjamanSaya(username);   break;
        case 0: printf("\nLogout berhasil.\n"); break;
        }
    } while (pilih != 0);
}

/* Navigasi berdasarkan role (sama dengan main.c: admin, selain itu member) */
void menuUtama(const User *akun)
{
    if (akun == NULL) return;

    muatData();
    logActivity(akun->username, "LOGIN", akun->role);

    if (strcmp(akun->role, "admin") == 0)
        menuAdmin(akun->username);
    else
        menuUser(akun->username);

    logActivity(akun->username, "LOGOUT", "-");
    katalog_free(&katalog);
}