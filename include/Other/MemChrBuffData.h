#ifndef _MEMCHRBUFFDATA_H_
#define _MEMCHRBUFFDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Common/CommonTypes.h"

namespace Answer { class NetPacket; }

class MemChrBuffData
{
public:
    MemChrBuffData();
    ~MemChrBuffData();

    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    MemChrBuffVector buffVt;
};

#endif
