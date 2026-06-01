#include "Other/JiuShuCard.h"
#include "Game/Player.h"
#include <cstdlib>

JiuShuCard::JiuShuCard() : m_addon(0) {}
JiuShuCard::~JiuShuCard() {}
bool JiuShuCard::parseEffect(int32_t id, const std::string* strEffect)
{
    m_addon = atoi(strEffect->c_str());
    return m_addon > 0;
}
int32_t JiuShuCard::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (Player::SubPkValues(launcher, m_addon))
        return 0;
    return 10002;
}
