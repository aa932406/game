#ifndef _CKUNDATA_H_
#define _CKUNDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class CKunData
{
public:
    CKunData();
    ~CKunData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);
    std::string getLingZhuPosString();
    void ParesLingZhuPosString(std::string *p_LingZhuPos);
    std::string getDanTianString();
    void ParesDanTianString(std::string *p_DanTianString);

private:
    int64_t m_nBuyTime;
    int64_t m_nLianHuaTime;
    int32_t m_nKunLevel;
    int32_t m_nExp;
    char    m_DanTian[0x48];
    char    m_LingZhuBag[0x320];
    char    m_LingZhuPos[0x50];
};

#endif
