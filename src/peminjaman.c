#include <stdio.h>
#include <string.h>
#include "peminjaman.h"
#include "storage.h"
#include "menu.h"

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
    if (posisi_buku == -1) {
        printf("Buku dengan ID '%s' tidak ditemukan!\n", cari_ocaid);
        return;
    }

    if (list_buku[posisi_buku].stok <= 0) {
        printf("Maaf, stok buku '%s' habis.\n", list_buku[posisi_buku].title);
        return;
    }

    printf("Masukkan Tanggal Pinjam (DD-MM-YYYY): ");
    scanf("%19s", tanggal);

    int index_baru = *jumlah_pinjam;
    if (index_baru == 0) {
        list_pinjam[index_baru].id_peminjaman = 1;
    } else {
        list_pinjam[index_baru].id_peminjaman = list_pinjam[index_baru - 1].id_peminjaman + 1;
    }

    strcpy(list_pinjam[index_baru].username, username);
    strcpy(list_pinjam[index_baru].ocaid, cari_ocaid);
    strcpy(list_pinjam[index_baru].tanggal_pinjam, tanggal);
    strcpy(list_pinjam[index_baru].status, "DIPINJAM");

    *jumlah_pinjam = *jumlah_pinjam + 1;
    list_buku[posisi_buku].stok = list_buku[posisi_buku].stok - 1;

    save_books(FILE_BUKU, list_buku, jumlah_buku);
    save_peminjaman(FILE_PEMINJAMAN, list_pinjam, *jumlah_pinjam);

    printf("Berhasil meminjam buku '%s'! ID Peminjaman Anda: %u\n",
           list_buku[posisi_buku].title,
           list_pinjam[index_baru].id_peminjaman); }

void kembalikan_buku(Peminjaman list_pinjam[], int jumlah_pinjam, Buku list_buku[], int jumlah_buku, const char *username) {
    unsigned int cari_id;
    printf("Masukkan ID Peminjaman yang akan dikembalikan: ");
    scanf("%u", &cari_id);

    int posisi_pinjam = -1;
    for (int i = 0; i < jumlah_pinjam; i++) {
        if (list_pinjam[i].id_peminjaman == cari_id &&
            strcmp(list_pinjam[i].username, username) == 0 &&
            strcmp(list_pinjam[i].status, "DIPINJAM") == 0) {
            posisi_pinjam = i;
            break;
        }
    }

    if (posisi_pinjam == -1) {
        printf("Data peminjaman tidak ditemukan atau buku sudah dikembalikan.\n");
    return;    
    }
        strcpy(list_pinjam[posisi_pinjam].status, "KEMBALI");

    for (int i = 0; i < jumlah_buku; i++) {
        if (strcmp(list_buku[i].ocaid, list_pinjam[posisi_pinjam].ocaid) == 0) {
            list_buku[i].stok = list_buku[i].stok + 1;
            break;
        }
    }

    save_books(FILE_BUKU, list_buku, jumlah_buku);
    save_peminjaman(FILE_PEMINJAMAN, list_pinjam, jumlah_pinjam);

    printf("Buku berhasil dikembalikan. Terima kasih!\n");
}
void lihat_peminjaman_pribadi(const Peminjaman list_pinjam[], int jumlah_pinjam, const char *username) {
    printf("\n=== DAFTAR PEMINJAMAN ANDA (%s) ===\n", username);
    int ada = 0;

    for (int i = 0; i < jumlah_pinjam; i++) {
        if (strcmp(list_pinjam[i].username, username) == 0) {
            printf("ID Pinjam: %u | ID Buku (ocaid): %s | Tgl Pinjam: %s | Status: %s\n",
                   list_pinjam[i].id_peminjaman,
                   list_pinjam[i].ocaid,
                   list_pinjam[i].tanggal_pinjam,
                   list_pinjam[i].status);
            ada = 1;
        }
    }

    if (ada == 0) {
        printf("Anda belum memiliki riwayat peminjaman.\n");
    }
}
void lihat_peminjaman_aktif(const Peminjaman list_pinjam[], int jumlah_pinjam, const char *username) {
    int jumlahAktif = 0;

    for (int i = 0; i < jumlah_pinjam; i++) {
        int isUserMatch = (strcmp(list_pinjam[i].username, username) == 0);
        int isDipinjam  = (strcmp(list_pinjam[i].status, "DIPINJAM") == 0);

        if (isUserMatch && isDipinjam) {
            if (jumlahAktif == 0) {
                printf("\n=== BUKU YANG SEDANG DIPINJAM (%s) ===\n", username);
            }
            printf("ID Pinjam: %u | ID Buku (ocaid): %s | Tgl Pinjam: %s | Status: %s\n",
                   list_pinjam[i].id_peminjaman,
                   list_pinjam[i].ocaid,
                   list_pinjam[i].tanggal_pinjam,
                   list_pinjam[i].status);
            jumlahAktif++;
        }
    }

    if (jumlahAktif == 0) {
        printf("Tidak ada buku yang dipinjam.\n");
    }
}
void laporan_transaksi_global(const Peminjaman list_pinjam[], int jumlah_pinjam) {
    printf("\n=== LAPORAN TRANSAKSI PEMINJAMAN ===\n");
    if (jumlah_pinjam == 0) {
        printf("Belum ada transaksi peminjaman.\n");
        return;
    }

    for (int i = 0; i < jumlah_pinjam; i++) {
        printf("ID Pinjam: %u | User: %s | ID Buku (ocaid): %s | Tgl Pinjam: %s | Status: %s\n",
               list_pinjam[i].id_peminjaman,
               list_pinjam[i].username,
               list_pinjam[i].ocaid,
               list_pinjam[i].tanggal_pinjam,
               list_pinjam[i].status);
    }
}