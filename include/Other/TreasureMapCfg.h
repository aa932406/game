#ifndef _TREASUREMAPCFG_H_
#define _TREASUREMAPCFG_H_

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
struct MapPos;

class TreasureMapCfg
{
public:
    TreasureMapCfg();
    ~TreasureMapCfg();
    TreasureMapCfg(const TreasureMapCfg& other);

    std::map<int, int> m_EventRate;
    int32_t m_ItemId;
    std::vector<MapPos> m_MaxPosVector;
    int32_t m_MaxProbability;
};

#endif // _TREASUREMAPCFG_H_
