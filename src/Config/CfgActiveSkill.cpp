#include "Config/CfgActiveSkill.h"
#include "Config/CfgData.h"


CfgActiveSkill::CfgActiveSkill(const CfgActiveSkill& a2)
{
    id = a2.id;
    talent = a2.talent;
    groupid = a2.groupid;
    job = a2.job;
    kind = a2.kind;
    distance = a2.distance;
    range = a2.range;
    area = a2.area;
    self = a2.self;
    target_num = a2.target_num;
    beneficial = a2.beneficial;
    tar_type = a2.tar_type;
    addon_skill = a2.addon_skill;
    addon_time = a2.addon_time;
    addon_cd = a2.addon_cd;
    addon_delay = a2.addon_delay;
    addon_trig_times = a2.addon_trig_times;
    cd = a2.cd;
    cd_adjust = a2.cd_adjust;
    mp = a2.mp;
    power = a2.power;
    special = a2.special;
    attack_type = a2.attack_type;
    attack_modify = a2.attack_modify;
    building_modify = a2.building_modify;
    buff_rate = a2.buff_rate;
    buff = a2.buff;
    buff_time = a2.buff_time;
    chantTime = a2.chantTime;
    summon_id = a2.summon_id;
    summon_delay = a2.summon_delay;
    summon_attr = a2.summon_attr;
    summon_limit = a2.summon_limit;
    boss_addon_damage = a2.boss_addon_damage;
    append_value = a2.append_value;
    shu_lian_du = a2.shu_lian_du;
}

CfgActiveSkill& CfgActiveSkill::operator=(const CfgActiveSkill& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        talent = a2.talent;
        groupid = a2.groupid;
        job = a2.job;
        kind = a2.kind;
        distance = a2.distance;
        range = a2.range;
        area = a2.area;
        self = a2.self;
        target_num = a2.target_num;
        beneficial = a2.beneficial;
        tar_type = a2.tar_type;
        addon_skill = a2.addon_skill;
        addon_time = a2.addon_time;
        addon_cd = a2.addon_cd;
        addon_delay = a2.addon_delay;
        addon_trig_times = a2.addon_trig_times;
        cd = a2.cd;
        cd_adjust = a2.cd_adjust;
        mp = a2.mp;
        power = a2.power;
        special = a2.special;
        attack_type = a2.attack_type;
        attack_modify = a2.attack_modify;
        building_modify = a2.building_modify;
        buff_rate = a2.buff_rate;
        buff = a2.buff;
        buff_time = a2.buff_time;
        chantTime = a2.chantTime;
        summon_id = a2.summon_id;
        summon_delay = a2.summon_delay;
        summon_attr = a2.summon_attr;
        summon_limit = a2.summon_limit;
        boss_addon_damage = a2.boss_addon_damage;
        append_value = a2.append_value;
        shu_lian_du = a2.shu_lian_du;
    }
    return *this;
}
