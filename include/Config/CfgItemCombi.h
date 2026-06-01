#ifndef _CFGITEMCOMBI_H_
#define _CFGITEMCOMBI_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgItemCombi - Config data class
 */
class CfgItemCombi
{
public:
    int32_t m_nId;
    std::list<ItemData> m_lCostItem;
    std::vector<MemChrBag> m_vGiveItem;
    int32_t m_nMoney;
    int32_t m_nRate;
    int32_t m_nTotalRate;
    int32_t m_bBroadcast;
    std::vector<MemChrBag> m_vBackItem;
    int32_t m_bNeedActive;
    int32_t m_nCostPoint;
    std::vector<MemChrBag> m_vSpecialItems;
    std::vector<std::vector<int>> m_vProbability;
    int32_t m_nCombiType;

    CfgItemCombi() = default;
    CfgItemCombi(const CfgItemCombi& a2);
    ~CfgItemCombi() = default;
    CfgItemCombi& operator=(const CfgItemCombi& a2);
};

#endif // _CFGITEMCOMBI_H_
