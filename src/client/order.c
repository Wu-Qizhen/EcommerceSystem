/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.16
 */

#include "client/order.h"
#include <stdio.h>
#include <string.h>

#include "utils/control_util.h"
#include "utils/print_util.h"

Order orders[MAX_ORDERS];
int orderCount = 0;
const char *ORDER_DATA_FILE = "D:\\MyProjects\\CPP\\EcommerceSystem\\data\\orders.dat";
static Order userOrders[MAX_ORDERS];

void loadOrders(void) {
    FILE *file = fopen(ORDER_DATA_FILE, "rb");
    if (file == NULL) {
        orderCount = 0;
        return;
    }

    fread(&orderCount, sizeof(int), 1, file);
    if (orderCount > MAX_ORDERS) {
        orderCount = MAX_ORDERS;
    }

    for (int i = 0; i < orderCount; i++) {
        fread(&orders[i], sizeof(Order), 1, file);
    }

    fclose(file);
}

void saveOrders(void) {
    FILE *file = fopen(ORDER_DATA_FILE, "wb");
    if (file == NULL) {
        printError("无法保存订单数据！");
        return;
    }

    fwrite(&orderCount, sizeof(int), 1, file);
    for (int i = 0; i < orderCount; i++) {
        fwrite(&orders[i], sizeof(Order), 1, file);
    }

    fclose(file);
}

void createOrderFromCart(const char *address, const double totalAmount) {
    if (orderCount >= MAX_ORDERS) {
        printError("订单数量已达上限！");
        return;
    }

    Order *newOrder = &orders[orderCount];
    newOrder->id = orderCount > 0 ? orders[orderCount - 1].id + 1 : 1;
    newOrder->userId = currentCart.userId;
    newOrder->itemCount = currentCart.itemCount;
    newOrder->totalAmount = totalAmount;
    strcpy(newOrder->address, address);
    newOrder->createTime = time(NULL);
    newOrder->status = 1;

    /*for (int i = 0; i < currentCart.itemCount; i++) {
        newOrder->items[i] = currentCart.items[i];
    }*/

    for (int i = 0; i < currentCart.itemCount; i++) {
        newOrder->items[i].productId = currentCart.items[i].productId;
        newOrder->items[i].quantity = currentCart.items[i].quantity;
        newOrder->items[i].price = currentCart.items[i].price;
        strcpy(newOrder->items[i].productName, currentCart.items[i].productName);
    }


    orderCount++;
    saveOrders();
}

void viewAllOrders(void) {
    const int count = findOrdersByUserId(currentCart.userId, userOrders);

    printHeader("所有订单");
    printf("电商商城 > 订单管理 > 所有订单\n");
    printSubLine();

    if (count == 0) {
        printf("暂无订单\n");
        return;
    }

    const int idWidth = 8;
    const int timeWidth = 24;
    const int amountWidth = 16;
    const int statusWidth = 10;

    printPadded("订单 ID", idWidth);
    printPadded("下单时间", timeWidth);
    printPadded("总金额", amountWidth);
    printPadded("状态", statusWidth);
    printf("\n");
    printSubLine();

    for (int i = 0; i < count; i++) {
        const Order *order = &userOrders[i];

        char idStr[20];
        sprintf(idStr, "%d", order->id);
        printPadded(idStr, idWidth);

        char timeStr[20];
        const struct tm *timeInfo = localtime(&order->createTime);
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);
        printPadded(timeStr, timeWidth);

        char amountStr[20];
        sprintf(amountStr, "¥%.2f", order->totalAmount);
        printPadded(amountStr, amountWidth);

        const char *statusStr;
        switch (order->status) {
            case 0: statusStr = "待付款";
                break;
            case 1: statusStr = "已付款";
                break;
            case 2: statusStr = "已取消";
                break;
            default: statusStr = "未知";
        }
        printPadded(statusStr, statusWidth);

        printf("\n");
    }


    printf("\n共 %d 个订单\n", count);
    printSubLine();
    pauseScreen();
}

void viewPendingOrders(void) {
    // Order userOrders[MAX_ORDERS];
    const int count = findOrdersByUserId(currentCart.userId, userOrders);
    int pendingCount = 0;

    printHeader("待付款订单");
    printf("电商商城 > 订单管理 > 待付款订单\n");
    printSubLine();

    for (int i = 0; i < count; i++) {
        if (userOrders[i].status == 0) {
            pendingCount++;
        }
    }

    if (pendingCount == 0) {
        printf("暂无待付款订单\n");
        printSubLine();
        pauseScreen();
        return;
    }

    const int idWidth = 8;
    const int timeWidth = 24;
    const int amountWidth = 16;

    printPadded("订单 ID", idWidth);
    printPadded("下单时间", timeWidth);
    printPadded("总金额", amountWidth);
    printf("\n");
    printSubLine();

    for (int i = 0; i < count; i++) {
        if (userOrders[i].status == 0) {
            const Order *order = &userOrders[i];

            char idStr[20];
            sprintf(idStr, "%d", order->id);
            printPadded(idStr, idWidth);

            char timeStr[20];
            struct tm *timeInfo = localtime(&order->createTime);
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);
            printPadded(timeStr, timeWidth);

            char amountStr[20];
            sprintf(amountStr, "¥%.2f", order->totalAmount);
            printPadded(amountStr, amountWidth);

            printf("\n");
        }
    }

    printf("\n共 %d 个待付款订单\n", pendingCount);
    printSubLine();
    pauseScreen();
}

void viewPaidOrders(void) {
    // Order userOrders[MAX_ORDERS];
    const int count = findOrdersByUserId(currentCart.userId, userOrders);
    int paidCount = 0;

    printHeader("已付款订单");
    printf("电商商城 > 订单管理 > 已付款订单\n");
    printSubLine();

    for (int i = 0; i < count; i++) {
        if (userOrders[i].status == 1) {
            paidCount++;
        }
    }

    if (paidCount == 0) {
        printf("暂无已付款订单\n");
        return;
    }

    const int idWidth = 8;
    const int timeWidth = 24;
    const int amountWidth = 16;

    printPadded("订单 ID", idWidth);
    printPadded("下单时间", timeWidth);
    printPadded("总金额", amountWidth);
    printf("\n");
    printSubLine();

    for (int i = 0; i < count; i++) {
        if (userOrders[i].status == 1) {
            const Order *order = &userOrders[i];

            char idStr[20];
            sprintf(idStr, "%d", order->id);
            printPadded(idStr, idWidth);

            char timeStr[20];
            struct tm *timeInfo = localtime(&order->createTime);
            strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", timeInfo);
            printPadded(timeStr, timeWidth);

            char amountStr[20];
            sprintf(amountStr, "¥%.2f", order->totalAmount);
            printPadded(amountStr, amountWidth);

            printf("\n");
        }
    }

    printf("\n共 %d 个已付款订单\n", paidCount);
    printSubLine();
    pauseScreen();
}

void cancelOrder(void) {
    // Order userOrders[MAX_ORDERS];
    const int count = findOrdersByUserId(currentCart.userId, userOrders);

    printHeader("取消订单");
    printf("电商商城 > 订单管理 > 取消订单\n");
    printSubLine();

    int cancelableCount = 0;
    int cancelableIds[MAX_ORDERS];

    for (int i = 0; i < count; i++) {
        if (userOrders[i].status == 0) {
            cancelableIds[cancelableCount++] = userOrders[i].id;
            printf("订单 ID：%d, 金额：¥%.2f\n", userOrders[i].id, userOrders[i].totalAmount);
        }
    }

    if (cancelableCount == 0) {
        printWarning("没有可取消的订单！");
        return;
    }

    printSubLine();
    int orderId;
    printf("请输入要取消的订单 ID：");
    if (scanf_s("%d", &orderId) != 1) {
        printError("输入格式错误！");
        return;
    }

    int valid = 0;
    for (int i = 0; i < cancelableCount; i++) {
        if (cancelableIds[i] == orderId) {
            valid = 1;
            break;
        }
    }

    if (!valid) {
        printError("订单 ID 无效或无法取消！");
        return;
    }

    const int orderIndex = findOrderById(orderId);
    if (orderIndex != -1) {
        orders[orderIndex].status = 2;

        for (int i = 0; i < orders[orderIndex].itemCount; i++) {
            const OrderItem *item = &orders[orderIndex].items[i];
            const int productIndex = findProductById(item->productId);
            if (productIndex != -1) {
                products[productIndex].stock += item->quantity;
            }
        }

        saveOrders();
        saveProducts();
        printSuccess("订单取消成功！");
    } else {
        printError("订单不存在！");
    }
}

void viewOrderDetails(void) {
    // Order userOrders[MAX_ORDERS];
    const int count = findOrdersByUserId(currentCart.userId, userOrders);

    printHeader("订单详情");
    printf("电商商城 > 订单管理 > 订单详情\n");
    printSubLine();

    if (count == 0) {
        printf("暂无订单\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("订单 ID：%d, 金额：¥%.2f\n", userOrders[i].id, userOrders[i].totalAmount);
    }

    printSubLine();
    int orderId;
    printf("请输入要查看的订单 ID：");
    if (scanf_s("%d", &orderId) != 1) {
        printError("输入格式错误！");
        return;
    }

    const int orderIndex = findOrderById(orderId);
    if (orderIndex == -1 || orders[orderIndex].userId != currentCart.userId) {
        printError("订单不存在或无权访问！");
        return;
    }

    displayOrder(&orders[orderIndex]);
    printSubLine();
    pauseScreen();
}

void displayOrder(const Order *order) {
    if (order == NULL) {
        printError("订单信息为空！");
        return;
    }

    printHeader("订单详细信息");
    printf("电商商城 > 订单管理 > 订单详情\n");
    printSubLine();

    printf("订单 ID：%d\n", order->id);

    char timeStr[20];
    struct tm *timeInfo = localtime(&order->createTime);
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeInfo);
    printf("下单时间：%s\n", timeStr);

    printf("收货地址：%s\n", order->address);
    printf("订单金额：¥%.2f\n", order->totalAmount);

    const char *statusStr;
    switch (order->status) {
        case 0: statusStr = "待付款";
            break;
        case 1: statusStr = "已付款";
            break;
        case 2: statusStr = "已取消";
            break;
        default: statusStr = "未知";
    }
    printf("订单状态：%s\n", statusStr);

    printSubLine();
    // printf("商品列表：\n");

    const int nameWidth = 20;
    const int priceWidth = 10;
    const int quantityWidth = 8;
    const int subtotalWidth = 12;

    printPadded("商品名称", nameWidth);
    printPadded("单价", priceWidth);
    printPadded("数量", quantityWidth);
    printPadded("小计", subtotalWidth);
    printf("\n");
    printSubLine();

    for (int i = 0; i < order->itemCount; i++) {
        const OrderItem *item = &order->items[i];
        const double subtotal = item->price * item->quantity;

        printPadded(item->productName, nameWidth);

        char priceStr[20];
        sprintf(priceStr, "¥%.2f", item->price);
        printPadded(priceStr, priceWidth);

        char quantityStr[20];
        sprintf(quantityStr, "%d", item->quantity);
        printPadded(quantityStr, quantityWidth);

        char subtotalStr[20];
        sprintf(subtotalStr, "¥%.2f", subtotal);
        printPadded(subtotalStr, subtotalWidth);

        printf("\n");
    }
}

int findOrderById(const int orderId) {
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].id == orderId) {
            return i;
        }
    }
    return -1;
}

int findOrdersByUserId(const int userId, Order result[]) {
    int count = 0;
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].userId == userId) {
            result[count++] = orders[i];
        }
    }
    return count;
}
