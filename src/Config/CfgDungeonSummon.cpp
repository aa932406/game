#include "Config/CfgDungeonSummon.h"


CfgDungeonSummon::CfgDungeonSummon(const CfgDungeonSummon& a2)
{
    nDungeon = a2.nDungeon;
    nIndex = a2.nIndex;
    nGold = a2.nGold;
    nLimit = a2.nLimit;
    lMonsters = a2.lMonsters;
    ConstItem = a2.ConstItem;
    FreeCount = a2.FreeCount;
}

CfgDungeonSummon& CfgDungeonSummon::operator=(const CfgDungeonSummon& a2)
{
    if (this != &a2)
    {
        nDungeon = a2.nDungeon;
        nIndex = a2.nIndex;
        nGold = a2.nGold;
        nLimit = a2.nLimit;
        lMonsters = a2.lMonsters;
        ConstItem = a2.ConstItem;
        FreeCount = a2.FreeCount;
    }
    return *this;
}
