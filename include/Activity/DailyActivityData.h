#ifndef __DAILY_ACTIVITY_DATA_H__
#define __DAILY_ACTIVITY_DATA_H__

#include <list>
#include <vector>
#include <string>
#include <cstdint>

namespace Answer { class MySqlDBGuard; class NetPacket; }
struct SearchBackInfo;
struct SearchBackRecord;

struct SignInfoData
{
    int32_t sign_record;
    std::vector<int8_t> sign_reward;
    int32_t refresh_time;
    
    SignInfoData() : sign_record(0), refresh_time(0) {}
};

struct DailyActivityDBData
{
    struct
    {
        SignInfoData data;
        std::list<SearchBackInfo> lstTodayInfo;
        std::list<SearchBackRecord> lstTodayInfo2[3];
        std::string OnlimeReward;
    } signInfo;
};

class DailyActivityData
{
public:
    DailyActivityData();
    ~DailyActivityData();

    void CleanUp();
    void InitSignRewardInfo(const std::string& p_RewardString);
    void LoadFromDB(Answer::MySqlDBGuard* db, int64_t nCid);
    void PackageData(Answer::NetPacket* packet);
    void SaveToSqlString(std::list<std::string>& sqls, int64_t nCid);
    void UnPackageData(Answer::NetPacket* inPacket);
    
    std::string GetSignRewardString();
    
    // Getter/Setter
    SignInfoData& GetData() { return data; }
    const SignInfoData& GetData() const { return data; }
    std::list<SearchBackInfo>& GetLstTodayInfo() { return lstTodayInfo; }
    const std::list<SearchBackInfo>& GetLstTodayInfo() const { return lstTodayInfo; }
    std::list<SearchBackRecord>* GetVSeachBackRecord() { return vSeachBackRecord; }
    const std::list<SearchBackRecord>* GetVSeachBackRecord() const { return vSeachBackRecord; }
    std::string& GetOnlimeReward() { return OnlimeReward; }
    const std::string& GetOnlimeReward() const { return OnlimeReward; }

private:
    SignInfoData data;
    std::list<SearchBackInfo> lstTodayInfo;
    std::list<SearchBackRecord> vSeachBackRecord[3];
    std::string OnlimeReward;
};

#endif // __DAILY_ACTIVITY_DATA_H__