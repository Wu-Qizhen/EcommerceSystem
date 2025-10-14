/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "auth/login.h"

#include <stdio.h>
#include <string.h>

#include "user/user.h"
#include "merchant/merchant_main.h"
#include "utils/print_util.h"

void loginUser(const int role) {
    char account[MAX_ACCOUNT_LEN];
    char password[MAX_PASSWORD_LEN];

    printSubHeader("登录账号");

    printf("请输入账号：");
    scanf("%s", account);
    /*fgets(account, MAX_ACCOUNT_LEN, stdin);
    account[strcspn(account, "\n")] = 0;*/

    const int userIndex = findUserByAccountWithRole(account, role);
    if (userIndex == -1) {
        printError("账号不存在！");
        return;
    }

    printf("请输入密码：");
    scanf("%s", password);
    /*fgets(password,MAX_PASSWORD_LEN,stdin);
    password[strcspn(password, "\n")] = 0;*/

    if (strcmp(users[userIndex].password, password) == 0) {
        currentUser = &users[userIndex];
        if (role == USER_ROLE_CONSUMER) {
            printSuccess("客户端登录成功！");
            // TODO 商品系统
        } else if (role == USER_ROLE_MERCHANT) {
            printSuccess("商家端登录成功！");
            merchantSys(currentUser->id);
        } else {
            printError("系统错误，请联系客服！");
        }
    } else {
        printError("密码错误！");
    }
}
