#ifndef _CFGCFGVPLAN_H_
#define _CFGCFGVPLAN_H_

#include "Common/CommonTypes.h"
#include "Other/VplanReward.h"
#include "Other/VplanLevelReward.h"
#include "Other/VplanEveryDayReward.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgVplan - Config data class
 */
class CfgVplan
{
public:
    std::map<int, VplanReward> m_VplanRewardMap;
    std::map<int, VplanLevelReward> m_VplanLevelRewardMap;
    std::map<int, VplanEveryDayReward> m_VplanEveryDayRewardMap;

    CfgVplan() = default;
    ~CfgVplan() = default;
    CfgVplan(const CfgVplan& a2);
    CfgVplan& operator=(const CfgVplan& a2);
};

#endif // _CFGCFGVPLAN_H_
