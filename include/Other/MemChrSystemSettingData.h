#ifndef _MEMCHRSYSTEMSETTINGDATA_H_
#define _MEMCHRSYSTEMSETTINGDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MemChrSystemSettingData
{
public:
    MemChrSystemSettingData();
    ~MemChrSystemSettingData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);

private:
    std::string data;
};

#endif
