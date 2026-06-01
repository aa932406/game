#include "Config/CfgActivityNpc.h"


CfgActivityNpc::CfgActivityNpc(const CfgActivityNpc& a2)
{
    id = a2.id;
    activity_id = a2.activity_id;
    npc_id = a2.npc_id;
    count = a2.count;
    region_id = a2.region_id;
}

CfgActivityNpc& CfgActivityNpc::operator=(const CfgActivityNpc& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        activity_id = a2.activity_id;
        npc_id = a2.npc_id;
        count = a2.count;
        region_id = a2.region_id;
    }
    return *this;
}
