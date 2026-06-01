#ifndef _CFGCFGITEMBASE_H_
#define _CFGCFGITEMBASE_H_

#include <cstdint>
#include <string>


/**
 * @brief CfgItemBase - Config data class
 */
class CfgItemBase
{
public:
    std::string name;
    std::string desc;
    std::string url;
    std::string drop_url;

    CfgItemBase() = default;
    ~CfgItemBase() = default;
    CfgItemBase(const CfgItemBase& a2);
    CfgItemBase& operator=(const CfgItemBase& a2);
};

#endif // _CFGCFGITEMBASE_H_
