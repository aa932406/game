#include "Config/CfgNpc.h"


CfgNpc::CfgNpc(const CfgNpc& a2)
{
    id = a2.id;
    level = a2.level;
    mapids = a2.mapids;
    x = a2.x;
    y = a2.y;
    func = a2.func;
    func_extra = a2.func_extra;
    params = a2.params;
    unite_flag = a2.unite_flag;
    platforms = a2.platforms;
    m_vNpcCost = a2.m_vNpcCost;
}

CfgNpc& CfgNpc::operator=(const CfgNpc& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        level = a2.level;
        mapids = a2.mapids;
        x = a2.x;
        y = a2.y;
        func = a2.func;
        func_extra = a2.func_extra;
        params = a2.params;
        unite_flag = a2.unite_flag;
        platforms = a2.platforms;
        m_vNpcCost = a2.m_vNpcCost;
    }
    return *this;
}
