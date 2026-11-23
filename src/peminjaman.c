#include <stdio.h>
#include <string.h>
#include "../include/peminjaman.h"
#include "../include/storage.h"

void pinjam_buku(Peminjaman list_pinjam[], int *jumlah_pinjam, Buku list_buku[], int jumlah_buku, const char *username) {
    char cari_ocaid[64];
    char tanggal[20];

    printf("Masukkan ID Buku (ocaid) yang ingin dipinjam: ");
    scanf("%63s", cari_ocaid);

    int posisi_buku = -1;
    for (int i = 0; i < jumlah_buku; i++) {
        if (strcmp(list_buku[i].ocaid, cari_ocaid) == 0) {
            posisi_buku = i;
            break;
        }
    }