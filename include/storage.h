#ifndef STORAGE_H
#define STORAGE_H

#include "auth.h"
#include "buku.h"
#include "peminjaman.h"


int load_users(const char *filename, User list_user[], int max_data);
int load_books(const char *filename, Buku list_buku[], int max_data);
int load_peminjaman(const char *filename, Peminjaman list_pinjam[], int max_data);

void save_users(const char *filename, const User list_user[], int jumlah_user);
void save_books(const char *filename, const Buku list_buku[], int jumlah_buku);
void save_peminjaman(const char *filename, const Peminjaman list_pinjam[], int jumlah_pinjam);

#endif