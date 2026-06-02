#include "Other/SystemOpenGift.h"

#include <cstring>
#include <algorithm>

SystemOpenGift::SystemOpenGift(const SystemOpenGift& a2)
{
    this->Id = a2.Id;
    this->TypeId = a2.TypeId;
    this->Gifts = a2.Gifts;
    this->Star = a2.Star;
}

