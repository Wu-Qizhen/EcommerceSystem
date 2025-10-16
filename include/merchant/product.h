/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.14
 */

#ifndef ECOMMERCESYSTEM_PRODUCT_H
#define ECOMMERCESYSTEM_PRODUCT_H

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

extern Product products[MAX_PRODUCTS];
extern int productCount;
extern Product *currentProduct;

void loadProducts();

void saveProducts();

int findProductById(int id);

int findProductByName(int merchantId, const char *name);

int findProductsByMerchantIdActive(int merchantId, Product result[]);

int findProductsByMerchantIdAll(int merchantId, Product result[]);

int findProductsByMerchantIdActiveLimitCount(int merchantId, Product result[], int maxResults);

// int searchProducts(const char *keyword, Product result[]);

// int searchProductsLimitCount(const char *keyword, Product result[], int maxResults);

int searchProducts(const char *keyword, Product result[], int maxResults);

void displayProductInfo(const Product *product);

void displayCurrentProductInfo();

void displayCurrentProductInfoById(int productId);

void addProduct(int merchantId);

void queryProducts(int merchantId);

void manageProductStatus(int merchantId);

int activeProduct(int productId);

int unactiveProduct(int productId);

int toggleProductStatus(int productId);

void browseProductsByCategory(void);

void browseProductsByPrice(void);

void searchProductsForClient(void);

void viewProductDetailsForClient(void);

void displayProductsForClient(Product result[], int count);

void displayProductDetailsForClient(const Product *product);

#endif //ECOMMERCESYSTEM_PRODUCT_H
