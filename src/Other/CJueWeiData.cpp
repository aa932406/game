#include "Other/CJueWeiData.h"
#include "Network/NetPacket.h"

CJueWeiData::CJueWeiData()
{
    nJueWei = 0;
}

CJueWeiData::~CJueWeiData()
{
}

void CJueWeiData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt8(nJueWei);
}

void CJueWeiData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    nJueWei = inPacket->readInt8();
}
