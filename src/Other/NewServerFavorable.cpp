#include "Other/NewServerFavorable.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

NewServerFavorable::NewServerFavorable()
    : Index(0), NeedGold(0), nLimitTime(0)
{
}

NewServerFavorable::~NewServerFavorable()
{
}

NewServerFavorable::NewServerFavorable(const NewServerFavorable& other)
    : Index(other.Index)
    , NeedGold(other.NeedGold)
    , ItemVector(other.ItemVector)
    , GongGaoInfo(other.GongGaoInfo)
    , nLimitTime(other.nLimitTime)
{
}
