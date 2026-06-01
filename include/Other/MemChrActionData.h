#ifndef _MEMCHRACTIONDATA_H_
#define _MEMCHRACTIONDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct MemChrAction
{
    int32_t id;
    int32_t type;
};

class MemChrActionData
{
public:
    MemChrActionData();
    ~MemChrActionData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    MemChrAction actionArry[13];
};

#endif
