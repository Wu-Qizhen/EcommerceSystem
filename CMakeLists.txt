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
        src/client/client_menu.c
        src/merchant/merchant_menu.c
        src/client/cart.c
        src/client/order.c
        src/merchant/product.c
        src/merchant/discount.c
        # src/client/address.c
)

# 商户端源文件
set(MERCHANT_SOURCES
        src/merchant/merchant_menu.c
        src/client/client_menu.c
        src/merchant/product.c
        src/merchant/discount.c
)

# 创建客户端可执行文件和数据目录
add_executable(EcommerceClient
        src/client/client_main.c
        ${COMMON_SOURCES}
        ${CLIENT_SOURCES}
)

add_custom_command(TARGET EcommerceClient POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data
)

# 创建商家端可执行文件和数据目录
#[[add_executable(EcommerceMerchant
        src/merchant/merchant_main.c
        ${COMMON_SOURCES}
        ${MERCHANT_SOURCES}
)

add_custom_command(TARGET EcommerceMerchant POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/data
)]]
