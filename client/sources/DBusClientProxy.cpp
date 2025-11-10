#include "DBusClientProxy.h"
#include <iostream>

DBusClientProxy::DBusClientProxy() {
    DBusError err;
    dbus_error_init(&err);
    
    // like create socket
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    
    if (dbus_error_is_set(&err)) {
        std::cout << "DBus error: " << err.message << std::endl;
        dbus_error_free(&err);
    }
}

DBusClientProxy::~DBusClientProxy() { }

bool DBusClientProxy::callBool(const char* method, bool value) 
{
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        method
    );

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &value);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    return true;
}

bool DBusClientProxy::SetTestBool(bool param) {
    return callBool("SetTestBool", param);
}


bool DBusClientProxy::SetTestInt(int value) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestInt"
    );

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &value);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    return true;
}


bool DBusClientProxy::SetTestDouble(double value) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestDouble"
    );

    if (!msg) {
        std::cerr << "dbus_message_new_method_call failed" << std::endl;
        return false;
    }

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);

    // DBus 不存在 double 类型，但用 DBUS_TYPE_DOUBLE 就行
    dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &value);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);

    dbus_message_unref(msg);

    return true;
}




bool DBusClientProxy::SetTestString(std::string value) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestString"
    );

    if (!msg) return false;

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);

    const char* cstr = value.c_str();
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &cstr);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);
    return true;
}


bool DBusClientProxy::SetTestInfo(TestInfo info) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestInfo"
    );

    if (!msg) return false;

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);

    // bool（必须转换成 dbus_bool_t）
    dbus_bool_t flag = info.bool_param;
    dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &flag);

    // int
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &info.int_param);

    // double
    dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &info.double_param);

    // string（必须 char*）
    const char* cstr = info.string_param.c_str();
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &cstr);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    return true;
}




bool DBusClientProxy::GetTestBool() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestBool"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestBool): " << err.message << std::endl;
        dbus_error_free(&err);
        return false;
    }

    dbus_bool_t val = false;
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_BOOLEAN, &val,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return val;
}


int DBusClientProxy::GetTestInt() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestInt"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );

    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error: " << err.message << std::endl;
        dbus_error_free(&err);
        return -1;
    }

    int value = 0;
    dbus_message_get_args(reply, &err, DBUS_TYPE_INT32, &value, DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return value;
}


double DBusClientProxy::GetTestDouble() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestDouble"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestDouble): " << err.message << std::endl;
        dbus_error_free(&err);
        return 0.0;
    }

    double value = 0.0;
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_DOUBLE, &value,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return value;
}


std::string DBusClientProxy::GetTestString() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestString"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestString): " << err.message << std::endl;
        dbus_error_free(&err);
        return "";
    }

    const char* str = nullptr;
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_STRING, &str,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return str ? std::string(str) : "";
}


TestInfo DBusClientProxy::GetTestInfo() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestInfo"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    TestInfo info;

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestInfo): " << err.message << std::endl;
        dbus_error_free(&err);
        return info;  // 返回空结构体
    }

    // 临时变量接收 DBus 参数
    dbus_bool_t flag;
    int number;
    double ratio;
    const char* msg_str = nullptr;

    // 从 reply 中解析参数
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_BOOLEAN, &flag,
                          DBUS_TYPE_INT32,   &number,
                          DBUS_TYPE_DOUBLE,  &ratio,
                          DBUS_TYPE_STRING,  &msg_str,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);

    // ✅ 按 TestInfo.h 的字段名赋值
    info.bool_param = flag;
    info.int_param = number;
    info.double_param = ratio;
    info.string_param = msg_str ? msg_str : "";

    return info;
}

bool DBusClientProxy::SendFile(unsigned char* file_buf, size_t file_size) {
    std::cout << "[stub] SendFile called\n";
    return true;
}
