#ifndef _MEMCHRGEMDATA_H_
#define _MEMCHRGEMDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MemChrGemData
{
public:
    MemChrGemData();
    ~MemChrGemData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::string gemInfo;
    std::string posLevel;
    std::string Strengthen;
    std::string ShenYaoEquipPos;
};

#endif
