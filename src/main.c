#include <stdio.h>
#include "../include/auth.h"
#include "../include/menu.h"
#include "../include/utils.h"

static void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

int main() {
    User currentUser;

    int isLoginSuccess = promptLogin(&currentUser);
    if (!isLoginSuccess) {
        logActivity("UNKNOWN", "LOGIN_FAILED", "Percobaan login gagal atau dibatalkan");
        return 1;
    }

    clearInputBuffer();
    clearScreen();

    menuUtama(&currentUser);

    return 0;
}