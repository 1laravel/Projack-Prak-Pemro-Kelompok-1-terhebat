#include <string.h>
#include <ctype.h>
#include "sorting.h"

/* Bandingkan dua teks tanpa peduli huruf besar/kecil */
static int strcmp_ci(const char *a, const char *b) {
    while (*a && *b) {
        int ca = tolower((unsigned char)*a);
        int cb = tolower((unsigned char)*b);
        if (ca != cb)
            return ca - cb;
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

/* <0: a < b, 0: sama, >0: a > b */
static int bandingkan(const Buku *a, const Buku *b, KeySort key) {
    switch (key) {
        case KEY_OCAID: return strcmp(a->ocaid, b->ocaid);
        case KEY_TITLE: return strcmp_ci(a->title, b->title);
        case KEY_YEAR:  return a->year - b->year;
        case KEY_PAGE:  return a->page_count - b->page_count;
        case KEY_STOK:  return a->stok - b->stok;
    }
    return 0;
}

/* Apakah a seharusnya berada SETELAH b? */
static int perlu_tukar(const Buku *a, const Buku *b, KeySort key, Urutan u) {
    int c = bandingkan(a, b, key);
    return (u == ASC) ? (c > 0) : (c < 0);
}

/* ---------- Insertion Sort ---------- */
static void insertion_sort(Buku *a, int n, KeySort key, Urutan u) {
    for (int i = 1; i < n; i++) {
        Buku kunci = a[i];
        int j = i - 1;
        while (j >= 0 && perlu_tukar(&a[j], &kunci, key, u)) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = kunci;
    }
}

/* ---------- Selection Sort ---------- */
static void selection_sort(Buku *a, int n, KeySort key, Urutan u) {
    for (int i = 0; i < n - 1; i++) {
        int pilih = i;
        for (int j = i + 1; j < n; j++) {
            if (perlu_tukar(&a[pilih], &a[j], key, u))
                pilih = j;
        }
        if (pilih != i) {
            Buku tmp = a[i];
            a[i] = a[pilih];
            a[pilih] = tmp;
        }
    }
}

/* ---------- Bubble Sort ---------- */
static void bubble_sort(Buku *a, int n, KeySort key, Urutan u) {
    for (int i = 0; i < n - 1; i++) {
        int tukar = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (perlu_tukar(&a[j], &a[j + 1], key, u)) {
                Buku tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
                tukar = 1;
            }
        }
        if (!tukar)
            break;
    }
}

/* ---------- Fungsi publik ---------- */
void urutkan_katalog(Katalog *k, AlgoSort algo, KeySort key, Urutan u) {
    switch (algo) {
        case SORT_INSERTION: insertion_sort(k->data, k->jumlah, key, u); break;
        case SORT_SELECTION: selection_sort(k->data, k->jumlah, key, u); break;
        case SORT_BUBBLE:    bubble_sort(k->data, k->jumlah, key, u); break;
        default: break;
    }
}