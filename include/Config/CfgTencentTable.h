#ifndef _CFGCFGTENCENTTABLE_H_
#define _CFGCFGTENCENTTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <map>
#include "Config/CfgTencentGift.h"
#include "Config/CfgTencentSevenDayLogin.h"


/**
 * @brief CfgTencentTable - Config data class
 */
class CfgTencentTable
{
public:
    CfgTencentTable();
    ~CfgTencentTable();

    void Add(const CfgTencentGift& stu);
    void AddSevenDayLogin(const CfgTencentSevenDayLogin& stu);

public:
    std::map<int, CfgTencentGift> m_mTencentGift;
    std::map<int, CfgTencentSevenDayLogin> m_mTencentSevenDayLogin;
};

#endif // _CFGCFGTENCENTTABLE_H_
