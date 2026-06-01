#include "Other/UseItemGift.h"
#include "Other/CItemHelper.h"
#include "Config/CfgData.h"
#include "Game/Player.h"
#include "Character/CExtCharBag.h"

UseItemGift::UseItemGift()
{
    nId = 0;
    Item.m_nId = 0;
    Item.m_nClass = 0;
    Item.m_nCount = 0;
}

UseItemGift::~UseItemGift()
{
}

int32_t UseItemGift::effect(Player* launcher, Unit* target, int32_t* count)
{
    if (!launcher || !target || !count)
        return 2;
    if (*count <= 0)
        *count = 1;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    CfgItemGiftVector* pGifts = CfgData::getItemGift(cfg, nId);
    if (!pGifts || pGifts->empty())
        return 10002;

    CExtCharBag* Bag = Player::GetBag(launcher);
    if (!Bag)
        return 2;

    // 构建物品列表
    MemChrBagVector items;

    for (size_t i = 0; i < pGifts->size(); ++i)
    {
        const CfgItemGift& giftItem = (*pGifts)[i];
        Job_t Job = Player::getJob(launcher);
        if (Job != giftItem.job && giftItem.job > 0)
            continue;

        MemChrBag chrbag;
        memset(&chrbag, 0, sizeof(chrbag));
        chrbag.itemId = giftItem.item;
        chrbag.itemClass = giftItem.type;
        chrbag.itemCount = giftItem.count * (*count);
        chrbag.bind = giftItem.bind;

        items.push_back(chrbag);
    }

    if (items.empty())
        return 10002;

    // 移除原礼包物品
    ItemData costItem;
    costItem.m_nId = nId;
    costItem.m_nClass = 4; // 礼包类
    costItem.m_nCount = *count;
    if (!CExtCharBag::RemoveItem(Bag, &costItem, ITEM_CHANGE_REASON::ICR_BAG_USE))
        return 10002;

    // 添加奖励到背包
    if (!CExtCharBag::AddItemsAndMingGe(Bag, &items, ITEM_CHANGE_REASON::ICR_BAG_USE))
        return 10016;

    return 0;
}

bool UseItemGift::parseEffect(int32_t id, const std::string* strEffect)
{
    nId = id;
    ItemData v5 = CItemHelper::parseItemDataString(strEffect);
    Item.m_nId = v5.m_nId;
    Item.m_nClass = v5.m_nClass;
    Item.m_nCount = v5.m_nCount;
    return Item.m_nId > 0 && Item.m_nCount > 0 && Item.m_nClass > 0;
}

