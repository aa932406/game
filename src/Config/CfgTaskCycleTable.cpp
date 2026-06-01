#include "Config/CfgTaskCycleTable.h"
#include "Answer/Logger.h"
#include <algorithm>

CfgTaskCycleTable::CfgTaskCycleTable()
{
    m_mTaskCycle.clear();
    m_vStarRate.clear();
    m_vTaskReward.clear();
}

CfgTaskCycleTable::~CfgTaskCycleTable()
{
    m_mTaskCycle.clear();
    m_vStarRate.clear();
    m_vTaskReward.clear();
}

bool CfgTaskCycleTable::AddTask(int32_t nTaskId, int32_t nMinLevel, int32_t nMaxLevel)
{
    if (nTaskId <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTaskCycleTable::AddTask invalid task id: %d", nTaskId);
        return false;
    }
    
    if (nMinLevel > nMaxLevel && nMaxLevel != 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_WARN,
            "CfgTaskCycleTable::AddTask minLevel > maxLevel: %d > %d", nMinLevel, nMaxLevel);
    }
    
    TaskCycleInfo info;
    info.nMinLevel = nMinLevel;
    info.nMaxLevel = nMaxLevel;
    m_mTaskCycle[nTaskId] = info;
    return true;
}

bool CfgTaskCycleTable::AddStarRate(CycleStarRate& StarRate)
{
    if (StarRate.nRate <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTaskCycleTable::AddStarRate invalid rate: %d", StarRate.nRate);
        return false;
    }
    
    m_vStarRate.push_back(StarRate);
    return true;
}

void CfgTaskCycleTable::AddTaskReward(const TaskCycleReward& stu)
{
    if (stu.nStar <= 0)
    {
        Answer::Logger::print(Answer::LogLevel::LOG_LEVEL_ERROR,
            "CfgTaskCycleTable::AddTaskReward invalid star: %d", stu.nStar);
        return;
    }
    
    m_vTaskReward.push_back(stu);
}

int32_t CfgTaskCycleTable::GetTaskIdByLevel(int32_t nLevel)
{
    for (const auto& pair : m_mTaskCycle)
    {
        const TaskCycleInfo& info = pair.second;
        if (nLevel >= info.nMinLevel)
        {
            if (info.nMaxLevel == 0 || nLevel <= info.nMaxLevel)
            {
                return pair.first;
            }
        }
    }
    return 0;
}

int32_t CfgTaskCycleTable::GetStarRate(int32_t nStar)
{
    for (const auto& rate : m_vStarRate)
    {
        if (rate.nStar == nStar)
        {
            return rate.nRate;
        }
    }
    return 0;
}

const TaskCycleReward* CfgTaskCycleTable::GetTaskReward(int32_t nStar)
{
    for (const auto& reward : m_vTaskReward)
    {
        if (reward.nStar == nStar)
        {
            return &reward;
        }
    }
    return nullptr;
}

void CfgTaskCycleTable::Clear()
{
    m_mTaskCycle.clear();
    m_vStarRate.clear();
    m_vTaskReward.clear();
}