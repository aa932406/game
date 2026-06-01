#ifndef _CFGLEVELATTR_H_
#define _CFGLEVELATTR_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgLevelAttr - Config data class
 */
class CfgLevelAttr
{
public:
    int32_t job;
    int32_t level;
    std::vector<AttrAddon> addonattr;
    std::vector<AttrAddon> addonPoint;

    CfgLevelAttr() = default;
    CfgLevelAttr(const CfgLevelAttr& a2);
    ~CfgLevelAttr() = default;
    CfgLevelAttr& operator=(const CfgLevelAttr& a2);
};

#endif // _CFGLEVELATTR_H_
