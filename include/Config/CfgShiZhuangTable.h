#ifndef _CFGCFGSHIZHUANGTABLE_H_
#define _CFGCFGSHIZHUANGTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgShiZhuang.h"
#include "Config/CfgShiZhuangLevel.h"


/**
 * @brief CfgShiZhuangTable - Config data class
 */
class CfgShiZhuangTable
{
public:
    CfgShiZhuangTable();
    ~CfgShiZhuangTable();

    void Add(const CfgShiZhuang& stu);
    void AddLevel(const CfgShiZhuangLevel& stu);
    const CfgShiZhuang* GetShiZhuang(int32_t nId);
    const CfgShiZhuangLevel* GetShiZhuangLevel(int32_t nType, int32_t nLevel);
    int32_t GetMaxLevel(int32_t nType);
    bool IsExist(int32_t nId);
    void Clear();

public:
    std::map<int32_t, CfgShiZhuang> m_mShiZhuang;
    std::map<int32_t, std::map<int32_t, CfgShiZhuangLevel>> m_mShiZhuangLevel;
};

#endif // _CFGCFGSHIZHUANGTABLE_H_
