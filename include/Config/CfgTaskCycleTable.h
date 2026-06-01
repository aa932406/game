#ifndef _CFGCFGTASKCYCLETABLE_H_
#define _CFGCFGTASKCYCLETABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>
#include <map>


// Task cycle config data types
struct CycleStarRate
{
    int32_t nStar;
    int32_t nRate;
};

struct TaskCycleReward
{
    int32_t nStar;
    std::vector<MemChrBag> vReward;
};

struct TaskCycleInfo
{
    int32_t nMinLevel;
    int32_t nMaxLevel;
};


/**
 * @brief CfgTaskCycleTable - Config data class
 */
class CfgTaskCycleTable
{
public:
    CfgTaskCycleTable();
    ~CfgTaskCycleTable();

    bool AddTask(int32_t nTaskId, int32_t nMinLevel, int32_t nMaxLevel);
    bool AddStarRate(CycleStarRate& StarRate);
    void AddTaskReward(const TaskCycleReward& stu);

    int32_t GetTaskIdByLevel(int32_t nLevel);
    int32_t GetStarRate(int32_t nStar);
    const TaskCycleReward* GetTaskReward(int32_t nStar);
    void Clear();

public:
    std::vector<CycleStarRate> m_vStarRate;
    std::vector<TaskCycleReward> m_vTaskReward;
    std::map<int32_t, TaskCycleInfo> m_mTaskCycle;
};

#endif // _CFGCFGTASKCYCLETABLE_H_
