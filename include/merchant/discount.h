/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.15
 */

#ifndef ECOMMERCESYSTEM_DISCOUNT_H
#define ECOMMERCESYSTEM_DISCOUNT_H

#include <time.h>

#define MAX_NAME_LEN 50
#define MAX_DISCOUNTS 100

#define DISCOUNT_TYPE_PERCENTAGE 0     // 百分比折扣
#define DISCOUNT_TYPE_FIXED 1          // 固定金额折扣
#define DISCOUNT_TYPE_FULL_REDUCTION 2 // 满减活动

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
} Discount;

extern Discount discounts[MAX_DISCOUNTS];
extern int discountCount;
extern const char *DISCOUNT_DATA_FILE;

void loadDiscounts();

void saveDiscounts();

int findDiscountById(int id);

int findDiscountsByMerchantId(int merchantId, Discount result[]);

int findActiveDiscountsByMerchantId(int merchantId, Discount result[]);

void displayDiscountDetails(const Discount *discount);

void displayDiscountsByMerchantId(Discount result[], int count);

void addDiscount(int merchantId);

void queryDiscounts(int merchantId);

void manageDiscountStatus(int merchantId);

int activeDiscount(int discountId);

int unactiveDiscount(int discountId);

int toggleDiscountStatus(int discountId);

double calculateDiscountedPrice(double originalPrice, const Discount *discount);

int isValidDiscount(const Discount *discount);

#endif //ECOMMERCESYSTEM_DISCOUNT_H
