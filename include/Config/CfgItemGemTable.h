#ifndef _CFGCFGITEMGEMTABLE_H_
#define _CFGCFGITEMGEMTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include <vector>
#include "Config/CfgItemGem.h"


/**
 * @brief CfgItemGemTable - Config data class
 */
class CfgItemGemTable
{
public:
    CfgItemGemTable();
    ~CfgItemGemTable();

    void Add(const CfgItemGem& stu);

public:
    std::map<int, std::vector<CfgItemGem>> m_mItemGem;
};

#endif // _CFGCFGITEMGEMTABLE_H_
