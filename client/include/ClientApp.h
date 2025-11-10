#pragma once

#include "ITestService.h"
#include "ITestListener.h"

class ClientApp : public ITestListener {
public:
    ClientApp(ITestService* svc);

    void RunMenu();

    void OnTestBoolChanged(bool param) override;
    void OnTestIntChanged(int param) override;
    void OnTestDoubleChanged(double param) override;
    void OnTestStringChanged(std::string param) override;
    void OnTestInfoChanged(TestInfo param) override;

private:
    ITestService* service;
};