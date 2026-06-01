#include "Config/CfgTrap.h"


CfgTrap::CfgTrap(const CfgTrap& a2)
{
    id = a2.id;
    cd = a2.cd;
    delay = a2.delay;
    event_type = a2.event_type;
    effect = a2.effect;
    item_cost = a2.item_cost;
    life = a2.life;
}

CfgTrap& CfgTrap::operator=(const CfgTrap& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        cd = a2.cd;
        delay = a2.delay;
        event_type = a2.event_type;
        effect = a2.effect;
        item_cost = a2.item_cost;
        life = a2.life;
    }
    return *this;
}
