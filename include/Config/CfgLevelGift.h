#ifndef _CFGLEVELGIFT_H_
#define _CFGLEVELGIFT_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>


/**
 * @brief CfgLevelGift - Config data class
 */
class CfgLevelGift
{
public:
    int32_t Index;
    int32_t Level;
    std::vector<MemChrBag> ItemVector;

    CfgLevelGift() = default;
    CfgLevelGift(const CfgLevelGift& a2);
    ~CfgLevelGift() = default;
    CfgLevelGift& operator=(const CfgLevelGift& a2);
};

#endif // _CFGLEVELGIFT_H_
