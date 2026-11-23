#ifndef MENU_H
#define MENU_H

#include "auth.h"  

#ifndef FILE_BUKU
#define FILE_BUKU        "data/Books.txt"
#endif
#ifndef FILE_PEMINJAMAN
#define FILE_PEMINJAMAN  "data/peminjaman.txt"
#endif

#ifndef MAX_BUKU
#define MAX_BUKU    5000
#endif
#ifndef MAX_PINJAM
#define MAX_PINJAM  5000
#endif

void menuUtama(const User *akun);

#endif 