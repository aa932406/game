#include "Config/CfgMonsterRand.h"


CfgMonsterRand::CfgMonsterRand(const CfgMonsterRand& a2)
{
    nId = a2.nId;
    vAttrValue = a2.vAttrValue;
    vAttrRatio = a2.vAttrRatio;
    nSkill = a2.nSkill;
}

CfgMonsterRand& CfgMonsterRand::operator=(const CfgMonsterRand& a2)
{
    if (this != &a2)
    {
        nId = a2.nId;
        vAttrValue = a2.vAttrValue;
        vAttrRatio = a2.vAttrRatio;
        nSkill = a2.nSkill;
    }
    return *this;
}
