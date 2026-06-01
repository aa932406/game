#ifndef _CFGCFGEQUIPBOX_H_
#define _CFGCFGEQUIPBOX_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgEquipBox - Config data class
 */
class CfgEquipBox
{
public:
    int32_t nId;
    int32_t nWeight;
    int32_t nBind;
    std::list<Param2> lstStar;
    int32_t nProbility;

    CfgEquipBox() = default;
    ~CfgEquipBox() = default;
    CfgEquipBox(const CfgEquipBox& a2);
    CfgEquipBox& operator=(const CfgEquipBox& a2);
};

#endif // _CFGCFGEQUIPBOX_H_
