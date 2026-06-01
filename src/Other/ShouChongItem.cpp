#include "Other/ShouChongItem.h"
#include <cstdlib>

ShouChongItem::ShouChongItem() : Value(0) {}
ShouChongItem::~ShouChongItem() {}
bool ShouChongItem::parseEffect(int32_t id, const std::string* strEffect)
{
    Value = atoi(strEffect->c_str());
    return Value > 0;
}
