#include "Other/GoldEgg.h"
#include "Other/CfgData.h"

GoldEgg::GoldEgg()
{
    m_pCfgItem = nullptr;
}

GoldEgg::~GoldEgg()
{
}

bool GoldEgg::parseEffect(int32_t id, const std::string* strEffect)
{
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const CfgGoldEggTable* GoldEggTable = CfgData::GetGoldEggTable(v3);
    m_pCfgItem = CfgGoldEggTable::GetGoldEgg(GoldEggTable, id);
    return m_pCfgItem != nullptr;

int32_t GoldEgg::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count || !m_pCfgItem)
        return -1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
    for (auto it = m_pCfgItem->vItems.begin(); it != m_pCfgItem->vItems.end(); ++it)
    {
        int32_t nRand = pRand->generate(1, 10000);
        if (nRand <= it->nRate)
        {
            MemChrBag item;
            memset(&item, 0, sizeof(item));
            item.itemId = it->itemId;
            item.itemClass = it->itemClass;
            item.itemCount = it->itemCount * nCount;
            item.bind = it->bind;
            CExtCharBag::AddItem(pBag, &item, ITEM_CHANGE_REASON::ICR_BAG_USE);
        }
    }
    return 0;
}
