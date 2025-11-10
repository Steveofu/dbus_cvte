#!/bin/bash

# 构建脚本
set -e

BUILD_TYPE=${1:-Debug}  # 默认为Debug构建

echo "开始构建 Test Service 和 Client..."
echo "构建类型: $BUILD_TYPE"

# 清理旧的构建目录
rm -rf Service/build Client/build

# 构建Service
echo "=== 构建 Service ==="
cd Service
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make -j$(nproc)
cd ../..

# 构建Client
echo "=== 构建 Client ==="
cd Client
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make -j$(nproc)
cd ../..

echo "构建完成！"
echo "Service可执行文件: Service/build/bin/server"
echo "Service动态库: Service/build/lib/libtraining.so"
echo "Client可执行文件: Client/build/bin/client"

# 提供运行示例
echo ""
echo "运行方法:"
echo "1. 在终端1中运行: ./Service/build/bin/server"
echo "2. 在终端2中运行: ./Client/build/bin/client"
