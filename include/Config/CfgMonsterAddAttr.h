#ifndef _CFGCFGMONSTERADDATTR_H_
#define _CFGCFGMONSTERADDATTR_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgMonsterAddAttr - Config data class
 */
class CfgMonsterAddAttr
{
public:
    int32_t WorldBossLevelMin;
    int32_t WorldBossLevelMax;
    std::vector<AttrAddon> AttrVector;

    CfgMonsterAddAttr() = default;
    ~CfgMonsterAddAttr() = default;
    CfgMonsterAddAttr(const CfgMonsterAddAttr& a2);
    CfgMonsterAddAttr& operator=(const CfgMonsterAddAttr& a2);
};

#endif // _CFGCFGMONSTERADDATTR_H_
