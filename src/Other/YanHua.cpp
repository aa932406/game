#include "Other/YanHua.h"
#include "Other/Answer.h"

YanHua::YanHua()
{
    nId = 0;
    YanHuaType = 0;
    YanHuaValue = 0;
}

YanHua::~YanHua()
{
}

bool YanHua::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector params;
    Answer::StringUtility::split(&params, strEffect, ":", 0);
    nId = id;
    if (params.size() == 2)
    {
        YanHuaValue = atoi(params[0].c_str());
        YanHuaType = atoi(params[1].c_str());
        return true;
    }
    return false;
}

YanHua::effect()
{
      GameService *v4;
}
