#ifndef _CDATINGREWARD_H_
#define _CDATINGREWARD_H_

#include "Character/CExtSystemBase.h"
#include <map>

class CDaTingReward : public CExtSystemBase
{
public:
    CDaTingReward();
    virtual ~CDaTingReward();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* dbData) override;
    void OnSaveToDB(PlayerDBData* dbData) override;
    void GetInterestsProtocol(ProcIdList* procList) override;
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket) override;
    int32_t OnDaTingInfo(Answer::NetPacket* inPacket);
    int32_t OnDaTingReward(Answer::NetPacket* inPacket);
    int32_t OnDaTingBuy(Answer::NetPacket* inPacket);
    int32_t OnPlatformVipLevelUp(Answer::NetPacket* inPacket);
    int32_t OnPlatformVipActReward(Answer::NetPacket* inPacket);
    void SendDaTingInfo();
    void SendPlatformVipInfo();
    void AddPlatformVipExp(int32_t nExp);

private:
    int32_t m_nDaTingLevel;
    int32_t m_nDaTingExp;
    std::map<int, int> m_DaTingRewardFlag;
    int32_t m_nPlatformVipLevel;
    int32_t m_nPlatformVipExp;
    std::map<int, int> m_PlatformVipActRewardFlag;
};

#endif // _CDATINGREWARD_H_
