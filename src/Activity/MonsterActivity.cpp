#include "Activity/MonsterActivity.h"
#include "Activity/CActivityMap.h"
#include "Activity/CActivity.h"
#include "Config/CfgData.h"
#include "Config/CfgActivityMonster.h"
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Map/Map.h"
#include "Game/CTimer.h"
#include "Game/CPoolManager.h"
#include "Other/Buff.h"
#include "Other/Logger.h"
#include "Other/Answer.h"
#include <cmath>

MonsterActivity::MonsterActivity()
    : m_pActivityMap(nullptr)
    , m_nOwner(0)
    , m_nFamilyMoney(0)
    , m_nFamilyMoneyTick(0)
    , m_nFamilyId(0)
{
    memset(&m_cfgActivityMoster, 0, sizeof(m_cfgActivityMoster));
    memset(&m_killer, 0, sizeof(m_killer));
}

MonsterActivity::~MonsterActivity()
{
    m_road.clear();
    m_hpEvents.clear();
}

void MonsterActivity::AddFamilyMoney(int32_t money)
{
    if (!isAlive()) return;
    if (m_nFamilyId == 0) return;
    
    m_nFamilyMoney += money;
}

int32_t MonsterActivity::GetFamilyMoney() const
{
    return m_nFamilyMoney;
}

void MonsterActivity::addEventHp(const MonsterActivityHPEvent* hpEvent)
{
    if (!hpEvent) return;
    m_hpEvents.push_back(*hpEvent);
}

void MonsterActivity::broadcastBasicData()
{
    if (!m_pMap) return;
    
    int8_t nflag = m_bDie ? 1 : 0;
    
    GameService* gameService = GameService::GetInstance();
    Answer::NetPacket* packet = gameService->popNetpacket(Answer::PackType::PACK_DISPATCH, 0x2739);
    
    if (!packet) return;
    
    packet->writeInt64(getId());
    packet->writeInt8(static_cast<int8_t>(getType()));
    packet->writeInt64(GetMaxHP());
    packet->writeInt64(GetMaxHP());
    packet->writeInt16(GetLevel());
    packet->writeInt8(nflag);
    packet->writeInt32(GetMonsterId());
    packet->setSize(packet->getWOffset());
}

void MonsterActivity::checkFamilyMoney(int64_t curTick)
{
    if (curTick - m_nFamilyMoneyTick <= 4999) return;
    if (!isAlive()) return;
    if (m_nFamilyMoney <= 0) return;
    
    int64_t curHp = GetMaxHP();
    int64_t maxHp = GetMaxHP();
    
    if (curHp == maxHp) return;
    
    int64_t needHp = maxHp - curHp;
    if (needHp > 10000) needHp = 10000;
    
    int32_t v4 = static_cast<int32_t>(needHp / 100);
    if (needHp % 100 != 0) v4++;
    
    int32_t costMoney = v4;
    if (m_nFamilyMoney < costMoney)
    {
        costMoney = m_nFamilyMoney;
    }
    
    m_nFamilyMoney -= costMoney;
    AddHP(static_cast<int32_t>(100 * costMoney));
    m_nFamilyMoneyTick = curTick;
}

void MonsterActivity::checkHpEvent(int64_t curTick)
{
    if (!m_pActivityMap) return;
    
    int64_t curHp = GetMaxHP();
    int64_t maxHp = GetMaxHP();
    
    int32_t hpPercent = static_cast<int32_t>(std::round(static_cast<double>(curHp) * 100.0 / static_cast<double>(maxHp)));
    
    for (const auto& hpEvent : m_hpEvents)
    {
        if (hpEvent.minhp <= hpPercent && hpEvent.maxhp > hpPercent)
        {
            m_pActivityMap->OnMonsterHPEvent(this, hpEvent.id);
        }
    }
}

void MonsterActivity::destroy()
{
    CPoolManager* poolMgr = CPoolManager::GetInstance();
    poolMgr->Push(this);
}

void MonsterActivity::die()
{
    Monster::die();
    
    if (m_pActivityMap)
    {
        m_pActivityMap->OnMonsterDie(this, nullptr);
    }
    
    m_nFamilyMoney = 0;
}

void MonsterActivity::init(CActivityMap* pActivityMap, 
                           const CfgActivityMonster* cfgActivityMonster,
                           const CfgMonster* cfgmonster,
                           const CfgMapMonster* cfgmapmonster)
{
    if (!pActivityMap) return;
    
    m_pActivityMap = pActivityMap;
    m_cfgActivityMoster = *cfgActivityMonster;
    
    // 设置路线
    if (!m_cfgActivityMoster.road.empty())
    {
        m_road = m_cfgActivityMoster.road;
    }
    
    // 设置生命周期
    if (m_cfgActivityMoster.left > 0)
    {
        int32_t lifeEndTime = getNow() + m_cfgActivityMoster.left;
        SetLifeTime(static_cast<int64_t>(lifeEndTime));
    }
    
    // 获取世界等级
    GameService* gameService = GameService::GetInstance();
    int32_t WorldLevel = gameService->getWorldLevel();
    
    // 获取属性加成
    CfgData* cfgData = CfgData::GetInstance();
    std::vector<AttrAddon> vAttrAddon;
    (void)WorldLevel;
    
    // CfgMonster* pMonster = const_cast<CfgMonster*>(cfgmonster);
    // Monster::init(pMonster, cfgmapmonster, MonsterState::MS_STAND, &vAttrAddon);
    
    // 添加Buff
    if (m_cfgActivityMoster.buff > 0)
    {
        const CfgBuff* pCfgBuff = cfgData->getBuff(m_cfgActivityMoster.buff);
        if (pCfgBuff)
        {
            LauncherInfo launcher;
            launcher.id = getId();
            launcher.type = 3;  // 怪物类型
            
            Buff* pBuff = new Buff(this, pCfgBuff, &launcher, 3, 0);
            if (pBuff)
            {
                addBuff(pBuff);
                SetSyncTime(100);
            }
        }
    }
}

void MonsterActivity::onArriveTarget()
{
    if (GetState() != MonsterState::MS_RUN_ON_ROAD) return;
    if (m_road.empty()) return;
    
    Position curPos = m_currentTile;
    Position tarPos = m_road.front();
    m_road.pop_front();
    
    // 跳过当前位置
    while (!m_road.empty() && curPos == tarPos)
    {
        tarPos = m_road.front();
        m_road.pop_front();
    }
    
    if (!m_road.empty())
    {
        setTargetTile(tarPos.x, tarPos.y);
    }
    else
    {
        setState(MonsterState::MS_STAND);
        
        if (m_pActivityMap)
        {
            m_pActivityMap->OnMonsterArriveRoadEnd(this);
        }
    }
}

void MonsterActivity::postDamage(int32_t damage, UnitHandle launcher, int32_t Mid)
{
    Monster::postDamage(damage, launcher, Mid);
}

bool MonsterActivity::refresh()
{
    if (!m_pActivityMap) return false;
    
    int64_t curTick = m_pActivityMap->getTick();
    
    checkHpEvent(curTick);
    checkFamilyMoney(curTick);
    
    return Monster::refresh();
}

void MonsterActivity::remove()
{
    GameService* gameService = GameService::GetInstance();
    gameService->removeMonster(this);
    
    // leaveMap();
    setDelFlg();
}

void MonsterActivity::reset()
{
    Monster::reset();
    
    m_pActivityMap = nullptr;
    m_hpEvents.clear();
    m_road.clear();
    m_nFamilyId = 0;
    m_nFamilyMoney = 0;
    m_nFamilyMoneyTick = 0;
    m_nOwner = 0;
}

void MonsterActivity::runOnRoad()
{
    if (m_road.empty()) return;
    
    ClearTarget();
    
    Position curPos = m_currentTile;
    Position tarPos = m_road.front();
    m_road.pop_front();
    
    // 跳过当前位置
    while (!m_road.empty() && curPos == tarPos)
    {
        tarPos = m_road.front();
        m_road.pop_front();
    }
    
    setTargetTile(tarPos.x, tarPos.y);
    setState(MonsterState::MS_RUN_ON_ROAD);
}

void MonsterActivity::setKiller(int64_t nKiller)
{
    m_killer.id = nKiller;
    m_killer.type = 1;
}

void MonsterActivity::setFamilyId(int64_t nFamilyId)
{
    m_nFamilyId = nFamilyId;
}

int64_t MonsterActivity::getFamilyId() const
{
    return m_nFamilyId;
}

int32_t MonsterActivity::GetActivityMonsterId() const
{
    return m_cfgActivityMoster.id;
}

bool MonsterActivity::IsBoss() const
{
    return false;
}

int32_t MonsterActivity::GetBossSign() const
{
    return 0;
}

void MonsterActivity::SetSyncTime(int32_t ms)
{
    // 设置同步时间间隔
    m_nSyncInterval = ms;
}