#ifndef _CFGTOTALCHONGZHI_H_
#define _CFGTOTALCHONGZHI_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>

#ifndef CFG_GONG_GAO_DEFINED
#define CFG_GONG_GAO_DEFINED
struct CfgGongGao {
    // 成员变量
};
#endif /* CFG_GONG_GAO_DEFINED */


/**
 * @brief CfgTotalChongZhi - Config data class
 */
class CfgTotalChongZhi
{
public:
    int32_t Index;
    int32_t NeedGold;
    MemChrEquipBagVector ItemVector;
    std::list<CfgGongGao> GongGaoInfo;
    int32_t NewServerday;

    CfgTotalChongZhi() = default;
    CfgTotalChongZhi(const CfgTotalChongZhi& a2);
    ~CfgTotalChongZhi() = default;
    CfgTotalChongZhi& operator=(const CfgTotalChongZhi& a2);
};

#endif // _CFGTOTALCHONGZHI_H_
