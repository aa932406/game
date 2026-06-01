#ifndef _CJUEWEIDATA_H_
#define _CJUEWEIDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class CJueWeiData
{
public:
    CJueWeiData();
    ~CJueWeiData();

    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    int8_t nJueWei;
};

#endif
