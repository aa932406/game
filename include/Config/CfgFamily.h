#ifndef _CFGCFGFAMILY_H_
#define _CFGCFGFAMILY_H_

#include "Common/CommonTypes.h"
#include <cstdint>


/**
 * @brief CfgFamily - Config data class
 */
class CfgFamily
{
public:
    int32_t nLevel;
    int32_t nExp;
    int32_t nMaxMembers;
    int32_t nMaxBossPoints;
    int32_t vPosition_0;
    int32_t vPosition_1;
    int32_t vPosition_2;
    int32_t vPosition_3;

    CfgFamily() = default;
    ~CfgFamily() = default;
    CfgFamily(const CfgFamily& a2);
    CfgFamily& operator=(const CfgFamily& a2);
};

#endif // _CFGCFGFAMILY_H_
