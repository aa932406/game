#ifndef _CFGCFGDRAWREWARD_H_
#define _CFGCFGDRAWREWARD_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


/**
 * @brief CfgDrawReward - Config data class
 */
class CfgDrawReward
{
public:
    int32_t nIndex;
    int32_t nType;
    int32_t nRate;
    int32_t nBroad;
    std::vector<MemChrBag> vItem;
    std::string strGoodsName;

    CfgDrawReward() = default;
    ~CfgDrawReward() = default;
    CfgDrawReward(const CfgDrawReward& a2);
    CfgDrawReward& operator=(const CfgDrawReward& a2);
};

#endif // _CFGCFGDRAWREWARD_H_
