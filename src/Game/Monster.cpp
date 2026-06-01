#include "Game/Monster.h"
#include "Game/Player.h"
#include "Game/Map.h"
#include "Game/GameService.h"
#include "Game/CObjPet.h"
#include "Game/CPoolManager.h"
#include "Game/CWorldBoss.h"
#include "Game/CActivityManager.h"
#include "Other/CObjState.h"
#include "Other/StaticObj.h"
#include "Other/CObjAttrs.h"
#include "Other/CExtCharFamily.h"
#include "Other/CExtCharTeam.h"
#include "Other/CXingMai.h"
#include "Utility/UnitHandle.h"
#include "Common/EntityType.h"
#include "Common/Random.h"
#include "Config/CfgData.h"
#include "Config/CfgMonster.h"
#include "Config/CfgMapMonster.h"
#include "Config/CfgActiveSkill.h"
#include "Config/CfgSkillTable.h"
#include "Config/CfgMonsterRandTable.h"
#include "Activity/CFestivalDoubleEleven.h"
#include "Activity/CUniteServer.h"
#include "Activity/COpenBeta.h"
#include <algorithm>
#include <cmath>

Monster::Monster()
{
    Unit::Unit(this, EntityType::ET_MONSTER);
    CfgMonster::CfgMonster(&this->m_cfgmonster);
    std::list<int>::list(&this->m_lRandType);
    UnitHandle::UnitHandle(&this->m_killer, 0, 0);
    UnitHandle::UnitHandle(&this->m_target, 0, 0);
    std::map<long,UnitHandle>::map(&this->m_targetMap);
    std::string::string(&this->m_strFamilyName);
    this->m_nSyncTick = 0;
    this->m_nLastSyncTick = 0;
    std::list<long>::list(&this->m_lDamager);
    std::map<long,PlayerScore>::map(&this->m_PlayerScore);
    std::multiset<PlayerScore *,PlayerScoreRankGreater,std::allocator<PlayerScore *>>::multiset(&this->m_ScoreRank);
    std::map<int,PlayerScore>::map(&this->m_TeamScore);
    std::vector<Position>::vector(&this->m_RevivePos);
    std::string::string(&this->m_OwnerName);
    std::map<long,ActStruct>::map(&this->m_ActStructMap);
}

Monster::~Monster()
{
    std::map<long,ActStruct>::~map(&this->m_ActStructMap);
    std::string::~string(&this->m_OwnerName);
    std::vector<Position>::~vector(&this->m_RevivePos);
    std::map<int,PlayerScore>::~map(&this->m_TeamScore);
    std::multiset<PlayerScore *,PlayerScoreRankGreater,std::allocator<PlayerScore *>>::~multiset(&this->m_ScoreRank);
    std::map<long,PlayerScore>::~map(&this->m_PlayerScore);
    std::list<long>::~list(&this->m_lDamager);
    std::string::~string(&this->m_strFamilyName);
    std::map<long,UnitHandle>::~map(&this->m_targetMap);
    std::list<int>::~list(&this->m_lRandType);
    CfgMonster::~CfgMonster(&this->m_cfgmonster);
    Unit::~Unit(this);
}

bool Monster::refresh()
{
    Monster::updateState(this);
    Monster::checkHPRevive(this);
    Monster::checkLifeTime(this);
    if ( Monster::needCheckBossRank(this) )
    {
        Monster::checkEnterBossRegion(this);
        Monster::checkLeaveBossRegion(this);
        if ( this->m_DamageFlag )
        {
            Monster::refreshRank(this);
            Monster::SendAllDamageRank(this);
            this->m_DamageFlag = 0;
        }
    }
    Monster::ChangeOwner(this);
    Monster::SyncAround(this);
    if ( !Unit::isAlive(this) && this->m_state != MonsterState::MS_CORPSE && this->m_state != MonsterState::MS_GHOST )
        (*((void (__fastcall **)(Monster *const))this->_vptr_Entity + 46))(this);
    return 0;
}

void Monster::ChangeOwner()
{
    if ( this->m_ChangeOwner && this->m_OwnerLastAttack > 0 )
    {
        int32_t RunnerId = StaticObj::GetRunnerId(this);
        CharId_t m_OwnerId = this->m_OwnerId;
        GameService *v3 = Answer::Singleton<GameService>::instance();
        Player *Owner = GameService::getPlayer(v3, m_OwnerId, RunnerId, 1);
        if ( !Owner
            || StaticObj::getMapId(Owner) != Map::GetMapId(this->m_pMap)
            || Unit::getNow(this) - this->m_OwnerLastAttack > 5
            || !Unit::isAlive(Owner) )
        {
            this->m_OwnerName.clear();
            this->m_OwnerId = 0;
            this->m_OwnerLastAttack = 0;
            Monster::SetNeedSyncAround(this);
        }
    }
}

void Monster::getItem(MemChrBag *__return_ptr retstr)
{
    *retstr = this->m_cfgmonster.cItem;
}

void Monster::reset()
{
    Unit::reset(this);
    this->m_lastActionTick = 0;
    this->m_standTick = 0;
    this->m_standTime = 0;
    this->m_corpseTick = 0;
    this->m_ghostTick = 0;
    this->m_attackTick = 0;
    this->m_attackTime = 0;
    this->m_nFamilyId = 0;
    this->m_lastUpdateMinute = 0;
    this->m_DieType = 0;
    this->m_bHPRevive = 0;
    this->m_delFlag = 0;
    this->m_nLastReviveHPTick = 0;
    this->m_nSyncTick = 0;
    this->m_nLastSyncTick = 0;
    this->m_nLifeTime = -1;
    this->m_nReviveTime = -1;
    this->m_nLastCheckBossTick = 0;
    this->m_DamageFlag = 0;
    this->m_nEscapeTick = 0;
    this->m_nHPPecent = 0;
    this->m_nEscapeDirection = Direction::DOWN;
    this->m_nRunTick = 0;
    this->m_OwnerName.clear();
    this->m_OwnerId = 0;
    this->m_OwnerLastAttack = 0;
    this->m_NeedSyncAround = 0;
    std::map<long,PlayerScore>::clear(&this->m_PlayerScore);
    std::multiset<PlayerScore *,PlayerScoreRankGreater,std::allocator<PlayerScore *>>::clear(&this->m_ScoreRank);
    UnitHandle::clear(&this->m_target);
    UnitHandle::clear(&this->m_killer);
    std::list<long>::clear(&this->m_lDamager);
    std::vector<Position>::clear(&this->m_RevivePos);
    this->m_IsGroupMonster = 0;
    this->m_MaxRegion = 0;
    this->m_InRegion = 0;
    this->m_ChangeOwner = 1;
    this->m_nActivityId = 0;
    this->m_MonsterHp = 0;
    this->m_TunShi = 0;
    this->m_HaveItem = 1;
    this->m_strFamilyName.clear();
    std::map<long,ActStruct>::clear(&this->m_ActStructMap);
    Monster::resetSkill(this);
}

void Monster::addDamager(CharId_t cid)
{
    CharId_t cida = cid;
    auto endIt = std::list<long>::end(&this->m_lDamager);
    auto found = std::find(
        std::list<long>::begin(&this->m_lDamager),
        endIt,
        cida);
    if ( found == endIt )
        std::list<long>::push_back(&this->m_lDamager, &cida);
}

Position Monster::getRunPosition(const Position *const tarPos, int32_t nRunDistance)
{
    Position pos = StaticObj::getCurrentTile(this);
    Position curPos = pos;
    if ( !this->m_pMap )
        return pos;
    int32_t nDir = Map::tileDirection(tarPos->x, tarPos->y, pos.x, pos.y);
    if ( nDir == -1 )
        return pos;
    Int32Vector vDir;
    std::vector<int>::reserve(&vDir, 5u);
    std::vector<int>::push_back(&vDir, &nDir);
    int dir1 = (nDir + 1) % 8;
    std::vector<int>::push_back(&vDir, &dir1);
    int dir2 = (nDir + 2) % 8;
    std::vector<int>::push_back(&vDir, &dir2);
    int dir3 = (nDir + 6) % 8;
    std::vector<int>::push_back(&vDir, &dir3);
    int dir4 = (nDir + 7) % 8;
    std::vector<int>::push_back(&vDir, &dir4);
    Answer::Random *v4 = Answer::Singleton<Answer::Random>::instance();
    int32_t nRand = Answer::Random::generate(v4, 0, 4);
    int32_t nRandDir = *std::vector<int>::operator[](&vDir, nRand);
    for ( int32_t i = 0; i < nRunDistance; ++i )
    {
        if ( !Map::isWalkablePosition(this->m_pMap, pos.x, pos.y) )
            break;
        int32_t v5 = Position::tileDistance(&curPos, &pos);
        if ( v5 > Monster::getMoveRange(this) )
            break;
        pos = Map::getAroundTile(pos.x, pos.y, (Direction)nRandDir);
    }
    Position v3 = pos;
    std::vector<int>::~vector(&vDir);
    return v3;
}

void Monster::SetOwner(CharId_t Cid, std::string *p_Name)
{
    this->m_OwnerId = Cid;
    this->m_OwnerName = *p_Name;
    this->m_OwnerLastAttack = Unit::getNow(this);
    Monster::SetNeedSyncAround(this);
}

void Monster::postDamage(int32_t damge, UnitHandle launcher, int32_t Mid)
{
    if ( Mid > 0 )
        Monster::updatePlayerDamage(this, launcher, damge);
    int32_t RunnerId = StaticObj::GetRunnerId(this);
    GameService *v5 = Answer::Singleton<GameService>::instance();
    int32_t v6 = StaticObj::GetRunnerId(this);
    GameService *v7 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = (*((int64_t (__fastcall **)(UnitHandle *))launcher->_vptr_UnitHandle + 1))(&launcher);
    Player *pPlayer = GameService::getPlayer(v7, Cid, v6, 1);
    if ( pPlayer )
    {
        ActStruct *v9 = std::map<long,ActStruct>::operator[](&this->m_ActStructMap, &Cid);
        ActStruct *v10 = v9;
        v9->damage += damge;
        v10->nMid = Mid;
    }
}

void Monster::SetGroupMonster(int32_t MaxRegion, int32_t InRegion)
{
    this->m_IsGroupMonster = 1;
    this->m_MaxRegion = MaxRegion;
    this->m_InRegion = InRegion;
}

void Monster::SetRevivePos(std::vector<Position> *p_PosVector)
{
    this->m_RevivePos = *p_PosVector;
}

Position Monster::GetGroupMonsterPos()
{
    if ( this->m_RevivePos.empty() )
        return Monster::getBornPosition(this);
    int32_t m_InRegion = this->m_InRegion;
    int32_t m_MaxRegion = this->m_MaxRegion;
    int32_t MapId = Map::GetMapId(this->m_pMap);
    TileManager *v6 = Answer::Singleton<TileManager>::instance();
    Position Pos = TileManager::getRandPosByMap(v6, MapId, m_MaxRegion, m_InRegion);
    return Pos;
}

Position Monster::GetNearestPos()
{
    Position result = StaticObj::getCurrentTile(this);
    if ( this->m_RevivePos.empty() )
        return result;
    int32_t minDist = 9999999;
    for ( auto it = this->m_RevivePos.begin(); it != this->m_RevivePos.end(); ++it )
    {
        int32_t dist = std::abs((int32_t)(result.x - it->x)) + std::abs((int32_t)(result.y - it->y));
        if ( dist < minDist )
        {
            minDist = dist;
            result = *it;
        }
    }
    return result;
}

void Monster::SetNeedSyncAround()
{
    if ( this->m_cfgmonster.boss_sign > 0 )
        this->m_NeedSyncAround = 1;
}

void Monster::SyncAround()
{
    if ( this->m_NeedSyncAround )
    {
        GameService *v1 = Answer::Singleton<GameService>::instance();
        std::string val;
        int cid = (*((int64_t (__fastcall **)(Monster *const))this->_vptr_Entity + 12))(this);
        Player *pPlayer = GameService::getPlayer(v1, cid, StaticObj::GetRunnerId(this), 1);
        if ( pPlayer )
        {
            Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(Player::getConn(pPlayer));
            if ( packet )
            {
                Player::appendBossInfo(pPlayer, packet, this);
                Answer::NetPacket::setType(packet, Answer::PackType::PACK_USER);
                Answer::NetPacket::setProc(packet, 0x4E32u);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                Answer::TcpClient::sendPacket(Player::getConn(pPlayer), packet);
            }
        }
        this->m_NeedSyncAround = 0;
    }
}

void Monster::OnLevelUp(const CfgMonster *const cfgMonster)
{
    if ( cfgMonster )
    {
        for ( int32_t i = 0; i <= 49; ++i )
            Unit::SetAttrValue(this, (const CObjAttrs::Index_T)i, cfgMonster->vAttr[i]);
        this->m_cfgmonster = *cfgMonster;
    }
}

bool Monster::appendInfo(Answer::NetPacket *packet)
{
    int8_t Type = (int8_t)this->m_state;
    Answer::NetPacket::writeInt8(packet, Type);
    int64_t v4 = (*((int64_t (__fastcall **)(Monster *const))this->_vptr_Entity + 12))(this);
    Answer::NetPacket::writeInt64(packet, v4);
    Answer::NetPacket::writeInt32(packet, this->m_cfgmonster.id);
    Answer::NetPacket::writeInt16(packet, this->m_currentTile.x);
    Answer::NetPacket::writeInt16(packet, this->m_currentTile.y);
    Answer::NetPacket::writeInt16(packet, (int16_t)this->m_direction);
    Answer::NetPacket::writeInt16(packet, (int16_t)this->m_attackedDir);
    Answer::NetPacket::writeInt64(packet, this->m_target);
    return true;
}

void Monster::Kill(Player *pKiller)
{
    Unit::setHP(this, 0);
    if ( pKiller )
    {
        int64_t src = (*((int64_t (__fastcall **)(Player *))pKiller->_vptr_Entity + 12))(pKiller);
        Monster::setKiller(this, src);
    }
}

Position Monster::getBornPosition() const
{
    return Position(this->m_cfgmapmonster.x, this->m_cfgmapmonster.y);
}

int32_t Monster::getReviveTime() const
{
    if ( this->m_cfgmonster.revive_skin <= 0 )
        return 0;
    if ( this->m_nReviveTime >= 0 )
    {
        if ( this->m_nReviveTime > 0 )
            return this->m_nReviveTime;
    }
    return Monster::GetMonsterCfgReviveTime(this);
}

bool Monster::needDel() const
{
    return this->m_delFlag && std::list<UnitAddonSkill>::empty(&this->m_lAddonSkill);
}

void Monster::destroy()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    GameService::removeMonster(v1, this);
    CPoolManager *v2 = Answer::Singleton<CPoolManager>::instance();
    CPoolManager::push<Monster>(v2, this);
}

Player *Monster::getKiller()
{
    int32_t RunnerId = StaticObj::GetRunnerId(this);
    EntityId_t id = (*((int64_t (__fastcall **)(UnitHandle *))&this->m_killer + 1))(&this->m_killer);
    GameService *v3 = Answer::Singleton<GameService>::instance();
    Player *pKiller = GameService::getPlayer(v3, id, RunnerId, 1);
    if ( !pKiller )
    {
        int32_t v4 = StaticObj::GetRunnerId(this);
        EntityId_t v5 = (*((int64_t (__fastcall **)(UnitHandle *))&this->m_killer + 1))(&this->m_killer);
        GameService *v6 = Answer::Singleton<GameService>::instance();
        CObjPet *pPet = GameService::getPet(v6, v5, v4);
        if ( pPet )
        {
            pKiller = CObjPet::getOwner(pPet);
        }
    }
    return pKiller;
}

bool Monster::isActivity(int32_t Type, int32_t *const setValue)
{
    bool isFlg = 0;
    if ( this->m_cfgSpecialMonster.Type == Type )
    {
        isFlg = 1;
        *setValue = this->m_cfgSpecialMonster.Value;
    }
    return isFlg;
}

bool Monster::isActivity(int32_t Type)
{
    return this->m_cfgSpecialMonster.Type == Type;
}

void Monster::SetFamilyId(FamilyId_t fid, std::string *p_strFamilyName)
{
    this->m_nFamilyId = fid;
    this->m_strFamilyName = *p_strFamilyName;
    Unit::setNeedSync(this);
}

void Monster::broadcastIntoMap()
{
    if ( this->m_pMap )
        Map::broadcastIntoMap(this->m_pMap, this);
}

void Monster::updateState()
{
    switch ( this->m_state )
    {
        case MonsterState::MS_STAND:
            Monster::onStand(this);
            break;
        case MonsterState::MS_WALK_AROUND:
            Monster::onWalkAround(this);
            break;
        case MonsterState::MS_WALK_ATTACK:
            Monster::onWalkAttack(this);
            break;
        case MonsterState::MS_ATTACK:
            Monster::onAttack(this);
            break;
        case MonsterState::MS_WALK_HOME:
            Monster::onWalkHome(this);
            break;
        case MonsterState::MS_CORPSE:
            Monster::onCorpse(this);
            break;
        case MonsterState::MS_GHOST:
            Monster::onGhost(this);
            break;
        case MonsterState::MS_ESCAPE:
            Monster::onEscape(this);
            break;
        case MonsterState::MS_RUN_ON_ROAD:
            Monster::onRunOnRoad(this);
            break;
        case MonsterState::MS_RUN_AROUND:
            Monster::onRunAround(this);
            break;
        default:
            break;
    }
}

void Monster::onStand()
{
    if ( Unit::HasBuffState(this, CObjState::Index_T::OBS_BINGDONG)
        || Unit::HasBuffState(this, CObjState::Index_T::OBS_YUNXUAN)
        || Unit::HasBuffState(this, CObjState::Index_T::OBS_SLEEP)
        || Unit::HasBuffState(this, CObjState::Index_T::OBS_PLYSIS)
        || Unit::HasBuffState(this, CObjState::Index_T::OBS_FROZEN) )
        return;
    if ( Unit::getTick(this) - this->m_standTick >= this->m_standTime )
    {
        Answer::Random *v3 = Answer::Singleton<Answer::Random>::instance();
        int32_t v6 = Answer::Random::generate(v3, 0, 100);
        if ( v6 < Monster::getEscapeOdds(this) )
        {
            Monster::setEscapeTarget(this);
            Monster::setState(this, MonsterState::MS_ESCAPE);
        }
        else
        {
            Position targetTile;
            Answer::Random *v4 = Answer::Singleton<Answer::Random>::instance();
            int32_t nRange = Answer::Random::generate(v4, 0, Monster::getMoveRange(this));
            if ( nRange == 0 )
                nRange = 1;
            Monster::getTargetTile(this, &targetTile, nRange);
            if ( targetTile.x == this->m_currentTile.x && targetTile.y == this->m_currentTile.y )
            {
                Monster::setState(this, MonsterState::MS_STAND);
            }
            else
            {
                Unit::moveToTile(this, this->m_pMap, targetTile.x, targetTile.y);
                Monster::setState(this, MonsterState::MS_WALK_AROUND);
            }
        }
    }
}

void Monster::onWalkAround()
{
    if ( !Unit::isMoving(this) )
        Monster::setState(this, MonsterState::MS_STAND);
}

void Monster::onRunAround()
{
    if ( !Unit::isMoving(this) )
        Monster::setState(this, MonsterState::MS_WALK_ATTACK);
}

bool Monster::AnyAttacks(EntityId_t id)
{
    EntityId_t ida = id;
    auto it = std::map<long,UnitHandle>::find(&this->m_targetMap, &ida);
    auto __x = std::map<long,UnitHandle>::end(&this->m_targetMap);
    if ( it != __x )
        return true;
    return false;
}

void Monster::onWalkAttack()
{
    if ( !Unit::isMoving(this) )
    {
        UnitHandle target = this->m_target;
        if ( UnitHandle::isEmpty(&target) )
        {
            Monster::setState(this, MonsterState::MS_STAND);
        }
        else
        {
            EntityId_t id = (*((int64_t (__fastcall **)(UnitHandle *))target._vptr_UnitHandle + 1))(&target);
            int32_t RunnerId = StaticObj::GetRunnerId(this);
            GameService *v4 = Answer::Singleton<GameService>::instance();
            Unit *pTarget = GameService::getUnit(v4, id, RunnerId);
            if ( pTarget && Unit::isAlive(pTarget) )
            {
                int32_t v8 = Monster::getAttackRange(this);
                int32_t distance = Position::tileDistance(&this->m_currentTile, &pTarget->m_currentTile);
                if ( distance <= v8 )
                {
                    Monster::setState(this, MonsterState::MS_ATTACK);
                }
                else
                {
                    Monster::setEscapeTarget(this);
                    Monster::setState(this, MonsterState::MS_ESCAPE);
                }
            }
            else
            {
                Monster::setState(this, MonsterState::MS_STAND);
            }
        }
    }
}

void Monster::onWalkHome()
{
    if ( !Unit::isMoving(this) )
    {
        if ( Monster::getSide(this) == 1 )
        {
            Unit::setAtTile(this, this->m_pMap, this->m_cfgmapmonster.x, this->m_cfgmapmonster.y);
            UnitHandle::clear(&this->m_killer);
            UnitHandle::clear(&this->m_target);
            std::map<long,UnitHandle>::clear(&this->m_targetMap);
        }
    }
}

void Monster::onAttack()
{
    int32_t RunnerId = StaticObj::GetRunnerId(this);
    int32_t type = Monster::getAtkType(this);
    EntityId_t id = (*((int64_t (__fastcall **)(UnitHandle *))&this->m_target + 1))(&this->m_target);
    GameService *v4 = Answer::Singleton<GameService>::instance();
    Unit *pTarget = GameService::getUnit(v4, id, RunnerId);
    if ( pTarget && Unit::isAlive(pTarget) && StaticObj::getMap(pTarget) == this->m_pMap )
    {
        if ( !Monster::checkSkillTarget(this, this->m_pCfgSkill, pTarget) )
        {
            Monster::setState(this, MonsterState::MS_STAND);
            return;
        }
        if ( Monster::getSide(this) == Monster::IsEnemy(this, pTarget) )
        {
            autoSkill = Monster::getActiveSkill(this);
            if ( autoSkill.pCfg && autoSkill.pCfg->skill_range > 0 )
            {
                Unit::setDirection(this, Unit::getDirectionTo(this, pTarget));
                int32_t y = Monster::getAttackRange(this);
                int32_t distance = Position::tileDistance(&this->m_currentTile, &pTarget->m_currentTile);
                if ( distance <= autoSkill.pCfg->skill_range )
                {
                    Unit::useSkill(this, pTarget, SKILL_TYPE::ST_DAMAGE_SKILL, &autoSkill);
                }
                else
                {
                    int32_t run_range = Monster::getRunRange(this);
                    Position runPos = Monster::getRunPosition(this, &pTarget->m_currentTile, run_range);
                    Unit::moveToTile(this, this->m_pMap, runPos.x, runPos.y);
                    Monster::setState(this, MonsterState::MS_WALK_ATTACK);
                }
            }
            else
            {
                Monster::setEscapeTarget(this);
                Monster::setState(this, MonsterState::MS_ESCAPE);
            }
        }
        else
        {
            Monster::setState(this, MonsterState::MS_STAND);
        }
    }
}

void Monster::onEscape()
{
    if ( this->m_pMap && Unit::getTick(this) - this->m_nEscapeTick > 9999 )
    {
        if ( UnitHandle::isEmpty(&this->m_target) )
            Monster::setState(this, MonsterState::MS_STAND);
        else
            Monster::setState(this, MonsterState::MS_ATTACK);
    }
}

void Monster::setEscapeTarget()
{
    if ( !this->m_pMap )
        return;
    Position targetTile;
    Position pos;
    Answer::Random *v3 = Answer::Singleton<Answer::Random>::instance();
    int32_t nDir = Answer::Random::generate(v3, 0, 7);
    Position curPos = StaticObj::getCurrentTile(this);
    for ( targetTile = curPos; ; targetTile = Map::getAroundTile(targetTile.x, targetTile.y, (Direction)nDir) )
    {
        if ( !Map::isWalkablePosition(this->m_pMap, targetTile.x, targetTile.y) )
            break;
        int32_t dist = Position::tileDistance(&curPos, &targetTile);
        if ( dist > Monster::getEscapeRange(this) )
            break;
    }
    if ( targetTile.x == curPos.x && targetTile.y == curPos.y )
        Map::getAroudFreeTile(this->m_pMap, &targetTile, curPos.x, curPos.y);
    Unit::moveToTile(this, this->m_pMap, targetTile.x, targetTile.y);
    this->m_nEscapeTick = Unit::getTick(this);
}

bool Monster::checkSkillTarget(const CfgActiveSkill *pCfg, Unit *pUnit)
{
    if ( !pUnit || !pCfg )
        return false;
    FamilyId_t FamilyId = 0;
    int32_t RunnerId = StaticObj::GetRunnerId(this);
    GameService *v6 = Answer::Singleton<GameService>::instance();
    if ( Unit::getType(pUnit) == EntityType::ET_PLAYER )
    {
        CharId_t id = (*((int64_t (__fastcall **)(Unit *))pUnit->_vptr_Entity + 12))(pUnit);
        Player *player = GameService::getPlayer(v6, id, RunnerId, 1);
        if ( player )
            FamilyId = CExtCharFamily::GetFamilyId(player->m_extData);
    }
    else if ( Unit::getType(pUnit) == EntityType::ET_PET )
    {
        CharId_t id = (*((int64_t (__fastcall **)(Unit *))pUnit->_vptr_Entity + 12))(pUnit);
        CObjPet *pPet = GameService::getPet(v6, id, RunnerId);
        if ( pPet )
        {
            Player *owner = CObjPet::getOwner(pPet);
            if ( owner )
                FamilyId = CExtCharFamily::GetFamilyId(owner->m_extData);
        }
    }
    else if ( Unit::getType(pUnit) == EntityType::ET_MONSTER )
    {
        Monster *monster = static_cast<Monster*>(pUnit);
        FamilyId = monster->m_nFamilyId;
    }
    if ( pCfg->targetType == 1 )
    {
        if ( FamilyId != 0 && !CExtCharFamily::IsDeclareWarFamily(FamilyId, FamilyId) )
            return false;
    }
    else
    {
        if ( FamilyId == 0 )
            return true;
    }
    return true;
}

void Monster::onCorpse()
{
    if ( Monster::isTrap(this) )
    {
        Monster::leaveMap(this);
        Monster::setState(this, MonsterState::MS_GHOST);
    }
    else if ( this->m_TunShi || Unit::getTick(this) - this->m_corpseTick >= this->m_cfgmonster.corpse_time )
    {
        Monster::leaveMap(this);
        Monster::setState(this, MonsterState::MS_GHOST);
    }
}

void Monster::onGhost()
{
    Map *m_pMap = this->m_pMap;
    if ( m_pMap && !Monster::isTrap(this) )
    {
        if ( Monster::getSide(this) == 1 || Monster::IsRankBoss(this) )
        {
            Monster::OnRevive(this);
        }
        else
        {
            int32_t MapId = Map::GetMapId(m_pMap);
            if ( MapId > 0 )
            {
                CWorldBoss *v6 = Answer::Singleton<CWorldBoss>::instance();
                Position pos = CWorldBoss::GetWorldBossPos(v6, this->m_cfgmonster.id);
                if ( pos.x <= 0 && pos.y <= 0 )
                {
                    Monster::OnRevive(this);
                }
            }
        }
    }
}

void Monster::OnRevive()
{
    (*((void (__fastcall **)(Monster *const))this->_vptr_Entity + 47))(this);
    Unit::setAtTile(this, this->m_pMap, this->m_cfgmapmonster.x, this->m_cfgmapmonster.y);
    Monster::broadcastIntoMap(this);
    Monster::setState(this, MonsterState::MS_STAND);
}

bool Monster::IsShow()
{
    return Unit::isAlive(this) || this->m_cfgmonster.revive_skin > 0;
}

void Monster::onRunOnRoad()
{
    if ( this->m_pMap )
    {
        if ( Monster::getViewRange(this) > 0 )
        {
            int64_t nowTime = Unit::getTick(this);
            Unit *pUnit = Monster::findNextTarget(this);
            if ( pUnit )
            {
                if ( !Unit::HasBuffState(this, CObjState::Index_T::OBS_FROZEN) )
                {
                    Monster::setState(this, MonsterState::MS_WALK_ATTACK);
                }
            }
            else if ( this->m_standTime > 0 && nowTime - this->m_standTime >= this->m_cfgmonster.move_time )
            {
                Position targetTile;
                Monster::getTargetTile(this, &targetTile, Monster::getMoveRange(this));
                if ( targetTile.x != this->m_currentTile.x || targetTile.y != this->m_currentTile.y )
                {
                    Unit::moveToTile(this, this->m_pMap, targetTile.x, targetTile.y);
                }
            }
        }
    }
}

bool Monster::needSync()
{
    if ( this->m_nSyncTick > 0 )
    {
        if ( !Unit::needSync(this) )
            return 0;
        int64_t curTick = Unit::getTick(this);
        if ( curTick - this->m_nLastSyncTick < this->m_nSyncTick )
            return 0;
    }
    this->m_nLastSyncTick = Unit::getTick(this);
    return 1;
}

void Monster::setState(MonsterState state)
{
    this->m_state = state;
    switch ( state )
    {
        case MonsterState::MS_STAND:
        {
            Answer::Random *v2 = Answer::Singleton<Answer::Random>::instance();
            int32_t rest_time_max = this->m_cfgmonster.rest_time_max;
            int32_t rest_time_min = this->m_cfgmonster.rest_time_min;
            if ( rest_time_max <= rest_time_min )
                rest_time_max = rest_time_min + 2000;
            this->m_standTime = Answer::Random::generate(v2, rest_time_min, rest_time_max);
            this->m_standTick = Unit::getTick(this);
            break;
        }
        case MonsterState::MS_CORPSE:
            this->m_corpseTick = Unit::getTick(this);
            break;
        case MonsterState::MS_GHOST:
            this->m_ghostTick = Unit::getTick(this);
            break;
        case MonsterState::MS_ESCAPE:
            this->m_nEscapeTick = Unit::getTick(this);
            break;
        default:
            break;
    }
    Unit::setNeedSync(this);
}

void Monster::onSendKillerReward(Player *pPlayer)
{
    if ( pPlayer )
    {
        Answer::Random *v2 = Answer::Singleton<Answer::Random>::instance();
        int32_t money = Answer::Random::generate(v2, this->m_cfgmonster.money_min, this->m_cfgmonster.money_max);
        if ( money > 0 )
        {
            Player::addMoney(pPlayer, money, MoneyChangeType::MCT_MONSTER_DROP);
        }
    }
}

Player *Monster::GetOwner()
{
    int32_t RunnerId = StaticObj::GetRunnerId(this);
    CharId_t m_OwnerId = this->m_OwnerId;
    GameService *v3 = Answer::Singleton<GameService>::instance();
    return GameService::getPlayer(v3, m_OwnerId, RunnerId, 1);
}

int32_t Monster::CanAddBossSocre(Player *pPlayer, int32_t nAddValue)
{
    if ( !pPlayer )
        return 0;
    int32_t AlreadyGet = Player::getRecord(pPlayer, 2083);
    int32_t CanGetMax = Monster::getBossScoreMax(this);
    if ( AlreadyGet >= CanGetMax )
        return 0;
    return 1;
}

void Monster::die()
{
    int8_t autoPick = 0;
    int8_t bossSign = this->m_cfgmonster.boss_sign;
    int32_t MonsterMid = this->m_cfgmonster.id;
    ChrTask *Task = Player::getTask(this->m_killer);
    if ( Task )
        ChrTask::onKillMonster(Task, MonsterMid);
    int32_t BossSocre = Monster::getKillBossScore(this);
    if ( BossSocre > 0 )
    {
        Player *pKiller = Monster::getKiller(this);
        if ( pKiller && Monster::CanAddBossSocre(this, pKiller, BossSocre) )
        {
            Player::addRecord(pKiller, 2083, BossSocre);
        }
    }
    if ( Monster::isWorldBoss(this) )
    {
        CWorldBoss *v7 = Answer::Singleton<CWorldBoss>::instance();
        CWorldBoss::onDie(v7, this);
    }
    Monster::MonsterDamageDrop(this, Monster::getKiller(this));
}

void Monster::ActDrop(DropItemVector *const dropItems)
{
    CUniteServer *v2 = Answer::Singleton<CUniteServer>::instance();
    if ( CUniteServer::isActive(v2) )
    {
        CUniteServer *v3 = Answer::Singleton<CUniteServer>::instance();
        CUniteServer::onMonsterDie(v3, this, dropItems);
    }
    COpenBeta *v10 = Answer::Singleton<COpenBeta>::instance();
    COpenBeta::onMonsterDie(v10, this, dropItems);
}

void Monster::revive()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    GameService::addMonster(v1, this);
    int32_t Broadcast = 1;
    if ( Monster::isWorldBoss(this) )
    {
        CWorldBoss *v5 = Answer::Singleton<CWorldBoss>::instance();
        CWorldBoss::onRevive(v5, this);
    }
    Monster::broadcastIntoMap(this);
}

int32_t Monster::getMinute()
{
    tm tmnow;
    memset(&tmnow, 0, sizeof(tmnow));
    if ( this->m_pMap )
    {
        tm v2;
        Map::getLocalNow(&v2, this->m_pMap);
        tmnow = v2;
    }
    return tmnow.tm_min;
}

void Monster::refreshSkill(int32_t HPPecent)
{
    // Use base HPPecent to select skill based on monster config
    if ( HPPecent > 0 && this->m_cfgmonster.skillId > 0 )
    {
        int32_t skillid = this->m_cfgmonster.skillId;
        if ( !Monster::isSkillCD(this, skillid) )
        {
            Unit::useSkill(this, nullptr, SKILL_TYPE::ST_ACTIVE_SKILL, skillid);
            Monster::setState(this, MonsterState::MS_ATTACK);
        }
    }
}

bool Monster::isSkillCD(SkillId_t nSkillId)
{
    for ( int32_t i = 0; i <= 9; ++i )
    {
        if ( this->m_vSkills[i].nId == nSkillId )
        {
            int64_t nCDTime = this->m_vSkills[i].nCDTime;
            if ( nCDTime > Unit::getTick(this) )
                return true;
        }
    }
    return false;
}

void Monster::setSkillCD(SkillId_t nSkillId, int32_t nCDTime)
{
    for ( int32_t i = 0; i <= 9; ++i )
    {
        if ( this->m_vSkills[i].nId == nSkillId )
        {
            this->m_vSkills[i].nCDTime = Unit::getTick(this) + nCDTime;
            return;
        }
    }
}

void Monster::flyHome()
{
    UnitHandle::clear(&this->m_target);
    std::map<long,UnitHandle>::clear(&this->m_targetMap);
    if ( this->m_currentTile.x == this->m_cfgmapmonster.x && this->m_currentTile.y == this->m_cfgmapmonster.y )
    {
        Monster::setState(this, MonsterState::MS_STAND);
    }
    else
    {
        Unit::setAtTile(this, this->m_pMap, this->m_cfgmapmonster.x, this->m_cfgmapmonster.y);
        Monster::setState(this, MonsterState::MS_STAND);
    }
}

void Monster::backHome()
{
    if ( !Unit::HasBuffState(this, CObjState::Index_T::OBS_BINGDONG)
        && !Unit::HasBuffState(this, CObjState::Index_T::OBS_YUNXUAN)
        && !Unit::HasBuffState(this, CObjState::Index_T::OBS_SLEEP)
        && !Unit::HasBuffState(this, CObjState::Index_T::OBS_PLYSIS)
        && !Unit::HasBuffState(this, CObjState::Index_T::OBS_FROZEN) )
    {
        UnitHandle::clear(&this->m_target);
        std::map<long,UnitHandle>::clear(&this->m_targetMap);
        Unit::moveToTile(this, this->m_pMap, this->m_cfgmapmonster.x, this->m_cfgmapmonster.y);
        Monster::setState(this, MonsterState::MS_WALK_HOME);
    }
}

void Monster::checkHPRevive()
{
    if ( this->m_bHPRevive && Unit::isAlive(this) && this->m_cfgmonster.hpPercent > 0 )
    {
        int64_t nCurTick = Unit::getTick(this);
        if ( nCurTick - this->m_nLastReviveHPTick > 1000 )
        {
            int64_t nHP = Unit::getHP(this);
            int64_t nMaxHP = Unit::getMaxHP(this);
            int64_t nNewHP = nHP + nMaxHP * this->m_cfgmonster.hpPercent / 10000;
            if ( nNewHP > nMaxHP )
                nNewHP = nMaxHP;
            Unit::setHP(this, nNewHP);
            this->m_nLastReviveHPTick = nCurTick;
        }
    }
}

void Monster::checkLifeTime()
{
    if ( this->m_nLifeTime >= 0 )
    {
        if ( Unit::getNow(this) >= this->m_nLifeTime && Unit::isAlive(this) )
            Unit::setHP(this, 0);
        if ( Unit::getNow(this) >= this->m_nLifeTime )
            Monster::setDelFlg(this);
    }
}

void Monster::resetSkill()
{
    this->m_pCfgSkill = 0;
    for ( int32_t i = 0; i <= 9; ++i )
        this->m_vSkills[i].nCDTime = 0;
}

void Monster::resetRandAttr()
{
    CfgData *v2 = Answer::Singleton<CfgData>::instance();
    const CfgMonsterRandTable *MonsterRandTable = CfgData::GetMonsterRandTable(v2, this->m_cfgmonster.rand_id);
    if ( MonsterRandTable )
    {
        for ( int32_t index = 0; index < 50; ++index )
        {
            int32_t v6 = MonsterRandTable->vMinAttr[index];
            int32_t v7 = MonsterRandTable->vMaxAttr[index];
            if ( v7 > v6 )
            {
                Answer::Random *v8 = Answer::Singleton<Answer::Random>::instance();
                int32_t val = Answer::Random::generate(v8, v6, v7);
                Unit::SetAttrValue(this, (CObjAttrs::Index_T)index, val);
            }
        }
    }
}

void Monster::updatePlayerDamage(Player *player, int32_t damage)
{
    if ( player )
    {
        CharId_t cid = Player::getCid(player);
        auto iter = std::map<long,PlayerScore>::find(&this->m_PlayerScore, &cid);
        if ( iter != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
        {
            iter->second.nDamage += damage;
            this->m_DamageFlag = 1;
        }
    }
}

void Monster::addPlayer(Player *player, int32_t damage)
{
    if ( player )
    {
        CExtCharTeam *CharTeam = CExtCharTeam::GetTeamData(player);
        CharId_t cid = Player::getCid(player);
        PlayerScore score;
        score.nDamage = damage;
        score.nCid = cid;
        score.nTeamId = CharTeam ? CExtCharTeam::GetTeamId(CharTeam) : 0;
        auto insertResult = std::map<long,PlayerScore>::insert(&this->m_PlayerScore, std::make_pair(cid, score));
        if ( insertResult.second )
        {
            std::multiset<PlayerScore *,PlayerScoreRankGreater,std::allocator<PlayerScore *>>::insert(
                &this->m_ScoreRank, &insertResult.first->second);
            if ( CharTeam )
            {
                int32_t nTeamId = CExtCharTeam::GetTeamId(CharTeam);
                auto teamIter = std::map<int,PlayerScore>::find(&this->m_TeamScore, &nTeamId);
                if ( teamIter == std::map<int,PlayerScore>::end(&this->m_TeamScore) )
                {
                    PlayerScore teamScore;
                    teamScore.nDamage = damage;
                    teamScore.nCid = cid;
                    teamScore.nTeamId = nTeamId;
                    std::map<int,PlayerScore>::insert(&this->m_TeamScore, std::make_pair(nTeamId, teamScore));
                }
                else
                {
                    teamIter->second.nDamage += damage;
                }
            }
        }
    }
}

void Monster::SendAllDamageRank()
{
    auto iter = std::map<long,PlayerScore>::begin(&this->m_PlayerScore);
    auto __x = std::map<long,PlayerScore>::end(&this->m_PlayerScore);
    CharIdList Ranklist;
    while ( std::_Rb_tree_iterator<std::pair<long const,PlayerScore>>::operator!=(&iter, &__x) )
    {
        CharId_t cid = iter->first;
        std::list<long>::push_back(&Ranklist, &cid);
        std::_Rb_tree_iterator<std::pair<long const,PlayerScore>>::operator++(&iter);
    }
    // Send rank list to all players
    GameService *v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(v2->m_pConn);
    if ( packet )
    {
        Answer::NetPacket::writeInt32(packet, (int32_t)Ranklist.size());
        for ( auto it = Ranklist.begin(); it != Ranklist.end(); ++it )
            Answer::NetPacket::writeInt64(packet, *it);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_ALL);
        Answer::NetPacket::setProc(packet, 0x4E33u);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Answer::TcpClient::sendPacket(v2->m_pConn, packet);
    }
}

void Monster::SendAllDamageRank(Player *player)
{
    Monster::sendScorePlayerInfo(this, player, 1);
    Answer::NetPacket *packet = Monster::sendAllRankInfo(this);
    if ( packet )
    {
        GameService *v4 = Answer::Singleton<GameService>::instance();
        Answer::TcpClient::sendPacket(v4->m_pConn, packet);
    }
}

void Monster::sendScorePlayerInfo(Player *player, int8_t status)
{
    GameService *v4 = Answer::Singleton<GameService>::instance();
    CharId_t cid = Player::getCid(player);
    auto v6 = std::map<long,PlayerScore>::find(&this->m_PlayerScore, &cid);
    if ( v6 != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
    {
        Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(v4->m_pConn);
        if ( packet )
        {
            Answer::NetPacket::writeInt32(packet, this->m_cfgmonster.id);
            Answer::NetPacket::writeInt64(packet, v6->second.nDamage);
            Answer::NetPacket::writeInt8(packet, status);
            Answer::NetPacket::setType(packet, Answer::PackType::PACK_USER);
            Answer::NetPacket::setProc(packet, 0x4E34u);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            Answer::TcpClient::sendPacket(v4->m_pConn, packet);
        }
    }
}

Answer::NetPacket *Monster::sendAllRankInfo()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = Answer::TcpClient::popNetpacket(v1->m_pConn);
    if ( packet )
    {
        uint32_t offset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::writeInt32(packet, 0);
        int32_t nCount = 0;
        auto iter = std::multiset<PlayerScore *,PlayerScoreRankGreater,std::allocator<PlayerScore *>>::begin(&this->m_ScoreRank);
        auto __x = std::multiset<PlayerScore *,PlayerScoreRankGreater,std::allocator<PlayerScore *>>::end(&this->m_ScoreRank);
        while ( iter != __x && nCount < 10 )
        {
            PlayerScore *pScore = *iter;
            Answer::NetPacket::writeInt64(packet, pScore->nCid);
            Answer::NetPacket::writeInt32(packet, pScore->nDamage);
            ++nCount;
            ++iter;
        }
        uint32_t oldOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setWOffset(packet, offset);
        Answer::NetPacket::writeInt32(packet, nCount);
        Answer::NetPacket::setWOffset(packet, oldOffset);
        Answer::NetPacket::setType(packet, Answer::PackType::PACK_USER);
        Answer::NetPacket::setProc(packet, 0x4E35u);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
    }
    return packet;
}

void Monster::refreshRank()
{
    auto iter = std::map<long,PlayerScore>::begin(&this->m_PlayerScore);
    auto eiter = std::map<long,PlayerScore>::end(&this->m_PlayerScore);
    while ( iter != eiter )
    {
        auto &score = iter->second;
        if ( score.nTeamId > 0 )
        {
            auto teamIter = std::map<int,PlayerScore>::find(&this->m_TeamScore, &score.nTeamId);
            if ( teamIter != std::map<int,PlayerScore>::end(&this->m_TeamScore) )
            {
                // Update team score
                teamIter->second.nDamage += score.nDamage;
            }
        }
        ++iter;
    }
}

void Monster::checkEnterBossRegion()
{
    Position pos = StaticObj::getCurrentTile(this);
    int32_t MoveRange = Monster::getMoveRange(this);
    Map *pMap = this->m_pMap;
    if ( pMap )
    {
        PlayerList inPlayer;
        Map::getAreaPlayers(pMap, &inPlayer, pos.x, pos.y, MoveRange);
        for ( auto iter = inPlayer.begin(); iter != inPlayer.end(); ++iter )
        {
            Player *player = *iter;
            if ( player && Unit::isAlive(player) )
            {
                CharId_t cid = Player::getCid(player);
                auto findIter = std::map<long,PlayerScore>::find(&this->m_PlayerScore, &cid);
                if ( findIter == std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
                {
                    Monster::addPlayer(this, player, 0);
                }
            }
        }
    }
}

void Monster::checkLeaveBossRegion()
{
    Position pos = StaticObj::getCurrentTile(this);
    int32_t MoveRange = Monster::getMoveRange(this);
    Map *pMap = this->m_pMap;
    if ( pMap )
    {
        PlayerList inPlayer;
        Map::getAreaPlayers(pMap, &inPlayer, pos.x, pos.y, MoveRange * 2);
        auto iter = std::map<long,PlayerScore>::begin(&this->m_PlayerScore);
        while ( iter != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
        {
            CharId_t cid = iter->first;
            bool found = false;
            for ( auto pIter = inPlayer.begin(); pIter != inPlayer.end(); ++pIter )
            {
                if ( *pIter && Player::getCid(*pIter) == cid )
                {
                    found = true;
                    break;
                }
            }
            if ( !found )
            {
                // Player left area, remove from score
                std::map<long,PlayerScore>::erase(&this->m_PlayerScore, iter++);
            }
            else
            {
                ++iter;
            }
        }
    }
}

void Monster::updataBossDamageInfo(Player *player)
{
    Monster::sendScorePlayerInfo(this, player, 1);
}

void Monster::UpdateRegionPlayer()
{
    auto iter = std::map<long,PlayerScore>::begin(&this->m_PlayerScore);
    while ( iter != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
    {
        CharId_t nCharId = iter->first;
        int32_t RunnerId = StaticObj::GetRunnerId(this);
        GameService *v3 = Answer::Singleton<GameService>::instance();
        Player *player = GameService::getPlayer(v3, nCharId, RunnerId, 1);
        if ( player )
        {
            CExtCharTeam *CharTeam = CExtCharTeam::GetTeamData(player);
            int32_t newTeamId = CharTeam ? CExtCharTeam::GetTeamId(CharTeam) : 0;
            if ( iter->second.nTeamId != newTeamId )
            {
                int32_t oldTeamId = iter->second.nTeamId;
                iter->second.nTeamId = newTeamId;
                if ( oldTeamId > 0 )
                {
                    auto v4 = std::map<int,PlayerScore>::find(&this->m_TeamScore, &oldTeamId);
                    if ( v4 != std::map<int,PlayerScore>::end(&this->m_TeamScore) )
                    {
                        v4->second.nDamage -= iter->second.nDamage;
                        if ( v4->second.nDamage <= 0 )
                            std::map<int,PlayerScore>::erase(&this->m_TeamScore, oldTeamId);
                    }
                }
                if ( newTeamId > 0 )
                {
                    auto v5 = std::map<int,PlayerScore>::find(&this->m_TeamScore, &newTeamId);
                    if ( v5 == std::map<int,PlayerScore>::end(&this->m_TeamScore) )
                    {
                        PlayerScore teamScore;
                        teamScore.nDamage = iter->second.nDamage;
                        teamScore.nCid = nCharId;
                        teamScore.nTeamId = newTeamId;
                        std::map<int,PlayerScore>::insert(&this->m_TeamScore, std::make_pair(newTeamId, teamScore));
                    }
                    else
                    {
                        v5->second.nDamage += iter->second.nDamage;
                    }
                }
            }
        }
        ++iter;
    }
}

bool Monster::checkTeamClear(int32_t nTeamId) const
{
    auto iter = std::map<long,PlayerScore>::begin(&this->m_PlayerScore);
    auto __x = std::map<long,PlayerScore>::end(&this->m_PlayerScore);
    while ( iter != __x )
    {
        if ( iter->second.nTeamId == nTeamId )
        {
            CharId_t nCharId = iter->first;
            int32_t RunnerId = StaticObj::GetRunnerId(this);
            GameService *v5 = Answer::Singleton<GameService>::instance();
            Player *player = GameService::getPlayer(v5, nCharId, RunnerId, 1);
            if ( player && Unit::isAlive(player) )
                return false;
        }
        ++iter;
    }
    return true;
}

void Monster::clearTeam(const Int32List *const tList)
{
    for ( auto it = tList->begin(); it != tList->end(); ++it )
    {
        int32_t nTeam = *it;
        auto iter = std::map<long,PlayerScore>::begin(&this->m_PlayerScore);
        while ( iter != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
        {
            if ( iter->second.nTeamId == nTeam )
            {
                std::map<long,PlayerScore>::erase(&this->m_PlayerScore, iter++);
            }
            else
            {
                ++iter;
            }
        }
        std::map<int,PlayerScore>::erase(&this->m_TeamScore, nTeam);
    }
}

void Monster::onPlayerDie(Player *player)
{
    if ( player )
    {
        CharId_t cid = Player::getCid(player);
        auto iter = std::map<long,PlayerScore>::find(&this->m_PlayerScore, &cid);
        if ( iter != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
        {
            int32_t nTeamId = iter->second.nTeamId;
            if ( nTeamId > 0 )
            {
                auto v6 = std::map<long,PlayerScore>::begin(&this->m_PlayerScore);
                bool bChange = false;
                while ( v6 != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
                {
                    if ( v6->second.nTeamId == nTeamId )
                    {
                        CharId_t nCharId = v6->first;
                        int32_t RunnerId = StaticObj::GetRunnerId(this);
                        GameService *v5 = Answer::Singleton<GameService>::instance();
                        Player *p = GameService::getPlayer(v5, nCharId, RunnerId, 1);
                        if ( p && Unit::isAlive(p) )
                        {
                            bChange = true;
                            break;
                        }
                    }
                    ++v6;
                }
                if ( !bChange )
                {
                    Int32List teamClear;
                    std::list<int>::push_back(&teamClear, &nTeamId);
                    Monster::clearTeam(this, &teamClear);
                }
            }
        }
    }
}

void Monster::onTeamUpdate(Player *player)
{
    if ( player )
    {
        CharId_t cid = Player::getCid(player);
        auto iter = std::map<long,PlayerScore>::find(&this->m_PlayerScore, &cid);
        if ( iter != std::map<long,PlayerScore>::end(&this->m_PlayerScore) )
        {
            CExtCharTeam *CharTeam = CExtCharTeam::GetTeamData(player);
            int32_t nNewTeamId = CharTeam ? CExtCharTeam::GetTeamId(CharTeam) : 0;
            int32_t nOldTeamId = iter->second.nTeamId;
            if ( nOldTeamId != nNewTeamId )
            {
                if ( nOldTeamId > 0 )
                {
                    auto oldIter = std::map<int,PlayerScore>::find(&this->m_TeamScore, &nOldTeamId);
                    if ( oldIter != std::map<int,PlayerScore>::end(&this->m_TeamScore) )
                    {
                        oldIter->second.nDamage -= iter->second.nDamage;
                    }
                }
                iter->second.nTeamId = nNewTeamId;
                if ( nNewTeamId > 0 )
                {
                    auto newIter = std::map<int,PlayerScore>::find(&this->m_TeamScore, &nNewTeamId);
                    if ( newIter == std::map<int,PlayerScore>::end(&this->m_TeamScore) )
                    {
                        PlayerScore teamScore = iter->second;
                        std::map<int,PlayerScore>::insert(&this->m_TeamScore, std::make_pair(nNewTeamId, teamScore));
                    }
                    else
                    {
                        newIter->second.nDamage += iter->second.nDamage;
                    }
                }
            }
        }
    }
}


