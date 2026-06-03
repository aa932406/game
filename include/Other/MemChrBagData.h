#ifndef _MEMCHRBAGDATA_H_
#define _MEMCHRBAGDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

#ifndef MEMCHRBAG_DEFINED
#define MEMCHRBAG_DEFINED
struct MemChrBag
{
    int32_t itemId;
    int8_t  itemClass;
    int32_t itemCount;
    int8_t  bind;
    int32_t endTime;
    int64_t srcId;
    int32_t& slot() { return itemId; }
    const int32_t& slot() const { return itemId; }
};
#endif

class MemChrBagData
{
public:
    MemChrBagData();
    ~MemChrBagData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    MemChrBag bagData[112];
    std::map<int32_t, int32_t> m_ItemLimit;
};

#endif
