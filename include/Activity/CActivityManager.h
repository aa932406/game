#ifndef __CACTIVITY_MANAGER_H__
#define __CACTIVITY_MANAGER_H__

#include <map>
#include <list>
#include <string>
#include <cstdint>

class Player;
class CActivity;
struct ShowIcon;
namespace Answer { class NetPacket; }

struct CrossTowerWinerInfo
{
    int64_t Winner;
    int32_t Battle;
    std::string Name;
};

class CActivityManager
{
public:
    static CActivityManager* GetInstance();
    
    CActivityManager();
    ~CActivityManager();

    // 初始化
    void Init(int32_t line);
    
    // 活动管理
    CActivity* GetActivity(int32_t nId);
    bool IsActivityRunning(int32_t activityId);
    bool IsActivityRunningByType(int32_t typeId);
    
    // 玩家相关
    void NotifyActivityInfo(Player* player);
    void SendActivityInfo(Player* player, int32_t nId);
    void SendActivityRankInfo(Player* player, int32_t nId);
    void GiveDailyReward(Player* player, int32_t nId);
    int32_t HaveRewardCount(Player* player, int32_t nId);
    
    // 活动状态
    void GetActicityIconState(Player* player, std::list<ShowIcon>& iconList);
    void AppendActivityState(Answer::NetPacket* packet, int32_t* nCount);
    void UpdateActivityState(int32_t nId, int32_t nState);
    
    // 活动查找
    int32_t GetCurActivityId(int32_t nType);
    int32_t GetCrossTowerName();
    
    // 活动操作
    void ApplyCityWar(Player* player, int32_t cityId, int64_t FamilyId, int32_t Type);
    void AddTianJiangBaoXiangCount(Player* player, int32_t AddCount);
    void StopSqiderQueen();
    
    // 更新
    void OnUpdate(int64_t curTick);
    
    // 城战相关
    void OnCityWarResult(int32_t nActId, int8_t connid, int64_t nFamilyId,
                         int32_t nWinTime, int32_t nTime, int64_t nLeaderCid,
                         int32_t First, int32_t Second, int32_t Third, int32_t nIndex,
                         const std::string& p_FirstFamilyName,
                         const std::string& p_SecondFamilyName,
                         const std::string& p_ThirdFamilyName);
    void SetCityWarWinner(int8_t connid, int64_t nFamilyId);
    
    // 无双战相关
    void OnPeerlessWarResult(int32_t nActId, int8_t connid, int64_t winner, const std::string& name);
    void SetPeerlessWarWinner(int8_t connid, int64_t nCharId);
    
    // 跨服塔相关
    void OnCrossTower(int32_t result, int64_t Winner, int32_t Battle, const std::string& p_Name, int8_t connid);
    void SetCrossTowerResult(int64_t nCharId, int32_t Battle, const std::string& p_Name);
    void SetCrossTowerWinner(int64_t nCharId, int32_t Battle, const std::string& p_Name);
    
    // 辅助方法
    int32_t GetLine();

private:
    std::map<int32_t, CActivity*> m_mActivity;
    std::map<int8_t, int64_t> m_mWinFamily;
    std::map<int8_t, int64_t> m_mPeerlessWinner;
    CrossTowerWinerInfo m_CrossTowerWiner;
};

#endif // __CACTIVITY_MANAGER_H__