#include "Other/EquipBackRankCfg.h"

#include <cstring>
#include <algorithm>

EquipBackRankCfg::EquipBackRankCfg()
    : Cid(0)
    , Count(0)
    , Time(0)
{
}

EquipBackRankCfg::EquipBackRankCfg(const EquipBackRankCfg& other)
    : Cid(other.Cid)
    , nName(other.nName)
    , Count(other.Count)
    , Time(other.Time)
{
}

void EquipBackRankCfg::PackageData(Answer::NetPacket *packet)
{
    if (packet)
    {
        packet->writeInt64(Cid);
        packet->writeUTF8(nName);
        packet->writeInt32(Count);
        packet->writeInt32(Time);
    }
}
