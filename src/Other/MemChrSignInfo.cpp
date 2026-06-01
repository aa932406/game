#include "Other/MemChrSignInfo.h"

#include <cstring>
#include <algorithm>

MemChrSignInfo::MemChrSignInfo()
    : sign_record(0)
    , refresh_time(0)
{
    sign_reward.clear();
}

void MemChrSignInfo::CleanUp()
{
    sign_record = 0;
    refresh_time = 0;
    sign_reward.clear();
}
