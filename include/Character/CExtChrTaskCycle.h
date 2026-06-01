// CExtChrTaskCycle.h
#ifndef CEXTCHRTASKCYCLE_H
#define CEXTCHRTASKCYCLE_H

#include "CExtSystemBase.h"

class CExtChrTaskCycle : public CExtSystemBase
{
public:
    CExtChrTaskCycle();
    virtual ~CExtChrTaskCycle();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onReceiveTask(Answer::NetPacket* inPacket);
    int32_t onSubmitTask(Answer::NetPacket* inPacket);
    int32_t onQuickDone(Answer::NetPacket* inPacket);
    
    bool IsFunctionOpen();
    void OpenCycleTask();
    void sendTaskInfo();
    void refreshTask();
    int32_t getTaskCycleTimes();
    int32_t GetSurplusTimes();
    void addRandomReward();
    void sendRandomItem(const void* vItem);
    
private:
    int32_t m_nFinishTimes;
    int32_t m_nTaskId;
    int8_t m_nState;
};

#endif