# GameServer 源代码项目

## 项目说明

本项目是从反编译代码自动生成的C++源代码框架，基于 `gameserver.cc` 反编译文件。

## 目录结构

```
gameserver_src/
├── CMakeLists.txt          # CMake 构建配置
├── build.sh                # 一键构建脚本
├── README.md               # 本文件
├── include/                # 头文件目录
│   ├── Activity/           # 活动系统核心类
│   ├── ActivityMap/        # 活动地图类
│   ├── ActivityManager/    # 活动管理器
│   ├── Config/             # 配置数据类 (Cfg*)
│   ├── Character/          # 角色扩展系统 (CExt*)
│   ├── Game/               # 游戏核心系统
│   ├── Network/            # 网络模块
│   ├── Database/           # 数据库模块
│   ├── Utility/            # 工具类
│   └── Other/              # 其他业务类
└── src/                    # 源文件目录（结构与include对应）
```

## 构建方法

### 使用 CMake (推荐)

```bash
cd gameserver_src
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 使用一键脚本

```bash
cd gameserver_src
./build.sh
```

## 文件统计

- 总类数: ~574 个
- 头文件: ~574 个 (.h)
- 源文件: ~574 个 (.cpp)
- 总文件数: ~1148 个

## 重要提示

### ⚠️ 需要手动修复的内容

1. **成员变量类型**
   - 当前使用启发式推断，可能不准确
   - 需要根据反编译代码中的实际使用确认类型
   - 特别是 `std::list<T>`、`std::map<K,V>` 的模板参数

2. **继承关系**
   - 反编译代码丢失了 `class X : public Y` 信息
   - 需要从 `dynamic_cast` 和虚函数表推断继承关系
   - 例如: `CActivityMap` 应该继承 `Map`

3. **虚函数表(vptr)**
   - 需要手动确认虚函数表的偏移量
   - 反编译代码中显示为 `off_8C14D0` 等地址

4. **函数实现**
   - 当前源文件中函数体为空，只有TODO注释
   - 需要从反编译代码中还原实际逻辑

5. **宏定义和枚举**
   - `ACTIVITY_STATE`、`MapType`、`GameMsgCode` 等枚举需要定义
   - 原始宏定义丢失，需要手动补充

6. **第三方库依赖**
   - MySQL客户端库
   - 可能的网络库 (libcurl, boost.asio等)
   - 需要根据原始项目配置添加

## 开发建议

1. **先修复基础类**
   - Position, UnitHandle, GameMsgCode 等工具类
   - Map, Player, Unit, Monster 等核心游戏类

2. **再修复活动系统**
   - CActivity (基类)
   - CActivityMap
   - CActivityManager
   - 具体活动类 (CFamilyWar, CCampWar等)

3. **最后修复配置系统**
   - CfgData (配置管理器)
   - 各类 Cfg* 配置结构体

## 许可证

本项目代码来源于反编译，仅供学习和研究使用。
