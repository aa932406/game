#ifndef _FRIENDEXPREWARD_H_
#define _FRIENDEXPREWARD_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Other/DieRecord.h"

namespace Answer { class NetPacket; }

class FriendExpReward
{
public:
    FriendExpReward();
    ~FriendExpReward();

    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);

private:
    std::list<DieRecord> m_DieRecord;
};

#endif
