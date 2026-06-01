#include "Other/LingZhu.h"
#include "Game/Player.h"
#include "Other/CKunExt.h"

LingZhu::LingZhu()
{
    m_nId = 0;
    m_Chip = 0;
}

LingZhu::~LingZhu()
{
}

bool LingZhu::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    m_Chip = atoi(strEffect->c_str());
    return m_Chip > 0;
}

int32_t LingZhu::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (*count <= 0)
        return 10002;
    int32_t v5 = *count * m_Chip;
    CKunExt* pKunExt = Player::GetCKunExt(launcher);
    CKunExt::AddExp(pKunExt, v5);
    return 0;
}

