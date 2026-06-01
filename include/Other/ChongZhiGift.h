#ifndef _CHONGZHIGIFT_H_
#define _CHONGZHIGIFT_H_

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

class ChongZhiGift : public ItemEffect
{
public:
    ChongZhiGift();
    virtual ~ChongZhiGift();

    bool parseEffect(int32_t id, const std::string& strEffect);

private:
    int32_t nId;
    int32_t Value;
};

#endif // _CHONGZHIGIFT_H_