#include "ClientApp.h"
#include "DBusClientProxy.h"

int main() {
    DBusClientProxy service;
    ClientApp app(&service);

    app.RunMenu();
    return 0;
}