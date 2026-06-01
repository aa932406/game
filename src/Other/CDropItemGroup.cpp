#include "Other/CDropItemGroup.h"

#include <cstring>
#include <algorithm>

int8_t CDropItemGroup::calDropType(int8_t droperType, int8_t bossSign)
{
    if (droperType == 1)
        return 2;
    else
        return bossSign != 0;
}

int32_t CDropItemGroup::calPositionSize(int32_t nSize)
{
    // DROP_ITEM_COUNT and DROP_SIZE arrays need to be defined somewhere
    static const int32_t DROP_ITEM_COUNT[] = { 0, 1, 2, 3, 4, 5 };
    static const int32_t DROP_SIZE[] = { 1, 2, 3, 4, 5, 6 };
    
    for (int32_t i = 5; i >= 0; --i)
    {
        if (DROP_ITEM_COUNT[i] <= nSize)
            return DROP_SIZE[i];
    }
    return DROP_SIZE[0];
}

