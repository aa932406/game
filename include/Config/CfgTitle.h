#ifndef _CFGTITLE_H_
#define _CFGTITLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


/**
 * @brief CfgTitle - Config data class
 */
class CfgTitle
{
public:
    int32_t nId;
    int32_t nType;
    std::vector<int> vParams;
    std::string sPlatform;
    int32_t nJob;
    int32_t nSex;
    int32_t nSpecial;
    int32_t nPriority;
    std::vector<AttrAddon> vGetAttr;
    std::vector<AttrAddon> vDressAttr;

    CfgTitle() = default;
    CfgTitle(const CfgTitle& a2);
    ~CfgTitle() = default;
    CfgTitle& operator=(const CfgTitle& a2);
};

#endif // _CFGTITLE_H_
