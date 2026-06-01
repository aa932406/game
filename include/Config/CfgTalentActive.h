#ifndef _CFGTALENTACTIVE_H_
#define _CFGTALENTACTIVE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgTalentActive - Config data class
 */
class CfgTalentActive
{
public:
    int32_t nId;
    std::list<ItemData> lItems;

    CfgTalentActive() = default;
    CfgTalentActive(const CfgTalentActive& a2);
    ~CfgTalentActive() = default;
    CfgTalentActive& operator=(const CfgTalentActive& a2);
};

#endif // _CFGTALENTACTIVE_H_
