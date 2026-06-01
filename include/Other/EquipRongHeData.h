#ifndef _EQUIPRONGHEDATA_H_
#define _EQUIPRONGHEDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class EquipRongHeData
{
public:
    EquipRongHeData();
    ~EquipRongHeData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::string m_EquipRongHe;
};

#endif
