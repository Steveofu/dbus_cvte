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
    
    bool ReceiveFileChunk(int seq, int len);

    std::string compute_md5(const std::string& path);
    void StartLoop();

private:
    DBusConnection* conn;

    int lastInt = 0;
    double lastDouble = 0.0;
    bool lastBool = 0;
    std::string lastString="fault";
    TestInfo lastTestInfo;
    

    void broadcastInt(int v);
    void broadcastBool(bool v);
    void broadcastDouble(double v);
    void broadcastString(std::string param);
    void broadcastTestInfo(TestInfo info);
};

