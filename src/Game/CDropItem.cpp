#include "Game/CDropItem.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/Dungeon.h"
#include "Game/Map.h"
#include "Other/CEquipManager.h"
#include "Other/CItemHelper.h"
#include "Other/CXinMo.h"
#include "Character/CExtCharBag.h"
#include "Answer/NetPacket.h"
#include "Answer/Singleton.h"
#include "Character/CExtCurrency.h"

// CExtCurrency.h defines CURRENCY_* macros that conflict with CURRENCY_TYPE enum
// from CommonTypes.h (included via CDropItem.h -> StaticObj.h -> CommonTypes.h).
// Undefine them so CURRENCY_TYPE::CURRENCY_CASH etc. can be used safely in the code below.
#undef CURRENCY_MONEY
#undef CURRENCY_BIND_MONEY
#undef CURRENCY_GOLD
#undef CURRENCY_BIND_GOLD
#undef CURRENCY_HONOR
#undef CURRENCY_VIGOUR
#undef CURRENCY_CONTRIBUTION
#undef CURRENCY_AC_SOCRE
#undef CURRENCY_BOSS_SCORE
#undef CURRENCY_CASH
#undef CURRENCY_MAX

// CURRENCY_TYPE is defined in Common/CommonTypes.h (included via CDropItem.h)
// CURRENCY_CHANGE_REASON is defined in Common/CommonTypes.h (with GCR/MCR_GET_DROP_ITEM)

#include <cstring>
#include <algorithm>

CDropItem::CDropItem()
    : StaticObj(EntityType::ET_DROPITEM)
    , m_autoPick(false)
    , m_bPicked(false)
    , m_connid(0)
    , m_droperType(0)
    , m_endTick(0)
    , m_freeTick(0)
    , m_mid(0)
    , m_owner(0)
    , m_showTick(0)
    , m_teamId(0)
{
}

CDropItem::~CDropItem()
{
}

void CDropItem::reset()
{
    StaticObj::reset();
    memset(&m_item, 0, sizeof(m_item));
    m_connid = 0;
    m_owner = 0;
    m_teamId = 0;
    m_showTick = 0;
    m_freeTick = 0;
    m_endTick = 0;
}

bool CDropItem::appendInfo(Answer::NetPacket *packet)
{
    if (!packet) return false;

    int8_t type = static_cast<int8_t>(m_entityType);
    packet->writeInt8(type);
    packet->writeInt64(m_mid);
    packet->writeInt32(m_item.nId);
    packet->writeInt8(m_item.nClass);
    packet->writeInt32(m_item.nCount);
    packet->writeInt64((static_cast<uint64_t>(static_cast<uint32_t>(m_currentPos.x)) << 32) | static_cast<uint64_t>(static_cast<uint32_t>(m_currentPos.y)));
    packet->writeInt32(m_teamId);
    packet->writeInt8(m_item.nCostType);
    return true;
}

bool CDropItem::checkOwner(CharId_t nCharId, int32_t nTeamId)
{
    if (!m_pMap)
        return false;
    if (m_pMap->getTick() >= m_freeTick)
        return true;
    if (!m_owner)
        return true;
    if (m_owner == nCharId)
        return true;
    if (m_teamId && m_teamId == nTeamId)
        return true;
    return false;
}

int32_t CDropItem::pick(Player *player)
{
    if (!m_pMap || !player || player->getMap() != m_pMap)
        return 10002;
    if (m_bPicked)
        return 10002;

    int32_t teamId = Player::GetTeamId(player);
    CharId_t cid = Player::getCid(player);
    if (!checkOwner(cid, teamId))
        return 10002;

    // Check cost affordability
    switch (m_item.nCostType)
    {
        case 2: // Cash (yuanbao)
            if (Player::GetCurrency(player, CURRENCY_TYPE::CURRENCY_CASH) < m_item.costValue)
                return 10002;
            break;
        case 3: // Gold
            if (Player::GetCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD) < m_item.costValue)
                return 10002;
            break;
        case 1: // Money (copper coins)
        {
            CExtCurrency *currency = Player::GetCurrency(player);
            if (!currency || CExtCurrency::GetMoneyBindAndNoBind(currency) < m_item.costValue)
                return 10002;
            break;
        }
    }

    // Build item to add to bag
    MemChrBag addItem;
    memset(&addItem, 0, sizeof(addItem));
    addItem.itemId = m_item.nId;
    addItem.itemClass = m_item.nClass;
    addItem.itemCount = m_item.nCount;
    addItem.bind = m_item.bindType;
    addItem.endTime = m_item.endTime;
    addItem.srcId = m_item.srcId;

    if (m_item.nCount > 0)
    {
        if (m_item.nClass == 2)
        {
            // Handle equipment: check bag space first
            if (m_pMap->IsXinMoMap() && CItemHelper::CanEnterXinMoBag(addItem.itemId, addItem.itemClass))
            {
                CXinMo *pXinMo = Player::GetCXinMo(player);
                if (!pXinMo || pXinMo->GetFreeSlotCount() <= 0)
                    return 10016;
            }
            else
            {
                CExtCharBag *bag = Player::GetBag(player);
                if (!bag || bag->GetFreeSlotCount() <= 0)
                    return 10016;
            }

            // Equipment source processing
            CEquipManager *pEquipMgr = Answer::Singleton<CEquipManager>::instance();
            if (m_item.srcId > 0)
            {
                // Try to fetch existing equipment by source ID
                MemEquip pEquip;
                CEquipManager::GetMemEquip(&pEquip, pEquipMgr, m_item.srcId);
                if (pEquip.id == m_item.srcId)
                {
                    if (pEquip.owner != cid)
                    {
                        pEquip.owner = cid;
                        if (m_pMap->IsCrossMap())
                        {
                            // Cross-server: create new equip record
                            MemEquip equip;
                            int8_t connId = Player::getConnId(player);
                            pEquipMgr->CreateMemEquip(&equip, connId, 1,
                                m_item.nId, Player::getSid(player), cid,
                                &pEquip.name, pEquip.MapId, pEquip.Mid, pEquip.time, 0, 0);
                            if (equip.id > 0)
                            {
                                equip.star = pEquip.star;
                                addItem.srcId = equip.id;
                                CEquipManager::UpdateMemEquip(pEquipMgr, connId, &equip, 1);
                                CEquipManager::SendPlayerEquipInfo(pEquipMgr, player, &equip);
                            }
                        }
                        else
                        {
                            int8_t connId = Player::getConnId(player);
                            CEquipManager::UpdateMemEquip(pEquipMgr, connId, &pEquip, 1);
                            CEquipManager::SendPlayerEquipInfo(pEquipMgr, player, &pEquip);
                        }
                    }
                }
            }
            else
            {
                // Create new equipment
                MemEquip pEquip;
                int8_t connId = Player::getConnId(player);
                pEquipMgr->CreateMemEquip(&pEquip, connId, 1,
                    m_item.nId, Player::getSid(player), cid,
                    &m_name, getMapId(), m_mid, 0, 0, 0);
                if (pEquip.id > 0)
                {
                    CEquipManager::SendPlayerEquipInfo(pEquipMgr, player, &pEquip);
                    addItem.srcId = pEquip.id;
                }
            }
        }

        // Add item to bag or xinmo bag
        bool bAdded = false;
        if (m_pMap->IsXinMoMap() && CItemHelper::CanEnterXinMoBag(addItem.itemId, addItem.itemClass))
        {
            std::vector<MemChrBag> items;
            items.push_back(addItem);
            CXinMo *pXinMo = Player::GetCXinMo(player);
            bAdded = pXinMo && pXinMo->AddItem(&items);
            if (!bAdded)
                return 10016;
        }
        else
        {
            CExtCharBag *bag = Player::GetBag(player);
            if (!bag || !bag->AddItem(&addItem, ITEM_CHANGE_REASON::ICR_PICK))
                return 10016;
        }

        // Equipment pick announcement for high-quality gear
        if (addItem.itemClass == 2)
        {
            int32_t itemType = CItemHelper::GetItemType(addItem.itemId, 2);
            if (itemType == 8)
            {
                GameService *pGS = Answer::Singleton<GameService>::instance();
                int8_t connId = Player::getConnId(player);
                Answer::NetPacket *packet = pGS->popNetpacket(connId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                if (packet)
                {
                    packet->writeInt32(413);
                    std::string val;
                    Player::getName(player, &val);
                    packet->writeUTF8(val);
                    packet->writeInt64(cid);
                    uint32_t wOffset = packet->getWOffset();
                    packet->setSize(wOffset);
                    pGS->worldBroadcast(connId, packet);
                }
            }
        }

        // Notify dungeon if this is a dungeon map
        Dungeon *pDungeon = dynamic_cast<Dungeon*>(m_pMap);
        if (pDungeon)
        {
            pDungeon->OnPickItem(player, &addItem);
        }
    }

    // Deduct cost
    switch (m_item.nCostType)
    {
        case 2:
            Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_CASH, m_item.costValue,
                CURRENCY_CHANGE_REASON::GCR_GET_DROP_ITEM, getMapId());
            break;
        case 3:
            Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, m_item.costValue,
                CURRENCY_CHANGE_REASON::GCR_GET_DROP_ITEM, getMapId());
            break;
        case 1:
        {
            CExtCurrency *currency = Player::GetCurrency(player);
            if (currency)
                currency->DecMoneyAndNoBind(m_item.costValue,
                    CURRENCY_CHANGE_REASON::MCR_GET_DROP_ITEM, getMapId());
            break;
        }
    }

    m_bPicked = true;
    return 0;
}

bool CDropItem::checkEnd()
{
    return !m_pMap || m_pMap->getTick() > m_endTick || m_item.nId == 0;
}
