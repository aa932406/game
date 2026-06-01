#include "Config/CfgDungeon.h"


CfgDungeon::CfgDungeon(const CfgDungeon& a2)
{
    id = a2.id;
    name = a2.name;
    mapid = a2.mapid;
    x = a2.x;
    y = a2.y;
    type = a2.type;
    sequence = a2.sequence;
    group_id = a2.group_id;
    duration = a2.duration;
    job = a2.job;
    level = a2.level;
    maxLevel = a2.maxLevel;
    vip = a2.vip;
    last_id = a2.last_id;
    next_id = a2.next_id;
    player_num = a2.player_num;
    weekday = a2.weekday;
    start_hour = a2.start_hour;
    end_hour = a2.end_hour;
    daily_count = a2.daily_count;
    player_buff = a2.player_buff;
    star = a2.star;
    double_cost = a2.double_cost;
    reward_time = a2.reward_time;
    costItem = a2.costItem;
    rewardOnce = a2.rewardOnce;
    rewardItem = a2.rewardItem;
    Battle = a2.Battle;
    costGold = a2.costGold;
    costMoney = a2.costMoney;
    buyTimes = a2.buyTimes;
    buyCost = a2.buyCost;
    quickDoneCost = a2.quickDoneCost;
    quickDoneDrop = a2.quickDoneDrop;
    needUpTowerStar = a2.needUpTowerStar;
    summon_boss = a2.summon_boss;
    summon_cost = a2.summon_cost;
    rand_time = a2.rand_time;
    task_id = a2.task_id;
    stay_position = a2.stay_position;
    win_star = a2.win_star;
    star_reward = a2.star_reward;
    star_ratio = a2.star_ratio;
    TeQuan = a2.TeQuan;
    backMapId = a2.backMapId;
    backX = a2.backX;
    backY = a2.backY;
    CanEnter = a2.CanEnter;
    BuffId = a2.BuffId;
}

CfgDungeon& CfgDungeon::operator=(const CfgDungeon& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        name = a2.name;
        mapid = a2.mapid;
        x = a2.x;
        y = a2.y;
        type = a2.type;
        sequence = a2.sequence;
        group_id = a2.group_id;
        duration = a2.duration;
        job = a2.job;
        level = a2.level;
        maxLevel = a2.maxLevel;
        vip = a2.vip;
        last_id = a2.last_id;
        next_id = a2.next_id;
        player_num = a2.player_num;
        weekday = a2.weekday;
        start_hour = a2.start_hour;
        end_hour = a2.end_hour;
        daily_count = a2.daily_count;
        player_buff = a2.player_buff;
        star = a2.star;
        double_cost = a2.double_cost;
        reward_time = a2.reward_time;
        costItem = a2.costItem;
        rewardOnce = a2.rewardOnce;
        rewardItem = a2.rewardItem;
        Battle = a2.Battle;
        costGold = a2.costGold;
        costMoney = a2.costMoney;
        buyTimes = a2.buyTimes;
        buyCost = a2.buyCost;
        quickDoneCost = a2.quickDoneCost;
        quickDoneDrop = a2.quickDoneDrop;
        needUpTowerStar = a2.needUpTowerStar;
        summon_boss = a2.summon_boss;
        summon_cost = a2.summon_cost;
        rand_time = a2.rand_time;
        task_id = a2.task_id;
        stay_position = a2.stay_position;
        win_star = a2.win_star;
        star_reward = a2.star_reward;
        star_ratio = a2.star_ratio;
        TeQuan = a2.TeQuan;
        backMapId = a2.backMapId;
        backX = a2.backX;
        backY = a2.backY;
        CanEnter = a2.CanEnter;
        BuffId = a2.BuffId;
    }
    return *this;
}
