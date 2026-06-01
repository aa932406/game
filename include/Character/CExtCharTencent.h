// CExtCharTencent.h
#ifndef CEXTCHARTENCENT_H
#define CEXTCHARTENCENT_H

#include "CExtSystemBase.h"
#include <string>

struct TencentInfo
{
    std::string m_pf;
    int8_t m_bYellowVip;
    int8_t m_bYellowYearVip;
    int8_t m_bYellowHighVip;
    int8_t m_nYellowVipLevel;
    int8_t m_bBlueVip;
    int8_t m_bBlueYearVip;
    int8_t m_bBlueHighVip;
    int8_t m_nBlueVipLevel;
};

class CExtCharTencent : public CExtSystemBase
{
public:
    CExtCharTencent();
    virtual ~CExtCharTencent();
    
    virtual void OnCleanUp() override;
    virtual void OnUpdate(int64_t curTick) override;
    virtual void OnDaySwitch(int32_t nDiffDays) override;
    virtual void OnLoadFromDB(const PlayerDBData* dbData) override;
    virtual void OnSaveToDB(PlayerDBData* dbData) override;
    virtual void GetInterestsProtocol(ProcIdList* procList) override;
    
    int32_t onRequestYellowStoneInfo(Answer::NetPacket* inPacket);
    int32_t onGetYellowNewerAward(Answer::NetPacket* inPacket);
    int32_t onGetYellowDailyAward(Answer::NetPacket* inPacket);
    int32_t onGetYellowYearAward(Answer::NetPacket* inPacket);
    int32_t onGetYellowLevelAward(Answer::NetPacket* inPacket);
    int32_t onGetQZoneGift(Answer::NetPacket* inPacket);
    int32_t onGetQQGameGift(Answer::NetPacket* inPacket);
    int32_t onRequestBlueStoneInfo(Answer::NetPacket* inPacket);
    int32_t onGetBlueAward(Answer::NetPacket* inPacket);
    int32_t onGetBlueDailyAward(Answer::NetPacket* inPacket);
    int32_t onGetBlueYearAward(Answer::NetPacket* inPacket);
    int32_t onGetBlueHighAward(Answer::NetPacket* inPacket);
    int32_t onGetBlueLevelAward(Answer::NetPacket* inPacket);
    int32_t onReloadTencentInfo(Answer::NetPacket* inPacket);
    int32_t onRequestSevenDayLoginInfo(Answer::NetPacket* inPacket);
    int32_t onGetSevenDayLoginReward(Answer::NetPacket* inPacket);
    int32_t onRequestTGPInfo(Answer::NetPacket* inPacket);
    int32_t onGetTGPAward(Answer::NetPacket* inPacket);
    int32_t onGetTGPDailyAward(Answer::NetPacket* inPacket);
    int32_t onGetTGPLevelAward(Answer::NetPacket* inPacket);
    
    void GetYellowStoneIcon(void* IconList);
    void SendYellowStoneIcon();
    void GetQZoneIcon(void* IconList);
    void SendQZoneIcon();
    void GetQQGameIcon(void* IconList);
    void SendQQGameIcon();
    void GetYellowNewerIcon(void* IconList);
    void SendYellowNewerIcon();
    void GetBlueStoneIcon(void* IconList);
    void SendBlueStoneIcon();
    void GetBluePayIcon(void* IconList);
    void SendBluePayIcon();
    void GetSevenDayLoginIcon(void* IconList);
    void SendSevenDayLoginIcon();
    void GetTGPIcon(void* IconList);
    void SendTGPIcon();
    void GetFriendIcon(void* IconList);
    
    void AppendInfo(Answer::NetPacket* packet);
    void InitTencentInfo(const TencentInfo* info);
    void UpdateTencentInfo(const TencentInfo* info);
    
    int32_t canGetYellowReward();
    int32_t canGetBlueReward();
    bool needShowBluePayIcon();
    bool isInSevenDay();
    bool needShowSevenDayIcon();
    int32_t getLoginDay();
    
    bool IsYellowVip();
    int32_t GetYellowVipLevel();
    bool IsYellowYearVip();
    bool IsBlueVip();
    int32_t GetBlueVipLevel();
    bool IsBlueYearVip();
    bool IsBlueHighVip();
    bool isYellowEnter();
    bool isBlueEnter();
    bool isTGPEnter();
    
private:
    void resetQZoneDailyReward();
    void resetQQGameDailyReward();
    bool checkQQGiftCondition(int8_t nType, int32_t nCondition);
    
    TencentInfo m_info;
};

#endif