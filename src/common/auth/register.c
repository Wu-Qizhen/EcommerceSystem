/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "auth/register.h"

#include <stdio.h>
#include <string.h>

#include "user/user.h"
#include "security/validation.h"
#include "utils/print_util.h"

void registerUser(const int role) {
    if (userCount >= MAX_USERS) {
        printError("用户数量已满！");
    }

    User user;

    printSubHeader("注册账号");

    do {
        printf("请输入账号：");
        scanf("%s", user.account);
        /*fgets(user.account,MAX_ACCOUNT_LEN,stdin);
        user.account[strspn(user.account, "\n")] = 0;*/
        if (findUserByAccount(user.account) != -1) {
            printWarning("账号已存在！");
        }
    } while (!validateAccount(user.account) || findUserByAccount(user.account) != -1);

    do {
        printf("请输入用户名：");
        scanf("%s", user.name);
        /*fgets(user.name,MAX_NAME_LEN,stdin);
        user.name[strspn(user.name, "\n")] = 0;*/
    } while (!validateUserName(user.name));

    do {
        printf("请输入生日：");
        scanf("%s", user.birth);
        /*fgets(user.birth,MAX_BIRTH_LEN,stdin);
        user.birth[strspn(user.birth, "\n")] = 0;*/
    } while (!validateBirth(user.birth));

    do {
        printf("请输入手机号：");
        scanf("%s", user.phone);
        /*fgets(user.phone,MAX_PHONE_LEN,stdin);
        user.phone[strspn(user.phone, "\n")] = 0;*/
        if (findUserByPhone(user.phone) != -1) {
            printWarning("手机号已被注册！");
        }
    } while (!validatePhone(user.phone) || findUserByPhone(user.phone) != -1);

    char password1[MAX_PASSWORD_LEN];
    char password2[MAX_PASSWORD_LEN];
    int done = 0;
    do {
        printf("请输入密码（8-20 位，包含字母和数字）：\n");
        scanf("%s", password1);
        /*fgets(password1, MAX_PASSWORD_LEN, stdin);
        password1[strspn(password1, "\n")] = 0;*/

        printf("请再次输入密码：");
        scanf("%s", password2);
        /*fgets(password2, MAX_PASSWORD_LEN, stdin);
        password2[strspn(password2, "\n")] = 0;*/

        if (strcmp(password1, password2) != 0) {
            printWarning("两次输入的密码不一致！");
            continue;
        }

        done = 1;
    } while (!validatePassword(password1) || done == 0);

    strcpy(user.password, password1);

    user.role = role;
    user.balance = 0;

    if (role == USER_ROLE_MERCHANT) {
        user.id = userCount + 1;
    } else {
        user.id = -1;
    }

    users[userCount] = user;
    userCount++;
    saveUsers();

    printSuccess("注册成功！");
}
