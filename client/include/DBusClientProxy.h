#pragma once

#include "ITestService.h"
#include <dbus/dbus.h>
#include <string>
#include <cstring>
#include <unistd.h>  

// 定义dbus协议 继承接口
class DBusClientProxy : public ITestService 
{
public:
    DBusClientProxy();
    ~DBusClientProxy();

    bool SetTestBool(bool param) override;
    bool SetTestInt(int param) override;
    bool SetTestDouble(double param) override;
    bool SetTestString(std::string param) override; 
    bool SetTestInfo(TestInfo param) override;  

    bool GetTestBool() override;
    int  GetTestInt() override;
    double GetTestDouble() override;
    std::string GetTestString() override;  
    TestInfo GetTestInfo() override;

    bool SendFile(const std::string& file_path); 
    void NotifyServiceFileChunkReceived(int seq, int len);


private:
    // 保持dbus连接的句柄
    DBusConnection* conn;
    bool callBool(const char* method, bool value);
    int  callInt(const char* method);
};
