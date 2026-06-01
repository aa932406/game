#ifndef _CFGCFGWISHREWARDTABLE_H_
#define _CFGCFGWISHREWARDTABLE_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>
#include "Config/CfgWishReward.h"


/**
 * @brief CfgWishRewardTable - Config data class
 */
class CfgWishRewardTable
{
public:
    CfgWishRewardTable();
    ~CfgWishRewardTable();

    void Add(const CfgWishReward& stu);

    const CfgWishReward* GetReward(int32_t nTime);
    const CfgWishReward* GetRewardByIndex(int32_t nIndex);
    int32_t GetRewardCount();
    int32_t GetMaxTime();
    int32_t GetMinTime();
    void Clear();

public:
    std::vector<CfgWishReward> m_vWishReward;
};

#endif // _CFGCFGWISHREWARDTABLE_H_
