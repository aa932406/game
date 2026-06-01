#include "Config/CfgBuff.h"


CfgBuff::CfgBuff(const CfgBuff& a2)
{
    id = a2.id;
    groupid = a2.groupid;
    attack_type = a2.attack_type;
    level = a2.level;
    beneficial = a2.beneficial;
    special = a2.special;
    modify = a2.modify;
    hpValue = a2.hpValue;
    hpPecent = a2.hpPecent;
    control = a2.control;
    duration = a2.duration;
    interval = a2.interval;
    buffAttr = a2.buffAttr;
    isShow = a2.isShow;
    shieldValue = a2.shieldValue;
    shieldRatio = a2.shieldRatio;
    effectType = a2.effectType;
    effectTimes = a2.effectTimes;
    canRemove = a2.canRemove;
    canRecover = a2.canRecover;
    angry = a2.angry;
    battle = a2.battle;
}

CfgBuff& CfgBuff::operator=(const CfgBuff& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        groupid = a2.groupid;
        attack_type = a2.attack_type;
        level = a2.level;
        beneficial = a2.beneficial;
        special = a2.special;
        modify = a2.modify;
        hpValue = a2.hpValue;
        hpPecent = a2.hpPecent;
        control = a2.control;
        duration = a2.duration;
        interval = a2.interval;
        buffAttr = a2.buffAttr;
        isShow = a2.isShow;
        shieldValue = a2.shieldValue;
        shieldRatio = a2.shieldRatio;
        effectType = a2.effectType;
        effectTimes = a2.effectTimes;
        canRemove = a2.canRemove;
        canRecover = a2.canRecover;
        angry = a2.angry;
        battle = a2.battle;
    }
    return *this;
}
