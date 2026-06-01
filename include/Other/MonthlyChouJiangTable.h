#ifndef _MONTHLYCHOUJIANGTABLE_H_
#define _MONTHLYCHOUJIANGTABLE_H_

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
struct RateItem;

class MonthlyChouJiangTable
{
public:
    MonthlyChouJiangTable();
    ~MonthlyChouJiangTable();

    void AddMonthlyChouJiangCfg(MonthlyChouJiangCfg stu);

private:
    std::map<int, MonthlyChouJiangCfg> m_MonthlyChouJiangCfgMap;
    std::map<int, std::map<int, RateItem>> m_MonthlyChouJiangItemMap;
};

#endif // _MONTHLYCHOUJIANGTABLE_H_
