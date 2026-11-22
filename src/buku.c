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