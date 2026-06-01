#ifndef _CFGGOBLINDATA_H_
#define _CFGGOBLINDATA_H_

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
struct CurrentData;

class cfgGoblinData
{
public:
    cfgGoblinData();
    ~cfgGoblinData();
    cfgGoblinData(const cfgGoblinData& other);

private:
    std::vector<CurrentData> m_RefreshMonsters;
    std::vector<Position> m_RevivePosVector;
    int32_t m_mapId;
    int32_t m_nIndex;
    int64_t m_nRefreshTime;
};

#endif // _CFGGOBLINDATA_H_
