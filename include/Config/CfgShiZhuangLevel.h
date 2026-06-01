#ifndef _CFGSHIZHUANGLEVEL_H_
#define _CFGSHIZHUANGLEVEL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgShiZhuangLevel - Config data class
 */
class CfgShiZhuangLevel
{
public:
    int32_t nType;
    int32_t nLevel;
    int32_t nLevelExp;
    int32_t nNeedLevel;
    int32_t nCostItem;
    int32_t nGetExp;
    int32_t nSmallCritRate;
    int32_t nLargeCritRate;
    int32_t nBroadcast;
    std::list<AddAttribute> vAttr;

    CfgShiZhuangLevel() = default;
    CfgShiZhuangLevel(const CfgShiZhuangLevel& a2);
    ~CfgShiZhuangLevel() = default;
    CfgShiZhuangLevel& operator=(const CfgShiZhuangLevel& a2);
};

#endif // _CFGSHIZHUANGLEVEL_H_
