#ifndef _CFGSHIZHUANG_H_
#define _CFGSHIZHUANG_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgShiZhuang - Config data class
 */
class CfgShiZhuang
{
public:
    int32_t nId;
    int32_t nType;
    std::list<AddAttribute> vAttr;
    int32_t nSuitId;

    CfgShiZhuang() = default;
    CfgShiZhuang(const CfgShiZhuang& a2);
    ~CfgShiZhuang() = default;
    CfgShiZhuang& operator=(const CfgShiZhuang& a2);
};

#endif // _CFGSHIZHUANG_H_
