#ifndef _CFGCFGAPPENDATTR_H_
#define _CFGCFGAPPENDATTR_H_

#include "Common/CommonTypes.h"
#include "Config/CfgData.h"
#include <cstdint>
#include <list>


/**
 * @brief CfgAppendAttr - Config data class
 */
#ifndef CFG_APPEND_ATTR_DEFINED
#define CFG_APPEND_ATTR_DEFINED
class CfgAppendAttr
{
public:
    int32_t Id;
    int32_t Job;
    std::list<AddAttribute> AttrList;

    CfgAppendAttr() = default;
    ~CfgAppendAttr() = default;
    CfgAppendAttr(const CfgAppendAttr& a2);
    CfgAppendAttr& operator=(const CfgAppendAttr& a2);
};

#endif // CFG_APPEND_ATTR_DEFINED
#endif // _CFGCFGAPPENDATTR_H_
