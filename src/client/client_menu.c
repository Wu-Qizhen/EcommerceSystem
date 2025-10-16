/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.16
 */

#include "client/client_menu.h"

#include <stdio.h>

#include "cart.h"
#include "order.h"
#include "user/user.h"
#include "utils/print_util.h"

void showClientMenu(void) {
    printHeader("电商商城");
    printf("1. 浏览商品\n");
    printf("2. 购物车\n");
    printf("3. 订单管理\n");
    printf("4. 用户信息\n");
    printf("5. 充值账户\n");
    printf("0. 退出登录\n");
    printSubLine();
    printInputHint();
}

void showProductBrowsingMenu(void) {
    int choice;
    do {
        printHeader("浏览商品");
        printf("电商商城 > 浏览商品\n");
        printSubLine();
        printf("0. 返回上级菜单\n");
        printf("1. 按分类浏览\n");
        printf("2. 按价格排序\n");
        printf("3. 搜索商品\n");
        printf("4. 查看商品详情\n");
        printSubLine();
        printInputHint();

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            continue;
        }

        switch (choice) {
            case 1:
                browseProductsByCategory();
                break;
            case 2:
                browseProductsByPrice();
                break;
            case 3:
                searchProductsForClient();
                break;
            case 4:
                viewProductDetailsForClient();
                break;
            case 0:
                return;
            default:
                printError("无效选择，请重新输入！");
                break;
        }
    } while (choice != 0);
}

void showCartMenu(void) {
    int choice;
    do {
        printHeader("购物车");
        printf("电商商城 > 购物车\n");
        printSubLine();
        displayCart();
        printSubLine();
        printf("0. 返回上级菜单\n");
        printf("1. 添加商品到购物车\n");
        printf("2. 修改商品数量\n");
        printf("3. 删除购物车商品\n");
        printf("4. 清空购物车\n");
        printf("5. 结算购物车\n");
        printSubLine();
        printInputHint();

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            continue;
        }

        switch (choice) {
            case 1:
                addToCart();
                break;
            case 2:
                updateCartItemQuantity();
                break;
            case 3:
                removeFromCart();
                break;
            case 4:
                clearCart();
                break;
            case 5:
                checkoutCart();
                break;
            case 0:
                return;
            default:
                printError("无效选择，请重新输入！");
                break;
        }
    } while (choice != 0);
}

void showOrderManagementMenu(void) {
    int choice;
    do {
        printHeader("订单管理");
        printf("电商商城 > 订单管理\n");
        printSubLine();
        printf("0. 返回上级菜单\n");
        printf("1. 查看所有订单\n");
        printf("2. 查看待付款订单\n");
        printf("3. 查看已付款订单\n");
        printf("4. 取消订单\n");
        printf("5. 查看订单详情\n");
        printSubLine();
        printInputHint();

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            continue;
        }

        switch (choice) {
            case 1:
                viewAllOrders();
                break;
            case 2:
                viewPendingOrders();
                break;
            case 3:
                viewPaidOrders();
                break;
            case 4:
                cancelOrder();
                break;
            case 5:
                viewOrderDetails();
                break;
            case 0:
                return;
            default:
                printError("无效选择，请重新输入！");
                break;
        }
    } while (choice != 0);
}

void clientSys(const int userId) {
    // system("chcp 65001");

    int choice;
    loadProducts();
    loadCart(userId);
    loadOrders();

    do {
        showClientMenu();

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            while (getchar() != '\n') {
            }
            continue;
        }

        switch (choice) {
            case 1:
                showProductBrowsingMenu();
                break;
            case 2:
                showCartMenu();
                break;
            case 3:
                showOrderManagementMenu();
                break;
            case 4:
                showUserMenu(USER_ROLE_CONSUMER);
                break;
            case 5:
                rechargeAccount(userId);
                break;
            case 0:
                saveCart();
                saveOrders();
                printSuccess("登出成功！");
                break;
            default:
                printError("无效操作，请重新选择！");
                break;
        }
    } while (choice != 0);
}
