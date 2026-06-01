#ifndef _NATIONALDAYDATA_H_
#define _NATIONALDAYDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class NationalDayData
{
public:
    NationalDayData();
    ~NationalDayData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::string m_NationalInfo;
    int32_t m_Level;
    int32_t m_Exp;
    int32_t m_UnLockReward;
    int32_t m_OrdinaryReward;
    int32_t m_SeniorReward;
};

#endif
