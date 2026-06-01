#include "Other/FamilyTaskReward.h"

FamilyTaskReward::FamilyTaskReward()
    : Index(0)
    , NeedCount(0)
{
}

FamilyTaskReward::FamilyTaskReward(const FamilyTaskReward& a2)
    : Index(a2.Index)
    , NeedCount(a2.NeedCount)
    , Rewards(a2.Rewards)
{
}
