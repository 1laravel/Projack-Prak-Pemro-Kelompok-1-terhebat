#ifndef BUKU_H
#define BUKU_H

typedef struct {
    char ocaid[64];
    char title[256];
    char author[128];
    int  year;
    int  page_count;
    int  stok;
} Buku;
 
typedef struct {
    Buku *data;
    int   jumlah;
    int   kapasitas;
} Katalog;

/* Manajemen memori */
void katalog_init(Katalog *k);
void katalog_free(Katalog *k);

/* CRUD (hanya mengubah data di memori) */
int  buku_tambah(Katalog *k, Buku b);
int  buku_update(Katalog *k, const char *ocaid, Buku baru);
int  buku_hapus(Katalog *k, const char *ocaid);

/* Pencarian */
int  buku_cari_index_by_id(const Katalog *k, const char *ocaid);
void buku_cari_by_judul(const Katalog *k, const char *keyword);

/* Tampilan */
void buku_tampil_semua(const Katalog *k);      /* admin: semua buku */
void buku_tampil_tersedia(const Katalog *k);   /* user: stok > 0 */

#endif