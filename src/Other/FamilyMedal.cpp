#include "Other/FamilyMedal.h"

FamilyMedal::FamilyMedal()
    : nLevel(0)
    , nNeedFamilyLevel(0)
    , nNeedFamilyExp(0)
    , nGongGaoId(0)
    , DefRdc(0)
{
}

FamilyMedal::FamilyMedal(const FamilyMedal& a2)
    : nLevel(a2.nLevel)
    , nNeedFamilyLevel(a2.nNeedFamilyLevel)
    , nNeedFamilyExp(a2.nNeedFamilyExp)
    , lAttrList(a2.lAttrList)
    , nGongGaoId(a2.nGongGaoId)
    , DefRdc(a2.DefRdc)
{
}
