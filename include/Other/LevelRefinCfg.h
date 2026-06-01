#ifndef _LEVELREFINCFG_H_
#define _LEVELREFINCFG_H_

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

class LevelRefinCfg
{
public:
    LevelRefinCfg();
    ~LevelRefinCfg();
    LevelRefinCfg(const LevelRefinCfg& other);

private:
    int32_t nLevel;
    std::vector<int> vNeedGold;
    std::vector<int> nLevelUp;
    int32_t nLimit;
    int32_t nGongGaoId;
};

#endif // _LEVELREFINCFG_H_
