#ifndef _CMONEYREWARDTASK_H_
#define _CMONEYREWARDTASK_H_

#include "Character/CExtSystemBase.h"
#include <map>
#include <list>

class CMoneyRewardTask : public CExtSystemBase
{
public:
    CMoneyRewardTask();
    virtual ~CMoneyRewardTask();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnTaskInfo(Answer::NetPacket* inPacket);
    int32_t OnTaskReward(Answer::NetPacket* inPacket);
    int32_t OnPdbfInfo(Answer::NetPacket* inPacket);
    int32_t OnEquipBackInfo(Answer::NetPacket* inPacket);
    int32_t OnTrailerInfo(Answer::NetPacket* inPacket);
    int32_t OnGetReward(Answer::NetPacket* inPacket);
    int32_t OnGetActReward(Answer::NetPacket* inPacket);
    void SendTaskInfo();
    void SendPdbfInfo();
    void SendEquipBackInfo();
    void SendTrailerInfo();
    void AddTaskProgress(int32_t nTaskId, int32_t nAdd);
    bool IsTaskCompleted(int32_t nTaskId);
    void CheckTaskComplete();

private:
    std::map<int, int> m_TaskProgress;
    std::map<int, int> m_TaskRewardFlag;
    std::list<int> m_FinishedTaskList;
    int32_t m_nPdbfLevel;
    int32_t m_nPdbfExp;
    int32_t m_nEquipBackLevel;
    int32_t m_nTrailerLevel;
};

#endif // _CMONEYREWARDTASK_H_
