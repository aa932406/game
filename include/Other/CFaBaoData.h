#ifndef _CFABAODATA_H_
#define _CFABAODATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class CFaBaoData
{
public:
    CFaBaoData();
    ~CFaBaoData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);
    std::string GetFaBaoLevelString();
    void ParesFaBaoLevelString(std::string *p_FaBaoLevelString);

private:
    char m_FaBao[0x20];
};

#endif
