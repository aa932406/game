#include "Activity/DailyActivityData.h"
#include "Activity/DailyActivity.h"
#include "Network/NetPacket.h"
#include "Database/MySqlDBGuard.h"
#include "Database/MySqlQuery.h"
#include "Utility/StringUtility.h"
#include "Other/Logger.h"
#include <cstring>
#include <cstdio>

DailyActivityData::DailyActivityData()
{
    memset(&data, 0, sizeof(data));
    lstTodayInfo.clear();
    vSeachBackRecord[0].clear();
    vSeachBackRecord[1].clear();
    vSeachBackRecord[2].clear();
    OnlimeReward.clear();
    CleanUp();
}

DailyActivityData::~DailyActivityData()
{
    OnlimeReward.clear();
    vSeachBackRecord[2].clear();
    vSeachBackRecord[1].clear();
    vSeachBackRecord[0].clear();
    lstTodayInfo.clear();
}

void DailyActivityData::CleanUp()
{
    memset(&data, 0, sizeof(data));
    lstTodayInfo.clear();
    
    for (int i = 0; i <= 2; ++i)
    {
        vSeachBackRecord[i].clear();
    }
    
    OnlimeReward.clear();
}

void DailyActivityData::InitSignRewardInfo(const std::string& p_RewardString)
{
    if (p_RewardString.empty()) return;
    
    std::vector<std::string> RewardVector;
    StringUtility::split(RewardVector, p_RewardString, "|");
    
    for (const auto& item : RewardVector)
    {
        int32_t value = atoi(item.c_str());
        data.sign_reward.push_back(static_cast<int8_t>(value));
    }
}

void DailyActivityData::LoadFromDB(Answer::MySqlDBGuard* db, int64_t nCid)
{
    char szSQL[4096];
    
    // 加载签到信息
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL) - 1, 
             "SELECT * FROM `mem_chr_sign_info` WHERE `cid`=%lld", nCid);
    
    Answer::MySqlQuery* v6 = db->query(szSQL);
    Answer::MySqlQuery result(v6);
    
    if (!result.eof())
    {
        data.sign_record = result.getIntValue("sign_record", 0);
        
        std::string p_RewardString = result.getStringValue("sign_reward", "");
        InitSignRewardInfo(p_RewardString);
        
        data.refresh_time = result.getIntValue("refresh_time", 0);
        OnlimeReward = result.getStringValue("online_reward", "");
    }
    
    // 加载找回信息
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL) - 1, 
             "SELECT * FROM `mem_chr_search_back_info` WHERE `cid`=%lld", nCid);
    
    Answer::MySqlQuery result2(db->query(szSQL));
    
    while (!result2.eof())
    {
        SearchBackInfo info;
        memset(&info, 0, sizeof(info));
        info.nType = result2.getIntValue("type", 0);
        info.nSubType = result2.getIntValue("sub_type", 0);
        info.nTimes = result2.getIntValue("times", 0);
        info.nParam = result2.getIntValue("param", 0);
        lstTodayInfo.push_back(info);
        
        result2.nextRow();
    }
    
    // 加载找回记录
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL) - 1, 
             "SELECT * FROM `mem_chr_search_back_record` WHERE `cid`=%lld", nCid);
    
    Answer::MySqlQuery result3(db->query(szSQL));
    
    while (!result3.eof())
    {
        SearchBackRecord record;
        memset(&record, 0, sizeof(record));
        
        int32_t nDay = result3.getIntValue("day", 0);
        record.nId = result3.getIntValue("id", 0);
        record.nTimes = result3.getIntValue("times", 0);
        record.nParam = result3.getIntValue("param", 0);
        
        if (nDay >= 0 && nDay <= 2)
        {
            vSeachBackRecord[nDay].push_back(record);
        }
        
        result3.nextRow();
    }
}

void DailyActivityData::PackageData(Answer::NetPacket* packet)
{
    if (!packet) return;
    
    // 写入签到记录
    packet->writeInt32(data.sign_record);
    
    // 写入签到奖励字符串
    std::string signRewardStr = GetSignRewardString();
    packet->writeUTF8(signRewardStr);
    
    // 写入刷新时间
    packet->writeInt32(data.refresh_time);
    
    // 写入今日信息数量
    int32_t infoCount = static_cast<int32_t>(lstTodayInfo.size());
    packet->writeInt32(infoCount);
    
    // 写入今日信息列表
    for (const auto& info : lstTodayInfo)
    {
        packet->writeInt8(info.nType);
        packet->writeInt8(info.nSubType);
        packet->writeInt32(info.nTimes);
        packet->writeInt32(info.nParam);
    }
    
    // 写入找回记录
    for (int i = 0; i <= 2; ++i)
    {
        int32_t recordCount = static_cast<int32_t>(vSeachBackRecord[i].size());
        packet->writeInt32(recordCount);
        
        for (const auto& record : vSeachBackRecord[i])
        {
            packet->writeInt32(record.nId);
            packet->writeInt32(record.nTimes);
            packet->writeInt32(record.nParam);
        }
    }
    
    // 写入在线奖励字符串
    packet->writeUTF8(OnlimeReward);
}

void DailyActivityData::SaveToSqlString(std::list<std::string>& sqls, int64_t nCid)
{
    char szSQL[4096];
    
    // 保存签到信息
    memset(szSQL, 0, sizeof(szSQL));
    
    std::string signRewardStr = GetSignRewardString();
    
    snprintf(szSQL, sizeof(szSQL) - 1,
             "INSERT INTO `mem_chr_sign_info` (`cid`, `sign_record`, `sign_reward`, `refresh_time`, `online_reward`) "
             "VALUES (%lld, %d, '%s', %d, '%s') "
             "ON DUPLICATE KEY UPDATE "
             "`sign_record` = %d, `sign_reward` = '%s', `refresh_time` = %d, `online_reward` = '%s'",
             nCid,
             data.sign_record,
             signRewardStr.c_str(),
             data.refresh_time,
             OnlimeReward.c_str(),
             data.sign_record,
             signRewardStr.c_str(),
             data.refresh_time,
             OnlimeReward.c_str());
    
    sqls.push_back(std::string(szSQL));
    
    // 删除旧的找回信息
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL) - 1, 
             "DELETE FROM `mem_chr_search_back_info` WHERE `cid`=%lld", nCid);
    sqls.push_back(std::string(szSQL));
    
    // 插入新的找回信息
    for (const auto& info : lstTodayInfo)
    {
        memset(szSQL, 0, sizeof(szSQL));
        snprintf(szSQL, sizeof(szSQL) - 1,
                 "INSERT INTO `mem_chr_search_back_info` (`cid`, `type`, `sub_type`, `times`, `param`) "
                 "VALUES (%lld, %d, %d, %d, %d) "
                 "ON DUPLICATE KEY UPDATE `times`=%d, `param`=%d",
                 nCid,
                 info.nType,
                 info.nSubType,
                 info.nTimes,
                 info.nParam,
                 info.nTimes,
                 info.nParam);
        sqls.push_back(std::string(szSQL));
    }
    
    // 删除旧的找回记录
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL) - 1, 
             "DELETE FROM `mem_chr_search_back_record` WHERE `cid`=%lld", nCid);
    sqls.push_back(std::string(szSQL));
    
    // 插入新的找回记录
    for (int i = 0; i <= 2; ++i)
    {
        for (const auto& record : vSeachBackRecord[i])
        {
            memset(szSQL, 0, sizeof(szSQL));
            snprintf(szSQL, sizeof(szSQL) - 1,
                     "INSERT INTO `mem_chr_search_back_record` (`cid`, `day`, `id`, `times`, `param`) "
                     "VALUES (%lld, %d, %d, %d, %d) "
                     "ON DUPLICATE KEY UPDATE `times`=%d, `param`=%d",
                     nCid,
                     i,
                     record.nId,
                     record.nTimes,
                     record.nParam,
                     record.nTimes,
                     record.nParam);
            sqls.push_back(std::string(szSQL));
        }
    }
}

void DailyActivityData::UnPackageData(Answer::NetPacket* inPacket)
{
    if (!inPacket) return;
    
    // 读取签到记录
    data.sign_record = inPacket->readInt32();
    
    // 读取签到奖励字符串
    std::string v5;
    inPacket->readUTF8(v5);
    InitSignRewardInfo(v5);
    
    // 读取刷新时间
    data.refresh_time = inPacket->readInt32();
    
    // 读取今日信息
    int32_t infoCount = inPacket->readInt32();
    for (int32_t i = 0; i < infoCount; ++i)
    {
        SearchBackInfo info;
        memset(&info, 0, sizeof(info));
        info.nType = inPacket->readInt8();
        info.nSubType = inPacket->readInt8();
        info.nTimes = inPacket->readInt32();
        info.nParam = inPacket->readInt32();
        lstTodayInfo.push_back(info);
    }
    
    // 读取找回记录
    for (int day = 0; day <= 2; ++day)
    {
        int32_t recordCount = inPacket->readInt32();
        for (int32_t i = 0; i < recordCount; ++i)
        {
            SearchBackRecord record;
            memset(&record, 0, sizeof(record));
            record.nId = inPacket->readInt32();
            record.nTimes = inPacket->readInt32();
            record.nParam = inPacket->readInt32();
            vSeachBackRecord[day].push_back(record);
        }
    }
    
    // 读取在线奖励字符串
    inPacket->readUTF8(OnlimeReward);
}

std::string DailyActivityData::GetSignRewardString()
{
    std::string result;
    
    for (size_t i = 0; i < data.sign_reward.size(); ++i)
    {
        if (i > 0)
        {
            result += "|";
        }
        result += std::to_string(static_cast<int>(data.sign_reward[i]));
    }
    
    return result;
}