/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "security/encryption.h"

#include <string.h>

void encryptPassword(char *password) {
    const char key = 0x55;
    const int len = strlen(password);
    for (int i = 0; i < len; i++) {
        password[i] = password[i] ^ key;
    }
}

void decryptPassword(char *password) {
    encryptPassword(password);
}
