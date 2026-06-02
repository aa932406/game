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
    const CfgWeiXingGift* GetGift(int32_t nIconId);
    bool IsGiftExist(int32_t nIconId);
    int32_t GetRewardCount(int32_t nIconId);
    void Clear();

public:
    std::map<int32_t, CfgWeiXingGift> m_mWeiXinGift;
};

#endif // _CFGCFGWEIXINTABLE_H_
