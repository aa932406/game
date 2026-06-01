#ifndef _CFGCFGDRAWTABLE_H_
#define _CFGCFGDRAWTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>
#include "Config/CfgDraw.h"


/**
 * @brief CfgDrawTable - Config data class
 */
class CfgDrawTable
{
public:
    CfgDrawTable();
    ~CfgDrawTable();

    void AddDraw(const CfgDraw& draw);

public:
    std::vector<CfgDraw> m_vDraw;
};

#endif // _CFGCFGDRAWTABLE_H_
