#include "Other/WingEquipPolish.h"

#include <cstring>
#include <algorithm>

WingEquipPolish::WingEquipPolish()
    : nSlot(0)
    , nLevel(0)
    , nConstMoney(0)
    , SuitId(0)
    , GongGaoId(0)
{
}

WingEquipPolish::~WingEquipPolish()
{
}

WingEquipPolish::WingEquipPolish(const WingEquipPolish& other)
    : nSlot(other.nSlot)
    , nLevel(other.nLevel)
    , lAttrList(other.lAttrList)
    , lCostList(other.lCostList)
    , nConstMoney(other.nConstMoney)
    , SuitId(other.SuitId)
    , GongGaoId(other.GongGaoId)
{
}
