#ifndef _XINMOQIQINGLEVELUPCFG_H_
#define _XINMOQIQINGLEVELUPCFG_H_

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

class XinMoQiQingLevelUpCfg
{
public:
    XinMoQiQingLevelUpCfg();
    ~XinMoQiQingLevelUpCfg();
    XinMoQiQingLevelUpCfg(const XinMoQiQingLevelUpCfg& other);

    std::list<ItemData> lCostList;
    std::list<AddAttribute> lAttr;
};

#endif // _XINMOQIQINGLEVELUPCFG_H_