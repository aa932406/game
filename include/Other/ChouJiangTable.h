#ifndef _CHOUJIANGTABLE_H_
#define _CHOUJIANGTABLE_H_

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
struct ChouJiangCfg;
struct ChouJiangLuckyCfg;
struct ChouJiangWeekReward;
struct ChouJiangCost;

class ChouJiangTable
{
public:
    ChouJiangTable();
    ~ChouJiangTable();

    void AddChouJiangCfg(ChouJiangCfg stu);
    void AddChouJiangLuckyCfg(ChouJiangLuckyCfg stu);
    void AddChouJiangWeekReward(ChouJiangWeekReward *p_stu);
    void AddChouJiangCost(ChouJiangCost stu);
    const ChouJiangLuckyCfg* GetChouJiangLuckyCfg(int32_t nIdex);
    const ChouJiangCost* GetChouJiangCost(int32_t nIdex);

private:
    std::map<int, ChouJiangCfg> m_ChouJiangCfgMap;
    std::map<int, ChouJiangCost> m_ChouJiangCostMap;
    std::map<int, ChouJiangLuckyCfg> m_ChouJiangLuckyCfgMap;
    std::map<int, ChouJiangWeekReward> m_ChouJiangWeekRewardMap;
};

#endif // _CHOUJIANGTABLE_H_
