#ifndef _CFGTALENTACTIVE_H_
#define _CFGTALENTACTIVE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


#ifndef CFGTALENTACTIVE_DEFINED
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
#endif // CFGTALENTACTIVE_DEFINED

#endif // _CFGTALENTACTIVE_H_
