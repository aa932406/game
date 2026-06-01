#include "Other/BossKilledRewardData.h"
#include "Network/NetPacket.h"

BossKilledRewardData::BossKilledRewardData()
{
    CleanUp();
}

BossKilledRewardData::~BossKilledRewardData()
{
}

void BossKilledRewardData::CleanUp()
{
    RewardString.clear();
}

void BossKilledRewardData::PackageData(Answer::NetPacket *packet)
{
    if (packet)
        packet->writeUTF8(&RewardString);
}
