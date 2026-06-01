#ifndef _CHUOYUEDU_H_
#define _CHUOYUEDU_H_

#include "Character/CExtSystemBase.h"
#include <map>

class HuoYueDuRecord
{
public:
    int32_t nCount;
    int8_t IsGetReward;
};

class CHuoYueDu : public CExtSystemBase
{
public:
    CHuoYueDu();
    virtual ~CHuoYueDu();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnAskHuoYueDuInfo(Answer::NetPacket* inPacket);
    int32_t OnGetHuoYueDuReward(Answer::NetPacket* inPacket);
    int32_t OnSec(Answer::NetPacket* inPacket);
    void AddKiaFuHuoYueDu(int32_t Value);
    void AddHuoYueDuRecord(int8_t Type, int32_t Effect, bool Complete);
    void SendHuoYueDuInfo();
    int32_t CalculateHuoYueDu();
    int32_t RewardCount();
    void GetHuoYueDuIcon(IconStateList* IconList);
    void SendtHuoYueDuIcon();
    void GettHuoYueDuIcon(ShowIcon* __return_ptr retstr);

private:
    std::map<int, HuoYueDuRecord> m_HuoYueDuRecord;
};

#endif // _CHUOYUEDU_H_
