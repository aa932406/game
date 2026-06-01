#ifndef _CFGSEVENLOGINREWRAD_H_
#define _CFGSEVENLOGINREWRAD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgSevenLoginRewrad - Config data class
 */
class CfgSevenLoginRewrad
{
public:
    int32_t Day;
    std::vector<CfgEquipItem> ItemVector;

    CfgSevenLoginRewrad() = default;
    CfgSevenLoginRewrad(const CfgSevenLoginRewrad& a2);
    ~CfgSevenLoginRewrad() = default;
    CfgSevenLoginRewrad& operator=(const CfgSevenLoginRewrad& a2);
};

#endif // _CFGSEVENLOGINREWRAD_H_
