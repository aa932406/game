#ifndef _MEMCHRSKILLDATA_H_
#define _MEMCHRSKILLDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MemChrSkillData
{
public:
    MemChrSkillData();
    ~MemChrSkillData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::string talents;
    int32_t     m_nPower;
};

#endif
