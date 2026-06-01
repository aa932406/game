#include "Other/LiBao.h"
#include "Other/CfgData.h"

LiBao::LiBao()
{
    m_month = 0;
    m_record = 0;
}

LiBao::~LiBao()
{
}

bool LiBao::parseEffect(int32_t id, const std::string* strEffect)
{
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const std::vector<CfgItemGift>* pGift = CfgData::getItemGift(v3, id);
    if (!pGift)
        return false;
    m_gifts = *pGift;
    return true;

int32_t LiBao::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count)
        return -1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    for (auto it = m_gifts.begin(); it != m_gifts.end(); ++it)
    {
        MemChrBag item;
        memset(&item, 0, sizeof(item));
        item.itemId = it->item;
        item.itemClass = 4;
        item.itemCount = it->count * nCount;
        item.bind = it->bind;
        CExtCharBag::AddItem(pBag, &item, ITEM_CHANGE_REASON::ICR_BAG_USE);
    }
    return 0;
}
