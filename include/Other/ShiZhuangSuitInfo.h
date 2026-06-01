#ifndef _SHIZHUANGSUITINFO_H_
#define _SHIZHUANGSUITINFO_H_

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

struct AddAttribute;

class ShiZhuangSuitInfo
{
public:
    ShiZhuangSuitInfo(ShiZhuangSuitInfo *const this, const ShiZhuangSuitInfo *const a2);

    int32_t nLevel;
    int32_t nSuitLevel;
    std::list<AddAttribute> vAttr;
};

#endif // _SHIZHUANGSUITINFO_H_
