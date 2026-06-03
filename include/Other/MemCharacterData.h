#ifndef _MEMCHARACTERDATA_H_
#define _MEMCHARACTERDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct CharacterData
{
    int64_t  cid;
    char     name[32];
    int32_t  level;
    int32_t  job;
    int32_t  camp;
    int32_t  mapid;
    int32_t  x;
    int32_t  y;
    // ... other fields, total size 0xA8
};

class MemCharacterData
{
public:
    MemCharacterData();
    ~MemCharacterData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

    int32_t weapon;
    int32_t cloth;
    int32_t wing;
    int32_t equipStar;
    int32_t equipGem;
    int32_t moneyDropRatio;

private:
    CharacterData data;
};

#endif
