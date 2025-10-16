/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "user/user.h"

#include <stdio.h>
#include <string.h>

#include "security/encryption.h"
#include "security/validation.h"
#include "utils/control_util.h"
#include "utils/print_util.h"

User users[MAX_USERS];
int userCount = 0;
User *currentUser = NULL;
const char *USER_DATA_FILE = "D:\\MyProjects\\CPP\\EcommerceSystem\\data\\users.dat";

void loadUsers() {
    FILE *file = fopen(USER_DATA_FILE, "rb");
    if (file == NULL) {
        printError("无法打开用户数据文件！");
        return;
    }

    fread(&userCount, sizeof(int), 1, file);
    for (int i = 0; i < userCount; i++) {
        fread(&users[i], sizeof(User), 1, file);
        decryptPassword(users[i].password);
    }

    fclose(file);
}

void saveUsers() {
    FILE *file = fopen(USER_DATA_FILE, "wb");
    if (file == NULL) {
        printError("无法保存用户数据！");
        return;
    }

    for (int i = 0; i < userCount; i++) {
        encryptPassword(users[i].password);
    }

    fwrite(&userCount, sizeof(int), 1, file);
    for (int i = 0; i < userCount; i++) {
        fwrite(&users[i], sizeof(User), 1, file);
    }

    for (int i = 0; i < userCount; i++) {
        decryptPassword(users[i].password);
    }

    fclose(file);
}

int findUserByAccount(const char *account) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].account, account) == 0) {
            return i;
        }
    }
    return -1;
}

int findUserByAccountWithRole(const char *account, const int role) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].account, account) == 0 && users[i].role == role) {
            return i;
        }
    }
    return -1;
}

int findUserByPhone(const char *phone) {
    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].phone, phone) == 0) {
            return i;
        }
    }
    return -1;
}

void displayCurrentUserInfo(const int userRole) {
    if (currentUser == NULL) return;

    if (userRole == USER_ROLE_MERCHANT) {
        printHeader("查看信息");
        printf("商品概览 > 商家信息 > 查看信息\n");
    } else {
        printHeader("查看信息");
        printf("商品概览 > 用户信息 > 查看信息\n"); // TODO
    }

    printSubLine();
    printf("姓名：%s\n", currentUser->name);
    printf("出生日期：%s\n", currentUser->birth);
    printf("账号：%s\n", currentUser->account);
    printf("手机号：%s\n", currentUser->phone);
    printSubLine();
    pauseScreen();
}

void changePassword(const int userRole) {
    if (currentUser == NULL) return;

    if (userRole == USER_ROLE_MERCHANT) {
        printHeader("修改密码");
        printf("商品概览 > 商家信息 > 修改密码\n");
    } else {
        printHeader("修改密码");
        printf("商品概览 > 用户信息 > 修改密码\n"); // TODO
    }

    printSubLine();

    char oldPassword[MAX_PASSWORD_LEN];
    char password1[MAX_PASSWORD_LEN];
    char password2[MAX_PASSWORD_LEN];

    printf("请输入当前密码：");
    scanf_s("%s", oldPassword);

    if (strcmp(currentUser->password, oldPassword) != 0) {
        printError("当前密码错误！");
        return;
    }

    int done = 0;
    do {
        printf("请输入密码（8-20 位，包含字母和数字）：");
        scanf_s("%s", password1);

        printf("请再次输入密码：");
        scanf_s("%s", password2);

        if (strcmp(password1, password2) != 0) {
            printWarning("两次输入的密码不一致！");
            continue;
        }

        done = 1;
    } while (!validatePassword(password1) || done == 0);

    strcpy(currentUser->password, password1);
    saveUsers();
    printSuccess("密码修改成功！");
}

void showUserMenu(const int userRole) {
    int choice;
    do {
        if (userRole == USER_ROLE_MERCHANT) {
            printHeader("商家信息");
            printf("商品概览 > 商家信息\n");
        } else {
            printHeader("用户信息");
            printf("商品概览 > 用户信息\n"); // TODO
        }

        printSubLine();
        printf("0. 返回上级菜单\n");
        printf("1. 修改密码\n");
        printf("2. 查看信息\n");
        printSubLine();
        printInputHint();
        scanf_s("%d", &choice);
        // clearInputBuffer();

        switch (choice) {
            case 1:
                changePassword(userRole);
                break;
            case 2:
                displayCurrentUserInfo(userRole);
                break;
            case 0:
                return;
            default:
                printWarning("无效操作，请重新选择！");
        }
    } while (choice != 0 && currentUser != NULL);
}

int getMerchantName(const int merchantId, char *merchantName) {
    for (int i = 0; i < userCount; i++) {
        if (users[i].id == merchantId) {
            strcpy(merchantName, users[i].name);
            return 1;
        }
    }
    return -1;
}
