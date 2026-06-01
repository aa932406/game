#ifndef _BOSSKILLEDREWARDDATA_H_
#define _BOSSKILLEDREWARDDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

class BossKilledRewardData
{
public:
    BossKilledRewardData();
    ~BossKilledRewardData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);

private:
    std::string RewardString;
};

#endif
