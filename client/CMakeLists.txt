cmake_minimum_required(VERSION 3.10)
project(client_demo)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SRC ${PROJECT_SOURCE_DIR}/sources/*.cpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)


add_executable(client ${SRC})

# 链接 DBus 库
target_link_libraries(client ${DBUS_LIBRARIES})

# 新增：显式添加 DBus 头文件路径
target_include_directories(client PRIVATE ${DBUS_INCLUDE_DIRS})

