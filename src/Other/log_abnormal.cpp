#include "Other/log_abnormal.h"

#include <cstring>
#include <algorithm>

log_abnormal::log_abnormal()
    : nCid(0)
    , nType(0)
    , nValue(0)
    , nOpway(0)
{
}

log_abnormal::log_abnormal(const log_abnormal& other)
    : nCid(other.nCid)
    , sName(other.sName)
    , nType(other.nType)
    , nValue(other.nValue)
    , nOpway(other.nOpway)
{
}

void log_abnormal::PackageData(Answer::NetPacket *packet)
{
    if (packet)
    {
        packet->writeInt64(nCid);
        packet->writeUTF8(sName);
        packet->writeInt8(static_cast<int8_t>(nType));
        packet->writeInt32(nValue);
        packet->writeInt32(nOpway);
    }
}
