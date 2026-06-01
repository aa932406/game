#include "Config/CfgActivity.h"


CfgActivity::CfgActivity(const CfgActivity& a2)
{
    id = a2.id;
    typeId = a2.typeId;
    weekday = a2.weekday;
    begin_date = a2.begin_date;
    end_date = a2.end_date;
    line = a2.line;
    start_hour = a2.start_hour;
    duration = a2.duration;
    level = a2.level;
    world_event_chapter = a2.world_event_chapter;
    maps = a2.maps;
    target_mapid = a2.target_mapid;
    target_regiona = a2.target_regiona;
    target_regionb = a2.target_regionb;
    state = a2.state;
    gift_id = a2.gift_id;
    daily_reward = a2.daily_reward;
    exp_add_interval = a2.exp_add_interval;
    iconid = a2.iconid;
    StartGongGao = a2.StartGongGao;
}

CfgActivity& CfgActivity::operator=(const CfgActivity& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        typeId = a2.typeId;
        weekday = a2.weekday;
        begin_date = a2.begin_date;
        end_date = a2.end_date;
        line = a2.line;
        start_hour = a2.start_hour;
        duration = a2.duration;
        level = a2.level;
        world_event_chapter = a2.world_event_chapter;
        maps = a2.maps;
        target_mapid = a2.target_mapid;
        target_regiona = a2.target_regiona;
        target_regionb = a2.target_regionb;
        state = a2.state;
        gift_id = a2.gift_id;
        daily_reward = a2.daily_reward;
        exp_add_interval = a2.exp_add_interval;
        iconid = a2.iconid;
        StartGongGao = a2.StartGongGao;
    }
    return *this;
}
