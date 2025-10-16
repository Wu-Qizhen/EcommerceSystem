cmake_minimum_required(VERSION 4.0)
project(EcommerceSystem C)

set(CMAKE_C_STANDARD 11)

# 包含目录
include_directories(include)
include_directories(include/common)
include_directories(include/client)
include_directories(include/merchant)

# 公共源文件
set(COMMON_SOURCES
        src/common/auth/login.c
        src/common/auth/register.c
        src/common/auth/reset.c
        src/common/user/user.c
        src/common/security/validation.c
        src/common/security/encryption.c
        src/common/utils/control_util.c
        src/common/utils/print_util.c
)

# 客户端源文件
set(CLIENT_SOURCES
        # src/client/client_menu.c
        # src/client/shopping_cart.c
        # src/client/order.c
        # src/client/address.c
)

# 商户端源文件
set(MERCHANT_SOURCES
        src/merchant/merchant_menu.c
        src/merchant/product.c
        src/merchant/discount.c
        # src/merchant/inventory_alert.c
)

# 创建客户端可执行文件
#[[add_executable(EcommerceClient
        src/client/client_main.c
        ${COMMON_SOURCES}
        ${CLIENT_SOURCES}
)]]

# 创建商户端可执行文件
add_executable(EcommerceMerchant
        src/merchant/merchant_main.c
        ${COMMON_SOURCES}
        ${MERCHANT_SOURCES}
)

# 创建数据目录
#[[add_custom_command(TARGET EcommerceClient POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data
)]]

add_custom_command(TARGET EcommerceMerchant POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data
)

#[[add_executable(EcommerceSystem src/client/client_main.c
        src/common/utils/print_util.c
        src/common/utils/print_util.h
        src/common/utils/control_util.c
        src/common/utils/control_util.h
        src/common/user/user.c
        src/common/user/user.h
        src/common/security/encryption.c
        src/common/security/encryption.h
        src/common/auth/register.c
        src/common/auth/register.h
        src/common/security/validation.c
        src/common/security/validation.h
        src/common/auth/login.c
        src/common/auth/login.h
        src/common/auth/reset.c
        src/common/auth/reset.h
)]]
