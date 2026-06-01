#ifndef _CMINGGETABLE_H_
#define _CMINGGETABLE_H_

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
class CMingGeCfg;
struct MingGeDrawCost;
struct MingGeDrawRate;
class Param2;

class CMingGeTable
{
public:
    CMingGeTable();
    ~CMingGeTable();

    const CMingGeCfg* GetCMingGeCfg(int32_t nId);
    void AddMingGeCfg(CMingGeCfg *p_stu);
    void AddMingGeDrawCost(MingGeDrawCost stu);
    void AddMingGeDrawRate(int32_t nType, Param2 ItemRate);
    const MingGeDrawCost* GetMingGeDrawCost(int32_t nType);
    int32_t RandMingGe(int32_t nType);

private:
    std::map<int, CMingGeCfg> m_CMingGeCfgMap;
    std::map<int, MingGeDrawCost> m_MingGeDrawCostMap;
    std::map<int, MingGeDrawRate> m_MingGeDrawRateMap;
};

#endif // _CMINGGETABLE_H_
