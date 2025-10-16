/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.16
 */

#ifndef ECOMMERCESYSTEM_CART_H
#define ECOMMERCESYSTEM_CART_H

#include "merchant/product.h"

#define MAX_CART_ITEMS 100
#define MAX_ADDRESS_LEN 200

typedef struct {
    int productId;
    char productName[MAX_NAME_LEN];
    double price;
    int quantity;
    int merchantId;
} CartItem;

typedef struct {
    int userId;
    CartItem items[MAX_CART_ITEMS];
    int itemCount;
    double totalAmount;
} Cart;

extern Cart currentCart;
extern const char *CART_DATA_FILE;

void loadCart(int userId);

void saveCart(void);

void displayCart(void);

void addToCart(void);

void updateCartItemQuantity(void);

void removeFromCart(void);

void clearCart(void);

void checkoutCart(void);

double calculateCartTotal(void);

int findCartItemByProductId(int productId);

#endif //ECOMMERCESYSTEM_CART_H
