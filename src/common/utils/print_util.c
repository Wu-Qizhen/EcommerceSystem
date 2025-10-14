/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "utils/print_util.h"

#include <stdio.h>

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
    const int totalLength = 40;
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

void printLine() {
    printf("========================================\n");
}

void printSubLine() {
    printf("----------------------------------------\n");
}

void printInputHint() {
    printf("请选择操作：");
}

void printWarning(char *warning) {
    printSubLine();
    printf("Warning: %s\n", warning);
    printSubLine();
    pauseScreen();
}

void printError(char *err) {
    printSubLine();
    printf("Error: %s\n", err);
    printSubLine();
    pauseScreen();
}

void printSuccess(char *msg) {
    printSubLine();
    printf("%s\n", msg);
    printSubLine();
    pauseScreen();
}
