#include "Other/EnterDungeon.h"

EnterDungeon::EnterDungeon()
{
    m_DungeonId = 0;
}

EnterDungeon::~EnterDungeon()
{
}

bool EnterDungeon::parseEffect(int32_t id, const std::string* strEffect)
{
    m_DungeonId = atoi(strEffect->c_str());
    return true;
}

EnterDungeon::effect()
{
      CExtCharTeamDungeon *CharTeamDungeon;
}
