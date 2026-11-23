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