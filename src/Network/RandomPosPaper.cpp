#include "Network/RandomPosPaper.h"

#include "Game/Player.h"

RandomPosPaper::RandomPosPaper()
    : ItemEffect()
    , m_itemid(0)
{
}

RandomPosPaper::~RandomPosPaper()
{
}

int32_t RandomPosPaper::effect(Player* launcher, Unit* /*target*/, int32_t* /*count*/)
{
    return launcher->OnRandPos();
}

bool RandomPosPaper::parseEffect(int32_t id, const std::string* /*strEffect*/)
{
    m_itemid = id;
    return true;
}
