#ifndef _PLATFORMDATING_H_
#define _PLATFORMDATING_H_

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

class PlatformDaTing
{
public:
    PlatformDaTing();
    ~PlatformDaTing();
    PlatformDaTing(const PlatformDaTing& other);

    std::string Platform;
    int32_t nId;
    int32_t nType;
    int32_t nParam;
    std::vector<MemChrBag> vItems;
    int32_t nIcon;
    int32_t nParam2;
};

#endif // _PLATFORMDATING_H_
