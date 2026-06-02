#include "Other/VipCardCfg.h"

#include <cstring>
#include <algorithm>

VipCardCfg::VipCardCfg(const VipCardCfg& a2)
{
    this->VipCardId = a2.VipCardId;
    this->AddVipTime = a2.AddVipTime;
    this->NeedGold = a2.NeedGold;
    this->AddAttr = a2.AddAttr;
    this->VipLevel = a2.VipLevel;
    this->ReNeedGold = a2.ReNeedGold;
    this->Money = a2.Money;
}

