#include "Config/CfgMapEvent.h"


CfgMapEvent::CfgMapEvent(const CfgMapEvent& a2)
{
    id = a2.id;
    mapid = a2.mapid;
    trigger_id = a2.trigger_id;
    trigger_type = a2.trigger_type;
    trigger_param = a2.trigger_param;
    event_type = a2.event_type;
    effect = a2.effect;
    repeate = a2.repeate;
    nOpenTime = a2.nOpenTime;
    nTrigTime = a2.nTrigTime;
    nCloseTime = a2.nCloseTime;
    nTrigTimes = a2.nTrigTimes;
    bInitOpen = a2.bInitOpen;
}

CfgMapEvent& CfgMapEvent::operator=(const CfgMapEvent& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        mapid = a2.mapid;
        trigger_id = a2.trigger_id;
        trigger_type = a2.trigger_type;
        trigger_param = a2.trigger_param;
        event_type = a2.event_type;
        effect = a2.effect;
        repeate = a2.repeate;
        nOpenTime = a2.nOpenTime;
        nTrigTime = a2.nTrigTime;
        nCloseTime = a2.nCloseTime;
        nTrigTimes = a2.nTrigTimes;
        bInitOpen = a2.bInitOpen;
    }
    return *this;
}
