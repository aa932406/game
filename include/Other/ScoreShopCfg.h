#ifndef _SCORESHOPCFG_H_
#define _SCORESHOPCFG_H_

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

class ScoreShopCfg
{
public:
    ScoreShopCfg(const ScoreShopCfg\& a2);

    int32_t Index;
    int32_t PlayerLevel;
    int32_t LimitType;
    int32_t LimitCount;
    int32_t Item;
    int32_t CostType;
    int32_t CostValue;
};

#endif // _SCORESHOPCFG_H_
