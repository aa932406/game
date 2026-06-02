#ifndef _CFGCACHET_H_
#define _CFGCACHET_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>


#ifndef ADD_ATTRIBUTE_DEFINED
#define ADD_ATTRIBUTE_DEFINED
struct AddAttribute {
    int m_nAddAttrType;
    int m_nAddAttrValue;
};
#endif

/**
 * @brief CfgCachet - Config data class
 */
#ifndef CFG_CACHET_DEFINED
#define CFG_CACHET_DEFINED
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

#endif // CFG_CACHET_DEFINED
#endif // _CFGCACHET_H_
