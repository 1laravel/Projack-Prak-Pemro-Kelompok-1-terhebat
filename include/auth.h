#ifndef AUTH_H
#define AUTH_H

//cons define
#define USER_FILE_PATH "data/user.txt"
#define MAX_USERNAME_LEN 50
#define MAX_PASSWORD_LEN 50
#define MAX_ROLE_LEN 20
#define MAX_LOGIN_ATTEMPTS 3

typedef struct {
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char role[MAX_ROLE_LEN];
} User;

int verifyCredentials(const char *inputUser, const char *inputPass, User *loggedInUser);
int promptLogin(User *loggedInUser);

#endif