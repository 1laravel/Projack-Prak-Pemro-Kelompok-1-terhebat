#ifndef MENU_H
#define MENU_H

#include "auth.h"

#ifndef FILE_BUKU_RAW
#define FILE_BUKU_RAW    "data/Books.txt"
#endif
#ifndef FILE_BUKU
#define FILE_BUKU        "data/buku.txt"
#endif
#ifndef FILE_PEMINJAMAN
#define FILE_PEMINJAMAN  "data/peminjaman.txt"
#endif

#ifndef MAX_BUKU
#define MAX_BUKU    15000
#endif
#ifndef MAX_PINJAM
#define MAX_PINJAM  15000
#endif

void menuUtama(const User *akun);

#endif