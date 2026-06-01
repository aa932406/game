#ifndef _MEMATTRDATA_H_
#define _MEMATTRDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MemAttrData
{
public:
    MemAttrData();
    ~MemAttrData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);
    std::string SaveString();

private:
    int32_t vAttr[50];
};

#endif
