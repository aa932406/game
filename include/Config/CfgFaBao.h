#ifndef _CFGFABAO_H_
#define _CFGFABAO_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgFaBao - Config data class
 */
class CfgFaBao
{
public:
    int32_t FaBaoLevel;
    int32_t NeedCurr;
    int32_t nNeedLevel;
    std::vector<AttrAddon> vAttr;
    int32_t GongGaoId;

    CfgFaBao() = default;
    CfgFaBao(const CfgFaBao& a2);
    ~CfgFaBao() = default;
    CfgFaBao& operator=(const CfgFaBao& a2);
};

#endif // _CFGFABAO_H_
