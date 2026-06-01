#include "Config/CfgDungeonMonster.h"


CfgDungeonMonster::CfgDungeonMonster(const CfgDungeonMonster& a2)
{
    id = a2.id;
    wave = a2.wave;
    mids = a2.mids;
    x = a2.x;
    y = a2.y;
    count = a2.count;
    side = a2.side;
    road = a2.road;
    delay = a2.delay;
    times = a2.times;
    money = a2.money;
    life = a2.life;
    randpos = a2.randpos;
    wait = a2.wait;
}

CfgDungeonMonster& CfgDungeonMonster::operator=(const CfgDungeonMonster& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        wave = a2.wave;
        mids = a2.mids;
        x = a2.x;
        y = a2.y;
        count = a2.count;
        side = a2.side;
        road = a2.road;
        delay = a2.delay;
        times = a2.times;
        money = a2.money;
        life = a2.life;
        randpos = a2.randpos;
        wait = a2.wait;
    }
    return *this;
}
