#include "Other/TreasureMapData.h"

#include "Utility/Position.h"

#include <cstring>
#include <algorithm>

TreasureMapData::TreasureMapData()
{
    Position::Position(&this->Pos, 0, 0);
}

