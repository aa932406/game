#include "Other/CDaZheQuan.h"
#include "Other/ItemEffect.h"

#include <cstring>
#include <algorithm>
#include <cstdlib>

CDaZheQuan::CDaZheQuan()
    : ItemEffect()
    , Index(0)
{
}

CDaZheQuan::~CDaZheQuan()
{
}

bool CDaZheQuan::parseEffect(int32_t id, const std::string& strEffect)
{
    Index = atoi(strEffect.c_str());
    return Index > 0;
}
