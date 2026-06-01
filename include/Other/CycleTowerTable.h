#ifndef _CYCLETOWERTABLE_H_
#define _CYCLETOWERTABLE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;
struct CycleTowerMapCfg;
struct CycleTowerCost;
struct ItemData;

class CycleTowerTable
{
public:
    CycleTowerTable();
    ~CycleTowerTable();

    void AddCycleTowerMapCfg(int32_t MapId, CycleTowerMapCfg stu);
    void AddCycleTowerCost(int32_t Times, CycleTowerCost stu);
    int32_t GetMapId(int32_t Floor);
    const CycleTowerMapCfg* GetFloorCfg(int32_t MapId);
    ItemData GetCostItem(int32_t Times);
    int32_t GetCostGold(int32_t Times);

private:
    std::map<int, CycleTowerCost> m_CycleTowerCostMap;
    std::map<int, int> m_CycleTowerFloor;
    std::map<int, CycleTowerMapCfg> m_CycleTowerMapCfgMap;
};

#endif // _CYCLETOWERTABLE_H_
