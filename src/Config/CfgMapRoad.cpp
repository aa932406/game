#include "Config/CfgMapRoad.h"


CfgMapRoad::CfgMapRoad(const CfgMapRoad& a2)
{
    nIndex = a2.nIndex;
    nNextIndex = a2.nNextIndex;
    nMapId = a2.nMapId;
    road = a2.road;
}

CfgMapRoad& CfgMapRoad::operator=(const CfgMapRoad& a2)
{
    if (this != &a2)
    {
        nIndex = a2.nIndex;
        nNextIndex = a2.nNextIndex;
        nMapId = a2.nMapId;
        road = a2.road;
    }
    return *this;
}
