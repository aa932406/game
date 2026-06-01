#ifndef _CFGCFGEQUIPBLESSTABLE_H_
#define _CFGCFGEQUIPBLESSTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgEquipBless.h"


/**
 * @brief CfgEquipBlessTable - Config data class
 */
class CfgEquipBlessTable
{
public:
    CfgEquipBlessTable();
    ~CfgEquipBlessTable();

    void Add(const CfgEquipBless& stu);

public:
    std::map<int, CfgEquipBless> m_mEquipBless;
};

#endif // _CFGCFGEQUIPBLESSTABLE_H_
