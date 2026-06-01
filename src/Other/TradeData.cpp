#include "Other/TradeData.h"

#include <cstring>
#include <algorithm>

void TradeData::OnClanup()
{
    this->BagSlot = -1;
    bzero(&this->CharBagData, 0x20u);
}

