#ifndef _CVPLANDATA_H_
#define _CVPLANDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class CVplanData
{
public:
    CVplanData();
    ~CVplanData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::string m_strSGPf;
    int8_t  m_VplanType;
    int32_t m_VplanLevel;
    int32_t m_SwVipLevel;
    int32_t m_YYLevel;
    int32_t m_YYVipLevel;
    int32_t m_YySuperLevel;
    int32_t m_SgHallLevel;
};

#endif
