cmake_minimum_required(VERSION 3.10)
project(client_demo)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)


include_directories(
    ${PROJECT_SOURCE_DIR}/include

)
link_directories(${DBUS_LIBRARY_DIRS})


file(GLOB SRC ${PROJECT_SOURCE_DIR}/sources/*.cpp)

# 头文件路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${DBUS_INCLUDE_DIRS}
)


add_executable(client ${SRC})

# 链接 DBus 库
target_link_libraries(client ${DBUS_LIBRARIES})