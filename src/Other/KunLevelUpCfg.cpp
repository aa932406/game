#include "Other/KunLevelUpCfg.h"

KunLevelUpCfg::KunLevelUpCfg()
    : nLevel(0)
    , nExp(0)
    , nGongGaoId(0)
    , nTalentId(0)
    , nTalentLevel(0)
    , nCostMoney(0)
{
}

KunLevelUpCfg::KunLevelUpCfg(const KunLevelUpCfg& a2)
    : nLevel(a2.nLevel)
    , nExp(a2.nExp)
    , lAttrList(a2.lAttrList)
    , nGongGaoId(a2.nGongGaoId)
    , nTalentId(a2.nTalentId)
    , nTalentLevel(a2.nTalentLevel)
    , nCostMoney(a2.nCostMoney)
{
}
