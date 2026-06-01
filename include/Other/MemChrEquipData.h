#ifndef _MEMCHREQUIPDATA_H_
#define _MEMCHREQUIPDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct EquipSlot
{
    int32_t itemId;
    int8_t  itemClass;
    int32_t itemCount;
    int8_t  bind;
};

class MemChrEquipData
{
public:
    MemChrEquipData();
    ~MemChrEquipData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    EquipSlot vEquip[93];
};

#endif
