cmake_minimum_required(VERSION 3.10)
project(server_demo)

set(CMAKE_CXX_STANDARD 17)

# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /opt/homebrew/opt/dbus/include/dbus-1.0
    /opt/homebrew/opt/dbus/lib/dbus-1.0/include
)

# 库文件路径 (必加)
link_directories(
    /opt/homebrew/opt/dbus/lib
)

file(GLOB SRC ${PROJECT_SOURCE_DIR}/sources/*.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

add_executable(server ${SRC})

# 链接 DBus 库
target_link_libraries(server dbus-1)
