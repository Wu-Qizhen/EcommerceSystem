/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "merchant/merchant_main.h"
#include <stdio.h>
#include <string.h>

#include "product.h"
#include "utils/print_util.h"

void showMerchantMenu(Product result[], const int count) {
    printSubHeader("欢迎商家");
    printTitle("商品信息");
    printSubLine();
    displayProductByMerchantId(result, count);
    printSubLine();
    printf("1. 添加商品 2. 查询商品");
    printf("3. 上架商品 4. 下架商品\n");
    printf("5. 退出登录\n");
    printSubLine();
    printInputHint();
}

/*void displayProductByMerchantId(const int merchantId) {
    Product result[MAX_PRODUCTS];
    const int count = findProductsByMerchantId(merchantId, result);
    for (int i = 0; i < count; i++) {
        char isActive[3];
        if (result[i].isActive) {
            strncpy(isActive, "上架", sizeof(isActive) - 1);
        } else {
            strncpy(isActive, "下架", sizeof(isActive) - 1);
        }
        printf("%d. %s [%s]\n", i + 1, result[i].name, isActive);
    }

    if (count == 0) {
        printf("\n现在还没有商品，请先添加商品！");
    } else {
        printf("\n共有 %d 件商品！", count);
    }
}*/

void displayProductByMerchantId(Product result[], const int count) {
    for (int i = 0; i < count; i++) {
        char isActive[3];
        if (result[i].isActive) {
            strncpy(isActive, "上架", sizeof(isActive) - 1);
        } else {
            strncpy(isActive, "下架", sizeof(isActive) - 1);
        }
        printf("%d. %s [%s]\n", i + 1, result[i].name, isActive);
    }

    if (count == 0) {
        printf("\n现在还没有商品，请先添加商品！");
    } else {
        printf("\n共有 %d 件商品！", count);
    }
}

void merchantSys(const int merchantId) {
    loadProducts();

    Product result[MAX_PRODUCTS];
    const int count = findProductsByMerchantId(merchantId, result);

    int opt;

    do {
        showMerchantMenu(result, count);
        scanf_s("%d", &opt);

        switch (opt) {
            case 1:
                addProduct(merchantId);
                break;
            case 2:
                printf("查询");
                // TODO
                break;
            case 3:
                printf("上架");
                // TODO
                break;
            case 4:
                printf("下架");
                // TODO
                break;
            case 5:
                printSuccess("退出登录成功！");
                break;
            default:
                printError("无效操作，请重新选择！");
                break;
        }
    } while (opt != 4);

    saveProducts();
}
