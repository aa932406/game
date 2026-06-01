#ifndef _ZEROBUYPETCFG_H_
#define _ZEROBUYPETCFG_H_

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

class ZeroBuyPetCfg
{
public:
    ZeroBuyPetCfg();
    ZeroBuyPetCfg(const ZeroBuyPetCfg& other);

    bool IsEmpty() const;

    std::vector<MemChrBag> cItems;
    int32_t nTime;
    int32_t nGold;
    int32_t nMailId;
};

#endif // _ZEROBUYPETCFG_H_
