#include "Game/CDropItem.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/Dungeon.h"
#include "Game/Map.h"
#include "Other/CEquipManager.h"
#include "Other/CExtCurrency.h"
#include "Other/CExtCharBag.h"
#include "Other/CItemHelper.h"
#include "Other/CXinMo.h"
#include "Common/CurrencyType.h"
#include "Answer/Singleton.h"

#include <cstring>
#include <algorithm>

CDropItem::CDropItem()
{
    StaticObj::StaticObj(this, EntityType::ET_DROPITEM);
    std::string::string(&this->m_name);
}

CDropItem::~CDropItem()
{
    std::string::~string(&this->m_name);
    StaticObj::~StaticObj(this);
}

void CDropItem::reset()
{
    StaticObj::reset(this);
    bzero(&this->m_item, 0x28u);
    this->m_connid = 0;
    this->m_owner = 0;
    this->m_teamId = 0;
    this->m_showTick = 0;
    this->m_freeTick = 0;
    this->m_endTick = 0;
}

bool CDropItem::appendInfo(Answer::NetPacket *packet)
{
    int8_t Type = this->m_type;
    Answer::NetPacket::writeInt8(packet, Type);
    Answer::NetPacket::writeInt64(packet, this->m_id);
    Answer::NetPacket::writeInt32(packet, this->m_item.nId);
    Answer::NetPacket::writeInt8(packet, this->m_item.nClass);
    Answer::NetPacket::writeInt32(packet, this->m_item.nCount);
    Answer::NetPacket::writeInt64(packet, this->m_pos.toUint64());
    Answer::NetPacket::writeInt32(packet, this->m_teamId);
    Answer::NetPacket::writeInt8(packet, this->m_item.nCostType);
    return true;
}

bool CDropItem::checkOwner(CharId_t nCharId, int32_t nTeamId)
{
    if ( !this->m_pMap )
        return 0;
    if ( Map::getTick(this->m_pMap) >= this->m_freeTick )
        return 1;
    if ( !this->m_owner )
        return 1;
    if ( this->m_owner == nCharId )
        return 1;
    if ( this->m_teamId && this->m_teamId == nTeamId )
        return 1;
    return 0;
}

int32_t CDropItem::pick(Player *const player)
{
    if ( !this->m_pMap || StaticObj::getMap(player) != this->m_pMap )
        return 10002;
    if ( this->m_bPicked )
        return 10002;

    int32_t TeamId = Player::GetTeamId(player);
    CharId_t Cid = Player::getCid(player);
    if ( !CDropItem::checkOwner(this, Cid, TeamId) )
        return 10002;

    // 检查消耗
    int costType = this->m_item.costType;
    switch ( costType )
    {
        case 2:
            if ( Player::GetCurrency(player, CURRENCY_TYPE::CURRENCY_CASH) < this->m_item.costValue )
                return 10002;
            break;
        case 3:
            if ( Player::GetCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD) < this->m_item.costValue )
                return 10002;
            break;
        case 1:
        {
            CExtCurrency *Currency = Player::GetCurrency(player);
            if ( CExtCurrency::GetMoneyBindAndNoBind(Currency) < this->m_item.costValue )
                return 10002;
            break;
        }
    }

    // 构建添加的物品
    MemChrBag addItem;
    memset(&addItem, 0, sizeof(addItem));
    addItem.itemId = this->m_item.itemId;
    addItem.itemClass = this->m_item.itemClass;
    addItem.itemCount = this->m_item.itemCount;
    addItem.bind = this->m_item.bindType;
    addItem.endTime = this->m_item.endTime;
    addItem.srcId = this->m_item.srcId;

    if ( this->m_item.itemCount > 0 )
    {
        if ( this->m_item.itemClass == 2 )
        {
            // 处理装备
            if ( Map::IsXinMoMap(this->m_pMap) && CItemHelper::CanEnterXinMoBag(addItem.itemId, addItem.itemClass) )
            {
                CXinMo *pXinMo = Player::GetCXinMo(player);
                if ( CXinMo::GetFreeSlotCount(pXinMo) <= 0 )
                    return 10016;
            }
            else
            {
                CExtCharBag *Bag = Player::GetBag(player);
                if ( CExtCharBag::GetFreeSlotCount(Bag) <= 0 )
                    return 10016;
            }

            // 装备来源处理
            if ( this->m_item.srcId > 0 )
            {
                MemEquip pEquip;
                CEquipManager *pEquipMgr = Answer::Singleton<CEquipManager>::instance();
                CEquipManager::GetMemEquip(&pEquip, pEquipMgr, this->m_item.srcId);
                if ( pEquip.id == this->m_item.srcId )
                {
                    if ( pEquip.owner != Player::getCid(player) )
                    {
                        pEquip.owner = Player::getCid(player);
                        if ( Map::IsCrossMap(this->m_pMap) )
                        {
                            MemEquip equip;
                            int8_t ConnId = Player::getConnId(player);
                            CEquipManager::CreateMemEquip(&equip, pEquipMgr, ConnId, 1,
                                this->m_item.itemId, Player::getSid(player), Player::getCid(player),
                                &pEquip.name, pEquip.MapId, pEquip.Mid, pEquip.time, pEquip.Lucky, 0);
                            if ( equip.id > 0 )
                            {
                                equip.star = pEquip.star;
                                addItem.srcId = equip.id;
                                CEquipManager::UpdateMemEquip(pEquipMgr, ConnId, &equip, 1);
                                CEquipManager::SendPlayerEquipInfo(pEquipMgr, player, &equip);
                            }
                            MemEquip::~MemEquip(&equip);
                        }
                        else
                        {
                            int8_t ConnId = Player::getConnId(player);
                            CEquipManager::UpdateMemEquip(pEquipMgr, ConnId, &pEquip, 1);
                            CEquipManager::SendPlayerEquipInfo(pEquipMgr, player, &pEquip);
                        }
                    }
                }
                MemEquip::~MemEquip(&pEquip);
            }
            else
            {
                // 创建新装备
                MemEquip pEquip;
                int8_t ConnId = Player::getConnId(player);
                CEquipManager *pEquipMgr = Answer::Singleton<CEquipManager>::instance();
                CEquipManager::CreateMemEquip(&pEquip, pEquipMgr, ConnId, 1,
                    this->m_item.itemId, Player::getSid(player), Player::getCid(player),
                    &this->m_name, StaticObj::getMapId(player), this->m_mid, 0, 0, 0);
                if ( pEquip.id > 0 )
                {
                    CEquipManager::SendPlayerEquipInfo(pEquipMgr, player, &pEquip);
                    addItem.srcId = pEquip.id;
                }
                MemEquip::~MemEquip(&pEquip);
            }
        }

        // 加入背包/心魔背包
        bool bAdded = false;
        if ( Map::IsXinMoMap(this->m_pMap) && CItemHelper::CanEnterXinMoBag(addItem.itemId, addItem.itemClass) )
        {
            std::vector<MemChrBag> Items;
            std::vector<MemChrBag>::vector(&Items);
            std::vector<MemChrBag>::push_back(&Items, &addItem);
            CXinMo *pXinMo = Player::GetCXinMo(player);
            bAdded = CXinMo::AddItem(pXinMo, &Items);
            std::vector<MemChrBag>::~vector(&Items);
            if ( !bAdded )
                return 10016;
        }
        else
        {
            CExtCharBag *Bag = Player::GetBag(player);
            if ( !CExtCharBag::AddItem(Bag, &addItem, ITEM_CHANGE_REASON::ICR_PICK) )
                return 10016;
        }

        // 装备拾取公告
        if ( addItem.itemClass == 2 )
        {
            int32_t Type = CItemHelper::GetItemType(addItem.itemId, 2);
            if ( Type == 8 )
            {
                GameService *pGS = Answer::Singleton<GameService>::instance();
                int8_t ConnId = Player::getConnId(player);
                Answer::NetPacket *packet = GameService::popNetpacket(pGS, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
                if ( packet )
                {
                    Answer::NetPacket::writeInt32(packet, 413);
                    std::string val;
                    Player::getName((const Player *const)&val);
                    Answer::NetPacket::writeUTF8(packet, &val);
                    std::string::~string(&val);
                    Answer::NetPacket::writeInt64(packet, Player::getCid(player));
                    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                    Answer::NetPacket::setSize(packet, WOffset);
                    GameService::worldBroadcast(pGS, ConnId, packet);
                }
            }
        }

        // 副本拾取回调
        if ( this->m_pMap && (*((unsigned __int8 (__fastcall **)(Map *))this->m_pMap->_vptr_Map + 18))(this->m_pMap) )
        {
            Dungeon *pDungeon = static_cast<Dungeon*>(this->m_pMap);
            if ( pDungeon )
                Dungeon::OnPickItem(pDungeon, player, &addItem);
        }
    }

    // 扣除消耗
    switch ( this->m_item.costType )
    {
        case 2:
            Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_CASH, this->m_item.costValue,
                CURRENCY_CHANGE_REASON::GCR_GET_DROP_ITEM, this->getHandle());
            break;
        case 3:
            Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, this->m_item.costValue,
                CURRENCY_CHANGE_REASON::GCR_GET_DROP_ITEM, this->getHandle());
            break;
        case 1:
        {
            CExtCurrency *Currency = Player::GetCurrency(player);
            CExtCurrency::DecMoneyAndNoBind(Currency, this->m_item.costValue,
                CURRENCY_CHANGE_REASON::MCR_GET_DROP_ITEM, this->getHandle());
            break;
        }
    }

    this->m_bPicked = 1;
    return 0;
}

bool CDropItem::checkEnd()
{
    return !this->m_pMap || Map::getTick(this->m_pMap) > this->m_endTick || this->m_item.nId == 0;
}
