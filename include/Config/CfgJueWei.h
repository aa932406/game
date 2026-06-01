#ifndef _CFGJUEWEI_H_
#define _CFGJUEWEI_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgJueWei - Config data class
 */
class CfgJueWei
{
public:
    int32_t nId;
    int32_t nNeedLevel;
    int32_t nNeedViGour;
    std::vector<AttrAddon> vAttr;

    CfgJueWei() = default;
    CfgJueWei(const CfgJueWei& a2);
    ~CfgJueWei() = default;
    CfgJueWei& operator=(const CfgJueWei& a2);
};

#endif // _CFGJUEWEI_H_
