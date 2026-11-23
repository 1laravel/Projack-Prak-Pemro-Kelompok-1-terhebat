#ifndef SORTING_H
#define SORTING_H

#include "buku.h"

typedef enum { SORT_BUBBLE = 1, SORT_SELECTION, SORT_INSERTION } AlgoSort;
typedef enum { KEY_OCAID = 1, KEY_TITLE, KEY_YEAR, KEY_PAGE, KEY_STOK } KeySort;
typedef enum { ASC = 1, DESC } Urutan;

/* Mengurutkan katalog langsung */
void urutkan_katalog(Katalog *k, AlgoSort algo, KeySort key, Urutan u);

/* Menampilkan katalog yang telah diurutkan. */
void katalog_tampil_terurut(const Katalog *k, AlgoSort algo,
                            KeySort key, Urutan u, int hanya_tersedia);

#endif
