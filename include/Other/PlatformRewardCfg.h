#ifndef _PLATFORMREWARDCFG_H_
#define _PLATFORMREWARDCFG_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Common/CommonTypes.h"

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class PlatformRewardCfg
{
public:
    PlatformRewardCfg();
    ~PlatformRewardCfg();
    PlatformRewardCfg(const PlatformRewardCfg& other);

    int32_t nId;
    std::vector<MemChrBag> vItem;
    int32_t nIcon;
    std::string sPlatform;
    int32_t nIsAllShow;
};

#endif // _PLATFORMREWARDCFG_H_
