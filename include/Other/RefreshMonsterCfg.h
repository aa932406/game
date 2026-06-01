#ifndef _REFRESHMONSTERCFG_H_
#define _REFRESHMONSTERCFG_H_

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

class RefreshMonsterCfg
{
public:
    RefreshMonsterCfg(RefreshMonsterCfg *const this, const RefreshMonsterCfg *const a2);

    int32_t nIndex;
    int32_t nType;
    int32_t nId;
    int32_t nCount;
    int32_t nStartTime;
    int32_t nDiffTime;
    std::vector<Position> vRevivePosVector;
};

#endif // _REFRESHMONSTERCFG_H_
