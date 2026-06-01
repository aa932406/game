#include "Other/BackEquipTask.h"
#include "Common/CommonTypes.h"

#include <cstring>
#include <algorithm>

BackEquipTask::BackEquipTask()
    : Index(0), PlayerLevelMin(0), PlayerLevelMax(0), Star(0), GetCurr(0), RandGold(0)
{
}

BackEquipTask::~BackEquipTask()
{
}

BackEquipTask::BackEquipTask(const BackEquipTask& other)
    : Index(other.Index)
    , Equips(other.Equips)
    , PlayerLevelMin(other.PlayerLevelMin)
    , PlayerLevelMax(other.PlayerLevelMax)
    , Star(other.Star)
    , GetCurr(other.GetCurr)
    , RandGold(other.RandGold)
    , Items(other.Items)
{
}
