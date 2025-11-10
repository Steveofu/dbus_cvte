#include "../Include/test_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ClientConnection* init_client_connection(void) {
    ClientConnection* client = malloc(sizeof(ClientConnection));
    if (!client) {
        printf("内存分配失败\n");
        return NULL;
    }
    
    dbus_error_init(&client->error);
    
    // 连接到会话总线
    client->connection = dbus_bus_get(DBUS_BUS_SESSION, &client->error);
    if (dbus_error_is_set(&client->error)) {
        printf("连接D-Bus失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        free(client);
        return NULL;
    }
    
    printf("客户端D-Bus连接建立成功\n");
    return client;
}

void cleanup_client_connection(ClientConnection* client) {
    if (client) {
        if (dbus_error_is_set(&client->error)) {
            dbus_error_free(&client->error);
        }
        free(client);
    }
}

void cleanup_test_info(TestInfo* info) {
    if (info && info->string_param) {
        free(info->string_param);
        info->string_param = NULL;
    }
}

// 布尔值接口函数
bool set_test_bool(ClientConnection* client, bool value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "SetTestBool");
    if (!msg) return false;
    
    dbus_bool_t dbus_value = (dbus_bool_t)value;
    if (!dbus_message_append_args(msg, DBUS_TYPE_BOOLEAN, &dbus_value, DBUS_TYPE_INVALID)) {
        dbus_message_unref(msg);
        return false;
    }
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_bool_t result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_BOOLEAN, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    printf("SetTestBool(%s) 调用成功\n", value ? "true" : "false");
    return (bool)result;
}

bool get_test_bool(ClientConnection* client, bool* value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "GetTestBool");
    if (!msg) return false;
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_bool_t result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_BOOLEAN, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    *value = (bool)result;
    printf("GetTestBool() 返回: %s\n", *value ? "true" : "false");
    return true;
}

// 整数接口函数
bool set_test_int(ClientConnection* client, int value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "SetTestInt");
    if (!msg) return false;
    
    dbus_int32_t dbus_value = (dbus_int32_t)value;
    if (!dbus_message_append_args(msg, DBUS_TYPE_INT32, &dbus_value, DBUS_TYPE_INVALID)) {
        dbus_message_unref(msg);
        return false;
    }
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_bool_t result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_BOOLEAN, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    printf("SetTestInt(%d) 调用成功\n", value);
    return (bool)result;
}

bool get_test_int(ClientConnection* client, int* value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "GetTestInt");
    if (!msg) return false;
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_int32_t result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_INT32, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    *value = (int)result;
    printf("GetTestInt() 返回: %d\n", *value);
    return true;
}

// 浮点数接口函数
bool set_test_double(ClientConnection* client, double value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "SetTestDouble");
    if (!msg) return false;
    
    if (!dbus_message_append_args(msg, DBUS_TYPE_DOUBLE, &value, DBUS_TYPE_INVALID)) {
        dbus_message_unref(msg);
        return false;
    }
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_bool_t result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_BOOLEAN, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    printf("SetTestDouble(%f) 调用成功\n", value);
    return (bool)result;
}

bool get_test_double(ClientConnection* client, double* value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "GetTestDouble");
    if (!msg) return false;
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    double result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_DOUBLE, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    *value = result;
    printf("GetTestDouble() 返回: %f\n", *value);
    return true;
}

// 字符串接口函数
bool set_test_string(ClientConnection* client, const char* value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "SetTestString");
    if (!msg) return false;
    
    if (!dbus_message_append_args(msg, DBUS_TYPE_STRING, &value, DBUS_TYPE_INVALID)) {
        dbus_message_unref(msg);
        return false;
    }
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_bool_t result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_BOOLEAN, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    printf("SetTestString(\"%s\") 调用成功\n", value);
    return (bool)result;
}

bool get_test_string(ClientConnection* client, char** value) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "GetTestString");
    if (!msg) return false;
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    const char* result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_STRING, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    *value = strdup(result);
    dbus_message_unref(reply);
    printf("GetTestString() 返回: \"%s\"\n", *value);
    return true;
}

// TestInfo结构体接口函数
bool set_test_info(ClientConnection* client, const TestInfo* info) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "SetTestInfo");
    if (!msg) return false;
    
    dbus_bool_t bool_param = (dbus_bool_t)info->bool_param;
    dbus_int32_t int_param = (dbus_int32_t)info->int_param;
    const char* string_param = info->string_param ? info->string_param : "";
    
    if (!dbus_message_append_args(msg,
                                 DBUS_TYPE_BOOLEAN, &bool_param,
                                 DBUS_TYPE_INT32, &int_param,
                                 DBUS_TYPE_DOUBLE, &info->double_param,
                                 DBUS_TYPE_STRING, &string_param,
                                 DBUS_TYPE_INVALID)) {
        dbus_message_unref(msg);
        return false;
    }
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_bool_t result;
    if (!dbus_message_get_args(reply, &client->error, DBUS_TYPE_BOOLEAN, &result, DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    dbus_message_unref(reply);
    printf("SetTestInfo({bool: %s, int: %d, double: %f, string: \"%s\"}) 调用成功\n",
           info->bool_param ? "true" : "false",
           info->int_param,
           info->double_param,
           string_param);
    return (bool)result;
}

bool get_test_info(ClientConnection* client, TestInfo* info) {
    DBusMessage* msg = dbus_message_new_method_call(SERVICE_NAME,
                                                   SERVICE_PATH,
                                                   SERVICE_INTERFACE,
                                                   "GetTestInfo");
    if (!msg) return false;
    
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(client->connection, msg, 5000, &client->error);
    dbus_message_unref(msg);
    
    if (dbus_error_is_set(&client->error)) {
        printf("发送消息失败: %s\n", client->error.message);
        dbus_error_free(&client->error);
        return false;
    }
    
    dbus_bool_t bool_param;
    dbus_int32_t int_param;
    double double_param;
    const char* string_param;
    
    if (!dbus_message_get_args(reply, &client->error,
                              DBUS_TYPE_BOOLEAN, &bool_param,
                              DBUS_TYPE_INT32, &int_param,
                              DBUS_TYPE_DOUBLE, &double_param,
                              DBUS_TYPE_STRING, &string_param,
                              DBUS_TYPE_INVALID)) {
        dbus_error_free(&client->error);
        dbus_message_unref(reply);
        return false;
    }
    
    info->bool_param = (bool)bool_param;
    info->int_param = (int)int_param;
    info->double_param = double_param;
    info->string_param = strdup(string_param);
    
    dbus_message_unref(reply);
    printf("GetTestInfo() 返回: {bool: %s, int: %d, double: %f, string: \"%s\"}\n",
           info->bool_param ? "true" : "false",
           info->int_param,
           info->double_param,
           info->string_param);
    return true;
}

void show_menu(void) {
    printf("\n=== Test Client 菜单 ===\n");
    printf("1. 设置布尔值 (SetTestBool)\n");
    printf("2. 获取布尔值 (GetTestBool)\n");
    printf("3. 设置整数值 (SetTestInt)\n");
    printf("4. 获取整数值 (GetTestInt)\n");
    printf("5. 设置浮点值 (SetTestDouble)\n");
    printf("6. 获取浮点值 (GetTestDouble)\n");
    printf("7. 设置字符串 (SetTestString)\n");
    printf("8. 获取字符串 (GetTestString)\n");
    printf("9. 设置结构体 (SetTestInfo)\n");
    printf("10. 获取结构体 (GetTestInfo)\n");
    printf("0. 退出\n");
    printf("请选择操作: ");
}

void handle_user_input(ClientConnection* client) {
    int choice;
    bool bool_val;
    int int_val;
    double double_val;
    char string_buffer[256];
    char* string_result;
    TestInfo info = {0};
    
    while (1) {
        show_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("输入无效，请重新输入\n");
            while (getchar() != '\n'); // 清除输入缓冲区
            continue;
        }
        
        switch (choice) {
            case 1:
                printf("请输入布尔值 (0=false, 1=true): ");
                if (scanf("%d", &int_val) == 1) {
                    bool_val = (int_val != 0);
                    set_test_bool(client, bool_val);
                } else {
                    printf("输入无效\n");
                }
                break;
                
            case 2:
                if (get_test_bool(client, &bool_val)) {
                    printf("当前布尔值: %s\n", bool_val ? "true" : "false");
                }
                break;
                
            case 3:
                printf("请输入整数值: ");
                if (scanf("%d", &int_val) == 1) {
                    set_test_int(client, int_val);
                } else {
                    printf("输入无效\n");
                }
                break;
                
            case 4:
                if (get_test_int(client, &int_val)) {
                    printf("当前整数值: %d\n", int_val);
                }
                break;
                
            case 5:
                printf("请输入浮点值: ");
                if (scanf("%lf", &double_val) == 1) {
                    set_test_double(client, double_val);
                } else {
                    printf("输入无效\n");
                }
                break;
                
            case 6:
                if (get_test_double(client, &double_val)) {
                    printf("当前浮点值: %f\n", double_val);
                }
                break;
                
            case 7:
                printf("请输入字符串: ");
                // 清除输入缓冲区中的换行符
                while (getchar() != '\n');
                if (fgets(string_buffer, sizeof(string_buffer), stdin)) {
                    // 移除fgets读取的换行符
                    string_buffer[strcspn(string_buffer, "\n")] = '\0';
                    set_test_string(client, string_buffer);
                } else {
                    printf("输入无效\n");
                }
                break;
                
            case 8:
                if (get_test_string(client, &string_result)) {
                    printf("当前字符串: \"%s\"\n", string_result);
                    free(string_result);
                }
                break;
                
            case 9:
                printf("请输入TestInfo结构体:\n");
                printf("布尔值 (0/1): ");
                if (scanf("%d", &int_val) != 1) { printf("输入无效\n"); break; }
                info.bool_param = (int_val != 0);
                
                printf("整数值: ");
                if (scanf("%d", &info.int_param) != 1) { printf("输入无效\n"); break; }
                
                printf("浮点值: ");
                if (scanf("%lf", &info.double_param) != 1) { printf("输入无效\n"); break; }
                
                printf("字符串: ");
                // 清除输入缓冲区中的换行符  
                while (getchar() != '\n');
                if (fgets(string_buffer, sizeof(string_buffer), stdin)) {
                    // 移除fgets读取的换行符
                    string_buffer[strcspn(string_buffer, "\n")] = '\0';
                    info.string_param = strdup(string_buffer);
                } else {
                    printf("输入无效\n");
                    break;
                }
                
                set_test_info(client, &info);
                cleanup_test_info(&info);
                break;
                
            case 10:
                if (get_test_info(client, &info)) {
                    printf("当前TestInfo: {bool: %s, int: %d, double: %f, string: \"%s\"}\n",
                           info.bool_param ? "true" : "false",
                           info.int_param,
                           info.double_param,
                           info.string_param ? info.string_param : "");
                    cleanup_test_info(&info);
                }
                break;
                
            case 0:
                printf("退出程序\n");
                return;
                
            default:
                printf("无效选择，请重新选择\n");
                break;
        }
    }
}

int main() {
    printf("启动Test Client...\n");
    
    // 初始化客户端连接
    ClientConnection* client = init_client_connection();
    if (!client) {
        printf("初始化客户端失败\n");
        return -1;
    }
    
    // 处理用户输入
    handle_user_input(client);
    
    // 清理资源
    cleanup_client_connection(client);
    
    return 0;
}