#ifndef _SEVENTASKTABLE_H_
#define _SEVENTASKTABLE_H_

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
struct SevenTaskSumReward;
struct SevenTaskCfg;

class SevenTaskTable
{
public:
    SevenTaskTable();
    ~SevenTaskTable();

    void AddSevenTaskSumRewardMap(SevenTaskSumReward stu);
    void AddSevenTaskCfgMap(SevenTaskCfg stu);
    const SevenTaskSumReward* GetSevenTaskSumReward(int32_t nId);
    const SevenTaskCfg* GetSevenTaskCfg(int32_t nId);
    const std::map<int, SevenTaskCfg>& GetSevenTaskCfgMap() const;
    const std::map<int, SevenTaskSumReward>& GetSevenTaskSumRewardMap() const;

private:
    std::map<int, SevenTaskCfg> m_SevenTaskCfgMap;
    std::map<int, SevenTaskSumReward> m_SevenTaskSumRewardMap;
};

#endif // _SEVENTASKTABLE_H_
