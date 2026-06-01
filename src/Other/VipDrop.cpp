#include "Other/VipDrop.h"

VipDrop::VipDrop()
{
    m_Type = 0;
}

VipDrop::~VipDrop()
{
}

bool VipDrop::parseEffect(int32_t id, const std::string* strEffect)
{
    m_Type = atoi(strEffect->c_str());
    return m_Type > 0;

int32_t VipDrop::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !m_pPlayer || !count)
        return -1;
    int32_t nCount = *count;
    if (nCount <= 0)
        nCount = 1;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgVipDropTable* pTable = CfgData::GetVipDropTable(pCfgData);
    const CfgVipDrop* pCfg = CfgVipDropTable::GetRow(pTable, m_Type);
    if (!pCfg)
        return -1;
    CExtCharBag* pBag = Player::GetBag(launcher);
    Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
    for (auto it = pCfg->vItems.begin(); it != pCfg->vItems.end(); ++it)
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
