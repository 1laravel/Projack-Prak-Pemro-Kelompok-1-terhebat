#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buku.h"

void katalog_init(Katalog *k) {
    k->data = NULL;
    k->jumlah = 0;
    k->kapasitas = 0;
}

void katalog_free(Katalog *k) {
    free(k->data);
    k->data = NULL;
    k->jumlah = 0;
    k->kapasitas = 0;
}

int buku_cari_index_by_id(const Katalog *k, const char *ocaid) {
    for (int i = 0; i < k->jumlah; i++) {
        if (strcmp(k->data[i].ocaid, ocaid) == 0)
            return i;
    }
    return -1;
}

int buku_tambah(Katalog *k, Buku b) {
    if (buku_cari_index_by_id(k, b.ocaid) != -1)
        return 0;                       /* ID duplikat */
    if (b.year <= 0 || b.page_count <= 0 || b.stok < 0)
        return 0;                       /* data tidak valid */

    if (k->jumlah == k->kapasitas) {
        int baru = (k->kapasitas == 0) ? 16 : k->kapasitas * 2;
        Buku *tmp = realloc(k->data, baru * sizeof(Buku));
        if (tmp == NULL)
            return 0;
        k->data = tmp;
        k->kapasitas = baru;
    }

    k->data[k->jumlah] = b;
    k->jumlah++;
    return 1;
}

int buku_hapus(Katalog *k, const char *ocaid) {
    int idx = buku_cari_index_by_id(k, ocaid);
    if (idx == -1)
        return 0;

    for (int i = idx; i < k->jumlah - 1; i++)
        k->data[i] = k->data[i + 1];

    k->jumlah--;
    return 1;
}

int buku_update(Katalog *k, const char *ocaid, Buku baru) {
    int idx = buku_cari_index_by_id(k, ocaid);
    if (idx == -1)
        return 0;
    if (baru.year <= 0 || baru.page_count <= 0 || baru.stok < 0)
        return 0;

    strcpy(baru.ocaid, k->data[idx].ocaid);   /* ID tidak boleh berubah */
    k->data[idx] = baru;
    return 1;
}

static void cetak_header(void) {
    printf("%-4s %-22s %-34s %-22s %-6s %-6s %-5s\n",
           "No", "ID (ocaid)", "Judul", "Pengarang",
           "Tahun", "Hal", "Stok");
    printf("--------------------------------------------------------------"
           "-----------------------------------\n");
}

static void cetak_baris(int no, const Buku *b) {
    printf("%-4d %-22.22s %-34.34s %-22.22s %-6d %-6d %-5d\n",
           no, b->ocaid, b->title, b->author,
           b->year, b->page_count, b->stok);
}

void buku_tampil_semua(const Katalog *k) {
    if (k->jumlah == 0) {
        printf("Katalog kosong.\n");
        return;
    }
    cetak_header();
    for (int i = 0; i < k->jumlah; i++)
        cetak_baris(i + 1, &k->data[i]);
    printf("Total: %d buku\n", k->jumlah);
}

void buku_tampil_tersedia(const Katalog *k) {
    int no = 0;
    for (int i = 0; i < k->jumlah; i++) {
        if (k->data[i].stok > 0) {
            if (no == 0)
                cetak_header();
            no++;
            cetak_baris(no, &k->data[i]);
        }
    }
    if (no == 0)
        printf("Tidak ada buku yang tersedia.\n");
    else
        printf("Total: %d buku tersedia\n", no);
}