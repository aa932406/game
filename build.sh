#!/bin/bash
# GameServer 构建脚本

set -e

echo "========================================"
echo "  GameServer 构建脚本"
echo "========================================"

# 创建构建目录
mkdir -p build
cd build

# 运行 CMake
echo "[1/3] 运行 CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译
echo "[2/3] 编译..."
make -j$(nproc)

echo "[3/3] 构建完成!"
echo ""
echo "可执行文件: build/gameserver"
