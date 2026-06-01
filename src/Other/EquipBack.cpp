#include "Other/EquipBack.h"

EquipBack::EquipBack()
    : nId(0)
    , nType(0)
    , nRecovType(0)
    , nRecovValues(0)
    , nBuyBackType(0)
    , nBuyBackValue(0)
{
}

EquipBack::EquipBack(const EquipBack& a2)
    : nId(a2.nId)
    , nType(a2.nType)
    , nEquipList(a2.nEquipList)
    , nRecovType(a2.nRecovType)
    , nRecovValues(a2.nRecovValues)
    , nBuyBackType(a2.nBuyBackType)
    , nBuyBackValue(a2.nBuyBackValue)
{
}
