/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#ifndef ECOMMERCESYSTEM_PRODUCT_H
#define ECOMMERCESYSTEM_PRODUCT_H

#include <time.h>

#define MAX_PRODUCTS 1000
#define MAX_NAME_LEN 50
#define MAX_CATEGORY_LEN 50
#define MAX_DESC_LEN 200

#define DISCOUNT_TYPE_PERCENTAGE 0
#define DISCOUNT_TYPE_FIXED 1

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    char category[MAX_CATEGORY_LEN];
    char description[MAX_DESC_LEN];
    double price;
    int stock;
    int merchantId;
    int isActive;
    int isDeleted;
} Product;

typedef struct {
    int id;
    int merchantId;
    char name[MAX_NAME_LEN];
    int type;
    double discountRate;
    double minAmount;
    double discountAmount;

    time_t startTime;
    time_t endTime;
    int isActive;
    int isDeleted;
} DiscountActivity;

extern Product products[MAX_PRODUCTS];
extern int productCount;
extern Product *currentProduct;

void loadProducts();

void saveProducts();

int findProductById(int id);

int findProductByName(int merchantId, const char *name);

int findProductsByMerchantId(int merchantId, Product result[]);

int findProductsByMerchantIdLimitCount(int merchantId, Product result[], int maxResults);

int searchProducts(const char *keyword, Product result[], int maxResults);

int searchProductsLimitCount(const char *keyword, Product result[], int maxResults);

void displayCurrentProductInfo();

void displayCurrentProductInfoById(int productId);

void addProduct(int merchantId);

int activeProduct(int productId);

int unactiveProduct(int productId);

#endif //ECOMMERCESYSTEM_PRODUCT_H
