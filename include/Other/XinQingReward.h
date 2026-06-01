#ifndef _XINQINGREWARD_H_
#define _XINQINGREWARD_H_

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

struct MemChrBag;

class XinQingReward
{
public:
    XinQingReward(XinQingReward *const this, const XinQingReward *const a2);

    int32_t GongGaoId;
    std::vector<MemChrBag> Items;
};

#endif // _XINQINGREWARD_H_