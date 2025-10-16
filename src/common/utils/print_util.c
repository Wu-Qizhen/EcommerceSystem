/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "utils/print_util.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils/control_util.h"

void printHeader(char *title) {
    printLine();
    printTitle(title);
    printLine();
}

void printSubHeader(char *title) {
    printSubLine();
    printTitle(title);
    printSubLine();
}

void printTitle(char *title) {
    const int totalLength = 80;
    int titleLength = 0;
    const char *ptr = title;

    while (*ptr != '\0') {
        titleLength++;
        ptr++;
    }

    const int padding = (totalLength - titleLength - 2) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }

    printf(" %s ", title);

    printf("\n");
}

void printPlusLine() {
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

void printStarLine() {
    printf("********************************************************************************\n");
}

void printLine() {
    printf("================================================================================\n");
}

void printSubLine() {
    printf("--------------------------------------------------------------------------------\n");
}

void printInputHint() {
    printf("请选择操作：");
}

void printWarning(char *warning) {
    printStarLine();
    printf("⚠️ %s\n", warning);
    printStarLine();
    pauseScreen();
}

void printError(char *err) {
    printStarLine();
    printf("❌ %s\n", err);
    printStarLine();
    pauseScreen();
}

void printSuccess(char *msg) {
    printPlusLine();
    printf("✅ %s\n", msg);
    printPlusLine();
    pauseScreen();
}

int getDisplayWidth(const char *str) {
    int width = 0;
    while (*str) {
        if ((*str & 0xC0) != 0x80) {
            if ((unsigned char) *str >= 0x80) {
                // 中文字符
                width += 2;
            } else {
                // 英文字符
                width += 1;
            }
        }
        str++;
    }
    return width;
}

void printPadded(const char *str, const int targetWidth) {
    const int actualWidth = getDisplayWidth(str);
    printf("%s", str);
    for (int i = 0; i < targetWidth - actualWidth; i++) {
        printf(" ");
    }
}
