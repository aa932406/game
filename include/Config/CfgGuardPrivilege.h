#ifndef _CFGGUARDPRIVILEGE_H_
#define _CFGGUARDPRIVILEGE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgGuardPrivilege - Config data class
 */
class CfgGuardPrivilege
{
public:
    int32_t nIndex;
    int32_t nType;
    int32_t nDaily;
    int32_t nParam;
    std::vector<MemChrBag> vReward;

    CfgGuardPrivilege() = default;
    CfgGuardPrivilege(const CfgGuardPrivilege& a2);
    ~CfgGuardPrivilege() = default;
    CfgGuardPrivilege& operator=(const CfgGuardPrivilege& a2);
};

#endif // _CFGGUARDPRIVILEGE_H_
