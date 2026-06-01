#ifndef _CFGCFGWEIXINTABLE_H_
#define _CFGCFGWEIXINTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgWeiXingGift.h"


/**
 * @brief CfgWeiXinTable - Config data class
 */
class CfgWeiXinTable
{
public:
    CfgWeiXinTable();
    ~CfgWeiXinTable();

    void Add(const CfgWeiXingGift& stu);

public:
    std::map<int, CfgWeiXingGift> m_mWeiXinGift;
};

#endif // _CFGCFGWEIXINTABLE_H_
