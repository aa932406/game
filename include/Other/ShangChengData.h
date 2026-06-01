#ifndef _SHANGCHENGDATA_H_
#define _SHANGCHENGDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class ShangChengData
{
public:
    ShangChengData();
    ~ShangChengData();

    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::map<int32_t, int32_t> m_LiMitMap;
};

#endif
