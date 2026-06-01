#include "Config/CfgPassiveSkill.h"


CfgPassiveSkill::CfgPassiveSkill(const CfgPassiveSkill& a2)
{
    id = a2.id;
    type = a2.type;
    vAttrs = a2.vAttrs;
    lTalentAddon = a2.lTalentAddon;
    dropMoneyRate = a2.dropMoneyRate;
}

CfgPassiveSkill& CfgPassiveSkill::operator=(const CfgPassiveSkill& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        type = a2.type;
        vAttrs = a2.vAttrs;
        lTalentAddon = a2.lTalentAddon;
        dropMoneyRate = a2.dropMoneyRate;
    }
    return *this;
}
