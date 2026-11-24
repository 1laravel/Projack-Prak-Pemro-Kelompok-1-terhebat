#include <stdio.h>
#include <string.h>
#include "storage.h"
#define MAX_LINE_LEN   2048
#define DEFAULT_STOK   5

/* Ambil teks di dalam tanda kutip "..." lalu geser cursor ke setelah kutip tutup */
static int ambilTeksKutip(const char **cursor, char *dest, size_t maxLen) {
    const char *start = strchr(*cursor, '"');
    if (start == NULL) return 0;
    start++;

    const char *end = strchr(start, '"');
    if (end == NULL) return 0;

    size_t len = (size_t)(end - start);
    if (len >= maxLen) {
        len = maxLen - 1;
    }
    memcpy(dest, start, len);
    dest[len] = '\0';

    *cursor = end + 1;
    return 1;
}
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
        printf("[INFO] load_books: File '%s' belum ada / gagal dibuka.\n", filename);
        return 0;
    }

    char line[MAX_LINE_LEN];
    int count = 0;

    while (count < max_data && fgets(line, sizeof(line), file) != NULL) {
        /* Jika baris lebih panjang dari buffer, buang sisanya agar tidak jadi baris baru */
        if (strchr(line, '\n') == NULL) {
            int ch;
            while ((ch = fgetc(file)) != '\n' && ch != EOF) { }
        }

        /* Lewati baris header atau baris kosong */
        if (strncmp(line, "ocaid", 5) == 0 || line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        Buku b;
        memset(&b, 0, sizeof(Buku));

        if (sscanf(line, "%63s", b.ocaid) != 1) {
            continue;
        }

        const char *cursor = line;
        if (!ambilTeksKutip(&cursor, b.title, sizeof(b.title))) continue;
        if (!ambilTeksKutip(&cursor, b.author, sizeof(b.author))) continue;

        int parsedNums = sscanf(cursor, "%d %d %d", &b.year, &b.page_count, &b.stok);
        if (parsedNums < 2) {
            continue;
        }
        if (parsedNums == 2) {
            /* Format mentah Books.txt (tidak ada kolom stok) */
            b.stok = DEFAULT_STOK;
        }

        if (b.year > 0 && b.page_count > 0 && b.stok >= 0) {
            list_buku[count] = b;
            count++;
        }
    }

    fclose(file);
    return count;
}
void save_books(const char *filename, const Buku list_buku[], int jumlah_buku) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s\n", filename);
        return;
    }

    for (int i = 0; i < jumlah_buku; i++) {
        fprintf(file, "%s \"%s\" \"%s\" %d %d %d\n",
                list_buku[i].ocaid,
                list_buku[i].title,
                list_buku[i].author,
                list_buku[i].year,
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
void save_peminjaman(const char *filename, const Peminjaman list_pinjam[], int jumlah_pinjam) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file %s\n", filename);
        return;
    }

    for (int i = 0; i < jumlah_pinjam; i++) {
        fprintf(file, "%u %s %s %s %s\n",
                list_pinjam[i].id_peminjaman,
                list_pinjam[i].username,
                list_pinjam[i].ocaid,
                list_pinjam[i].tanggal_pinjam,
                list_pinjam[i].status);
    }

    fclose(file);
}
