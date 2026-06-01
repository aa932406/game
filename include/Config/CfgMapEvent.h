#ifndef _CFGMAPEVENT_H_
#define _CFGMAPEVENT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


// 如果 CfgData.h 已经定义了 struct CfgMapEvent，则不再重复定义
#ifndef CFGMAPEVENT_STRUCT_DEFINED

/**
 * @brief CfgMapEvent - Config data class
 */
class CfgMapEvent
{
public:
    int32_t id;
    int32_t mapid;
    int32_t trigger_id;
    int32_t trigger_type;
    std::vector<int> trigger_param;
    int32_t event_type;
    std::string effect;
    int32_t repeate;
    int32_t nOpenTime;
    int32_t nTrigTime;
    int32_t nCloseTime;
    int32_t nTrigTimes;
    int32_t bInitOpen;

    CfgMapEvent() = default;
    CfgMapEvent(const CfgMapEvent& a2);
    ~CfgMapEvent() = default;
    CfgMapEvent& operator=(const CfgMapEvent& a2);
};

#endif // CFGMAPEVENT_STRUCT_DEFINED

#endif // _CFGMAPEVENT_H_
