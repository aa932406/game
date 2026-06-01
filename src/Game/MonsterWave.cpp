#include "Game/MonsterWave.h"

#include <cstring>
#include <algorithm>

bool MonsterWave::alive()
{
    return this->count > this->killed + this->escaped;
}
