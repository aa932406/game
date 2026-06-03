#include "common.h"
#include "Other/CFestivalDoubleEleven.h"

#include <cstring>
#include <algorithm>
#include <sstream>

CFestivalDoubleEleven::CFestivalDoubleEleven()
{
    m_nDay = -1;
    m_nMinute = 0;
    Answer::Mutex::Mutex(&m_lock);
    m_bDie = 1;
    m_MonsterId = 0;
    m_nVersion = 0;
    m_nStartTime = 0;
    m_nEndTime = 0;
    m_nStartDay = 0;
    m_nEndDay = 0;
    m_nIcon = 0;
    m_nIconHideDay = 0;
    m_nDropGroup = 0;
    m_nBossScoreRate = 100;
    m_nWishNeedRecharge = 0;
    m_ChouJiangType = 0;
    m_ChouJiangCount = 0;
    m_nRechargeBackSize = 0;
    memset(m_vStartDay, 0, sizeof(m_vStartDay));
    memset(m_vEndDay, 0, sizeof(m_vEndDay));
    memset(&m_sMysteryShopRefreshCost, 0, sizeof(m_sMysteryShopRefreshCost));
}

CFestivalDoubleEleven::~CFestivalDoubleEleven()
{
    std::vector<LianRechargeCfg>::~vector(&m_LianRechargeCfgVt);
    std::vector<MemChrBag>::~vector(&m_vGouWuCheItem);
    std::vector<int>::~vector(&m_vGouWuCheGiftPrice);
    std::vector<RateItem>::~vector(&m_vBossScoreDrawReward);
    std::vector<int>::~vector(&m_nDailyRechargeDrawLimit);
    std::vector<int>::~vector(&m_vScoreLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vBossScoreReward);
    std::vector<int>::~vector(&m_vGetBossScoreMail);
    std::vector<int>::~vector(&m_vBossScoreiLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vBossScoreRankReward);
    std::vector<int>::~vector(&m_vBossScoreRankMail);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vFriendQingYiReward);
    std::vector<int>::~vector(&m_vFriendQingYiLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vEquipQingYiReward);
    std::vector<int>::~vector(&m_vEquipQingYiLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vCrossChouJiangRankReward);
    std::vector<int>::~vector(&m_vCrossChouJiangRankMail);
    std::vector<int>::~vector(&m_vCrossChouJiangRankLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vCrossXiaoFeiSumRankReward);
    std::vector<int>::~vector(&m_vCrossXiaoFeiSumRankMail);
    std::vector<int>::~vector(&m_vCrossXiaoFeiSumRankLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vCrossRechargeSumRankReward);
    std::vector<int>::~vector(&m_vCrossRechargeSumRankMail);
    std::vector<int>::~vector(&m_vCrossRechargeSumRankLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vRechargeSumRankReward);
    std::vector<int>::~vector(&m_vRechargeSumRankMail);
    std::vector<int>::~vector(&m_vRechargeSumRankLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vPetIllusionItemReward);
    std::vector<int>::~vector(&m_vPetIllusionItemLimit);
    std::vector<int>::~vector(&m_vRechargeBackValue);
    std::vector<int>::~vector(&m_vRechargeBackLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vBuyGiftItem);
    std::vector<int>::~vector(&m_vBuyGiftBroadcast);
    std::vector<int>::~vector(&m_vBuyGiftPrice);
    std::vector<int>::~vector(&m_nRechargeDrawValue);
    std::map<int,int>::~map(&m_mEquipUpStarBackStarCount);
    std::vector<ShopGoods>::~vector(&m_vDailyLimitShop2Goods);
    std::vector<int>::~vector(&m_vDailyLimitShop2Broadcast);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vDaTiRankReward);
    std::vector<int>::~vector(&m_vDaTiRankMail);
    std::vector<MemChrBag>::~vector(&m_Items);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vXiaoFeiSumReward);
    std::vector<int>::~vector(&m_vXiaoFeiSumBroadcast);
    std::vector<int>::~vector(&m_vXiaoFeiSumLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vRechargeSumReward);
    std::vector<int>::~vector(&m_vRechargeSumBroadcast);
    std::vector<int>::~vector(&m_vRechargeSumLimit);
    std::vector<MemChrBag>::~vector(&m_BackItem);
    std::vector<int>::~vector(&m_NeedValue);
    std::vector<std::vector<ShopGoods>>::~vector(&m_vGiftShopGoods);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vXiaoFeiSumRankReward);
    std::vector<int>::~vector(&m_vXiaoFeiSumRankMail);
    std::vector<int>::~vector(&m_vDailyRechargeBroad);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vDailyRechargeReward);
    std::vector<int>::~vector(&m_vDailyRechargeLimit);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vMoYuShiJieDrop);
    std::vector<int>::~vector(&m_nXiaoFeiDrawValue);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vXiaoFeiRankReward);
    std::vector<int>::~vector(&m_vXiaoFeiRankMail);
    std::vector<int>::~vector(&m_vXiaoFeiRankLimit);
    std::vector<MemChrBag>::~vector(&m_vWishReward);
    std::vector<MemChrBag>::~vector(&m_vOnlineReward);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vLandSumGift);
    std::vector<MemChrBag>::~vector(&m_vDailyLimitShopItem);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vHuoYueDuSumReward);
    std::vector<int>::~vector(&m_vHuoYueDuSumLimit);
    std::vector<TimeArea>::~vector(&m_vWorldBossMinute);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vDrawRankReward);
    std::vector<int>::~vector(&m_vDrawRankMail);
    std::vector<MemChrBag>::~vector(&m_vDrawLoopReward);
    std::vector<std::vector<MemChrBag>>::~vector(&m_vLandGift);
    std::map<signed char,std::vector<FestivalRank>>::~map(&m_mBossScoreRank);
    std::map<signed char,std::vector<FestivalRank>>::~map(&m_mCrossChouJiangRank);
    std::map<signed char,std::vector<FestivalRank>>::~map(&m_mCrossRechargeSumRank);
    std::map<signed char,std::vector<FestivalRank>>::~map(&m_mCrossXiaoFeiSumRank);
    std::map<signed char,std::vector<FestivalRank>>::~map(&m_mRechargeSumRank);
    std::map<signed char,std::vector<FestivalRank>>::~map(&m_mXiaoFeiSumRank);
    std::map<signed char,std::vector<std::vector<FestivalRank>>>::~map(&m_mXiaoFeiRank);
    std::map<signed char,std::vector<FestivalRank>>::~map(&m_mDrawRank);
    Answer::Mutex::~Mutex(&m_lock);
}

void CFestivalDoubleEleven::OnDaySwitch()
{
    CTimer* v1 = Answer::Singleton<CTimer>::instance();
    int nowTime = CTimer::GetNow(v1);
    int v2 = Answer::DayTime::dayzero(nowTime);
    int nOldDay = m_nDay;
    m_nDay = (v2 - Answer::DayTime::dayzero(m_nStartDay)) / 86400;

    if (m_nDay == m_nIconHideDay)
        hideIcon(m_nIcon);

    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_MO_YU_SHI_JIE)
        && (m_nDay == m_vStartDay[14] || m_nDay == m_vEndDay[14]))
    {
        BroadCastMoYuShiJieIconState();
    }

    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_DAILY_RECHARGE)
        && m_nDay == m_vEndDay[2])
    {
        hideIcon(52);
    }

    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_DRAW_RANK) && m_nDay == m_vEndDay[4])
        sendDrawRankMail();

    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_XIAO_FEI_SUM_RANK)
        && m_nDay == m_vEndDay[16])
    {
        sendXiaoFeiSumRankMail();
    }

    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_DAILY_XIAO_FEI_RANK))
    {
        if (nOldDay != m_nDay && m_nDay >= m_vStartDay[3] && m_nDay < m_vEndDay[3])
        {
            // 新的一天，重置每日消费排行�?
            int8_t connid = 0;
            auto it = m_mXiaoFeiRank.find(connid);
            if (it != m_mXiaoFeiRank.end())
            {
                int nDayIndex = m_nDay - m_vStartDay[3];
                if (nDayIndex >= 0 && nDayIndex < (int)it->second.size())
                {
                    it->second[nDayIndex].clear();
                }
            }
        }
        sendXiaoFeiRankMail();
    }

    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_RECHARGE_SUM_RANK)
        && m_nDay == m_vEndDay[32])
    {
        sendRechargeSumRankMail();
    }

    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_BOSS_SCORE_RANK)
        && m_nDay == m_vEndDay[43])
    {
        sendBossScoreRankMail();
    }
    
    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_RECHARGE_SUM_RANK)
        && m_nDay == m_vEndDay[33])
    {
        // 发送跨服累计充值排行榜奖励
        sendCrossRechargeSumRankMail();
    }
    
    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_XIAO_FEI_SUM_RANK)
        && m_nDay == m_vEndDay[34])
    {
        // 发送跨服累计消费排行榜奖励
        sendCrossXiaoFeiSumRankMail();
    }
    
    if (isOpen(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_CHOU_JIANG_RANK)
        && m_nDay == m_vEndDay[35])
    {
        // 发送跨服抽奖排行榜奖励
        sendCrossChouJiangRankMail();
    }
}

void CFestivalDoubleEleven::OnNewMinute(int32_t nMinute)
{
    m_nMinute = nMinute;
    HotUpdate();
    if (IsInTime())
    {
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_DRAW_RANK))
            checkDrawRank();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_WORLD_BOSS))
            checkWorldBoss();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_DAILY_XIAO_FEI_RANK))
            checkXiaoFeiRank();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_XIAO_FEI_SUM_RANK))
            checkXiaoFeiSumRank();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_MYSTERY_SHOP))
            checkRefreshMysteryShop();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_RECHARGE_SUM_RANK))
            checkRechargeSumRank();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_RECHARGE_SUM_RANK))
            checkCrossRechargeSumRank();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_XIAO_FEI_SUM_RANK))
            checkCrossXiaoFeiSumRank();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_CHOU_JIANG_RANK))
            checkCrossChouJiangRank();
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_BOSS_SCORE_RANK))
            checkBossScoreRank();
    }
}

void CFestivalDoubleEleven::Init(int32_t line)
{
    initCfgData();
    int32_t nVersion = loadVersion(line);
    if (nVersion <= 0 || m_nVersion == nVersion)
    {
        loadRankData(line);
    }
    else
    {
        resetRankData(line);
        resetPlayerData();
        m_nVersion = nVersion;
        saveVersion(line);
        loadRankData(line);
    }
}

void CFestivalDoubleEleven::initOpenList(const std::string *const str)
{
    if (!str || str->empty()) return;
    
    std::vector<std::string> vOpen;
    std::string delims = "|";
    
    std::string temp = *str;
    size_t pos = 0;
    std::string token;
    while ((pos = temp.find(delims)) != std::string::npos)
    {
        token = temp.substr(0, pos);
        if (!token.empty())
        {
            vOpen.push_back(token);
        }
        temp.erase(0, pos + delims.length());
    }
    if (!temp.empty())
    {
        vOpen.push_back(temp);
    }
    
    m_setOpenList.clear();
    for (auto& typeStr : vOpen)
    {
        int8_t nType = (int8_t)atoi(typeStr.c_str());
        m_setOpenList.insert(nType);
    }
}

void CFestivalDoubleEleven::HotUpdate()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    char szCol[64];
    Answer::Inifile ini;
    std::string file = "config/double_eleven.ini";
    std::string key;
    std::string section;
    
    if (Answer::Inifile::Load(&ini, file.c_str()))
    {
        // 重新加载配置
        int nOldStartDay = m_nStartDay;
        int nOldEndDay = m_nEndDay;
        
        initCfgData();
        
        // 如果活动时间发生变化，重新计算当前天�?
        if (nOldStartDay != m_nStartDay || nOldEndDay != m_nEndDay)
        {
            CTimer* timer = Answer::Singleton<CTimer>::instance();
            int nowTime = CTimer::GetNow(timer);
            int dayzero = Answer::DayTime::dayzero(nowTime);
            m_nDay = (dayzero - Answer::DayTime::dayzero(m_nStartDay)) / 86400;
        }
        
        if (IsInTime())
        {
            // 刷新所有排行榜数据
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_DRAW_RANK))
                loadDrawRank();
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_DAILY_XIAO_FEI_RANK))
                loadXiaoFeiRank();
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_XIAO_FEI_SUM_RANK))
                loadXiaoFeiSumRank();
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_RECHARGE_SUM_RANK))
                loadRechargeSumRank();
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_RECHARGE_SUM_RANK))
                loadCrossRechargeSumRank();
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_XIAO_FEI_SUM_RANK))
                loadCrossXiaoFeiSumRank();
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_CHOU_JIANG_RANK))
                loadCrossChouJiangRank();
            if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_BOSS_SCORE_RANK))
                loadBossScoreRank();
        }
    }
}

static void ParseItemString(const std::string& str, std::vector<MemChrBag>& items)
{
    if (str.empty()) return;
    
    // 格式: itemId,count|itemId,count
    std::stringstream ss(str);
    std::string itemStr;
    while (std::getline(ss, itemStr, '|'))
    {
        if (itemStr.empty()) continue;
        size_t commaPos = itemStr.find(',');
        if (commaPos != std::string::npos)
        {
            MemChrBag item;
            item.nItemId = atoi(itemStr.substr(0, commaPos).c_str());
            item.nCount = atoi(itemStr.substr(commaPos + 1).c_str());
            items.push_back(item);
        }
    }
}

static void ParseRewardString(const std::string& str, std::vector<std::vector<MemChrBag>>& rewards)
{
    if (str.empty()) return;
    
    // 格式: rank1_reward|rank2_reward|... 每个reward格式: itemId,count:itemId,count
    std::stringstream ss(str);
    std::string rankStr;
    while (std::getline(ss, rankStr, '|'))
    {
        if (rankStr.empty()) continue;
        std::vector<MemChrBag> items;
        std::stringstream ss2(rankStr);
        std::string itemStr;
        while (std::getline(ss2, itemStr, ':'))
        {
            if (itemStr.empty()) continue;
            size_t commaPos = itemStr.find(',');
            if (commaPos != std::string::npos)
            {
                MemChrBag item;
                item.nItemId = atoi(itemStr.substr(0, commaPos).c_str());
                item.nCount = atoi(itemStr.substr(commaPos + 1).c_str());
                items.push_back(item);
            }
        }
        rewards.push_back(items);
    }
}

static void ParseIntVector(const std::string& str, std::vector<int>& vec)
{
    if (str.empty()) return;
    
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, '|'))
    {
        if (!token.empty())
        {
            vec.push_back(atoi(token.c_str()));
        }
    }
}

void CFestivalDoubleEleven::initCfgData()
{
    Answer::Inifile ini;
    std::string file = "config/double_eleven.ini";
    
    if (!Answer::Inifile::Load(&ini, file.c_str()))
        return;
    
    // 初始化基础配置
    m_nStartDay = Answer::Inifile::GetInt(&ini, "base", "start_day", 0);
    m_nEndDay = Answer::Inifile::GetInt(&ini, "base", "end_day", 0);
    m_nStartTime = Answer::Inifile::GetInt(&ini, "base", "start_time", 0);
    m_nEndTime = Answer::Inifile::GetInt(&ini, "base", "end_time", 0);
    m_nIcon = Answer::Inifile::GetInt(&ini, "base", "icon", 0);
    m_nIconHideDay = Answer::Inifile::GetInt(&ini, "base", "icon_hide_day", 0);
    m_nDropGroup = Answer::Inifile::GetInt(&ini, "base", "drop_group", 0);
    m_nBossScoreRate = Answer::Inifile::GetInt(&ini, "base", "boss_score_rate", 100);
    m_nWishNeedRecharge = Answer::Inifile::GetInt(&ini, "base", "wish_need_recharge", 0);
    m_ChouJiangType = Answer::Inifile::GetInt(&ini, "base", "chou_jiang_type", 0);
    m_ChouJiangCount = Answer::Inifile::GetInt(&ini, "base", "chou_jiang_count", 0);
    m_nRechargeBackSize = Answer::Inifile::GetInt(&ini, "base", "recharge_back_size", 0);
    
    // 初始化活动开始结束天数组
    for (int i = 0; i < 50; i++)
    {
        char section[32];
        snprintf(section, sizeof(section), "activity_%d", i);
        m_vStartDay[i] = Answer::Inifile::GetInt(&ini, section, "start_day", 0);
        m_vEndDay[i] = Answer::Inifile::GetInt(&ini, section, "end_day", 0);
    }
    
    // 初始化登录奖�?
    m_vLandGift.clear();
    for (int i = 0; i < 30; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "land_gift_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "land_gift", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> gifts;
        ParseItemString(value, gifts);
        m_vLandGift.push_back(gifts);
    }
    
    // 初始化抽奖循环奖�?
    m_vDrawLoopReward.clear();
    std::string drawLoopReward = Answer::Inifile::GetString(&ini, "draw", "loop_reward", "");
    ParseItemString(drawLoopReward, m_vDrawLoopReward);
    
    // 初始化抽奖排行榜奖励
    m_vDrawRankReward.clear();
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "rank_reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "draw_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vDrawRankReward.push_back(rewards);
    }
    
    m_vDrawRankMail.clear();
    std::string mailIds = Answer::Inifile::GetString(&ini, "draw_rank", "mail_ids", "");
    ParseIntVector(mailIds, m_vDrawRankMail);
    
    // 初始化世界BOSS时间
    m_vWorldBossMinute.clear();
    for (int i = 0; i < 10; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "boss_time_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "world_boss", key, "");
        if (value.empty()) break;
        
        TimeArea area;
        size_t commaPos = value.find(',');
        if (commaPos != std::string::npos)
        {
            area.nStart = atoi(value.substr(0, commaPos).c_str());
            area.nEnd = atoi(value.substr(commaPos + 1).c_str());
        }
        m_vWorldBossMinute.push_back(area);
    }
    
    // 初始化活跃度奖励
    m_vHuoYueDuSumLimit.clear();
    m_vHuoYueDuSumReward.clear();
    std::string huoYueDuLimit = Answer::Inifile::GetString(&ini, "huoyuedu", "limit", "");
    std::string huoYueDuReward = Answer::Inifile::GetString(&ini, "huoyuedu", "reward", "");
    ParseIntVector(huoYueDuLimit, m_vHuoYueDuSumLimit);
    ParseRewardString(huoYueDuReward, m_vHuoYueDuSumReward);
    
    // 初始化每日限购商�?
    m_vDailyLimitShopItem.clear();
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "shop_item_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "daily_limit_shop", key, "");
        if (value.empty()) break;
        
        MemChrBag item;
        size_t commaPos = value.find(',');
        if (commaPos != std::string::npos)
        {
            item.nItemId = atoi(value.substr(0, commaPos).c_str());
            item.nCount = atoi(value.substr(commaPos + 1).c_str());
        }
        m_vDailyLimitShopItem.push_back(item);
    }
    
    // 初始化累计登录奖�?
    m_vLandSumGift.clear();
    for (int i = 0; i < 30; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "land_sum_gift_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "land_sum", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> gifts;
        ParseItemString(value, gifts);
        m_vLandSumGift.push_back(gifts);
    }
    
    // 初始化在线奖�?
    m_vOnlineReward.clear();
    for (int i = 0; i < 10; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "online_reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "online", key, "");
        if (value.empty()) break;
        
        MemChrBag item;
        size_t commaPos = value.find(',');
        if (commaPos != std::string::npos)
        {
            item.nItemId = atoi(value.substr(0, commaPos).c_str());
            item.nCount = atoi(value.substr(commaPos + 1).c_str());
            // 假设后面还有 need_time
            size_t commaPos2 = value.find(',', commaPos + 1);
            if (commaPos2 != std::string::npos)
            {
                item.nNeedTime = atoi(value.substr(commaPos + 1, commaPos2 - commaPos - 1).c_str());
            }
        }
        m_vOnlineReward.push_back(item);
    }
    
    // 初始化许愿奖�?
    m_vWishReward.clear();
    std::string wishReward = Answer::Inifile::GetString(&ini, "wish", "reward", "");
    ParseItemString(wishReward, m_vWishReward);
    
    // 初始化消费排行榜配置
    m_vXiaoFeiRankLimit.clear();
    std::string xiaoFeiLimit = Answer::Inifile::GetString(&ini, "xiao_fei_rank", "limit", "");
    ParseIntVector(xiaoFeiLimit, m_vXiaoFeiRankLimit);
    
    m_vXiaoFeiRankMail.clear();
    std::string xiaoFeiMail = Answer::Inifile::GetString(&ini, "xiao_fei_rank", "mail", "");
    ParseIntVector(xiaoFeiMail, m_vXiaoFeiRankMail);
    
    m_vXiaoFeiRankReward.clear();
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "xiao_fei_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vXiaoFeiRankReward.push_back(rewards);
    }
    
    // 初始化消费抽奖�?
    m_nXiaoFeiDrawValue.clear();
    std::string drawValue = Answer::Inifile::GetString(&ini, "xiao_fei_draw", "value", "");
    ParseIntVector(drawValue, m_nXiaoFeiDrawValue);
    
    // 初始化魔域世界掉�?
    m_vMoYuShiJieDrop.clear();
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "drop_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "mo_yu", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> drops;
        ParseItemString(value, drops);
        m_vMoYuShiJieDrop.push_back(drops);
    }
    
    // 初始化每日充值配�?
    m_vDailyRechargeLimit.clear();
    m_vDailyRechargeReward.clear();
    m_vDailyRechargeBroad.clear();
    std::string dailyRechargeLimit = Answer::Inifile::GetString(&ini, "daily_recharge", "limit", "");
    std::string dailyRechargeReward = Answer::Inifile::GetString(&ini, "daily_recharge", "reward", "");
    std::string dailyRechargeBroad = Answer::Inifile::GetString(&ini, "daily_recharge", "broadcast", "");
    ParseIntVector(dailyRechargeLimit, m_vDailyRechargeLimit);
    ParseRewardString(dailyRechargeReward, m_vDailyRechargeReward);
    ParseIntVector(dailyRechargeBroad, m_vDailyRechargeBroad);
    
    // 初始化累计消费排行榜
    m_vXiaoFeiSumRankMail.clear();
    m_vXiaoFeiSumRankReward.clear();
    m_vXiaoFeiSumLimit.clear();
    m_vXiaoFeiSumBroadcast.clear();
    m_vXiaoFeiSumReward.clear();
    
    std::string xiaoFeiSumMail = Answer::Inifile::GetString(&ini, "xiao_fei_sum_rank", "mail", "");
    ParseIntVector(xiaoFeiSumMail, m_vXiaoFeiSumRankMail);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "xiao_fei_sum_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vXiaoFeiSumRankReward.push_back(rewards);
    }
    
    std::string xiaoFeiSumLimit = Answer::Inifile::GetString(&ini, "xiao_fei_sum", "limit", "");
    std::string xiaoFeiSumBroadcast = Answer::Inifile::GetString(&ini, "xiao_fei_sum", "broadcast", "");
    std::string xiaoFeiSumReward = Answer::Inifile::GetString(&ini, "xiao_fei_sum", "reward", "");
    ParseIntVector(xiaoFeiSumLimit, m_vXiaoFeiSumLimit);
    ParseIntVector(xiaoFeiSumBroadcast, m_vXiaoFeiSumBroadcast);
    ParseRewardString(xiaoFeiSumReward, m_vXiaoFeiSumReward);
    
    // 初始化礼包商�?
    m_vGiftShopGoods.clear();
    for (int i = 0; i < 10; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "goods_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "gift_shop", key, "");
        if (value.empty()) break;
        
        std::vector<ShopGoods> goods;
        // 格式: goodsId,price,limit|goodsId,price,limit
        std::stringstream ss(value);
        std::string goodsStr;
        while (std::getline(ss, goodsStr, '|'))
        {
            if (goodsStr.empty()) continue;
            ShopGoods sg;
            size_t commaPos = goodsStr.find(',');
            if (commaPos != std::string::npos)
            {
                sg.nGoodsId = atoi(goodsStr.substr(0, commaPos).c_str());
                size_t commaPos2 = goodsStr.find(',', commaPos + 1);
                if (commaPos2 != std::string::npos)
                {
                    sg.nPrice = atoi(goodsStr.substr(commaPos + 1, commaPos2 - commaPos - 1).c_str());
                    sg.nLimit = atoi(goodsStr.substr(commaPos2 + 1).c_str());
                }
            }
            goods.push_back(sg);
        }
        m_vGiftShopGoods.push_back(goods);
    }
    
    // 初始化返利配�?
    m_NeedValue.clear();
    m_BackItem.clear();
    std::string needValue = Answer::Inifile::GetString(&ini, "recharge_back", "need_value", "");
    std::string backItem = Answer::Inifile::GetString(&ini, "recharge_back", "back_item", "");
    ParseIntVector(needValue, m_NeedValue);
    ParseItemString(backItem, m_BackItem);
    
    // 初始化累计充值配�?
    m_vRechargeSumLimit.clear();
    m_vRechargeSumBroadcast.clear();
    m_vRechargeSumReward.clear();
    std::string rechargeSumLimit = Answer::Inifile::GetString(&ini, "recharge_sum", "limit", "");
    std::string rechargeSumBroadcast = Answer::Inifile::GetString(&ini, "recharge_sum", "broadcast", "");
    std::string rechargeSumReward = Answer::Inifile::GetString(&ini, "recharge_sum", "reward", "");
    ParseIntVector(rechargeSumLimit, m_vRechargeSumLimit);
    ParseIntVector(rechargeSumBroadcast, m_vRechargeSumBroadcast);
    ParseRewardString(rechargeSumReward, m_vRechargeSumReward);
    
    // 初始化打体排行榜
    m_vDaTiRankMail.clear();
    m_vDaTiRankReward.clear();
    std::string daTiMail = Answer::Inifile::GetString(&ini, "da_ti_rank", "mail", "");
    ParseIntVector(daTiMail, m_vDaTiRankMail);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "da_ti_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vDaTiRankReward.push_back(rewards);
    }
    
    // 初始化每日限购商�?
    m_vDailyLimitShop2Broadcast.clear();
    m_vDailyLimitShop2Goods.clear();
    std::string shop2Broadcast = Answer::Inifile::GetString(&ini, "daily_limit_shop2", "broadcast", "");
    ParseIntVector(shop2Broadcast, m_vDailyLimitShop2Broadcast);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "goods_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "daily_limit_shop2", key, "");
        if (value.empty()) break;
        
        ShopGoods goods;
        size_t commaPos = value.find(',');
        if (commaPos != std::string::npos)
        {
            goods.nGoodsId = atoi(value.substr(0, commaPos).c_str());
            size_t commaPos2 = value.find(',', commaPos + 1);
            if (commaPos2 != std::string::npos)
            {
                goods.nPrice = atoi(value.substr(commaPos + 1, commaPos2 - commaPos - 1).c_str());
                goods.nLimit = atoi(value.substr(commaPos2 + 1).c_str());
            }
        }
        m_vDailyLimitShop2Goods.push_back(goods);
    }
    
    // 初始化装备升星返星数配置
    m_mEquipUpStarBackStarCount.clear();
    std::string equipBack = Answer::Inifile::GetString(&ini, "equip_upstar_back", "star_count", "");
    // 格式: starLevel,backCount|starLevel,backCount
    std::stringstream ss(equipBack);
    std::string token;
    while (std::getline(ss, token, '|'))
    {
        if (token.empty()) continue;
        size_t commaPos = token.find(',');
        if (commaPos != std::string::npos)
        {
            int starLevel = atoi(token.substr(0, commaPos).c_str());
            int backCount = atoi(token.substr(commaPos + 1).c_str());
            m_mEquipUpStarBackStarCount[starLevel] = backCount;
        }
    }
    
    // 初始化充值抽奖�?
    m_nRechargeDrawValue.clear();
    std::string rechargeDrawValue = Answer::Inifile::GetString(&ini, "recharge_draw", "value", "");
    ParseIntVector(rechargeDrawValue, m_nRechargeDrawValue);
    
    // 初始化购买礼包配�?
    m_vBuyGiftPrice.clear();
    m_vBuyGiftBroadcast.clear();
    m_vBuyGiftItem.clear();
    for (int i = 0; i < 50; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "price_%d", i);
        int price = Answer::Inifile::GetInt(&ini, "buy_gift", key, 0);
        if (price == 0 && i > 0) break;
        m_vBuyGiftPrice.push_back(price);
        
        snprintf(key, sizeof(key), "broadcast_%d", i);
        int broadcast = Answer::Inifile::GetInt(&ini, "buy_gift", key, 0);
        m_vBuyGiftBroadcast.push_back(broadcast);
        
        snprintf(key, sizeof(key), "item_%d", i);
        std::string itemStr = Answer::Inifile::GetString(&ini, "buy_gift", key, "");
        std::vector<MemChrBag> items;
        ParseItemString(itemStr, items);
        m_vBuyGiftItem.push_back(items);
    }
    
    // 初始化充值返利配�?
    m_vRechargeBackLimit.clear();
    m_vRechargeBackValue.clear();
    for (int i = 0; i < m_nRechargeBackSize; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "limit_%d", i);
        int limit = Answer::Inifile::GetInt(&ini, "recharge_back", key, 0);
        m_vRechargeBackLimit.push_back(limit);
        
        snprintf(key, sizeof(key), "value_%d", i);
        int value = Answer::Inifile::GetInt(&ini, "recharge_back", key, 0);
        m_vRechargeBackValue.push_back(value);
    }
    
    // 初始化宠物幻化配�?
    m_vPetIllusionItemLimit.clear();
    m_vPetIllusionItemReward.clear();
    std::string petLimit = Answer::Inifile::GetString(&ini, "pet_illusion", "limit", "");
    std::string petReward = Answer::Inifile::GetString(&ini, "pet_illusion", "reward", "");
    ParseIntVector(petLimit, m_vPetIllusionItemLimit);
    ParseRewardString(petReward, m_vPetIllusionItemReward);
    
    // 初始化累计充值排行榜
    m_vRechargeSumRankLimit.clear();
    m_vRechargeSumRankMail.clear();
    m_vRechargeSumRankReward.clear();
    std::string rechargeSumRankLimit = Answer::Inifile::GetString(&ini, "recharge_sum_rank", "limit", "");
    std::string rechargeSumRankMail = Answer::Inifile::GetString(&ini, "recharge_sum_rank", "mail", "");
    ParseIntVector(rechargeSumRankLimit, m_vRechargeSumRankLimit);
    ParseIntVector(rechargeSumRankMail, m_vRechargeSumRankMail);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "recharge_sum_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vRechargeSumRankReward.push_back(rewards);
    }
    
    // 初始化跨服累计充值排行榜
    m_vCrossRechargeSumRankLimit.clear();
    m_vCrossRechargeSumRankMail.clear();
    m_vCrossRechargeSumRankReward.clear();
    std::string crossRechargeLimit = Answer::Inifile::GetString(&ini, "cross_recharge_sum_rank", "limit", "");
    std::string crossRechargeMail = Answer::Inifile::GetString(&ini, "cross_recharge_sum_rank", "mail", "");
    ParseIntVector(crossRechargeLimit, m_vCrossRechargeSumRankLimit);
    ParseIntVector(crossRechargeMail, m_vCrossRechargeSumRankMail);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "cross_recharge_sum_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vCrossRechargeSumRankReward.push_back(rewards);
    }
    
    // 初始化跨服累计消费排行榜
    m_vCrossXiaoFeiSumRankLimit.clear();
    m_vCrossXiaoFeiSumRankMail.clear();
    m_vCrossXiaoFeiSumRankReward.clear();
    std::string crossXiaoFeiLimit = Answer::Inifile::GetString(&ini, "cross_xiao_fei_sum_rank", "limit", "");
    std::string crossXiaoFeiMail = Answer::Inifile::GetString(&ini, "cross_xiao_fei_sum_rank", "mail", "");
    ParseIntVector(crossXiaoFeiLimit, m_vCrossXiaoFeiSumRankLimit);
    ParseIntVector(crossXiaoFeiMail, m_vCrossXiaoFeiSumRankMail);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "cross_xiao_fei_sum_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vCrossXiaoFeiSumRankReward.push_back(rewards);
    }
    
    // 初始化跨服抽奖排行榜
    m_vCrossChouJiangRankLimit.clear();
    m_vCrossChouJiangRankMail.clear();
    m_vCrossChouJiangRankReward.clear();
    std::string crossChouJiangLimit = Answer::Inifile::GetString(&ini, "cross_chou_jiang_rank", "limit", "");
    std::string crossChouJiangMail = Answer::Inifile::GetString(&ini, "cross_chou_jiang_rank", "mail", "");
    ParseIntVector(crossChouJiangLimit, m_vCrossChouJiangRankLimit);
    ParseIntVector(crossChouJiangMail, m_vCrossChouJiangRankMail);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "cross_chou_jiang_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vCrossChouJiangRankReward.push_back(rewards);
    }
    
    // 初始化装备情义配�?
    m_vEquipQingYiLimit.clear();
    m_vEquipQingYiReward.clear();
    std::string equipQingYiLimit = Answer::Inifile::GetString(&ini, "equip_qing_yi", "limit", "");
    std::string equipQingYiReward = Answer::Inifile::GetString(&ini, "equip_qing_yi", "reward", "");
    ParseIntVector(equipQingYiLimit, m_vEquipQingYiLimit);
    ParseRewardString(equipQingYiReward, m_vEquipQingYiReward);
    
    // 初始化好友情义配�?
    m_vFriendQingYiLimit.clear();
    m_vFriendQingYiReward.clear();
    std::string friendQingYiLimit = Answer::Inifile::GetString(&ini, "friend_qing_yi", "limit", "");
    std::string friendQingYiReward = Answer::Inifile::GetString(&ini, "friend_qing_yi", "reward", "");
    ParseIntVector(friendQingYiLimit, m_vFriendQingYiLimit);
    ParseRewardString(friendQingYiReward, m_vFriendQingYiReward);
    
    // 初始化BOSS积分排行�?
    m_vBossScoreRankMail.clear();
    m_vBossScoreRankReward.clear();
    m_vBossScoreiLimit.clear();
    m_vGetBossScoreMail.clear();
    m_vBossScoreReward.clear();
    m_vScoreLimit.clear();
    m_nDailyRechargeDrawLimit.clear();
    m_vBossScoreDrawReward.clear();
    
    std::string bossScoreRankMail = Answer::Inifile::GetString(&ini, "boss_score_rank", "mail", "");
    ParseIntVector(bossScoreRankMail, m_vBossScoreRankMail);
    
    for (int i = 0; i < 100; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "rank_reward_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "boss_score_rank", key, "");
        if (value.empty()) break;
        
        std::vector<MemChrBag> rewards;
        ParseItemString(value, rewards);
        m_vBossScoreRankReward.push_back(rewards);
    }
    
    std::string bossScoreiLimit = Answer::Inifile::GetString(&ini, "boss_score", "limit", "");
    std::string getBossScoreMail = Answer::Inifile::GetString(&ini, "boss_score", "mail", "");
    std::string bossScoreReward = Answer::Inifile::GetString(&ini, "boss_score", "reward", "");
    ParseIntVector(bossScoreiLimit, m_vBossScoreiLimit);
    ParseIntVector(getBossScoreMail, m_vGetBossScoreMail);
    ParseRewardString(bossScoreReward, m_vBossScoreReward);
    
    std::string scoreLimit = Answer::Inifile::GetString(&ini, "score_draw", "limit", "");
    std::string dailyRechargeDrawLimit = Answer::Inifile::GetString(&ini, "score_draw", "daily_limit", "");
    ParseIntVector(scoreLimit, m_vScoreLimit);
    ParseIntVector(dailyRechargeDrawLimit, m_nDailyRechargeDrawLimit);
    
    // 初始化BOSS积分抽奖物品
    m_vBossScoreDrawReward.clear();
    for (int i = 0; i < 50; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "draw_item_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "score_draw", key, "");
        if (value.empty()) break;
        
        RateItem item;
        size_t commaPos = value.find(',');
        if (commaPos != std::string::npos)
        {
            item.nItemId = atoi(value.substr(0, commaPos).c_str());
            size_t commaPos2 = value.find(',', commaPos + 1);
            if (commaPos2 != std::string::npos)
            {
                item.nCount = atoi(value.substr(commaPos + 1, commaPos2 - commaPos - 1).c_str());
                item.nRate = atoi(value.substr(commaPos2 + 1).c_str());
            }
        }
        m_vBossScoreDrawReward.push_back(item);
    }
    
    // 初始化购物车礼包
    m_vGouWuCheGiftPrice.clear();
    m_vGouWuCheItem.clear();
    std::string gouWuChePrice = Answer::Inifile::GetString(&ini, "gou_wu_che", "price", "");
    std::string gouWuCheItem = Answer::Inifile::GetString(&ini, "gou_wu_che", "item", "");
    ParseIntVector(gouWuChePrice, m_vGouWuCheGiftPrice);
    ParseItemString(gouWuCheItem, m_vGouWuCheItem);
    
    // 初始化连充配�?
    m_LianRechargeCfgVt.clear();
    for (int i = 0; i < 30; i++)
    {
        char key[32];
        snprintf(key, sizeof(key), "lian_recharge_%d", i);
        std::string value = Answer::Inifile::GetString(&ini, "lian_recharge", key, "");
        if (value.empty()) break;
        
        LianRechargeCfg cfg;
        size_t commaPos = value.find(',');
        if (commaPos != std::string::npos)
        {
            cfg.nDay = atoi(value.substr(0, commaPos).c_str());
            cfg.nRewardId = atoi(value.substr(commaPos + 1).c_str());
        }
        m_LianRechargeCfgVt.push_back(cfg);
    }
    
    // 神秘商店刷新消�?
    std::string refreshCost = Answer::Inifile::GetString(&ini, "mystery_shop", "refresh_cost", "");
    size_t commaPos = refreshCost.find(',');
    if (commaPos != std::string::npos)
    {
        m_sMysteryShopRefreshCost.m_nClass = atoi(refreshCost.substr(0, commaPos).c_str());
        m_sMysteryShopRefreshCost.m_nType = atoi(refreshCost.substr(commaPos + 1).c_str());
        size_t commaPos2 = refreshCost.find(',', commaPos + 1);
        if (commaPos2 != std::string::npos)
        {
            m_sMysteryShopRefreshCost.m_nCount = atoi(refreshCost.substr(commaPos + 1, commaPos2 - commaPos - 1).c_str());
        }
    }
}

void CFestivalDoubleEleven::loadRankData(int32_t line)
{
    if (line == 1)
    {
        loadDrawRank();
        loadXiaoFeiRank();
        loadXiaoFeiSumRank();
        loadRechargeSumRank();
        loadCrossRechargeSumRank();
        loadCrossXiaoFeiSumRank();
        loadCrossChouJiangRank();
        loadBossScoreRank();
    }
}

void CFestivalDoubleEleven::resetRankData(int32_t line)
{
    Answer::DBPool *v2 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v2);
    
    if (line == 1)
    {
        Answer::MySqlDBGuard::excute(&db, "DELETE FROM mem_double_eleven_rank");
        
        m_mDrawRank.clear();
        m_mXiaoFeiRank.clear();
        m_mXiaoFeiSumRank.clear();
        m_mRechargeSumRank.clear();
        m_mCrossXiaoFeiSumRank.clear();
        m_mCrossRechargeSumRank.clear();
        m_mCrossChouJiangRank.clear();
        m_mBossScoreRank.clear();
    }
    
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::resetPlayerData()
{
    int32_t m_nVersion = this->m_nVersion;
    GameService *v2 = Answer::Singleton<GameService>::instance();
    GameService::ResetFestivalData(v2, m_nVersion);
}

int32_t CFestivalDoubleEleven::loadVersion(int32_t line)
{
    Answer::DBPool *v2 = Answer::Singleton<Answer::DBPool>::instance();
    char szSQL[4096];
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v2);
    snprintf(szSQL, sizeof(szSQL), 
             "SELECT version FROM mem_double_eleven_version WHERE line=%d", line);
    Answer::MySqlQuery::MySqlQuery(&result);
    
    int32_t v6 = 0;
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        if (Answer::MySqlQuery::next(&result))
        {
            const char *StringValue = Answer::MySqlQuery::getString(&result, 0);
            v6 = atoi(StringValue);
        }
    }
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
    
    return v6;
}

void CFestivalDoubleEleven::saveVersion(int32_t line)
{
    Answer::DBPool *v2 = Answer::Singleton<Answer::DBPool>::instance();
    int32_t m_nVersion = this->m_nVersion;
    char szSQL[4096];
    Answer::MySqlDBGuard db;
    std::string tname;
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v2);
    
    snprintf(szSQL, sizeof(szSQL), 
             "DELETE FROM mem_double_eleven_version WHERE line=%d", line);
    Answer::MySqlDBGuard::excute(&db, szSQL);
    
    int32_t nNowTime = time(NULL);
    snprintf(szSQL, sizeof(szSQL),
             "INSERT INTO mem_double_eleven_version (line, version, update_time) VALUES (%d, %d, %d)",
             line, m_nVersion, nNowTime);
    Answer::MySqlDBGuard::excute(&db, szSQL);
    
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::loadDrawRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=1 ORDER BY rank_value DESC LIMIT 100");
    
    m_mDrawRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 2);
            const char* name = Answer::MySqlQuery::getString(&result, 3);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            m_mDrawRank[rank.nRankType].push_back(rank);
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::checkDrawRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mDrawRank.find(connid);
    if (it != m_mDrawRank.end())
    {
        std::vector<FestivalRank>& vRank = it->second;
        for (auto it2 = vRank.begin(); it2 != vRank.end(); )
        {
            if (!GameService::IsPlayerExist(it2->nCharId))
                it2 = vRank.erase(it2);
            else
                ++it2;
        }
    }
}

void CFestivalDoubleEleven::checkDrawRank()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mDrawRank)
    {
        for (auto& rank : kv.second)
        {
            char szSQL[1024];
            snprintf(szSQL, sizeof(szSQL),
                     "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, rank_value, char_name, update_time) VALUES (%d, 1, %d, %d, '%s', %d)",
                     rank.nCharId, kv.first, rank.nValue, rank.szName, (int)time(NULL));
            // 执行SQL
            Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
            Answer::MySqlDBGuard db;
            Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
            Answer::MySqlDBGuard::excute(&db, szSQL);
            Answer::MySqlDBGuard::~MySqlDBGuard(&db);
        }
    }
}

void CFestivalDoubleEleven::sendDrawRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mDrawRank)
    {
        int nRank = 1;
        for (auto& rank : kv.second)
        {
            int rewardIndex = nRank - 1;
            if (rewardIndex < (int)m_vDrawRankReward.size())
            {
                char szTitle[256];
                snprintf(szTitle, sizeof(szTitle), "抽奖排行榜奖�?- �?d�?, nRank);
                // MailSystem::SendMail(rank.nCharId, m_vDrawRankMail[0], szTitle, "", m_vDrawRankReward[rewardIndex]);
            }
            nRank++;
        }
    }
}
nvoid CFestivalDoubleEleven::GetIconState(CFestivalDoubleEleven* _this, Player* player, std::list<ShowIcon>* IconList)
{
    if (!_this || !player || !IconList)
        return;
    if (!_this->needShowIcon())
        return;

    ShowIcon stu;
    memset(&stu, 0, sizeof(stu));
    stu.nId = _this->m_nIcon;
    stu.nState = 1;
    stu.nLeftTime = _this->getLeftTime();

    if (_this->canGetLandGift(player) ||
        _this->canGetDrawGift(player) ||
        _this->canGetOnlineGift(player) ||
        _this->canGetWishGift(player))
    {
        stu.nState = 2;
    }

    IconList->push_back(stu);
}

void CFestivalDoubleEleven::GetWorldBossIconState(std::list<ShowIcon>* IconList)
{
    if (!IconList)
        return;

    ShowIcon stu;
    memset(&stu, 0, sizeof(stu));
    stu.nId = 53;
    stu.nState = 1;
    stu.nLeftTime = -1;
    IconList->push_back(stu);
}

void CFestivalDoubleEleven::GetMoYuShiJieIconState(std::list<ShowIcon>* IconList)
{
    if (!IconList)
        return;

    ShowIcon stu;
    memset(&stu, 0, sizeof(stu));
    stu.nId = 54;
    stu.nState = 1;
    stu.nLeftTime = -1;
    IconList->push_back(stu);
}

void CFestivalDoubleEleven::SendIconState(Player *player)
{
    ShowIcon stu;
    if (player)
    {
        if (needShowIcon())
        {
            getIconState(&stu, this, player);
            Player::SendIconState(player, &stu);
        }
    }
}

void CFestivalDoubleEleven::hideIcon(int32_t nIconId)
{
    GameService *v2 = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(v2) == 1)
    {
        Answer::NetPacket *packet = Answer::NetPacket::Create();
        packet->WriteInt(nIconId);
        GameService::BroadcastToAll(v2, packet);
        Answer::NetPacket::Release(packet);
    }
}

void CFestivalDoubleEleven::SendActivityInfo(Player *player)
{
    if (!player) return;
    
    // 构建活动信息响应�?
    Answer::NetPacket* packet = Answer::NetPacket::Create();
    packet->WriteInt(m_nDay);  // 当前活动天数
    packet->WriteInt(getLeftTime());  // 剩余时间
    
    // 登录奖励状�?
    int landState = 0;
    if (canGetLandGift(player)) landState = 1;
    else if (Player::GetOperateLimit(player) && CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 2030) > 0) landState = 2;
    packet->WriteChar(landState);
    
    // 累计登录奖励状�?
    for (int i = 0; i < (int)m_vLandSumGift.size(); i++)
    {
        int state = 0;
        if (canGetLandSumGift(player, i)) state = 1;
        else if (Player::getRecord(player, 2031) & (1 << i)) state = 2;
        packet->WriteChar(state);
    }
    
    // 更多活动信息...
    
    Player::SendPacket(player, packet);
    Answer::NetPacket::Release(packet);
}

bool CFestivalDoubleEleven::IsInTime()
{
    CTimer* timer = Answer::Singleton<CTimer>::instance();
    int nowTime = CTimer::GetNow(timer);
    return nowTime >= m_nStartTime && nowTime < m_nEndTime;
}

bool CFestivalDoubleEleven::needShowIcon()
{
    CTimer* timer = Answer::Singleton<CTimer>::instance();
    int nowTime = CTimer::GetNow(timer);
    if (nowTime >= m_nStartTime && nowTime < m_nEndTime)
        return true;
    // 检查是否有任何子活动在开�?
    for (int i = 0; i < 50; i++)
    {
        if (isOpen((FESTIVAL_ACTIVITY_TYPE)i) && m_nDay >= m_vStartDay[i] && m_nDay < m_vEndDay[i])
            return true;
    }
    return false;
}

bool CFestivalDoubleEleven::IsInTime(FESTIVAL_ACTIVITY_TYPE nType)
{
    return IsOpen(nType)
        && m_nDay >= m_vStartDay[nType]
        && m_nDay < m_vEndDay[nType];
}

bool CFestivalDoubleEleven::IsOpen(FESTIVAL_ACTIVITY_TYPE nType)
{
    return IsInTime() && isOpen(nType);
}

bool CFestivalDoubleEleven::isOpen(FESTIVAL_ACTIVITY_TYPE nType)
{
    return m_setOpenList.find((int8_t)nType) != m_setOpenList.end();
}

bool CFestivalDoubleEleven::CheckDropGroup(int32_t nDropGroup)
{
    return IsInTime() && m_nDropGroup == nDropGroup;
}

int32_t CFestivalDoubleEleven::GetLandGift(Player *player)
{
    if (!player)
        return 10002;
    if (!canGetLandGift(player))
        return 10002;

    int nIndex = m_nDay;
    if (m_nDay < 0 || (int)m_vLandGift.size() <= nIndex)
        return 10002;

    std::vector<MemChrBag>* v4 = &m_vLandGift[nIndex];
    CExtCharBag* Bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(Bag, v4, ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_LAND_GIFT))
        return 10002;

    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    CExtOperateLimit::UpdateLimitCount(OperateLimit, 2030, 1);
    SendIconState(player);
    return 0;
}

int32_t CFestivalDoubleEleven::GetLandSumGift(Player *player, int8_t nIndex)
{
    if (!player)
        return 10002;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM))
        return 10002;
    
    if (nIndex < 0 || nIndex >= (int8_t)m_vLandSumGift.size())
        return 10002;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int32_t nOldRecord = CExtOperateLimit::GetLimitCount(OperateLimit, 2031);
    
    int nLandDays = m_nDay - m_vStartDay[FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM] + 1;
    if (nLandDays < 0) nLandDays = 0;
    
    if (nLandDays < nIndex + 1)
        return 10002;
    
    if ((nOldRecord & (1 << nIndex)) != 0)
        return 10002;
    
    std::vector<MemChrBag>* v7 = &m_vLandSumGift[nIndex];
    CExtCharBag* Bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(Bag, v7, ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_LAND_GIFT))
        return 10002;
    
    int nNewRecord = nOldRecord | (1 << nIndex);
    CExtOperateLimit::SetLimitCount(OperateLimit, 2031, nNewRecord);
    
    return 0;
}

int32_t CFestivalDoubleEleven::GetDrawGift(Player *player)
{
    if (!player)
        return 10002;
    
    if (!canGetDrawGift(player))
        return 10002;
    
    CExtCharBag* Bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(Bag, &m_vDrawLoopReward, ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_DRAW_GIFT))
        return 10002;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    CExtOperateLimit::AddLimitCount(OperateLimit, 1602, 1);
    
    return 0;
}

int32_t CFestivalDoubleEleven::GetOnlineGift(Player *player)
{
    if (!player)
        return 10002;
    
    if (!canGetOnlineGift(player))
        return 10002;
    
    int nRewardTimes = GetOnlineTimeRewardTime(player);
    if (nRewardTimes < 0 || nRewardTimes >= (int)m_vOnlineReward.size())
        return 10002;
    
    CExtCharBag* Bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(Bag, &m_vOnlineReward[nRewardTimes], ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_ONLINE_GIFT))
        return 10002;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    CExtOperateLimit::AddLimitCount(OperateLimit, 1604, 1);
    
    return 0;
}

int32_t CFestivalDoubleEleven::GetWishGift(Player *player)
{
    if (!player)
        return 10002;
    
    if (!canGetWishGift(player))
        return 10002;
    
    CExtCharBag* Bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(Bag, &m_vWishReward, ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_WISH_GIFT))
        return 10002;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    CExtOperateLimit::AddLimitCount(OperateLimit, 1615, 1);
    
    return 0;
}

int32_t CFestivalDoubleEleven::BuyDailyLimitShopItem(Player *player)
{
    if (!player)
        return 10002;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_DAILY_LIMIT_SHOP))
        return 10002;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nBoughtCount = CExtOperateLimit::GetLimitCount(OperateLimit, 1605);
    
    if (nBoughtCount >= (int)m_vDailyLimitShopItem.size())
        return 10002;
    
    // 假设每个物品价格100金币，实际需要从配置读取
    // if (!Player::DecCurrency(player, CURRENCY_TYPE::GOLD, 100))
    //     return 10002;
    
    CExtCharBag* Bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(Bag, &m_vDailyLimitShopItem[nBoughtCount], ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_SHOP))
        return 10002;
    
    CExtOperateLimit::AddLimitCount(OperateLimit, 1605, 1);
    
    return 0;
}

bool CFestivalDoubleEleven::BuyTitle(Player *player)
{
    if (!player)
        return false;
    
    if (Player::getRecord(player, 1631) > 0)
        return false;
    
    // 扣费逻辑（具体价格需要从配置读取�?
    // if (!Player::DecCurrency(player, CURRENCY_TYPE::GOLD, 500))
    //     return false;
    
    Player::setRecord(player, 1631, 1);
    
    return true;
}

int32_t CFestivalDoubleEleven::BuyGiftItem(Player *player, int8_t nIndex)
{
    if (!player)
        return 10002;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_GIFT_SHOP))
        return 10002;
    
    if (nIndex < 0 || nIndex >= (int8_t)m_vBuyGiftPrice.size())
        return 10002;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nBoughtFlag = CExtOperateLimit::GetLimitCount(OperateLimit, 1616);
    
    if ((nBoughtFlag & (1 << nIndex)) != 0)
        return 10002;
    
    int nPrice = m_vBuyGiftPrice[nIndex];
    // if (!Player::DecCurrency(player, CURRENCY_TYPE::GOLD, nPrice))
    //     return 10002;
    
    std::vector<MemChrBag>* v6 = &m_vBuyGiftItem[nIndex];
    CExtCharBag* Bag = Player::GetBag(player);
    if (!CExtCharBag::AddItem(Bag, v6, ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_BUY_GIFT))
        return 10002;
    
    CExtOperateLimit::SetLimitCount(OperateLimit, 1616, nBoughtFlag | (1 << nIndex));
    
    if ((int)m_vBuyGiftBroadcast.size() > nIndex && m_vBuyGiftBroadcast[nIndex] > 0)
    {
        // 广播玩家购买礼包
        // BroadcastPlayerBuyGift(player, nIndex);
    }
    
    return 0;
}

int32_t CFestivalDoubleEleven::GetRechargeBack(Player *player)
{
    if (!player)
        return 10002;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_RECHARGE_BACK))
        return 10002;
    
    if (!canGetRechargeBack(player))
        return 10002;
    
    int nRechargeBack = calRechargeBack(Player::GetTotalRecharge(player));
    
    // Player::AddCurrency(player, CURRENCY_TYPE::GOLD, nRechargeBack);
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    CExtOperateLimit::SetLimitCount(OperateLimit, 1620, 0);
    
    return 0;
}

int32_t CFestivalDoubleEleven::GetEquipUpStarBackItem(Player *player)
{
    if (!player)
        return 10002;
    
    if (!canGetEquipUpStarBack(player))
        return 10002;
    
    // 计算返利物品
    for (auto& kv : m_mEquipUpStarBackStarCount)
    {
        int nStarLevel = kv.first;
        int nBackCount = kv.second;
        // TODO: 根据玩家升星记录发放返利物品
        if (nBackCount > 0)
        {
            MemChrBag item;
            item.nItemId = 0; // 返利物品ID
            item.nCount = nBackCount;
            CExtCharBag* Bag = Player::GetBag(player);
            CExtCharBag::AddItem(Bag, &item, ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_EQUIP_BACK);
        }
    }
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    CExtOperateLimit::SetLimitCount(OperateLimit, 1617, 0);
    
    return 0;
}

void CFestivalDoubleEleven::GongGao(int32_t GongGaoId, Player *player)
{
    if (player)
    {
        // 发送公告给玩家
        // Player::SendGongGao(player, GongGaoId);
    }
}

void CFestivalDoubleEleven::AddDrawTimes(Player *player, int32_t nCount)
{
    if (!player) return;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_DRAW_RANK))
        return;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nOldDrawTimes = CExtOperateLimit::GetLimitCount(OperateLimit, 1600);
    int nNewDrawTimes = nOldDrawTimes + nCount;
    CExtOperateLimit::SetLimitCount(OperateLimit, 1600, nNewDrawTimes);
    
    // 更新排行�?
    int8_t connid = 0;
    auto it = m_mDrawRank.find(connid);
    if (it == m_mDrawRank.end())
    {
        std::vector<FestivalRank> vRank;
        m_mDrawRank[connid] = vRank;
        it = m_mDrawRank.find(connid);
    }
    
    bool bFound = false;
    for (auto& rank : it->second)
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nNewDrawTimes;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nNewDrawTimes;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second.push_back(newRank);
    }
    
    // 排序
    std::sort(it->second.begin(), it->second.end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    // 保留�?00�?
    if (it->second.size() > 100)
        it->second.resize(100);
}

void CFestivalDoubleEleven::AddHuoYueDu(Player *player, int32_t nValue)
{
    if (player)
    {
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_HUO_YUE_DU))
        {
            CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
            CExtOperateLimit::AddLimitCount(OperateLimit, 1603, nValue);
            
            // 检查是否达到领取条�?
            int nTotal = CExtOperateLimit::GetLimitCount(OperateLimit, 1603);
            for (size_t i = 0; i < m_vHuoYueDuSumLimit.size(); i++)
            {
                if (nTotal >= m_vHuoYueDuSumLimit[i])
                {
                    // 标记可领�?
                    int nClaimed = CExtOperateLimit::GetLimitCount(OperateLimit, 1610);
                    if (!(nClaimed & (1 << i)))
                    {
                        // 可以领取
                    }
                }
            }
        }
    }
}

int32_t CFestivalDoubleEleven::getLeftTime()
{
    if (!IsInTime())
        return 0;
    
    CTimer *v2 = Answer::Singleton<CTimer>::instance();
    int nNowTime = CTimer::GetNow(v2);
    int nDayEndTime = Answer::DayTime::dayzero(nNowTime) + 86400;
    return nDayEndTime - nNowTime;
}

bool CFestivalDoubleEleven::canGetLandGift(Player *player)
{
    if (!player)
        return false;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_LAND))
        return false;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    return CExtOperateLimit::GetLimitCount(OperateLimit, 2030) <= 0;
}

bool CFestivalDoubleEleven::canGetLandSumGift(Player *player)
{
    if (!player)
        return false;
    
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM))
        return false;
    
    int32_t nLandDays = m_nDay - m_vStartDay[FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM] + 1;
    if (nLandDays < 0) nLandDays = 0;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nRecord = CExtOperateLimit::GetLimitCount(OperateLimit, 2031);
    
    for (int32_t i = 0; i < (int32_t)m_vLandSumGift.size(); i++)
    {
        if (nLandDays >= i + 1 && (nRecord & (1 << i)) == 0)
            return true;
    }
    
    return false;
}

bool CFestivalDoubleEleven::canGetLandSumGift(Player *player, int8_t nIndex)
{
    if (!player)
        return false;
    
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM))
        return false;
    
    if (nIndex < 0 || nIndex >= (int8_t)m_vLandSumGift.size())
        return false;
    
    int32_t nLandDays = m_nDay - m_vStartDay[FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM] + 1;
    if (nLandDays < 0) nLandDays = 0;
    
    if (nLandDays < nIndex + 1)
        return false;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nRecord = CExtOperateLimit::GetLimitCount(OperateLimit, 2031);
    
    return (nRecord & (1 << nIndex)) == 0;
}

bool CFestivalDoubleEleven::canGetDrawGift(Player *player)
{
    if (!player)
        return false;
    
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_DRAW))
        return false;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int32_t nDrawTimes = CExtOperateLimit::GetLimitCount(OperateLimit, 1601);
    int nRewardTimes = CExtOperateLimit::GetLimitCount(OperateLimit, 1602);
    
    return nDrawTimes > nRewardTimes && nDrawTimes > 0;
}

bool CFestivalDoubleEleven::canGetOnlineGift(Player *player)
{
    if (!player)
        return false;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_ONLINE_TIME))
        return false;
    
    int32_t nOnlineTime = Player::GetOnlineTime(player);
    int nRewardTimes = GetOnlineTimeRewardTime(player);
    
    if (nRewardTimes >= (int)m_vOnlineReward.size())
        return false;
    
    int nNeedTime = m_vOnlineReward[nRewardTimes].nNeedTime;
    return nOnlineTime >= nNeedTime;
}

int32_t CFestivalDoubleEleven::GetOnlineTimeRewardTime(Player *player)
{
    if (!player)
        return 0;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nRewardTimes = CExtOperateLimit::GetLimitCount(OperateLimit, 1604);
    
    return nRewardTimes;
}

bool CFestivalDoubleEleven::canGetWishGift(Player *player)
{
    if (!player)
        return false;
    
    if (!IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_BEST_WISH))
        return false;
    
    int32_t nRecharge = Player::GetTotalRecharge(player);
    int32_t nReward = Player::getRecord(player, 1615);
    
    return nRecharge >= m_nWishNeedRecharge && nReward == 0;
}

bool CFestivalDoubleEleven::canGetFaBaoBack(Player *player)
{
    if (!player)
        return false;
    
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_FA_BAO_BACK))
        return false;
    
    // 检查法宝返还条�?
    // 需要根据玩家升星的法宝数量计算
    return false;
}

bool CFestivalDoubleEleven::canGetEquipUpStarBack(Player *player)
{
    if (!player)
        return false;
    
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_EQUIP_UPSTAR_BACK))
        return false;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    return CExtOperateLimit::GetLimitCount(OperateLimit, 1617) > 0;
}

bool CFestivalDoubleEleven::canGetRechargeBack(Player *player)
{
    if (!player)
        return false;
    
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_RECHARGE_BACK))
        return false;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    return CExtOperateLimit::GetLimitCount(OperateLimit, 1620) > 0;
}

int32_t CFestivalDoubleEleven::getLandSum(Player *player)
{
    int32_t nEndDay = m_vEndDay[FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM];
    int32_t nStart = m_vStartDay[FESTIVAL_ACTIVITY_TYPE::FAT_LAND_SUM];
    
    int nCount = 0;
    for (int32_t i = nStart; i <= m_nDay && i <= nEndDay; i++)
    {
        nCount++;
    }
    
    return nCount;
}

void CFestivalDoubleEleven::getIconState(ShowIcon* stu, CFestivalDoubleEleven* thiz, Player* player)
{
    if (!stu || !thiz || !player) return;
    
    stu->nIconId = thiz->m_nIcon;
    stu->nState = 1; // 显示
    
    // 检查是否有可领取奖�?
    if (thiz->canGetLandGift(player) ||
        thiz->canGetDrawGift(player) ||
        thiz->canGetOnlineGift(player) ||
        thiz->canGetWishGift(player))
    {
        stu->nState = 2; // 红点提示
    }
}

void CFestivalDoubleEleven::SendWorldBossIconState()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    ShowIcon stu;
    
    if (GameService::getLine(v1) == 1)
    {
        Answer::NetPacket *packet = Answer::NetPacket::Create();
        packet->WriteInt(53); // 世界BOSS图标ID
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_WORLD_BOSS))
        {
            // 检查当前是否在世界BOSS时间�?
            bool bInBossTime = false;
            for (size_t i = 0; i < m_vWorldBossMinute.size(); i++)
            {
                if (m_nMinute >= m_vWorldBossMinute[i].nStart && m_nMinute < m_vWorldBossMinute[i].nEnd)
                {
                    bInBossTime = true;
                    break;
                }
            }
            packet->WriteChar(bInBossTime ? 1 : 0);
        }
        else
        {
            packet->WriteChar(0);
        }
        GameService::BroadcastToAll(v1, packet);
        Answer::NetPacket::Release(packet);
    }
}

void CFestivalDoubleEleven::checkWorldBoss()
{
    for (size_t i = 0; i < m_vWorldBossMinute.size(); i++)
    {
        TimeArea& area = m_vWorldBossMinute[i];
        if (m_nMinute >= area.nStart && m_nMinute < area.nEnd)
        {
            if (m_bDie == 1 && m_MonsterId == 0)
            {
                // 开始世界BOSS
                // StartWorldBoss();
                broadcastWorldBossStart();
                m_bDie = 0;
            }
        }
        else
        {
            if (m_bDie == 0 && m_MonsterId != 0)
            {
                // 结束世界BOSS
                broadcastWorldBossEnd();
                m_bDie = 1;
                m_MonsterId = 0;
            }
        }
    }
}

void CFestivalDoubleEleven::broadcastWorldBossStart()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(v1) == 1)
    {
        Answer::NetPacket *packet = Answer::NetPacket::Create();
        packet->WriteInt(1); // 开始事�?
        packet->WriteInt(m_MonsterId);
        GameService::BroadcastToAll(v1, packet);
        Answer::NetPacket::Release(packet);
    }
}

void CFestivalDoubleEleven::broadcastWorldBossEnd()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(v1) == 1)
    {
        Answer::NetPacket *packet = Answer::NetPacket::Create();
        packet->WriteInt(2); // 结束事件
        GameService::BroadcastToAll(v1, packet);
        Answer::NetPacket::Release(packet);
    }
}

void CFestivalDoubleEleven::AddOnlineRecord(Player *player)
{
    if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_ONLINE_TIME) && player)
    {
        int32_t nOnlineTime = Player::GetOnlineTime(player);
        int nRewardTimes = GetOnlineTimeRewardTime(player);
        
        if (nRewardTimes < (int)m_vOnlineReward.size())
        {
            int nNeedTime = m_vOnlineReward[nRewardTimes].nNeedTime;
            if (nOnlineTime >= nNeedTime)
            {
                // 自动发放奖励或标记可领取
                // 这里可以发送通知给客户端，让客户端可以领�?
            }
        }
    }
}

void CFestivalDoubleEleven::UpdateXiaoFeiRank(Player *player)
{
    if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_DAILY_XIAO_FEI_RANK))
        updateXiaoFeiRank(player);
    
    if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_XIAO_FEI_SUM_RANK))
        updateXiaoFeiSumRank(player);
    
    if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CROSS_XIAO_FEI_SUM_RANK))
        updateCrossXiaoFeiSumRank(player);
}

void CFestivalDoubleEleven::AddXiaoFeiRecord(Player *player, int32_t nCount)
{
    if (player && IsInTime())
    {
        int32_t nOldCount = GetXiaoFeiSumGiftCount(player);
        CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(OperateLimit, 1606, nCount);
        
        UpdateXiaoFeiRank(player);
        
        int32_t nNewCount = GetXiaoFeiSumGiftCount(player);
        if (nNewCount > nOldCount)
        {
            // 发放消费返利奖励
            for (int32_t i = nOldCount; i < nNewCount && i < (int32_t)m_vXiaoFeiSumReward.size(); i++)
            {
                CExtCharBag* Bag = Player::GetBag(player);
                CExtCharBag::AddItem(Bag, &m_vXiaoFeiSumReward[i], ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_XIAO_FEI_SUM);
            }
        }
    }
}

int32_t CFestivalDoubleEleven::GetXiaoFeiSumGiftCount(Player *player)
{
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int32_t Count = CExtOperateLimit::GetLimitCount(OperateLimit, 1606);
    int nRewardSum = 0;
    
    for (int32_t i = 0; i < (int32_t)m_vXiaoFeiSumLimit.size(); i++)
    {
        if (Count >= m_vXiaoFeiSumLimit[i])
            nRewardSum++;
    }
    
    int nClaimed = CExtOperateLimit::GetLimitCount(OperateLimit, 1607);
    return nRewardSum - nClaimed;
}

void CFestivalDoubleEleven::AddFriendQingYi(Player *player, int32_t nValue)
{
    if (player)
    {
        if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_FRIEND_QING_YI))
        {
            CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
            int nOldValue = CExtOperateLimit::GetLimitCount(OperateLimit, 1624);
            CExtOperateLimit::AddLimitCount(OperateLimit, 1624, nValue);
            int nNewValue = nOldValue + nValue;
            
            // 检查并发放亲友值奖�?
            for (size_t i = 0; i < m_vFriendQingYiLimit.size(); i++)
            {
                if (nOldValue < m_vFriendQingYiLimit[i] && nNewValue >= m_vFriendQingYiLimit[i])
                {
                    // 发放奖励
                    CExtCharBag* Bag = Player::GetBag(player);
                    if (i < m_vFriendQingYiReward.size())
                    {
                        CExtCharBag::AddItem(Bag, &m_vFriendQingYiReward[i], ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_FRIEND_QING_YI);
                    }
                }
            }
        }
    }
}

bool CFestivalDoubleEleven::CanUseXiaoFeiDraw(Player *player)
{
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_XIAO_FEI_DRAW))
        return false;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nXiaoFeiSum = CExtOperateLimit::GetLimitCount(OperateLimit, 1606);
    int32_t nRewardTimes = CExtOperateLimit::GetLimitCount(OperateLimit, 1608);
    int32_t nCanGetTime = 0;
    
    for (int32_t i = 0; i < (int32_t)m_nXiaoFeiDrawValue.size(); i++)
    {
        if (nXiaoFeiSum >= m_nXiaoFeiDrawValue[i])
            nCanGetTime++;
    }
    
    return nCanGetTime > nRewardTimes;
}

bool CFestivalDoubleEleven::CanUseRechargeDraw(Player *player)
{
    if (!IsOpen(FESTIVAL_ACTIVITY_TYPE::FAT_RECHARGE_DRAW))
        return false;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nChongZhi = Player::GetTotalRecharge(player);
    int32_t nRewardTimes = CExtOperateLimit::GetLimitCount(OperateLimit, 1609);
    int32_t nCanGetTime = 0;
    
    for (int32_t i = 0; i < (int32_t)m_nRechargeDrawValue.size(); i++)
    {
        if (nChongZhi >= m_nRechargeDrawValue[i])
            nCanGetTime++;
    }
    
    return nCanGetTime > nRewardTimes;
}

void CFestivalDoubleEleven::loadXiaoFeiRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, sub_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=2 ORDER BY rank_value DESC");
    
    m_mXiaoFeiRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            int nSubType = Answer::MySqlQuery::getInt(&result, 2);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 3);
            const char* name = Answer::MySqlQuery::getString(&result, 4);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            auto it = m_mXiaoFeiRank.find(rank.nRankType);
            if (it == m_mXiaoFeiRank.end())
            {
                std::vector<std::vector<FestivalRank>> vRank;
                vRank.resize(m_vXiaoFeiRankLimit.size());
                m_mXiaoFeiRank[rank.nRankType] = vRank;
                it = m_mXiaoFeiRank.find(rank.nRankType);
            }
            if (nSubType >= 0 && nSubType < (int)it->second.size())
            {
                it->second[nSubType].push_back(rank);
            }
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::updateXiaoFeiRank(Player *player)
{
    if (!player) return;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nXiaoFei = CExtOperateLimit::GetLimitCount(OperateLimit, 1606);
    
    int nDayIndex = m_nDay - m_vStartDay[FESTIVAL_ACTIVITY_TYPE::FAT_DAILY_XIAO_FEI_RANK];
    if (nDayIndex < 0 || nDayIndex >= (int)m_vXiaoFeiRankLimit.size())
        return;
    
    int8_t connid = 0;
    auto it = m_mXiaoFeiRank.find(connid);
    if (it == m_mXiaoFeiRank.end())
    {
        std::vector<std::vector<FestivalRank>> vRank;
        vRank.resize(m_vXiaoFeiRankLimit.size());
        m_mXiaoFeiRank[connid] = vRank;
        it = m_mXiaoFeiRank.find(connid);
    }
    
    if (nDayIndex >= (int)it->second.size())
        return;
    
    bool bFound = false;
    for (auto& rank : it->second[nDayIndex])
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nXiaoFei;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nXiaoFei;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second[nDayIndex].push_back(newRank);
    }
    
    std::sort(it->second[nDayIndex].begin(), it->second[nDayIndex].end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    if (it->second[nDayIndex].size() > m_vXiaoFeiRankLimit[nDayIndex])
        it->second[nDayIndex].resize(m_vXiaoFeiRankLimit[nDayIndex]);
}

void CFestivalDoubleEleven::checkXiaoFeiRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mXiaoFeiRank.find(connid);
    if (it != m_mXiaoFeiRank.end())
    {
        for (auto& dayRank : it->second)
        {
            for (auto it2 = dayRank.begin(); it2 != dayRank.end(); )
            {
                if (!GameService::IsPlayerExist(it2->nCharId))
                    it2 = dayRank.erase(it2);
                else
                    ++it2;
            }
        }
    }
}

void CFestivalDoubleEleven::checkXiaoFeiRank()
{
    for (auto& kv : m_mXiaoFeiRank)
    {
        for (size_t day = 0; day < kv.second.size(); day++)
        {
            for (auto& rank : kv.second[day])
            {
                char szSQL[1024];
                snprintf(szSQL, sizeof(szSQL),
                         "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, sub_type, rank_value, char_name, update_time) VALUES (%d, 2, %d, %d, %d, '%s', %d)",
                         rank.nCharId, kv.first, (int)day, rank.nValue, rank.szName, (int)time(NULL));
                Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
                Answer::MySqlDBGuard db;
                Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
                Answer::MySqlDBGuard::excute(&db, szSQL);
                Answer::MySqlDBGuard::~MySqlDBGuard(&db);
            }
        }
    }
}

void CFestivalDoubleEleven::sendXiaoFeiRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mXiaoFeiRank)
    {
        for (size_t day = 0; day < kv.second.size(); day++)
        {
            int nRank = 1;
            for (auto& rank : kv.second[day])
            {
                int rewardIndex = nRank - 1;
                if (rewardIndex < (int)m_vXiaoFeiRankReward.size())
                {
                    char szTitle[256];
                    snprintf(szTitle, sizeof(szTitle), "每日消费排行榜奖�?- �?d�?, nRank);
                    // MailSystem::SendMail(rank.nCharId, m_vXiaoFeiRankMail[0], szTitle, "", m_vXiaoFeiRankReward[rewardIndex]);
                }
                nRank++;
            }
        }
    }
}

void CFestivalDoubleEleven::loadXiaoFeiSumRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=3 ORDER BY rank_value DESC LIMIT 100");
    
    m_mXiaoFeiSumRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 2);
            const char* name = Answer::MySqlQuery::getString(&result, 3);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            m_mXiaoFeiSumRank[rank.nRankType].push_back(rank);
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::updateXiaoFeiSumRank(Player *player)
{
    if (!player) return;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nTotalXiaoFei = CExtOperateLimit::GetLimitCount(OperateLimit, 1606);
    
    int8_t connid = 0;
    auto it = m_mXiaoFeiSumRank.find(connid);
    if (it == m_mXiaoFeiSumRank.end())
    {
        std::vector<FestivalRank> vRank;
        m_mXiaoFeiSumRank[connid] = vRank;
        it = m_mXiaoFeiSumRank.find(connid);
    }
    
    bool bFound = false;
    for (auto& rank : it->second)
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nTotalXiaoFei;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nTotalXiaoFei;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second.push_back(newRank);
    }
    
    std::sort(it->second.begin(), it->second.end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    int nLimit = 100;
    if (m_vXiaoFeiSumRankLimit.size() > 0) nLimit = m_vXiaoFeiSumRankLimit[0];
    if (it->second.size() > (size_t)nLimit)
        it->second.resize(nLimit);
}

void CFestivalDoubleEleven::checkXiaoFeiSumRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mXiaoFeiSumRank.find(connid);
    if (it != m_mXiaoFeiSumRank.end())
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); )
        {
            if (!GameService::IsPlayerExist(it2->nCharId))
                it2 = it->second.erase(it2);
            else
                ++it2;
        }
    }
}

void CFestivalDoubleEleven::checkXiaoFeiSumRank()
{
    for (auto& kv : m_mXiaoFeiSumRank)
    {
        for (auto& rank : kv.second)
        {
            char szSQL[1024];
            snprintf(szSQL, sizeof(szSQL),
                     "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, rank_value, char_name, update_time) VALUES (%d, 3, %d, %d, '%s', %d)",
                     rank.nCharId, kv.first, rank.nValue, rank.szName, (int)time(NULL));
            Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
            Answer::MySqlDBGuard db;
            Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
            Answer::MySqlDBGuard::excute(&db, szSQL);
            Answer::MySqlDBGuard::~MySqlDBGuard(&db);
        }
    }
}

void CFestivalDoubleEleven::sendXiaoFeiSumRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mXiaoFeiSumRank)
    {
        int nRank = 1;
        for (auto& rank : kv.second)
        {
            int rewardIndex = nRank - 1;
            if (rewardIndex < (int)m_vXiaoFeiSumRankReward.size())
            {
                char szTitle[256];
                snprintf(szTitle, sizeof(szTitle), "累计消费排行榜奖�?- �?d�?, nRank);
                // MailSystem::SendMail(rank.nCharId, m_vXiaoFeiSumRankMail[0], szTitle, "", m_vXiaoFeiSumRankReward[rewardIndex]);
            }
            nRank++;
        }
    }
}

void CFestivalDoubleEleven::loadRechargeSumRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=4 ORDER BY rank_value DESC LIMIT 100");
    
    m_mRechargeSumRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 2);
            const char* name = Answer::MySqlQuery::getString(&result, 3);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            m_mRechargeSumRank[rank.nRankType].push_back(rank);
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::updateRechargeSumRank(Player *player)
{
    if (!player) return;
    
    int nTotalRecharge = Player::GetTotalRecharge(player);
    
    int8_t connid = 0;
    auto it = m_mRechargeSumRank.find(connid);
    if (it == m_mRechargeSumRank.end())
    {
        std::vector<FestivalRank> vRank;
        m_mRechargeSumRank[connid] = vRank;
        it = m_mRechargeSumRank.find(connid);
    }
    
    bool bFound = false;
    for (auto& rank : it->second)
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nTotalRecharge;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nTotalRecharge;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second.push_back(newRank);
    }
    
    std::sort(it->second.begin(), it->second.end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    int nLimit = 100;
    if (m_vRechargeSumRankLimit.size() > 0) nLimit = m_vRechargeSumRankLimit[0];
    if (it->second.size() > (size_t)nLimit)
        it->second.resize(nLimit);
}

void CFestivalDoubleEleven::checkRechargeSumRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mRechargeSumRank.find(connid);
    if (it != m_mRechargeSumRank.end())
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); )
        {
            if (!GameService::IsPlayerExist(it2->nCharId))
                it2 = it->second.erase(it2);
            else
                ++it2;
        }
    }
}

void CFestivalDoubleEleven::checkRechargeSumRank()
{
    for (auto& kv : m_mRechargeSumRank)
    {
        for (auto& rank : kv.second)
        {
            char szSQL[1024];
            snprintf(szSQL, sizeof(szSQL),
                     "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, rank_value, char_name, update_time) VALUES (%d, 4, %d, %d, '%s', %d)",
                     rank.nCharId, kv.first, rank.nValue, rank.szName, (int)time(NULL));
            Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
            Answer::MySqlDBGuard db;
            Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
            Answer::MySqlDBGuard::excute(&db, szSQL);
            Answer::MySqlDBGuard::~MySqlDBGuard(&db);
        }
    }
}

void CFestivalDoubleEleven::sendRechargeSumRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mRechargeSumRank)
    {
        int nRank = 1;
        for (auto& rank : kv.second)
        {
            int rewardIndex = nRank - 1;
            if (rewardIndex < (int)m_vRechargeSumRankReward.size())
            {
                char szTitle[256];
                snprintf(szTitle, sizeof(szTitle), "累计充值排行榜奖励 - �?d�?, nRank);
                // MailSystem::SendMail(rank.nCharId, m_vRechargeSumRankMail[0], szTitle, "", m_vRechargeSumRankReward[rewardIndex]);
            }
            nRank++;
        }
    }
}

void CFestivalDoubleEleven::BroadCastMoYuShiJieIconState()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    
    if (GameService::getLine(v1) == 1)
    {
        Answer::NetPacket *packet = Answer::NetPacket::Create();
        packet->WriteInt(54); // 魔域世界图标ID
        if (m_nDay >= m_vStartDay[14] && m_nDay < m_vEndDay[14])
        {
            packet->WriteChar(1); // 显示
        }
        else
        {
            packet->WriteChar(0); // 隐藏
        }
        GameService::BroadcastToAll(v1, packet);
        Answer::NetPacket::Release(packet);
    }
}

bool CFestivalDoubleEleven::RefreshMysteryShopItem(Player *player)
{
    if (!player)
        return false;
    
    if (m_sMysteryShopRefreshCost.m_nClass == 4)
    {
        // 消耗货币刷�?
        // if (!Player::DecCurrency(player, (CURRENCY_TYPE)m_sMysteryShopRefreshCost.m_nType, m_sMysteryShopRefreshCost.m_nCount))
        //     return false;
    }
    
    // 刷新神秘商店商品
    // refreshMysteryShopItems();
    
    return true;
}

void CFestivalDoubleEleven::checkRefreshMysteryShop()
{
    if (!(m_nMinute % 120))
    {
        broadcastRefreshMysteryShop();
    }
}

void CFestivalDoubleEleven::broadcastRefreshMysteryShop()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    if (GameService::getLine(v1) == 1)
    {
        Answer::NetPacket *packet = Answer::NetPacket::Create();
        packet->WriteInt(55); // 神秘商店刷新事件
        GameService::BroadcastToAll(v1, packet);
        Answer::NetPacket::Release(packet);
    }
}

int32_t CFestivalDoubleEleven::CalBossScoreAddValue(int32_t nValue)
{
    if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_BOSS_SCORE))
        return nValue * m_nBossScoreRate / 100;
    else
        return nValue;
}

int32_t CFestivalDoubleEleven::calRechargeBack(int32_t nValue)
{
    int32_t nBackValue = 0;
    int32_t nResult;
    int32_t remaining = nValue;
    
    for (int i = 0; i < (int)m_vRechargeBackLimit.size() && remaining > 0; i++)
    {
        nResult = calRechargeBackHelper(&remaining);
        nBackValue += nResult;
    }
    
    return nBackValue;
}

int32_t CFestivalDoubleEleven::calRechargeBackHelper(int32_t *const nValue)
{
    for (int i = 0; i < m_nRechargeBackSize && i < (int)m_vRechargeBackLimit.size(); i++)
    {
        if (*nValue >= m_vRechargeBackLimit[i])
        {
            int v3 = m_vRechargeBackValue[i];
            *nValue -= m_vRechargeBackLimit[i];
            return v3;
        }
    }
    
    return 0;
}

void CFestivalDoubleEleven::loadCrossRechargeSumRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=5 ORDER BY rank_value DESC LIMIT 100");
    
    m_mCrossRechargeSumRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 2);
            const char* name = Answer::MySqlQuery::getString(&result, 3);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            m_mCrossRechargeSumRank[rank.nRankType].push_back(rank);
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::updateCrossRechargeSumRank(Player *player)
{
    if (!player) return;
    
    int nTotalRecharge = Player::GetTotalRecharge(player);
    
    int8_t connid = 0;
    auto it = m_mCrossRechargeSumRank.find(connid);
    if (it == m_mCrossRechargeSumRank.end())
    {
        std::vector<FestivalRank> vRank;
        m_mCrossRechargeSumRank[connid] = vRank;
        it = m_mCrossRechargeSumRank.find(connid);
    }
    
    bool bFound = false;
    for (auto& rank : it->second)
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nTotalRecharge;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nTotalRecharge;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second.push_back(newRank);
    }
    
    std::sort(it->second.begin(), it->second.end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    int nLimit = 100;
    if (m_vCrossRechargeSumRankLimit.size() > 0) nLimit = m_vCrossRechargeSumRankLimit[0];
    if (it->second.size() > (size_t)nLimit)
        it->second.resize(nLimit);
}

void CFestivalDoubleEleven::checkCrossRechargeSumRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mCrossRechargeSumRank.find(connid);
    if (it != m_mCrossRechargeSumRank.end())
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); )
        {
            if (!GameService::IsPlayerExist(it2->nCharId))
                it2 = it->second.erase(it2);
            else
                ++it2;
        }
    }
}

void CFestivalDoubleEleven::checkCrossRechargeSumRank()
{
    for (auto& kv : m_mCrossRechargeSumRank)
    {
        for (auto& rank : kv.second)
        {
            char szSQL[1024];
            snprintf(szSQL, sizeof(szSQL),
                     "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, rank_value, char_name, update_time) VALUES (%d, 5, %d, %d, '%s', %d)",
                     rank.nCharId, kv.first, rank.nValue, rank.szName, (int)time(NULL));
            Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
            Answer::MySqlDBGuard db;
            Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
            Answer::MySqlDBGuard::excute(&db, szSQL);
            Answer::MySqlDBGuard::~MySqlDBGuard(&db);
        }
    }
}

void CFestivalDoubleEleven::sendCrossRechargeSumRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mCrossRechargeSumRank)
    {
        int nRank = 1;
        for (auto& rank : kv.second)
        {
            int rewardIndex = nRank - 1;
            if (rewardIndex < (int)m_vCrossRechargeSumRankReward.size())
            {
                char szTitle[256];
                snprintf(szTitle, sizeof(szTitle), "跨服累计充值排行榜奖励 - �?d�?, nRank);
                // MailSystem::SendMail(rank.nCharId, m_vCrossRechargeSumRankMail[0], szTitle, "", m_vCrossRechargeSumRankReward[rewardIndex]);
            }
            nRank++;
        }
    }
}

void CFestivalDoubleEleven::loadCrossXiaoFeiSumRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=6 ORDER BY rank_value DESC LIMIT 100");
    
    m_mCrossXiaoFeiSumRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 2);
            const char* name = Answer::MySqlQuery::getString(&result, 3);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            m_mCrossXiaoFeiSumRank[rank.nRankType].push_back(rank);
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::updateCrossXiaoFeiSumRank(Player *player)
{
    if (!player) return;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nTotalXiaoFei = CExtOperateLimit::GetLimitCount(OperateLimit, 1606);
    
    int8_t connid = 0;
    auto it = m_mCrossXiaoFeiSumRank.find(connid);
    if (it == m_mCrossXiaoFeiSumRank.end())
    {
        std::vector<FestivalRank> vRank;
        m_mCrossXiaoFeiSumRank[connid] = vRank;
        it = m_mCrossXiaoFeiSumRank.find(connid);
    }
    
    bool bFound = false;
    for (auto& rank : it->second)
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nTotalXiaoFei;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nTotalXiaoFei;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second.push_back(newRank);
    }
    
    std::sort(it->second.begin(), it->second.end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    int nLimit = 100;
    if (m_vCrossXiaoFeiSumRankLimit.size() > 0) nLimit = m_vCrossXiaoFeiSumRankLimit[0];
    if (it->second.size() > (size_t)nLimit)
        it->second.resize(nLimit);
}

void CFestivalDoubleEleven::checkCrossXiaoFeiSumRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mCrossXiaoFeiSumRank.find(connid);
    if (it != m_mCrossXiaoFeiSumRank.end())
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); )
        {
            if (!GameService::IsPlayerExist(it2->nCharId))
                it2 = it->second.erase(it2);
            else
                ++it2;
        }
    }
}

void CFestivalDoubleEleven::checkCrossXiaoFeiSumRank()
{
    for (auto& kv : m_mCrossXiaoFeiSumRank)
    {
        for (auto& rank : kv.second)
        {
            char szSQL[1024];
            snprintf(szSQL, sizeof(szSQL),
                     "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, rank_value, char_name, update_time) VALUES (%d, 6, %d, %d, '%s', %d)",
                     rank.nCharId, kv.first, rank.nValue, rank.szName, (int)time(NULL));
            Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
            Answer::MySqlDBGuard db;
            Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
            Answer::MySqlDBGuard::excute(&db, szSQL);
            Answer::MySqlDBGuard::~MySqlDBGuard(&db);
        }
    }
}

void CFestivalDoubleEleven::sendCrossXiaoFeiSumRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mCrossXiaoFeiSumRank)
    {
        int nRank = 1;
        for (auto& rank : kv.second)
        {
            int rewardIndex = nRank - 1;
            if (rewardIndex < (int)m_vCrossXiaoFeiSumRankReward.size())
            {
                char szTitle[256];
                snprintf(szTitle, sizeof(szTitle), "跨服累计消费排行榜奖�?- �?d�?, nRank);
                // MailSystem::SendMail(rank.nCharId, m_vCrossXiaoFeiSumRankMail[0], szTitle, "", m_vCrossXiaoFeiSumRankReward[rewardIndex]);
            }
            nRank++;
        }
    }
}

void CFestivalDoubleEleven::loadCrossChouJiangRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=7 ORDER BY rank_value DESC LIMIT 100");
    
    m_mCrossChouJiangRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 2);
            const char* name = Answer::MySqlQuery::getString(&result, 3);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            m_mCrossChouJiangRank[rank.nRankType].push_back(rank);
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::updateCrossChouJiangRank(Player *player)
{
    if (!player) return;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nDrawTimes = CExtOperateLimit::GetLimitCount(OperateLimit, 1600);
    
    int8_t connid = 0;
    auto it = m_mCrossChouJiangRank.find(connid);
    if (it == m_mCrossChouJiangRank.end())
    {
        std::vector<FestivalRank> vRank;
        m_mCrossChouJiangRank[connid] = vRank;
        it = m_mCrossChouJiangRank.find(connid);
    }
    
    bool bFound = false;
    for (auto& rank : it->second)
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nDrawTimes;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nDrawTimes;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second.push_back(newRank);
    }
    
    std::sort(it->second.begin(), it->second.end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    int nLimit = 100;
    if (m_vCrossChouJiangRankLimit.size() > 0) nLimit = m_vCrossChouJiangRankLimit[0];
    if (it->second.size() > (size_t)nLimit)
        it->second.resize(nLimit);
}

void CFestivalDoubleEleven::checkCrossChouJiangRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mCrossChouJiangRank.find(connid);
    if (it != m_mCrossChouJiangRank.end())
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); )
        {
            if (!GameService::IsPlayerExist(it2->nCharId))
                it2 = it->second.erase(it2);
            else
                ++it2;
        }
    }
}

void CFestivalDoubleEleven::checkCrossChouJiangRank()
{
    for (auto& kv : m_mCrossChouJiangRank)
    {
        for (auto& rank : kv.second)
        {
            char szSQL[1024];
            snprintf(szSQL, sizeof(szSQL),
                     "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, rank_value, char_name, update_time) VALUES (%d, 7, %d, %d, '%s', %d)",
                     rank.nCharId, kv.first, rank.nValue, rank.szName, (int)time(NULL));
            Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
            Answer::MySqlDBGuard db;
            Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
            Answer::MySqlDBGuard::excute(&db, szSQL);
            Answer::MySqlDBGuard::~MySqlDBGuard(&db);
        }
    }
}

void CFestivalDoubleEleven::sendCrossChouJiangRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mCrossChouJiangRank)
    {
        int nRank = 1;
        for (auto& rank : kv.second)
        {
            int rewardIndex = nRank - 1;
            if (rewardIndex < (int)m_vCrossChouJiangRankReward.size())
            {
                char szTitle[256];
                snprintf(szTitle, sizeof(szTitle), "跨服抽奖排行榜奖�?- �?d�?, nRank);
                // MailSystem::SendMail(rank.nCharId, m_vCrossChouJiangRankMail[0], szTitle, "", m_vCrossChouJiangRankReward[rewardIndex]);
            }
            nRank++;
        }
    }
}

void CFestivalDoubleEleven::loadBossScoreRank()
{
    Answer::DBPool *v1 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlDBGuard db;
    Answer::MySqlQuery result;
    char szSQL[4096];
    
    Answer::MySqlDBGuard::MySqlDBGuard(&db, v1);
    snprintf(szSQL, sizeof(szSQL),
             "SELECT char_id, rank_type, rank_value, char_name FROM mem_double_eleven_rank WHERE activity_type=8 ORDER BY rank_value DESC LIMIT 100");
    
    m_mBossScoreRank.clear();
    if (Answer::MySqlDBGuard::query(&db, &result, szSQL))
    {
        while (Answer::MySqlQuery::next(&result))
        {
            FestivalRank rank;
            rank.nCharId = Answer::MySqlQuery::getInt(&result, 0);
            rank.nRankType = (int8_t)Answer::MySqlQuery::getInt(&result, 1);
            rank.nValue = Answer::MySqlQuery::getInt(&result, 2);
            const char* name = Answer::MySqlQuery::getString(&result, 3);
            if (name) strncpy(rank.szName, name, sizeof(rank.szName) - 1);
            else rank.szName[0] = '\0';
            
            m_mBossScoreRank[rank.nRankType].push_back(rank);
        }
    }
    
    Answer::MySqlQuery::~MySqlQuery(&result);
    Answer::MySqlDBGuard::~MySqlDBGuard(&db);
}

void CFestivalDoubleEleven::updateBossScoreRank(Player *player)
{
    if (!player) return;
    
    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(player);
    int nScore = CExtOperateLimit::GetLimitCount(OperateLimit, 1625);
    
    int8_t connid = 0;
    auto it = m_mBossScoreRank.find(connid);
    if (it == m_mBossScoreRank.end())
    {
        std::vector<FestivalRank> vRank;
        m_mBossScoreRank[connid] = vRank;
        it = m_mBossScoreRank.find(connid);
    }
    
    bool bFound = false;
    for (auto& rank : it->second)
    {
        if (rank.nCharId == player->GetCharId())
        {
            rank.nValue = nScore;
            bFound = true;
            break;
        }
    }
    
    if (!bFound)
    {
        FestivalRank newRank;
        newRank.nCharId = player->GetCharId();
        newRank.nValue = nScore;
        strncpy(newRank.szName, player->GetName(), sizeof(newRank.szName) - 1);
        it->second.push_back(newRank);
    }
    
    std::sort(it->second.begin(), it->second.end(),
              [](const FestivalRank& a, const FestivalRank& b) { return a.nValue > b.nValue; });
    
    int nLimit = 100;
    if (m_vBossScoreRankLimit.size() > 0) nLimit = m_vBossScoreRankLimit[0];
    if (it->second.size() > (size_t)nLimit)
        it->second.resize(nLimit);
}

void CFestivalDoubleEleven::checkBossScoreRankInvalid(int8_t connid)
{
    Answer::MutexGuard lock(&m_lock);
    
    auto it = m_mBossScoreRank.find(connid);
    if (it != m_mBossScoreRank.end())
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); )
        {
            if (!GameService::IsPlayerExist(it2->nCharId))
                it2 = it->second.erase(it2);
            else
                ++it2;
        }
    }
}

void CFestivalDoubleEleven::checkBossScoreRank()
{
    for (auto& kv : m_mBossScoreRank)
    {
        for (auto& rank : kv.second)
        {
            char szSQL[1024];
            snprintf(szSQL, sizeof(szSQL),
                     "REPLACE INTO mem_double_eleven_rank (char_id, activity_type, rank_type, rank_value, char_name, update_time) VALUES (%d, 8, %d, %d, '%s', %d)",
                     rank.nCharId, kv.first, rank.nValue, rank.szName, (int)time(NULL));
            Answer::DBPool* pool = Answer::Singleton<Answer::DBPool>::instance();
            Answer::MySqlDBGuard db;
            Answer::MySqlDBGuard::MySqlDBGuard(&db, pool);
            Answer::MySqlDBGuard::excute(&db, szSQL);
            Answer::MySqlDBGuard::~MySqlDBGuard(&db);
        }
    }
}

void CFestivalDoubleEleven::sendBossScoreRankMail()
{
    Answer::MutexGuard lock(&m_lock);
    
    for (auto& kv : m_mBossScoreRank)
    {
        int nRank = 1;
        for (auto& rank : kv.second)
        {
            int rewardIndex = nRank - 1;
            if (rewardIndex < (int)m_vBossScoreRankReward.size())
            {
                char szTitle[256];
                snprintf(szTitle, sizeof(szTitle), "BOSS积分排行榜奖�?- �?d�?, nRank);
                // MailSystem::SendMail(rank.nCharId, m_vBossScoreRankMail[0], szTitle, "", m_vBossScoreRankReward[rewardIndex]);
            }
            nRank++;
        }
    }
}

int8_t CFestivalDoubleEleven::GetChouJiangType()
{
    if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CHOU_JIANG_RANK))
        return m_ChouJiangType;
    else
        return 0;
}

int32_t CFestivalDoubleEleven::GetChouJiangSpecialTime()
{
    if (IsInTime(FESTIVAL_ACTIVITY_TYPE::FAT_CHOU_JIANG_RANK))
        return m_ChouJiangCount;
    else
        return 0;
}

int32_t CFestivalDoubleEleven::GetScoreCanDrawTime(Player *pPlayer)
{
    if (!pPlayer)
        return 0;
    
    int32_t Count = 0;
    int Score = Player::getRecord(pPlayer, 1625);
    
    for (int32_t i = 0; i < (int32_t)m_vScoreLimit.size(); i++)
    {
        if (Score >= m_vScoreLimit[i])
            Count++;
    }
    
    int32_t nDrawTimes = Player::getRecord(pPlayer, 1626);
    
    return Count - nDrawTimes;
}

int32_t CFestivalDoubleEleven::OnRandScoreDrawItem(Player *pPlayer)
{
    if (!pPlayer) return 10002;
    
    int32_t nCanDrawTime = GetScoreCanDrawTime(pPlayer);
    if (nCanDrawTime <= 0) return 10002;
    
    // 检查每日限�?
    int nDailyDrawCount = Player::getRecord(pPlayer, 1627);
    if (m_nDailyRechargeDrawLimit.size() > 0 && nDailyDrawCount >= m_nDailyRechargeDrawLimit[0])
        return 10002;
    
    // 随机抽取物品
    int totalRate = 0;
    for (auto& item : m_vBossScoreDrawReward)
    {
        totalRate += item.nRate;
    }
    
    if (totalRate <= 0) return 10002;
    
    Answer::Random *v7 = Answer::Singleton<Answer::Random>::instance();
    int randValue = Answer::Random::Rand(v7) % totalRate;
    
    int curRate = 0;
    for (auto& item : m_vBossScoreDrawReward)
    {
        curRate += item.nRate;
        if (randValue < curRate)
        {
            CExtCharBag *Bag = Player::GetBag(pPlayer);
            MemChrBag reward;
            reward.nItemId = item.nItemId;
            reward.nCount = item.nCount;
            if (!CExtCharBag::AddItem(Bag, &reward, ITEM_CHANGE_REASON::ICR_DOUBLE_ELEVEN_SCORE_DRAW))
                return 10002;
            
            int nDrawTimes = Player::getRecord(pPlayer, 1626);
            Player::setRecord(pPlayer, 1626, nDrawTimes + 1);
            
            int nDailyDraw = Player::getRecord(pPlayer, 1627);
            Player::setRecord(pPlayer, 1627, nDailyDraw + 1);
            
            return 0;
        }
    }
    
    return 10002;
}

CFestivalDoubleEleven::GetHuoYueDuSumGift()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::AddEquipUpStarBack()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::UpdateMonsterScore()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::GetLianRechargeReward()
{
      int32_t Record;
}


CFestivalDoubleEleven::UpdateRank()
{
      switch ( nType )
}


CFestivalDoubleEleven::BossDie()
{
      if ( m_MonsterId > 0 && m_MonsterId == MonsterId )
}


CFestivalDoubleEleven::GetRechargeSumGift()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::updateDrawRank()
{
      std::vector<FestivalRank> *v4;
}


CFestivalDoubleEleven::GetDailyRechargeGift()
{
      int32_t TodayPayGold;
}


CFestivalDoubleEleven::BuyGiftShopItem2()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::SendDaTiReward()
{
      std::vector<MemChrBag> *v4;
}


CFestivalDoubleEleven::GetXiaoFeiSumGift()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::BroadcastWorldBossKilled()
{
      GameService *v3;
}


CFestivalDoubleEleven::GetFriendQingYiGift()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::AddFaBaoValue()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::OnRecharge()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::getMoYuShiJieIconState()
{
      *(_QWORD *)&retstr->nId = 0;
}


CFestivalDoubleEleven::AddMoYuShiJieDrop()
{
      std::vector<MemChrBag> *v3;
}


CFestivalDoubleEleven::GetEquipQingYiGift()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::AddPetIllusionItemRecord()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::GouWuChe()
{
      int32_t v3;
}


CFestivalDoubleEleven::GetFaBaoCritBackItem()
{
      int32_t v4;
}


CFestivalDoubleEleven::getWorldBossIconState()
{
      ShowIcon __x; // [rsp+10h] [rbp-20h] BYREF
}


CFestivalDoubleEleven::broadcastPlayerAction()
{
      GameService *v5;
}


CFestivalDoubleEleven::BuyGiftShopItem()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::AddRechargeRecord()
{
      CExtOperateLimit *OperateLimit;
}


CFestivalDoubleEleven::GetPetIllusionItemGift()
{
      CExtOperateLimit *OperateLimit;
}
