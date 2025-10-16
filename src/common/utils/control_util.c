/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "utils/control_util.h"

#include <stdio.h>
#include <stdlib.h>

void pauseScreen() {
    printf("按任意键继续……");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    getchar();
}

void clearScreen() {
    /*printf("\033[2J\033[H");
    fflush(stdout);*/
    system("cls"); // 清屏命令
}
