#ifndef _CFGCFGITEM_H_
#define _CFGCFGITEM_H_

#include "Common/CommonTypes.h"
#include "Config/CfgItemBase.h"
#include <cstdint>
#include <string>


/**
 * @brief CfgItem - Config data class
 */
class CfgItem : public CfgItemBase
{
public:
    std::string effect;
    std::string use_method;
    Param2 RongHeReceovery;

    CfgItem() = default;
    ~CfgItem() = default;
    CfgItem(const CfgItem& a2);
    CfgItem& operator=(const CfgItem& a2);
};

#endif // _CFGCFGITEM_H_
