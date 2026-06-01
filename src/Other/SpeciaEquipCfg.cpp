#include "Other/SpeciaEquipCfg.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

SpeciaEquipCfg::SpeciaEquipCfg()
    : nIndex(0), nNeedVipLevel(0), nContinueLogin(0), nNeedGold(0), nGongGaoId(0)
{
}

SpeciaEquipCfg::~SpeciaEquipCfg()
{
}

SpeciaEquipCfg::SpeciaEquipCfg(const SpeciaEquipCfg& other)
    : nIndex(other.nIndex)
    , nAttrList(other.nAttrList)
    , nNeedVipLevel(other.nNeedVipLevel)
    , nContinueLogin(other.nContinueLogin)
    , nNeedGold(other.nNeedGold)
    , nAttrList2(other.nAttrList2)
    , nGongGaoId(other.nGongGaoId)
{
}
