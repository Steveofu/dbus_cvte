#pragma once  
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
    
    
    // ✅ 新增路径版本（客户端使用）
    virtual bool SendFile(const std::string& file_path) { return false; };

};
