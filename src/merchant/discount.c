/*
 * 代码不注释，同事两行泪！（给！爷！写！）
 * Elegance is not a dispensable luxury but a quality that decides between success and failure!
 * Created by Wu Qizhen on 2025.10.15
 */

#include "merchant/discount.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "utils/control_util.h"
#include "utils/print_util.h"

Discount discounts[MAX_DISCOUNTS];
int discountCount = 0;
const char *DISCOUNT_DATA_FILE = "D:\\MyProjects\\CPP\\EcommerceSystem\\data\\discounts.dat";

void loadDiscounts() {
    FILE *file = fopen(DISCOUNT_DATA_FILE, "rb");
    if (file == NULL) {
        printWarning("折扣活动数据文件不存在，将创建新文件");
        discountCount = 0;
        return;
    }

    fread(&discountCount, sizeof(int), 1, file);

    if (discountCount > MAX_DISCOUNTS) {
        discountCount = MAX_DISCOUNTS;
        printWarning("折扣数据异常，已自动修正！");
    }

    for (int i = 0; i < discountCount; i++) {
        fread(&discounts[i], sizeof(Discount), 1, file);
    }

    fclose(file);
}

void saveDiscounts() {
    FILE *file = fopen(DISCOUNT_DATA_FILE, "wb");
    if (file == NULL) {
        printError("无法保存折扣活动数据！");
        return;
    }

    fwrite(&discountCount, sizeof(int), 1, file);
    for (int i = 0; i < discountCount; i++) {
        fwrite(&discounts[i], sizeof(Discount), 1, file);
    }

    fclose(file);
}

int findDiscountById(const int id) {
    for (int i = 0; i < discountCount; i++) {
        if (discounts[i].id == id && !discounts[i].isDeleted) {
            return i;
        }
    }
    return -1;
}

int findDiscountsByMerchantId(const int merchantId, Discount result[]) {
    int count = 0;
    for (int i = 0; i < discountCount; i++) {
        if (discounts[i].merchantId == merchantId && !discounts[i].isDeleted) {
            result[count++] = discounts[i];
        }
    }
    return count;
}

int findActiveDiscountsByMerchantId(const int merchantId, Discount result[]) {
    int count = 0;
    const time_t currentTime = time(NULL);

    for (int i = 0; i < discountCount; i++) {
        if (discounts[i].merchantId == merchantId &&
            discounts[i].isActive &&
            !discounts[i].isDeleted &&
            discounts[i].startTime <= currentTime &&
            discounts[i].endTime >= currentTime) {
            result[count++] = discounts[i];
        }
    }
    return count;
}

void displayDiscountDetails(const Discount *discount) {
    if (discount == NULL) {
        printError("折扣活动信息为空！");
        return;
    }

    printHeader("折扣活动详细信息");
    printf("商品概览 > 优惠政策 > 折扣活动详细信息\n");
    printSubLine();
    printf("活动 ID：%d\n", discount->id);
    printf("活动名称：%s\n", discount->name);

    const char *typeStr;
    switch (discount->type) {
        case DISCOUNT_TYPE_PERCENTAGE:
            typeStr = "百分比折扣";
            break;
        case DISCOUNT_TYPE_FIXED:
            typeStr = "固定金额折扣";
            break;
        case DISCOUNT_TYPE_FULL_REDUCTION:
            typeStr = "满减活动";
            break;
        default:
            typeStr = "未知类型";
    }
    printf("活动类型：%s\n", typeStr);

    if (discount->type == DISCOUNT_TYPE_PERCENTAGE) {
        printf("折扣率：%.0f%%\n", discount->discountRate * 100);
    } else if (discount->type == DISCOUNT_TYPE_FIXED) {
        printf("折扣金额：¥%.2f\n", discount->discountAmount);
    } else if (discount->type == DISCOUNT_TYPE_FULL_REDUCTION) {
        printf("满减条件：满 ¥%.2f 减 ¥%.2f\n", discount->minAmount, discount->discountAmount);
    }

    char startTimeStr[20], endTimeStr[20];

    const struct tm *timeInfo = localtime(&discount->startTime);
    strftime(startTimeStr, sizeof(startTimeStr), "%Y-%m-%d %H:%M", timeInfo);

    timeInfo = localtime(&discount->endTime);
    strftime(endTimeStr, sizeof(endTimeStr), "%Y-%m-%d %H:%M", timeInfo);

    printf("开始时间：%s\n", startTimeStr);
    printf("结束时间：%s\n", endTimeStr);

    const time_t currentTime = time(NULL);
    const int isExpired = discount->endTime < currentTime;
    const int notStarted = discount->startTime > currentTime;

    printf("激活状态：%s\n", discount->isActive ? "✅ 已激活" : "❌ 未激活");
    printf("时间状态：");
    if (notStarted) {
        printf("未开始\n");
    } else if (isExpired) {
        printf("已过期\n");
    } else {
        printf("进行中\n");
    }
    printf("删除状态：%s\n", discount->isDeleted ? "❌ 已删除" : "✅ 未删除");
}

void displayDiscountsByMerchantId(Discount result[], const int count) {
    if (count == 0) {
        printf("现在还没有折扣活动，请先添加活动！\n");
        return;
    }

    const int idWidth = 4;
    const int nameWidth = 20;
    const int typeWidth = 12;
    const int contentWidth = 15;
    const int startTimeWidth = 12;
    const int endTimeWidth = 12;
    const int statusWidth = 8;

    printPadded("ID", idWidth);
    printPadded("活动名称", nameWidth);
    printPadded("类型", typeWidth);
    printPadded("优惠内容", contentWidth);
    printPadded("开始时间", startTimeWidth);
    printPadded("结束时间", endTimeWidth);
    printPadded("状态", statusWidth);
    printf("\n");
    printSubLine();

    const time_t currentTime = time(NULL);

    for (int i = 0; i < count; i++) {
        const char *typeStr;
        char discountInfo[20];
        char startStr[12], endStr[12];

        switch (result[i].type) {
            case DISCOUNT_TYPE_PERCENTAGE:
                typeStr = "百分比";
                snprintf(discountInfo, sizeof(discountInfo), "%.0f 折", result[i].discountRate * 10);
                break;
            case DISCOUNT_TYPE_FIXED:
                typeStr = "固定金额";
                snprintf(discountInfo, sizeof(discountInfo), "减 ¥%.1f", result[i].discountAmount);
                break;
            case DISCOUNT_TYPE_FULL_REDUCTION:
                typeStr = "满减";
                snprintf(discountInfo, sizeof(discountInfo), "满 %.0f 减 %.0f",
                         result[i].minAmount, result[i].discountAmount);
                break;
            default:
                typeStr = "未知";
                strcpy(discountInfo, "-");
        }

        const struct tm *timeInfo = localtime(&result[i].startTime);
        strftime(startStr, sizeof(startStr), "%m-%d %H:%M", timeInfo);

        timeInfo = localtime(&result[i].endTime);
        strftime(endStr, sizeof(endStr), "%m-%d %H:%M", timeInfo);

        const char *status;
        if (!result[i].isActive) {
            status = "❌";
        } else if (result[i].endTime < currentTime) {
            status = "<-";
        } else if (result[i].startTime > currentTime) {
            status = "->";
        } else {
            status = "✅";
        }

        char idStr[20];
        sprintf(idStr, "%d", result[i].id);
        printPadded(idStr, idWidth);

        printPadded(result[i].name, nameWidth);

        printPadded(typeStr, typeWidth);

        printPadded(discountInfo, contentWidth);

        printPadded(startStr, startTimeWidth);

        printPadded(endStr, endTimeWidth);

        printPadded(status, statusWidth);

        printf("\n");
    }

    printf("\n共有 %d 个折扣活动！\n", count);
}

void addDiscount(const int merchantId) {
    if (discountCount >= MAX_DISCOUNTS) {
        printError("折扣活动数量已达上限，无法添加新活动！");
        return;
    }

    Discount newDiscount = {0};
    newDiscount.merchantId = merchantId;

    printHeader("添加折扣活动");
    printf("商品概览 > 优惠政策 > 添加折扣活动\n");
    printSubLine();

    do {
        printf("请输入活动名称（输入 0 退出）：");
        if (scanf_s("%49s", newDiscount.name, (unsigned) sizeof(newDiscount.name)) != 1) {
            printError("输入格式错误！");
            continue;
        }

        if (strcmp(newDiscount.name, "0") == 0) {
            printWarning("已取消添加折扣活动！");
            return;
        }

        if (strlen(newDiscount.name) == 0) {
            printError("活动名称不能为空！");
            continue;
        }
        break;
    } while (1);

    do {
        printSubLine();
        printf("活动类型：\n");
        printf("1. 百分比折扣（如：8 折）\n");
        printf("2. 固定金额折扣（如：减 10 元）\n");
        printf("3. 满减活动（如：满 100 减 20）\n");
        printSubLine();
        printf("请选择活动类型（输入 0 退出）：");

        int typeChoice;
        if (scanf_s("%d", &typeChoice) != 1) {
            printError("输入格式错误！");
            continue;
        }

        switch (typeChoice) {
            case 1:
                newDiscount.type = DISCOUNT_TYPE_PERCENTAGE;
                break;
            case 2:
                newDiscount.type = DISCOUNT_TYPE_FIXED;
                break;
            case 3:
                newDiscount.type = DISCOUNT_TYPE_FULL_REDUCTION;
                break;
            case 0:
                printWarning("已取消添加折扣活动！");
                return;
            default:
                printError("无效的选择！");
                continue;
        }
        break;
    } while (1);

    if (newDiscount.type == DISCOUNT_TYPE_PERCENTAGE) {
        do {
            printf("请输入折扣率（如：0.8 表示 8 折，输入 0 退出）：");
            if (scanf_s("%lf", &newDiscount.discountRate) != 1) {
                printError("输入格式错误！");
                continue;
            }

            if (newDiscount.discountRate == 0) {
                printWarning("已取消添加折扣活动！");
                return;
            }

            if (newDiscount.discountRate <= 0 || newDiscount.discountRate >= 1) {
                printError("折扣率必须在 0-1 之间！");
                continue;
            }
            break;
        } while (1);
    } else if (newDiscount.type == DISCOUNT_TYPE_FIXED) {
        do {
            printf("请输入折扣金额（输入 0 退出）：");
            if (scanf_s("%lf", &newDiscount.discountAmount) != 1) {
                printError("输入格式错误！");
                continue;
            }

            if (newDiscount.discountAmount == 0) {
                printWarning("已取消添加折扣活动！");
                return;
            }

            if (newDiscount.discountAmount <= 0) {
                printError("折扣金额必须大于 0！");
                continue;
            }
            break;
        } while (1);
    } else {
        do {
            printf("请输入满减门槛金额（输入 0 退出）：");
            if (scanf_s("%lf", &newDiscount.minAmount) != 1) {
                printError("输入格式错误！");
                continue;
            }

            if (newDiscount.minAmount == 0) {
                printWarning("已取消添加折扣活动！");
                return;
            }

            if (newDiscount.minAmount <= 0) {
                printError("门槛金额必须大于 0！");
                continue;
            }
            break;
        } while (1);

        do {
            printf("请输入优惠金额（输入 0 退出）：");
            if (scanf_s("%lf", &newDiscount.discountAmount) != 1) {
                printError("输入格式错误！");
                continue;
            }

            if (newDiscount.discountAmount == 0) {
                printWarning("已取消添加折扣活动！");
                return;
            }

            if (newDiscount.discountAmount <= 0 || newDiscount.discountAmount >= newDiscount.minAmount) {
                printError("优惠金额必须大于 0 且小于门槛金额！");
                continue;
            }
            break;
        } while (1);
    }

    const time_t currentTime = time(NULL);

    newDiscount.startTime = currentTime;

    do {
        int days;
        printf("请输入活动持续天数（输入 0 退出）：");
        if (scanf_s("%d", &days) != 1) {
            printError("输入格式错误！");
            continue;
        }

        if (days == 0) {
            printWarning("已取消添加折扣活动！");
            return;
        }

        if (days < 0) {
            printError("持续天数必须大于 0！");
            continue;
        }
        newDiscount.endTime = currentTime + days * 24 * 3600;
        break;
    } while (1);

    int c;
    printf("是否立即激活该活动（Y/N，输入 0 退出）：");
    while ((c = getchar()) != '\n' && c != EOF) {
    }
    char choice = (char) getchar();
    choice = (char) tolower(choice);

    if (choice == '0') {
        printWarning("已取消添加折扣活动！");
        return;
    }

    if (choice != 'y' && choice != 'n') {
        printWarning("输入无效，将设置为不激活！");
        choice = 'n';
    }

    newDiscount.isActive = choice == 'y';
    newDiscount.isDeleted = 0;
    newDiscount.id = discountCount > 0 ? discounts[discountCount - 1].id + 1 : 1;

    discounts[discountCount] = newDiscount;
    discountCount++;
    saveDiscounts();

    displayDiscountDetails(&newDiscount);
    printSuccess("折扣活动添加成功！");
}

void queryDiscounts(const int merchantId) {
    int choice;
    do {
        printHeader("查询折扣活动");
        printf("商品概览 > 优惠政策 > 查询折扣活动\n");
        printSubLine();
        printf("0. 返回上级菜单\n");
        printf("1. 查看所有活动\n");
        printf("2. 查看进行中活动\n");
        printf("3. 查看活动详情\n");
        printSubLine();
        printInputHint();

        if (scanf_s("%d", &choice) != 1) {
            printError("输入格式错误！");
            continue;
        }

        switch (choice) {
            case 1: {
                Discount result[MAX_DISCOUNTS];
                const int count = findDiscountsByMerchantId(merchantId, result);
                printHeader("所有活动");
                printf("商品概览 > 优惠政策 > 查询折扣活动 > 所有活动\n");
                printSubLine();
                displayDiscountsByMerchantId(result, count);
                printSubLine();
                pauseScreen();
                break;
            }
            case 2: {
                Discount result[MAX_DISCOUNTS];
                const int count = findActiveDiscountsByMerchantId(merchantId, result);
                if (count > 0) {
                    printHeader("进行中的活动");
                    printf("商品概览 > 优惠政策 > 查询折扣活动 > 进行中的活动\n");
                    printSubLine();
                    // printf_s("找到 %d 个进行中的活动：", count);
                    displayDiscountsByMerchantId(result, count);
                    printSubLine();
                    pauseScreen();
                } else {
                    printWarning("没有进行中的活动！");
                }
                break;
            }
            case 3: {
                int discountId;
                printHeader("活动详情");
                printf("商品概览 > 优惠政策 > 查询折扣活动 > 活动详情\n");
                printSubLine();
                printf("请输入活动 ID：");
                if (scanf_s("%d", &discountId) != 1) {
                    printError("输入格式错误！");
                    break;
                }

                const int index = findDiscountById(discountId);
                if (index == -1 || discounts[index].merchantId != merchantId) {
                    printError("活动不存在或无权访问！");
                    break;
                }

                displayDiscountDetails(&discounts[index]);
                printSubLine();
                pauseScreen();
                break;
            }
            case 0:
                return;
            default:
                printError("无效选择，请重新输入！");
                break;
        }
    } while (choice != 0);
}

void manageDiscountStatus(const int merchantId) {
    Discount allDiscounts[MAX_DISCOUNTS];
    const int count = findDiscountsByMerchantId(merchantId, allDiscounts);

    if (count == 0) {
        printWarning("您还没有折扣活动，请先添加活动！");
        return;
    }

    printHeader("管理活动状态");
    printf("商品概览 > 优惠政策 > 管理活动状态\n");
    printSubLine();
    displayDiscountsByMerchantId(allDiscounts, count);
    printSubLine();

    int discountId;
    printf("请输入要操作的活动 ID：");
    if (scanf_s("%d", &discountId) != 1) {
        printError("输入格式错误！");
        return;
    }

    const int index = findDiscountById(discountId);
    if (index == -1 || discounts[index].merchantId != merchantId) {
        printError("活动不存在或无权操作！");
        return;
    }

    Discount *discount = &discounts[index];

    printSubLine();
    printf("当前活动：%s\n", discount->name);
    printf("当前状态：%s\n", discount->isActive ? "✅ 已激活" : "❌ 未激活");
    printSubLine();

    char confirm;
    printf("确认%s该活动？（Y/N）：", discount->isActive ? "禁用" : "激活");
    while (getchar() != '\n') {
    }
    scanf_s("%c", &confirm, 1);

    if (tolower(confirm) == 'y') {
        discount->isActive = !discount->isActive;
        saveDiscounts();
        printSubLine();
        printf("新状态：%s\n", discount->isActive ? "✅ 已激活" : "❌ 未激活");
        printSuccess("活动状态更新成功！");
    } else {
        printWarning("操作已取消！");
    }
}

double calculateDiscountedPrice(const double originalPrice, const Discount *discount) {
    if (discount == NULL || !discount->isActive) {
        return originalPrice;
    }

    const time_t currentTime = time(NULL);
    if (currentTime < discount->startTime || currentTime > discount->endTime) {
        return originalPrice;
    }

    switch (discount->type) {
        case DISCOUNT_TYPE_PERCENTAGE:
            return originalPrice * discount->discountRate;
        case DISCOUNT_TYPE_FIXED:
            return originalPrice - discount->discountAmount;
        case DISCOUNT_TYPE_FULL_REDUCTION:
            if (originalPrice >= discount->minAmount) {
                return originalPrice - discount->discountAmount;
            }
            return originalPrice;
        default:
            return originalPrice;
    }
}

int isValidDiscount(const Discount *discount) {
    if (discount == NULL || !discount->isActive || discount->isDeleted) {
        return 0;
    }

    const time_t currentTime = time(NULL);
    return (currentTime >= discount->startTime && currentTime <= discount->endTime);
}

int activeDiscount(const int discountId) {
    const int index = findDiscountById(discountId);
    if (index == -1) {
        printError("折扣活动不存在！");
        return -1;
    }
    discounts[index].isActive = 1;
    saveDiscounts();
    return 1;
}

int unactiveDiscount(const int discountId) {
    const int index = findDiscountById(discountId);
    if (index == -1) {
        printError("折扣活动不存在！");
        return -1;
    }
    discounts[index].isActive = 0;
    saveDiscounts();
    return 1;
}

int toggleDiscountStatus(const int discountId) {
    const int index = findDiscountById(discountId);
    if (index == -1) {
        printError("折扣活动不存在！");
        return -1;
    }
    discounts[index].isActive = !discounts[index].isActive;
    saveDiscounts();
    return discounts[index].isActive;
}
