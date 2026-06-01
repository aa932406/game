#ifndef _CFGCFGEQUIPTABLE_H_
#define _CFGCFGEQUIPTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgEquip.h"


/**
 * @brief CfgEquipTable - Config data class
 */
class CfgEquipTable
{
public:
    CfgEquipTable();
    ~CfgEquipTable();

    void AddEquip(const CfgEquip& stu);

public:
    std::map<int, CfgEquip> m_mEquip;
};

#endif // _CFGCFGEQUIPTABLE_H_
