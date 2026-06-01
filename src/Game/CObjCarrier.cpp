#include "Game/CObjCarrier.h"
#include "Game/Player.h"
#include "Game/Map.h"
#include "Game/GameService.h"
#include "Other/CfgData.h"
#include "Other/CfgCarrierTable.h"
#include "Other/CfgTalentTable.h"
#include "Other/CExtCharCarrier.h"
#include "Other/CObjAttrs.h"
#include "Other/EntityType.h"
#include "Other/AttrAddon.h"

#include <cstring>
#include <algorithm>
#include <list>

CObjCarrier::CObjCarrier()
    : Unit(EntityType::ET_CARRIER)
    , m_nId(0)
    , m_pPlayer(nullptr)
    , m_needSync(false)
    , m_needSyncAttr(false)
    , m_needSyncSelf(false)
{
}

CObjCarrier::~CObjCarrier()
{
}

bool CObjCarrier::refresh()
{
    if (!m_pPlayer)
        return true;

    if (!isAlive())
    {
        CExtCharCarrier *CharCarrier = Player::GetCharCarrier(m_pPlayer);
        CExtCharCarrier::LeaveCarrier(CharCarrier);
        return true;
    }

    if (m_needSync)
    {
        Player::SetNeedSync(m_pPlayer);
        m_needSync = false;
    }

    if (m_needSyncSelf)
    {
        Player::SetNeedSyncSelf(m_pPlayer);
        m_needSyncSelf = false;
    }

    if (m_needSyncAttr)
    {
        Player::SetNeedSyncAttr(m_pPlayer);
        m_needSyncAttr = false;
    }

    return false;
}

UnitHandle CObjCarrier::getHandle() const
{
    if (m_pPlayer)
    {
        CharId_t Cid = Player::getCid(m_pPlayer);
        return UnitHandle(Cid, 1);
    }

    return Unit::getHandle();
}

CharId_t CObjCarrier::getOwner() const
{
    if (m_pPlayer)
        return Player::getCid(m_pPlayer);
    return 0;
}

int32_t CObjCarrier::getLevel() const
{
    if (m_pPlayer)
        return Player::getLevel(m_pPlayer);
    return 0;
}

int32_t CObjCarrier::getBattle() const
{
    if (m_pPlayer)
        return Player::getBattle(m_pPlayer);
    return 0;
}

bool CObjCarrier::isFriendSide(Unit *pUnit)
{
    return m_pPlayer && Player::isFriendSide(m_pPlayer, pUnit);
}

bool CObjCarrier::checkSkillTarget(const CfgActiveSkill *pCfg, Unit *pUnit)
{
    return m_pPlayer && Player::checkSkillTarget(m_pPlayer, pCfg, pUnit);
}

void CObjCarrier::reset()
{
    Unit::reset();
    m_nId = 0;
    m_pPlayer = nullptr;
}

void CObjCarrier::Init(Player *pOwner, int32_t nId)
{
    m_nId = nId;
    m_pPlayer = pOwner;

    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgCarrierTable *CarrierTable = CfgData::GetCarrierTable(pCfgData);
    const CfgCarrier *pCfgCarrier = CfgCarrierTable::GetCarrier(CarrierTable, nId);

    if (pCfgCarrier)
    {
        for (auto iter = pCfgCarrier->lSkills.begin(); iter != pCfgCarrier->lSkills.end(); ++iter)
        {
            int32_t skillId = *iter;
            const CfgTalentTable *TalentTable = CfgData::GetTalentTable(pCfgData);
            const CfgTalent *pCfgTalent = CfgTalentTable::GetTalent(TalentTable, skillId, 1);

            if (pCfgTalent)
            {
                m_lSkills.push_back(pCfgTalent->skillid);
            }
        }

        Unit::ResetAttrs(this);
        initBaseAttr();
        Unit::adjustUnitAttr(this);
        // 更新生命值
    }
}

bool CObjCarrier::appendInfo(Answer::NetPacket *packet)
{
    return m_pPlayer && packet;
}

bool CObjCarrier::HaveSkill(int32_t nSkillId) const
{
    for (auto iter = m_lSkills.begin(); iter != m_lSkills.end(); ++iter)
    {
        if (*iter == nSkillId)
            return true;
    }
    return false;
}

void CObjCarrier::initBaseAttr()
{
    if (!m_pPlayer)
        return;

    int32_t level = Player::getLevel(m_pPlayer);
    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgCarrierTable *CarrierTable = CfgData::GetCarrierTable(pCfgData);
    const CfgCarrierAttr *pCfgAttr = CfgCarrierTable::GetCarrierAttr(CarrierTable, m_nId, level);

    if (pCfgAttr)
    {
        for (auto iter = pCfgAttr->vAttr.begin(); iter != pCfgAttr->vAttr.end(); ++iter)
        {
            Unit::AddAttrValue(this, static_cast<CObjAttrs::Index_T>(iter->index), iter->addon);
        }
    }
}
