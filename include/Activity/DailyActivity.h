#ifndef __DAILY_ACTIVITY_H__
#define __DAILY_ACTIVITY_H__

#include "Character/CExtSystemBase.h"
#include "Activity/DailyActivityData.h"
#include <list>
#include <vector>
#include <string>
#include <map>
#include <cstdint>

class Player;
namespace Answer { class NetPacket; }

struct SearchBackInfo
{
    int8_t nType;
    int8_t nSubType;
    int32_t nTimes;
    int32_t nParam;
    
    SearchBackInfo() : nType(0), nSubType(0), nTimes(0), nParam(0) {}
};

struct SearchBackRecord
{
    int32_t nId;
    int32_t nTimes;
    int32_t nParam;
    
    SearchBackRecord() : nId(0), nTimes(0), nParam(0) {}
};

struct SignInfo
{
    int32_t sign_record;
    std::vector<int8_t> sign_reward;
    int32_t refresh_time;
    
    SignInfo() : sign_record(0), refresh_time(0) {}
};

class DailyActivity : public CExtSystemBase
{
public:
    DailyActivity();
    virtual ~DailyActivity();

    // 初始化与清理
    virtual void OnCleanUp();
    virtual void OnLoadFromDB(const DailyActivityDBData* dbData);
    virtual void OnSaveToDB(DailyActivityDBData* dbData);
    virtual void OnUpdate(int64_t curTick);
    
    // 网络消息处理
    virtual void DispatchNetDatas(Answer::NetPacket* inPacket, uint16_t nProcId);
    virtual void GetInterestsProtocol(std::list<uint16_t>& procList);
    
    // 记录添加
    void AddActivityRecord(int32_t nType, int32_t nId);
    void AddDungeonRecord(int32_t nType, int32_t nTimes, int32_t nParam);
    void AddTaskRecord(int32_t nType);
    
    // 签到相关
    void OnSign(Answer::NetPacket* inPacket);
    void OnGetSignReward(Answer::NetPacket* inPacket);
    void OnGetSignDailyReward(Answer::NetPacket* inPacket);
    void OnQuerySignInfo(Answer::NetPacket* inPacket);
    void SendSignInfo();
    void RefreshSignInfo();
    bool TodayIsSign();
    bool HaveSignReward();
    
    // 在线奖励相关
    void OnGetOnLineReward(Answer::NetPacket* inPacket);
    void OnQueryOnLineInfo(Answer::NetPacket* inPacket);
    void OnGetWeekOnLineReward(Answer::NetPacket* inPacket);
    void SendOnlineRewardInfo();
    void RefreshWeekTime();
    int32_t GetTodayOnLineTime();
    int32_t GetWeekOnlineTime();
    int32_t GetLastWeekOnlineTime();
    bool HaveOnLineReward();
    bool HaveWeekReward();
    
    // 离线经验相关
    void OnGetOffLineExp(Answer::NetPacket* inPacket);
    void OnQueryOffLineExpInfo(Answer::NetPacket* inPacket);
    void SendOffLineInfo();
    bool HaveOffLineReard();
    
    // 七日登录相关
    void OnGetSevenLoginRewrad(Answer::NetPacket* inPacket);
    void OnQuerySevenLoginInfo(Answer::NetPacket* inPacket);
    void SendSevenLoginInfo();
    void GetSevenDayLoginIcon(std::list<ShowIcon>& IconList);
    void SendSevenDayLoginIcon();
    bool HaveSevenLoginReward();
    
    // 找回相关
    void OnGetSearchBackReward(Answer::NetPacket* inPacket);
    void SendSearchBackInfo();
    void GetSearchBackIcon(std::list<ShowIcon>& IconList);
    void SendSearchBackIcon();
    void RefreshSearchBack(int32_t nDiffDays);
    void UpdateSearchBackRecord(int32_t nId, int32_t Times);
    bool GetSearchReward(const SearchBackRecord& record, int8_t nFree, int32_t Times);
    SearchBackRecord GetSearchBackRecord(int32_t nId);
    void GetTodaySearchBackRecord(std::list<SearchBackRecord>* lst, int32_t type);
    bool HaveSearchBackReward();
    
    // 等级礼包相关
    void OnGetLevelGift(Answer::NetPacket* inPacket);
    void OnQueryLevelGiftInfo(Answer::NetPacket* inPacket);
    void SendLevelGiftInfo();
    void OnLevelUp(int32_t newLevel);
    bool HaveLevelReward();
    
    // 维护补偿相关
    void OnGetMaintainCompensateReward(Answer::NetPacket* inPacket);
    bool HaveMaintainCompensateReward();
    
    // 称号相关
    void OnGetDogTitle(Answer::NetPacket* inPacket);
    void OnAskDogTitleInfo(Answer::NetPacket* inPacket);
    
    // 活动大厅相关
    void OnQueryHuoDaoDaTingData(Answer::NetPacket* inPacket);
    void SendHuoDaoDaTingData(int8_t Type);
    void GetHuoDongDaTingIcon(std::list<ShowIcon>& IconList);
    void SendHuoDongDaTingIcon();
    
    // 奖励大厅相关
    void GetJiangLiDaTingIcon(std::list<ShowIcon>& IconList);
    void SendJiangLiDaTingIcon();
    void UpDateJiangLiDatingIcon(int64_t curTick);
    
    // 其他
    void RecordEnterNumber(Answer::NetPacket* inPacket);
    void OnDaySwitch();
    int32_t GetNewSeverWeek();
    int32_t HaveActivityRewardCount();
    bool HaveEquipShow();
    void PasOnlineReward(const std::string& p_OnlineReward);
    void SetPlayer(Player* player);
    
protected:
    void GetHuoDongDaTingIcon(ShowIcon* icon);
    void GetJiangLiDaTingIcon(ShowIcon* icon);
    void GetSearchBackIcon(ShowIcon* icon);
    void GetSevenDayLoginIcon(ShowIcon* icon);
    std::string GetOnlineRewardString();

private:
    SignInfo m_signInfo;
    std::list<SearchBackInfo> m_lstTodayInfo;
    std::list<SearchBackRecord> m_vSeachBackRecord[3];
    std::map<int32_t, int32_t> m_OnlimeTimeGetReward;
    int64_t m_LastUpdate;
    int64_t m_LastUpdateIcon;
    int8_t m_IsLogin;
    Player* m_pPlayer;
};

#endif // __DAILY_ACTIVITY_H__