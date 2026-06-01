#ifndef _CFGACTIVESKILL_H_
#define _CFGACTIVESKILL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgActiveSkill - Config data class
 */
class CfgActiveSkill
{
public:
    int32_t id;
    int32_t talent;
    int32_t groupid;
    int32_t job;
    int32_t kind;
    int32_t distance;
    int32_t range;
    int32_t area;
    int32_t self;
    int32_t target_num;
    int32_t beneficial;
    int32_t tar_type;
    int32_t addon_skill;
    int32_t addon_time;
    int32_t addon_cd;
    int32_t addon_delay;
    int32_t addon_trig_times;
    int32_t cd;
    int32_t cd_adjust;
    int32_t mp;
    int32_t power;
    int32_t special;
    int32_t attack_type;
    int32_t attack_modify;
    int32_t building_modify;
    int32_t buff_rate;
    int32_t buff;
    int32_t buff_time;
    int32_t chantTime;
    int32_t summon_id;
    int32_t summon_delay;
    std::vector<AttrAddon> summon_attr;
    int32_t summon_limit;
    int32_t boss_addon_damage;
    int32_t append_value;
    int32_t shu_lian_du;

    CfgActiveSkill() = default;
    CfgActiveSkill(const CfgActiveSkill& a2);
    ~CfgActiveSkill() = default;
    CfgActiveSkill& operator=(const CfgActiveSkill& a2);
};

#endif // _CFGACTIVESKILL_H_
