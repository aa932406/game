#ifndef _MEMCHRAUTOFIGHTDATA_H_
#define _MEMCHRAUTOFIGHTDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MemChrAutoFightData
{
public:
    MemChrAutoFightData();
    ~MemChrAutoFightData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);

private:
    char data[0x4B2];
};

#endif
