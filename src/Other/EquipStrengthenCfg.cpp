#include "Other/EquipStrengthenCfg.h"

EquipStrengthenCfg::EquipStrengthenCfg()
    : Level(0)
    , Pos(0)
    , ConstItem(0)
    , AttrRate(0)
    , DropRate(0)
    , NeedPos(0)
{
}

EquipStrengthenCfg::EquipStrengthenCfg(const EquipStrengthenCfg& a2)
    : Level(a2.Level)
    , Pos(a2.Pos)
    , ConstItem(a2.ConstItem)
    , AttrRate(a2.AttrRate)
    , DropRate(a2.DropRate)
    , vAttrAddon(a2.vAttrAddon)
    , NeedPos(a2.NeedPos)
{
}
