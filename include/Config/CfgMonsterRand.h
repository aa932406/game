#ifndef _CFGMONSTERRAND_H_
#define _CFGMONSTERRAND_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMonsterRand - Config data class
 */
class CfgMonsterRand
{
public:
    int32_t nId;
    std::vector<AttrAddon> vAttrValue;
    std::vector<AttrAddon> vAttrRatio;
    int32_t nSkill;

    CfgMonsterRand() = default;
    CfgMonsterRand(const CfgMonsterRand& a2);
    ~CfgMonsterRand() = default;
    CfgMonsterRand& operator=(const CfgMonsterRand& a2);
};

#endif // _CFGMONSTERRAND_H_
