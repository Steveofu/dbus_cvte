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

bool DBusService::SetTestBool(bool) { return true; }
bool DBusService::SetTestDouble(double) { return true; }
bool DBusService::SetTestString( std::string) { return true; }
bool DBusService::SetTestInfo( TestInfo) { return true; }

bool DBusService::GetTestBool() { return true; }
int DBusService::GetTestInt() { return lastInt; }
double DBusService::GetTestDouble() { return 0.0; }
std::string DBusService::GetTestString() { return "default"; }
TestInfo DBusService::GetTestInfo() { return {true,1,2.0,"x"}; }

bool DBusService::SendFile(unsigned char*, size_t) { return true; }

void DBusService::StartLoop() {
    while (true) {
        dbus_connection_read_write(conn, 0);
        DBusMessage* msg = dbus_connection_pop_message(conn);
        if (!msg) continue;

        // 匹配方法调用
        if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestInt")) {
            int val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_INT32, &val,
                                  DBUS_TYPE_INVALID);

            SetTestInt(val);
        }

        dbus_message_unref(msg);
    }
}

