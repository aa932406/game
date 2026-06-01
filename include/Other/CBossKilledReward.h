#ifndef _CBOSSKILLEDREWARD_H_
#define _CBOSSKILLEDREWARD_H_

#include "Character/CExtSystemBase.h"
#include <map>

class BossKilledRewardInfo;

class CBossKilledReward : public CExtSystemBase
{
public:
    CBossKilledReward();
    virtual ~CBossKilledReward();

    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    void OnLoadFromDB(const PlayerDBData* const dbData) override;
    void OnSaveToDB(PlayerDBData* const dbData) override;
    void GetInterestsProtocol(ProcIdList* const procList) override;
    int32_t OnGetBossKilledReward(Answer::NetPacket* inPacket);
    void AddBossKilled(int32_t BossId);
    void SendBossKilledInfo();
    void GetBossKilledIcon(IconStateList* const IconList);
    void SendBossKilledIcon();
    bool IsAllRewardGet();
    int32_t CanGetRewardCount();
    void GetBossKilledIcon(ShowIcon* __return_ptr retstr);

private:
    std::map<signed char, BossKilledRewardInfo> m_BossKilledRewardMap;
};

#endif // _CBOSSKILLEDREWARD_H_


