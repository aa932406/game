#ifndef _MONEYREWARDTASKDATA_H_
#define _MONEYREWARDTASKDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MoneyRewardTaskData
{
public:
    MoneyRewardTaskData();
    ~MoneyRewardTaskData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);

private:
    int8_t  m_IsGetReward;
    int32_t m_FinishTimes;
    int32_t m_TaskId;
    int32_t m_Star;
    int32_t m_RandStarTimes;
    int32_t m_PdbfFinishTimes;
    int32_t m_EquipBackTaskId;
    std::string m_MoneyRewardTaskInfo;
    std::string m_XiangYaoTask;
    std::string m_ShenWeiTask;
};

#endif
