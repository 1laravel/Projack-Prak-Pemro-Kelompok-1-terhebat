#ifndef PEMINJAMAN_H
#define PEMINJAMAN_H

struct Buku;

typedef struct {
    unsigned int id_peminjaman;
    char username[32];
    char ocaid[64];
    char tanggal_pinjam[20]; 
    char status;        
} Peminjaman;

void pinjam_buku(Peminjaman list_pinjam[], int *jumlah_pinjam, struct Buku list_buku[], int jumlah_buku, const char *username);
void kembalikan_buku(Peminjaman list_pinjam[], int jumlah_pinjam, struct Buku list_buku[], int jumlah_buku, const char *username);
void lihat_peminjaman_pribadi(const Peminjaman list_pinjam[], int jumlah_pinjam, const char *username);

void laporan_transaksi_global(const Peminjaman list_pinjam[], int jumlah_pinjam);

#endif