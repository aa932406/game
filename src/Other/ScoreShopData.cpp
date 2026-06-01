#include "Other/ScoreShopData.h"
#include "Network/NetPacket.h"

ScoreShopData::ScoreShopData()
{
}

ScoreShopData::~ScoreShopData()
{
}

void ScoreShopData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(static_cast<int32_t>(m_LiMitMap.size()));
    for (auto &it : m_LiMitMap)
    {
        packet->writeInt32(it.first);
        packet->writeInt32(it.second);
    }
}

void ScoreShopData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_LiMitMap.clear();
    int32_t nSize = inPacket->readInt32();
    for (int32_t i = 0; i < nSize; ++i)
    {
        int32_t key = inPacket->readInt32();
        int32_t val = inPacket->readInt32();
        m_LiMitMap[key] = val;
    }
}
