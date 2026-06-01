#ifndef _ULTIMATECHALLENGECFG_H_
#define _ULTIMATECHALLENGECFG_H_

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

class UltimateChallengeCfg
{
public:
    UltimateChallengeCfg();
    ~UltimateChallengeCfg();
    UltimateChallengeCfg(const UltimateChallengeCfg& other);

    std::vector<MemChrBag> GetItems;
    std::list<Param2> DropRateList;
};

#endif // _ULTIMATECHALLENGECFG_H_