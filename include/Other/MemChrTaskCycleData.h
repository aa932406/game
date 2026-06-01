#ifndef _MEMCHRTASKCYCLEDATA_H_
#define _MEMCHRTASKCYCLEDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MemChrTaskCycleData
{
public:
    MemChrTaskCycleData();
    ~MemChrTaskCycleData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    int32_t nFinishTimes;
    int32_t nTaskId;
    int8_t  nState;
};

#endif
