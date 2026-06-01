#include "Other/MonthTouZi.h"

MonthTouZi::MonthTouZi()
    : nDay(0)
    , nGongGaoId(0)
{
}

MonthTouZi::MonthTouZi(const MonthTouZi& a2)
    : nDay(a2.nDay)
    , vItem(a2.vItem)
    , nGongGaoId(a2.nGongGaoId)
{
}
