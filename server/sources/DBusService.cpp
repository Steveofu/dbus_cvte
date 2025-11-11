#include "DBusService.h"
#include <iostream>

DBusService::DBusService() {
    DBusError err;
    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    dbus_bus_request_name(conn, "com.demo.Service",
                          DBUS_NAME_FLAG_REPLACE_EXISTING,
                          &err);

    std::cout << "[server] Service started\n";
}

DBusService::~DBusService() {}

bool DBusService::SetTestInt(int param) {
    std::cout << "[server] SetTestInt(" << param << ")\n";
    lastInt = param;
    broadcastInt(param);
    return true;
}

void DBusService::broadcastInt(int v) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",
        "com.demo.Interface",
        "OnTestIntChanged"
    );

    dbus_message_append_args(msg,
                             DBUS_TYPE_INT32, &v,
                             DBUS_TYPE_INVALID);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);
}


void DBusService::broadcastBool(bool v) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",       // 信号路径
        "com.demo.Interface",     // 信号接口
        "OnTestBoolChanged"       // 信号名称
    );

    dbus_message_append_args(msg,
                             DBUS_TYPE_BOOLEAN, &v,   // 传递 bool 类型参数
                             DBUS_TYPE_INVALID);      // 结束参数列表

    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for bool!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}

bool DBusService::SetTestBool(bool param) {
    std::cout << "[server] SetTestBool(" << param << ")\n";
    lastBool = param;  // 假设 lastBool 是服务端存储 bool 值的成员变量
    broadcastBool(param);  // 广播新值
    return true;
}



bool DBusService::SetTestDouble(double param) {
    std::cout << "[server] SetTestDouble(" << param << ")\n";
    lastDouble = param;  // 假设 lastDouble 是服务端存储 double 值的成员变量
    broadcastDouble(param);  // 广播新值
    return true;
}

void DBusService::broadcastDouble(double v) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",       // 信号路径
        "com.demo.Interface",     // 信号接口
        "OnTestDoubleChanged"     // 信号名称
    );

    dbus_message_append_args(msg,
                             DBUS_TYPE_DOUBLE, &v,   // 传递 double 类型参数
                             DBUS_TYPE_INVALID);     // 结束参数列表

    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for double!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}

bool DBusService::SetTestString(std::string param) {
    if (param.empty()) {
        std::cerr << "[server] Empty string passed to SetTestString!" << std::endl;
        return false;
    }
    std::cout << "[server] SetTestString(" << param << ")\n";
    lastString = param;
    broadcastString(param);
    return true;
}


void DBusService::broadcastString(std::string v) {
    if (v.empty()) {
        std::cerr << "[server] Attempting to broadcast an empty string!" << std::endl;
        return;
    }

    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",       // 信号路径
        "com.demo.Interface",     // 信号接口
        "OnTestStringChanged"     // 信号名称
    );

    const char* str_data = v.c_str();
    if (!dbus_message_append_args(msg, DBUS_TYPE_STRING, &str_data, DBUS_TYPE_INVALID)) {
        std::cerr << "[server] Failed to append string argument to the signal!" << std::endl;
        dbus_message_unref(msg);
        return;
    }

    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for string!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}



bool DBusService::SetTestInfo(TestInfo info) {
    std::cout << "[server] SetTestInfo(" 
              << "bool_param: " << info.bool_param 
              << ", int_param: " << info.int_param
              << ", double_param: " << info.double_param 
              << ", string_param: " << info.string_param 
              << ")\n";
    
    // 存储结构体信息
    lastTestInfo = info;

    // 广播信息变化
    broadcastTestInfo(info);

    return true;
}

void DBusService::broadcastTestInfo(TestInfo info) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",         // 信号的路径
        "com.demo.Interface",       // 信号的接口
        "OnTestInfoChanged"         // 信号的名称
    );

    // 确保传递的 bool 值为合法的值 (0 或 1)
    dbus_bool_t bool_value = (info.bool_param) ? TRUE : FALSE;  // 确保是 0 或 1

    // 将结构体的各个字段作为参数附加到信号中
    dbus_message_append_args(msg,
                             DBUS_TYPE_BOOLEAN, &bool_value,
                             DBUS_TYPE_INT32, &info.int_param,
                             DBUS_TYPE_DOUBLE, &info.double_param,
                             DBUS_TYPE_STRING, info.string_param.empty() ? "" : info.string_param.c_str(),  // 处理空字符串
                             DBUS_TYPE_INVALID);  // 结束参数列表

    // 发送信号
    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for TestInfo!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}


bool DBusService::GetTestBool() { return lastBool; }
int DBusService::GetTestInt() { return lastInt; }
double DBusService::GetTestDouble() { return lastDouble; }

std::string DBusService::GetTestString() {
    return lastString.empty() ? "default_string" : lastString;
}

TestInfo DBusService::GetTestInfo() { return {true,1,2.0,"x"}; }

bool DBusService::SendFile(unsigned char*, size_t) { return true; }


void DBusService::StartLoop() {
    while (true) {
        dbus_connection_read_write(conn, 0);
        DBusMessage* msg = dbus_connection_pop_message(conn);
        if (!msg) continue;

        // 打印接收到的消息类型和方法名称
        std::cout << "[server] Received message: " << dbus_message_get_member(msg) << std::endl;

        // 匹配方法调用 SetTestInt
        if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestInt")) {
            int val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_INT32, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestInt: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestInt(val);
        }
        
        // 匹配方法调用 GetTestInt
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestInt")) {
            int value = GetTestInt();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &value);

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestInt!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        // 匹配方法调用 SetTestBool
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestBool")) {
            bool val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_BOOLEAN, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestBool: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestBool(val);
        }

        // 匹配方法调用 GetTestBool
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestBool")) {
            bool value = GetTestBool();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &value);

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestBool!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        // 匹配方法调用 SetTestDouble
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestDouble")) {
            double val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_DOUBLE, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestDouble: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestDouble(val);
        }

        // 匹配方法调用 GetTestDouble
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestDouble")) {
            double value = GetTestDouble();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &value);

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestDouble!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        // 匹配方法调用 SetTestString
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestString")) {
            const char* val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_STRING, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestString: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestString(val);
        }

        // 匹配方法调用 GetTestString
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestString")) {
            // 获取当前的字符串值
            std::string value = GetTestString();
            const char* str_data = value.c_str(); // 获取字符串指针

            // 创建返回消息
            DBusMessage* reply = dbus_message_new_method_return(msg);
            if (!reply) {
                std::cerr << "[server] Failed to create reply for GetTestString!" << std::endl;
                dbus_message_unref(msg);
                return;
            }

            // 初始化消息迭代器（不需要检查返回值）
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);  // 不需要检查返回值

            // 直接添加字符串参数
            dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &str_data);

            // 发送消息
            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestString!" << std::endl;
            }
            dbus_connection_flush(conn);

            // 清理资源
            dbus_message_unref(reply);
        }



        // 匹配方法调用 SetTestInfo
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestInfo")) {
            TestInfo val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_BOOLEAN, &val.bool_param,
                                  DBUS_TYPE_INT32, &val.int_param,
                                  DBUS_TYPE_DOUBLE, &val.double_param,
                                  DBUS_TYPE_STRING, &val.string_param,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestInfo: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestInfo(val);
        }

        // 匹配方法调用 GetTestInfo
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestInfo")) {
            TestInfo value = GetTestInfo();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &value.bool_param);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &value.int_param);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &value.double_param);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, value.string_param.c_str());

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestInfo!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        dbus_message_unref(msg);
    }
}
