#include "Config/CfgTrigSkill.h"


CfgTrigSkill::CfgTrigSkill(const CfgTrigSkill& a2)
{
    id = a2.id;
    groupid = a2.groupid;
    trigType = a2.trigType;
    trigParam = a2.trigParam;
    targetType = a2.targetType;
    trigRate = a2.trigRate;
    cdTime = a2.cdTime;
    trigBuff = a2.trigBuff;
    special = a2.special;
    specialParam = a2.specialParam;
    IsPvp = a2.IsPvp;
}

CfgTrigSkill& CfgTrigSkill::operator=(const CfgTrigSkill& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        groupid = a2.groupid;
        trigType = a2.trigType;
        trigParam = a2.trigParam;
        targetType = a2.targetType;
        trigRate = a2.trigRate;
        cdTime = a2.cdTime;
        trigBuff = a2.trigBuff;
        special = a2.special;
        specialParam = a2.specialParam;
        IsPvp = a2.IsPvp;
    }
    return *this;
}
