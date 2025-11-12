#include "DBusService.h"
#include <iostream>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstring>
// 已有的头文件下方添加
#include <dbus/dbus.h>
#include <map>
#include <vector>
#include <openssl/md5.h>
#include <iomanip>
#include <sstream>


#define SHM_NAME "/file_shm"  // 共享内存的名称
#define SHM_SIZE 1024         // 每次传输1KB数据


// 包结构体定义 —— 与客户端保持一致
struct FileChunk {
    int seq;             // 当前包序号
    int len;             // 当前包长度（单位：字节）
    int total_chunks;    // 总包数
    char filename[64];   // 文件名
    unsigned char data[SHM_SIZE - 80]; // 实际数据
};


DBusService::DBusService() {
    DBusError err;
    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        std::cerr << "[server] Failed to connect to D-Bus session: " << err.message << std::endl;
        dbus_error_free(&err);
    } else {
        dbus_bus_request_name(conn, "com.demo.Service", DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
        std::cout << "[server] Service started\n"<<std::endl;

        // 注册 signal match，让 bus 将来自客户端的 OnFileChunkReceived 信号转发到本连接
        // 否则即使客户端发送了信号，bus 也不会投递到我们的连接上
        const char* match_rule = "type='signal',path='/com/demo/Object',interface='com.demo.Interface',member='OnFileChunkReceived'";
        dbus_bus_add_match(conn, match_rule, &err);

        if (dbus_error_is_set(&err)) {
            std::cerr << "[server] dbus_bus_add_match failed: " << err.message << std::endl;
            dbus_error_free(&err);
        } else {
            std::cout << "[server] Added match rule for OnFileChunkReceived\n" << std::endl;
        }
    }
}


DBusService::~DBusService() {}

bool DBusService::SetTestInt(int param) {
    std::cout << "[server] SetTestInt(" << param << ")\n";
    lastInt = param;
    broadcastInt(param);
    return true;
}

void DBusService::broadcastInt(int v) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",
        "com.demo.Interface",
        "OnTestIntChanged"
    );

    dbus_message_append_args(msg,
                             DBUS_TYPE_INT32, &v,
                             DBUS_TYPE_INVALID);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);
}


void DBusService::broadcastBool(bool v) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",       // 信号路径
        "com.demo.Interface",     // 信号接口
        "OnTestBoolChanged"       // 信号名称
    );

    dbus_message_append_args(msg,
                             DBUS_TYPE_BOOLEAN, &v,   // 传递 bool 类型参数
                             DBUS_TYPE_INVALID);      // 结束参数列表

    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for bool!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}

bool DBusService::SetTestBool(bool param) {
    std::cout << "[server] SetTestBool(" << param << ")\n";
    lastBool = param;  // 假设 lastBool 是服务端存储 bool 值的成员变量
    broadcastBool(param);  // 广播新值
    return true;
}



bool DBusService::SetTestDouble(double param) {
    std::cout << "[server] SetTestDouble(" << param << ")\n";
    lastDouble = param;  // 假设 lastDouble 是服务端存储 double 值的成员变量
    broadcastDouble(param);  // 广播新值
    return true;
}

void DBusService::broadcastDouble(double v) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",       // 信号路径
        "com.demo.Interface",     // 信号接口
        "OnTestDoubleChanged"     // 信号名称
    );

    dbus_message_append_args(msg,
                             DBUS_TYPE_DOUBLE, &v,   // 传递 double 类型参数
                             DBUS_TYPE_INVALID);     // 结束参数列表

    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for double!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}

bool DBusService::SetTestString(std::string param) {
    if (param.empty()) {
        std::cerr << "[server] Empty string passed to SetTestString!" << std::endl;
        return false;
    }
    std::cout << "[server] SetTestString(" << param << ")\n";
    lastString = param;
    broadcastString(param);
    return true;
}


void DBusService::broadcastString(std::string v) {
    if (v.empty()) {
        std::cerr << "[server] Attempting to broadcast an empty string!" << std::endl;
        return;
    }

    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",       // 信号路径
        "com.demo.Interface",     // 信号接口
        "OnTestStringChanged"     // 信号名称
    );

    const char* str_data = v.c_str();
    if (!dbus_message_append_args(msg, DBUS_TYPE_STRING, &str_data, DBUS_TYPE_INVALID)) {
        std::cerr << "[server] Failed to append string argument to the signal!" << std::endl;
        dbus_message_unref(msg);
        return;
    }

    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for string!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}



bool DBusService::SetTestInfo(TestInfo info) {
    std::cout << "[server] SetTestInfo(" 
              << "bool_param: " << info.bool_param 
              << ", int_param: " << info.int_param
              << ", double_param: " << info.double_param 
              << ", string_param: " << info.string_param 
              << ")\n";
    
    // 存储结构体信息
    lastTestInfo = info;

    // 广播信息变化
    broadcastTestInfo(info);

    return true;
}

void DBusService::broadcastTestInfo(TestInfo info) {
    DBusMessage* msg = dbus_message_new_signal(
        "/com/demo/Object",         // 信号的路径
        "com.demo.Interface",       // 信号的接口
        "OnTestInfoChanged"         // 信号的名称
    );

    // 确保传递的 bool 值为合法的值 (0 或 1)
    dbus_bool_t bool_value = (info.bool_param) ? TRUE : FALSE;  // 确保是 0 或 1

    // 将结构体的各个字段作为参数附加到信号中
    dbus_message_append_args(msg,
                             DBUS_TYPE_BOOLEAN, &bool_value,
                             DBUS_TYPE_INT32, &info.int_param,
                             DBUS_TYPE_DOUBLE, &info.double_param,
                             DBUS_TYPE_STRING, info.string_param.empty() ? "" : info.string_param.c_str(),  // 处理空字符串
                             DBUS_TYPE_INVALID);  // 结束参数列表

    // 发送信号
    if (!dbus_connection_send(conn, msg, nullptr)) {
        std::cerr << "[server] Failed to send broadcast signal for TestInfo!" << std::endl;
    }
    dbus_connection_flush(conn);
    dbus_message_unref(msg);  // 清理消息
}


bool DBusService::GetTestBool() { return lastBool; }
int DBusService::GetTestInt() { return lastInt; }
double DBusService::GetTestDouble() { return lastDouble; }

std::string DBusService::GetTestString() {
    return lastString.empty() ? "default_string" : lastString;
}

TestInfo DBusService::GetTestInfo() { return {true,1,2.0,"x"}; }

bool DBusService::SendFile(unsigned char*, size_t) { return true; }


// 计算文件的 MD5 值
std::string DBusService::compute_md5(const std::string& path) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX ctx;
    MD5_Init(&ctx);

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return "error";

    char buffer[1024];
    while (file.good()) {
        file.read(buffer, sizeof(buffer));
        MD5_Update(&ctx, buffer, file.gcount());
    }
    file.close();

    MD5_Final(digest, &ctx);

    std::ostringstream oss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    return oss.str();
}

// ------------------------------
// 服务端：分包接收 + 重组 + 校验
// ------------------------------
bool DBusService::ReceiveFileChunk(int seq, int len)
{
    // ✅ 文件传输结束信号（len == 0）
    if (len == 0) {
        std::cout << "[server] File transfer complete. Calculating MD5...\n";

        // 对文件计算 MD5
        std::ifstream file("received_files/tmp_received", std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "[server] Failed to open received file for MD5 check.\n";
            return false;
        }

        MD5_CTX md5_ctx;
        MD5_Init(&md5_ctx);

        char buf[4096];
        while (file.read(buf, sizeof(buf)) || file.gcount() > 0) {
            MD5_Update(&md5_ctx, buf, file.gcount());
        }

        unsigned char md5_result[MD5_DIGEST_LENGTH];
        MD5_Final(md5_result, &md5_ctx);
        file.close();

        // 输出 MD5 结果
        std::ostringstream md5_str;
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i)
            md5_str << std::hex << std::setw(2) << std::setfill('0') << (int)md5_result[i];

        std::cout << "[server] ✅ MD5: " << md5_str.str() << "\n";
        std::cout<< std::endl;

        // 删除共享内存
        shm_unlink(SHM_NAME);
        return true;
    }

    // ✅ 打开共享内存（客户端已创建）
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return false;
    }

    // ✅ 映射共享内存
    void* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        return false;
    }

    // ✅ 从共享内存中取出包数据
    FileChunk chunk{};
    memcpy(&chunk, shm_ptr, sizeof(FileChunk));

    // ✅ 确保保存目录存在
    system("mkdir -p received_files");

    // ✅ 构建文件路径（临时保存）
    std::string file_path = "received_files/tmp_received";

    // ✅ 打印当前包信息
    std::cout << "[server] Received chunk #" << chunk.seq
              << " (" << chunk.len << " bytes of "
              << chunk.total_chunks << " total)\n";

    // ✅ 追加写入文件
    std::ofstream ofs(file_path, std::ios::binary | std::ios::app);
    if (!ofs.is_open()) {
        std::cerr << "[server] Failed to open output file.\n";
        munmap(shm_ptr, SHM_SIZE);
        close(shm_fd);
        return false;
    }

    ofs.write(reinterpret_cast<char*>(chunk.data), chunk.len);
    ofs.close();

    // ✅ 清理共享内存映射
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);

    return true;
}



void DBusService::StartLoop() {
    while (true) {
        dbus_connection_read_write(conn, 50);
        DBusMessage* msg = dbus_connection_pop_message(conn);
        if (!msg) continue;

 
        int msg_type = dbus_message_get_type(msg);
        const char* member = dbus_message_get_member(msg);
        const char* iface = dbus_message_get_interface(msg);
        const char* path = dbus_message_get_path(msg);

        // std::cout << "[server] Message: type=" << msg_type
        //   << ", interface=" << (iface ? iface : "null")
        //   << ", member=" << (member ? member : "null")
        //   << ", path=" << (path ? path : "null") << std::endl;

        //std::cout << std::endl;

        if (dbus_message_is_signal(msg, "com.demo.Interface", "OnFileChunkReceived")) {
            int seq = 0;
            int len = 0;

            // 从 D-Bus 信号中提取参数（包号和长度）
            if (!dbus_message_get_args(msg, nullptr,
                                    DBUS_TYPE_INT32, &seq,
                                    DBUS_TYPE_INT32, &len,
                                    DBUS_TYPE_INVALID)) {
                std::cerr << "[server] Failed to parse OnFileChunkReceived args\n";
                dbus_message_unref(msg);
                continue;
            }

            // 调试打印
            std::cout << "[server]  Got OnFileChunkReceived signal: seq="
                    << seq << ", len=" << len << std::endl;

            // 调用接收函数（根据 seq 与 len 拼包）
            ReceiveFileChunk(seq, len);
        }


        // 匹配方法调用 SetTestInt
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestInt")) {
            int val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_INT32, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestInt: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestInt(val);
        }
        
        // 匹配方法调用 GetTestInt
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestInt")) {
            int value = GetTestInt();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &value);

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestInt!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        // 匹配方法调用 SetTestBool
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestBool")) {
            bool val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_BOOLEAN, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestBool: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestBool(val);
        }

        // 匹配方法调用 GetTestBool
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestBool")) {
            bool value = GetTestBool();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &value);

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestBool!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        // 匹配方法调用 SetTestDouble
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestDouble")) {
            double val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_DOUBLE, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestDouble: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestDouble(val);
        }

        // 匹配方法调用 GetTestDouble
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestDouble")) {
            double value = GetTestDouble();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &value);

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestDouble!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        // 匹配方法调用 SetTestString
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestString")) {
            const char* val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_STRING, &val,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestString: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestString(val);
        }

        // 匹配方法调用 GetTestString
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestString")) {
            // 获取当前的字符串值
            std::string value = GetTestString();
            const char* str_data = value.c_str(); // 获取字符串指针

            // 创建返回消息
            DBusMessage* reply = dbus_message_new_method_return(msg);
            if (!reply) {
                std::cerr << "[server] Failed to create reply for GetTestString!" << std::endl;
                dbus_message_unref(msg);
                return;
            }

            // 初始化消息迭代器（不需要检查返回值）
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);  // 不需要检查返回值

            // 直接添加字符串参数
            dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &str_data);

            // 发送消息
            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestString!" << std::endl;
            }
            dbus_connection_flush(conn);

            // 清理资源
            dbus_message_unref(reply);
        }



        // 匹配方法调用 SetTestInfo
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "SetTestInfo")) {
            TestInfo val;
            DBusError err;
            dbus_error_init(&err);

            dbus_message_get_args(msg, &err,
                                  DBUS_TYPE_BOOLEAN, &val.bool_param,
                                  DBUS_TYPE_INT32, &val.int_param,
                                  DBUS_TYPE_DOUBLE, &val.double_param,
                                  DBUS_TYPE_STRING, &val.string_param,
                                  DBUS_TYPE_INVALID);

            if (dbus_error_is_set(&err)) {
                std::cerr << "[server] Error getting arguments for SetTestInfo: " << err.message << std::endl;
                dbus_error_free(&err);
                continue;
            }

            SetTestInfo(val);
        }

        // 匹配方法调用 GetTestInfo
        else if (dbus_message_is_method_call(msg, "com.demo.Interface", "GetTestInfo")) {
            TestInfo value = GetTestInfo();

            DBusMessage* reply = dbus_message_new_method_return(msg);
            DBusMessageIter args;
            dbus_message_iter_init_append(reply, &args);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &value.bool_param);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &value.int_param);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &value.double_param);
            dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, value.string_param.c_str());

            if (!dbus_connection_send(conn, reply, nullptr)) {
                std::cerr << "[server] Failed to send method return for GetTestInfo!" << std::endl;
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        dbus_message_unref(msg);
    }
}
