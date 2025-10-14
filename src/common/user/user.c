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
#include "utils/print_util.h"

User users[MAX_USERS];
int userCount = 0;
User *currentUser = NULL;

void loadUsers() {
    FILE *file = fopen("D:\\MyProjects\\CPP\\EcommerceSystem\\data\\users.dat", "rb");
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
    FILE *file = fopen("D:\\MyProjects\\CPP\\EcommerceSystem\\data\\users.dat", "wb");
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

void displayCurrentUserInfo() {
    if (currentUser == NULL) return;

    printSubHeader("个人信息");
    printf("姓名：%s\n", currentUser->name);
    printf("出生日期：%s\n", currentUser->birth);
    printf("账号：%s\n", currentUser->account);
    printf("手机号：%s\n", currentUser->phone);
}

void changePassword() {
    if (currentUser == NULL) return;

    char oldPassword[MAX_PASSWORD_LEN];
    char newPassword[MAX_PASSWORD_LEN];

    printSubHeader("修改密码");

    printf("请输入当前密码：");
    scanf("%s", oldPassword);

    if (strcmp(currentUser->password, oldPassword) != 0) {
        printError("当前密码错误！");
        return;
    }

    do {
        printf("请输入新密码（8-20 位，包含字母和数字）：\n");
        scanf("%s", newPassword);
    } while (!validatePassword(newPassword));

    strcpy(currentUser->password, newPassword);
    saveUsers();
    printSuccess("密码修改成功！");
}

void showUserMenu() {
    int choice;
    do {
        printSubHeader("用户菜单");
        printf("1. 修改密码\n");
        printf("2. 查看个人信息\n");
        printf("3. 退出登录\n");
        printf("4. 返回\n");
        printSubLine();
        printInputHint();
        scanf_s("%d", &choice);
        // clearInputBuffer();

        switch (choice) {
            case 1:
                changePassword();
                break;
            case 2:
                displayCurrentUserInfo();
                break;
            case 3:
                currentUser = NULL;
                printSuccess("已退出登录！");
                break;
            case 4:
                return;
            default:
                printWarning("无效操作，请重新选择！");
        }
    } while (choice != 3 && currentUser != NULL);
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
