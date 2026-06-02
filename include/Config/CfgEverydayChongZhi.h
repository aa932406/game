#ifndef _CFGCFGEVERYDAYCHONGZHI_H_
#define _CFGCFGEVERYDAYCHONGZHI_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgEverydayChongZhi - Config data class
 */
class CfgEverydayChongZhi
{
public:
    int32_t Index;
    int32_t Type;
    int32_t NeedGold;
    MemChrEquipBagVector ItemVector;
    std::list<CfgGongGao> GongGaoInfo;

    CfgEverydayChongZhi() = default;
    ~CfgEverydayChongZhi() = default;
    CfgEverydayChongZhi(const CfgEverydayChongZhi& a2);
    CfgEverydayChongZhi& operator=(const CfgEverydayChongZhi& a2);
};

#endif // _CFGCFGEVERYDAYCHONGZHI_H_
