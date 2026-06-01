#include "Game/CLittleHelper.h"
#include "Game/Player.h"
#include "Game/Map.h"
#include "Game/GameService.h"
#include "Game/CDropItem.h"
#include "Other/EntityType.h"
#include "Other/CExtCharBag.h"
#include "Other/Entity.h"
#include "Other/CfgData.h"
#include "Other/LittleHelperCfg.h"
#include "Other/CObjAttrs.h"
#include "Other/StaticObj.h"
#include "Utility/Position.h"

#include <cstring>
#include <algorithm>
#include <cmath>

CLittleHelper::CLittleHelper()
    : Unit(EntityType::ET_LITTLE_HELPER)
    , m_pPlayer(nullptr)
    , m_LittleHelperId(0)
    , m_Clothes(0)
    , m_nStandPlace(Direction::LEFT)
    , m_nState(LITTLE_HELPER_STATE::LHS_REST)
    , m_WorkState(LITTLE_HELPER_WORK_STATE::LHWS_AWAY_FROM_MAP)
    , m_TarDropId(0)
    , m_PickTick(0)
    , m_LastTick(0)
{
}

CLittleHelper::~CLittleHelper()
{
}

void CLittleHelper::reset()
{
    Unit::reset();

    m_pPlayer = nullptr;
    m_LittleHelperId = 0;
    m_Clothes = 0;
    m_nStandPlace = Direction::LEFT;
    m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_AWAY_FROM_MAP;
    m_TarDropId = 0;
    m_PickTick = 0;
    m_LastTick = 0;
    m_nState = LITTLE_HELPER_STATE::LHS_REST;
}

bool CLittleHelper::refresh()
{
    if (m_nState == LITTLE_HELPER_STATE::LHS_WORK)
    {
        if (!m_pPlayer)
            return false;

        if (Unit::isAlive(m_pPlayer))
        {
            CheckSwitchMap();
            updateState();
        }
        return false;
    }
    return false;
}

CharId_t CLittleHelper::getOwner()
{
    if (m_pPlayer)
        return Player::getCid(m_pPlayer);
    return 0;
}

bool CLittleHelper::appendInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return false;

    int8_t Type = Entity::getType(this);
    Answer::NetPacket::writeInt8(packet, Type);

    EntityId_t EntityId = Entity::getId(this);
    Answer::NetPacket::writeInt64(packet, EntityId);

    CharId_t Cid = 0;
    if (m_pPlayer)
        Cid = Player::getCid(m_pPlayer);
    Answer::NetPacket::writeInt64(packet, Cid);

    Answer::NetPacket::writeInt32(packet, m_LittleHelperId);
    Answer::NetPacket::writeInt32(packet, m_Clothes);

    Answer::NetPacket::writeInt16(packet, m_currentTile.x);
    Answer::NetPacket::writeInt16(packet, m_currentTile.y);
    Answer::NetPacket::writeInt16(packet, m_targetTile.x);
    Answer::NetPacket::writeInt16(packet, m_targetTile.y);

    return true;
}

void CLittleHelper::init(Player *player, int32_t LittleHelperId, int32_t Clothes)
{
    m_pPlayer = player;
    m_LittleHelperId = LittleHelperId;
    m_Clothes = Clothes;
    m_nStandPlace = Direction::LEFT;
    m_nState = LITTLE_HELPER_STATE::LHS_REST;
    AddAttr();
}

void CLittleHelper::AddAttr()
{
    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const LittleHelperCfg *pLittleHelper = CfgData::GetLittleHelperCfg(pCfgData, m_LittleHelperId);

    if (pLittleHelper)
    {
        Unit::AddAttrValue(this, CObjAttrs::ATTR_MOV_SPD, pLittleHelper->nMoveSpeed);
    }
}

void CLittleHelper::EnterMap()
{
    if (!m_pPlayer)
        return;

    Map *pMap = StaticObj::getMap(m_pPlayer);
    if (!pMap)
        return;

    Direction dir = m_pPlayer->getDirection();
    int32_t tx = m_pPlayer->getCurrentTile().x;
    int32_t ty = m_pPlayer->getCurrentTile().y;

    Position targetTile = getFollowTile(tx, ty, dir);

    if (!Map::isWalkablePosition(pMap, targetTile.x, targetTile.y))
    {
        targetTile = Map::getAroudFreeTile(pMap, tx, ty);
    }

    Map::addLittleHelper(pMap, this);
    Map::broadcastIntoMap(pMap, this);
    Unit::setTargetTile(this, targetTile.x, targetTile.y);
    Unit::setAtTile(this, pMap, m_currentTile.x, m_currentTile.y);

    m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_STAND;
}

void CLittleHelper::LeaveMap()
{
    if (m_pMap)
    {
        m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_AWAY_FROM_MAP;
        Unit::broadcastLeave(this);
        Map::removeLittleHelper(m_pMap, this);
        m_pMap = nullptr;
    }
}

void CLittleHelper::OnLogout()
{
    if (m_pPlayer)
    {
        if (m_pMap)
            LeaveMap();
    }
}

void CLittleHelper::CheckSwitchMap()
{
    if (!m_pPlayer)
        return;

    Map *pMap = StaticObj::getMap(m_pPlayer);
    if (pMap)
    {
        if (Unit::isAlive(m_pPlayer) && m_pMap != pMap)
        {
            LeaveMap();
            EnterMap();
        }
    }
}

void CLittleHelper::ChangeClothe(int32_t ClotheId)
{
    m_Clothes = ClotheId;
    SynsAreaAround();
}

void CLittleHelper::ChangeLittleHelperId(int32_t LittleHelperId, int32_t ClotheId)
{
    m_LittleHelperId = LittleHelperId;
    m_Clothes = ClotheId;
    AddAttr();
    SynsAreaAround();
}

Position CLittleHelper::getFollowTile(const int32_t tx, int32_t ty, Direction dir)
{
    Position result(tx, ty);

    switch (dir)
    {
        case Direction::DOWN:
            result.y = ty + 1;
            break;
        case Direction::RIGHT:
            result.x = tx - 1;
            break;
        case Direction::UP:
            result.y = ty - 1;
            break;
        case Direction::LEFT:
            result.x = tx + 1;
            break;
        case Direction::DIR_NE:
            result.x = tx + 1;
            result.y = ty - 1;
            break;
        case Direction::DIR_NW:
            result.x = tx - 1;
            result.y = ty - 1;
            break;
        case Direction::DIR_SE:
            result.x = tx + 1;
            result.y = ty + 1;
            break;
        case Direction::DIR_SW:
            result.x = tx - 1;
            result.y = ty + 1;
            break;
        default:
            result.x = tx + 1;
            break;
    }

    return result;
}

void CLittleHelper::SynsAreaAround()
{
    if (!m_pPlayer || !m_pMap)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, ConnId, Answer::PackType::PACK_DISPATCH, 0x4B);
    if (!packet)
        return;

    // 序列化小精灵信息到广播包
    appendInfo(packet);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    Map::broadcastAreaAround(m_pMap, packet, this);
}

void CLittleHelper::updateState()
{
    int64_t CurrTick = Unit::getTick(this);
    if (CurrTick - m_LastTick <= 199)
        return;

    m_LastTick = CurrTick;

    switch (m_WorkState)
    {
        case LITTLE_HELPER_WORK_STATE::LHWS_STAND:
            onStand();
            break;
        case LITTLE_HELPER_WORK_STATE::LHWS_FOLLOW:
            onFollow();
            break;
        case LITTLE_HELPER_WORK_STATE::LHWS_WALK_PICK:
            onWalkPick();
            break;
        case LITTLE_HELPER_WORK_STATE::LHWS_PICK:
            onPick();
            break;
        default:
            break;
    }

    checkFarAway();
}

void CLittleHelper::checkFarAway()
{
    if (!m_pPlayer || !m_pMap)
        return;

    int32_t dist = std::abs(m_currentTile.x - m_pPlayer->getCurrentTile().x)
                 + std::abs(m_currentTile.y - m_pPlayer->getCurrentTile().y);

    if (dist > 15)
    {
        Direction dir = m_pPlayer->getDirection();
        int32_t tx = m_pPlayer->getCurrentTile().x;
        int32_t ty = m_pPlayer->getCurrentTile().y;

        Position targetTile = getFollowTile(tx, ty, dir);

        if (!Map::isWalkablePosition(m_pMap, targetTile.x, targetTile.y))
        {
            targetTile = Map::getAroudFreeTile(m_pMap, tx, ty);
        }

        Unit::instantMove(this, targetTile.x, targetTile.y, InstanceMoveReason::IMR_TELEPORT, -1);
        m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_STAND;
    }
}

void CLittleHelper::ResetTargetTile(int32_t tx, int32_t ty, Direction dir)
{
    if (!m_pMap)
        return;

    if (m_WorkState != LITTLE_HELPER_WORK_STATE::LHWS_STAND
        && m_WorkState != LITTLE_HELPER_WORK_STATE::LHWS_FOLLOW)
        return;

    Position targetTile = getFollowTile(tx, ty, dir);

    if (Map::isWalkablePosition(m_pMap, targetTile.x, targetTile.y))
    {
        Unit::resetDirection(this, targetTile.x, targetTile.y);
        Unit::setTargetTile(this, targetTile.x, targetTile.y);
        m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_FOLLOW;
    }
}

void CLittleHelper::onStand()
{
    if (m_pPlayer && m_pMap)
    {
        ChangeTarget();
    }
}

void CLittleHelper::ChangeTarget()
{
    if (!m_pMap)
        return;

    CDropItem *pItem = Map::GetNearestDropItem(m_pMap, this);
    if (pItem)
    {
        Position itemPos = StaticObj::getCurrentTile(pItem);
        Unit::resetDirection(this, itemPos.x, itemPos.y);
        Unit::setTargetTile(this, itemPos.x, itemPos.y);
        m_TarDropId = Entity::getId(pItem);
        m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_WALK_PICK;
    }
    else
    {
        m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_STAND;
    }
}

void CLittleHelper::onFollow()
{
    if (!m_pPlayer || !m_pMap)
        return;

    Position CurrentTile = m_currentTile;
    if (CurrentTile.x == m_targetTile.x && CurrentTile.y == m_targetTile.y)
    {
        m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_STAND;
    }
}

void CLittleHelper::onWalkPick()
{
    if (!m_pPlayer || !m_pMap)
        return;

    if (m_TarDropId == 0)
    {
        ChangeTarget();
        return;
    }

    CDropItem *pItem = Map::getDropItem(m_pMap, m_TarDropId);
    if (!pItem || CDropItem::picked(pItem))
    {
        ChangeTarget();
        return;
    }

    Position CurrentTile = m_currentTile;
    Position itemPos = StaticObj::getCurrentTile(pItem);

    if (CurrentTile.x == itemPos.x && CurrentTile.y == itemPos.y)
    {
        m_WorkState = LITTLE_HELPER_WORK_STATE::LHWS_PICK;
    }
}

void CLittleHelper::onPick()
{
    if (!m_pPlayer || !m_pMap)
        return;

    int64_t curTick = Unit::getTick(m_pPlayer);
    if (curTick - m_PickTick < 1000)
        return;

    CDropItem *pItem = Map::getDropItem(m_pMap, m_TarDropId);
    if (pItem && !CDropItem::picked(pItem) && CDropItem::pick(pItem, m_pPlayer))
    {
        m_TarDropId = 0;
        m_PickTick = curTick;
    }

    ChangeTarget();
}
