// CommonTypes.h
#ifndef COMMONTYPES_H
#define COMMONTYPES_H

#include <cstdint>
#include <string>
#include <list>
#include <vector>
#include <map>

// 基础类型定义
typedef int64_t CharId_t;
typedef int64_t EquipId_t;
typedef int64_t FamilyId_t;
typedef int64_t EntityId_t;
typedef int32_t Time_t;
typedef int32_t ItemId_t;
typedef int32_t ProcId_t;
typedef int32_t GateIndex_t;
typedef int8_t ConnId_t;
typedef int32_t Job_t;
typedef int8_t Sex_t;
typedef int32_t ServerId_t;

// 方向枚举 (仅在此处定义，防止与 CfgData.h 冲突)
#ifndef DIRECTION_ENUM_DEFINED
#define DIRECTION_ENUM_DEFINED
typedef int8_t Direction;
#endif

// 基础结构体前向声明
struct AttrAddon;
struct Position;
struct ItemData;
struct MemChrBag;
struct CfgMapRegion;
struct AddAttribute;

#endif