#include "Other/PetAtkSpd.h"
#include <cstdlib>

PetAtkSpd::PetAtkSpd() : Value(0) {}
PetAtkSpd::~PetAtkSpd() {}
bool PetAtkSpd::parseEffect(int32_t id, const std::string* strEffect)
{
    Value = atoi(strEffect->c_str());
    return Value > 0;
}
