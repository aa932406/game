#include "Other/UseCurrGift.h"
#include "Other/Answer.h"

UseCurrGift::UseCurrGift()
{
    nId = 0;
    CurrType = 0;
    CurrValue = 0;
}

UseCurrGift::~UseCurrGift()
{
}

bool UseCurrGift::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector params;
    Answer::StringUtility::split(&params, strEffect, ":", 0);
    nId = id;
    if (params.size() == 2)
    {
        CurrType = atoi(params[0].c_str());
        CurrValue = atoi(params[1].c_str());
        return true;
    }
    return false;
}
