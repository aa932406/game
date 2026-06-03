# GameServer 源代码项目

## 项目说明

本项目基于反编译代码重构的 C++ 游戏服务器源代码，包含完整的地图、副本、活动、战斗、经济等 MMORPG 核心系统。

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

## 修复进度

### ✅ 已完成修复（目标文件 0 编译错误）

| 阶段 | 文件 | 修复内容 | 错误数 |
|------|------|----------|--------|
| 1 | `include/Database/MailDBData.h` | 移除 `PlayerDBData.h` 循环依赖 | 0 |
| 1 | `include/Database/ExchangeDBData.h` | 同上 | 0 |
| 1 | `include/Database/MagicBoxDBData.h` | 同上 | 0 |
| 1 | `include/Database/MemPetDBData.h` | 同上 | 0 |
| 1 | `include/Database/MysteryShopDBData.h` | 同上 | 0 |
| 1 | `include/Database/OperateLimitDBData.h` | 同上 | 0 |
| 1 | `include/Database/PetDBData.h` | 同上 | 0 |
| 1 | `include/Database/PortalDBData.h` | 同上 | 0 |
| 1 | `include/Database/WorldBossInfo.h` | 同上 | 0 |
| 1 | `include/Database/WorshipDBData.h` | 同上 | 0 |
| 1 | `include/Database/WuHunShopDBData.h` | 同上 | 0 |
| 1 | `include/Database/XinMoDBData.h` | 同上 | 0 |
| 1 | `include/Database/PlayerDBData.h` | 添加 forward-declaration，完整重写 | 0 |
| 1 | `src/Database/PlayerDBData.cpp` | 精确重写 `CleanUp()` | 0 |
| 1 | `src/Database/PortalDBData.cpp` | 添加缺失 include | 0 |
| 1 | `src/Database/XinMoDBData.cpp` | 添加缺失 include | 0 |
| 1 | `include/Character/CExtFlopDraw.h` | 私有成员改为 `public` | 0 |
| 1 | `include/Character/CLittleHelperData.h` | 同上 | 0 |
| 1 | `include/Character/CJueWeiData.h` | 同上 | 0 |
| 1 | `include/Character/MemChrSkillData.h` | 同上 | 0 |
| 2 | `include/Game/CCharWuHun.h` | 完整重写：继承 `CExtSystemBase`，添加成员 | 0 |
| 2 | `src/Game/CCharWuHun.cpp` | 完整重写：清除 186 个反编译器错误 | 0 |
| 2 | `include/Common/CommonTypes.h` | 添加 `ICR_WU_HUN` 枚举值 | 0 |
| 2 | `include/Character/CExtCharBag.h` | 添加 `ICR_WU_HUN` 枚举值 | 0 |
| 2 | `include/Config/CfgData.h` | `RateItem` 结构体填充字段 | 0 |
| 2 | `include/Other/CreateWuHun.h` | 成员改为 `public`；`SpecialCost` 改为 `ItemData` | 0 |
| 2 | `include/Other/WuHunItem.h` | 修复反斜杠语法错误 | 0 |
| 3 | `include/Game/CDropItem.h` | 完整重写：添加 `StaticObj.h` include，`DropItemData` 结构体 | 0 |
| 3 | `src/Game/CDropItem.cpp` | 完整重写：清除 75 个反编译器错误 | 0 |
| 3 | `include/Common/CommonTypes.h` | 移除 `enum class ITEM_CHANGE_REASON`（解决与 `CExtCharBag.h` 重复定义） | 0 |
| 3 | `include/Common/CommonTypes.h` | 添加 `GCR/MCR_GET_DROP_ITEM` 到 `CURRENCY_CHANGE_REASON` | 0 |
| 3 | `include/Common/CurrencyType.h` | 扩展 `CURRENCY_TYPE` 枚举；移除 `CURRENCY_CHANGE_REASON` | 0 |
| 3 | `include/Character/CExtCharBag.h` | 添加 701-708 区间缺失的枚举值 | 0 |
| 3 | `include/Game/Player.h` | 添加 `GetTeamId` 静态包装方法 | 0 |

### 🔄 剩余待修复文件（按错误数排序）

| 文件 | 错误数 | 状态 |
|------|--------|------|
| `src/Game/GameService.cpp` | 1482 | 待修复 |
| `src/Other/CFestivalDoubleEleven.cpp` | 1252 | 待修复 |
| `src/Game/Player.cpp` | 713 | 待修复 |
| `src/Game/Monster.cpp` | 549 | 待修复 |
| `src/Game/DBService.cpp` | 528 | 待修复 |
| `src/Other/ChrTask.cpp` | 459 | 待修复 |
| `src/Other/CYunYingHD.cpp` | 426 | 待修复 |
| `src/Game/CObjPet.cpp` | 421 | 待修复 |
| `src/Other/CCityWar.cpp` | 348 | 待修复 |
| `src/Game/Map.cpp` | 305 | 待修复 |
| `src/Other/CUniteServer.cpp` | 298 | 待修复 |
| `src/Game/Dungeon.cpp` | 297 | 待修复 |
| ... | ... | ... |
| **总计** | **~15000** | 逐步修复中 |

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
