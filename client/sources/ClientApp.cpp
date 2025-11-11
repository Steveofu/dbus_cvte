#include "ClientApp.h"
#include <iostream>

ClientApp::ClientApp(ITestService* svc) : service(svc) {}

void ClientApp::RunMenu() {
    while (true) {
        std::cout << "\n===== Client Menu =====\n";
        std::cout << "1. Set Int\n";
        std::cout << "2. Get Int\n";
        std::cout << "3. Set Bool\n";
        std::cout << "4. Get Bool\n";
        std::cout << "5. Set Double\n";
        std::cout << "6. Get Double\n";
        std::cout << "7. Set String\n";
        std::cout << "8. Get String\n";
        std::cout << "9. Set Info\n";
        std::cout << "10. Get Info\n";
        std::cout << "11. Exit\n";

        int op;
        std::cin >> op;

        if (op == 1) {  // Set Int
            int v;
            std::cout << "Enter int value: ";
            std::cin >> v;
            service->SetTestInt(v);
        }
        else if (op == 2) {  // Get Int
            std::cout << "Get Int = " << service->GetTestInt() << "\n";
        }
        else if (op == 3) {  // Set Bool
            bool v;
            std::cout << "Enter bool value (1 for true, 0 for false): ";
            std::cin >> v;
            service->SetTestBool(v);
        }
        else if (op == 4) {  // Get Bool
            std::cout << "Get Bool = " << (service->GetTestBool() ? "true" : "false") << "\n";
        }
        else if (op == 5) {  // Set Double
            double v;
            std::cout << "Enter double value: ";
            std::cin >> v;
            service->SetTestDouble(v);
        }
        else if (op == 6) {  // Get Double
            std::cout << "Get Double = " << service->GetTestDouble() << "\n";
        }
        else if (op == 7) {  // Set String
            std::string v;
            std::cout << "Enter string value: ";
            std::cin.ignore();  // Ignore any leftover newline character
            std::getline(std::cin, v);
            service->SetTestString(v);
        }
        else if (op == 8) {  // Get String
            std::cout << "Get String = " << service->GetTestString() << "\n";
        }
        else if (op == 9) {  // Set Info
            TestInfo info;
            std::cout << "Enter bool value for info: ";
            std::cin >> info.bool_param;
            std::cout << "Enter int value for info: ";
            std::cin >> info.int_param;
            std::cout << "Enter double value for info: ";
            std::cin >> info.double_param;
            std::cout << "Enter string value for info: ";
            std::cin.ignore();  // Ignore any leftover newline character
            std::getline(std::cin, info.string_param);
            service->SetTestInfo(info);
        }
        else if (op == 10) {  // Get Info
            TestInfo info = service->GetTestInfo();
            std::cout << "Get Info = " 
                      << "Bool: " << info.bool_param << ", "
                      << "Int: " << info.int_param << ", "
                      << "Double: " << info.double_param << ", "
                      << "String: " << info.string_param << "\n";
        }
        else if (op == 11) {  // Exit
            break;
        }
        else {
            std::cout << "Invalid option. Try again.\n";
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