#include <stdio.h>
#include <string.h>
#include "../include/auth.h"

int verifyCredentials(const char *inputUser, const char *inputPass, User *loggedInUser) {
    FILE *file = fopen(USER_FILE_PATH, "r");
    
    if (file == NULL) {
        printf("ERROR! Gagal membuka file '%s'.\n", USER_FILE_PATH);
        return -1;
    }

    User tempUser;
    int isMatchFound = 0;

    while (fscanf(file, "%49[^|]|%49[^|]|%19[^\n]\n", 
                  tempUser.username, 
                  tempUser.password, 
                  tempUser.role) == 3) {
        
        int isUsernameMatch = (strcmp(inputUser, tempUser.username) == 0);
        int isPasswordMatch = (strcmp(inputPass, tempUser.password) == 0);

        if (isUsernameMatch && isPasswordMatch) {
            *loggedInUser = tempUser;
            isMatchFound = 1;
            break;
        }
    }

    fclose(file);
    return isMatchFound;
}

int promptLogin(User *loggedInUser) {
    char inputUser[MAX_USERNAME_LEN];
    char inputPass[MAX_PASSWORD_LEN];
    int attempts = 0;

    printf("========================================\n");
    printf("       LOGIN SISTEM PERPUSTAKAAN        \n");
    printf("========================================\n");

    while (attempts < MAX_LOGIN_ATTEMPTS) {
        printf("\nUsername : ");
        scanf("%49s", inputUser);
        printf("Password : ");
        scanf("%49s", inputPass);

        int authStatus = verifyCredentials(inputUser, inputPass, loggedInUser);

        if (authStatus == -1) {
            return 0;
        }

        if (authStatus == 1) {
            printf("\nSelamat datang, %s! (Role: %s)\n", 
                   loggedInUser->username, loggedInUser->role);
            return 1;
        }

        attempts++;
        int remainingAttempts = MAX_LOGIN_ATTEMPTS - attempts;
        printf("Username atau password salah! Sisa percobaan: %d\n", remainingAttempts);
    }

    printf("\nBatas percobaan login habis. Program dihentikan.\n");
    return 0;
}