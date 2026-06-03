#ifndef _CFGDUNGEON_H_
#define _CFGDUNGEON_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <string>
#include <vector>


#ifndef CFGDUNGEON_DEFINED
/**
 * @brief CfgDungeon - Config data class
 */
class CfgDungeon
{
public:
    int32_t id;
    std::string name;
    int32_t mapid;
    int32_t x;
    int32_t y;
    int32_t type;
    int32_t sequence;
    int32_t group_id;
    int32_t duration;
    int32_t job;
    int32_t level;
    int32_t maxLevel;
    int32_t vip;
    int32_t last_id;
    int32_t next_id;
    int32_t player_num;
    int32_t weekday;
    int32_t start_hour;
    int32_t end_hour;
    int32_t daily_count;
    int32_t player_buff;
    int32_t star;
    int32_t double_cost;
    int32_t reward_time;
    int32_t costItem;
    std::vector<MemChrBag> rewardOnce;
    std::vector<MemChrBag> rewardItem;
    int32_t Battle;
    int32_t costGold;
    int32_t costMoney;
    int32_t buyTimes;
    int32_t buyCost;
    int32_t quickDoneCost;
    int32_t quickDoneDrop;
    int32_t needUpTowerStar;
    int32_t summon_boss;
    int32_t summon_cost;
    int32_t rand_time;
    int32_t task_id;
    int32_t stay_position;
    std::vector<int> win_star;
    std::vector<MemChrBag> star_reward;
    std::vector<int> star_ratio;
    int32_t TeQuan;
    int32_t backMapId;
    int32_t backX;
    int32_t backY;
    int32_t CanEnter;
    int32_t BuffId;

    CfgDungeon() = default;
    CfgDungeon(const CfgDungeon& a2);
    ~CfgDungeon() = default;
    CfgDungeon& operator=(const CfgDungeon& a2);
};
#endif // CFGDUNGEON_DEFINED

#endif // _CFGDUNGEON_H_
