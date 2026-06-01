#ifndef _CFGPLANTEVENTEFFECT_H_
#define _CFGPLANTEVENTEFFECT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgPlantEventEffect - Config data class
 */
class CfgPlantEventEffect
{
public:
    int32_t EventId;
    int32_t EventType;
    std::string EventEffect;
    int32_t GongGaoId;

    CfgPlantEventEffect() = default;
    CfgPlantEventEffect(const CfgPlantEventEffect& a2);
    ~CfgPlantEventEffect() = default;
    CfgPlantEventEffect& operator=(const CfgPlantEventEffect& a2);
};

#endif // _CFGPLANTEVENTEFFECT_H_
