/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.16
 */

#include "client/cart.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "order.h"
#include "user/user.h"
#include "utils/print_util.h"

Cart currentCart;
const char *CART_DATA_FILE = "D:\\MyProjects\\CPP\\EcommerceSystem\\data\\carts.dat";

void loadCart(const int userId) {
    FILE *file = fopen(CART_DATA_FILE, "rb");
    if (file == NULL) {
        currentCart.userId = userId;
        currentCart.itemCount = 0;
        currentCart.totalAmount = 0.0;
        return;
    }

    int found = 0;
    Cart tempCart;

    while (fread(&tempCart, sizeof(Cart), 1, file)) {
        if (tempCart.userId == userId) {
            currentCart = tempCart;
            found = 1;
            break;
        }
    }

    if (!found) {
        currentCart.userId = userId;
        currentCart.itemCount = 0;
        currentCart.totalAmount = 0.0;
    }

    fclose(file);
}

void saveCart(void) {
    FILE *file = fopen(CART_DATA_FILE, "rb+");
    if (file == NULL) {
        file = fopen(CART_DATA_FILE, "wb");
        if (file == NULL) {
            printError("无法保存购物车数据！");
            return;
        }
    }

    FILE *tempFile = tmpfile();
    Cart tempCart;
    int found = 0;

    while (fread(&tempCart, sizeof(Cart), 1, file)) {
        if (tempCart.userId == currentCart.userId) {
            fwrite(&currentCart, sizeof(Cart), 1, tempFile);
            found = 1;
        } else {
            fwrite(&tempCart, sizeof(Cart), 1, tempFile);
        }
    }

    if (!found) {
        fwrite(&currentCart, sizeof(Cart), 1, tempFile);
    }

    fclose(file);
    freopen(CART_DATA_FILE, "wb", tempFile);
    fseek(tempFile, 0, SEEK_SET);

    while (fread(&tempCart, sizeof(Cart), 1, tempFile)) {
        fwrite(&tempCart, sizeof(Cart), 1, file);
    }

    fclose(tempFile);
}

void displayCart(void) {
    if (currentCart.itemCount == 0) {
        printf("购物车为空\n");
        return;
    }

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

    for (int i = 0; i < currentCart.itemCount; i++) {
        CartItem *item = &currentCart.items[i];
        double subtotal = item->price * item->quantity;

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

    currentCart.totalAmount = calculateCartTotal();
    printSubLine();
    printf("总计：¥%.2f\n", currentCart.totalAmount);
}

void addToCart(void) {
    if (currentCart.itemCount >= MAX_CART_ITEMS) {
        printError("购物车已满，无法添加更多商品！");
        return;
    }

    printHeader("添加商品到购物车");
    printf("电商商城 > 购物车 > 添加商品\n");
    printSubLine();

    int productId, quantity;

    printf("请输入商品 ID：");
    if (scanf_s("%d", &productId) != 1) {
        printError("输入格式错误！");
        return;
    }

    int productIndex = findProductById(productId);
    if (productIndex == -1 || !products[productIndex].isActive || products[productIndex].isDeleted) {
        printError("商品不存在或已下架！");
        return;
    }

    Product *product = &products[productIndex];

    printf("请输入购买数量：");
    if (scanf_s("%d", &quantity) != 1 || quantity <= 0) {
        printError("数量必须为正整数！");
        return;
    }

    if (quantity > product->stock) {
        printError("商品库存不足！");
        return;
    }

    const int cartIndex = findCartItemByProductId(productId);
    if (cartIndex != -1) {
        if (currentCart.items[cartIndex].quantity + quantity > product->stock) {
            printError("商品库存不足！");
            return;
        }
        currentCart.items[cartIndex].quantity += quantity;
    } else {
        CartItem *newItem = &currentCart.items[currentCart.itemCount];
        newItem->productId = productId;
        strcpy(newItem->productName, product->name);
        newItem->price = product->price;
        newItem->quantity = quantity;
        newItem->merchantId = product->merchantId;
        currentCart.itemCount++;
    }

    saveCart();
    printSuccess("商品已成功添加到购物车！");
}

void updateCartItemQuantity(void) {
    if (currentCart.itemCount == 0) {
        printWarning("购物车为空！");
        return;
    }

    printHeader("修改商品数量");
    printf("电商商城 > 购物车 > 修改数量\n");
    printSubLine();
    displayCart();
    printSubLine();

    int productId, newQuantity;

    printf("请输入要修改的商品 ID：");
    if (scanf_s("%d", &productId) != 1) {
        printError("输入格式错误！");
        return;
    }

    const int cartIndex = findCartItemByProductId(productId);
    if (cartIndex == -1) {
        printError("购物车中未找到该商品！");
        return;
    }

    printf("请输入新的数量：");
    if (scanf_s("%d", &newQuantity) != 1) {
        printError("输入格式错误！");
        return;
    }

    if (newQuantity <= 0) {
        for (int i = cartIndex; i < currentCart.itemCount - 1; i++) {
            currentCart.items[i] = currentCart.items[i + 1];
        }
        currentCart.itemCount--;
        printSuccess("商品已从购物车移除！");
    } else {
        const int productIndex = findProductById(productId);
        if (productIndex != -1 && newQuantity > products[productIndex].stock) {
            printError("商品库存不足！");
            return;
        }
        currentCart.items[cartIndex].quantity = newQuantity;
        printSuccess("商品数量已更新！");
    }

    saveCart();
}

void removeFromCart(void) {
    if (currentCart.itemCount == 0) {
        printWarning("购物车为空！");
        return;
    }

    printHeader("删除购物车商品");
    printf("电商商城 > 购物车 > 删除商品\n");
    printSubLine();
    displayCart();
    printSubLine();

    int productId;

    printf("请输入要删除的商品 ID：");
    if (scanf_s("%d", &productId) != 1) {
        printError("输入格式错误！");
        return;
    }

    const int cartIndex = findCartItemByProductId(productId);
    if (cartIndex == -1) {
        printError("购物车中未找到该商品！");
        return;
    }

    for (int i = cartIndex; i < currentCart.itemCount - 1; i++) {
        currentCart.items[i] = currentCart.items[i + 1];
    }
    currentCart.itemCount--;

    saveCart();
    printSuccess("商品已从购物车删除！");
}

void clearCart(void) {
    if (currentCart.itemCount == 0) {
        printWarning("购物车已为空！");
        return;
    }

    printHeader("清空购物车");
    printf("电商商城 > 购物车 > 清空购物车\n");
    printSubLine();

    char confirm;
    printf("确认清空购物车？（Y/N）：");
    while (getchar() != '\n') {
    }
    scanf_s("%c", &confirm, 1);

    if (tolower(confirm) == 'y') {
        currentCart.itemCount = 0;
        currentCart.totalAmount = 0.0;
        saveCart();
        printSuccess("购物车已清空！");
    } else {
        printWarning("操作已取消！");
    }
}

/*void checkoutCart(void) {
    if (currentCart.itemCount == 0) {
        printWarning("购物车为空，无法结算！");
        return;
    }

    printHeader("结算购物车");
    printf("电商商城 > 购物车 > 结算\n");
    printSubLine();
    displayCart();
    printSubLine();

    for (int i = 0; i < currentCart.itemCount; i++) {
        const CartItem *item = &currentCart.items[i];
        const int productIndex = findProductById(item->productId);
        if (productIndex == -1 || products[productIndex].stock < item->quantity) {
            printError("商品库存不足！");
            return;
        }
    }

    char address[MAX_ADDRESS_LEN];
    printf("请输入收货地址：");
    while (getchar() != '\n') {
    }
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;

    if (strlen(address) == 0) {
        printError("收货地址不能为空！");
        return;
    }

    const double total = calculateCartTotal();
    printf("订单总金额：¥%.2f\n", total);

    char confirm;
    printf("确认支付？（Y/N）：");
    scanf_s("%c", &confirm, 1);

    if (tolower(confirm) == 'y') {
        createOrderFromCart(address, total);

        for (int i = 0; i < currentCart.itemCount; i++) {
            const CartItem *item = &currentCart.items[i];
            const int productIndex = findProductById(item->productId);
            if (productIndex != -1) {
                products[productIndex].stock -= item->quantity;
            }
        }

        currentCart.itemCount = 0;
        saveCart();
        saveProducts();

        printSuccess("订单创建成功！");
    } else {
        printWarning("支付已取消！");
    }
}*/

void checkoutCart(void) {
    if (currentCart.itemCount == 0) {
        printWarning("购物车为空，无法结算！");
        return;
    }

    printHeader("结算购物车");
    printf("电商商城 > 购物车 > 结算\n");
    printSubLine();
    displayCart();
    printSubLine();

    for (int i = 0; i < currentCart.itemCount; i++) {
        const CartItem *item = &currentCart.items[i];
        const int productIndex = findProductById(item->productId);
        if (productIndex == -1 || products[productIndex].stock < item->quantity) {
            printError("商品库存不足！");
            return;
        }
    }

    const double total = calculateCartTotal();
    printf("订单总金额：¥%.2f\n", total);

    if (currentUser->balance < total) {
        printError("余额不足，无法支付！");
        return;
    }

    char address[MAX_ADDRESS_LEN];
    printf("请输入收货地址：");
    while (getchar() != '\n') {
    }
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;

    if (strlen(address) == 0) {
        printError("收货地址不能为空！");
        return;
    }

    char confirm;
    printf("确认支付？（Y/N）：");
    scanf_s("%c", &confirm, 1);

    if (tolower(confirm) == 'y') {
        currentUser->balance -= total;
        users[currentUser->id - 1].balance = currentUser->balance;

        createOrderFromCart(address, total);

        for (int i = 0; i < currentCart.itemCount; i++) {
            const CartItem *item = &currentCart.items[i];
            const int productIndex = findProductById(item->productId);
            if (productIndex != -1) {
                products[productIndex].stock -= item->quantity;
            }
        }

        currentCart.itemCount = 0;
        saveCart();
        saveProducts();
        saveUsers();

        printSuccess("订单创建成功！");
    } else {
        printWarning("支付已取消！");
    }
}

double calculateCartTotal(void) {
    double total = 0.0;
    for (int i = 0; i < currentCart.itemCount; i++) {
        total += currentCart.items[i].price * currentCart.items[i].quantity;
    }
    return total;
}

int findCartItemByProductId(const int productId) {
    for (int i = 0; i < currentCart.itemCount; i++) {
        if (currentCart.items[i].productId == productId) {
            return i;
        }
    }
    return -1;
}
