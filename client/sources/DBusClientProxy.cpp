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

bool DBusClientProxy::SetTestDouble(double v) {
    std::cout << "[stub] SetTestDouble called\n";
    return true;
}

bool DBusClientProxy::SetTestString(std::string param) {
    std::cout << "[stub] SetTestString called\n";
    return true;
}

bool DBusClientProxy::SetTestInfo(TestInfo param) {
    std::cout << "[stub] SetTestInfo called\n";
    return true;
}

bool DBusClientProxy::GetTestBool() {
    return true;
}

int DBusClientProxy::GetTestInt() {
    return 100;
}

double DBusClientProxy::GetTestDouble() {
    return 3.14;
}

std::string DBusClientProxy::GetTestString() {
    return "hello";
}

TestInfo DBusClientProxy::GetTestInfo() {
    return { true, 10, 3.33, "client" };
}

bool DBusClientProxy::SendFile(unsigned char* file_buf, size_t file_size) {
    std::cout << "[stub] SendFile called\n";
    return true;
}
