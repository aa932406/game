#include "Other/SuperCurse.h"
#include <cstdlib>

SuperCurse::SuperCurse()
    : Value(0)
{
}

SuperCurse::~SuperCurse()
{
}

bool SuperCurse::parseEffect(int32_t id, const std::string* strEffect)
{
    Value = atoi(strEffect->c_str());
    return Value > 0;
}
