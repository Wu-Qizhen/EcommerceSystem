/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "merchant/merchant_main.h"
#include <stdio.h>
#include <string.h>

#include "discount.h"
#include "product.h"
#include "user/user.h"
#include "utils/control_util.h"
#include "utils/print_util.h"

void showMerchantMenu(Product result[], const int count) {
    printHeader("商品概览");
    displayProductByMerchantId(result, count);
    printSubLine();
    printf("1. 添加商品 2. 查询商品\n");
    printf("3. 上架商品 4. 下架商品\n");
    printf("5. 商家信息 6. 优惠政策\n");
    printf("0. 退出登录\n");
    printSubLine();
    printInputHint();
}

void showDiscountManagementMenu(const int merchantId) {
    int choice;
    do {
        printHeader("优惠政策");
        printf("商品概览 > 优惠政策\n");
        printSubLine();
        printf("0. 返回上级菜单\n");
        printf("1. 添加折扣活动\n");
        printf("2. 查询折扣活动\n");
        printf("3. 管理活动状态\n");
        printf("4. 查看有效活动\n");
        printSubLine();
        printInputHint();

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            continue;
        }

        switch (choice) {
            case 1:
                addDiscount(merchantId);
                break;
            case 2:
                queryDiscounts(merchantId);
                break;
            case 3:
                manageDiscountStatus(merchantId);
                break;
            case 4: {
                Discount result[MAX_DISCOUNTS];
                const int count = findActiveDiscountsByMerchantId(merchantId, result);
                if (count > 0) {
                    printHeader("查看有效活动");
                    printf("商品概览 > 优惠政策 > 查看有效活动\n");
                    printSubLine();
                    displayDiscountsByMerchantId(result, count);
                    printSubLine();
                    pauseScreen();
                } else {
                    printWarning("暂无有效活动！");
                }
                break;
            }
            case 0:
                return;
            default:
                printError("无效选择，请重新输入！");
                break;
        }
    } while (choice != 0);
}

/*void displayProductByMerchantId(Product result[], const int count) {
    for (int i = 0; i < count; i++) {
        char isActive[] = "未上架";
        if (result[i].isActive) {
            strncpy(isActive, "已上架", sizeof(isActive) - 1);
        }
        printf("%d. %s [%s]\n", i + 1, result[i].name, isActive);
    }

    if (count == 0) {
        printf("\n现在还没有商品，请先添加商品！\n");
    } else {
        printf("\n共有 %d 件商品！\n", count);
    }
}*/

void merchantSys(const int merchantId) {
    // system("chcp 65001");

    int choice;
    loadProducts();

    do {
        Product result[MAX_PRODUCTS];
        const int count = findProductsByMerchantIdActive(merchantId, result);

        showMerchantMenu(result, count);

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            while (getchar() != '\n') {
            }
            continue;
        }

        switch (choice) {
            case 1:
                addProduct(merchantId);
                break;
            case 2:
                queryProducts(merchantId);
                break;
            case 3:
            case 4:
                manageProductStatus(merchantId);
                break;
            case 5:
                showUserMenu(USER_ROLE_MERCHANT);
                break;
            case 6:
                showDiscountManagementMenu(merchantId);
                break;
            case 0:
                saveProducts();
                printSuccess("登出成功！");
                break;
            default:
                printError("无效操作，请重新选择！");
                break;
        }
    } while (choice != 0);
}
