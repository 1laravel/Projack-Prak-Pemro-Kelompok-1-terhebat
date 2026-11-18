#include <stdio.h>
#include "../include/auth.h"

int main() {
    User currentUser;

    if (promptLogin(&currentUser) == 0) {
        return 1; 
    }

    if (strcmp(currentUser.role, "admin") == 0) {
        printf("\n--> Membuka Menu Admin (Kelola & Sorting Buku)...\n");
    } else {
        printf("\n--> Membuka Menu Member (Pinjam & Cari Buku)...\n");
    }

    return 0;
}