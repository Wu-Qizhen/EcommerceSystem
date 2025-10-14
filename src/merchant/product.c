/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#include "merchant/product.h"

#include <stdio.h>
#include <string.h>

#include "security/validation.h"
#include "user/user.h"
#include "utils/print_util.h"

Product products[MAX_PRODUCTS];
int productCount = 0;
Product *currentProduct = NULL;

void loadProducts() {
    FILE *file = fopen("D:\\MyProjects\\CPP\\EcommerceSystem\\data\\products.dat", "rb");
    if (file == NULL) {
        printError("无法打开商品数据文件！");
        return;
    }

    fread(&productCount, sizeof(int), 1, file);

    fclose(file);
}

void saveProducts() {
    FILE *file = fopen("D:\\MyProjects\\CPP\\EcommerceSystem\\data\\products.dat", "wb");
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

int findProductsByMerchantId(const int merchantId, Product result[]) {
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

int findProductsByMerchantIdLimitCount(const int merchantId, Product result[], const int maxResults) {
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

int searchProducts(const char *keyword, Product result[], const int maxResults) {
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
}

int searchProductsLimitCount(const char *keyword, Product result[], const int maxResults) {
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
}

void displayCurrentProductInfo() {
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
}

void addProduct(const int merchantId) {
    if (productCount >= MAX_PRODUCTS) {
        printError("商品数量已满！");
    }

    Product product;

    printSubHeader("添加商品");

    do {
        printf("请输入商品名称：");
        scanf("%s", product.name);
        if (findProductByName(merchantId, product.name) != -1) {
            printWarning("商品已存在！");
        }
    } while (!validateProductName(product.name) || findProductByName(merchantId, product.name) != -1);

    do {
        printf("请输入商品分类：");
        scanf("%s", product.category);
    } while (!validateCategory(product.category));

    do {
        printf("请输入商品描述：");
        scanf("%s", product.description);
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
    scanf("%s", &isActive);
    product.isActive = isActive == 'Y' || isActive == 'y';

    product.merchantId = merchantId;
    product.isDeleted = 0;
    product.id = productCount + 1;

    products[productCount] = product;
    productCount++;
    saveProducts();

    printSuccess("添加成功！");
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
