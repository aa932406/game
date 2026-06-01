#ifndef _SYSUSERDATA_H_
#define _SYSUSERDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct SysUserBaseData
{
    int64_t uid;
    int32_t sid;
    char    reserve[0x114];
};

class SysUserData
{
public:
    SysUserData();
    ~SysUserData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    SysUserBaseData data;
};

#endif
