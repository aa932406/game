#ifndef _CFGONLINEREWARD_H_
#define _CFGONLINEREWARD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgOnlineReward - Config data class
 */
class CfgOnlineReward
{
public:
    int32_t Id;
    int32_t NeedTime;
    std::vector<MemChrBag> ItemVector;

    CfgOnlineReward() = default;
    CfgOnlineReward(const CfgOnlineReward& a2);
    ~CfgOnlineReward() = default;
    CfgOnlineReward& operator=(const CfgOnlineReward& a2);
};

#endif // _CFGONLINEREWARD_H_
