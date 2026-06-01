#include "Config/CfgFaBao.h"


CfgFaBao::CfgFaBao(const CfgFaBao& a2)
{
    FaBaoLevel = a2.FaBaoLevel;
    NeedCurr = a2.NeedCurr;
    nNeedLevel = a2.nNeedLevel;
    vAttr = a2.vAttr;
    GongGaoId = a2.GongGaoId;
}

CfgFaBao& CfgFaBao::operator=(const CfgFaBao& a2)
{
    if (this != &a2)
    {
        FaBaoLevel = a2.FaBaoLevel;
        NeedCurr = a2.NeedCurr;
        nNeedLevel = a2.nNeedLevel;
        vAttr = a2.vAttr;
        GongGaoId = a2.GongGaoId;
    }
    return *this;
}
