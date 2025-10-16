/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#ifndef ECOMMERCESYSTEM_PRINT_UTIL_H
#define ECOMMERCESYSTEM_PRINT_UTIL_H

void printHeader(char *title);

void printSubHeader(char *title);

void printTitle(char *title);

void printPlusLine();

void printStarLine();

void printLine();

void printSubLine();

void printInputHint();

void printWarning(char *warning);

void printError(char *err);

void printSuccess(char *msg);

int getDisplayWidth(const char *str);

void printPadded(const char *str, int targetWidth);

#endif //ECOMMERCESYSTEM_PRINT_UTIL_H
