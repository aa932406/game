#include "Other/CharTitle.h"
#include "Config/CfgTitle.h"
#include "Config/CfgData.h"

#include <cstring>
#include <algorithm>

CharTitle::CharTitle(const CfgTitle *title)
{
    cfgTitle = title;
    nState = 0;
}

bool CharTitle::CheckJob(int32_t nJob)
{
    return !cfgTitle->nJob || cfgTitle->nJob == nJob;
}

bool CharTitle::CheckSex(int8_t nSex)
{
    return !cfgTitle->nSex || cfgTitle->nSex == nSex;
}

bool CharTitle::CheckPlatform(const std::string *platform)
{
    return cfgTitle->sPlatform == "-1" || cfgTitle->sPlatform == *platform;
}

bool CharTitle::IsInParams(int32_t nParam)
{
    for (size_t i = 0; i < cfgTitle->vParams.size(); ++i)
    {
        if (cfgTitle->vParams[i] == nParam)
        {
            return true;
        }
    }
    return false;
}

int32_t CharTitle::GetCardTitleParams()
{
    if (!cfgTitle->vParams.empty())
    {
        return cfgTitle->vParams[0];
    }
    return 0;
}

bool CharTitle::IsGet()
{
    return nState == 1 || nState == 2;
}

bool CharTitle::Dress()
{
    if (nState != 1)
    {
        return false;
    }
    nState = 2;
    return true;
}

bool CharTitle::UnDress()
{
    if (nState != 2)
    {
        return false;
    }
    nState = 1;
    return true;
}

void CharTitle::Get()
{
    if (!nState)
    {
        nState = 1;
    }
}
