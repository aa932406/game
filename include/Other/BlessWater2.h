#ifndef _BLESSWATER2_H_
#define _BLESSWATER2_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

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
#include "Other/ItemEffect.h"

class BlessWater2 : public ItemEffect
{
public:
    BlessWater2();
    virtual ~BlessWater2();

    bool parseEffect(int32_t id, const std::string& strEffect);

private:
    int32_t m_nId;
};

#endif // _BLESSWATER2_H_
