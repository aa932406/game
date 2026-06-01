#ifndef _CFGPETUPSTAR_H_
#define _CFGPETUPSTAR_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgPetUpStar - Config data class
 */
class CfgPetUpStar
{
public:
    int32_t nStar;
    int32_t nCostMoney;
    std::list<ItemData> lCostItem;
    std::vector<AttrAddon> vAddPetAttr;
    std::vector<AttrAddon> vAddPlayerAttr;
    std::vector<int> vSkillLevel;
    int32_t GongGaoId;
    int32_t nCostGold;

    CfgPetUpStar() = default;
    CfgPetUpStar(const CfgPetUpStar& a2);
    ~CfgPetUpStar() = default;
    CfgPetUpStar& operator=(const CfgPetUpStar& a2);
};

#endif // _CFGPETUPSTAR_H_
