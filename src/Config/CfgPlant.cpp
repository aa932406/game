#include "Config/CfgPlant.h"


CfgPlant::CfgPlant(const CfgPlant& a2)
{
    id = a2.id;
    type = a2.type;
    level = a2.level;
    Events = a2.Events;
    EventMaxRate = a2.EventMaxRate;
    item_cost = a2.item_cost;
    start_hour = a2.start_hour;
    end_hour = a2.end_hour;
    gather_time = a2.gather_time;
    revive_time = a2.revive_time;
    hide_time = a2.hide_time;
    get_points = a2.get_points;
    boss_id = a2.boss_id;
    BuffId = a2.BuffId;
    revive_skin = a2.revive_skin;
    TaskId = a2.TaskId;
    ItemVt = a2.ItemVt;
}

CfgPlant& CfgPlant::operator=(const CfgPlant& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        type = a2.type;
        level = a2.level;
        Events = a2.Events;
        EventMaxRate = a2.EventMaxRate;
        item_cost = a2.item_cost;
        start_hour = a2.start_hour;
        end_hour = a2.end_hour;
        gather_time = a2.gather_time;
        revive_time = a2.revive_time;
        hide_time = a2.hide_time;
        get_points = a2.get_points;
        boss_id = a2.boss_id;
        BuffId = a2.BuffId;
        revive_skin = a2.revive_skin;
        TaskId = a2.TaskId;
        ItemVt = a2.ItemVt;
    }
    return *this;
}
