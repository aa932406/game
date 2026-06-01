#ifndef _CFGKAIFUHUODONGDATA_H_
#define _CFGKAIFUHUODONGDATA_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


/**
 * @brief CfgKaiFuHuoDongData - Config data class
 */
class CfgKaiFuHuoDongData
{
public:
    int32_t Index;
    int32_t Type;
    int32_t Level;
    int32_t StartDay;
    int32_t EndDay;
    int32_t GetRewardDay;
    int32_t Conditions;
    std::vector<MemChrBag> ItemVector;
    std::string ItemString;
    int32_t PetAttr;
    int32_t GongGaoId;
    int32_t Limit;
    int32_t PetEffect;
    int32_t Conditions1;
    std::string FlopString;

    CfgKaiFuHuoDongData() = default;
    CfgKaiFuHuoDongData(const CfgKaiFuHuoDongData& a2);
    ~CfgKaiFuHuoDongData() = default;
    CfgKaiFuHuoDongData& operator=(const CfgKaiFuHuoDongData& a2);
};

#endif // _CFGKAIFUHUODONGDATA_H_
