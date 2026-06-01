#include "Other/DamageSum.h"

DamageSum::DamageSum()
    : cid(0)
    , damage(0)
{
}

DamageSum::DamageSum(const DamageSum& a2)
    : cid(a2.cid)
    , name(a2.name)
    , damage(a2.damage)
{
}
