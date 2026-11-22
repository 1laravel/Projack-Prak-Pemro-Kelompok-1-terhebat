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