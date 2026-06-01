#ifndef _CFGMAPPLANT_H_
#define _CFGMAPPLANT_H_

#include <cstdint>

class CfgMapPlant
{
public:
    CfgMapPlant() = default;
    virtual ~CfgMapPlant() = default;

    int32_t id;
    int32_t plantId;
    int32_t posX;
    int32_t posY;
    int32_t lifeTime;
    int32_t refreshTime;
    int32_t maxCount;
};

#endif // _CFGMAPPLANT_H_
