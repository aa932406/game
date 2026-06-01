#ifndef _PLATFORMVIP_H_
#define _PLATFORMVIP_H_

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

class PlatformVip
{
public:
    PlatformVip();
    ~PlatformVip();
    PlatformVip(const PlatformVip& other);

    std::string sPlatform;
    int32_t nId;
    int32_t nNeedVipLevel;
    std::vector<MemChrBag> vItem;
    int32_t nIcon;
};

#endif // _PLATFORMVIP_H_
