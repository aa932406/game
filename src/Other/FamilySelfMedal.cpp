#include "Other/FamilySelfMedal.h"

FamilySelfMedal::FamilySelfMedal()
    : nLevel(0)
    , nNeedExp(0)
    , nAttrRate(0)
{
}

FamilySelfMedal::FamilySelfMedal(const FamilySelfMedal& a2)
    : nLevel(a2.nLevel)
    , nNeedExp(a2.nNeedExp)
    , nAttrRate(a2.nAttrRate)
    , lAttrList(a2.lAttrList)
{
}
