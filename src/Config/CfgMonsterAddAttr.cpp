#include "Config/CfgMonsterAddAttr.h"

CfgMonsterAddAttr::CfgMonsterAddAttr(const CfgMonsterAddAttr& a2)
{
    WorldBossLevelMin = a2.WorldBossLevelMin;
    WorldBossLevelMax = a2.WorldBossLevelMax;
    AttrVector = a2.AttrVector;
}

CfgMonsterAddAttr& CfgMonsterAddAttr::operator=(const CfgMonsterAddAttr& a2)
{
    if (this != &a2)
    {
        WorldBossLevelMin = a2.WorldBossLevelMin;
        WorldBossLevelMax = a2.WorldBossLevelMax;
        AttrVector = a2.AttrVector;
    }
    return *this;
}
