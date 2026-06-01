#ifndef _CMONTHLYCHOUJIANG_H_
#define _CMONTHLYCHOUJIANG_H_

#include "Character/CExtSystemBase.h"
#include <list>
#include <map>

class CMonthlyChouJiang : public CExtSystemBase
{
public:
    CMonthlyChouJiang();
    virtual ~CMonthlyChouJiang();

    void OnLoadFromDB(const PlayerDBData* const dbData) override;
    void OnSaveToDB(PlayerDBData* const dbData) override;
    void GetInterestsProtocol(ProcIdList* const procList) override;
    void OnCleanUp() override;
    void OnUpdate(int64_t curTick) override;
    void OnDaySwitch(int32_t nDiffDays) override;
    int32_t GetKaiFuMonth();
    int32_t OnMonthlyChouJiang(Answer::NetPacket* inPacket);
    void OnMonthlySwitch();
    void OnChongZhi(int32_t NewValues, int32_t OldValues);
    void SendMonthlyChouJiangInfo();
    void SendMonthlyChouJiangResult(int32_t nId);
    int32_t GetSocreTimes(int32_t nId);
    void GetIcon(IconStateList* const IconList);
    void SendIcon();
    void GetIcon(ShowIcon* __return_ptr retstr);
    bool TodaySocreIsAllGet();

private:
    int32_t m_Score;
    int32_t m_AllScore;
    std::list<int> m_GetItemList;
    std::map<int, int> m_GetTimes;
};

#endif // _CMONTHLYCHOUJIANG_H_
