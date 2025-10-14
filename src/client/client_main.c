/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

#include "user/user.h"
#include "auth/login.h"
#include "auth/register.h"
#include "auth/reset.h"
#include "utils/print_util.h"

void showApp() {
    printHeader("客户端");
}

void showAuthMenu() {
    printSubHeader("用户认证");
    printf("1. 登录账号\n");
    printf("2. 注册账号\n");
    printf("3. 忘记密码\n");
    printf("4. 退出系统\n");
    printSubLine();
    printInputHint();
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) {
    system("chcp 65001");

    loadUsers();

    int opt;

    showApp();

    do {
        showAuthMenu();
        scanf_s("%d", &opt);
        // clearInputBuffer();
        switch (opt) {
            case 1:
                loginUser(USER_ROLE_CONSUMER);
                break;
            case 2:
                registerUser(USER_ROLE_CONSUMER);
                break;
            case 3:
                forgetPassword();
                break;
            case 4:
                printSubLine();
                printf("感谢使用，再见！\n");
                break;
            default:
                printError("无效操作，请重新选择！");
                break;
        }
    } while (opt != 4);

    saveUsers();
    return 0;
}
