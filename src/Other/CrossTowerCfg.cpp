#include "Other/CrossTowerCfg.h"
#include "Common/CommonTypes.h"

#include <cstring>
#include <algorithm>

CrossTowerCfg::CrossTowerCfg()
    : MapId(0), NextMapId(0), LastMapId(0), MaxPlayer(0), MaxScore(0), MinScore(0), KillCount(0)
{
}

CrossTowerCfg::~CrossTowerCfg()
{
}

CrossTowerCfg::CrossTowerCfg(const CrossTowerCfg& other)
    : MapId(other.MapId)
    , NextMapId(other.NextMapId)
    , LastMapId(other.LastMapId)
    , MaxPlayer(other.MaxPlayer)
    , MaxScore(other.MaxScore)
    , MinScore(other.MinScore)
    , KillCount(other.KillCount)
    , RewardVt(other.RewardVt)
    , TopRewardVt(other.TopRewardVt)
{
}
