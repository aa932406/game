#ifndef _CFGCFGEQUIPBOXTABLE_H_
#define _CFGCFGEQUIPBOXTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgEquipBox.h"


/**
 * @brief CfgEquipBoxTable - Config data class
 */
class CfgEquipBoxTable
{
public:
    CfgEquipBoxTable();
    ~CfgEquipBoxTable();

    void Add(const CfgEquipBox& stu);

public:
    std::map<int, CfgEquipBox> m_mEquipBox;
};

#endif // _CFGCFGEQUIPBOXTABLE_H_
