cmake_minimum_required(VERSION 3.10)
project(TestClient C)

# 设置C标准
set(CMAKE_C_STANDARD 99)

# 查找依赖库
find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)

# 包含头文件目录
include_directories(Include)
include_directories(${DBUS_INCLUDE_DIRS})

# 收集源文件
file(GLOB SOURCES "Sources/*.c")

# 设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -DDEBUG")
    message(STATUS "Debug build configured")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -DNDEBUG -s")
    message(STATUS "Release build configured")
endif()

# 创建可执行文件
add_executable(client ${SOURCES})

# 链接库
target_link_libraries(client ${DBUS_LIBRARIES})
target_compile_options(client PRIVATE ${DBUS_CFLAGS_OTHER})

# 设置输出目录
set_target_properties(client PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# 安装规则
install(TARGETS client DESTINATION bin)
