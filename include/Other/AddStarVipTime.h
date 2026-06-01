#ifndef _ADDSTARVIPTIME_H_
#define _ADDSTARVIPTIME_H_

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

class AddStarVipTime : public ItemEffect
{
public:
    AddStarVipTime();
    virtual ~AddStarVipTime();

    bool parseEffect(int32_t id, const std::string& strEffect);

private:
    int32_t Value;
};

#endif // _ADDSTARVIPTIME_H_
