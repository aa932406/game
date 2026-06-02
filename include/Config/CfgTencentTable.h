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
    const CfgTencentGift* GetGift(int32_t nLevel);
    const CfgTencentGift* GetVipGift(int32_t nLevel);
    const CfgTencentSevenDayLogin* GetSevenDayLogin(int32_t nDays);
    int32_t GetMaxGiftLevel();
    int32_t GetMaxVipGiftLevel();
    void Clear();

public:
    std::map<int32_t, CfgTencentGift> m_mTencentGift;
    std::map<int32_t, CfgTencentGift> m_mTencentVipGift;
    std::map<int32_t, CfgTencentSevenDayLogin> m_mTencentSevenDayLogin;
};

#endif // _CFGCFGTENCENTTABLE_H_
