#include "Config/CfgMonster.h"


CfgMonster::CfgMonster(const CfgMonster& a2)
{
    mid = a2.mid;
    level = a2.level;
    group_id = a2.group_id;
    quality = a2.quality;
    rand_types = a2.rand_types;
    battle = a2.battle;
    exp = a2.exp;
    type = a2.type;
    revive_time = a2.revive_time;
    skill_id = a2.skill_id;
    hpPercent = a2.hpPercent;
    ai = a2.ai;
    broadcast = a2.broadcast;
    drop_free = a2.drop_free;
    corpse_time = a2.corpse_time;
    boss_sign = a2.boss_sign;
    BossSocre = a2.BossSocre;
    TaskShare = a2.TaskShare;
    camp = a2.camp;
    kill_point = a2.kill_point;
    revive_skin = a2.revive_skin;
    prestige = a2.prestige;
    dust = a2.dust;
    is_building = a2.is_building;
    PureDamage = a2.PureDamage;
    IfCurse = a2.IfCurse;
    Score = a2.Score;
    IsShowOwner = a2.IsShowOwner;
    DiligenceType = a2.DiligenceType;
    DiligenceValue = a2.DiligenceValue;
    MonsterHp = a2.MonsterHp;
    CanTunshi = a2.CanTunshi;
    Mid2 = a2.Mid2;
    FamilyValue = a2.FamilyValue;
    PortalRatio = a2.PortalRatio;
    PortalId = a2.PortalId;
    PortalDuration = a2.PortalDuration;
    IsXinMoMonster = a2.IsXinMoMonster;
    QieGe = a2.QieGe;
    Energy = a2.Energy;
    cItem = a2.cItem;
    IsSunAndMoon = a2.IsSunAndMoon;
}

CfgMonster& CfgMonster::operator=(const CfgMonster& a2)
{
    if (this != &a2)
    {
        mid = a2.mid;
        level = a2.level;
        group_id = a2.group_id;
        quality = a2.quality;
        rand_types = a2.rand_types;
        battle = a2.battle;
        exp = a2.exp;
        type = a2.type;
        revive_time = a2.revive_time;
        skill_id = a2.skill_id;
        hpPercent = a2.hpPercent;
        ai = a2.ai;
        broadcast = a2.broadcast;
        drop_free = a2.drop_free;
        corpse_time = a2.corpse_time;
        boss_sign = a2.boss_sign;
        BossSocre = a2.BossSocre;
        TaskShare = a2.TaskShare;
        camp = a2.camp;
        kill_point = a2.kill_point;
        revive_skin = a2.revive_skin;
        prestige = a2.prestige;
        dust = a2.dust;
        is_building = a2.is_building;
        PureDamage = a2.PureDamage;
        IfCurse = a2.IfCurse;
        Score = a2.Score;
        IsShowOwner = a2.IsShowOwner;
        DiligenceType = a2.DiligenceType;
        DiligenceValue = a2.DiligenceValue;
        MonsterHp = a2.MonsterHp;
        CanTunshi = a2.CanTunshi;
        Mid2 = a2.Mid2;
        FamilyValue = a2.FamilyValue;
        PortalRatio = a2.PortalRatio;
        PortalId = a2.PortalId;
        PortalDuration = a2.PortalDuration;
        IsXinMoMonster = a2.IsXinMoMonster;
        QieGe = a2.QieGe;
        Energy = a2.Energy;
        cItem = a2.cItem;
        IsSunAndMoon = a2.IsSunAndMoon;
    }
    return *this;
}
