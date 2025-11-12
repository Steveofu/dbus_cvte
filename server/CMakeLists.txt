cmake_minimum_required(VERSION 3.10)
project(server_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "-g")


find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)


link_directories(${DBUS_LIBRARY_DIRS})


find_package(OpenSSL REQUIRED)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/sources/*.cpp)

# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${DBUS_INCLUDE_DIRS}
)
include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(server ${SRC})

# 链接 DBus 库
target_link_libraries(server ${DBUS_LIBRARIES})
target_link_libraries(server rt)  # 将 rt 库链接到你的服务器可执行文件
target_link_libraries(server ${DBUS_LIBRARIES} ssl crypto)

