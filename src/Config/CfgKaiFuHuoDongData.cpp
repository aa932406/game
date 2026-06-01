#include "Config/CfgKaiFuHuoDongData.h"


CfgKaiFuHuoDongData::CfgKaiFuHuoDongData(const CfgKaiFuHuoDongData& a2)
{
    Index = a2.Index;
    Type = a2.Type;
    Level = a2.Level;
    StartDay = a2.StartDay;
    EndDay = a2.EndDay;
    GetRewardDay = a2.GetRewardDay;
    Conditions = a2.Conditions;
    ItemVector = a2.ItemVector;
    ItemString = a2.ItemString;
    PetAttr = a2.PetAttr;
    GongGaoId = a2.GongGaoId;
    Limit = a2.Limit;
    PetEffect = a2.PetEffect;
    Conditions1 = a2.Conditions1;
    FlopString = a2.FlopString;
}

CfgKaiFuHuoDongData& CfgKaiFuHuoDongData::operator=(const CfgKaiFuHuoDongData& a2)
{
    if (this != &a2)
    {
        Index = a2.Index;
        Type = a2.Type;
        Level = a2.Level;
        StartDay = a2.StartDay;
        EndDay = a2.EndDay;
        GetRewardDay = a2.GetRewardDay;
        Conditions = a2.Conditions;
        ItemVector = a2.ItemVector;
        ItemString = a2.ItemString;
        PetAttr = a2.PetAttr;
        GongGaoId = a2.GongGaoId;
        Limit = a2.Limit;
        PetEffect = a2.PetEffect;
        Conditions1 = a2.Conditions1;
        FlopString = a2.FlopString;
    }
    return *this;
}
