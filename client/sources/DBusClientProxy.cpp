#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "DBusClientProxy.h"
#include <iostream>
#include <algorithm>

#define SHM_NAME "/file_shm"  // 共享内存的名称
#define SHM_SIZE 1024         // 每次传输1KB数据



DBusClientProxy::DBusClientProxy() {
    DBusError err;
    dbus_error_init(&err);
    
    // like create socket
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    
    if (dbus_error_is_set(&err)) {
        std::cout << "DBus error: " << err.message << std::endl;
        dbus_error_free(&err);
    }
}

DBusClientProxy::~DBusClientProxy() {}


bool DBusClientProxy::callBool(const char* method, bool value) 
{
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        method
    );

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &value);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    return true;
}


bool DBusClientProxy::SetTestBool(bool param) {
    return callBool("SetTestBool", param);
}


bool DBusClientProxy::SetTestInt(int value) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestInt"
    );

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &value);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    return true;
}


bool DBusClientProxy::SetTestDouble(double value) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestDouble"
    );

    if (!msg) {
        std::cerr << "dbus_message_new_method_call failed" << std::endl;
        return false;
    }

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);

    // DBus 不存在 double 类型，但用 DBUS_TYPE_DOUBLE 就行
    dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &value);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);

    dbus_message_unref(msg);

    return true;
}



bool DBusClientProxy::SetTestString(std::string value) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestString"
    );

    if (!msg) return false;

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);

    const char* cstr = value.c_str();
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &cstr);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);
    return true;
}


bool DBusClientProxy::SetTestInfo(TestInfo info) {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "SetTestInfo"
    );

    if (!msg) return false;

    DBusMessageIter args;
    dbus_message_iter_init_append(msg, &args);

    // bool（必须转换成 dbus_bool_t）
    dbus_bool_t flag = info.bool_param;
    dbus_message_iter_append_basic(&args, DBUS_TYPE_BOOLEAN, &flag);

    // int
    dbus_message_iter_append_basic(&args, DBUS_TYPE_INT32, &info.int_param);

    // double
    dbus_message_iter_append_basic(&args, DBUS_TYPE_DOUBLE, &info.double_param);

    // string（必须 char*）
    const char* cstr = info.string_param.c_str();
    dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &cstr);

    dbus_connection_send(conn, msg, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(msg);

    return true;
}



bool DBusClientProxy::GetTestBool() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestBool"
    );

    DBusError err;
    dbus_error_init(&err);

    // 发送消息并等待服务端响应
    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestBool): " << err.message << std::endl;
        dbus_error_free(&err);
        return false;
    }

    // 从 reply 里取出一个 bool 类型参数（即服务端返回的值）
    dbus_bool_t val = false;
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_BOOLEAN, &val,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return val;
}


int DBusClientProxy::GetTestInt() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestInt"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );

    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error: " << err.message << std::endl;
        dbus_error_free(&err);
        return -1;
    }

    int value = 0;
    dbus_message_get_args(reply, &err, DBUS_TYPE_INT32, &value, DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return value;
}


double DBusClientProxy::GetTestDouble() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestDouble"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestDouble): " << err.message << std::endl;
        dbus_error_free(&err);
        return 0.0;
    }

    double value = 0.0;
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_DOUBLE, &value,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return value;
}


std::string DBusClientProxy::GetTestString() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestString"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestString): " << err.message << std::endl;
        dbus_error_free(&err);
        return "";
    }

    const char* str = nullptr;
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_STRING, &str,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);
    return str ? std::string(str) : "";
}


TestInfo DBusClientProxy::GetTestInfo() {
    DBusMessage* msg = dbus_message_new_method_call(
        "com.demo.Service",
        "/com/demo/Object",
        "com.demo.Interface",
        "GetTestInfo"
    );

    DBusError err;
    dbus_error_init(&err);

    DBusMessage* reply = dbus_connection_send_with_reply_and_block(
        conn, msg, 2000, &err
    );
    dbus_message_unref(msg);

    TestInfo info;

    if (dbus_error_is_set(&err)) {
        std::cerr << "DBus error(GetTestInfo): " << err.message << std::endl;
        dbus_error_free(&err);
        return info;  // 返回空结构体
    }

    // 临时变量接收 DBus 参数
    dbus_bool_t flag;
    int number;
    double ratio;
    const char* msg_str = nullptr;

    // 从 reply 中解析参数
    dbus_message_get_args(reply, &err,
                          DBUS_TYPE_BOOLEAN, &flag,
                          DBUS_TYPE_INT32,   &number,
                          DBUS_TYPE_DOUBLE,  &ratio,
                          DBUS_TYPE_STRING,  &msg_str,
                          DBUS_TYPE_INVALID);

    dbus_message_unref(reply);

    // 按 TestInfo.h 的字段名赋值
    info.bool_param = flag;
    info.int_param = number;
    info.double_param = ratio;
    info.string_param = msg_str ? std::string(msg_str) : "";

    return info;
}


// 客户端：从文件路径读取内容，分包写入共享内存并通过 D-Bus 通知服务端
bool DBusClientProxy::SendFile(const std::string& file_path) 
{
    // 读取文件内容到内存
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[client]  Failed to open file: " << file_path << std::endl;
        return false;
    }

    // get the size of the file
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<unsigned char> file_buf(file_size);

    // 强制转换
    file.read(reinterpret_cast<char*>(file_buf.data()), file_size);
    file.close();


    std::cout << "[client]  Loaded file '" << file_path << "' (" << file_size << " bytes) << successfully \n";

    // 创建或重置共享内存（每次 1KB）
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return false;
    }

    ftruncate(shm_fd, SHM_SIZE);

    void* shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        close(shm_fd);
        return false;
    }

    // 分包结构体定义
    struct FilePack{
        int seq;             // 当前包序号（从0开始）
        int len;             // 当前包长度
        int total_packs;    // 总包数
        char filename[64];   // 文件名（用于服务端保存）
        unsigned char data[SHM_SIZE - 80]; // 实际数据（留头部空间）
    };

    // 计算总包数
    // 每包数据容量
    const int MAX_DATA_PER_Pack = sizeof(FilePack::data);
    int total_packs = (file_size + MAX_DATA_PER_Pack - 1) / MAX_DATA_PER_Pack;

    size_t bytes_sent = 0;
    int seq = 0;

    // 循环分块写入共享内存并发信号
    while (bytes_sent < file_size) {
        FilePack pack{};
        pack.seq = seq;
        pack.len = std::min((int)MAX_DATA_PER_Pack, (int)(file_size - bytes_sent));
        pack.total_packs = total_packs;

        // 拷贝文件名（取 basename）
        std::string base_name = file_path.substr(file_path.find_last_of('/') + 1);
        strncpy(pack.filename, base_name.c_str(), sizeof(pack.filename) - 1);

        // 拷贝当前数据块
        memcpy(pack.data, file_buf.data() + bytes_sent, pack.len);

        // 写入共享内存
        memcpy(shm_ptr, &pack, sizeof(pack));

        // 发送 D-Bus 信号通知服务端
        NotifyServiceFileChunkReceived(seq, pack.len);

        std::cout << "[client] Sent pack #" << seq << " (" << pack.len << " bytes)\n";

        bytes_sent += pack.len;
        seq++;
        usleep(20000); // 控制发送速率（20ms）
    }

    // 发送结束信号（len=0 表示文件传输完成）
    NotifyServiceFileChunkReceived(seq, 0);

    std::cout << "[client] File send complete: " << total_packs << " packs, " << file_size << " bytes.\n";

    // 理资源
    munmap(shm_ptr, SHM_SIZE);
    close(shm_fd);

    return true;
}



// 客户端：发送文件块信号（带序号和长度参数）
void DBusClientProxy::NotifyServiceFileChunkReceived(int seq, int len) {
    DBusMessage* signal = dbus_message_new_signal(
        "/com/demo/Object",       // 路径：必须一致
        "com.demo.Interface",     // 接口名：必须一致
        "OnFileChunkReceived"     // 信号名
    );

    dbus_message_append_args(signal,
                             DBUS_TYPE_INT32, &seq,
                             DBUS_TYPE_INT32, &len,
                             DBUS_TYPE_INVALID);

    dbus_connection_send(conn, signal, nullptr);
    dbus_connection_flush(conn);
    dbus_message_unref(signal);
}

