// CExtChrTaskCycle.cpp
#include "CExtChrTaskCycle.h"
#include <cstring>

CExtChrTaskCycle::CExtChrTaskCycle()
{
    OnCleanUp();
}

CExtChrTaskCycle::~CExtChrTaskCycle()
{
}

void CExtChrTaskCycle::OnCleanUp()
{
    m_nFinishTimes = 0;
    m_nTaskId = 0;
    m_nState = 0;
}

void CExtChrTaskCycle::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (IsFunctionOpen() && m_pPlayer && dbData)
    {
        m_nFinishTimes = dbData->taskCycleData.nFinishTimes;
        m_nTaskId = dbData->taskCycleData.nTaskId;
        m_nState = dbData->taskCycleData.nState;
    }
}

void CExtChrTaskCycle::OnSaveToDB(PlayerDBData* dbData)
{
    if (IsFunctionOpen() && dbData)
    {
        dbData->taskCycleData.nFinishTimes = m_nFinishTimes;
        dbData->taskCycleData.nTaskId = m_nTaskId;
        dbData->taskCycleData.nState = m_nState;
    }
}

void CExtChrTaskCycle::OnDaySwitch(int32_t nDiffDays)
{
    if (m_pPlayer && IsFunctionOpen())
    {
        refreshTask();
        m_nFinishTimes = 0;
        sendTaskInfo();
    }
}

void CExtChrTaskCycle::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 772;
    procList->push_back(v3);
}

int32_t CExtChrTaskCycle::onReceiveTask(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtChrTaskCycle::onSubmitTask(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtChrTaskCycle::onQuickDone(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

bool CExtChrTaskCycle::IsFunctionOpen()
{
    return true;
}

void CExtChrTaskCycle::OpenCycleTask()
{
    if (!m_pPlayer) return;
    refreshTask();
    sendTaskInfo();
}

void CExtChrTaskCycle::sendTaskInfo()
{
    if (!m_pPlayer) return;
}

void CExtChrTaskCycle::refreshTask()
{
    if (!m_pPlayer) return;
}

int32_t CExtChrTaskCycle::getTaskCycleTimes()
{
    return 10;
}

int32_t CExtChrTaskCycle::GetSurplusTimes()
{
    return getTaskCycleTimes() - m_nFinishTimes;
}

void CExtChrTaskCycle::addRandomReward()
{
    if (!m_pPlayer) return;
}

void CExtChrTaskCycle::sendRandomItem(const void* vItem)
{
    if (!m_pPlayer) return;
}

void CExtChrTaskCycle::OnUpdate(int64_t curTick)
{
}