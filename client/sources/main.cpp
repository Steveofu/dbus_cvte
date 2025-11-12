#include "ClientApp.h"
#include "DBusClientProxy.h"

int main() 
{
    // 创建DBus客户端代理
    DBusClientProxy service;
    ClientApp app(&service);

    app.RunMenu();
    return 0;
}