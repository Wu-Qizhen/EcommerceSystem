/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "security/validation.h"

#include <ctype.h>
#include <string.h>

#include "../../../include/merchant/product.h"
#include "utils/print_util.h"


int validateUserName(const char *name) {
    const int size = strlen(name);
    if (size < 3 || size > 50) {
        printWarning("用户名长度必须在 3-50 位之间！");
        return 0;
    }

    return 1;
}

int validateAccount(const char *account) {
    const int size = strlen(account);
    if (size < 6 || size > 20) {
        printWarning("账号长度必须在 6-20 位之间！");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (!isalnum(account[i])) {
            printWarning("账号只能包含数字和字母！");
            return 0;
        }
    }

    return 1;
}

int validatePhone(const char *phone) {
    const int size = strlen(phone);
    if (size != 11) {
        printWarning("手机号长度必须为 11 位！");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (!isdigit(phone[i])) {
            printWarning("手机号只能包含数字！");
            return 0;
        }
    }

    return 1;
}

int validateBirth(const char *birth) {
    const int size = strlen(birth);
    if (size != 10 || birth[4] != '.' || birth[7] != '.') {
        printWarning("日期格式错误！");
        return 0;
    }

    for (int i = 0; i < size; i++) {
        if (i != 4 & i != 7) {
            if (!isdigit(birth[i])) {
                printWarning("日期格式错误！");
                return 0;
            }
        }
    }

    return 1;
}

int validatePassword(const char *password) {
    const int size = strlen(password);
    if (size < 8 || size > 20) {
        printWarning("密码长度必须在 8-20 位之间！");
        return 0;
    }

    int hasLetter = 0, hasDigit = 0;
    for (int i = 0; i < size; i++) {
        if (isalpha(password[i])) hasLetter = 1;
        if (isdigit(password[i])) hasDigit = 1;
    }

    if (!hasLetter || !hasDigit) {
        printWarning("密码必须包含字母和数字！");
        return 0;
    }

    return 1;
}

int validateProductName(const char *name) {
    const int size = strlen(name);
    if (size < 2 || size > 50) {
        printWarning("商品名长度必须在 2-50 位之间！");
        return 0;
    }

    return 1;
}

int validateCategory(const char *category) {
    const int size = strlen(category);
    if (size > MAX_CATEGORY_LEN) {
        printWarning("分类长度不能超过 50 位！");
        return 0;
    }

    return 1;
}

int validateDescription(const char *description) {
    const int size = strlen(description);
    if (size > MAX_DESC_LEN) {
        printWarning("商品描述长度不能超过 200 位！");
        return 0;
    }

    return 1;
}

int validatePrice(const double price) {
    if (price <= 0) {
        printWarning("商品价格必须大于 0！");
        return 0;
    }

    return 1;
}

int validateStock(const int stock) {
    if (stock < 0) {
        printWarning("库存数量必须大于等于 0！");
        return 0;
    }

    return 1;
}
