#ifndef STORAGE_H
#define STORAGE_H

#include "auth.h"
#include "buku.h"
#include "peminjaman.h"

#ifndef BUKU_H_STRUCT
#define BUKU_H_STRUCT
typedef struct Buku {
    char ocaid[64];
    char judul[128];
    char penulis[128];
    char penerbit[128];
    int tahun;
    int page_count;
    int stok;
} Buku;
#endif

#ifndef AUTH_H_STRUCT
#define AUTH_H_STRUCT
typedef struct User {
    char username[32];
    char password[32];
    char role[12];
} User;
#endif

int load_users(const char *filename, User list_user[], int max_data);
int load_books(const char *filename, Buku list_buku[], int max_data);
int load_peminjaman(const char *filename, Peminjaman list_pinjam[], int max_data);

void save_users(const char *filename, const User list_user[], int jumlah_user);
void save_books(const char *filename, const Buku list_buku[], int jumlah_buku);
void save_peminjaman(const char *filename, const Peminjaman list_pinjam[], int jumlah_pinjam);

#endif