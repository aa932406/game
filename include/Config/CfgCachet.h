#ifndef _CFGCACHET_H_
#define _CFGCACHET_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgCachet - Config data class
 */
class CfgCachet
{
public:
    int32_t nLevel;
    int32_t nNeedCacht;
    std::list<AddAttribute> nAttrList;
    int32_t nDeduct;
    int32_t nGongGaoId;

    CfgCachet() = default;
    CfgCachet(const CfgCachet& a2);
    ~CfgCachet() = default;
    CfgCachet& operator=(const CfgCachet& a2);
};

#endif // _CFGCACHET_H_
