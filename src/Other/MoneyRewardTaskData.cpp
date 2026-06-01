#include "Other/MoneyRewardTaskData.h"
#include "Network/NetPacket.h"

MoneyRewardTaskData::MoneyRewardTaskData()
{
    CleanUp();
}

MoneyRewardTaskData::~MoneyRewardTaskData()
{
}

void MoneyRewardTaskData::CleanUp()
{
    m_IsGetReward = 0;
    m_FinishTimes = 0;
    m_TaskId = 0;
    m_Star = 0;
    m_RandStarTimes = 0;
    m_PdbfFinishTimes = 0;
    m_EquipBackTaskId = 0;
    m_MoneyRewardTaskInfo.clear();
    m_XiangYaoTask.clear();
    m_ShenWeiTask.clear();
}

void MoneyRewardTaskData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt8(m_IsGetReward);
    packet->writeInt32(m_FinishTimes);
    packet->writeUTF8(&m_MoneyRewardTaskInfo);
    packet->writeInt32(m_TaskId);
    packet->writeInt32(m_Star);
    packet->writeInt32(m_RandStarTimes);
    packet->writeInt32(m_PdbfFinishTimes);
    packet->writeInt32(m_EquipBackTaskId);
}
