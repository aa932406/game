#include "Config/CfgTalent.h"


CfgTalent::CfgTalent(const CfgTalent& a2)
{
    id = a2.id;
    Level = a2.Level;
    Playerlevel = a2.Playerlevel;
    maxLevel = a2.maxLevel;
    skillid = a2.skillid;
    costItem = a2.costItem;
    powerSkills = a2.powerSkills;
    battle = a2.battle;
    Point = a2.Point;
    GongGaoId = a2.GongGaoId;
}

CfgTalent& CfgTalent::operator=(const CfgTalent& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        Level = a2.Level;
        Playerlevel = a2.Playerlevel;
        maxLevel = a2.maxLevel;
        skillid = a2.skillid;
        costItem = a2.costItem;
        powerSkills = a2.powerSkills;
        battle = a2.battle;
        Point = a2.Point;
        GongGaoId = a2.GongGaoId;
    }
    return *this;
}
