#ifndef _CFGPLANT_H_
#define _CFGPLANT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>

// CfgPlantEvent struct (needed by CfgPlant)
#ifndef CFGPLANTEVENT_STRUCT_DEFINED
#define CFGPLANTEVENT_STRUCT_DEFINED
struct CfgPlantEvent
{
    int32_t EventId;
    int32_t Probability;
    
    CfgPlantEvent() : EventId(0), Probability(0) {}
};
#endif

/**
 * @brief CfgPlant - Config data class
 */
class CfgPlant
{
public:
    int32_t id;
    int32_t type;
    int32_t level;
    std::vector<CfgPlantEvent> Events;
    int32_t EventMaxRate;
    int32_t item_cost;
    int32_t start_hour;
    int32_t end_hour;
    int32_t gather_time;
    int32_t revive_time;
    int32_t hide_time;
    int32_t get_points;
    int32_t boss_id;
    int32_t BuffId;
    int32_t revive_skin;
    int32_t TaskId;
    std::vector<int> ItemVt;

    CfgPlant() = default;
    CfgPlant(const CfgPlant& a2);
    ~CfgPlant() = default;
    CfgPlant& operator=(const CfgPlant& a2);
};

#endif // _CFGPLANT_H_
