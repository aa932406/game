#ifndef _CFGMYSTERYSHOPTABLE_H_
#define _CFGMYSTERYSHOPTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>


/**
 * @brief CfgMysteryShop - Mystery shop config entry
 */
struct CfgMysteryShop
{
    int32_t nId;
    int32_t nType;
    MemChrBag item;
    int32_t nCostType;
    int32_t nPrice;
    int32_t nRate;
    int32_t nMinLevel;
    int32_t nMaxLevel;
    int32_t nBroad;
    ItemData exchange;
};

/**
 * @brief CfgMysteryShopTable - Mystery shop config table
 */
class CfgMysteryShopTable
{
public:
    CfgMysteryShopTable();
    ~CfgMysteryShopTable();

    bool Add(const CfgMysteryShop& shop);
    const CfgMysteryShop* GetShopGood(int32_t nId);
    void CleanUp();

private:
    std::map<int32_t, CfgMysteryShop> m_mMysteryShop;
};

#endif // _CFGMYSTERYSHOPTABLE_H_
