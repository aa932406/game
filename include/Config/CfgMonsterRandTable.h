#ifndef _CFGCFGMONSTERRANDTABLE_H_
#define _CFGCFGMONSTERRANDTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include <vector>
#include "Config/CfgMonsterRand.h"


/**
 * @brief CfgMonsterRandTable - Config data class
 */
class CfgMonsterRandTable
{
public:
    CfgMonsterRandTable();
    ~CfgMonsterRandTable();

    void AddMonsterRand(const CfgMonsterRand& stu);

public:
    std::vector<int> m_vId;
    std::map<int, CfgMonsterRand> m_mMonsterRand;
};

#endif // _CFGCFGMONSTERRANDTABLE_H_
