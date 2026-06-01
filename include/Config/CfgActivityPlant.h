#ifndef _CFGACTIVITYPLANT_H_
#define _CFGACTIVITYPLANT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgActivityPlant - Config data class
 */
class CfgActivityPlant
{
public:
    int32_t id;
    int32_t activity_id;
    int32_t plant_id;
    int32_t count;
    int32_t wave;
    int32_t region_id;
    int32_t whoplant;
    int32_t life_time;
    std::vector<Position> EnterPosVector;

    CfgActivityPlant() = default;
    CfgActivityPlant(const CfgActivityPlant& a2);
    ~CfgActivityPlant() = default;
    CfgActivityPlant& operator=(const CfgActivityPlant& a2);
};

#endif // _CFGACTIVITYPLANT_H_
