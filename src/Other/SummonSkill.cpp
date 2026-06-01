#include "Other/SummonSkill.h"

#include "Utility/Position.h"

#include <cstring>
#include <algorithm>

SummonSkill::SummonSkill()
{
    Position::Position(&this->targetPos, 0, 0);
}

