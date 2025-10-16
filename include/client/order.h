/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.16
 */

#ifndef ECOMMERCESYSTEM_ORDER_H
#define ECOMMERCESYSTEM_ORDER_H
#include <time.h>

#include "cart.h"

#define MAX_ORDERS 1000
#define MAX_ORDER_ITEMS 50

#define ORDER_STATUS_PENDING 0  // 待付款
#define ORDER_STATUS_PAID 1     // 已付款
#define ORDER_STATUS_CANCELED 2 // 已取消

typedef struct {
    int productId;
    char productName[MAX_NAME_LEN];
    double price;
    int quantity;
    int merchantId;
} OrderItem;

typedef struct {
    int id;
    int userId;
    OrderItem items[MAX_ORDER_ITEMS];
    int itemCount;
    double totalAmount;
    char address[MAX_ADDRESS_LEN];
    time_t createTime;
    int status;
} Order;

extern Order orders[MAX_ORDERS];
extern int orderCount;
extern const char *ORDER_DATA_FILE;

void loadOrders(void);

void saveOrders(void);

void createOrderFromCart(const char *address, double totalAmount);

void viewAllOrders(void);

void viewPendingOrders(void);

void viewPaidOrders(void);

void cancelOrder(void);

void viewOrderDetails(void);

void displayOrder(const Order *order);

int findOrderById(int orderId);

int findOrdersByUserId(int userId, Order result[]);

#endif //ECOMMERCESYSTEM_ORDER_H
