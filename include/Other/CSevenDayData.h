#ifndef _CSEVENDAYDATA_H_
#define _CSEVENDAYDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class CSevenDayData
{
public:
    CSevenDayData();
    ~CSevenDayData();

    void CleanUp();
    std::string getRewardState();
    void SaveRewardState(std::string *p_String);
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    int64_t m_OpenTime;
    std::map<int32_t, int32_t> m_RewardState;
    int32_t m_SumRewardState;
};

#endif
