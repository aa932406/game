#include "Config/CfgTask.h"


CfgTask::CfgTask(const CfgTask& a2)
{
    id = a2.id;
    type = a2.type;
    group = a2.group;
    can_giveup = a2.can_giveup;
    pretask = a2.pretask;
    posttask = a2.posttask;
    main_order = a2.main_order;
    job = a2.job;
    level = a2.level;
    max_level = a2.max_level;
    kingdom = a2.kingdom;
    start_npc = a2.start_npc;
    end_npc = a2.end_npc;
    items_receive = a2.items_receive;
    award_exp = a2.award_exp;
    award_money = a2.award_money;
    gold = a2.gold;
    dilong = a2.dilong;
    rongyu = a2.rongyu;
    fuwen = a2.fuwen;
    BossScore = a2.BossScore;
    dungeon = a2.dungeon;
    award_item = a2.award_item;
    award_optional = a2.award_optional;
    condition = a2.condition;
    done_count = a2.done_count;
    request = a2.request;
    JunTuanZiJin = a2.JunTuanZiJin;
    GongXian = a2.GongXian;
    Double = a2.Double;
    acScore = a2.acScore;
    quickDoneCost = a2.quickDoneCost;
    quality = a2.quality;
    ratio = a2.ratio;
    dust = a2.dust;
    activity_score = a2.activity_score;
    talent_point = a2.talent_point;
    drop_list = a2.drop_list;
    DoubleGold = a2.DoubleGold;
    BuffId = a2.BuffId;
}

CfgTask& CfgTask::operator=(const CfgTask& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        type = a2.type;
        group = a2.group;
        can_giveup = a2.can_giveup;
        pretask = a2.pretask;
        posttask = a2.posttask;
        main_order = a2.main_order;
        job = a2.job;
        level = a2.level;
        max_level = a2.max_level;
        kingdom = a2.kingdom;
        start_npc = a2.start_npc;
        end_npc = a2.end_npc;
        items_receive = a2.items_receive;
        award_exp = a2.award_exp;
        award_money = a2.award_money;
        gold = a2.gold;
        dilong = a2.dilong;
        rongyu = a2.rongyu;
        fuwen = a2.fuwen;
        BossScore = a2.BossScore;
        dungeon = a2.dungeon;
        award_item = a2.award_item;
        award_optional = a2.award_optional;
        condition = a2.condition;
        done_count = a2.done_count;
        request = a2.request;
        JunTuanZiJin = a2.JunTuanZiJin;
        GongXian = a2.GongXian;
        Double = a2.Double;
        acScore = a2.acScore;
        quickDoneCost = a2.quickDoneCost;
        quality = a2.quality;
        ratio = a2.ratio;
        dust = a2.dust;
        activity_score = a2.activity_score;
        talent_point = a2.talent_point;
        drop_list = a2.drop_list;
        DoubleGold = a2.DoubleGold;
        BuffId = a2.BuffId;
    }
    return *this;
}
