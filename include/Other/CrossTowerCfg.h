#ifndef _CROSSTOWERCFG_H_
#define _CROSSTOWERCFG_H_

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

class CrossTowerCfg
{
public:
    CrossTowerCfg();
    ~CrossTowerCfg();
    CrossTowerCfg(const CrossTowerCfg& other);

private:
    int32_t MapId;
    int32_t NextMapId;
    int32_t LastMapId;
    int32_t MaxPlayer;
    int32_t MaxScore;
    int32_t MinScore;
    int32_t KillCount;
    std::vector<MemChrBag> RewardVt;
    std::vector<MemChrBag> TopRewardVt;
};

#endif // _CROSSTOWERCFG_H_
