#include "Other/OreItem.h"

OreItem::OreItem()
{
    m_nId = 0;
    m_Exp = 0;
}

OreItem::~OreItem()
{
}

bool OreItem::parseEffect(int32_t id, const std::string* strEffect)
{
    m_nId = id;
    m_Exp = atoi(strEffect->c_str());
    return m_Exp > 0;

int32_t OreItem::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count)
        return -1;
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    CExtCharBag::AddExpOre(pBag, m_Exp * nCount);
    return 0;
}
