#include "Other/BossFirstKilled.h"

BossFirstKilled::BossFirstKilled()
    : BossId(0)
    , RewardType(0)
    , RewardValue(0)
    , Killer(0)
    , KillTime(0)
    , GongGaoId(0)
{
}

BossFirstKilled::BossFirstKilled(const BossFirstKilled& a2)
    : BossId(a2.BossId)
    , RewardType(a2.RewardType)
    , RewardValue(a2.RewardValue)
    , Killer(a2.Killer)
    , KillerName(a2.KillerName)
    , KillTime(a2.KillTime)
    , GongGaoId(a2.GongGaoId)
{
}
