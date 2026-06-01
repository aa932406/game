#ifndef _CFGCFGTGPGIFT_H_
#define _CFGCFGTGPGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


/**
 * @brief CfgTGPGift - Config data class
 */
class CfgTGPGift
{
public:
    std::string strPF;
    int32_t nIndex;
    int32_t nLevel;
    std::vector<MemChrBag> vReward;

    CfgTGPGift() = default;
    ~CfgTGPGift() = default;
    CfgTGPGift(const CfgTGPGift& a2);
    CfgTGPGift& operator=(const CfgTGPGift& a2);
};

#endif // _CFGCFGTGPGIFT_H_
