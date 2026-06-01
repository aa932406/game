#ifndef _CFGOUTLINKFESTIVAL_H_
#define _CFGOUTLINKFESTIVAL_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgOutLinkFestival - Config data class
 */
class CfgOutLinkFestival
{
public:
    int32_t nIndex;
    int32_t nStartTime;
    int32_t nEndTime;
    int32_t nIcon;
    std::string strPlatfrom;

    CfgOutLinkFestival() = default;
    CfgOutLinkFestival(const CfgOutLinkFestival& a2);
    ~CfgOutLinkFestival() = default;
    CfgOutLinkFestival& operator=(const CfgOutLinkFestival& a2);
};

#endif // _CFGOUTLINKFESTIVAL_H_
