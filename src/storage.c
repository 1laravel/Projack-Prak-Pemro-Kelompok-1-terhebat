#include <stdio.h>
#include <string.h>
#include "storage.h"
#include <stdio.h>
#include <string.h>
#include "../include/storage.h"

int load_users(const char *filename, User list_user[], int max_data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int i = 0;
    while (i < max_data) {
        int hasil = fscanf(file, "%31s %31s %11s",
                           list_user[i].username,
                           list_user[i].password,
                           list_user[i].role);
        if (hasil != 3) {
            break;
        }
        i++;
    }

    fclose(file);
    return i;
}
void save_users(const char *filename, const User list_user[], int jumlah_user) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s\n", filename);
        return;
    }

    for (int i = 0; i < jumlah_user; i++) {
        fprintf(file, "%s %s %s\n",
                list_user[i].username,
                list_user[i].password,
                list_user[i].role);
    }

    fclose(file);
}
int load_books(const char *filename, Buku list_buku[], int max_data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int i = 0;
    while (i < max_data) {
        int hasil = fscanf(file, "%63s \"%127[^\"]\" \"%127[^\"]\" \"%127[^\"]\" %d %d %d",
                           list_buku[i].ocaid,
                           list_buku[i].judul,
                           list_buku[i].penulis,
                           list_buku[i].penerbit,
                           &list_buku[i].tahun,
                           &list_buku[i].page_count,
                           &list_buku[i].stok);
        if (hasil != 7) {
            break;
        }
        if (list_buku[i].stok >= 0 && list_buku[i].page_count > 0) {
            i++;
        }
    }

    fclose(file);
    return i;
}
void save_books(const char *filename, const Buku list_buku[], int jumlah_buku) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s\n", filename);
        return;
    }

    for (int i = 0; i < jumlah_buku; i++) {
        fprintf(file, "%s \"%s\" \"%s\" \"%s\" %d %d %d\n",
                list_buku[i].ocaid,
                list_buku[i].judul,
                list_buku[i].penulis,
                list_buku[i].penerbit,
                list_buku[i].tahun,
                list_buku[i].page_count,
                list_buku[i].stok);
    }

    fclose(file);
}
int load_peminjaman(const char *filename, Peminjaman list_pinjam[], int max_data) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    int i = 0;
    while (i < max_data) {
        int hasil = fscanf(file, "%u %31s %63s %19s %19s",
                           &list_pinjam[i].id_peminjaman,
                           list_pinjam[i].username,
                           list_pinjam[i].ocaid,
                           list_pinjam[i].tanggal_pinjam,
                           list_pinjam[i].status);
        if (hasil != 5) {
            break;
        }
        i++;
    }

    fclose(file);
    return i;
}
