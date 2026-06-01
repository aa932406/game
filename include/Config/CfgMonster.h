#ifndef _CFGMONSTER_H_
#define _CFGMONSTER_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMonster - Config data class
 */
class CfgMonster
{
public:
    int32_t mid;
    int32_t level;
    int32_t group_id;
    int32_t quality;
    std::vector<int> rand_types;
    int32_t battle;
    int32_t exp;
    int32_t type;
    int32_t revive_time;
    int32_t skill_id;
    int32_t hpPercent;
    int32_t ai;
    int32_t broadcast;
    int32_t drop_free;
    int32_t corpse_time;
    int32_t boss_sign;
    int32_t BossSocre;
    int32_t TaskShare;
    int32_t camp;
    int32_t kill_point;
    int32_t revive_skin;
    int32_t prestige;
    int32_t dust;
    int32_t is_building;
    int32_t PureDamage;
    int32_t IfCurse;
    int32_t Score;
    int32_t IsShowOwner;
    int32_t DiligenceType;
    int32_t DiligenceValue;
    int32_t MonsterHp;
    int32_t CanTunshi;
    int32_t Mid2;
    int32_t FamilyValue;
    int32_t PortalRatio;
    int32_t PortalId;
    int32_t PortalDuration;
    int32_t IsXinMoMonster;
    int32_t QieGe;
    int32_t Energy;
    int32_t cItem;
    int32_t IsSunAndMoon;

    CfgMonster() = default;
    CfgMonster(const CfgMonster& a2);
    ~CfgMonster() = default;
    CfgMonster& operator=(const CfgMonster& a2);
};

#endif // _CFGMONSTER_H_
