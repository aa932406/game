#include "Config/CfgActivityMonster.h"


CfgActivityMonster::CfgActivityMonster(const CfgActivityMonster& a2)
{
    id = a2.id;
    wave = a2.wave;
    mid = a2.mid;
    x = a2.x;
    y = a2.y;
    count = a2.count;
    side = a2.side;
    road = a2.road;
    delay = a2.delay;
    times = a2.times;
    buff = a2.buff;
    randpos = a2.randpos;
    left = a2.left;
}

CfgActivityMonster& CfgActivityMonster::operator=(const CfgActivityMonster& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        wave = a2.wave;
        mid = a2.mid;
        x = a2.x;
        y = a2.y;
        count = a2.count;
        side = a2.side;
        road = a2.road;
        delay = a2.delay;
        times = a2.times;
        buff = a2.buff;
        randpos = a2.randpos;
        left = a2.left;
    }
    return *this;
}
