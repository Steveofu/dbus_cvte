#ifndef TEST_SERVICE_H
#define TEST_SERVICE_H

#include <dbus/dbus.h>
#include <stdbool.h>
#include <stdint.h>

// 确保bool类型可用
#ifndef bool
#define bool _Bool
#define true 1
#define false 0
#endif

// D-Bus 服务相关常量
#define SERVICE_NAME "com.example.TestService"
#define SERVICE_PATH "/com/example/TestService"
#define SERVICE_INTERFACE "com.example.TestService"

// TestInfo 结构体定义
typedef struct {
    bool bool_param;
    int int_param;
    double double_param;
    char* string_param;
} TestInfo;

// 服务端数据存储结构
typedef struct {
    bool test_bool;
    int test_int;
    double test_double;
    char* test_string;
    TestInfo test_info;
} ServiceData;

// 函数声明
int start_dbus_service(void);
DBusHandlerResult handle_method_call(DBusConnection* connection, DBusMessage* message, void* user_data);
void cleanup_service_data(ServiceData* data);

// 方法处理函数
DBusMessage* handle_set_test_bool(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_get_test_bool(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_set_test_int(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_get_test_int(DBusMessage* msg, ServiceData* data);

// 新增
DBusMessage* handle_set_test_double(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_get_test_double(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_set_test_string(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_get_test_string(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_set_test_info(DBusMessage* msg, ServiceData* data);
DBusMessage* handle_get_test_info(DBusMessage* msg, ServiceData* data);

// 辅助函数
bool serialize_test_info(DBusMessage* msg, const TestInfo* info);
bool deserialize_test_info(DBusMessage* msg, TestInfo* info);

#endif // TEST_SERVICE_H