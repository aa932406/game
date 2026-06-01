#ifndef _MEMEQUIP_H_
#define _MEMEQUIP_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class MemEquip
{
public:
    MemEquip();
    MemEquip(const MemEquip &other);
    ~MemEquip();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet) const;
    void UnPackageData(Answer::NetPacket *inPacket);
    void PackageClientData(Answer::NetPacket *packet) const;

    int64_t   id;
    int32_t   base;
    int64_t   owner;
    int8_t    star;
    std::string name;
    int32_t   time;
    int32_t   MapId;
    int32_t   Mid;
};

#endif
