#ifndef TEST_CLIENT_H
#define TEST_CLIENT_H

#include <dbus/dbus.h>
#include <stdbool.h>
#include <stdint.h>

// D-Bus 服务相关常量（与服务端保持一致）
#define SERVICE_NAME "com.example.TestService"
#define SERVICE_PATH "/com/example/TestService"
#define SERVICE_INTERFACE "com.example.TestService"

// TestInfo 结构体定义（与服务端保持一致）
typedef struct {
    bool bool_param;
    int int_param;
    double double_param;
    char* string_param;
} TestInfo;

// 客户端连接结构
typedef struct {
    DBusConnection* connection;
    DBusError error;
} ClientConnection;

// 函数声明
ClientConnection* init_client_connection(void);
void cleanup_client_connection(ClientConnection* client);

// 布尔值接口函数
bool set_test_bool(ClientConnection* client, bool value);
bool get_test_bool(ClientConnection* client, bool* value);

// 整数接口函数
bool set_test_int(ClientConnection* client, int value);
bool get_test_int(ClientConnection* client, int* value);

// 浮点数接口函数
bool set_test_double(ClientConnection* client, double value);
bool get_test_double(ClientConnection* client, double* value);

// 字符串接口函数
bool set_test_string(ClientConnection* client, const char* value);
bool get_test_string(ClientConnection* client, char** value);

// TestInfo结构体接口函数
bool set_test_info(ClientConnection* client, const TestInfo* info);
bool get_test_info(ClientConnection* client, TestInfo* info);

// 辅助函数
void show_menu(void);
void handle_user_input(ClientConnection* client);
void cleanup_test_info(TestInfo* info);

#endif // TEST_CLIENT_H