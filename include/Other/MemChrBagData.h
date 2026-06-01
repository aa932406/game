#ifndef _MEMCHRBAGDATA_H_
#define _MEMCHRBAGDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct MemChrBag
{
    int32_t itemId;
    int8_t  itemClass;
    int32_t itemCount;
    int8_t  bind;
};

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
