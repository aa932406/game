#include "Config/CfgPlantEventEffect.h"


CfgPlantEventEffect::CfgPlantEventEffect(const CfgPlantEventEffect& a2)
{
    EventId = a2.EventId;
    EventType = a2.EventType;
    EventEffect = a2.EventEffect;
    GongGaoId = a2.GongGaoId;
}

CfgPlantEventEffect& CfgPlantEventEffect::operator=(const CfgPlantEventEffect& a2)
{
    if (this != &a2)
    {
        EventId = a2.EventId;
        EventType = a2.EventType;
        EventEffect = a2.EventEffect;
        GongGaoId = a2.GongGaoId;
    }
    return *this;
}
