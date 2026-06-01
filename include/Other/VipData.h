#ifndef _VIPDATA_H_
#define _VIPDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class VipData
{
public:
    VipData();
    ~VipData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    int32_t m_Vip;
    int32_t m_level;
    int32_t m_ForeverFlg;
    int32_t m_Club;
    int32_t m_ClubDropTimes;
    int32_t m_BuyTimes;
    std::string m_VipEndTime;
    std::string m_DropString;
};

#endif
