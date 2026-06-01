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

public:
    std::map<int, CfgShiZhuang> m_mShiZhuang;
    std::map<int, CfgShiZhuangLevel> m_mShiZhuangLevel;
};

#endif // _CFGCFGSHIZHUANGTABLE_H_
