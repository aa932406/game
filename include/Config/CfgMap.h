#ifndef _CFGMAP_H_
#define _CFGMAP_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgMap - Config data class
 */
class CfgMap
{
public:
    int32_t id;
    std::string name;
    int32_t type;
    int32_t param;
    int32_t width;
    int32_t height;
    int32_t revive;
    int32_t reviveTime;
    int32_t pk_mode;
    int32_t anti_protect;
    int32_t isMount;
    int32_t isFly;
    int32_t isVicegeneral;
    int32_t player_level;
    int32_t player_level_max;
    int32_t team_member;
    int32_t hide_mini;
    int32_t runnerId;
    int32_t jump;
    int32_t GongGaoId;
    int32_t pvp;
    int32_t cross;
    int32_t x;
    int32_t y;
    int32_t drop_rate;
    int32_t XingMaiLevel;
    int32_t SuitId;
    int32_t VipLevel;
    int32_t JueWeiLevel;
    int32_t NeedBroadcast;
    int32_t MapCanUsePet;
    int32_t SignOut;
    int32_t Rdx;
    int32_t Refresh;
    int32_t CanRand;
    int32_t LimitDay;
    int32_t XinMoAct;

    CfgMap() = default;
    CfgMap(const CfgMap& a2);
    ~CfgMap() = default;
    CfgMap& operator=(const CfgMap& a2);
};

#endif // _CFGMAP_H_
