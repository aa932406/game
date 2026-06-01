#include "Other/DanTianCfg.h"
#include "Other/Param2.h"
#include "Config/CfgData.h"

#include <cstring>
#include <cstdlib>
#include <algorithm>

DanTianCfg::DanTianCfg()
{
}

DanTianCfg::DanTianCfg(const DanTianCfg &a2)
{
    nMid = a2.nMid;
    nItemRate = a2.nItemRate;
    nMaxRate = a2.nMaxRate;
    nTime = a2.nTime;
    nRate = a2.nRate;
    nHighItemRate = a2.nHighItemRate;
    nHighMaxRate = a2.nHighMaxRate;
}

DanTianCfg::~DanTianCfg()
{
}

int32_t DanTianCfg::RandHighDanTianId()
{
    if (nHighItemRate.empty())
    {
        return 0;
    }
    int32_t nRand = rand() % nHighMaxRate;
    int32_t nCumulativeRate = 0;
    for (auto it = nHighItemRate.begin(); it != nHighItemRate.end(); ++it)
    {
        nCumulativeRate += it->nParam2;
        if (nRand < nCumulativeRate)
        {
            return it->nParam1;
        }
    }
    return nHighItemRate.back().nParam1;
}

int32_t DanTianCfg::RandDanTianId()
{
    if (nItemRate.empty())
    {
        return 0;
    }
    int32_t nRand = rand() % nMaxRate;
    int32_t nCumulativeRate = 0;
    for (auto it = nItemRate.begin(); it != nItemRate.end(); ++it)
    {
        nCumulativeRate += it->nParam2;
        if (nRand < nCumulativeRate)
        {
            return it->nParam1;
        }
    }
    return nItemRate.back().nParam1;
}
