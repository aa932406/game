#include "common.h"
#include "Game/Player.h"
#include "Game/Map.h"
#include "Game/Dungeon.h"
#include "Game/GameService.h"
#include "Game/MapManager.h"
#include "Game/Trailer.h"
#include "Game/CObjPet.h"
#include "Game/Plant.h"
#include "Game/Trap.h"
#include "Game/StaticObj.h"
#include "Game/PlayerRobot.h"
#include "Character/ChrTask.h"
#include "Character/CExtSystemMgr.h"
#include "Character/CExtFightChecker.h"
#include "Character/CExtCharCarrier.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtEquip.h"
#include "Character/CExtCharSkill.h"
#include "Character/CExtOperateLimit.h"
#include "Character/CExtCurrency.h"
#include "Database/PlayerDBData.h"
#include "Config/CfgData.h"
#include "Config/CfgDungeon.h"
#include "Config/CfgActivity.h"
#include "Config/CfgMap.h"
#include "Config/CfgMonster.h"
#include "Config/CfgTask.h"
#include "Config/CfgChrShop.h"
#include "Config/CfgGameShop.h"
#include "Config/CfgLevelExp.h"
#include "Config/CfgActiveSkill.h"
#include "Config/CfgBeastShrineTable.h"
#include "Config/CfgCycleTowerTable.h"
#include "Config/CfgEnergy.h"
#include "Config/CfgEquipBlessTable.h"
#include "Activity/CActivityManager.h"
#include "Activity/CActivityMap.h"
#include "Activity/CFestivalDoubleEleven.h"
#include "Activity/CFestivalActivity.h"
#include "Activity/CKiaFuRecharge.h"
#include "Activity/CZongHeYunYingHD.h"
#include "Activity/CWorldBoss.h"
#include "Activity/CUniteServer.h"
#include "Activity/CFamilyWar.h"
#include "Other/CGMBackstate.h"
#include "Other/LuckDrop.h"
#include "Other/DamageSum.h"
#include "Other/MonsterDungeon.h"
#include "Other/MGLog.h"
#include "Other/logout_log.h"
#include "Other/proc_log.h"
#include "Other/online_user_24th_log.h"
#include "Other/level_up_log.h"
#include "Character/CExtCharDepotManager.h"

#include <cstring>
#include <algorithm>

// ============================================================
// 构造函数和析构函数
// ============================================================

Player::Player()
    : Unit(EntityType::ET_PLAYER)
    , m_cid(0)
    , m_level(1)
    , m_job(0)
    , m_camp(0)
    , m_pkMode(0)
    , m_actState(0)
    , m_exp(0)
    , m_money(0)
    , m_gold(0)
    , m_bindGold(0)
    , m_honor(0)
    , m_contribution(0)
    , m_lastSaveTick(0)
    , m_lastPoolTick(0)
    , m_levelStartTime(0)
    , m_nDieTick(0)
    , m_startGatherTick(0)
    , m_lastPickTick(0)
    , m_AcceleratorTime(0)
    , m_needSyncStatus(false)
    , m_needSyncAttr(false)
    , m_needRecalAttr(false)
    , m_isBackCity(false)
    , m_bDie(false)
    , m_inFight(false)
    , m_inPvP(false)
    , m_connid(0)
    , m_cgindex(0)
    , m_extSysMgr(nullptr)
    , m_task(nullptr)
    , m_pet(nullptr)
    , m_trailer(nullptr)
    , m_plantId(0)
    , m_plantState(0)
{
}

Player::~Player()
{
    if (m_task) {
        delete m_task;
        m_task = nullptr;
    }

    if (m_extSysMgr) {
        delete m_extSysMgr;
        m_extSysMgr = nullptr;
    }

    if (m_pet) {
        delete m_pet;
        m_pet = nullptr;
    }

    for (auto* packet : m_netPackets) {
        delete packet;
    }
    m_netPackets.clear();
}

// ============================================================
// 基础信息方法
// ============================================================

std::string Player::getName() const
{
    return m_name;
}

std::string Player::getFamilyName() const
{
    return m_familyName;
}

std::string Player::getIP() const
{
    return m_ip;
}

std::string Player::GetPassport() const
{
    return m_passport;
}

std::string Player::GetPlatform() const
{
    return m_platform;
}

std::string Player::GetFromWay() const
{
    return m_fromWay;
}

// ============================================================
// 状态方法
// ============================================================

void Player::SetInFight()
{
    int64_t tick = getTick();
    m_extFightChecker.SetInFight(tick);
}

void Player::SetInPvP()
{
    int64_t tick = getTick();
    m_extFightChecker.SetInPvP(tick);
}

void Player::SetLevel(int32_t Level)
{
    int32_t OldLevel = m_level;
    m_level = Level;
    LevelUped(OldLevel, Level);
}

void Player::checkDie()
{
    if (!m_bDie && !isAlive()) {
        m_bDie = true;
        m_nDieTick = getTick();
        int32_t deathCount = getRecord(1012) + 1;
        updateRecord(1012, deathCount);
    }
}

// ============================================================
// 虚函数重写
// ============================================================

bool Player::isAlive() const
{
    return !m_bDie;
}

int32_t Player::GetViewHP() const
{
    const CObjCarrier* pCarrier = m_extCharCarrier.GetCarrier();
    if (pCarrier) {
        return pCarrier->GetViewHP();
    }
    return Unit::GetViewHP();
}

int32_t Player::GetViewMaxHP() const
{
    const CObjCarrier* pCarrier = m_extCharCarrier.GetCarrier();
    if (pCarrier) {
        return pCarrier->GetViewMaxHP();
    }
    return Unit::GetViewMaxHP();
}

int32_t Player::GetMoveSpeed() const
{
    const CObjCarrier* pCarrier = m_extCharCarrier.GetCarrier();
    if (pCarrier) {
        return pCarrier->GetMoveSpeed();
    }
    return Unit::GetMoveSpeed();
}

int32_t Player::GetAttackSpeed() const
{
    const CObjCarrier* pCarrier = m_extCharCarrier.GetCarrier();
    if (pCarrier) {
        return pCarrier->GetAttackSpeed();
    }
    return Unit::GetAttackSpeed();
}

// ============================================================
// 刷新和更新
// ============================================================

bool Player::refresh()
{
    if (m_needRecalAttr) {
        recalcAttr(true, false);
    }

    checkSaveToDB();

    if (checkNetPackets()) {
        return true;
    }

    CheckTrailer();

    return true;
}

void Player::reset()
{
    Unit::reset();

    m_cid = 0;
    m_level = 1;
    m_job = 0;
    m_camp = 0;
    m_pkMode = 0;
    m_actState = 0;

    m_name.clear();
    m_familyName.clear();
    m_ip.clear();
    m_passport.clear();
    m_platform.clear();
    m_fromWay.clear();

    m_exp = 0;
    m_money = 0;
    m_gold = 0;
    m_bindGold = 0;
    m_honor = 0;
    m_contribution = 0;

    m_bDie = false;
    m_inFight = false;
    m_inPvP = false;
    m_needSyncStatus = false;
    m_needSyncAttr = false;
    m_needRecalAttr = false;

    CExtCharDepotManager::OnPlayerLogout(this);

    m_lBossRank.clear();
    m_packetHandlers.clear();

    for (auto* packet : m_netPackets) {
        delete packet;
    }
    m_netPackets.clear();
}

void Player::postDamage(int32_t damge, UnitHandle launcher, int32_t Mid)
{
    if (!m_pMap || !m_extSysMgr)
        return;

    int32_t RunnerId = launcher.getRunnerId();
    GameService* v7 = Answer::Singleton<GameService>::instance();
    int32_t v8 = RunnerId;
    GameService* v9 = Answer::Singleton<GameService>::instance();

    CharId_t Cid = getCid();
    if (m_task) {
        m_task->onDamage(damge);
    }
}

void Player::PkModeTiShi()
{
    bool v1 = m_pkMode_bk == 0;
    int32_t v2 = v1 ? 0 : m_pkMode_bk;
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int16_t cgindex = m_cgindex;
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt8(packet, v2);
        GameService::sendPacketTo(v8, connId, cgindex, packet);
    }
}

bool Player::IsActDropItemMap()
{
    if (!m_pMap)
        return false;
    if (m_pMap->GetType() != 4)
        return false;
    uint32_t nMapParam = m_pMap->GetMapParam();
    return nMapParam <= 0x11 && ((1LL << nMapParam) & 0x25020) != 0;
}

void Player::EnterPrisonByPlayer(Player *killer)
{
    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    CharId_t v5 = killer ? killer->getCid() : 0;
    uint32_t WOffset = GameService::getWOffset(v3);
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    std::string val;
    if (killer) {
        val = killer->getName();
    }

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeString(packet, &val);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }

    // Enter prison map
    int32_t mapid = 0;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    mapid = cfgData->getPrisonMapId();
    MapManager* mapMgr = Answer::Singleton<MapManager>::instance();
    Map* pPrisonMap = MapManager::getMap(mapMgr, mapid);
    if (pPrisonMap) {
        int32_t region = 0;
        CfgMapRegion* pCfgRegion = cfgData->getMapRegion(mapid, region);
        Position pos;
        if (pCfgRegion) {
            pos = pCfgRegion->getBirthPos();
        }
        switchMap(pPrisonMap, pos.x, pos.y, false);
    }
}

void Player::EnterPrisonByMonster(int32_t Mid)
{
    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    uint32_t WOffset = GameService::getWOffset(v3);
    int8_t v6 = connId;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    std::string val = "monster";
    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeString(packet, &val);
        GameService::sendPacketTo(v7, connId, m_cgindex, packet);
    }

    int32_t mapid = 0;
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    mapid = cfgData->getPrisonMapId();
    MapManager* mapMgr = Answer::Singleton<MapManager>::instance();
    Map* pPrisonMap = MapManager::getMap(mapMgr, mapid);
    if (pPrisonMap) {
        int32_t region = 0;
        CfgMapRegion* pCfgRegion = cfgData->getMapRegion(mapid, region);
        Position pos;
        if (pCfgRegion) {
            pos = pCfgRegion->getBirthPos();
        }
        switchMap(pPrisonMap, pos.x, pos.y, false);
    }
}

void Player::KillerPlayerGongGao(Player *killer)
{
    GameService* v2 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    int32_t MapId = m_pMap ? m_pMap->GetId() : 0;
    CharId_t v5 = killer ? killer->getCid() : 0;
    uint32_t WOffset = GameService::getWOffset(v2);
    GameService* v7 = Answer::Singleton<GameService>::instance();

    std::string val;
    std::string v9;
    if (killer) {
        val = killer->getName();
        v9 = getName();
    }

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, MapId);
        Answer::NetPacket::writeString(packet, &val);
        Answer::NetPacket::writeString(packet, &v9);
        GameService::sendPacketTo(v7, m_connid, m_cgindex, packet);
    }
}

void Player::SendFamilyMemberKilled(Player *Killer)
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    CharId_t v5 = Killer ? Killer->getCid() : 0;
    int32_t MapId = m_pMap ? m_pMap->GetId() : 0;
    int16_t PosX = m_pos.px;
    int16_t PosY = m_pos.py;
    uint32_t WOffset = GameService::getWOffset(v2);

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, Cid);
        Answer::NetPacket::writeInt64(packet, v5);
        Answer::NetPacket::writeInt32(packet, MapId);
        Answer::NetPacket::writeInt16(packet, PosX);
        Answer::NetPacket::writeInt16(packet, PosY);
        GameService::sendPacketTo(v2, connId, m_cgindex, packet);
    }
}

bool Player::isFriendSide(Unit *pUnit)
{
    if (!pUnit)
        return false;

    if (pUnit->getEntityType() == EntityType::ET_PET) {
        CObjPet* pPet = static_cast<CObjPet*>(pUnit);
        if (pPet->getOwner() == getCid())
            return true;
    }

    if (pUnit->getEntityType() == EntityType::ET_PLAYER) {
        Player* pPlayer = static_cast<Player*>(pUnit);
        if (IsInTeam() && getTeamId() == pPlayer->getTeamId())
            return true;
        FamilyId_t FamilyId = getFamilyId();
        if (FamilyId && FamilyId == pPlayer->getFamilyId())
            return true;
    }

    return false;
}

bool Player::checkSkillTarget(const CfgActiveSkill *pCfg, Unit *pUnit)
{
    if (!pCfg || !pUnit)
        return false;

    if (isFriendSide(pUnit))
        return false;

    if (pUnit->getEntityType() == EntityType::ET_PLAYER) {
        Player* v6 = static_cast<Player*>(pUnit);
        if (v6->m_inPvP)
            return true;
    }

    if (pUnit->getEntityType() == EntityType::ET_PET) {
        CObjPet* v8 = static_cast<CObjPet*>(pUnit);
        if (v8->getOwner() == getCid())
            return false;
    }

    int32_t RunnerId = pUnit->getRunnerId();
    GameService* v10 = Answer::Singleton<GameService>::instance();

    if (!m_pMap)
        return false;

    Position pos = getPos();
    Position CurrentTile = getTilePos();
    Position v15 = pUnit->getPos();
    Position v16 = pUnit->getTilePos();

    return m_pMap->isInRange(pos, v15, pCfg->range);
}

void Player::SaveDBData(PlayerDBData *const dbData)
{
    int32_t pk_time = m_chr.pk_time;
    int32_t v4 = getNow();
    int32_t level_stay_time = m_chr.level_stay_time;

    if (m_levelStartTime > 0) {
        level_stay_time += (v4 - m_levelStartTime);
    }

    MemChrBuffVector __x;
    m_buffMgr.saveToVector(&__x);

    int32_t mapid = 0;
    int32_t x = 0;
    int32_t y = 0;
    if (m_pMap) {
        mapid = m_pMap->GetId();
        x = m_pos.px;
        y = m_pos.py;
    }

    dbData->setBaseInfo(m_cid, m_name, m_level, m_job, m_camp);
    dbData->setExp(m_chr.exp);
    dbData->setMoney(m_chr.money);
    dbData->setMapInfo(mapid, x, y);
    dbData->setPkInfo(m_chr.pk_mode, pk_time, m_chr.pk_value);
    dbData->setLevelStayTime(level_stay_time);
    dbData->setBuffData(__x);

    CExtCharDepotManager::OnSaveToDB(this, dbData);
}

void Player::saveToDB(int32_t reason, int32_t param, int32_t logout_time)
{
    int8_t connId = m_connid;
    DBService* v5 = Answer::Singleton<DBService>::instance();
    PlayerDBData dbData;
    PlayerDBData::PlayerDBData(&dbData);
    SaveDBData(&dbData);

    if (logout_time) {
        dbData.setLogoutTime(logout_time);
    }

    DBService::savePlayerData(v5, &dbData, reason);
}

void Player::initNetPacketHandlers()
{
    setNetPacketHandler(8, &Player::onSyncTime);
    setNetPacketHandler(9, &Player::onLogout);
    setNetPacketHandler(10, &Player::onMove);
    setNetPacketHandler(11, &Player::onJump);
    setNetPacketHandler(12, &Player::onHit);
    setNetPacketHandler(16, &Player::onSwitchMap);
    setNetPacketHandler(18, &Player::onSwitchPkMode);
    setNetPacketHandler(21, &Player::onSafeRevive);
}

void Player::init(PlayerDBData *const dbData)
{
    int32_t pk_time = dbData->getPkTime();
    int64_t Tick = getTick();
    CExtOperateLimit* OperateLimit = getOperateLimit();
    int32_t LimitCount = OperateLimit->GetLimitCount(2107);
    CfgData* v7 = Answer::Singleton<CfgData>::instance();
    const EnergyCfg* EnergyCfg = v7->getEnergyCfg();
    int32_t Now = getNow();

    m_chr = dbData->getCharacterData();
    m_cid = m_chr.cid;
    m_level = m_chr.level;
    m_job = m_chr.job;
    m_camp = m_chr.camp;
    m_pkMode = m_chr.pk_mode;
    m_exp = m_chr.exp;
    m_money = m_chr.money;

    if (m_extSysMgr) {
        m_extSysMgr->init(dbData);
    }

    if (m_task) {
        m_task->init(dbData);
    }

    CExtCharDepotManager::OnPlayerLogin(this);
    CExtCharDepotManager::OnLoadFromDB(this, dbData);

    m_lastSaveTick = Tick;
}

void Player::LoginGongGao()
{
    int8_t ConnId = m_connid;
    CActivityManager* v2 = Answer::Singleton<CActivityManager>::instance();
    int8_t v4 = ConnId;
    GameService* v5 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    uint32_t WOffset = GameService::getWOffset(v5);
    int8_t v8 = ConnId;
    GameService* v9 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, Cid);
        GameService::sendPacketTo(v9, ConnId, m_cgindex, packet);
    }
}

void Player::initBuff(const MemChrBuffVector *const buffVt)
{
    if (!buffVt)
        return;

    CfgData* cfgData = Answer::Singleton<CfgData>::instance();

    for (size_t i = 0; i < buffVt->size(); ++i) {
        const MemChrBuff& buffData = (*buffVt)[i];
        int32_t buffId = buffData.buffId;
        const CfgBuff* pCfgBuff = cfgData->getBuff(buffId);
        if (!pCfgBuff)
            continue;

        ItemBuff* buff = new ItemBuff();
        buff->init(buffData);
        m_buffMgr.addBuff(buff);
    }
    m_buffMgr.activateAll();
}

bool Player::appendInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return false;

    int8_t Type = static_cast<int8_t>(getEntityType());
    CharId_t Cid = getCid();
    FamilyId_t FamilyId = getFamilyId();
    int8_t FamilyPosition = getFamilyPosition();
    Sex_t Sex = getSex();
    Job_t Job = m_job;
    int16_t v9 = static_cast<int16_t>(getLevel());
    int8_t IsBuleName = m_extFightChecker.IsBuleName() ? 1 : 0;

    Answer::NetPacket::writeInt8(packet, Type);
    Answer::NetPacket::writeInt64(packet, Cid);
    Answer::NetPacket::writeInt64(packet, FamilyId);
    Answer::NetPacket::writeInt8(packet, FamilyPosition);
    Answer::NetPacket::writeInt8(packet, Sex);
    Answer::NetPacket::writeInt8(packet, Job);
    Answer::NetPacket::writeInt16(packet, v9);
    Answer::NetPacket::writeString(packet, &m_name);
    Answer::NetPacket::writeString(packet, &m_familyName);
    Answer::NetPacket::writeInt8(packet, IsBuleName);

    return true;
}

int32_t Player::switchMap(Map *pMap, int32_t x, int32_t y, bool isFly)
{
    if (!pMap)
        return 10002;

    int32_t v5 = pMap->GetType();
    int64_t MapId = pMap->GetId();
    int16_t cgindex = m_cgindex;
    int8_t connId = m_connid;
    GameService* v9 = Answer::Singleton<GameService>::instance();
    int8_t ConnId = connId;
    DBService* v11 = Answer::Singleton<DBService>::instance();
    int32_t v12 = 0;

    if (m_pMap) {
        m_pMap->removeUnit(this);
    }

    m_pMap = pMap;
    m_pos.px = x;
    m_pos.py = y;
    pMap->addUnit(this, x, y);

    setNeedSyncSelf();
    SetNeedSyncAround();

    return 0;
}

void Player::EnterMapGongGao(int32_t GongGaoId, int32_t MapId)
{
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    uint32_t WOffset = GameService::getWOffset(v4);
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    std::string val;
    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, GongGaoId);
        Answer::NetPacket::writeInt32(packet, MapId);
        Answer::NetPacket::writeString(packet, &val);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

int32_t Player::OnCheckAccelerator(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int16_t cgindex = m_cgindex;
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();
    int32_t Tick = getTick();
    int64_t CurTick = getTick();

    int32_t nTime = Answer::NetPacket::readInt32(inPacket);
    m_AcceleratorTime = nTime;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, nTime);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }

    return 0;
}

void Player::moveToReviveRegion(bool bInAct)
{
    CfgData* v2 = Answer::Singleton<CfgData>::instance();
    int32_t mapid = v2->getReviveMapId();
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    int32_t v5 = mapid;
    MapManager* v6 = Answer::Singleton<MapManager>::instance();

    Position pos;
    int32_t region = 0;
    CfgMapRegion* pCfgMapRegion = v2->getMapRegion(mapid, region);
    if (pCfgMapRegion) {
        pos = pCfgMapRegion->getBirthPos();
    }

    Map* pMap = MapManager::getMap(v6, mapid);
    if (pMap) {
        switchMap(pMap, pos.x, pos.y, false);
    }
}

int32_t Player::getActivityBirthRegion(Int32Vector *pRegsions)
{
    if (!pRegsions)
        return 0;

    size_t lenth = pRegsions->size();
    if (lenth == 1)
        return (*pRegsions)[0];

    if (lenth <= 0)
        return 0;

    Answer::Random* v3 = Answer::Singleton<Answer::Random>::instance();
    int32_t idx = v3->randInt(0, static_cast<int32_t>(lenth) - 1);
    return (*pRegsions)[idx];
}

int32_t Player::leaveDungeon()
{
    Dungeon* pDungeon = nullptr;
    if (m_pMap && m_pMap->GetType() == 3) {
        pDungeon = static_cast<Dungeon*>(m_pMap);
    }

    if (!pDungeon)
        return 10002;

    Position oldPosition;
    int32_t BackMapId = pDungeon->GetBackMapId();
    MapManager* v3 = Answer::Singleton<MapManager>::instance();
    Map* pTargetMap = MapManager::getMap(v3, BackMapId);
    if (!pTargetMap)
        return 10002;

    int32_t v4 = switchMap(pTargetMap, m_oldPosition.px, m_oldPosition.py, false);
    return v4;
}

int32_t Player::leaveActivity()
{
    int32_t mapid = 0;
    MapManager* v3 = Answer::Singleton<MapManager>::instance();
    GameService* v4 = Answer::Singleton<GameService>::instance();
    CfgData* v6 = Answer::Singleton<CfgData>::instance();
    int32_t v7 = mapid;
    MapManager* v8 = Answer::Singleton<MapManager>::instance();
    int32_t RunnerId = 0;
    int32_t iparam2 = 0;

    if (m_oldPosition.runnerId > 0) {
        mapid = m_oldPosition.mapId;
    } else {
        CfgData* cfgData = Answer::Singleton<CfgData>::instance();
        mapid = cfgData->getBirthMapId();
    }

    Map* pTargetMap = MapManager::getMap(v8, mapid);
    if (!pTargetMap)
        return 10002;

    return switchMap(pTargetMap, m_oldPosition.px, m_oldPosition.py, false);
}

bool Player::isInRectangle(Position x, Position y) const
{
    return Unit::isInRectangle(x, y);
}

int32_t Player::getCreatedDays()
{
    return Answer::DayTime::daydiff(m_chr.create_time) + 1;
}

void Player::SetCamp(int8_t nCamp)
{
    m_camp = nCamp;
    setSyncStatusFlag();
}

void Player::getOtherQueryInfo()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    int32_t Sid = 0;
    int32_t v6 = getLevel();
    Job_t Job = m_job;
    FamilyId_t FamilyId = getFamilyId();
    int32_t v9 = getBattle();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0xFFu, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, Cid);
        Answer::NetPacket::writeString(packet, &m_name);
        Answer::NetPacket::writeInt8(packet, Job);
        Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(v6));
        Answer::NetPacket::writeInt64(packet, FamilyId);
        Answer::NetPacket::writeString(packet, &m_familyName);
        Answer::NetPacket::writeInt32(packet, v9);
        GameService::sendPacketTo(v2, connId, m_cgindex, packet);
    }
}

void Player::SendOtherPlayerInfo(int8_t connid, int16_t cgindex)
{
    GameService* v3 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* playerInfo = Answer::NetPacket::allocPacket(0, 0xFFu, 0, 0);
    if (playerInfo) {
        Answer::NetPacket::writeInt64(playerInfo, getCid());
        Answer::NetPacket::writeString(playerInfo, &m_name);
        Answer::NetPacket::writeInt8(playerInfo, m_job);
        Answer::NetPacket::writeInt16(playerInfo, static_cast<int16_t>(getLevel()));
        Answer::NetPacket::writeInt64(playerInfo, getFamilyId());
        Answer::NetPacket::writeString(playerInfo, &m_familyName);
        Answer::NetPacket::writeInt32(playerInfo, getBattle());
        GameService::sendPacketTo(v3, connid, cgindex, playerInfo);
    }
}

void Player::delExp(int64_t nValue)
{
    m_chr.exp -= nValue;
    if (m_chr.exp < 0)
        m_chr.exp = 0;
    setNeedSyncSelf();
}

void Player::sendExpGainInfo(int64_t value, int64_t BaseExp, BenefitType benefitType)
{
    int8_t connId = m_connid;
    GameService* v5 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v5);
    int16_t cgindex = m_cgindex;
    int8_t v8 = connId;
    GameService* v9 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, value);
        Answer::NetPacket::writeInt32(packet, static_cast<int32_t>(benefitType));
        GameService::sendPacketTo(v9, connId, m_cgindex, packet);
    }
}

void Player::addExp(int64_t addon, int64_t BaseExp, bool Iswallow)
{
    if (addon <= 0)
        return;

    int64_t addona = addon;
    if (Iswallow) {
        double ratio = benefitRatio();
        addona = static_cast<int64_t>(static_cast<double>(addon) * ratio);
    }

    m_chr.exp += addona;
    autoUpgradeLevel();
    sendExpGainInfo(addona, BaseExp, benefitType());
}

int64_t Player::GetLevelExp()
{
    int32_t level = getLevel();
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    return cfgData->getNeedLevelExp(level);
}

int64_t Player::GetLevelMaxExp()
{
    int32_t level = getLevel();
    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    return cfgData->getMaxExp(level);
}

void Player::UpdateGmGold()
{
    GameService* v1 = Answer::Singleton<GameService>::instance();
    Answer::DBPool* v2 = Answer::Singleton<Answer::DBPool>::instance();
    int32_t sid = GameService::getSid(v1);
    int64_t Uid = getUid();

    char szSQL[4096];
    snprintf(szSQL, sizeof(szSQL), "UPDATE player SET gold=%d WHERE uid=%lld",
             m_chr.gold, Uid);
    Answer::MySqlQuery* v5 = new Answer::MySqlQuery();
    Answer::MySqlDBGuard db(v2);
    v5->execute(szSQL);
    delete v5;
}

void Player::syncGold(int32_t nGold)
{
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::DBPool* v3 = Answer::Singleton<Answer::DBPool>::instance();
    Answer::MySqlQuery* v4 = new Answer::MySqlQuery();

    char szSQL[4096];
    snprintf(szSQL, sizeof(szSQL), "UPDATE player SET gold=%d WHERE uid=%lld",
             nGold, getUid());

    Answer::MySqlDBGuard db(v3);
    v4->execute(szSQL);

    int32_t gold_pay = 0;
    Answer::MySqlQuery result;
    snprintf(szSQL, sizeof(szSQL), "SELECT gold_pay FROM player WHERE uid=%lld", getUid());
    result.execute(szSQL);
    if (result.next()) {
        gold_pay = result.getInt(0);
    }

    delete v4;
    m_chr.gold = nGold;
    setNeedSyncSelf();
}

bool Player::UseRechargeCard(int32_t nValue, bool bReal)
{
    GameService* v3 = Answer::Singleton<GameService>::instance();
    bool v4 = false;
    Answer::DBPool* v5 = Answer::Singleton<Answer::DBPool>::instance();
    int32_t Sid = GameService::getSid(v3);
    int64_t Uid = getUid();

    char szSQL[4096];
    Answer::MySqlDBGuard db(v5);
    Answer::MySqlQuery result;

    snprintf(szSQL, sizeof(szSQL), "SELECT gold FROM player_recharge WHERE uid=%lld AND status=0", Uid);
    result.execute(szSQL);
    if (result.next()) {
        int32_t gold = result.getInt(0);
        if (gold >= nValue) {
            v4 = true;
            if (bReal) {
                snprintf(szSQL, sizeof(szSQL),
                    "UPDATE player_recharge SET status=1 WHERE uid=%lld AND gold>=%d", Uid, nValue);
                result.execute(szSQL);
            }
        }
    }

    return v4;
}

void Player::SendRechargeNotice(int32_t nGold)
{
    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v3);
    int16_t cgindex = m_cgindex;
    int8_t v6 = connId;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, nGold);
        GameService::sendPacketTo(v7, connId, m_cgindex, packet);
    }
}

void Player::PayedDispose(const int32_t AddGold)
{
    int32_t TodayPayGold = getRecord(1101);
    int32_t v3 = TodayPayGold + AddGold;
    int32_t v4 = v3;
    updateRecord(1101, v3);

    CKiaFuRecharge* v5 = Answer::Singleton<CKiaFuRecharge>::instance();
    int32_t v6 = v4;
    CFestivalDoubleEleven* v7 = Answer::Singleton<CFestivalDoubleEleven>::instance();
    int32_t v8 = AddGold;
    int32_t v9 = AddGold;

    m_chr.gold += AddGold;
    setNeedSyncSelf();
    SendRechargeNotice(AddGold);
}

void Player::SendRechargeGold(const int32_t Gold)
{
    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    int32_t GateIndex = GameService::getGateIndex(v3);
    uint32_t WOffset = GameService::getWOffset(v3);
    int8_t v6 = connId;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, Gold);
        GameService::sendPacketTo(v7, connId, m_cgindex, packet);
    }
}

void Player::syncTodayGoldPay()
{
    m_PlayerYunYingHD.SendEveryDayChongZhiInfo(3);
    m_PlayerYunYingHD.SendEveryDayChongZhiInfo(2);
}

int32_t Player::getMainWeaponId()
{
    return m_extEquip.GetEquipSlot(0)->itemId;
}

int32_t Player::getClothesId()
{
    return m_extEquip.GetEquipSlot(2)->itemId;
}

void Player::setTrailer(Trailer *pTrailer)
{
    m_trailer = pTrailer;
    if (pTrailer) {
        int32_t TaskId = Trailer::GetTaskId(pTrailer);
        if (m_task) {
            m_task->setTaskCanSubmit(TaskId, 0);
        }
    }
}

void Player::CheckTrailer()
{
    if (!m_trailer)
        return;

    if (!m_pMap)
        return;

    int32_t MapId = m_pMap->GetId();
    int32_t PosY = m_pos.py;
    int32_t PosX = m_pos.px;
    Position pos = { PosX, PosY };
    Position CurrentPixel = getPixelPos();

    m_trailer->CheckMove(this, CurrentPixel);
}

double Player::benefitRatio()
{
    if (m_sysUser.adult)
        return 1.0;

    int32_t total_online_time = m_sysUser.total_online_time;
    int32_t onlineTime = total_online_time + getNow() - m_sysUser.last_login_time;

    if (onlineTime <= 18000)
        return 1.0;
    if (onlineTime <= 21600)
        return 0.5;

    return 0.0;
}

BenefitType Player::benefitType()
{
    if (m_sysUser.adult)
        return BenefitType::BT_NORMAL;

    int32_t total_online_time = m_sysUser.total_online_time;
    int32_t onlineTime = total_online_time + getNow() - m_sysUser.last_login_time;

    if (onlineTime <= 18000)
        return BenefitType::BT_NORMAL;
    if (onlineTime <= 21600)
        return BenefitType::BT_HALF;

    return BenefitType::BT_ZERO;
}

void Player::getBagSlotData(MemChrBag *__return_ptr retstr, int32_t slot)
{
    const MemChrBag* slotData = m_extCharBag.GetSlotData(slot);
    if (slotData) {
        *retstr = *slotData;
    }
}

bool Player::hasItem(int32_t baseid, int32_t type, int32_t count)
{
    return m_extCharBag.HasItem(baseid, type, count);
}

void Player::CalBattle()
{
    CfgData* v1 = Answer::Singleton<CfgData>::instance();
    int32_t OldBattle = m_Battle;
    int64_t nAttrBattle = m_extEquip.CalcBattle();
    int64_t nAttrBattlea = nAttrBattle;

    m_Battle = static_cast<int32_t>(nAttrBattle);

    if (OldBattle != m_Battle) {
        setNeedSyncSelf();
    }
}

void Player::setPkMode(int32_t mode, bool backUp)
{
    m_chr.pk_mode = mode;
    if (backUp)
        m_pkMode_bk = m_chr.pk_mode;
    if (mode != 1)
        SetPkProtectTime(0);
    setNeedSyncSelf();
    SetNeedSyncAround();
}

void Player::resetPkModeOnEnterMap(const Map *pMap)
{
    if (!pMap)
        return;

    if (pMap->GetType() == 1 || pMap->GetType() == 2) {
        m_chr.pk_mode = 1;
        setNeedSyncSelf();
    }
}

void Player::onDropItem(Player *pKiller, int32_t Mid)
{
    if (!m_pMap)
        return;

    int32_t PkValue = getPkValue();
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    CVip* PlayerVip = getVip();

    MemChrBag stu;
    int64_t srcId = 0;
    Position CentosPos = getPos();
    CDropItemGroup pDropItemGroup;

    // Drop items from bag based on pk value
    if (PkValue > 0) {
        std::vector<MemChrBag> items = m_extCharBag.GetDropItems(PkValue);
        for (auto& item : items) {
            m_pMap->addDropItem(item, CentosPos);
        }
    }
}

void Player::updatePkValue(Player *pKiller, Map *pMap)
{
    if (!pKiller || !pMap)
        return;

    bool v3 = pKiller->IsInTeam();
    bool v4 = false;
    GameService* v5 = Answer::Singleton<GameService>::instance();
    CExtEquip* Equip = pKiller->getExtEquip();
    CfgData* v8 = Answer::Singleton<CfgData>::instance();
    const CfgEquipBlessTable* EquipBlessTable = v8->getEquipBlessTable();
    FamilyId_t v10 = getFamilyId();
    FamilyId_t FamilyId = pKiller->getFamilyId();

    if (v10 != FamilyId) {
        int32_t pkValue = getPkValue();
        if (pkValue < 99) {
            addPkValue(1);
        }
    }
}

void Player::addPkValue(int32_t addon)
{
    if (addon > 0 && m_chr.pk_value <= 0)
        m_lastPkValueTick = getTick();
    m_chr.pk_value += addon;
    if (m_chr.pk_value < 0)
        m_chr.pk_value = 0;
    if (m_chr.pk_value > 99)
        m_extFightChecker.ChangeBuleName(0);
    SetNeedSyncAround();
}

bool Player::SubPkValues(int32_t Addon)
{
    if (getPkValue() <= 0)
        return false;
    if (Addon <= 0)
        return false;
    addPkValue(-Addon);
    return true;
}

int32_t Player::getWeaponId()
{
    return m_extEquip.GetEquipSlot(0)->itemId;
}

void Player::SetPkProtectTime(int32_t Time)
{
    m_chr.pk_time = Time;
    SetNeedSyncAround();
    setNeedSyncSelf();
}

int32_t Player::OnLevelPrison(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t mapid = v3->getPrisonMapId();
    MapManager* v5 = Answer::Singleton<MapManager>::instance();
    Position pos;
    CfgMapRegion* pCfgRegion = v3->getMapRegion(mapid, 0);
    Map* pTargetMap = MapManager::getMap(v5, mapid);

    if (pCfgRegion) {
        pos = pCfgRegion->getBirthPos();
    }

    if (pTargetMap) {
        return switchMap(pTargetMap, pos.x, pos.y, false);
    }

    return 10002;
}

int32_t Player::OnSubPkValus(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int8_t ClearType = Answer::NetPacket::readInt8(inPacket);
    int8_t CostType = Answer::NetPacket::readInt8(inPacket);
    int32_t BuyCount = Answer::NetPacket::readInt32(inPacket);
    int CurPkValue = getPkValue();
    CfgGameShop* pCfgGameShop = nullptr;
    int32_t nShopId = 0;
    int32_t nCostGold = 0;

    if (ClearType == 1) {
        nShopId = 9001;
    } else {
        nShopId = 9002;
    }

    pCfgGameShop = v3->getGameShop(nShopId);
    if (!pCfgGameShop)
        return 10002;

    nCostGold = pCfgGameShop->getPrice() * BuyCount;
    if (m_chr.gold < nCostGold)
        return 10002;

    m_chr.gold -= nCostGold;
    if (ClearType == 1) {
        m_chr.pk_value = 0;
    } else {
        addPkValue(-10 * BuyCount);
    }

    setNeedSyncSelf();
    return 0;
}

void Player::setTaskCanSubmit(int32_t tid)
{
    if (m_task) {
        m_task->setTaskCanSubmit(tid, 0);
    }
}

void Player::checkTaskCanSubmit(int32_t dungeonID, int8_t dungeonType)
{
    if (m_task) {
        m_task->checkTaskCanSubmit(dungeonID, dungeonType);
    }
}

void Player::updateRecord(int32_t id, int32_t param)
{
    m_extOperateLimit.UpdateLimitCount(id, param);
}

int32_t Player::getRecord(int32_t id)
{
    return m_extOperateLimit.GetLimitCount(id);
}

void Player::clearRecordRange(int32_t start, int32_t end, int32_t diffDay)
{
    m_extOperateLimit.ResetRange(start, end, diffDay);
}

void Player::checkFestivalVersion()
{
    CFestivalDoubleEleven* v1 = Answer::Singleton<CFestivalDoubleEleven>::instance();
    CFestivalActivity* v2 = Answer::Singleton<CFestivalActivity>::instance();
    int32_t nVersion = CFestivalDoubleEleven::GetVersion(v1);
    int32_t nVersion_0 = CFestivalActivity::GetVersion(v2);

    int32_t nCurVersion = getRecord(1599);
    if (nCurVersion != nVersion) {
        ResetFestivalData(nVersion);
    }
}

void Player::ResetFestivalData(int32_t nVersion)
{
    m_extOperateLimit.UpdateLimitCount(1599, nVersion);
    m_extOperateLimit.ResetRange(1600, 1699, 0);
}

int32_t Player::getAction()
{
    if (!m_pMap)
        return 0;

    int64_t PlantId = GetPlantId();
    Plant* plant = m_pMap->getPlant(PlantId);
    if (plant) {
        return plant->getAction();
    }

    return 0;
}

PlayerTeamStatus Player::GetTeamStatus()
{
    if (!IsInTeam())
        return PlayerTeamStatus::PTS_FREE;
    if (IsTeamLeader())
        return PlayerTeamStatus::PTS_TEAM_LEADER;
    return PlayerTeamStatus::PTS_TEAM_MEMBER;
}

void Player::onNewDayCome()
{
    int32_t Now = getNow();
    GameService* v4 = Answer::Singleton<GameService>::instance();
    DBService* v5 = Answer::Singleton<DBService>::instance();

    online_user_24th_log stu;
    stu.cid = getCid();
    stu.time = Now;
    MGLog::log(stu);

    if (m_task) {
        m_task->onNewDay();
    }

    CExtCharDepotManager::OnDaySwitch(this);

    m_extOperateLimit.ResetDaily();
}

void Player::kickBackFromCross(int32_t Reason)
{
    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    int64_t Uid = getUid();
    int32_t Sid = GameService::getSid(v3);
    uint32_t WOffset = GameService::getWOffset(v3);
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, Reason);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::refeshKillerRecord()
{
    m_extOperateLimit.Reset(1010);
}

void Player::refreshDailyCheck(bool first)
{
    int32_t v2 = getNow();
    GameService* v3 = Answer::Singleton<GameService>::instance();
    CUniteServer* v6 = Answer::Singleton<CUniteServer>::instance();
    CFestivalDoubleEleven* v7 = Answer::Singleton<CFestivalDoubleEleven>::instance();
    CFestivalActivity* v8 = Answer::Singleton<CFestivalActivity>::instance();
    CZongHeYunYingHD* v9 = Answer::Singleton<CZongHeYunYingHD>::instance();
    CKiaFuRecharge* v10 = Answer::Singleton<CKiaFuRecharge>::instance();
    CKiaFuRecharge* v11 = Answer::Singleton<CKiaFuRecharge>::instance();

    checkFestivalVersion();
    m_extOperateLimit.ResetDaily();
    saveToDB(3, 0, 0);
}

void Player::saveOnlineTime()
{
    int32_t v1 = getNow();
    int32_t v2 = m_sysUser.total_online_time;
    CharId_t Cid = getCid();
    int8_t ConnId = m_connid;
    DBService* v5 = Answer::Singleton<DBService>::instance();
    int32_t nNowTime = getNow();

    m_sysUser.total_online_time += (nNowTime - m_sysUser.last_login_time);
    m_sysUser.last_login_time = nNowTime;

    char szSQL[4096];
    snprintf(szSQL, sizeof(szSQL),
        "UPDATE sys_user SET total_online_time=%d,last_login_time=%d WHERE uid=%lld",
        m_sysUser.total_online_time, m_sysUser.last_login_time, getUid());
    DBService::execute(v5, szSQL);
}

void Player::addNetPacket(Answer::NetPacket *inPacket, uint32_t rsize)
{
    if (!inPacket)
        return;

    uint32_t Size = rsize;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    Answer::PackType Type = Answer::NetPacket::getType(inPacket);
    int8_t connId = m_connid;
    GameService* v8 = Answer::Singleton<GameService>::instance();
    uint32_t v9 = Size;
    const char* v10 = Answer::NetPacket::getData(inPacket);
    uint32_t v11 = Size;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeUint16(packet, Proc);
        Answer::NetPacket::writeData(packet, v10, Size);

        if (GameService::getLine(v8) != 9) {
            m_netPackets.push_back(packet);
        } else {
            delete packet;
        }
    }
}

void Player::broadcastRevive()
{
    if (!m_pMap)
        return;

    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    uint32_t WOffset = GameService::getWOffset(v2);

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, Cid);
        m_pMap->broadcastPacket(packet);
    }
}

void Player::setOldPosition()
{
    if (!m_pMap)
        return;

    bool v1 = StaticObj::InDungeon(this) || StaticObj::InActivity(this);
    if (!v1 && m_pMap->CanStayInMap()) {
        m_oldPosition.runnerId = getRunnerId();
        m_oldPosition.mapId = m_pMap->GetId();
        m_oldPosition.px = m_pos.px;
        m_oldPosition.py = m_pos.py;
    }
}

void Player::getOldPosition(PlayerPosition *__return_ptr retstr)
{
    *retstr = m_oldPosition;
}

void Player::addLogoutPacket(int32_t reason, int32_t param)
{
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int64_t Now = getNow();
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, reason);
        Answer::NetPacket::writeInt32(packet, param);
        Answer::NetPacket::writeInt64(packet, Now);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::sendBasicInfo(int32_t reason)
{
    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v3);
    int16_t cgindex = m_cgindex;
    int8_t v6 = connId;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, getCid());
        Answer::NetPacket::writeString(packet, &m_name);
        Answer::NetPacket::writeInt8(packet, m_job);
        Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(getLevel()));
        Answer::NetPacket::writeInt64(packet, m_chr.exp);
        Answer::NetPacket::writeInt32(packet, m_Battle);
        GameService::sendPacketTo(v7, connId, m_cgindex, packet);
    }
}

void Player::sendItemEffect(const std::string *const effect)
{
    if (!effect || !m_pMap)
        return;

    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    int32_t x = m_pos.px;
    int32_t y = m_pos.py;
    uint32_t WOffset = GameService::getWOffset(v3);

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeString(packet, effect);
        Answer::NetPacket::writeInt32(packet, x);
        Answer::NetPacket::writeInt32(packet, y);
        m_pMap->broadcastPacket(packet);
    }
}

void Player::sendGainInfo(int32_t type, int64_t value, BenefitType benefitType)
{
    int8_t connId = m_connid;
    GameService* v5 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v5);
    int16_t cgindex = m_cgindex;
    int8_t v8 = connId;
    GameService* v9 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, type);
        Answer::NetPacket::writeInt64(packet, value);
        Answer::NetPacket::writeInt8(packet, static_cast<int8_t>(benefitType));
        GameService::sendPacketTo(v9, connId, m_cgindex, packet);
    }
}

void Player::sendKillerRankSelf()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int32_t Record = getRecord(1010);
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v6 = connId;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, Record);
        GameService::sendPacketTo(v7, connId, m_cgindex, packet);
    }
}

void Player::sendPreventWallow()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v5 = connId;
    GameService* v6 = Answer::Singleton<GameService>::instance();

    int32_t totalTime = m_sysUser.total_online_time;
    if (!m_sysUser.adult) {
        totalTime += getNow() - m_sysUser.last_login_time;
    }

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, totalTime);
        Answer::NetPacket::writeInt8(packet, m_sysUser.adult ? 1 : 0);
        GameService::sendPacketTo(v6, connId, m_cgindex, packet);
    }
}

void Player::sendChrLoginInInfo()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int8_t Debug = v3->getDebugMode();
    GameService* v5 = Answer::Singleton<GameService>::instance();
    int32_t Line = GameService::getLine(v5);
    uint32_t WOffset = GameService::getWOffset(v5);
    int16_t cgindex = m_cgindex;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt8(packet, Debug);
        Answer::NetPacket::writeInt32(packet, Line);
        GameService::sendPacketTo(v5, connId, m_cgindex, packet);
    }
}

void Player::sendGambel(const MemChrBagVector *const item)
{
    if (!item)
        return;

    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    int32_t v4 = static_cast<int32_t>(item->size());
    uint32_t WOffset = GameService::getWOffset(v3);
    int16_t cgindex = m_cgindex;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, v4);
        for (size_t i = 0; i < item->size(); ++i) {
            const MemChrBag& bag = (*item)[i];
            Answer::NetPacket::writeInt32(packet, bag.itemId);
            Answer::NetPacket::writeInt32(packet, bag.count);
        }
        GameService::sendPacketTo(v3, connId, m_cgindex, packet);
    }
}

void Player::sendPublicChat(int32_t channel, Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    int32_t v5 = getLevel();
    CharId_t Cid = getCid();
    int32_t Sid = getRunnerId();
    int8_t VipType = getVipType();
    int8_t VipLevel = getVipLevel();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, channel);
        Answer::NetPacket::writeInt64(packet, Cid);
        Answer::NetPacket::writeString(packet, &m_name);
        Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(v5));
        Answer::NetPacket::writeInt8(packet, VipType);
        Answer::NetPacket::writeInt8(packet, VipLevel);
        Answer::NetPacket::writeData(packet, Answer::NetPacket::getData(inPacket), Answer::NetPacket::getSize(inPacket));
        GameService::sendPacketTo(v4, connId, m_cgindex, packet);
    }
}

int32_t Player::ChatValidateed(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    CharId_t Cid = getCid();
    int8_t v5 = 0;
    GameService* v6 = Answer::Singleton<GameService>::instance();
    int32_t RunnerId = getRunnerId();
    CharId_t SendCharId = 0;
    GameService* v9 = Answer::Singleton<GameService>::instance();

    std::string content(reinterpret_cast<const char*>(Answer::NetPacket::getData(inPacket)),
                        Answer::NetPacket::getSize(inPacket));

    CGMBackstate* gmBackstate = Answer::Singleton<CGMBackstate>::instance();
    if (gmBackstate->ValidateChat(Cid, content)) {
        return 0;
    }

    return 10002;
}

void Player::SendPublicChat(int32_t channel, Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    if (channel == 1) {
        sendPublicChat(1, inPacket);
    } else if (channel == 2) {
        sendPublicChat(2, inPacket);
    } else {
        sendPublicChat(channel, inPacket);
    }
}

int32_t Player::onBuyXuWuValue(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    unsigned int nTime = getRecord(2107);
    if (nTime >= 10)
        return 10002;

    int32_t ConstGold = 100;
    if (m_chr.gold < ConstGold)
        return 10002;

    m_chr.gold -= ConstGold;
    updateRecord(2107, nTime + 1);
    setNeedSyncSelf();

    return 0;
}

void Player::setNetPacketHandler(int32_t proc, Player::NetPacketHandler handler)
{
    if (proc > 5 && proc <= 996) {
        m_packetHandlers[proc] = handler;
    }
}

Player::NetPacketHandler Player::getNetPacketHandler(int32_t proc)
{
    if (proc <= 5 || proc > 996) {
        if (proc <= 20052 || proc > 20087) {
            return Player::NetPacketHandler();
        }
    }

    auto it = m_packetHandlers.find(proc);
    if (it != m_packetHandlers.end()) {
        return it->second;
    }

    return Player::NetPacketHandler();
}

void Player::setBaseAttr()
{
    int32_t v1 = getLevel();
    int32_t Job = m_job;
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t addon = 0;
    int32_t index = 0;
    int32_t v6 = 0;

    CfgLevelAttr levelAttr;
    v3->getLevelAttr(Job, v1, &levelAttr);

    setAttr(CObjAttrs::ATTR_STR, levelAttr.str);
    setAttr(CObjAttrs::ATTR_DEX, levelAttr.dex);
    setAttr(CObjAttrs::ATTR_INT, levelAttr.inte);
    setAttr(CObjAttrs::ATTR_CON, levelAttr.con);
    setAttr(CObjAttrs::ATTR_LUK, levelAttr.luk);

    setNeedSyncSelf();
}

void Player::recalcAttr(bool bNow, bool bInit)
{
    CObjAttrs newAttr;
    CObjAttrs oldAttr = m_attrs;
    AttrAddon addon;
    AttrAddonList addAttrs;
    int32_t ShiHua = 0;
    int32_t Valuse = 0;
    int32_t DropRate = 0;

    setBaseAttr();

    // Apply equipment bonuses
    m_extEquip.CalcAddons(&addAttrs);
    for (auto& a : addAttrs) {
        addAttr(a.index, a.value);
    }

    // Apply skill bonuses
    m_extCharSkill.CalcAddons(&addAttrs);
    for (auto& a : addAttrs) {
        addAttr(a.index, a.value);
    }

    m_attrs = newAttr;
    CalBattle();

    if (!bInit) {
        setNeedSyncSelf();
    }

    m_needRecalAttr = false;
}

bool Player::queryBagInfo(Answer::NetPacket *inPacket, Int32Vector *const vSlot)
{
    if (!inPacket)
        return false;

    int32_t slotCount = Answer::NetPacket::readInt32(inPacket);
    if (static_cast<uint32_t>(slotCount) > 150)
        return false;

    for (int32_t i = 0; i < slotCount; ++i) {
        int32_t slot = Answer::NetPacket::readInt32(inPacket);
        vSlot->push_back(slot);
    }

    return true;
}

bool Player::isDeadProc(ProcId_t nProc)
{
    if (nProc > 0x4E20u && nProc <= 0x61AAu)
        return true;
    if (nProc <= 0x3Au)
        return nProc >= 0x38u || nProc == 8 || nProc == 38;
    if (nProc == 434)
        return true;
    if (nProc > 0x1B2u)
        return nProc == 440 || nProc == 463;
    return false;
}

bool Player::checkNetPackets()
{
    Answer::NetPacket* packet = nullptr;

    while (true) {
        if (m_netPackets.empty())
            break;
        packet = m_netPackets.front();
        m_netPackets.pop_front();
        if (!packet)
            break;

        uint16_t Proc = Answer::NetPacket::getProc(packet);
        int8_t ConnId = m_connid;
        DBService* v7 = Answer::Singleton<DBService>::instance();

        proc_log stu;
        stu.cid = getCid();
        stu.proc = Proc;
        stu.time = getNow();
        MGLog::log(stu);

        Player::NetPacketHandler handler = getNetPacketHandler(Proc);
        if (handler) {
            int32_t result = (this->*handler)(packet);
            if (result != 0) {
                delete packet;
                return true;
            }
        }

        delete packet;
    }

    return false;
}

int32_t Player::onLogout(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t reason = Answer::NetPacket::readInt32(inPacket);
    int32_t param = Answer::NetPacket::readInt32(inPacket);
    onLogout(reason, param);
    return 0;
}

void Player::onLogout(int32_t nReason, int32_t nParam)
{
    int32_t MapId = m_pMap ? m_pMap->GetId() : 0;
    int32_t Now = getNow();
    CharId_t Cid = getCid();
    int8_t connId = m_connid;
    DBService* v7 = Answer::Singleton<DBService>::instance();

    CExtCharDepotManager::OnPlayerLogout(this);

    saveOnlineTime();
    saveToDB(nReason, nParam, Now);

    logout_log stu;
    stu.cid = Cid;
    stu.reason = nReason;
    stu.param = nParam;
    stu.time = Now;
    MGLog::log(stu);
}

int32_t Player::onSyncTime(Answer::NetPacket *inPacket)
{
    SyncTime();
    return 0;
}

void Player::SyncTime()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int32_t Now = getNow();
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v6 = connId;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, Now);
        GameService::sendPacketTo(v7, connId, m_cgindex, packet);
    }
}

void Player::SetTeJieCRI()
{
    m_TeJieCRI = getTick();
}

void Player::LogProc(int32_t procId)
{
    int8_t ConnId = m_connid;
    DBService* v3 = Answer::Singleton<DBService>::instance();

    PacketProcLog stu;
    stu.cid = getCid();
    stu.proc = procId;
    stu.time = getNow();
    MGLog::log(stu);
}

void Player::LogNpc(int32_t NpcId, int32_t Opway, int32_t OtherNpcId)
{
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    CfgData* v5 = Answer::Singleton<CfgData>::instance();
    int32_t v6 = NpcId;
    int32_t v7 = Opway;
    int32_t v8 = getNow();
    int32_t v9 = NpcId;
    int32_t v10 = Opway;
    int32_t v11 = OtherNpcId;

    NpcFuncLog stu;
    stu.cid = getCid();
    stu.npcId = NpcId;
    stu.opWay = Opway;
    stu.otherNpcId = OtherNpcId;
    stu.time = getNow();
    MGLog::log(stu);
}

int32_t Player::onMove(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int64_t Tick = getTick();
    CExtCharSkill* CharSkill = getExtCharSkill();
    int32_t y = Answer::NetPacket::readInt32(inPacket);
    int32_t x = Answer::NetPacket::readInt32(inPacket);
    int32_t v8 = x;
    int32_t v9 = y;
    Direction Direction = Answer::NetPacket::readInt8(inPacket);
    Direction v11 = Answer::NetPacket::readInt8(inPacket);

    if (!m_pMap)
        return 10002;

    Position targetPos = { x, y };
    m_pMap->moveUnit(this, targetPos, Direction);

    return 0;
}

int32_t Player::onTrailerMove(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t y = Answer::NetPacket::readInt32(inPacket);
    int32_t x = Answer::NetPacket::readInt32(inPacket);
    int32_t v5 = y;
    int32_t v6 = x;
    int32_t v7 = 0;
    int32_t v8 = 0;

    Position Pixel = { x, y };
    Position pos = { x, y };

    if (m_trailer && m_pMap) {
        m_trailer->MoveTo(this, pos);
    }

    return 0;
}

int32_t Player::onJump(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t y = Answer::NetPacket::readInt32(inPacket);
    int32_t x = Answer::NetPacket::readInt32(inPacket);
    Direction v6 = Answer::NetPacket::readInt8(inPacket);

    Position pos = { x, y };
    Position CurrentTile = getTilePos();
    Position v9 = { 0, 0 };
    Position v10 = { 0, 0 };
    Position v11 = { 0, 0 };

    if (!m_pMap)
        return 10002;

    m_pMap->jumpUnit(this, pos);
    return 0;
}

int32_t Player::onHit(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t y = Answer::NetPacket::readInt32(inPacket);
    int32_t x = Answer::NetPacket::readInt32(inPacket);

    Position pos = { x, y };
    Position CurrentTile = getTilePos();
    Position v9 = { 0, 0 };
    Position v10 = { 0, 0 };
    Position v11 = { 0, 0 };

    if (!m_pMap)
        return 10002;

    m_pMap->hitUnit(this, pos);
    return 0;
}

int32_t Player::onUnitThrowed(Answer::NetPacket *inPacket)
{
    if (!m_pMap || !inPacket)
        return 10002;

    int32_t RunnerId = 0;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    int64_t id = Answer::NetPacket::readInt64(inPacket);
    int32_t type = Answer::NetPacket::readInt32(inPacket);
    Unit* pUnit = m_pMap->getUnit(id);
    if (!pUnit)
        return 10002;

    RunnerId = pUnit->getRunnerId();
    return 0;
}

int32_t Player::onKickOutMove(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pMap)
        return 10002;

    int32_t RunnerId = 0;
    GameService* v5 = Answer::Singleton<GameService>::instance();
    Direction direction = static_cast<Direction>(Answer::NetPacket::readInt8(inPacket));
    int64_t id = Answer::NetPacket::readInt64(inPacket);
    int32_t type = Answer::NetPacket::readInt32(inPacket);
    Unit* pUnit = m_pMap->getUnit(id);
    if (!pUnit)
        return 10002;

    int16_t currentX = m_pos.px;
    int16_t currentY = m_pos.py;

    Position newPos = { currentX, currentY };
    m_pMap->moveUnit(this, newPos, direction);
    return 0;
}

int Player::BirthdayIsRight(std::string *p_cardId)
{
    if (!p_cardId || p_cardId->empty())
        return 0;

    const char* v2 = p_cardId->c_str();
    int len = static_cast<int>(p_cardId->length());

    if (len < 15)
        return 0;

    std::string day;
    std::string month;
    tm tmNow;
    time_t now = time(nullptr);
    localtime_s(&tmNow, &now);

    if (len == 18) {
        month = p_cardId->substr(10, 2);
        day = p_cardId->substr(12, 2);
    } else {
        month = p_cardId->substr(8, 2);
        day = p_cardId->substr(10, 2);
    }

    int nMonth = atoi(month.c_str());
    int nDay = atoi(day.c_str());

    if (nMonth == tmNow.tm_mon + 1 && nDay == tmNow.tm_mday)
        return 1;

    return 0;
}

int Player::GetDay(int year, int month)
{
    if (static_cast<unsigned int>(month) > 12)
        return -1;

    int64_t v3 = 1LL << month;
    if ((v3 & 0x15AA) != 0)
        return 31;
    if ((v3 & 0xA50) != 0)
        return 30;

    if (month == 2) {
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
            return 29;
        return 28;
    }

    return -1;
}

int32_t Player::checkPreventWallow(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    const char* v3 = nullptr;
    const char* v4 = nullptr;

    std::string identitycard;
    std::string name;
    std::string p_card;
    bool isUseFull = false;

    name = Answer::NetPacket::readString(inPacket);
    identitycard = Answer::NetPacket::readString(inPacket);

    if (isPreventWallow(&identitycard)) {
        m_sysUser.adult = true;
        v2 = 0;
    } else {
        v2 = 10002;
    }

    return v2;
}

bool Player::CheckString(std::string *p_card)
{
    if (!p_card || p_card->empty())
        return false;

    const char* v2 = p_card->c_str();
    int nLen1 = static_cast<int>(p_card->length());

    char szStr[16];
    char szCardBuf[28];
    int nFlag = 0;
    int nLen2 = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < nLen1; ++i) {
        if (v2[i] >= '0' && v2[i] <= '9') {
            szCardBuf[nLen2++] = v2[i] - '0';
        }
    }

    if (nLen2 != 15 && nLen2 != 18)
        return false;

    for (i = 0; i < nLen2; ++i) {
        nFlag += szCardBuf[i];
    }

    return nFlag > 0;
}

bool Player::isPreventWallow(std::string *p_card)
{
    if (!p_card || p_card->empty())
        return false;

    int no[20];
    char id[16];
    std::string v6;
    std::string p_cardId = *p_card;
    bool bResult = false;
    int nLen = static_cast<int>(p_cardId.length());
    bool bFlag = false;

    if (nLen == 15 || nLen == 18) {
        // Simple check: verify the ID card format
        bFlag = CheckString(&p_cardId);
        if (bFlag) {
            bResult = (nLen == 18 || nLen == 15);
        }
    }

    return bResult;
}

int32_t Player::onSwitchMap(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    MapManager* v3 = Answer::Singleton<MapManager>::instance();
    int32_t v6 = getLevel();
    CActivityManager* v9 = Answer::Singleton<CActivityManager>::instance();
    CfgData* v10 = Answer::Singleton<CfgData>::instance();
    CfgData* v11 = Answer::Singleton<CfgData>::instance();

    int32_t nMapId = Answer::NetPacket::readInt32(inPacket);
    Map* pMap = MapManager::getMap(v3, nMapId);
    if (!pMap)
        return 10002;

    return switchMap(pMap, m_pos.px, m_pos.py, false);
}

int32_t Player::onEnterActivity(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pMap)
        return 10002;

    int32_t actID = Answer::NetPacket::readInt32(inPacket);
    int32_t Proc = Answer::NetPacket::getProc(inPacket);
    EnterActivity(actID, Proc);
    return 0;
}

int32_t Player::OnGetMapBossInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = getLevel();
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t BossDistribution = v3->getBossDistribution(v2);
    int32_t v5 = BossDistribution;
    MapManager* v6 = Answer::Singleton<MapManager>::instance();
    int8_t connId = m_connid;
    GameService* v8 = Answer::Singleton<GameService>::instance();
    int32_t v9 = BossDistribution;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, BossDistribution);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }

    return 0;
}

int32_t Player::OnGetLevelBossInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = getLevel();
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t BossDistribution = v3->getBossDistribution(v2);
    int32_t v5 = BossDistribution;
    MapManager* v6 = Answer::Singleton<MapManager>::instance();
    int8_t connId = m_connid;
    GameService* v8 = Answer::Singleton<GameService>::instance();
    int32_t v9 = BossDistribution;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, BossDistribution);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }

    return 0;
}

int32_t Player::OnPaiMaiHangHanHua(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CExtCurrency* Currency = getExtCurrency();
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = m_cgindex;
    int8_t ConnId = m_connid;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    return 0;
}

int32_t Player::EnterActivity(int32_t nActId, int32_t nProc)
{
    int16_t cgindex = m_cgindex;
    int8_t connId = m_connid;
    GameService* v6 = Answer::Singleton<GameService>::instance();
    int16_t v8 = cgindex;
    int8_t v9 = connId;
    GameService* v10 = Answer::Singleton<GameService>::instance();
    CActivityManager* v11 = Answer::Singleton<CActivityManager>::instance();
    CfgData* v13 = Answer::Singleton<CfgData>::instance();

    CActivity* pActivity = v11->GetActivity(nActId);
    if (!pActivity)
        return 10002;

    return pActivity->Enter(this, nProc);
}

int32_t Player::onLeaveActivity(Answer::NetPacket *inPacket)
{
    if (StaticObj::InActivity(this)) {
        leaveActivity();
    }
    return 0;
}

int32_t Player::onActivityChangeMap(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    MapManager* v4 = Answer::Singleton<MapManager>::instance();
    int32_t nMapId = Answer::NetPacket::readInt32(inPacket);
    unsigned int nPosX = Answer::NetPacket::readInt32(inPacket);
    unsigned int nPosY = Answer::NetPacket::readInt32(inPacket);
    unsigned int nParam = Answer::NetPacket::readInt32(inPacket);

    Map* Map = MapManager::getMap(v4, nMapId);
    if (!Map)
        return 10002;

    CActivityMap* pActMap = dynamic_cast<CActivityMap*>(Map);
    if (pActMap) {
        pActMap->PlayerEnter(this, nPosX, nPosY);
    }

    return 0;
}

Position Player::GetSummonBossPos()
{
    Position Pos = { 0, 0 };
    std::vector<Position> vPos;

    if (!m_pMap)
        return Pos;

    m_pMap->getSummonBossPositions(&vPos);
    if (vPos.empty())
        return Pos;

    Answer::Random* v2 = Answer::Singleton<Answer::Random>::instance();
    int32_t idx = v2->randInt(0, static_cast<int32_t>(vPos.size()) - 1);
    return vPos[idx];
}

int32_t Player::OnCycleTowerEvent(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t MapId = m_pMap ? m_pMap->GetId() : 0;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CycleTowerTable* CycleTowerTable = v4->getCycleTowerTable();
    int32_t v6 = MapId;
    CfgData* v7 = Answer::Singleton<CfgData>::instance();
    CfgData* v8 = Answer::Singleton<CfgData>::instance();
    CPoolManager* v9 = Answer::Singleton<CPoolManager>::instance();
    int32_t Now = getNow();

    return 0;
}

void Player::AddBeiGongAttr(int32_t nType, int32_t nValue)
{
    auto it = m_BeiGongAttr.find(nType);
    if (it != m_BeiGongAttr.end()) {
        it->second += nValue;
    } else {
        m_BeiGongAttr[nType] = nValue;
    }
}

void Player::SendBeiGongAttr()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v8 = connId;
    GameService* v9 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, static_cast<int32_t>(m_BeiGongAttr.size()));
        for (auto& [type, value] : m_BeiGongAttr) {
            Answer::NetPacket::writeInt32(packet, type);
            Answer::NetPacket::writeInt32(packet, value);
        }
        GameService::sendPacketTo(v9, connId, m_cgindex, packet);
    }
}

void Player::UpdateCycleTowerState(CYCLE_TOWER_ENVET_TYPE nType)
{
    if (!StaticObj::IsCycleTower(this))
        return;

    int32_t MapId = StaticObj::getMapId(this);
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const CycleTowerTable* CycleTowerTable = v3->getCycleTowerTable();
    const CycleTowerMapCfg* pMapCfg = CycleTowerTable->getMapCfg(MapId);

    if (!pMapCfg)
        return;

    if (nType == CYCLE_TOWER_ENVET_TYPE::CTET_ENTER) {
        updateRecord(1922, getNow());
    } else if (nType == CYCLE_TOWER_ENVET_TYPE::CTET_LEAVE) {
        updateRecord(1922, 0);
    }
}

int32_t Player::OnRollTheDice(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    Answer::Random* v3 = Answer::Singleton<Answer::Random>::instance();
    int32_t MapId = m_pMap ? m_pMap->GetId() : 0;
    CfgData* v5 = Answer::Singleton<CfgData>::instance();
    const CycleTowerTable* CycleTowerTable = v5->getCycleTowerTable();
    CfgData* v7 = Answer::Singleton<CfgData>::instance();
    const CycleTowerTable* v8 = v7->getCycleTowerTable();
    MapManager* v9 = Answer::Singleton<MapManager>::instance();
    CfgData* v10 = Answer::Singleton<CfgData>::instance();

    int32_t diceResult = v3->randInt(1, 6);
    return 0;
}

void Player::CheckEnterCycleTower()
{
    int32_t Now = getNow();
    MapManager* v2 = Answer::Singleton<MapManager>::instance();
    int32_t RunnerId = getRunnerId();
    int32_t v4 = 0;
    MapManager* v5 = Answer::Singleton<MapManager>::instance();
    int32_t MapId = 0;
    int32_t GongGaoId = 0;
    Position EnterPos = { 0, 0 };

    CfgData* cfgData = Answer::Singleton<CfgData>::instance();
    const CycleTowerTable* towerTable = cfgData->getCycleTowerTable();
    if (towerTable) {
        MapId = towerTable->getEnterMapId();
        GongGaoId = towerTable->getEnterGongGaoId();
        EnterPos = towerTable->getEnterPos();
    }

    Map* pMap = MapManager::getMap(v2, MapId);
    if (pMap) {
        switchMap(pMap, EnterPos.x, EnterPos.y, false);
    }
}

void Player::CheckLeaveCycleTower()
{
    int EnterTime = getRecord(1922);
    if (EnterTime > 0 && getNow() >= EnterTime + 1800) {
        updateRecord(1922, 0);
    }
}

void Player::CheckLevelWastelands()
{
    if (!m_pMap)
        return;

    if (m_pMap->IsWastelands()) {
        int LevelTime = getRecord(2106);
        if (LevelTime > 0 && getNow() >= LevelTime) {
            moveToReviveRegion(false);
        }
    }
}

void Player::CheckAddWastelandsValue()
{
    if (m_pMap && m_pMap->IsWastelands()) {
        AddWastelandsValue(1);
    }
}

int32_t Player::GetMaxValue()
{
    return 600 * getRecord(2107) + 900;
}

void Player::AddWastelandsValue(int32_t Value)
{
    if (Value <= 0)
        return;

    if (getRecord(2106) > 0)
        return;

    int32_t OldRecord = getRecord(2105);
    if (GetMaxValue() > OldRecord) {
        updateRecord(2105, OldRecord + Value);
    }
}

void Player::AddBeastShrineValue(int32_t Value)
{
    if (!m_pMap)
        return;

    if (!m_pMap->IsBeastShrine())
        return;

    int OldRecord = getRecord(1163);
    if (OldRecord <= 599) {
        updateRecord(1163, OldRecord + Value);
    }
}

void Player::CheckAddBeastShrineValue()
{
    if (m_pMap && m_pMap->IsBeastShrine()) {
        AddBeastShrineValue(1);
    }
}

void Player::CheckBeastShrinePoints()
{
    if (m_pMap && m_pMap->IsBeastShrine()
        && getRecord(1164) <= 0
        && getRecord(1163) > 599) {
        int32_t v1 = getNow() + 20;
        updateRecord(1164, v1);
    }
}

void Player::CheckBeastShrineAutoKick()
{
    if (!m_pMap)
        return;

    MapManager* v1 = Answer::Singleton<MapManager>::instance();
    int32_t RunnerId = getRunnerId();
    int32_t v3 = 0;
    MapManager* v4 = Answer::Singleton<MapManager>::instance();
    int nRecord = getRecord(1164);

    if (nRecord > 0 && getNow() >= nRecord) {
        Map* pTargetMap = MapManager::getMap(v1, 1001);
        if (pTargetMap) {
            switchMap(pTargetMap, 50, 50, false);
        }
    }
}

void Player::SendCycleTowerInfo()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int32_t Record = getRecord(1922);
    int32_t v4 = getRecord(1923);
    int8_t v5 = static_cast<int8_t>(Record > 0 ? 1 : 0);
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v8 = connId;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt8(packet, v5);
        Answer::NetPacket::writeInt32(packet, v4);
        GameService::sendPacketTo(v2, connId, m_cgindex, packet);
    }
}

void Player::SendCycleTowerTime()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int v3 = getRecord(1922);
    int32_t v4 = 0;
    if (v3 > 0) {
        v4 = v3 + 1800 - getNow();
        if (v4 < 0)
            v4 = 0;
    }
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, v4);
        GameService::sendPacketTo(v7, connId, m_cgindex, packet);
    }
}

void Player::SetPlantState(bool bState)
{
    bool Change = false;
    if (bState) {
        if (m_plantState != 1) {
            m_plantState = 1;
            Change = true;
        }
    } else {
        if (m_plantState != 0) {
            m_plantState = 0;
            Change = true;
        }
    }

    if (Change) {
        SetNeedSyncAround();
    }
}

int32_t Player::OnEnterSpecialBossMap(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    MapManager* v4 = Answer::Singleton<MapManager>::instance();
    int32_t MapId = Answer::NetPacket::readInt32(inPacket);
    CfgData* v6 = Answer::Singleton<CfgData>::instance();
    int32_t v7 = MapId;
    CfgData* v8 = Answer::Singleton<CfgData>::instance();
    const CfgBeastShrineTable* BeastShrineTable = v8->getBeastShrineTable();
    CExtCharBag* Bag = getExtCharBag();
    CExtCharBag* v11 = getExtCharBag();

    Map* pMap = MapManager::getMap(v4, MapId);
    if (!pMap)
        return 10002;

    return switchMap(pMap, 0, 0, false);
}

int32_t Player::OnLeaveSpecialBossMap(Answer::NetPacket *inPacket)
{
    return 0;
}

int32_t Player::OnChristmasDuiHuan(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    CExtCharBag* Bag = getExtCharBag();
    CExtOperateLimit* OperateLimit = getOperateLimit();
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = m_cgindex;
    int8_t ConnId = m_connid;
    GameService* v10 = Answer::Singleton<GameService>::instance();

    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    return 0;
}

int32_t Player::onDungeonRandom(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    int32_t nRandom = Answer::NetPacket::readInt32(inPacket);
    pDungeon->Random(nRandom);
    return 0;
}

int32_t Player::OnBuyJingLiValue(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const EnergyCfg* EnergyCfg = v3->getEnergyCfg();
    int32_t Record = getRecord(1103);
    int32_t v6 = Record;
    int64_t v7 = 0;
    int32_t BuyAddEnergy = EnergyCfg->getBuyEnergy(Record);
    CExtOperateLimit* OperateLimit = getOperateLimit();

    if (BuyAddEnergy <= 0 || static_cast<uint32_t>(Record) >= EnergyCfg->getMaxBuyCount()) {
        return 10002;
    }

    int32_t CostGold = EnergyCfg->getBuyGold(Record);
    if (m_chr.gold < CostGold)
        return 10002;

    m_chr.gold -= CostGold;
    updateRecord(1103, Record + 1);
    // Add energy
    m_chr.energy += BuyAddEnergy;
    setNeedSyncSelf();

    return 0;
}

void Player::SendJingLiValue()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int32_t Record = getRecord(1103);
    int32_t v4 = getRecord(1104);
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, Record);
        Answer::NetPacket::writeInt32(packet, v4);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::ResetJingLi()
{
    CfgData* v1 = Answer::Singleton<CfgData>::instance();
    const EnergyCfg* EnergyCfg = v1->getEnergyCfg();
    int32_t Record = getRecord(1104);
    int32_t v5 = EnergyCfg->getMaxEnergy();
    int32_t Now = getNow();

    tm NowTime;
    time_t now = time(nullptr);
    localtime_s(&NowTime, &now);

    if (Record < v5) {
        updateRecord(1104, Record + 1);
    }
}

int32_t Player::OnBuyRandomPosTimes(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    int32_t nTimes = Answer::NetPacket::readInt32(inPacket);
    pDungeon->BuyRandomPosTimes(this, nTimes);
    return 0;
}

int32_t Player::onFamilyWarActivePillar(Answer::NetPacket *inPacket)
{
    if (!inPacket || !StaticObj::InActivity(this))
        return 10002;

    int8_t nIndex = Answer::NetPacket::readInt8(inPacket);

    CActivityMap* pActMap = dynamic_cast<CActivityMap*>(m_pMap);
    if (!pActMap)
        return 10002;

    CActivity* Activity = pActMap->GetActivity();
    if (!Activity)
        return 10002;

    CFamilyWar* v6 = dynamic_cast<CFamilyWar*>(Activity);
    if (v6) {
        v6->ActivePillar(this, nIndex);
    }

    return 0;
}

int32_t Player::GetDungeonDailyEnterLimit(int32_t nDungeonId)
{
    CfgData* v2 = Answer::Singleton<CfgData>::instance();
    CfgDungeon* pCfgDungeon = v2->getDungeon(nDungeonId);
    if (!pCfgDungeon)
        return 0;

    int32_t v4 = pCfgDungeon->getDailyEnterLimit();
    return v4;
}

int32_t Player::onBuyDungeonEnterTime(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t v4 = 0;
    int32_t v5 = 0;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t cgindex = m_cgindex;
    int8_t connId = m_connid;
    GameService* v9 = Answer::Singleton<GameService>::instance();

    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    return 0;
}

int32_t Player::onDungeonQuickDone(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = getLevel();
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int weekday = 0;
    Job_t job = m_job;
    int v7 = 0;
    int v8 = 0;
    CharId_t Cid = getCid();
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);

    return 0;
}

int32_t Player::onEnterDungeon(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pMap)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t nDungeonId = Answer::NetPacket::readInt32(inPacket);
    CfgDungeon* pCfgDungeon = v3->getDungeon(nDungeonId);
    if (!pCfgDungeon)
        return 10002;

    MapManager* mapMgr = Answer::Singleton<MapManager>::instance();
    Map* pDungeonMap = MapManager::createDungeon(mapMgr, nDungeonId);
    if (!pDungeonMap)
        return 10002;

    return switchMap(pDungeonMap, 0, 0, false);
}

int32_t Player::onLeaveDungeon(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    Dungeon* pDungeon = nullptr;
    if (m_pMap && m_pMap->GetType() == 3) {
        pDungeon = static_cast<Dungeon*>(m_pMap);
    }

    if (!pDungeon)
        return 10002;

    int8_t connId = m_connid;
    DBService* v6 = Answer::Singleton<DBService>::instance();

    return leaveDungeon();
}

int32_t Player::onDungeonBuildTower(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    int16_t nPosX = Answer::NetPacket::readInt16(inPacket);
    int16_t nPosY = Answer::NetPacket::readInt16(inPacket);

    pDungeon->BuildTower(this, nId, nPosX, nPosY);
    return 0;
}

int32_t Player::onDungeonBuyTower(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    pDungeon->BuyTower(this, nId);
    return 0;
}

int32_t Player::onGuessTheSize(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    int32_t nId = Answer::NetPacket::readInt32(inPacket);
    int32_t nSize = Answer::NetPacket::readInt32(inPacket);

    pDungeon->GuessTheSize(this, nId, nSize);
    return 0;
}

int32_t Player::onDungeonYJSKGuWu(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    pDungeon->YJSKGuWu(this, Proc);
    return 0;
}

int32_t Player::onDungeonStart(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    pDungeon->Start(this);
    return 0;
}

int32_t Player::onDungeonSelectReward(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    int8_t index = Answer::NetPacket::readInt8(inPacket);
    pDungeon->SelectReward(this, index);
    return 0;
}

int32_t Player::onDungeonSummonBoss(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    pDungeon->SummonBoss(this);
    return 0;
}

int32_t Player::onDungeonSummon(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (!m_pMap || m_pMap->GetType() != 3)
        return 10002;

    Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
    if (!pDungeon)
        return 10002;

    int32_t nIndex = Answer::NetPacket::readInt32(inPacket);
    pDungeon->Summon(this, nIndex);
    return 0;
}

int32_t Player::onDungeonSaoDang(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);

    int32_t nExp = 0;
    MemChrBagVector vItem;
    int32_t nCurFloor = 0;

    if (m_pMap && m_pMap->GetType() == 3) {
        Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
        if (pDungeon) {
            pDungeon->SaoDang(this, &nExp, &vItem, &nCurFloor);
        }
    }

    return 0;
}

int32_t Player::onDungeonReset(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (StaticObj::InDungeon(this)) {
        Dungeon* pDungeon = static_cast<Dungeon*>(m_pMap);
        if (pDungeon) {
            pDungeon->ResetFloor(this);
        }
    }

    return 0;
}

int32_t Player::onEnterChargeDungeon(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (StaticObj::InDungeon(this))
        return 10002;

    int32_t TodayPayGold = getRecord(1101);
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    int32_t ChargeDungeonId = v4->getChargeDungeonId(TodayPayGold);
    int32_t nId = ChargeDungeonId;

    if (ChargeDungeonId <= 0)
        return 10002;

    MapManager* mapMgr = Answer::Singleton<MapManager>::instance();
    Map* pDungeonMap = MapManager::createDungeon(mapMgr, ChargeDungeonId);
    if (!pDungeonMap)
        return 10002;

    return switchMap(pDungeonMap, 0, 0, false);
}

int32_t Player::onPickDropItem(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pMap)
        return 10002;

    int64_t id = Answer::NetPacket::readInt64(inPacket);
    m_pMap->pickDropItem(this, id);
    return 0;
}

int32_t Player::onUseTrap(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pMap)
        return 10002;

    int64_t id = Answer::NetPacket::readInt64(inPacket);
    Trap* trap = m_pMap->getTrap(id);
    if (trap) {
        trap->use(this);
    }

    return 0;
}

int32_t Player::onSwitchPkMode(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pMap)
        return 10002;

    int pkMode = Answer::NetPacket::readInt32(inPacket);
    setPkMode(pkMode, true);

    return 0;
}

int32_t Player::onSafeRevive(Answer::NetPacket *inPacket)
{
    int32_t Now = getNow();
    safeRevive();
    return 0;
}

void Player::SafeToPrison()
{
    CfgData* v1 = Answer::Singleton<CfgData>::instance();
    int32_t mapid = v1->getPrisonMapId();
    MapManager* v3 = Answer::Singleton<MapManager>::instance();
    Position pos;
    CfgMapRegion* pCfgRegion = v1->getMapRegion(mapid, 0);
    Map* pTempMap = MapManager::getMap(v3, mapid);

    if (pCfgRegion) {
        pos = pCfgRegion->getBirthPos();
    }

    if (pTempMap) {
        switchMap(pTempMap, pos.x, pos.y, false);
    }
}

int32_t Player::onSiteRevive(Answer::NetPacket *inPacket)
{
    int32_t v2 = getNow();
    int8_t connId = m_connid;
    GameService* v7 = Answer::Singleton<GameService>::instance();
    int v10 = 0;

    if (!isAlive()) {
        safeRevive();
    }

    return 0;
}

int32_t Player::onStrongRevive(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    if (isAlive())
        return 10002;

    FillHP(100);
    SetDieTick();
    FillMP(100);
    setNeedSyncSelf();

    return 0;
}

int32_t Player::onQueryChrInfo(Answer::NetPacket *inPacket)
{
    setNeedSyncSelf();
    return 0;
}

int32_t Player::onUpgradeLevel(Answer::NetPacket *inPacket)
{
    int32_t OldLevel = getLevel();
    if (upgradeLevel(0)) {
        int32_t v2 = getLevel();
        LevelUped(OldLevel, v2);
    }
    return 0;
}

int32_t Player::onQueryTaskList(Answer::NetPacket *inPacket)
{
    if (m_task) {
        m_task->sendTaskList();
    }
    return 0;
}

int32_t Player::onReceiveTask(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t tid = Answer::NetPacket::readInt32(inPacket);
    if (m_task) {
        return m_task->receiveTask(tid);
    }
    return 10002;
}

int32_t Player::onSubmitTask(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    CVip* PlayerVip = getVip();

    int32_t tid = Answer::NetPacket::readInt32(inPacket);
    if (m_task) {
        return m_task->submitTask(tid);
    }
    return 10002;
}

int32_t Player::onGiveUpTask(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t tid = Answer::NetPacket::readInt32(inPacket);
    if (m_task) {
        return m_task->giveUp(tid);
    }
    return 10002;
}

int32_t Player::onSetTaskCanSubmit(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t tid = Answer::NetPacket::readInt32(inPacket);
    CfgTask* pCfgTask = v3->getTask(tid);
    if (!pCfgTask)
        return 10002;

    setTaskCanSubmit(tid);
    return 0;
}

int32_t Player::onTalkWithNpc(Answer::NetPacket *inPacket)
{
    if (!m_pMap || !inPacket)
        return 10002;

    Position pos = getPos();
    Position CurrentTile = getTilePos();
    int64_t npcid = Answer::NetPacket::readInt64(inPacket);
    Npc* npc = m_pMap->getNpc(npcid);
    if (!npc)
        return 10002;

    npc->onTalk(this);
    return 0;
}

int32_t Player::onQuickDone(Answer::NetPacket *inPacket)
{
    if (inPacket) {
        Answer::NetPacket::readInt32(inPacket);
        Answer::NetPacket::readInt32(inPacket);
    }
    return 10002;
}

int32_t Player::onTeleport(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t targetId = Answer::NetPacket::readInt32(inPacket);
    return doTeleport(targetId);
}

int32_t Player::onTeleportActivity(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t targetId = Answer::NetPacket::readInt32(inPacket);
    return doTeleportActivity(targetId);
}

int32_t Player::onAddAction(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int slot = Answer::NetPacket::readInt32(inPacket);
    int32_t id = Answer::NetPacket::readInt32(inPacket);
    int32_t type = Answer::NetPacket::readInt32(inPacket);
    // Add action to slot
    if (slot > 0 && slot <= 10) {
        m_actions[slot - 1] = { id, type };
    }
    return 0;
}

int32_t Player::onRemoveAction(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int slot = Answer::NetPacket::readInt32(inPacket);
    if (slot <= 0 || slot > 10)
        return 10002;

    m_actions[slot - 1] = { 0, 0 };
    return 0;
}

int32_t Player::onExchangeAction(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int oldslot = Answer::NetPacket::readInt32(inPacket);
    int newslot = Answer::NetPacket::readInt32(inPacket);
    if (oldslot <= 0 || oldslot > 10 || newslot <= 0 || newslot > 10)
        return 10002;

    std::swap(m_actions[oldslot - 1], m_actions[newslot - 1]);
    return 0;
}

int32_t Player::onSetAutoFight(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    std::string pick = Answer::NetPacket::readString(inPacket);
    std::string fight = Answer::NetPacket::readString(inPacket);

    m_autoFight.pick = pick;
    m_autoFight.fight = fight;

    return 0;
}

int32_t Player::onSetSystemSetting(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t Key = Answer::NetPacket::readInt32(inPacket);
    int32_t Values = Answer::NetPacket::readInt32(inPacket);

    m_systemSettings[Key] = Values;
    setNeedSyncSelf();

    return 0;
}

int32_t Player::onBuyBackChrShopItem(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t index = Answer::NetPacket::readInt32(inPacket);
    int32_t itemID = Answer::NetPacket::readInt32(inPacket);
    int8_t itemClass = Answer::NetPacket::readInt8(inPacket);

    return m_extCharBag.buyBackChrShopItem(index, itemID, itemClass);
}

int32_t Player::onBuyTaskCount(Answer::NetPacket *inPacket)
{
    if (inPacket)
        return 0;
    else
        return 10002;
}

int32_t Player::onBuyResource(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;
    Answer::NetPacket::readInt32(inPacket);
    Answer::NetPacket::readInt32(inPacket);
    return 0;
}

int32_t Player::onBuyChrShopItem(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t id = Answer::NetPacket::readInt32(inPacket);
    int count = Answer::NetPacket::readInt32(inPacket);
    CfgChrShop* pCharShop = v3->getChrShop(id);
    if (!pCharShop)
        return 10002;

    return m_extCharBag.buyChrShopItem(id, count);
}

int32_t Player::onGamePublicChat(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CActivityMap* v3 = dynamic_cast<CActivityMap*>(m_pMap);
    CharId_t Cid = getCid();
    CGMBackstate* v6 = Answer::Singleton<CGMBackstate>::instance();

    if (ChatValidateed(inPacket) == 0) {
        SendPublicChat(1, inPacket);
    }

    return 0;
}

int32_t Player::onCrossPrivateChat(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CharId_t v3 = getCid();
    CGMBackstate* v4 = Answer::Singleton<CGMBackstate>::instance();

    if (ChatValidateed(inPacket) == 0) {
        // Forward to cross-server
    }

    return 0;
}

int32_t Player::GetChatLevel()
{
    CfgData* v1 = Answer::Singleton<CfgData>::instance();
    int32_t ServerDiffDay = v1->getServerDiffDay(SERVER_TYPE::SVT_NORMAL);
    int32_t Day = ServerDiffDay + 1;

    if (!ServerDiffDay) {
        return getLevel();
    }

    if (Day <= 7) {
        return getLevel();
    }

    return getLevel();
}

void Player::onSendSocialData(CharId_t cid)
{
    int8_t ConnId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    int32_t GateIndex = GameService::getGateIndex(v3);
    uint32_t WOffset = GameService::getWOffset(v3);
    int8_t v6 = ConnId;
    GameService* v7 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* Packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (Packet) {
        Answer::NetPacket::writeInt64(Packet, cid);
        GameService::sendPacketTo(v7, ConnId, m_cgindex, Packet);
    }
}

int32_t Player::onUpdateFlyIconInt(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t Int32 = Answer::NetPacket::readInt32(inPacket);
    updateRecord(1013, Int32);
    return 0;
}

int32_t Player::onSetGuaJi(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    m_nGuaJi = Answer::NetPacket::readInt8(inPacket);
    return 0;
}

int32_t Player::onQueryKillerRankSelf(Answer::NetPacket *inPacket)
{
    sendKillerRankSelf();
    return 0;
}

int32_t Player::onTeleportByItem(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    int16_t cgindex = m_cgindex;
    int8_t connId = m_connid;
    GameService* v5 = Answer::Singleton<GameService>::instance();
    MapManager* v6 = Answer::Singleton<MapManager>::instance();
    int64_t v7 = Answer::NetPacket::readInt64(inPacket);

    int32_t itemId = static_cast<int32_t>(v7);
    m_extCharBag.UseItem(itemId);
    return 0;
}

int32_t Player::onClickPayButton(Answer::NetPacket *inPacket)
{
    ++m_chr.pay_click_count;
    return 0;
}

int32_t Player::onMapEntered(Answer::NetPacket *inPacket)
{
    m_sysUser.map_enter_time = getNow();
    return 0;
}

int32_t Player::onDebugCmd(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    MapManager* v5 = Answer::Singleton<MapManager>::instance();

    return 0;
}

int32_t Player::onQueryPlayerInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t RunnerId = 0;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    int64_t nTargetId = Answer::NetPacket::readInt64(inPacket);
    Player* pTarget = nullptr;
    MapManager* mapMgr = Answer::Singleton<MapManager>::instance();
    // pTarget = mapMgr->getPlayer(nTargetId);
    if (pTarget) {
        pTarget->SendOtherPlayerInfo(m_connid, m_cgindex);
    }

    return 0;
}

int32_t Player::onSocialUseWeiXinCard(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int16_t v3 = m_cgindex;
    int8_t v4 = m_connid;
    GameService* v5 = Answer::Singleton<GameService>::instance();

    MemChrBag item;
    int8_t Type = Answer::NetPacket::readInt8(inPacket);

    return m_extCharBag.UseWeiXinCard(Type);
}

int32_t Player::onSocialUseExchangeCode(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t v2 = 0;
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int8_t v6 = connId;
    GameService* v7 = Answer::Singleton<GameService>::instance();
    int16_t cgindex = m_cgindex;
    int8_t v9 = connId;

    std::string code = Answer::NetPacket::readString(inPacket);
    return 0;
}

void Player::sendLoginInfo()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int32_t Record = getRecord(0);
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    int8_t ServerType = v4->getServerType();
    CfgData* v6 = Answer::Singleton<CfgData>::instance();
    int32_t ServerStartTime = v6->getServerStartTime();
    CfgData* v8 = Answer::Singleton<CfgData>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, getCid());
        Answer::NetPacket::writeString(packet, &m_name);
        Answer::NetPacket::writeInt8(packet, m_job);
        Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(getLevel()));
        Answer::NetPacket::writeInt64(packet, m_chr.exp);
        Answer::NetPacket::writeInt32(packet, m_Battle);
        GameService::sendPacketTo(v2, connId, m_cgindex, packet);
    }
}

void Player::sendChrInfo()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    FamilyId_t FamilyId = getFamilyId();
    Sex_t Sex = getSex();
    Job_t Job = m_job;
    int16_t v7 = static_cast<int16_t>(getLevel());
    int64_t Exp = m_chr.exp;

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, Cid);
        Answer::NetPacket::writeString(packet, &m_name);
        Answer::NetPacket::writeInt8(packet, Sex);
        Answer::NetPacket::writeInt8(packet, Job);
        Answer::NetPacket::writeInt16(packet, v7);
        Answer::NetPacket::writeInt64(packet, Exp);
        Answer::NetPacket::writeInt64(packet, FamilyId);
        GameService::sendPacketTo(v2, connId, m_cgindex, packet);
    }
}

void Player::sendCharAttrInfo()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int32_t v3 = getLevel();
    int32_t AllMoneyRate = 100;
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt8(packet, m_job);
        Answer::NetPacket::writeInt16(packet, static_cast<int16_t>(v3));
        Answer::NetPacket::writeInt32(packet, m_Battle);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::appendCharAttrInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    for (int32_t i = 1; i <= 49; ++i) {
        int32_t AttrValue = GetAttrValue(static_cast<CObjAttrs::Index_T>(i));
        Answer::NetPacket::writeInt32(packet, AttrValue);
    }
}

void Player::sendAttrAddon(const AttrAddonList *const attrs)
{
    if (!attrs)
        return;

    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    int32_t v4 = static_cast<int32_t>(attrs->size());
    uint32_t WOffset = GameService::getWOffset(v3);

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, v4);
        for (auto& a : *attrs) {
            Answer::NetPacket::writeInt32(packet, a.index);
            Answer::NetPacket::writeInt32(packet, a.value);
        }
        GameService::sendPacketTo(v3, connId, m_cgindex, packet);
    }
}

void Player::sendActionList()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v5 = connId;
    GameService* v6 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, 10);
        for (int32_t i = 0; i < 10; ++i) {
            Answer::NetPacket::writeInt32(packet, m_actions[i].id);
            Answer::NetPacket::writeInt32(packet, m_actions[i].type);
        }
        GameService::sendPacketTo(v6, connId, m_cgindex, packet);
    }
}

void Player::sendAutoFight()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v5 = connId;
    GameService* v6 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeString(packet, &m_autoFight.pick);
        Answer::NetPacket::writeString(packet, &m_autoFight.fight);
        GameService::sendPacketTo(v6, connId, m_cgindex, packet);
    }
}

void Player::sendSystemSetting()
{
    int8_t connId = m_connid;
    GameService* v2 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v2);
    int16_t cgindex = m_cgindex;
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, static_cast<int32_t>(m_systemSettings.size()));
        for (auto& [key, value] : m_systemSettings) {
            Answer::NetPacket::writeInt32(packet, key);
            Answer::NetPacket::writeInt32(packet, value);
        }
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::UpdateKilledByPlayer(CharId_t KillerId)
{
    GameService* v2 = Answer::Singleton<GameService>::instance();
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    int32_t GateIndex = GameService::getGateIndex(v4);
    uint32_t WOffset = GameService::getWOffset(v4);
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, KillerId);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::sendKilledByPlayer(CharId_t cid, const std::string *const name)
{
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int16_t cgindex = m_cgindex;
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, cid);
        Answer::NetPacket::writeString(packet, name);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::sendUpdateSocialPlayerInfo(PlayerInfoIndex index, int64_t value)
{
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    uint32_t WOffset = GameService::getWOffset(v4);
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, static_cast<int32_t>(index));
        Answer::NetPacket::writeInt64(packet, value);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::checkSaveToDB()
{
    GameService* v1 = Answer::Singleton<GameService>::instance();
    if (getTick() - m_lastSaveTick >= m_saveDataTimeCount) {
        if (GameService::getLine(v1) != 9) {
            saveToDB(1, 0, 0);
        }
        m_lastSaveTick = getTick();
    }
}

void Player::checkSyncStatus()
{
    int8_t connId = m_connid;
    GameService* v3 = Answer::Singleton<GameService>::instance();
    CharId_t Cid = getCid();
    int8_t TeamStatus = static_cast<int8_t>(GetTeamStatus());
    int8_t VipType = getVipType();
    int8_t VipLevel = getVipLevel();
    FamilyId_t FamilyId = getFamilyId();
    int8_t FamilyPosition = getFamilyPosition();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt64(packet, Cid);
        Answer::NetPacket::writeInt8(packet, TeamStatus);
        Answer::NetPacket::writeInt8(packet, VipType);
        Answer::NetPacket::writeInt8(packet, VipLevel);
        Answer::NetPacket::writeInt64(packet, FamilyId);
        Answer::NetPacket::writeInt8(packet, FamilyPosition);
        GameService::sendPacketTo(v3, connId, m_cgindex, packet);
    }
}

void Player::checkPool()
{
    Dungeon* pDungeon = nullptr;
    if (m_pMap && m_pMap->GetType() == 3) {
        pDungeon = static_cast<Dungeon*>(m_pMap);
    }

    int64_t DieTick = getDieTick();
    int32_t v6 = 0;
    tm time;
    int64_t curTick = getTick();
    int32_t Now = getNow();

    if (m_lastPoolTick > 0 && curTick - m_lastPoolTick > 5000) {
        // Pool expired
        m_lastPoolTick = 0;
    }
}

void Player::minuteCheck(bool bSend)
{
    int32_t Record = getRecord(0);
    GameService* v4 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int16_t GateIndex = m_cgindex;
    int8_t ConnId = m_connid;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    if (bSend) {
        SendCycleTowerTime();
        SendJingLiValue();
    }

    CheckEnterCycleTower();
    CheckLeaveCycleTower();
    CheckLevelWastelands();
    CheckAddWastelandsValue();
    CheckBeastShrinePoints();
    CheckBeastShrineAutoKick();
}

bool Player::autoUpgradeLevel()
{
    bool upgrade = false;
    int32_t oldLevel = getLevel();

    while (upgradeLevel(true)) {
        upgrade = true;
    }

    if (upgrade) {
        LevelUped(oldLevel, getLevel());
    }

    return upgrade;
}

void Player::LevelUped(int32_t OldLevel, int32_t NewLevel)
{
    int8_t ConnId = m_connid;
    DBService* v4 = Answer::Singleton<DBService>::instance();

    level_up_log stu;
    stu.cid = getCid();
    stu.oldLevel = OldLevel;
    stu.newLevel = NewLevel;
    stu.time = getNow();
    MGLog::log(stu);

    recalcAttr(true, false);
    setNeedSyncSelf();

    if (m_task) {
        m_task->onLevelUp(OldLevel, NewLevel);
    }

    CfgData* v8 = Answer::Singleton<CfgData>::instance();
    int32_t ServerDiffDay = v8->getServerDiffDay(SERVER_TYPE::SVT_NORMAL);
    int32_t v10 = 0;
}

bool Player::upgradeLevel(bool bAuto)
{
    int32_t level = getLevel();
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int64_t max_exp = GetLevelMaxExp();
    int64_t exp = m_chr.exp;
    CfgLevelExp* pCfgLevelExp = v3->getLevelExp(level);
    int32_t maxLevel = v3->getMaxLevel();

    if (level >= maxLevel)
        return false;

    if (exp < max_exp)
        return false;

    m_chr.exp -= max_exp;
    m_level = level + 1;
    SetLevelStartTime();
    return true;
}

int32_t Player::getShiZhuangWeapon()
{
    return m_extShiZhuang.GetWearId(0);
}

int32_t Player::getShiZhuangClothes()
{
    return m_extShiZhuang.GetWearId(1);
}

int32_t Player::getShiZhuangFeet()
{
    return m_extShiZhuang.GetWearId(2);
}

void Player::SetLevelStartTime()
{
    m_chr.level_stay_time = 0;
    m_levelStartTime = getNow();
}

int32_t Player::doTeleportActivity(int32_t aid)
{
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    int32_t id = aid;
    CActivityManager* v5 = Answer::Singleton<CActivityManager>::instance();
    CfgData* v6 = Answer::Singleton<CfgData>::instance();
    int32_t target_mapid = v6->getActivityMapId();
    MapManager* v8 = Answer::Singleton<MapManager>::instance();
    Position pos;
    CfgActivity* activityInfo = v6->getActivity(aid);

    if (activityInfo) {
        target_mapid = activityInfo->getMapId();
        pos = activityInfo->getBirthPos();
    }

    Map* pMap = MapManager::getMap(v8, target_mapid);
    if (!pMap)
        return 10002;

    return switchMap(pMap, pos.x, pos.y, false);
}

void Player::FullHPAndSync()
{
    FillHP(100);
    FillMP(100);
    SetDieTick();
    setNeedSync();
}

void Player::safeRevive()
{
    if (!isAlive()) {
        FillHP(100);
        SetDieTick();
        FillMP(100);
        setNeedSyncSelf();
        broadcastRevive();
    }
}

void Player::onDamageEvent(UnitHandle target, int32_t attackValue)
{
    int32_t RunnerId = target.getRunnerId();
    GameService* v4 = Answer::Singleton<GameService>::instance();
    UnitHandle PetUnit;
    Player* player = nullptr;
    CObjPet* pPet = nullptr;

    // Handle pet damage
    if (m_pet) {
        m_pet->onOwnerDamage(target, attackValue);
    }
}

void Player::onDamagedEvent(UnitHandle launcher, int32_t attackValue)
{
    int32_t RunnerId = launcher.getRunnerId();
    GameService* v4 = Answer::Singleton<GameService>::instance();

    UnitHandle PetUnit;
    FamilyId_t FamilyId = getFamilyId();

    m_extFightChecker.SetInFight(getTick());
    m_extFightChecker.SetInPvP(getTick());

    // Handle pet damaged
    if (m_pet) {
        m_pet->onOwnerDamaged(launcher, attackValue);
    }
}

int32_t Player::OnAskBossInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    CWorldBoss* v5 = Answer::Singleton<CWorldBoss>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int16_t cgindex = m_cgindex;
    int8_t v8 = connId;
    GameService* v9 = Answer::Singleton<GameService>::instance();
    int8_t BossType = Answer::NetPacket::readInt8(inPacket);

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, 1);
        GameService::sendPacketTo(v9, connId, m_cgindex, packet);
    }

    return 0;
}

int32_t Player::OnRequestActivityInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t nActivityId = Answer::NetPacket::readInt32(inPacket);
    CActivityManager* v3 = Answer::Singleton<CActivityManager>::instance();
    v3->SendActivityInfo(this, nActivityId);

    return 0;
}

int32_t Player::OnRequestActivityRankInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t nActivityId = Answer::NetPacket::readInt32(inPacket);
    CActivityManager* v3 = Answer::Singleton<CActivityManager>::instance();
    v3->SendActivityRankInfo(this, nActivityId);

    return 0;
}

int32_t Player::OnActivityWorldBossGuWu(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    int32_t nActivityId = Answer::NetPacket::readInt32(inPacket);
    int8_t Int8 = Answer::NetPacket::readInt8(inPacket);

    CActivityManager* v4 = Answer::Singleton<CActivityManager>::instance();
    return 0;
}

int32_t Player::OnActivityGetDailyReward(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 10002;

    CActivityManager* v3 = Answer::Singleton<CActivityManager>::instance();
    int32_t nActivityId = Answer::NetPacket::readInt32(inPacket);

    v3->GetDailyReward(this, nActivityId);
    return 0;
}

int32_t Player::OnActivityApplyCityWar(Answer::NetPacket *inPacket)
{
    FamilyId_t FamilyId = getFamilyId();
    CActivityManager* v4 = Answer::Singleton<CActivityManager>::instance();
    int8_t nType = Answer::NetPacket::readInt8(inPacket);

    v4->ApplyCityWar(this, nType);
    return 0;
}

void Player::TiShiInfo(int32_t TiShiId, int32_t Pos)
{
    int8_t connId = m_connid;
    GameService* v4 = Answer::Singleton<GameService>::instance();
    uint32_t WOffset = GameService::getWOffset(v4);
    int16_t cgindex = m_cgindex;
    int8_t v7 = connId;
    GameService* v8 = Answer::Singleton<GameService>::instance();

    Answer::NetPacket* packet = Answer::NetPacket::allocPacket(0, 0x14u, 0, 0);
    if (packet) {
        Answer::NetPacket::writeInt32(packet, TiShiId);
        Answer::NetPacket::writeInt32(packet, Pos);
        GameService::sendPacketTo(v8, connId, m_cgindex, packet);
    }
}

void Player::AddDaTiExp(int32_t Index, int8_t Result)
{
    CFestivalDoubleEleven* v3 = Answer::Singleton<CFestivalDoubleEleven>::instance();
    int32_t Level = getLevel();
    int Exp = 10 * Level;

    if (Result == 1) {
        m_chr.exp += Exp;
        autoUpgradeLevel();
    }
}

// ============================================================
// 虚拟函数重写 - 生命周期更新
// ============================================================

int32_t Player::onUpdate(int64_t curTick)
{
    CExtCharDepotManager::OnUpdate(this, curTick);
    return 0;
}
