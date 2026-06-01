#ifndef _CFGGROUPICON_H_
#define _CFGGROUPICON_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgGroupIcon - Config data class
 */
class CfgGroupIcon
{
public:
    int32_t nId;
    int32_t bShowInCross;
    std::string platfrom;

    CfgGroupIcon() = default;
    CfgGroupIcon(const CfgGroupIcon& a2);
    ~CfgGroupIcon() = default;
    CfgGroupIcon& operator=(const CfgGroupIcon& a2);
};

#endif // _CFGGROUPICON_H_
