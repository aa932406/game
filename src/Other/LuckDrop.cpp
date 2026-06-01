#include "Other/LuckDrop.h"

LuckDrop::LuckDrop()
    : Type(0)
    , VipLevel(0)
    , Rate(0)
{
}

LuckDrop::LuckDrop(const LuckDrop& a2)
    : Type(a2.Type)
    , VipLevel(a2.VipLevel)
    , ItemList(a2.ItemList)
    , Rate(a2.Rate)
{
}
