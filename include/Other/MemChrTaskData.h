#ifndef _MEMCHRTASKDATA_H_
#define _MEMCHRTASKDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct MemChrTask
{
    int32_t id;
    int32_t state;
    int32_t monster;
};

typedef std::vector<MemChrTask> MemChrTaskVector;

class MemChrTaskData
{
public:
    MemChrTaskData();
    ~MemChrTaskData();

    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    MemChrTaskVector taskVt;
};

#endif
