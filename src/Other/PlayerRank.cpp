#include "Other/PlayerRank.h"

PlayerRank::PlayerRank()
    : nCharId(0)
{
}

PlayerRank::PlayerRank(const PlayerRank& a2)
    : nCharId(a2.nCharId)
    , strName(a2.strName)
{
}
