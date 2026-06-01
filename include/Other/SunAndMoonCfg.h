#ifndef _SUNANDMOONCFG_H_
#define _SUNANDMOONCFG_H_

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
struct ItemData;
struct AddAttribute;

class SunAndMoonCfg
{
public:
    SunAndMoonCfg();
    ~SunAndMoonCfg();
    SunAndMoonCfg(const SunAndMoonCfg& other);

private:
    int32_t nLevel;
    std::list<ItemData> lConstItems;
    std::list<AddAttribute> lAttr;
    int32_t nGetItemTimes;
    int32_t nSunTelentId;
    int32_t nSunTelentLevel;
    int32_t nMoonTelentId;
};

#endif // _SUNANDMOONCFG_H_
