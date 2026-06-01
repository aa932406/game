#ifndef _CFGSOUGOUSKIN_H_
#define _CFGSOUGOUSKIN_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgSouGouSkin - Config data class
 */
class CfgSouGouSkin
{
public:
    int32_t nIcon;
    std::vector<MemChrBag> vReward;

    CfgSouGouSkin() = default;
    CfgSouGouSkin(const CfgSouGouSkin& a2);
    ~CfgSouGouSkin() = default;
    CfgSouGouSkin& operator=(const CfgSouGouSkin& a2);
};

#endif // _CFGSOUGOUSKIN_H_
