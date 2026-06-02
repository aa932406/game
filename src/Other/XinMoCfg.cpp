#include "Other/XinMoCfg.h"

#include <cstring>
#include <algorithm>

XinMoCfg::XinMoCfg(const XinMoCfg& a2)
{
    this->XinMoLevel = a2.XinMoLevel;
    this->CostMoney = a2.CostMoney;
    this->ConstShenYaoBi = a2.ConstShenYaoBi;
    this->ConstExp = a2.ConstExp;
    this->NeedQiQingLevel = a2.NeedQiQingLevel;
    this->Attr = a2.Attr;
    this->GongGaoId = a2.GongGaoId;
}

