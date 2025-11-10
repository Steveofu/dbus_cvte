#pragma once  // 建议添加，避免头文件重复包含
#include <string>
#include "TestInfo.h"

class ITestService{
public:
    virtual ~ITestService(){}
    //Set 接口
    virtual bool SetTestBool(bool param) = 0;
    virtual bool SetTestInt(int param) = 0;
    virtual bool SetTestDouble(double param) = 0;
    virtual bool SetTestString(std::string param) = 0;
    virtual bool SetTestInfo(TestInfo param) = 0;
    
    //Get 接口
    virtual bool GetTestBool() = 0;
    virtual int GetTestInt() = 0;
    virtual double GetTestDouble() = 0;
    virtual std::string GetTestString() = 0;
    virtual TestInfo GetTestInfo() = 0;
    
    virtual bool SendFile(unsigned char* file_buf, size_t file_size) = 0;
};
