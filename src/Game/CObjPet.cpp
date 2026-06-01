#include "Game/CObjPet.h"

#include <cstring>
#include <algorithm>

CObjPet::CObjPet()
    : Unit(EntityType::ET_PET)
    , m_target(0, EntityType::ET_PET)
{
    m_pPlayer = nullptr;
}

CObjPet::~CObjPet()
{
}

bool CObjPet::refresh()
{
    if (m_bAlive && !isAlive())
        onDie();
    if (m_nState == static_cast<int32_t>(PET_STATE::PS_FIGHTING) && isAlive())
    {
        checkSpeed();
        checkSwitchMap();
        updateFightState();
    }
    if (m_pPlayer && CheckRelive())
    {
        CExtCharPet *CharPet = Player::GetCharPet(m_pPlayer);
        CExtCharPet::SendPetInfo(CharPet);
    }
    return false;
}

CharId_t CObjPet::getOwner() const
{
    if (m_pPlayer)
        return m_pPlayer->getCid();
    return 0;
}

int32_t CObjPet::getLevel() const
{
    if (m_pPlayer)
        return m_pPlayer->getLevel();
    return 0;
}

int32_t CObjPet::getBattle() const
{
    if (m_pPlayer)
        return m_pPlayer->getBattle();
    return 0;
}

bool CObjPet::isFriendSide(Unit *pUnit)
{
    return m_pPlayer
        && m_pPlayer->isFriendSide(pUnit);
}

bool CObjPet::checkSkillTarget(const CfgActiveSkill *pCfg, Unit *pUnit)
{
    return m_pPlayer
        && m_pPlayer->checkSkillTarget(pCfg, pUnit);
}

void CObjPet::postDamage(int32_t damge, UnitHandle launcher, int32_t Mid)
{
    if (!m_pPlayer)
        return;
    Map *pMap = m_pMap;
    if (!pMap)
        return;

    if (!m_AiState || (m_AiState == 1 && launcher.type == 4))
    {
        CExtCharPet *CharPet = Player::GetCharPet(m_pPlayer);
        CExtCharPet::AddTarget(CharPet, launcher, 0);
    }

    if (!isAlive())
        return;

    if (launcher.type == 1 || launcher.type == 4)
        Player::SetInPvP(m_pPlayer);

    Player *player = nullptr;
    if (launcher.type == 1)
    {
        int32_t RunnerId = StaticObj::GetRunnerId(this);
        GameService *v6 = Answer::Singleton<GameService>::instance();
        player = GameService::getPlayer(v6, launcher.id, RunnerId, 1);
    }
    else if (launcher.type == 4)
    {
        int32_t v7 = StaticObj::GetRunnerId(this);
        GameService *v8 = Answer::Singleton<GameService>::instance();
        CObjPet *pPet = GameService::getPet(v8, launcher.id, v7);
        if (pPet)
            player = CObjPet::GetPlayer(pPet);
    }

    if (player && !StaticObj::InPrison(player) && !Map::IsPVPMap(pMap))
    {
        CExtFightChecker *FightChecker = Player::GetFightChecker(m_pPlayer);
        if (!CExtFightChecker::IsBuleName(FightChecker)
            && Player::getPkValue(m_pPlayer) <= 99
            && Player::getPkValue(player) <= 99)
        {
            FamilyId_t FamilyId = Player::getFamilyId(player);
            CExtCharFamily *CharFamily = Player::GetCharFamily(m_pPlayer);
            if (!CExtCharFamily::IsDeclareWarFamily(CharFamily, FamilyId) && !StaticObj::InActivity(this))
            {
                CExtFightChecker *v14 = Player::GetFightChecker(player);
                CExtFightChecker::ChangeBuleName(v14, 1);
            }
        }
    }
}

void CObjPet::reset()
{
    Unit::reset();
    m_pPlayer = nullptr;
    m_pCfgPet = nullptr;
    m_nSummonSkillId = 0;
    m_nDieTick = 0;
    m_nDieTime = 0;
    m_nPetId = 0;
    memset(m_strName, 0, sizeof(m_strName));
    memset(m_vSkill, 0, sizeof(m_vSkill));
    memset(m_vEquip, 0, sizeof(m_vEquip));
    m_bAlive = false;
    m_nState = static_cast<int32_t>(PET_STATE::PS_REST);
    m_nFightState = static_cast<int32_t>(PET_FIGHT_STATE::PFS_AWAY_FROM_MAP);
    m_nStandPlace = Direction::LEFT;
    m_nLastSkillTick = 0;
    m_nLastReplyHp = 0;
    m_pNextSkill = nullptr;
    m_target = UnitHandle();
    m_AiState = 1;
}

void CObjPet::Init(Player *pOwner, PetId_t nPetId, int32_t nBaseId, bool bAlive, PET_STATE state, Direction nDir, int8_t AiState)
{
    m_nPetId = nPetId;
    CfgData *v8 = Answer::Singleton<CfgData>::instance();
    const CfgPetTable *PetTable = CfgData::GetPetTable(v8);
    m_pCfgPet = CfgPetTable::GetPet(PetTable, nBaseId);
    m_pPlayer = pOwner;
    m_bAlive = bAlive;
    m_nState = static_cast<int32_t>(state);
    m_nFightState = static_cast<int32_t>(PET_FIGHT_STATE::PFS_AWAY_FROM_MAP);
    m_nStandPlace = nDir;
    m_AiState = AiState;
    clearTarget();
    if (m_pCfgPet)
    {
        memset(m_vSkill, 0, sizeof(m_vSkill));
        for (int32_t i = 0; i <= 8; ++i)
            m_vSkill[i].nId = m_pCfgPet->m_vSkill[i];
    }
    recalAttr();
    SetHP(100);
}

void CObjPet::InitSummon(Player *pOwner, int32_t nBaseId, int32_t nSkillId)
{
    m_nPetId = getEntityId();
    m_nSummonSkillId = nSkillId;
    CfgData *v4 = Answer::Singleton<CfgData>::instance();
    const CfgPetTable *PetTable = CfgData::GetPetTable(v4);
    m_pCfgPet = CfgPetTable::GetPet(PetTable, nBaseId);
    m_pPlayer = pOwner;
    m_bAlive = true;
    m_nState = static_cast<int32_t>(PET_STATE::PS_FIGHTING);
    m_nFightState = static_cast<int32_t>(PET_FIGHT_STATE::PFS_AWAY_FROM_MAP);
    m_AiState = 1;
    clearTarget();
    if (m_pCfgPet)
    {
        memset(m_vSkill, 0, sizeof(m_vSkill));
        for (int32_t i = 0; i <= 8; ++i)
        {
            if (m_pCfgPet->m_vSkill[i] > 0)
            {
                m_vSkill[i].nId = m_pCfgPet->m_vSkill[i];
                m_vSkill[i].nLevel = 1;
            }
        }
    }
    recalAttr();
    SetHP(100);
}

void CObjPet::ReplyHp(int64_t CurTick)
{
    if (!AliveFlag())
        return;

    if (getHpPercent() > 90)
    {
        if (CurTick - m_nLastReplyHp <= 9999)
            return;
    }
    else if (CurTick - m_nLastReplyHp <= 1999)
    {
        return;
    }

    m_nLastReplyHp = CurTick;
    int64_t v2 = getHp();
    if (v2 < getMaxHp())
    {
        int32_t ReplyRate = 10;
        if (getHpPercent() <= 90)
            ReplyRate = 20;
        int32_t NeedAddHp = getMaxHp() / 100 * ReplyRate;
        AddHP(NeedAddHp);
    }
}

bool CObjPet::appendInfo(Answer::NetPacket *packet)
{
    if (!m_pPlayer || !packet)
        return false;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(EntityType::ET_PET));
    Answer::NetPacket::writeInt64(packet, m_nPetId);
    Answer::NetPacket::writeInt32(packet, GetBaseId());

    CharId_t Cid = Player::getCid(m_pPlayer);
    Answer::NetPacket::writeInt64(packet, Cid);

    std::string name;
    Player::getName(m_pPlayer, &name);
    Answer::NetPacket::writeUTF8(packet, &name);

    Answer::NetPacket::writeUTF8(packet, m_strName);

    Position pos = StaticObj::getCurrentTile(this);
    Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(pos.x));
    Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(pos.y));

    Position targetTile = getTargetTile();
    Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(targetTile.x));
    Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(targetTile.y));

    Answer::NetPacket::writeInt32(packet, getMoveSpeed());
    Answer::NetPacket::writeInt32(packet, getHp());
    Answer::NetPacket::writeInt32(packet, getMaxHp());

    packageBuffList(packet);

    CExtCharPet *CharPet = Player::GetCharPet(m_pPlayer);
    Answer::NetPacket::writeInt32(packet, CExtCharPet::GetStar(CharPet));

    int32_t HuanHua = CExtCharPet::GetHuanHua(CharPet);
    Answer::NetPacket::writeInt32(packet, HuanHua);

    if (Player::getRecord(m_pPlayer, 1030) <= 0)
        Answer::NetPacket::writeInt8(packet, 0);
    else
        Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(Player::getRecord(m_pPlayer, 1029)));

    int8_t Record = Player::getRecord(m_pPlayer, 1153) > 0;
    Answer::NetPacket::writeInt8(packet, Record);

    return true;
}

int32_t CObjPet::GetBaseId() const
{
    if (m_pCfgPet)
        return m_pCfgPet->m_nPetId;
    return 0;
}

void CObjPet::CheckSkillOpen(int32_t nStar)
{
    if (!m_pCfgPet)
        return;

    CfgData *v2 = Answer::Singleton<CfgData>::instance();
    const CfgPetTable *PetTable = CfgData::GetPetTable(v2);
    const CfgPetUpStar *pCfgUpStar = CfgPetTable::GetPetUpStar(PetTable, nStar);
    if (!pCfgUpStar)
        return;

    int32_t nSkillSize = static_cast<int32_t>(pCfgUpStar->vSkillLevel.size());
    for (int32_t i = 0; i <= 8; ++i)
    {
        if (i >= nSkillSize)
            m_vSkill[i].nLevel = 0;
        else
            m_vSkill[i].nLevel = pCfgUpStar->vSkillLevel[i];
    }
}

void CObjPet::Die()
{
    m_bAlive = false;
    m_nDieTick = getTick();
    m_nDieTime = getNow();
    ClearTarget();
    clearAddonSkills();
}

bool CObjPet::CheckRelive()
{
    if (m_bAlive || IsSummonPet())
        return false;
    int32_t Now = getNow();
    if (Now < getReliveTime())
        return false;
    m_bAlive = true;
    return true;
}

void CObjPet::SendPetStatus()
{
    if (!m_pPlayer || !m_pMap)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DD7u);
    if (!packet)
        return;

    Answer::NetPacket::writeInt64(packet, m_nPetId);

    CExtCharPet *CharPet = Player::GetCharPet(m_pPlayer);
    Answer::NetPacket::writeInt32(packet, CExtCharPet::GetStar(CharPet));

    CExtCharPet *v5 = Player::GetCharPet(m_pPlayer);
    Answer::NetPacket::writeInt32(packet, CExtCharPet::GetHuanHua(v5));

    Answer::NetPacket::writeUTF8(packet, m_strName);

    if (Player::getRecord(m_pPlayer, 1030) <= 0)
        Answer::NetPacket::writeInt8(packet, 0);
    else
        Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(Player::getRecord(m_pPlayer, 1029)));

    Answer::NetPacket::writeInt8(packet, Player::getRecord(m_pPlayer, 1153) > 0);

    Answer::NetPacket::setSize(packet, Answer::NetPacket::getWOffset(packet));
    Map::broadcastAreaAround(m_pMap, packet, this);
}

bool CObjPet::AppendPetInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return false;

    Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(m_nState));
    int32_t ReliveTime = getReliveTime();
    Answer::NetPacket::writeInt32(packet, ReliveTime);
    Answer::NetPacket::writeUTF8(packet, m_strName);
    appendAttrInfo(packet);
    appendEquipInfo(packet);
    Answer::NetPacket::writeInt8(packet, m_AiState);
    return true;
}

void CObjPet::appendAttrInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    for (int32_t i = 1; i <= 49; ++i)
    {
        int32_t AttrValue = GetAttrValue( static_cast<CObjAttrs::Index_T>(i));
        Answer::NetPacket::writeInt32(packet, AttrValue);
    }
}

void CObjPet::appendEquipInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    uint32_t nOldOffset = Answer::NetPacket::getWOffset(packet);
    int8_t nCount = 0;
    Answer::NetPacket::writeInt8(packet, 0);

    for (int8_t i = 0; i <= 3; ++i)
    {
        if (m_vEquip[i] > 0)
        {
            Answer::NetPacket::writeInt8(packet, i);
            Answer::NetPacket::writeInt32(packet, m_vEquip[i]);
            ++nCount;
        }
    }

    uint32_t nNewOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nOldOffset);
    Answer::NetPacket::writeInt8(packet, nCount);
    Answer::NetPacket::setWOffset(packet, nNewOffset);
}

void CObjPet::appendSkillInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    uint32_t nOldOffset = Answer::NetPacket::getWOffset(packet);
    int8_t nCount = 0;
    Answer::NetPacket::writeInt8(packet, 0);

    for (int8_t i = 0; i <= 8; ++i)
    {
        if (m_vSkill[i].nId > 0)
        {
            Answer::NetPacket::writeInt8(packet, i);
            Answer::NetPacket::writeInt32(packet, m_vSkill[i].nLevel);
            ++nCount;
        }
    }

    uint32_t nNewOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nOldOffset);
    Answer::NetPacket::writeInt8(packet, nCount);
    Answer::NetPacket::setWOffset(packet, nNewOffset);
}

PetSkillInfo* CObjPet::GetSkill(int8_t nSkillPos)
{
    if (static_cast<uint8_t>(nSkillPos) <= 8u)
        return &m_vSkill[nSkillPos];
    return nullptr;
}

void CObjPet::LearnSkill(int8_t nSkillPos)
{
    if (static_cast<uint8_t>(nSkillPos) <= 8u)
    {
        ++m_vSkill[nSkillPos].nLevel;
        recalAttr();
    }
}

void CObjPet::ChangeName(const std::string *strName)
{
    const char *v2 = strName->c_str();
    snprintf(m_strName, sizeof(m_strName), "%s", v2);
    if (IsFighting())
        SendPetStatus();
}

void CObjPet::ChangeAi(int8_t State)
{
    if (m_AiState != State)
    {
        m_AiState = State;
        if (m_AiState == 1)
            ClearTarget();
    }
}

std::string CObjPet::GetName() const
{
    return std::string(m_strName);
}

void CObjPet::LeaveMap()
{
    if (!m_pMap)
        return;

    ClearTarget();
    clearAddonSkills();
    setFightState(PET_FIGHT_STATE::PFS_AWAY_FROM_MAP);
    broadcastLeave();
    Map::removePet(m_pMap, this);
    m_pMap = nullptr;
}

void CObjPet::EnterMap()
{
    if (!m_pPlayer)
        return;

    Map *pMap = StaticObj::getMap(m_pPlayer);
    if (!pMap)
        return;

    Direction dir = m_pPlayer->getDirection();
    int32_t y = StaticObj::getCurrentTile(m_pPlayer).y;
    int32_t x = StaticObj::getCurrentTile(m_pPlayer).x;
    Position targetTile = getFollowTile(x, y, dir);
    setAtTile( pMap, targetTile.x, targetTile.y);
    Map::addPet(pMap, this);
    ClearTarget();
    setFightState(PET_FIGHT_STATE::PFS_STAND);
}

void CObjPet::EnterMap(const Position *const pos)
{
    if (!m_pPlayer)
        return;

    Map *pMap = StaticObj::getMap(m_pPlayer);
    if (!pMap)
        return;

    setAtTile( pMap, pos->x, pos->y);
    Map::addPet(pMap, this);
    ClearTarget();
    setFightState(PET_FIGHT_STATE::PFS_STAND);
}

void CObjPet::OnLogout()
{
    if (m_pPlayer)
    {
        if (m_pMap)
            LeaveMap();
    }
}

void CObjPet::checkSwitchMap()
{
    if (!m_pPlayer)
        return;

    Map *pMap = StaticObj::getMap(m_pPlayer);
    if (!pMap)
        return;

    if (m_pPlayer->isAlive()
        && pMap->isPetMap()
        && m_pMap != pMap)
    {
        LeaveMap();
        EnterMap();
    }
}

void CObjPet::OnLevelUp()
{
    recalAttr();
    if (m_bAlive)
        SetHP(100);
    setNeedSync();
}

int8_t CObjPet::calTargetPriority(const UnitHandle *const target)
{
    switch (target->type)
    {
        case 2:
        {
            int32_t RunnerId = StaticObj::GetRunnerId(this);
            Monster *pMonster = GameService::getMonster(
                Answer::Singleton<GameService>::instance(),
                target->id, RunnerId);
            if (pMonster)
            {
                if (pMonster->isPetTarget())
                    return 2;
                return 1;
            }
            return 0;
        }
        case 4:
            return 3;
        case 1:
            return 4;
        default:
            return 0;
    }
}

void CObjPet::AddTarget(const UnitHandle *const target, bool bForce)
{
    if (!AliveFlag()
        || m_nState != static_cast<int32_t>(PET_STATE::PS_FIGHTING)
        || !m_nFightState
        || m_target == *target)
        return;

    if (target->type == 4)
    {
        if (m_target.type == 4)
        {
            if (Answer::Random::generate(Answer::Singleton<Answer::Random>::instance(), 1, 100) <= 69)
                return;
        }
    }
    else if (!m_target.isEmpty())
    {
        if (m_AiState == 1)
            return;
        if (Answer::Random::generate(Answer::Singleton<Answer::Random>::instance(), 1, 100) <= 69)
            return;
    }

    m_target = *target;
    setFightState(PET_FIGHT_STATE::PFS_WALK_ATTACK);
}

void CObjPet::ResetTargetTile(int32_t tx, int32_t ty, Direction dir)
{
    if (!m_pMap)
        return;

    bool v4 = HasBuffState(CObjState::Index_T::OBS_FROZEN)
        || HasBuffState(CObjState::Index_T::OBS_STUN)
        || HasBuffState(CObjState::Index_T::OBS_SLEEP)
        || HasBuffState(CObjState::Index_T::OBS_PLYSIS)
        || HasBuffState(CObjState::Index_T::OBS_FROZEN);

    if (!v4
        && (m_nFightState == static_cast<int32_t>(PET_FIGHT_STATE::PFS_STAND)
            || m_nFightState == static_cast<int32_t>(PET_FIGHT_STATE::PFS_FOLLOW)))
    {
        Position targetTile = getFollowTile(tx, ty, dir);
        if (Map::isWalkablePosition(m_pMap, targetTile.x, targetTile.y))
            setTargetTile( targetTile.x, targetTile.y);
        else
            setTargetTile( tx, ty);
        broadcastMove();
        setFightState(PET_FIGHT_STATE::PFS_FOLLOW);
    }
}

void CObjPet::backToPlayer()
{
    if (!m_pPlayer)
        return;

    if (!HasBuffState(CObjState::Index_T::OBS_FROZEN)
        && !HasBuffState(CObjState::Index_T::OBS_STUN)
        && !HasBuffState(CObjState::Index_T::OBS_SLEEP)
        && !HasBuffState(CObjState::Index_T::OBS_PLYSIS)
        && !HasBuffState(CObjState::Index_T::OBS_FROZEN))
    {
        ClearTarget();
        setFightState(PET_FIGHT_STATE::PFS_STAND);
        Direction dir = m_pPlayer->getDirection();
        int32_t y = StaticObj::getCurrentTile(m_pPlayer).y;
        int32_t x = StaticObj::getCurrentTile(m_pPlayer).x;
        ResetTargetTile(x, y, dir);
    }
}

Position CObjPet::getFollowTile(int32_t tx, int32_t ty, Direction dir) const
{
    int32_t nDir = (static_cast<int32_t>(dir) + static_cast<int32_t>(m_nStandPlace)) % 8;
    Position result;

    switch (static_cast<Direction>(nDir))
    {
        case Direction::DOWN:
            result.x = tx;
            result.y = ty + 3;
            break;
        case Direction::DOWN_RIGHT:
            result.x = tx + 2;
            result.y = ty + 2;
            break;
        case Direction::RIGHT:
            result.x = tx + 3;
            result.y = ty;
            break;
        case Direction::UP_RIGHT:
            result.x = tx + 2;
            result.y = ty - 2;
            break;
        case Direction::UP:
            result.x = tx;
            result.y = ty - 3;
            break;
        case Direction::UP_LEFT:
            result.x = tx - 2;
            result.y = ty - 2;
            break;
        case Direction::LEFT:
            result.x = tx - 3;
            result.y = ty;
            break;
        case Direction::DOWN_LEFT:
            result.x = tx - 2;
            result.y = ty + 2;
            break;
        default:
            result.x = tx;
            result.y = ty;
            break;
    }
    return result;
}

void CObjPet::recalAttr()
{
    resetUnitAttr();
    initBaseAttr();
    addSystemAttr();
    addPassiveSkillAttr();
    addEquipAttr();
    addOtherAttr();
    addSummonAttr();
    adjustUnitAttr();
    setNeedSync();
}

void CObjPet::initBaseAttr()
{
    if (!m_pCfgPet || !m_pPlayer)
        return;

    for (auto it = m_pCfgPet->m_vBaseAttr.begin(); it != m_pCfgPet->m_vBaseAttr.end(); ++it)
    {
        int32_t addon = it->addon;
        int32_t index = it->index;
        SetAttrValue( static_cast<CObjAttrs::Index_T>(index), addon);
    }
}

void CObjPet::addSystemAttr()
{
    if (!m_pPlayer)
        return;

    CExtCharPet *CharPet = Player::GetCharPet(m_pPlayer);
    CExtCharPet::AddPetAttr(CharPet, this);
}

void CObjPet::addPassiveSkillAttr()
{
    if (!m_pPlayer)
        return;

    for (int32_t i = 0; i <= 8; ++i)
    {
        if (m_vSkill[i].nId <= 0 || m_vSkill[i].nLevel <= 0)
            continue;

        int32_t nLevel = m_vSkill[i].nLevel;
        int32_t nId = m_vSkill[i].nId;
        CfgData *v3 = Answer::Singleton<CfgData>::instance();
        const CfgTalentTable *TalentTable = CfgData::GetTalentTable(v3);
        const CfgTalent *pCfgTalent = CfgTalentTable::GetTalent(TalentTable, nId, nLevel);
        if (!pCfgTalent)
            continue;

        int32_t skillid = pCfgTalent->skillid;
        if (skillid <= 0)
            continue;

        CfgData *v6 = Answer::Singleton<CfgData>::instance();
        const CfgSkillTable *SkillTable = CfgData::GetSkillTable(v6);
        const CfgActiveSkill *pCfgSkill = CfgSkillTable::GetActiveSkill(SkillTable, skillid);
        if (!pCfgSkill || pCfgSkill->skillType != 2)
            continue;

        for (auto iter = pCfgSkill->vAttrs.begin(); iter != pCfgSkill->vAttrs.end(); ++iter)
        {
            if (iter->addon > 0)
                AddAttrValue( static_cast<CObjAttrs::Index_T>(iter->index), iter->addon);
        }
    }
}

void CObjPet::AddEquipAttr(Unit *pTarget)
{
    if (!pTarget)
        return;

    CfgData *v2 = Answer::Singleton<CfgData>::instance();
    const CfgPetEquipTable *PetEquipTable = CfgData::GetPetEquipTable(v2);

    for (int8_t i = 0; i <= 3; ++i)
    {
        int32_t nEquipId = m_vEquip[i];
        if (nEquipId <= 0)
            continue;

        const CfgPetEquip *pCfgEquip = CfgPetEquipTable::GetEquip(PetEquipTable, nEquipId);
        if (!pCfgEquip)
            continue;

        for (auto iter = pCfgEquip->vOwnerAttr.begin(); iter != pCfgEquip->vOwnerAttr.end(); ++iter)
        {
            if (iter->addon > 0)
                pTarget->AddAttrValue( static_cast<CObjAttrs::Index_T>(iter->index), iter->addon);
        }
    }
}

int32_t CObjPet::GetSkillBattle() const
{
    int32_t nBattle = 0;
    for (int32_t i = 0; i <= 8; ++i)
    {
        if (m_vSkill[i].nId > 0 && m_vSkill[i].nLevel > 0)
        {
            int32_t nLevel = m_vSkill[i].nLevel;
            int32_t nId = m_vSkill[i].nId;
            CfgData *v3 = Answer::Singleton<CfgData>::instance();
            const CfgTalentTable *TalentTable = CfgData::GetTalentTable(v3);
            const CfgTalent *pCfgTalent = CfgTalentTable::GetTalent(TalentTable, nId, nLevel);
            if (pCfgTalent)
                nBattle += pCfgTalent->battle;
        }
    }
    return nBattle;
}

void CObjPet::sendPetLearnSkill(int32_t nSkill)
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *v3 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v3, ConnId, Answer::PackType::PACK_DISPATCH, 0x2DD5u);
    if (!packet)
        return;

    Answer::NetPacket::writeInt64(packet, m_nPetId);
    Answer::NetPacket::writeInt32(packet, nSkill);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v6 = Player::getConnId(m_pPlayer);
    GameService *v7 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v7, v6, GateIndex, packet);
}

void CObjPet::broadcastPetSkillLevelUp(int16_t nSkillId, int16_t nSkillLevel)
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
    if (!packet)
        return;

    Answer::NetPacket::writeInt32(packet, 113);

    std::string val;
    Player::getName(m_pPlayer, &val);
    Answer::NetPacket::writeUTF8(packet, &val);

    CharId_t Cid = Player::getCid(m_pPlayer);
    Answer::NetPacket::writeInt64(packet, Cid);

    int32_t BaseId = GetBaseId();
    Answer::NetPacket::writeInt32(packet, BaseId);

    PetId_t PetId = GetPetId();
    Answer::NetPacket::writeInt64(packet, PetId);

    Answer::NetPacket::writeInt16(packet, nSkillId);
    Answer::NetPacket::writeInt16(packet, nSkillLevel);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v4, ConnId, GateIndex, packet);
}

void CObjPet::OnLoadFromDB(const DBPet *const pet, int32_t nStar)
{
    CfgData *v3 = Answer::Singleton<CfgData>::instance();
    const CfgPetTable *PetTable = CfgData::GetPetTable(v3);
    m_pCfgPet = CfgPetTable::GetPet(PetTable, 101);

    if (m_pCfgPet)
    {
        m_nPetId = pet->nPetId;
        m_bAlive = pet->bAlive;
        m_nState = pet->nState;
        m_nDieTime = pet->nDieTime;
        m_AiState = pet->AiState;

        snprintf(m_strName, sizeof(m_strName), "%s", pet->strName);

        for (int8_t i = 0; i <= 3; ++i)
            m_vEquip[i] = pet->vEquip[i];

        for (int8_t i = 0; i <= 8; ++i)
        {
            if (m_pCfgPet->m_vSkill[i] > 0)
                m_vSkill[i].nId = m_pCfgPet->m_vSkill[i];
        }

        CheckSkillOpen(nStar);
        recalAttr();
        SetHP(pet->nHP);
    }
}

void CObjPet::OnSaveToDB(DBPet *const pet)
{
    pet->nPetId = m_nPetId;
    pet->bAlive = m_bAlive;
    pet->nState = m_nState;
    pet->nHP = getHp();
    pet->nDieTime = m_nDieTime;
    pet->AiState = m_AiState;

    snprintf(pet->strName, sizeof(pet->strName), "%s", m_strName);

    for (int8_t i = 0; i <= 3; ++i)
        pet->vEquip[i] = m_vEquip[i];

    for (int8_t i = 0; i <= 8; ++i)
        pet->vSkillLvel[i] = m_vSkill[i].nLevel;
}

void CObjPet::updateFightState()
{
    checkFarAway();
    PET_FIGHT_STATE m_nFightState = static_cast<PET_FIGHT_STATE>(this->m_nFightState);

    if (m_nFightState == PET_FIGHT_STATE::PFS_FOLLOW)
    {
        onFollow();
    }
    else if (m_nFightState > PET_FIGHT_STATE::PFS_FOLLOW)
    {
        if (m_nFightState == PET_FIGHT_STATE::PFS_ATTACK)
        {
            onAttack();
        }
        else if (m_nFightState == PET_FIGHT_STATE::PFS_WALK_ATTACK)
        {
            onWalkAttack();
        }
    }
    else if (m_nFightState == PET_FIGHT_STATE::PFS_STAND)
    {
        onStand();
    }
}

void CObjPet::setFightState(PET_FIGHT_STATE state)
{
    m_nFightState = static_cast<int32_t>(state);
    if (state == PET_FIGHT_STATE::PFS_STAND || state == PET_FIGHT_STATE::PFS_ATTACK)
        setStand();
}

void CObjPet::onStand()
{
    if (!m_pPlayer || !m_pMap || !AliveFlag())
        return;

    if (m_AiState == 1)
    {
        ClearTarget();
        setFightState(PET_FIGHT_STATE::PFS_STAND);
        return;
    }

    int64_t nowTime = getNow();
    if (nowTime - m_nLastSkillTick <= 499)
        return;

    Unit *pUnit = m_pMap->findPetTarget(this);
    if (pUnit)
    {
        Position pos = StaticObj::getCurrentTile(this);
        Position CurrentTile = StaticObj::getCurrentTile(pUnit);
        int32_t dist = Position::tileDistance(&pos, &CurrentTile);

        if (dist <= 5)
        {
            m_target = UnitHandle(pUnit->getEntityId(), pUnit->getEntityType());
            setFightState(PET_FIGHT_STATE::PFS_ATTACK);
        }
        else
        {
            backToPlayer();
        }
    }
    else
    {
        backToPlayer();
    }
}

void CObjPet::onFollow()
{
    if (!m_pPlayer || !m_pMap)
        return;

    Position param = getTargetTile();
    Position CurrentTile = StaticObj::getCurrentTile(this);
    if (CurrentTile.x == param.x && CurrentTile.y == param.y)
        setFightState(PET_FIGHT_STATE::PFS_STAND);
}

void CObjPet::changeTarget()
{
    if (m_AiState == 1)
    {
        ClearTarget();
        setFightState(PET_FIGHT_STATE::PFS_STAND);
        return;
    }

    Unit *pUnit = m_pMap->findPetTarget(this);
    if (pUnit)
    {
        Position pos = StaticObj::getCurrentTile(this);
        Position CurrentTile = StaticObj::getCurrentTile(pUnit);
        int32_t dist = Position::tileDistance(&pos, &CurrentTile);

        if (dist <= 5)
        {
            m_target = UnitHandle(pUnit->getEntityId(), pUnit->getEntityType());
        }
        else
        {
            backToPlayer();
        }
    }
    else
    {
        backToPlayer();
    }
}

void CObjPet::onAttack()
{
    if (!m_pPlayer || !m_pMap || !AliveFlag())
        return;

    int64_t nowTime = getNow();
    if (nowTime - m_nLastSkillTick <= 499)
        return;

    Unit *pTarget = m_target.getUnit();
    if (!pTarget || !pTarget->isAlive() || pTarget->getMap() != m_pMap)
    {
        changeTarget();
        return;
    }

    if (HasBuffState(CObjState::Index_T::OBS_SILENCE)
        || HasBuffState(CObjState::Index_T::OBS_STUN)
        || HasBuffState(CObjState::Index_T::OBS_SLEEP)
        || HasBuffState(CObjState::Index_T::OBS_PLYSIS)
        || HasBuffState(CObjState::Index_T::OBS_FROZEN))
        return;

    if (!m_pNextSkill)
        refreshSkill(nowTime);
    if (!m_pNextSkill)
        return;

    int32_t nLevel = m_pNextSkill->nLevel;
    int32_t nId = m_pNextSkill->nId;
    CfgData *v8 = Answer::Singleton<CfgData>::instance();
    const CfgTalentTable *TalentTable = CfgData::GetTalentTable(v8);
    const CfgTalent *pCfgTalent = CfgTalentTable::GetTalent(TalentTable, nId, nLevel);
    if (!pCfgTalent)
        return;

    int32_t skillid = pCfgTalent->skillid;
    CfgData *v10 = Answer::Singleton<CfgData>::instance();
    const CfgSkillTable *SkillTable = CfgData::GetSkillTable(v10);
    const CfgActiveSkill *pCfgSkill = CfgSkillTable::GetActiveSkill(SkillTable, skillid);
    if (!pCfgSkill)
        return;

    Position targetPos = StaticObj::getCurrentTile(pTarget);
    Position petPos = StaticObj::getCurrentTile(this);
    int32_t dist = Position::tileDistance(&petPos, &targetPos);
    int32_t range = pCfgSkill->range;
    if (range <= 0)
        range = 1;

    if (dist > range)
    {
        setFightState(PET_FIGHT_STATE::PFS_WALK_ATTACK);
        return;
    }

    if (pCfgSkill->IsRanged())
    {
        std::vector<UnitHandle> targets;
        Skill::getTargets(this, pCfgSkill, pTarget, &targets);

        Direction dir = getDirectionTo(pTarget);
        setDirection(dir);

        Skill::posAction(this, pCfgSkill, &targetPos, nullptr);
    }
    else
    {
        Direction dir = getDirectionTo(pTarget);
        setDirection(dir);

        Skill::unitAction(this, pCfgSkill, pTarget);
    }

    if (!pCfgSkill->IsBeneficial())
    {
        if (pTarget->getEntityType() == EntityType::ET_PLAYER)
            Player::onDamagedEvent(m_pPlayer, static_cast<Player*>(pTarget));

        if (pTarget->getEntityType() == EntityType::ET_PET)
        {
            CObjPet *pTargetPet = static_cast<CObjPet*>(pTarget);
            Player *pTargetPlayer = pTargetPet->GetPlayer();
            if (pTargetPlayer)
                Player::onDamagedEvent(m_pPlayer, pTargetPlayer);
        }

        if (!Map::IsPVPMap(m_pMap))
            Player::SetInPvP(m_pPlayer);
    }

    int32_t addon_skill = pCfgSkill->addon_skill;
    if (addon_skill > 0)
    {
        SkillAddonInfo info;
        info.nSkillId = addon_skill;
        info.nTarget = m_target;
        info.nLastTime = pCfgSkill->addon_last_time;
        info.nInterval = pCfgSkill->addon_interval;
        m_lAddonSkill.push_back(info);
    }

    m_pNextSkill->nCDTime = nowTime + pCfgSkill->nCDTime;
    m_nLastSkillTick = nowTime;
}

void CObjPet::onWalkAttack()
{
    if (!m_pPlayer || !m_pMap)
    {
        changeTarget();
        return;
    }

    Unit *pTarget = m_target.getUnit();
    if (!pTarget || !pTarget->isAlive() || pTarget->getMap() != m_pMap)
    {
        changeTarget();
        return;
    }

    if (m_AiState == 1)
    {
        backToPlayer();
        return;
    }

    if (!m_pNextSkill)
        refreshSkill(getNow());
    if (!m_pNextSkill)
    {
        backToPlayer();
        return;
    }

    int32_t nLevel = m_pNextSkill->nLevel;
    int32_t nId = m_pNextSkill->nId;
    CfgData *v4 = Answer::Singleton<CfgData>::instance();
    const CfgTalentTable *TalentTable = CfgData::GetTalentTable(v4);
    const CfgTalent *pCfgTalent = CfgTalentTable::GetTalent(TalentTable, nId, nLevel);
    if (!pCfgTalent)
    {
        backToPlayer();
        return;
    }

    int32_t skillid = pCfgTalent->skillid;
    CfgData *v7 = Answer::Singleton<CfgData>::instance();
    const CfgSkillTable *SkillTable = CfgData::GetSkillTable(v7);
    const CfgActiveSkill *pCfgSkill = CfgSkillTable::GetActiveSkill(SkillTable, skillid);
    if (!pCfgSkill)
    {
        backToPlayer();
        return;
    }

    Position targetPos = StaticObj::getCurrentTile(pTarget);
    Position petPos = StaticObj::getCurrentTile(this);
    int32_t dist = Position::tileDistance(&petPos, &targetPos);
    int32_t range = pCfgSkill->range;
    if (range <= 0)
        range = 1;

    if (dist > range)
    {
        Position targetTile;
        if (Map::getNearestWalkablePosition(m_pMap, targetPos.x, targetPos.y, &targetTile))
        {
            setTargetTile( targetTile.x, targetTile.y);
            broadcastMove();
        }
        else
        {
            backToPlayer();
        }
    }
    else
    {
        setFightState(PET_FIGHT_STATE::PFS_ATTACK);
    }
}

void CObjPet::checkFarAway()
{
    if (!m_pPlayer)
        return;

    if (HasBuffState(CObjState::Index_T::OBS_FROZEN)
        || HasBuffState(CObjState::Index_T::OBS_STUN)
        || HasBuffState(CObjState::Index_T::OBS_SLEEP)
        || HasBuffState(CObjState::Index_T::OBS_PLYSIS)
        || HasBuffState(CObjState::Index_T::OBS_FROZEN)
        || m_nFightState == static_cast<int32_t>(PET_FIGHT_STATE::PFS_ATTACK))
        return;

    Position pos = StaticObj::getCurrentTile(m_pPlayer);
    Position CurrentTile = StaticObj::getCurrentTile(this);
    if (Position::tileDistance(&CurrentTile, &pos) > 15)
    {
        ClearTarget();
        Direction dir = m_pPlayer->getDirection();
        int32_t y = StaticObj::getCurrentTile(m_pPlayer).y;
        int32_t x = StaticObj::getCurrentTile(m_pPlayer).x;
        Position targetTile = getFollowTile(x, y, dir);
        instantMove( targetTile.x, targetTile.y, InstanceMoveReason::IMR_BACK_PET, 0);
        setFightState(PET_FIGHT_STATE::PFS_STAND);

        if (m_pPlayer->isMoving())
        {
            Direction v5 = m_pPlayer->getDirection();
            int32_t v6 = m_pPlayer->getTargetTile().y;
            int32_t v7 = m_pPlayer->getTargetTile().x;
            ResetTargetTile(v7, v6, v5);
        }
    }
}

void CObjPet::checkSpeed()
{
    if (!m_pPlayer)
        return;

    int32_t v1 = getMoveSpeed();
    int32_t playerSpeed = m_pPlayer->GetMoveSpeed();
    if (v1 != playerSpeed)
    {
        SetAttrValue( CObjAttrs::Index_T::ATTR_MOV_SPD, playerSpeed);
        setNeedSync();
    }
}

int32_t CObjPet::calAddBattle(int32_t nIndex) const
{
    if (nIndex <= 0)
        return 0;

    switch (nIndex)
    {
        case 1:
            return 20;
        case 2:
            return 19;
        case 3:
            return 18;
    }

    if (nIndex <= 10)
        return 15;
    if (nIndex <= 20)
        return 13;
    if (nIndex <= 50)
        return 12;
    if (nIndex <= 80)
        return 10;
    if (nIndex <= 90)
        return 8;
    if (nIndex > 100)
        return 0;
    return 5;
}

void CObjPet::addEquipAttr()
{
    if (!m_pPlayer)
        return;

    CfgData *v1 = Answer::Singleton<CfgData>::instance();
    const CfgPetEquipTable *PetEquipTable = CfgData::GetPetEquipTable(v1);

    for (int8_t i = 0; i <= 3; ++i)
    {
        int32_t nEquipId = m_vEquip[i];
        if (nEquipId <= 0)
            continue;

        const CfgPetEquip *pCfgEquip = CfgPetEquipTable::GetEquip(PetEquipTable, nEquipId);
        if (!pCfgEquip)
            continue;

        for (auto iter = pCfgEquip->vAttr.begin(); iter != pCfgEquip->vAttr.end(); ++iter)
        {
            if (iter->addon > 0)
                AddAttrValue( static_cast<CObjAttrs::Index_T>(iter->index), iter->addon);
        }
    }
}

void CObjPet::addSummonAttr()
{
    if (!m_pPlayer || !IsSummonPet())
        return;

    CfgData *v3 = Answer::Singleton<CfgData>::instance();
    const CfgSkillTable *SkillTable = CfgData::GetSkillTable(v3);
    const CfgActiveSkill *pCfgSkill = CfgSkillTable::GetActiveSkill(SkillTable, m_nSummonSkillId);
    if (!pCfgSkill)
        return;

    for (auto iter = pCfgSkill->summon_attr.begin(); iter != pCfgSkill->summon_attr.end(); ++iter)
    {
        if (iter->addon > 0)
        {
            int32_t AttrValue = m_pPlayer->GetAttrValue( static_cast<CObjAttrs::Index_T>(iter->index));
            int32_t v7 = (iter->addon * AttrValue) / 100;
            AddAttrValue( static_cast<CObjAttrs::Index_T>(iter->index), v7);
        }
    }
}

int32_t CObjPet::GetEquip(int8_t nPos)
{
    if (static_cast<uint8_t>(nPos) < 4u)
        return m_vEquip[nPos];
    return 0;
}

void CObjPet::SetEquip(int8_t nPos, int32_t nEquipId)
{
    if (static_cast<uint8_t>(nPos) < 4u)
    {
        m_vEquip[nPos] = nEquipId;
        recalAttr();
    }
}

void CObjPet::ClearTarget()
{
    m_target = UnitHandle();
}

void CObjPet::clearTarget()
{
    m_target = UnitHandle();
}

bool CObjPet::AliveFlag()
{
    return m_bAlive && isAlive();
}

bool CObjPet::IsFighting()
{
    return m_nState == static_cast<int32_t>(PET_STATE::PS_FIGHTING) && m_bAlive;
}

bool CObjPet::IsSummonPet()
{
    return m_nSummonSkillId > 0;
}

int32_t CObjPet::getReliveTime()
{
    return m_nDieTime + m_pPlayer->getParameter(Param2::PET_RELIVE_TIME);
}

void CObjPet::setStand()
{
    Unit::setStand();
}

void CObjPet::onDie()
{
    Die();
}

int32_t CObjPet::getMoveSpeed() const
{
    return GetAttrValue(CObjAttrs::Index_T::ATTR_MOV_SPD);
}

int32_t CObjPet::getHpPercent() const
{
    if (GetMaxHP() <= 0)
        return 0;
    return static_cast<int32_t>(GetViewHP() * 100 / GetMaxHP());
}

int32_t CObjPet::getHp() const
{
    return GetViewHP();
}

int32_t CObjPet::getMaxHp() const
{
    return static_cast<int32_t>(GetMaxHP());
}

void CObjPet::refreshSkill(int64_t nCurTick)
{
    if (!m_pPlayer)
        return;

    for (int32_t i = 0; i <= 8; ++i)
    {
        if (m_vSkill[i].nId <= 0 || m_vSkill[i].nLevel <= 0)
            continue;

        int32_t nLevel = m_vSkill[i].nLevel;
        int32_t nId = m_vSkill[i].nId;
        CfgData *v4 = Answer::Singleton<CfgData>::instance();
        const CfgTalentTable *TalentTable = CfgData::GetTalentTable(v4);
        const CfgTalent *pCfgTalent = CfgTalentTable::GetTalent(TalentTable, nId, nLevel);
        if (!pCfgTalent)
            continue;

        int32_t skillid = pCfgTalent->skillid;
        if (skillid <= 0)
            continue;

        CfgData *v7 = Answer::Singleton<CfgData>::instance();
        const CfgSkillTable *SkillTable = CfgData::GetSkillTable(v7);
        const CfgActiveSkill *pCfgSkill = CfgSkillTable::GetActiveSkill(SkillTable, skillid);
        if (!pCfgSkill || pCfgSkill->skillType != 1)
            continue;

        if (nCurTick >= m_vSkill[i].nCDTime)
        {
            m_pNextSkill = &m_vSkill[i];
            return;
        }
    }
}
