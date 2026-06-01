#include "Config/CfgMap.h"


CfgMap::CfgMap(const CfgMap& a2)
{
    id = a2.id;
    name = a2.name;
    type = a2.type;
    param = a2.param;
    width = a2.width;
    height = a2.height;
    revive = a2.revive;
    reviveTime = a2.reviveTime;
    pk_mode = a2.pk_mode;
    anti_protect = a2.anti_protect;
    isMount = a2.isMount;
    isFly = a2.isFly;
    isVicegeneral = a2.isVicegeneral;
    player_level = a2.player_level;
    player_level_max = a2.player_level_max;
    team_member = a2.team_member;
    hide_mini = a2.hide_mini;
    runnerId = a2.runnerId;
    jump = a2.jump;
    GongGaoId = a2.GongGaoId;
    pvp = a2.pvp;
    cross = a2.cross;
    x = a2.x;
    y = a2.y;
    drop_rate = a2.drop_rate;
    XingMaiLevel = a2.XingMaiLevel;
    SuitId = a2.SuitId;
    VipLevel = a2.VipLevel;
    JueWeiLevel = a2.JueWeiLevel;
    NeedBroadcast = a2.NeedBroadcast;
    MapCanUsePet = a2.MapCanUsePet;
    SignOut = a2.SignOut;
    Rdx = a2.Rdx;
    Refresh = a2.Refresh;
    CanRand = a2.CanRand;
    LimitDay = a2.LimitDay;
    XinMoAct = a2.XinMoAct;
}

CfgMap& CfgMap::operator=(const CfgMap& a2)
{
    if (this != &a2)
    {
        id = a2.id;
        name = a2.name;
        type = a2.type;
        param = a2.param;
        width = a2.width;
        height = a2.height;
        revive = a2.revive;
        reviveTime = a2.reviveTime;
        pk_mode = a2.pk_mode;
        anti_protect = a2.anti_protect;
        isMount = a2.isMount;
        isFly = a2.isFly;
        isVicegeneral = a2.isVicegeneral;
        player_level = a2.player_level;
        player_level_max = a2.player_level_max;
        team_member = a2.team_member;
        hide_mini = a2.hide_mini;
        runnerId = a2.runnerId;
        jump = a2.jump;
        GongGaoId = a2.GongGaoId;
        pvp = a2.pvp;
        cross = a2.cross;
        x = a2.x;
        y = a2.y;
        drop_rate = a2.drop_rate;
        XingMaiLevel = a2.XingMaiLevel;
        SuitId = a2.SuitId;
        VipLevel = a2.VipLevel;
        JueWeiLevel = a2.JueWeiLevel;
        NeedBroadcast = a2.NeedBroadcast;
        MapCanUsePet = a2.MapCanUsePet;
        SignOut = a2.SignOut;
        Rdx = a2.Rdx;
        Refresh = a2.Refresh;
        CanRand = a2.CanRand;
        LimitDay = a2.LimitDay;
        XinMoAct = a2.XinMoAct;
    }
    return *this;
}
