#ifndef _CFGTASK_H_
#define _CFGTASK_H_

#include "Common/CommonTypes.h"
#include <cstdint>
#include <list>
#include <vector>


/**
 * @brief CfgTask - Config data class
 */
class CfgTask
{
public:
    int32_t id;
    int32_t type;
    int32_t group;
    int32_t can_giveup;
    int32_t pretask;
    int32_t posttask;
    int32_t main_order;
    int32_t job;
    int32_t level;
    int32_t max_level;
    int32_t kingdom;
    int32_t start_npc;
    int32_t end_npc;
    std::vector<MemChrBag> items_receive;
    int32_t award_exp;
    int32_t award_money;
    int32_t gold;
    int32_t dilong;
    int32_t rongyu;
    int32_t fuwen;
    int32_t BossScore;
    int32_t dungeon;
    std::vector<MemChrBag> award_item;
    std::vector<MemChrJobBag> award_optional;
    int32_t condition;
    int32_t done_count;
    int32_t request;
    int32_t JunTuanZiJin;
    int32_t GongXian;
    int32_t Double;
    int32_t acScore;
    int32_t quickDoneCost;
    int32_t quality;
    int32_t ratio;
    int32_t dust;
    int32_t activity_score;
    int32_t talent_point;
    std::list<TaskDrop> drop_list;
    int32_t DoubleGold;
    int32_t BuffId;

    CfgTask() = default;
    CfgTask(const CfgTask& a2);
    ~CfgTask() = default;
    CfgTask& operator=(const CfgTask& a2);
};

#endif // _CFGTASK_H_
