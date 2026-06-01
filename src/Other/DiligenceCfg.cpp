#include "Other/DiligenceCfg.h"
#include "Config/CfgData.h"
#include "Map/MapManager.h"

#include <cstring>
#include <algorithm>

DiligenceCfg::DiligenceCfg()
    : nId(0), nValues(0)
{
}

DiligenceCfg::DiligenceCfg(const DiligenceCfg& other)
    : nId(other.nId)
    , nValues(other.nValues)
    , lItems(other.lItems)
{
}

void DiligenceCfg::RandItem(RateItem* retstr)
{
    int32_t nTotalRate = 0;
    for (auto& item : lItems)
    {
        nTotalRate += item.nRate;
    }
    if (nTotalRate <= 0)
        return;

    Answer::Random* random = Answer::Random::GetInstance();
    int32_t nRand = random->generate(1, nTotalRate);
    int32_t nCur = 0;
    for (auto& item : lItems)
    {
        nCur += item.nRate;
        if (nRand <= nCur)
        {
            *retstr = item;
            return;
        }
    }
}

bool DiligenceCfg::IsInItemList(int32_t ItemId, int8_t ItemCalss)
{
    for (auto& item : lItems)
    {
        if (item.nItemId == ItemId && item.nItemClass == ItemCalss)
            return true;
    }
    return false;
}
