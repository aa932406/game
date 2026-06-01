#ifndef _CFGCFGPETEQUIPTABLE_H_
#define _CFGCFGPETEQUIPTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgPetEquip.h"


/**
 * @brief CfgPetEquipTable - Config data class
 */
class CfgPetEquipTable
{
public:
    CfgPetEquipTable();
    ~CfgPetEquipTable();

    void Add(const CfgPetEquip& stu);

public:
    std::map<int, CfgPetEquip> m_mPetEquip;
};

#endif // _CFGCFGPETEQUIPTABLE_H_
