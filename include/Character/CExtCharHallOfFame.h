// CExtCharHallOfFame.h
#ifndef CEXTCHARHALLOFFAME_H
#define CEXTCHARHALLOFFAME_H

#include "CExtSystemBase.h"

class CExtCharHallOfFame : public CExtSystemBase
{
public:
    CExtCharHallOfFame();
    virtual ~CExtCharHallOfFame();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onFight(Answer::NetPacket* inPacket);
    int32_t onBuyTimes(Answer::NetPacket* inPacket);
    int32_t onClearCD(Answer::NetPacket* inPacket);
    int32_t onRequestInfo(Answer::NetPacket* inPacket);
    int32_t onSocialFightResult(Answer::NetPacket* inPacket);
    int32_t onSocialGetReward(Answer::NetPacket* inPacket);
    int32_t onSocialRankRewardInfo(Answer::NetPacket* inPacket);
    
    void sendSocialFight(int32_t nIndex);
    void sendSocialRequestInfo();
    void sendSocialRewardResult(int32_t nIndex, int32_t nResult);
    void addFightTimes();
    void addBuyTimes();
    void addCDTime(bool bWin);
    void clearCDTime();
    void GetIconState(void* iconList);
    void SendIconState(int32_t nReward);
    
    int32_t getFightTimes();
    int32_t getLimitFightTimes();
    int32_t getBuyTimes();
    int32_t getLimitBuyTimes();
    int32_t getCDTime();
    bool IsFunctionOpen();
    
private:
    int32_t m_nFightTimes;
    int32_t m_nBuyTimes;
    int32_t m_nCDTime;
};

#endif