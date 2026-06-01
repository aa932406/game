#ifndef _CFGSHOUCHONG_H_
#define _CFGSHOUCHONG_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgShouChong - Config data class
 */
class CfgShouChong
{
public:
    int32_t nIndex;
    int32_t nGold;
    int32_t nBroadcast;
    std::vector<CfgEquipItem> vReward;

    CfgShouChong() = default;
    CfgShouChong(const CfgShouChong& a2);
    ~CfgShouChong() = default;
    CfgShouChong& operator=(const CfgShouChong& a2);
};

#endif // _CFGSHOUCHONG_H_
