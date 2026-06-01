#include "Other/HallOfFameReward.h"

HallOfFameReward::HallOfFameReward()
    : nIndexMin(0)
    , nIndexMax(0)
    , nHonor(0)
{
}

HallOfFameReward::HallOfFameReward(const HallOfFameReward& a2)
    : nIndexMin(a2.nIndexMin)
    , nIndexMax(a2.nIndexMax)
    , nHonor(a2.nHonor)
    , items(a2.items)
{
}
