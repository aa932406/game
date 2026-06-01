#ifndef __CFESTIVAL_ACTIVITY_H__
#define __CFESTIVAL_ACTIVITY_H__

#include <list>
#include <vector>
#include <string>
#include <cstdint>

#include "Config/CfgData.h"

class Player;
struct ShowIcon;

// 节日活动专用枚举 - 注意与 Common/CommonTypes.h 中的 FESTIVAL_ACTIVITY_TYPE_2 是不同枚举
enum class FESTIVAL_CTRL_ACTIVITY_TYPE : int32_t
{
    FAT2_MAIN = 0,
    FAT2_DA_TI = 1,
    FAT2_DUI_HUAN = 2,
    FAT2_COLLECTION = 3,
    FAT2_COLLECTION2 = 4
};

struct ChangeItemCfg
{
    int32_t Times;
    int32_t nCostGold;
    std::vector<MemChrBag> GetItems;
    std::vector<MemChrBag> lCostItem;
};

// 节日活动位置结构 - 区别于 Common/CommonTypes.h 的 MapPos(x,y)
struct FestivalMapPos
{
    int32_t m_MapId;
    struct Position
    {
        int32_t x;
        int32_t y;
    } m_Pos;
};

class CFestivalActivity
{
public:
    CFestivalActivity();
    ~CFestivalActivity();

    // 初始化
    void Init(int32_t activityId);
    void InitCfgData();
    void InitOpenList(const std::string& str);
    
    // 时间相关
    void OnDaySwitch();
    void OnNewMinute(int32_t nMinute);
    int32_t GetLeftTime();
    bool IsInTime();
    bool IsInTime(FESTIVAL_CTRL_ACTIVITY_TYPE nType);
    bool IsOpen(FESTIVAL_CTRL_ACTIVITY_TYPE nType);
    int32_t GetActDay(FESTIVAL_CTRL_ACTIVITY_TYPE nType);
    
    // 答题
    int32_t DaTi(Player* player, const std::string& answerId);
    int32_t GetDaTiReward(Player* player, int32_t RewardType);
    
    // 兑换
    int32_t DuiHuan(Player* player, int32_t exchangeId);
    
    // 采集
    int32_t GetPlantTimes();
    int32_t GetPlantTimes2();
    
    // 图标
    void GetIconState(Player* player, std::list<ShowIcon>& IconList);
    void SendIconState(Player* player);
    void HideIcon(int32_t nIconId);
    bool NeedShowIcon();
    
    // 版本管理
    int32_t LoadVersion();
    void SaveVersion();

protected:
    void getIconState(ShowIcon* stu, Player* player);
    bool HasReward(Player* player);

private:
    int32_t m_nDay;
    int32_t m_nMinute;
    int32_t m_nStartDay;
    int32_t m_nEndDay;
    int32_t m_nIcon;
    int32_t m_nVersion;
    int8_t m_vOpen[6];
    int32_t m_vStartDay[5];
    int32_t m_vEndDay[5];
    
    // 植物相关
    int32_t m_PlantId;
    int32_t m_PlantCount;
    std::vector<FestivalMapPos*> m_PlantMapPosVt;
    
    // NPC相关
    int32_t m_NpcId;
    int32_t m_NpcCount;
    std::vector<FestivalMapPos*> m_NpcMapPosVt;
    
    // 字符串向量
    std::vector<std::vector<std::string>*> m_StringVtVector;
    
    // 兑换配置
    std::vector<ChangeItemCfg*> m_ChangeItemCfgVt;
    
    // 奖励
    MemChrBag m_DayGift;
    MemChrBag m_ActGift;
    int32_t m_Times;
    int32_t m_PlantTime;
};

#endif // __CFESTIVAL_ACTIVITY_H__