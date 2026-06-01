#ifndef _CSHIZHUANGDATA_H_
#define _CSHIZHUANGDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Other/ShiZhuangInfo.h"

namespace Answer { class NetPacket; }

class CShiZhuangData
{
public:
    CShiZhuangData();
    ~CShiZhuangData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);
    std::string GetShiZhuangLevelString();
    void ParesShiZhuangLevelString(std::string *p_ShiZhuangLevel);

private:
    ShiZhuangInfo m_vShiZhuang[3];
    std::map<int32_t, int32_t> m_ShiZhuangLevel;
    int32_t m_EffectId;
};

#endif
