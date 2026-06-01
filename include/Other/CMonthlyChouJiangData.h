#ifndef _CMONTHLYCHOUJIANSDATA_H_
#define _CMONTHLYCHOUJIANSDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class CMonthlyChouJiangData
{
public:
    CMonthlyChouJiangData();
    ~CMonthlyChouJiangData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    std::string getItemListString();
    void ParesItemListString(std::string *p_ListString);
    std::string getScoreTime();

private:
    int32_t m_Score;
    int32_t m_AllScore;
    std::list<int32_t> m_GetItemList;
    std::map<int32_t, int32_t> m_GetTimes;
};

#endif
