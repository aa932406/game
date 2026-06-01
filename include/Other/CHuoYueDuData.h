#ifndef _CHUOYUEDUDATA_H_
#define _CHUOYUEDUDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct HuoYueDuRecord
{
    int32_t state;
    int32_t day;
};

class CHuoYueDuData
{
public:
    CHuoYueDuData();
    ~CHuoYueDuData();

    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);
    void InitHuoYueDu(std::string *p_HuoYueDuString);
    std::string GetHuoYueDuString();

private:
    std::map<int32_t, HuoYueDuRecord> m_HuoYueDuRecord;
};

#endif
