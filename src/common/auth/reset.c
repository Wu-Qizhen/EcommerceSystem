/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "auth/reset.h"

#include <stdio.h>
#include <string.h>


#include "security/validation.h"
#include "user/user.h"
#include "utils/print_util.h"

void forgetPassword() {
    char account[MAX_ACCOUNT_LEN];
    char phone[MAX_PHONE_LEN];

    printHeader("忘记密码");
    printf_s("认证 > 忘记密码\n");
    printSubLine();

    printf("请输入账号：");
    scanf_s("%s", account);
    /*fgets(account, MAX_ACCOUNT_LEN, stdin);
    account[strcspn(account, "\n")] = 0;*/

    const int userIndex = findUserByAccount(account);
    if (userIndex == -1) {
        printError("账号不存在！");
        return;
    }

    printf("请输入注册时使用的手机号：");
    scanf_s("%s", phone);
    /*fgets(phone, MAX_PHONE_LEN, stdin);
    phone[strcspn(phone, "\n")] = 0;*/

    if (strcmp(users[userIndex].phone, phone) == 0) {
        char password1[MAX_PASSWORD_LEN];
        char password2[MAX_PASSWORD_LEN];
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

        strcpy(users[userIndex].password, password1);
        saveUsers();
        printSuccess("密码重置成功！");
    } else {
        printError("手机号验证失败！");
    }
}
