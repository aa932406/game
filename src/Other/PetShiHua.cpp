#include "Other/PetShiHua.h"
#include <cstdlib>

PetShiHua::PetShiHua() : Value(0) {}
PetShiHua::~PetShiHua() {}
bool PetShiHua::parseEffect(int32_t id, const std::string* strEffect)
{
    Value = atoi(strEffect->c_str());
    return Value > 0;
}
