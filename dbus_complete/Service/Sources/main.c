#include "../Include/test_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

// 全局服务数据
ServiceData g_service_data = {0};
volatile bool g_running = true;

// 信号处理函数
void signal_handler(int sig) {
    printf("收到信号 %d，准备退出...\n", sig);
    g_running = false;
}

DBusHandlerResult handle_method_call(DBusConnection* connection, DBusMessage* message, void* user_data) {
    const char* method_name = dbus_message_get_member(message);
    const char* interface_name = dbus_message_get_interface(message);
    const char* path = dbus_message_get_path(message);
    
    printf("=== 收到D-Bus消息 ===\n");
    printf("路径: %s\n", path ? path : "NULL");
    printf("接口: %s\n", interface_name ? interface_name : "NULL");
    printf("方法: %s\n", method_name ? method_name : "NULL");
    
    // 检查是否是我们的接口
    if (!interface_name || strcmp(interface_name, SERVICE_INTERFACE) != 0) {
        printf("接口不匹配，忽略消息\n");
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
    
    if (!method_name) {
        printf("方法名为空，忽略消息\n");
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    }
    
    printf("处理方法调用: %s\n", method_name);
    
    DBusMessage* reply = NULL;
    
    // 布尔值方法
    if (strcmp(method_name, "SetTestBool") == 0) {
        printf("调用 handle_set_test_bool\n");
        reply = handle_set_test_bool(message, &g_service_data);
    } else if (strcmp(method_name, "GetTestBool") == 0) {
        printf("调用 handle_get_test_bool\n");
        reply = handle_get_test_bool(message, &g_service_data);
    }
    // 整数方法
    else if (strcmp(method_name, "SetTestInt") == 0) {
        printf("调用 handle_set_test_int\n");
        reply = handle_set_test_int(message, &g_service_data);
    } else if (strcmp(method_name, "GetTestInt") == 0) {
        printf("调用 handle_get_test_int\n");
        reply = handle_get_test_int(message, &g_service_data);
    }
    // 浮点数方法
    else if (strcmp(method_name, "SetTestDouble") == 0) {
        printf("调用 handle_set_test_double\n");
        reply = handle_set_test_double(message, &g_service_data);
    } else if (strcmp(method_name, "GetTestDouble") == 0) {
        printf("调用 handle_get_test_double\n");
        reply = handle_get_test_double(message, &g_service_data);
    }
    // 字符串方法
    else if (strcmp(method_name, "SetTestString") == 0) {
        printf("调用 handle_set_test_string\n");
        reply = handle_set_test_string(message, &g_service_data);
    } else if (strcmp(method_name, "GetTestString") == 0) {
        printf("调用 handle_get_test_string\n");
        reply = handle_get_test_string(message, &g_service_data);
    }
    // TestInfo结构体方法
    else if (strcmp(method_name, "SetTestInfo") == 0) {
        printf("调用 handle_set_test_info\n");
        reply = handle_set_test_info(message, &g_service_data);
    } else if (strcmp(method_name, "GetTestInfo") == 0) {
        printf("调用 handle_get_test_info\n");
        reply = handle_get_test_info(message, &g_service_data);
    }
    else {
        printf("未知方法: %s\n", method_name);
        reply = dbus_message_new_error(message, DBUS_ERROR_UNKNOWN_METHOD, "Unknown method");
    }
    
    if (reply) {
        printf("发送回复消息\n");
        if (!dbus_connection_send(connection, reply, NULL)) {
            printf("发送回复失败\n");
        }
        dbus_message_unref(reply);
        dbus_connection_flush(connection);
        printf("回复发送完成\n");
    } else {
        printf("警告: 没有创建回复消息\n");
    }
    
    printf("=== 消息处理完成 ===\n\n");
    return DBUS_HANDLER_RESULT_HANDLED;
}

// 布尔值处理函数
DBusMessage* handle_set_test_bool(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 SetTestBool\n");
    DBusError error;
    dbus_bool_t value;
    
    dbus_error_init(&error);
    
    if (!dbus_message_get_args(msg, &error,
                              DBUS_TYPE_BOOLEAN, &value,
                              DBUS_TYPE_INVALID)) {
        printf("参数解析失败: %s\n", error.message);
        dbus_error_free(&error);
        return dbus_message_new_error(msg, DBUS_ERROR_INVALID_ARGS, "Invalid arguments");
    }
    
    data->test_bool = (bool)value;
    printf("设置 test_bool = %s\n", data->test_bool ? "true" : "false");
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_bool_t success = TRUE;
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_BOOLEAN, &success,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    printf("SetTestBool 处理完成\n");
    return reply;
}

DBusMessage* handle_get_test_bool(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 GetTestBool\n");
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_bool_t value = (dbus_bool_t)data->test_bool;
    
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_BOOLEAN, &value,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    printf("返回 test_bool = %s\n", data->test_bool ? "true" : "false");
    return reply;
}

// 整数处理函数
DBusMessage* handle_set_test_int(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 SetTestInt\n");
    DBusError error;
    dbus_int32_t value;
    
    dbus_error_init(&error);
    
    if (!dbus_message_get_args(msg, &error,
                              DBUS_TYPE_INT32, &value,
                              DBUS_TYPE_INVALID)) {
        printf("参数解析失败: %s\n", error.message);
        dbus_error_free(&error);
        return dbus_message_new_error(msg, DBUS_ERROR_INVALID_ARGS, "Invalid arguments");
    }
    
    data->test_int = (int)value;
    printf("设置 test_int = %d\n", data->test_int);
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_bool_t success = TRUE;
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_BOOLEAN, &success,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    return reply;
}

DBusMessage* handle_get_test_int(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 GetTestInt\n");
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_int32_t value = (dbus_int32_t)data->test_int;
    
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_INT32, &value,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    printf("返回 test_int = %d\n", data->test_int);
    return reply;
}

// 浮点数处理函数
DBusMessage* handle_set_test_double(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 SetTestDouble\n");
    DBusError error;
    double value;
    
    dbus_error_init(&error);
    
    if (!dbus_message_get_args(msg, &error,
                              DBUS_TYPE_DOUBLE, &value,
                              DBUS_TYPE_INVALID)) {
        printf("参数解析失败: %s\n", error.message);
        dbus_error_free(&error);
        return dbus_message_new_error(msg, DBUS_ERROR_INVALID_ARGS, "Invalid arguments");
    }
    
    data->test_double = value;
    printf("设置 test_double = %f\n", data->test_double);
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_bool_t success = TRUE;
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_BOOLEAN, &success,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    return reply;
}

DBusMessage* handle_get_test_double(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 GetTestDouble\n");
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_DOUBLE, &data->test_double,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    printf("返回 test_double = %f\n", data->test_double);
    return reply;
}

// 字符串处理函数
DBusMessage* handle_set_test_string(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 SetTestString\n");
    DBusError error;
    const char* value;
    
    dbus_error_init(&error);
    
    if (!dbus_message_get_args(msg, &error,
                              DBUS_TYPE_STRING, &value,
                              DBUS_TYPE_INVALID)) {
        printf("参数解析失败: %s\n", error.message);
        dbus_error_free(&error);
        return dbus_message_new_error(msg, DBUS_ERROR_INVALID_ARGS, "Invalid arguments");
    }
    
    // 释放之前的字符串
    if (data->test_string) {
        free(data->test_string);
    }
    
    // 复制新字符串
    data->test_string = strdup(value);
    printf("设置 test_string = %s\n", data->test_string);
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_bool_t success = TRUE;
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_BOOLEAN, &success,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    return reply;
}

DBusMessage* handle_get_test_string(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 GetTestString\n");
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    const char* value = data->test_string ? data->test_string : "";
    
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_STRING, &value,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    printf("返回 test_string = %s\n", value);
    return reply;
}

// TestInfo结构体处理函数
DBusMessage* handle_set_test_info(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 SetTestInfo\n");
    DBusError error;
    dbus_bool_t bool_param;
    dbus_int32_t int_param;
    double double_param;
    const char* string_param;
    
    dbus_error_init(&error);
    
    if (!dbus_message_get_args(msg, &error,
                              DBUS_TYPE_BOOLEAN, &bool_param,
                              DBUS_TYPE_INT32, &int_param,
                              DBUS_TYPE_DOUBLE, &double_param,
                              DBUS_TYPE_STRING, &string_param,
                              DBUS_TYPE_INVALID)) {
        printf("参数解析失败: %s\n", error.message);
        dbus_error_free(&error);
        return dbus_message_new_error(msg, DBUS_ERROR_INVALID_ARGS, "Invalid arguments");
    }
    
    // 更新TestInfo数据
    data->test_info.bool_param = (bool)bool_param;
    data->test_info.int_param = (int)int_param;
    data->test_info.double_param = double_param;
    
    // 释放之前的字符串
    if (data->test_info.string_param) {
        free(data->test_info.string_param);
    }
    data->test_info.string_param = strdup(string_param);
    
    printf("设置 test_info = {bool: %s, int: %d, double: %f, string: %s}\n",
           data->test_info.bool_param ? "true" : "false",
           data->test_info.int_param,
           data->test_info.double_param,
           data->test_info.string_param);
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_bool_t success = TRUE;
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_BOOLEAN, &success,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    return reply;
}

DBusMessage* handle_get_test_info(DBusMessage* msg, ServiceData* data) {
    printf("开始处理 GetTestInfo\n");
    
    DBusMessage* reply = dbus_message_new_method_return(msg);
    if (!reply) {
        printf("创建回复消息失败\n");
        return NULL;
    }
    
    dbus_bool_t bool_param = (dbus_bool_t)data->test_info.bool_param;
    dbus_int32_t int_param = (dbus_int32_t)data->test_info.int_param;
    const char* string_param = data->test_info.string_param ? data->test_info.string_param : "";
    
    if (!dbus_message_append_args(reply,
                                 DBUS_TYPE_BOOLEAN, &bool_param,
                                 DBUS_TYPE_INT32, &int_param,
                                 DBUS_TYPE_DOUBLE, &data->test_info.double_param,
                                 DBUS_TYPE_STRING, &string_param,
                                 DBUS_TYPE_INVALID)) {
        printf("添加回复参数失败\n");
        dbus_message_unref(reply);
        return NULL;
    }
    
    printf("返回 test_info = {bool: %s, int: %d, double: %f, string: %s}\n",
           data->test_info.bool_param ? "true" : "false",
           data->test_info.int_param,
           data->test_info.double_param,
           string_param);
    
    return reply;
}

int start_dbus_service(void) {
    DBusConnection* connection = NULL;
    DBusError error;
    int ret;
    
    dbus_error_init(&error);
    
    printf("正在连接D-Bus会话总线...\n");
    
    // 连接到会话总线
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        printf("连接D-Bus失败: %s\n", error.message);
        dbus_error_free(&error);
        return -1;
    }
    printf("D-Bus连接成功\n");
    
    // 设置连接不自动退出
    dbus_connection_set_exit_on_disconnect(connection, FALSE);
    
    printf("正在请求服务名称: %s\n", SERVICE_NAME);
    
    // 请求服务名称
    ret = dbus_bus_request_name(connection, SERVICE_NAME,
                               DBUS_NAME_FLAG_REPLACE_EXISTING,
                               &error);
    if (dbus_error_is_set(&error)) {
        printf("请求服务名称失败: %s\n", error.message);
        dbus_error_free(&error);
        return -1;
    }
    
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        printf("不是服务的主要拥有者，返回值: %d\n", ret);
        return -1;
    }
    
    printf("D-Bus服务启动成功: %s\n", SERVICE_NAME);
    
    // 创建虚拟表结构
    static DBusObjectPathVTable vtable = {
        .message_function = handle_method_call,
        .unregister_function = NULL
    };
    
    printf("正在注册对象路径: %s\n", SERVICE_PATH);
    
    // 注册对象路径和消息处理函数
    if (!dbus_connection_register_object_path(connection, SERVICE_PATH,
                                             &vtable, NULL)) {
        printf("注册对象路径失败\n");
        return -1;
    }
    
    printf("对象路径注册成功\n");
    printf("开始监听D-Bus消息... (按 Ctrl+C 退出)\n");
    printf("等待客户端连接...\n\n");
    
    // 主循环 - 使用更简单的循环
    while (g_running) {
        // 处理待处理的消息
        dbus_connection_read_write_dispatch(connection, 100);
        
        // 检查连接状态
        if (!dbus_connection_get_is_connected(connection)) {
            printf("D-Bus连接断开\n");
            break;
        }
    }
    
    printf("服务主循环退出\n");
    
    // 清理连接
    if (connection) {
        dbus_connection_unref(connection);
    }
    
    return 0;
}

void cleanup_service_data(ServiceData* data) {
    if (data->test_string) {
        free(data->test_string);
        data->test_string = NULL;
    }
    if (data->test_info.string_param) {
        free(data->test_info.string_param);
        data->test_info.string_param = NULL;
    }
}

int main() {
    printf("启动Test Service...\n");
    
    // 设置信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    // 检查D-Bus环境变量
    const char* dbus_address = getenv("DBUS_SESSION_BUS_ADDRESS");
    if (!dbus_address || strlen(dbus_address) == 0) {
        printf("警告: DBUS_SESSION_BUS_ADDRESS 环境变量未设置\n");
        printf("请先启动D-Bus会话: eval `dbus-launch --sh-syntax`\n");
        return -1;
    }
    printf("D-Bus会话地址: %s\n", dbus_address);
    
    // 初始化服务数据
    memset(&g_service_data, 0, sizeof(ServiceData));
    
    // 启动D-Bus服务
    int result = start_dbus_service();
    
    // 清理资源
    cleanup_service_data(&g_service_data);
    
    printf("服务程序退出，返回码: %d\n", result);
    return result;
}