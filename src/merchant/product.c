/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "merchant/product.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "discount.h"
#include "security/validation.h"
#include "user/user.h"
#include "utils/control_util.h"
#include "utils/print_util.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

Product products[MAX_PRODUCTS];
int productCount = 0;
Product *currentProduct = NULL;
const char *PRODUCT_DATA_FILE = "D:\\MyProjects\\CPP\\EcommerceSystem\\data\\products.dat";

void loadProducts() {
    FILE *file = fopen(PRODUCT_DATA_FILE, "rb");
    if (file == NULL) {
        printError("无法打开商品数据文件！");
        return;
    }

    fread(&productCount, sizeof(int), 1, file);

    if (productCount > MAX_PRODUCTS) {
        productCount = MAX_PRODUCTS;
        printWarning("商品数据异常，已自动修正！");
    }

    for (int i = 0; i < productCount; i++) {
        fread(&products[i], sizeof(Product), 1, file);
    }

    fclose(file);
}

void saveProducts() {
    FILE *file = fopen(PRODUCT_DATA_FILE, "wb");
    if (file == NULL) {
        printError("无法保存商品数据！");
        return;
    }

    fwrite(&productCount, sizeof(int), 1, file);
    for (int i = 0; i < productCount; i++) {
        fwrite(&products[i], sizeof(Product), 1, file);
    }

    fclose(file);
}

int findProductById(const int id) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findProductByName(const int merchantId, const char *name) {
    for (int i = 0; i < productCount; i++) {
        if (products[i].merchantId == merchantId && strcmp(products[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int findProductsByMerchantIdActive(const int merchantId, Product result[]) {
    int count = 0;

    for (int i = 0; i < productCount; i++) {
        if (products[i].merchantId == merchantId &&
            products[i].isActive == 1 &&
            products[i].isDeleted == 0) {
            result[count] = products[i];
            count++;
        }
    }

    return count;
}

int findProductsByMerchantIdAll(const int merchantId, Product result[]) {
    int count = 0;
    for (int i = 0; i < productCount; i++) {
        if (products[i].merchantId == merchantId && !products[i].isDeleted) {
            result[count++] = products[i];
        }
    }
    return count;
}

int findProductsByMerchantIdActiveLimitCount(const int merchantId, Product result[], const int maxResults) {
    int count = 0;

    for (int i = 0; i < productCount && count < maxResults; i++) {
        if (products[i].merchantId == merchantId &&
            products[i].isActive == 1 &&
            products[i].isDeleted == 0) {
            result[count] = products[i];
            count++;
        }
    }

    return count;
}

/*int searchProducts(const char *keyword, Product result[]) {
    int count = 0;

    for (int i = 0; i < productCount; i++) {
        if (products[i].isActive == 1 &&
            products[i].isDeleted == 0 &&
            (strstr(products[i].name, keyword) != NULL ||
             strstr(products[i].category, keyword) != NULL)) {
            result[count] = products[i];
            count++;
        }
    }

    return count;
}*/

/*int searchProductsLimitCount(const char *keyword, Product result[], const int maxResults) {
    int count = 0;

    for (int i = 0; i < productCount && count < maxResults; i++) {
        if (products[i].isActive == 1 &&
            products[i].isDeleted == 0 &&
            (strstr(products[i].name, keyword) != NULL ||
             strstr(products[i].category, keyword) != NULL)) {
            result[count] = products[i];
            count++;
        }
    }

    return count;
}*/

int searchProducts(const char *keyword, Product result[], const int maxResults) {
    int count = 0;
    char lowerKeyword[MAX_NAME_LEN];

    strncpy(lowerKeyword, keyword, sizeof(lowerKeyword) - 1);
    for (int i = 0; lowerKeyword[i]; i++) {
        lowerKeyword[i] = (char) tolower(lowerKeyword[i]);
    }

    for (int i = 0; i < productCount && count < maxResults; i++) {
        if (products[i].isActive && !products[i].isDeleted) {
            char lowerName[MAX_NAME_LEN];
            char lowerCategory[MAX_CATEGORY_LEN];

            strncpy(lowerName, products[i].name, sizeof(lowerName) - 1);
            strncpy(lowerCategory, products[i].category, sizeof(lowerCategory) - 1);

            for (int j = 0; lowerName[j]; j++) lowerName[j] = (char) tolower(lowerName[j]);
            for (int j = 0; lowerCategory[j]; j++) lowerCategory[j] = (char) tolower(lowerCategory[j]);

            if (strstr(lowerName, lowerKeyword) != NULL ||
                strstr(lowerCategory, lowerKeyword) != NULL) {
                result[count++] = products[i];
            }
        }
    }
    return count;
}

/*void displayCurrentProductInfo() {
    if (currentProduct == NULL) return;

    printSubHeader("商品信息");
    printf("名称：%s\n", currentProduct->name);
    printf("分类：%s\n", currentProduct->category);
    printf("描述：%s\n", currentProduct->description);
    printf("价格：%.2lf\n", currentProduct->price);
    printf("库存：%d\n", currentProduct->stock);
    char merchantName[MAX_NAME_LEN];
    const int result = getMerchantName(currentProduct->merchantId, merchantName);
    if (result > 0) {
        printf("商家：%s\n", merchantName);
    }
    printf("上架情况：%s\n", currentProduct->isActive ? "是" : "否");
}

void displayCurrentProductInfoById(const int productId) {
    currentProduct = &products[findProductById(productId)];

    printSubHeader("商品信息");
    printf("名称：%s\n", currentProduct->name);
    printf("分类：%s\n", currentProduct->category);
    printf("描述：%s\n", currentProduct->description);
    printf("价格：%.2lf\n", currentProduct->price);
    printf("库存：%d\n", currentProduct->stock);
    char merchantName[MAX_NAME_LEN];
    const int result = getMerchantName(currentProduct->merchantId, merchantName);
    if (result > 0) {
        printf("商家：%s\n", merchantName);
    }
    printf("上架情况：%s\n", currentProduct->isActive ? "是" : "否");
}*/

void displayProductDetails(const Product *product) {
    printHeader("商品信息");
    printf("商品概览 > 商品信息\n");
    printSubLine();

    if (product == NULL) {
        printError("商品信息为空！");
        return;
    }

    printf("商品 ID：%d\n", product->id);
    printf("商品名称：%s\n", product->name);
    printf("商品分类：%s\n", product->category);
    printf("商品描述：%s\n", product->description);
    printf("商品价格：¥%.2f\n", product->price);
    printf("商品库存：%d\n", product->stock);

    char merchantName[MAX_NAME_LEN];
    if (getMerchantName(product->merchantId, merchantName) > 0) {
        printf("商家名称：%s\n", merchantName);
    }

    printf("上架状态：%s\n", product->isActive ? "✅ 已上架" : "❌ 未上架");
    printf("存在状态：%s\n", product->isDeleted ? "❌ 已删除" : "✅ 未删除");
    // printSubLine();
}

void displayProductByMerchantId(Product result[], const int count) {
    if (count == 0) {
        printf("现在还没有商品，请先添加商品！\n");
        return;
    }

    const int idWidth = 8;
    const int nameWidth = 16;
    const int categoryWidth = 16;
    const int priceWidth = 10;
    const int stockWidth = 8;
    const int statusWidth = 8;

    printPadded("ID", idWidth);
    printPadded("名称", nameWidth);
    printPadded("分类", categoryWidth);
    printPadded("价格", priceWidth);
    printPadded("库存", stockWidth);
    printPadded("上架状态", statusWidth);
    printf("\n");
    /*printf("%-8s %-16s %-16s %-8s %-8s %-4s\n",
           "ID", "名称", "分类", "价格", "库存", "上架状态");*/
    printSubLine();

    /*for (int i = 0; i < count; i++) {
        const char *status = result[i].isActive ? "✅" : "❌";
        if (result[i].stock > 10) {
            printf("%-8d %-16s %-16s ¥%-7.2f %-8d %-8s\n",
                   result[i].id,
                   result[i].name,
                   result[i].category,
                   result[i].price,
                   result[i].stock,
                   status);
        } else {
            printf("%-8d %-16s %-16s ¥%-7.2f ⚠️%-4d %-8s\n",
                   result[i].id,
                   result[i].name,
                   result[i].category,
                   result[i].price,
                   result[i].stock,
                   status);
        }
    }*/

    for (int i = 0; i < count; i++) {
        const char *status = result[i].isActive ? "✅" : "❌";

        char idStr[20];
        sprintf(idStr, "%d", result[i].id);
        printPadded(idStr, idWidth);

        printPadded(result[i].name, nameWidth);

        printPadded(result[i].category, categoryWidth);

        char priceStr[20];
        sprintf(priceStr, "¥%.2f", result[i].price);
        printPadded(priceStr, priceWidth);

        if (result[i].stock > 10) {
            char stockStr[20];
            sprintf(stockStr, "%d", result[i].stock);
            printPadded(stockStr, stockWidth);
        } else {
            char stockStr[20];
            sprintf(stockStr, "⚠️%d", result[i].stock);
            printPadded(stockStr, stockWidth);
        }

        printPadded(status, statusWidth);

        printf("\n");
    }

    printf("\n共有 %d 件商品！\n", count);
}

void displayCurrentProductInfo() {
    if (currentProduct == NULL) {
        printError("当前未选择任何商品！");
        return;
    }
    displayProductDetails(currentProduct);
}

void displayCurrentProductInfoById(const int productId) {
    const int index = findProductById(productId);
    if (index == -1) {
        printError("商品不存在！");
        return;
    }
    displayProductDetails(&products[index]);
}

/*void addProduct(const int merchantId) {
    if (productCount >= MAX_PRODUCTS) {
        printError("商品数量已满！");
    }

    Product product;

    printSubHeader("添加商品");

    do {
        printf("请输入商品名称：");
        scanf_s("%s", product.name);
        if (findProductByName(merchantId, product.name) != -1) {
            printWarning("商品已存在！");
        }
    } while (!validateProductName(product.name) || findProductByName(merchantId, product.name) != -1);

    do {
        printf("请输入商品分类：");
        scanf_s("%s", product.category);
    } while (!validateCategory(product.category));

    do {
        printf("请输入商品描述：");
        scanf_s("%s", product.description);
    } while (!validateDescription(product.description));

    do {
        printf("请输入商品价格：");
        scanf_s("%lf", &product.price);
    } while (!validatePrice(product.price));

    do {
        printf("请输入商品库存：");
        scanf_s("%d", &product.stock);
    } while (!validateStock(product.stock));

    char isActive;
    printf("是否立即上架商品（Y/N）：");
    scanf_s("%s", &isActive);
    product.isActive = isActive == 'Y' || isActive == 'y';#1#

    char input[10];
    printf("是否立即上架商品（Y/N）：");
    scanf_s("%s", input);
    product.isActive = (input[0] == 'Y' || input[0] == 'y');

    product.merchantId = merchantId;
    product.isDeleted = 0;
    product.id = productCount + 1;

    products[productCount] = product;
    productCount++;
    saveProducts();

    printSuccess("添加成功！");
}*/

/*void addProduct(const int merchantId) {
    printHeader("添加商品");
    printf("商品概览 > 添加商品\n");
    printSubLine();

    if (productCount >= MAX_PRODUCTS) {
        printError("商品数量已达上限，无法添加新商品！");
        return;
    }

    Product newProduct = {0};
    newProduct.merchantId = merchantId;

    do {
        printf("请输入商品名称：");
        if (scanf_s("%49s", newProduct.name, (unsigned) sizeof(newProduct.name)) != 1) {
            printError("输入格式错误！");
            // while (getchar() != '\n') {}
            continue;
        }

        if (!validateProductName(newProduct.name)) {
            printError("商品名称格式无效！");
            continue;
        }

        if (findProductByName(merchantId, newProduct.name) != -1) {
            printWarning("该商品名称已存在，请使用其他名称！");
            continue;
        }
        break;
    } while (1);

    do {
        printf("请输入商品分类：");
        if (scanf_s("%49s", newProduct.category, (unsigned) sizeof(newProduct.category)) != 1) {
            printError("输入格式错误！");
            // while (getchar() != '\n') {}
            continue;
        }
        if (validateCategory(newProduct.category)) break;
        printError("商品分类格式无效！");
    } while (1);

    do {
        printf("请输入商品描述：");
        if (scanf_s("%199s", newProduct.description, (unsigned) sizeof(newProduct.description)) != 1) {
            printError("输入格式错误！");
            // while (getchar() != '\n') {}
            continue;
        }
        if (validateDescription(newProduct.description)) break;
        printError("商品描述格式无效！");
    } while (1);

    do {
        printf("请输入商品价格：");
        if (scanf_s("%lf", &newProduct.price) != 1) {
            printError("输入格式错误！");
            // while (getchar() != '\n') {}
            continue;
        }
        if (validatePrice(newProduct.price)) break;
        printError("商品价格必须大于 0！");
    } while (1);

    do {
        printf("请输入商品库存：");
        if (scanf_s("%d", &newProduct.stock) != 1) {
            printError("输入格式错误！");
            // while (getchar() != '\n') {}
            continue;
        }
        if (validateStock(newProduct.stock)) break;
        printError("商品库存必须为非负整数！");
    } while (1);

    char choice;
    do {
        printf("是否立即上架商品（Y/N）：");
        // while (getchar() != '\n') {}
        choice = (char) getchar();
        choice = (char) tolower(choice);
    } while (choice != 'y' && choice != 'n');

    newProduct.isActive = choice == 'y';
    newProduct.isDeleted = 0;
    newProduct.id = productCount > 0 ? products[productCount - 1].id + 1 : 1;

    products[productCount] = newProduct;
    productCount++;
    saveProducts();

    printSuccess("商品添加成功！");
    displayProductDetails(&newProduct);
}*/

void addProduct(const int merchantId) {
    printHeader("添加商品");
    printf("商品概览 > 添加商品\n");
    printSubLine();

    if (productCount >= MAX_PRODUCTS) {
        printError("商品数量已达上限，无法添加新商品！");
        return;
    }

    Product newProduct = {0};
    newProduct.merchantId = merchantId;

    do {
        printf("请输入商品名称（输入 0 退出）：");
        if (scanf_s("%49s", newProduct.name, (unsigned) sizeof(newProduct.name)) != 1) {
            printError("输入格式错误！");
            continue;
        }

        if (strcmp(newProduct.name, "0") == 0) {
            printWarning("已取消添加商品！");
            return;
        }

        if (!validateProductName(newProduct.name)) {
            // printError("商品名称格式无效！");
            continue;
        }

        if (findProductByName(merchantId, newProduct.name) != -1) {
            printWarning("该商品名称已存在，请使用其他名称！");
            continue;
        }
        break;
    } while (1);

    do {
        printf("请输入商品分类（输入 0 退出）：");
        if (scanf_s("%49s", newProduct.category, (unsigned) sizeof(newProduct.category)) != 1) {
            printError("输入格式错误！");
            continue;
        }

        if (strcmp(newProduct.category, "0") == 0) {
            printWarning("已取消添加商品！");
            return;
        }

        if (validateCategory(newProduct.category)) break;
        printError("商品分类格式无效！");
    } while (1);

    do {
        printf("请输入商品描述（输入 0 退出）：");
        if (scanf_s("%199s", newProduct.description, (unsigned) sizeof(newProduct.description)) != 1) {
            printError("输入格式错误！");
            continue;
        }

        if (strcmp(newProduct.description, "0") == 0) {
            printWarning("已取消添加商品！");
            return;
        }

        if (validateDescription(newProduct.description)) break;
        printError("商品描述格式无效！");
    } while (1);

    do {
        printf("请输入商品价格（输入 0 退出）：");
        if (scanf_s("%lf", &newProduct.price) != 1) {
            printError("输入格式错误！");
            continue;
        }

        if (newProduct.price == 0) {
            printWarning("已取消添加商品！");
            return;
        }

        if (validatePrice(newProduct.price)) break;
        printError("商品价格必须大于 0！");
    } while (1);

    do {
        printf("请输入商品库存（输入 0 退出）：");
        if (scanf_s("%d", &newProduct.stock) != 1) {
            printError("输入格式错误！");
            continue;
        }

        if (newProduct.stock == 0) {
            printWarning("已取消添加商品！");
            return;
        }

        if (validateStock(newProduct.stock)) break;
        printError("商品库存必须为非负整数！");
    } while (1);

    /*char choice;
    int c;
    do {
        printf("是否立即上架商品（Y/N，输入 0 退出）：");
        while ((c = getchar()) != '\n' && c != EOF);
        choice = (char) getchar();
        choice = (char) tolower(choice);

        if (choice == '0') {
            printWarning("已取消添加商品！");
            return;
        }

        if (choice != 'y' && choice != 'n') {
            printError("无效输入，请输入 Y、N 或 0！");
            while ((c = getchar()) != '\n' && c != EOF);
        }
    } while (choice != 'y' && choice != 'n');*/

    int c;
    printf("是否立即上架商品（Y/N，输入 0 退出）：");
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    char choice = (char) getchar();
    choice = (char) tolower(choice);

    if (choice == '0') {
        printWarning("已取消添加商品！");
        return;
    }

    if (choice != 'y' && choice != 'n') {
        printWarning("输入无效，将设置为不上架！");
        choice = 'n';
    }

    newProduct.isActive = choice == 'y';
    newProduct.isDeleted = 0;
    newProduct.id = productCount > 0 ? products[productCount - 1].id + 1 : 1;

    products[productCount] = newProduct;
    productCount++;
    saveProducts();

    displayProductDetails(&newProduct);
    printSuccess("商品添加成功！");
}

void queryProducts(const int merchantId) {
    int choice;
    do {
        printHeader("查询商品");
        printf("商品概览 > 查询商品\n");
        printSubLine();

        printf("0. 返回上级菜单\n");
        printf("1. 查看所有商品\n");
        printf("2. 按名称搜索\n");
        printf("3. 查看商品详情\n");
        printSubLine();
        printInputHint();

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            // while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                Product result[MAX_PRODUCTS];
                const int count = findProductsByMerchantIdAll(merchantId, result);

                printHeader("查看所有商品");
                printf("商品概览 > 查询商品 > 查看所有商品\n");
                printSubLine();
                displayProductByMerchantId(result, count);
                printSubLine();
                pauseScreen();
                break;
            }
            case 2: {
                char keyword[MAX_NAME_LEN];

                printHeader("按名称搜索");
                printf("商品概览 > 查询商品 > 按名称搜索\n");
                printSubLine();
                printf("请输入搜索关键词：");
                scanf_s("%49s", keyword, (unsigned) sizeof(keyword));

                Product result[MAX_PRODUCTS];
                const int count = searchProducts(keyword, result, MAX_PRODUCTS);

                if (count > 0) {
                    printHeader("搜索结果");
                    printf("商品概览 > 查询商品 > 按名称搜索 > 搜索结果\n");
                    printSubLine();
                    // printf("找到 %d 个相关商品：\n", count);
                    displayProductByMerchantId(result, count);
                    printSubLine();
                    pauseScreen();
                } else {
                    printHeader("搜索结果");
                    printf("商品概览 > 查询商品 > 按名称搜索 > 搜索结果\n");
                    printWarning("未找到相关商品！");
                }
                break;
            }
            case 3: {
                int productId;

                printHeader("查看商品详情");
                printf("商品概览 > 查询商品 > 查看商品详情\n");
                printSubLine();
                printf("请输入商品 ID：");
                if (scanf_s("%d", &productId) != 1) {
                    printError("输入格式错误！");
                    break;
                }

                const int index = findProductById(productId);
                if (index == -1 || products[index].merchantId != merchantId) {
                    printError("商品不存在或无权访问！");
                    break;
                }

                displayProductDetails(&products[index]);
                printSubLine();
                pauseScreen();
                break;
            }
            case 0:
                // printSuccess("返回上级菜单！");
                return;
            default:
                printError("无效选择，请重新输入！");
                break;
        }
    } while (choice != 0);
}

void manageProductStatus(const int merchantId) {
    Product allProducts[MAX_PRODUCTS];
    const int count = findProductsByMerchantIdAll(merchantId, allProducts);

    if (count == 0) {
        printWarning("您还没有商品，请先添加商品！");
        return;
    }

    printHeader("管理商品状态");
    printf("商品概览 > 管理商品状态\n");
    printSubLine();

    displayProductByMerchantId(allProducts, count);

    int productId;
    printSubLine();
    printf("请输入要操作的商品 ID：");
    if (scanf_s("%d", &productId) != 1) {
        printError("输入格式错误！");
        return;
    }

    const int index = findProductById(productId);
    if (index == -1 || products[index].merchantId != merchantId) {
        printError("商品不存在或无权操作！");
        return;
    }

    Product *product = &products[index];

    printSubLine();
    printf("当前商品：%s\n", product->name);
    printf("当前状态：%s\n", product->isActive ? "✅ 已上架" : "❌ 未上架");

    char confirm;
    printSubLine();
    printf("确认%s该商品？（Y/N）：", product->isActive ? "下架" : "上架");
    while (getchar() != '\n') {
    }
    scanf_s("%c", &confirm, 1);

    if (tolower(confirm) == 'y') {
        product->isActive = !product->isActive;
        saveProducts();
        printSubLine();
        printf("新状态：%s\n", product->isActive ? "✅ 已上架" : "❌ 未上架");
        printSuccess("商品状态更新成功！");
    } else {
        printWarning("操作已取消！");
    }
}

int activeProduct(const int productId) {
    const int index = findProductById(productId);
    if (index == -1) {
        printError("商品不存在！");
        return -1;
    }
    products[index].isActive = 1;
    saveProducts();
    return 1;
}

int unactiveProduct(const int productId) {
    const int index = findProductById(productId);
    if (index == -1) {
        printError("商品不存在！");
        return -1;
    }
    products[index].isActive = 0;
    saveProducts();
    return 1;
}

int toggleProductStatus(const int productId) {
    const int index = findProductById(productId);
    if (index == -1) {
        printError("商品不存在！");
        return -1;
    }
    products[index].isActive = !products[index].isActive;
    saveProducts();
    return products[index].isActive;
}

void browseProductsByCategory(void) {
    printHeader("按分类浏览");
    printf("电商商城 > 浏览商品 > 按分类浏览\n");
    printSubLine();

    char categories[MAX_PRODUCTS][MAX_CATEGORY_LEN];
    int categoryCount = 0;

    for (int i = 0; i < productCount; i++) {
        if (products[i].isActive && !products[i].isDeleted) {
            int found = 0;
            for (int j = 0; j < categoryCount; j++) {
                if (strcmp(categories[j], products[i].category) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                strcpy(categories[categoryCount], products[i].category);
                categoryCount++;
            }
        }
    }

    if (categoryCount == 0) {
        printWarning("暂无商品分类！");
        return;
    }

    printf("可用分类：\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%d. %s\n", i + 1, categories[i]);
    }

    printSubLine();
    int choice;
    printf("请选择分类（输入 0 返回）：");
    if (scanf_s("%d", &choice) != 1) {
        printError("输入格式错误！");
        return;
    }

    if (choice == 0) return;

    if (choice < 1 || choice > categoryCount) {
        printError("无效选择！");
        return;
    }

    char selectedCategory[MAX_CATEGORY_LEN];
    strcpy(selectedCategory, categories[choice - 1]);

    Product result[MAX_PRODUCTS];
    int count = 0;

    for (int i = 0; i < productCount; i++) {
        if (products[i].isActive && !products[i].isDeleted &&
            strcmp(products[i].category, selectedCategory) == 0) {
            result[count++] = products[i];
        }
    }

    printHeader("分类商品");
    printf("电商商城 > 浏览商品 > 按分类浏览 > %s\n", selectedCategory);
    printSubLine();
    displayProductsForClient(result, count);
    printSubLine();
    pauseScreen();
}

void browseProductsByPrice(void) {
    printHeader("按价格排序");
    printf("电商商城 > 浏览商品 > 按价格排序\n");
    printSubLine();

    int choice;
    printf("排序方式：\n");
    printf("1. 从低到高\n");
    printf("2. 从高到低\n");
    printf("0. 返回\n");
    printSubLine();
    printInputHint();

    if (scanf_s("%d", &choice) != 1) {
        printError("输入格式错误！");
        return;
    }

    if (choice == 0) return;

    if (choice != 1 && choice != 2) {
        printError("无效选择！");
        return;
    }

    Product result[MAX_PRODUCTS];
    int count = 0;

    for (int i = 0; i < productCount; i++) {
        if (products[i].isActive && !products[i].isDeleted) {
            result[count++] = products[i];
        }
    }

    if (count == 0) {
        printWarning("暂无商品！");
        return;
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            int shouldSwap = 0;
            if (choice == 1) {
                shouldSwap = result[j].price > result[j + 1].price;
            } else {
                shouldSwap = result[j].price < result[j + 1].price;
            }

            if (shouldSwap) {
                Product temp = result[j];
                result[j] = result[j + 1];
                result[j + 1] = temp;
            }
        }
    }

    printHeader("排序结果");
    printf("电商商城 > 浏览商品 > 按价格排序\n");
    printSubLine();
    displayProductsForClient(result, count);
    printSubLine();
    pauseScreen();
}

void searchProductsForClient(void) {
    printHeader("搜索商品");
    printf("电商商城 > 浏览商品 > 搜索商品\n");
    printSubLine();

    char keyword[MAX_NAME_LEN];
    printf("请输入搜索关键词：");
    scanf_s("%49s", keyword, (unsigned) sizeof(keyword));

    Product result[MAX_PRODUCTS];
    const int count = searchProducts(keyword, result, MAX_PRODUCTS);

    if (count > 0) {
        printHeader("搜索结果");
        printf("电商商城 > 浏览商品 > 搜索商品 > 搜索结果\n");
        printSubLine();
        displayProductsForClient(result, count);
        printf("\n找到 %d 个相关商品：\n", count);
        printSubLine();
        pauseScreen();
    } else {
        printWarning("未找到相关商品！");
    }
}

void viewProductDetailsForClient(void) {
    printHeader("查看商品详情");
    printf("电商商城 > 浏览商品 > 查看商品详情\n");
    printSubLine();

    int productId;
    printf("请输入商品ID：");
    if (scanf_s("%d", &productId) != 1) {
        printError("输入格式错误！");
        return;
    }

    int index = findProductById(productId);
    if (index == -1 || !products[index].isActive || products[index].isDeleted) {
        printError("商品不存在或已下架！");
        return;
    }

    displayProductDetailsForClient(&products[index]);
    printSubLine();
    pauseScreen();
}

void displayProductsForClient(Product result[], const int count) {
    if (count == 0) {
        printf("暂无商品\n");
        return;
    }

    const int idWidth = 8;
    const int nameWidth = 20;
    const int categoryWidth = 16;
    const int priceWidth = 10;
    const int stockWidth = 8;
    const int merchantWidth = 16;

    printPadded("ID", idWidth);
    printPadded("名称", nameWidth);
    printPadded("分类", categoryWidth);
    printPadded("价格", priceWidth);
    printPadded("库存", stockWidth);
    printPadded("商家", merchantWidth);
    printf("\n");
    printSubLine();

    for (int i = 0; i < count; i++) {
        char idStr[20];
        sprintf(idStr, "%d", result[i].id);
        printPadded(idStr, idWidth);

        printPadded(result[i].name, nameWidth);
        printPadded(result[i].category, categoryWidth);

        char priceStr[20];
        sprintf(priceStr, "¥%.2f", result[i].price);
        printPadded(priceStr, priceWidth);

        char stockStr[20];
        if (result[i].stock > 10) {
            sprintf(stockStr, "%d", result[i].stock);
        } else {
            sprintf(stockStr, "⚠️%d", result[i].stock);
        }
        printPadded(stockStr, stockWidth);

        char merchantName[MAX_NAME_LEN];
        if (getMerchantName(result[i].merchantId, merchantName) > 0) {
            printPadded(merchantName, merchantWidth);
        } else {
            printPadded("未知", merchantWidth);
        }

        printf("\n");
    }

    printf("\n共有 %d 件商品！\n", count);
}

void displayProductDetailsForClient(const Product *product) {
    if (product == NULL) {
        printError("商品信息为空！");
        return;
    }

    printHeader("商品信息");
    printf("电商商城 > 浏览商品 > 商品信息\n");
    printSubLine();

    printf("商品 ID：%d\n", product->id);
    printf("商品名称：%s\n", product->name);
    printf("商品分类：%s\n", product->category);
    printf("商品描述：%s\n", product->description);
    printf("商品价格：¥%.2f\n", product->price);
    printf("商品库存：%d\n", product->stock);

    char merchantName[MAX_NAME_LEN];
    if (getMerchantName(product->merchantId, merchantName) > 0) {
        printf("商家名称：%s\n", merchantName);
    }

    Discount merchantDiscounts[MAX_DISCOUNTS];
    const int merchantDiscountsCount = findActiveDiscountsByMerchantId(product->merchantId, merchantDiscounts);

    if (merchantDiscountsCount > 0) {
        printf("优惠活动：\n");
        for (int i = 0; i < merchantDiscountsCount; i++) {
            const double discountedPrice = calculateDiscountedPrice(product->price, &merchantDiscounts[i]);
            if (discountedPrice < product->price) {
                printf("  - %s: ¥%.2f → ¥%.2f\n",
                       merchantDiscounts[i].name, product->price, discountedPrice);
            }
        }
    }
}
