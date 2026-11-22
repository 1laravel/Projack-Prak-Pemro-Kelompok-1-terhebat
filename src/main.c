#include <stdio.h>
#include <string.h>
#include "../include/auth.h"

int main() {
    User currentUser;

    if (promptLogin(&currentUser) == 0) {
        logActivity("UNKNOWN", "LOGIN_FAILED", "Percobaan login gagal atau dibatalkan");
        return 1; 
    }

    logActivity(currentUser.username, "LOGIN", "Berhasil masuk ke sistem");

    if (strcmp(currentUser.role, "admin") == 0) {
        printf("\n--> Membuka Menu Admin (Kelola & Sorting Buku)...\n");
    } else {
        printf("\n--> Membuka Menu Member (Pinjam & Cari Buku)...\n");
    }

    return 0;
}