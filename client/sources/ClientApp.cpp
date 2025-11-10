#include "ClientApp.h"
#include <iostream>

ClientApp::ClientApp(ITestService* svc) : service(svc) {}

void ClientApp::RunMenu() {
    while (true) {
        std::cout << "\n===== Client Menu =====\n";
        std::cout << "1. Set Int\n";
        std::cout << "2. Get Int\n";
        std::cout << "3. Exit\n";

        int op;
        std::cin >> op;

        if (op == 1) {
            int v;
            std::cout << "Enter int value: ";
            std::cin >> v;
            service->SetTestInt(v);
        }
        else if (op == 2) {
            std::cout << "GetInt = " << service->GetTestInt() << "\n";
        }
        else {
            break;
        }
    }
}

void ClientApp::OnTestIntChanged(int v) {
    std::cout << "[broadcast] Int changed: " << v << "\n";
}

void ClientApp::OnTestBoolChanged(bool param) {}
void ClientApp::OnTestDoubleChanged(double param) {}
void ClientApp::OnTestStringChanged(std::string param) {}
void ClientApp::OnTestInfoChanged(TestInfo param) {}