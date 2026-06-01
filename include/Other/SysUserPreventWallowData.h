#ifndef _SYSUSERPREVENTWALLOWDATA_H_
#define _SYSUSERPREVENTWALLOWDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct UserPreventWallowBaseData
{
    int64_t uid;
    int32_t sid;
    char    reserve[0x3C];
};

class SysUserPreventWallowData
{
public:
    SysUserPreventWallowData();
    ~SysUserPreventWallowData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);

private:
    UserPreventWallowBaseData data;
};

#endif
