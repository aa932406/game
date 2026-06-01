#ifndef _TOUZIDATA_H_
#define _TOUZIDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class TouZiData
{
public:
    TouZiData();
    ~TouZiData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    int64_t m_SevenDayTouZiTime;
    int32_t m_SevenDayRecord;
    int64_t m_MonthTouZiTime;
    int32_t m_MonthTouZiRecord;
};

#endif
