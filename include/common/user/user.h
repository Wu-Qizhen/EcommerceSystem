/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#ifndef ECOMMERCESYSTEM_USER_H
#define ECOMMERCESYSTEM_USER_H

#define MAX_USERS 100
#define MAX_NAME_LEN 50
#define MAX_ACCOUNT_LEN 30
#define MAX_PASSWORD_LEN 30
#define MAX_PHONE_LEN 15
#define MAX_BIRTH_LEN 11

#define USER_ROLE_CONSUMER 0
#define USER_ROLE_MERCHANT 1

/*typedef enum {
    USER_TYPE_CONSUMER = 0, // 消费者
    USER_TYPE_MERCHANT = 1 // 商家
} UserRole;*/

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char birth[MAX_BIRTH_LEN];
    char account[MAX_ACCOUNT_LEN];
    char password[MAX_PASSWORD_LEN];
    char phone[MAX_PHONE_LEN];
    int role;
    double balance;
} User;

extern User users[MAX_USERS];
extern int userCount;
extern User *currentUser;

void loadUsers();

void saveUsers();

int findUserByAccount(const char *account);

int findUserByAccountWithRole(const char *account, int role);

int findUserByPhone(const char *phone);

void displayCurrentUserInfo(int userRole);

void changePassword(int userRole);

void showUserMenu(int userRole);

int getMerchantName(int merchantId, char *merchantName);

#endif //ECOMMERCESYSTEM_USER_H
