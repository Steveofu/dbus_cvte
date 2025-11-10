#pragma once
#include <dbus/dbus.h>
#include <string>
#include "ITestService.h"

class DBusService : public ITestService {
public:
    DBusService();
    ~DBusService();

    bool SetTestBool(bool param) override;
    bool SetTestInt(int param) override;
    bool SetTestDouble(double param) override;
    bool SetTestString( std::string param) override;
    bool SetTestInfo( TestInfo param) override;

    bool GetTestBool() override;
    int  GetTestInt() override;
    double GetTestDouble() override;
    std::string GetTestString() override;
    TestInfo GetTestInfo() override;

    bool SendFile(unsigned char* file_buf, size_t file_size) override;

    void StartLoop();

private:
    DBusConnection* conn;

    int lastInt = 0;

    void broadcastInt(int v);
};

