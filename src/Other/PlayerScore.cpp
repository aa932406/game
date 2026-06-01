#include "Other/PlayerScore.h"

#include <cstring>
#include <algorithm>

PlayerScore::PlayerScore()
    : nCharId(0), nDamage(0), nIndex(0), nTeam(0), nTeamLeaderId(0)
{
}

PlayerScore::~PlayerScore()
{
}

PlayerScore::PlayerScore(const PlayerScore& other)
    : nCharId(other.nCharId)
    , strName(other.strName)
    , nDamage(other.nDamage)
    , nIndex(other.nIndex)
    , nTeam(other.nTeam)
    , teamName(other.teamName)
    , nTeamLeaderId(other.nTeamLeaderId)
{
}
