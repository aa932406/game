#include "Other/GroupMonster.h"

GroupMonster::GroupMonster()
    : RegionCount(0)
    , Mid(0)
    , MonsterCount(0)
    , IsSpecial(0)
{
}

GroupMonster::GroupMonster(const GroupMonster& a2)
    : RegionCount(a2.RegionCount)
    , Mid(a2.Mid)
    , MonsterCount(a2.MonsterCount)
    , IsSpecial(a2.IsSpecial)
    , RevivePos(a2.RevivePos)
{
}
