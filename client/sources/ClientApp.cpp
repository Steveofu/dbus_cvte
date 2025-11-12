#include "ClientApp.h"
#include <iostream>
#include <fstream>  


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
        std::cout << "11. Send File\n";
        std::cout << "12. Exit\n";

        int op = 0;
        std::cout << "Enter option: ";

        //  输入合法性检测
        if (!(std::cin >> op)) {
            std::cin.clear(); // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // 清空缓冲区
            std::cout << "[error] Invalid input. Please enter a number.\n";
            continue;
        }

        //  检查范围
        if (op < 1 || op > 12) {
            std::cout << "[error] Option out of range (1–12). Try again.\n";
            continue;
        }

        //  根据选项执行
        switch (op) {
            case 1: {
                int v;
                std::cout << "Enter int value: ";
                if (!(std::cin >> v)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[error] Invalid integer.\n";
                    continue;
                }
                service->SetTestInt(v);
                break;
            }

            case 2:
                std::cout << "Get Int = " << service->GetTestInt() << "\n";
                break;

            case 3: {
                int val;
                std::cout << "Enter bool (1=true, 0=false): ";
                if (!(std::cin >> val) || (val != 0 && val != 1)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[error] Enter 0 or 1 only.\n";
                    continue;
                }
                service->SetTestBool(val);
                break;
            }

            case 4:
                std::cout << "Get Bool = " << (service->GetTestBool() ? "true" : "false") << "\n";
                break;

            case 5: {
                double v;
                std::cout << "Enter double value: ";
                if (!(std::cin >> v)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[error] Invalid double.\n";
                    continue;
                }
                service->SetTestDouble(v);
                break;
            }

            case 6:
                std::cout << "Get Double = " << service->GetTestDouble() << "\n";
                break;

            case 7: {
                std::string v;
                std::cout << "Enter string value: ";
                std::cin.ignore();
                std::getline(std::cin, v);
                if (v.empty()) {
                    std::cout << "[warning] Empty string ignored.\n";
                    continue;
                }
                service->SetTestString(v);
                break;
            }

            case 8:
                std::cout << "Get String = " << service->GetTestString() << "\n";
                break;

            case 9: {
                TestInfo info;
                std::cout << "Enter bool value (1/0): ";
                if (!(std::cin >> info.bool_param) || (info.bool_param != 0 && info.bool_param != 1)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[error] Invalid bool input.\n";
                    continue;
                }

                std::cout << "Enter int value: ";
                if (!(std::cin >> info.int_param)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[error] Invalid int input.\n";
                    continue;
                }

                std::cout << "Enter double value: ";
                if (!(std::cin >> info.double_param)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "[error] Invalid double input.\n";
                    continue;
                }

                std::cout << "Enter string value: ";
                std::cin.ignore();
                std::getline(std::cin, info.string_param);

                service->SetTestInfo(info);
                break;
            }

            case 10: {
                TestInfo info = service->GetTestInfo();
                std::cout << "Get Info = "
                          << "Bool: " << info.bool_param << ", "
                          << "Int: " << info.int_param << ", "
                          << "Double: " << info.double_param << ", "
                          << "String: " << info.string_param << "\n";
                break;
            }

            case 11: {
                std::string file_path;
                std::cout << "Enter file path: ";
                std::cin.ignore();
                std::getline(std::cin, file_path);
                if (file_path.empty()) {
                    std::cout << "[error] File path cannot be empty.\n";
                    continue;
                }
                if (service->SendFile(file_path))
                    std::cout << "[client] File sent successfully.\n";
                else
                    std::cerr << "[client] Failed to send file.\n";
                break;
            }

            case 12:
                std::cout << "Exiting...\n";
                return;
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