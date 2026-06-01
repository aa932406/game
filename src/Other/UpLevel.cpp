#include "Other/UpLevel.h"
#include "Common/Common.h"
#include "Game/Player.h"

UpLevel::UpLevel() : m_AddLevel(0), m_MaxLevel(0), m_MinLevel(0) {}
UpLevel::~UpLevel() {}
bool UpLevel::parseEffect(int32_t id, const std::string* strEffect)
{
    StringVector params;
    Common::splistString(strEffect->c_str(), &params, ":");
    if (params.size() != 3)
        return false;
    m_MinLevel = atoi(params[0].c_str());
    m_MaxLevel = atoi(params[1].c_str());
    m_AddLevel = atoi(params[2].c_str());
    return true;
}
int32_t UpLevel::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (*count != 1)
        return 2;
    int32_t maxLevel = Player::GetMaxLevel(launcher);
    if (launcher->getLevel() >= maxLevel)
        return 2;
    if (launcher->getLevel() < m_MinLevel || launcher->getLevel() > m_MaxLevel)
        return 2;
    int32_t newLevel = launcher->getLevel() + m_AddLevel;
    if (newLevel > maxLevel)
        newLevel = maxLevel;
    launcher->setLevel(newLevel);
    CHuoYueDu* pHuoYueDu = Player::GetPlayerHuoYueDu(launcher);
    CHuoYueDu::AddHuoYueDuRecord(pHuoYueDu, 6, 0, 0);
    return 0;
}
