#ifndef _CFGACTIVITY_H_
#define _CFGACTIVITY_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgActivity - Config data class
 */
class CfgActivity
{
public:
    int32_t id;
    int32_t typeId;
    std::vector<int> weekday;
    int32_t begin_date;
    int32_t end_date;
    int32_t line;
    std::vector<int> start_hour;
    int32_t duration;
    int32_t level;
    int32_t world_event_chapter;
    std::vector<int> maps;
    int32_t target_mapid;
    std::vector<int> target_regiona;
    std::vector<int> target_regionb;
    int32_t state;
    std::vector<int> gift_id;
    std::vector<int> daily_reward;
    int32_t exp_add_interval;
    int32_t iconid;
    int32_t StartGongGao;

    CfgActivity() = default;
    CfgActivity(const CfgActivity& a2);
    ~CfgActivity() = default;
    CfgActivity& operator=(const CfgActivity& a2);
};

#endif // _CFGACTIVITY_H_
