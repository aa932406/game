#include "Other/RandHelper.h"

#include "Map/MapManager.h"

#include <cstring>
#include <algorithm>

int32_t RandHelper::CalRandTimes(int32_t nTotalTimes, int32_t nRate, int32_t nTotalRate)
{
    int32_t times = 0;
    for (int32_t i = 0; i < nTotalTimes; ++i)
    {
        Answer::Random* random = Answer::Random::GetInstance();
        if (random->generate(1, nTotalRate) <= nRate)
            ++times;
    }
    return times;
}
