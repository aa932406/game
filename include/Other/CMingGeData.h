#ifndef _CMINGGEDATA_H_
#define _CMINGGEDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class CMingGeData
{
public:
    CMingGeData();
    ~CMingGeData();

    void CleanUp();
    std::string GetNpcString();
    void parseMingGeNpc(std::string *p_NpcString);
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    int32_t m_MingGeExp;
    int32_t m_MingChip;
    char    m_MingGeBag[0x320];
    char    m_LieMingBag[0xF0];
    char    m_MingGeEquip[0x40];
    char    m_MingGeNpc[0x14];
};

#endif
