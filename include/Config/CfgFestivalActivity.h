#ifndef _CFGCFGFESTIVALACTIVITY_H_
#define _CFGCFGFESTIVALACTIVITY_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <vector>
#include <string>


/**
 * @brief CfgFestivalMapPos - Position data for festival activity plants/NPCs
 */
struct CfgFestivalMapPos
{
    int32_t nMapId;
    int32_t nX;
    int32_t nY;

    CfgFestivalMapPos() : nMapId(0), nX(0), nY(0) {}
    CfgFestivalMapPos(const CfgFestivalMapPos& a2) = default;
    ~CfgFestivalMapPos() = default;
    CfgFestivalMapPos& operator=(const CfgFestivalMapPos& a2) = default;
};

/**
 * @brief CfgChangeItemCfg - Exchange item config for festival activities
 */
struct CfgChangeItemCfg
{
    int32_t nTimes;
    int32_t nCostGold;
    std::vector<MemChrBag> vGetItems;
    std::vector<MemChrBag> vCostItems;

    CfgChangeItemCfg() : nTimes(0), nCostGold(0) {}
    CfgChangeItemCfg(const CfgChangeItemCfg& a2) = default;
    ~CfgChangeItemCfg() = default;
    CfgChangeItemCfg& operator=(const CfgChangeItemCfg& a2) = default;
};

/**
 * @brief CfgFestivalActivity - Config data class for festival activities
 */
class CfgFestivalActivity
{
public:
    int32_t nIcon;
    int32_t nStartDay;
    int32_t nEndDay;
    int32_t nPlantId;
    int32_t nPlantCount;
    int32_t nNpcId;
    int32_t nNpcCount;
    int32_t nTimes;
    int32_t nPlantTime;
    MemChrBag dayGift;
    MemChrBag actGift;
    std::vector<int32_t> vStartDayList;
    std::vector<int32_t> vEndDayList;
    std::vector<CfgFestivalMapPos> vPlantPosList;
    std::vector<CfgFestivalMapPos> vNpcPosList;
    std::vector<CfgChangeItemCfg> vChangeCfgList;
    std::vector<std::vector<std::string>> vAnswerList;

    CfgFestivalActivity() = default;
    CfgFestivalActivity(const CfgFestivalActivity& a2) = default;
    ~CfgFestivalActivity() = default;
    CfgFestivalActivity& operator=(const CfgFestivalActivity& a2) = default;
};

#endif // _CFGCFGFESTIVALACTIVITY_H_
