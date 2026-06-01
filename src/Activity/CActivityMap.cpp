#include "Activity/CActivityMap.h"
#include "Activity/CActivity.h"
#include "Activity/CActivityManager.h"
#include "Activity/MonsterActivity.h"
#include "Activity/PlantActivity.h"
#include "Activity/NpcActivity.h"
#include "Config/CfgData.h"
#include "Config/CfgMap.h"
#include "Config/CfgMapEvent.h"
#include "Config/CfgActivityMonster.h"
#include "Game/GameService.h"
#include "Game/Player.h"
#include "Game/CObjPet.h"
#include "Game/Trap.h"
#include "Map/MapManager.h"
#include "Map/MapRunner.h"
#include "Game/CTimer.h"
#include "Network/NetPacket.h"
#include "Other/Answer.h"
#include "Game/CPoolManager.h"
#include "Other/Logger.h"
#include "Utility/StringUtility.h"
#include <algorithm>

CActivityMap::CActivityMap()
    : m_pActivity(nullptr)
    , m_nStartTick(0)
{
}

CActivityMap::~CActivityMap()
{
    Clear();
}

void CActivityMap::AddActivityNpc(const std::string& Effect)
{
    if (!m_pActivity) return;
    
    std::vector<std::string> strIds;
    StringUtility::split(strIds, Effect, ":|");
    
    int32_t EndTime = m_pActivity->GetLeftTime();
    
    for (const auto& idStr : strIds)
    {
        int32_t v4 = atoi(idStr.c_str());
        
        CfgData* cfgData = CfgData::GetInstance();
        const CfgActivityNpc* pNpcAct = cfgData->getActivityNpc(v4);
        
        if (!pNpcAct) continue;
        
        std::vector<int32_t> regionIds = pNpcAct->region_id;
        if (regionIds.empty()) continue;
        
        std::random_shuffle(regionIds.begin(), regionIds.end());
        
        const CfgMapRegion* pCfgRegion = cfgData->getMapRegion(regionIds[0]);
        if (!pCfgRegion) continue;
        
        int32_t mapid = pCfgRegion->mapid;
        if (mapid != m_cfgmap->id) continue;
        
        for (int32_t i = 0; i < pNpcAct->count; ++i)
        {
            Position pos = getRandomWalkablePositionInRegion(*pCfgRegion);
            if (pos.x <= 0 || pos.y <= 0) continue;
            
            int32_t npc_id = pNpcAct->npc_id;
            const CfgNpc* pNpc = cfgData->getNpc(npc_id);
            if (!pNpc) break;
            
            NpcActivity* npc = new NpcActivity();
            if (npc)
            {
                int32_t Id = m_pActivity->GetId();
                npc->init(pNpc, Id, EndTime);
                addNpc(npc, pos.x, pos.y);
            }
        }
    }
}

bool CActivityMap::CanSitRevive(Player* player)
{
    if (m_pActivity)
    {
        return m_pActivity->CanSitRevive();
    }
    return Map::CanSitRevive(player);
}

int32_t CActivityMap::GetTop10Battle()
{
    std::vector<int32_t> BattleVt;
    
    for (auto& pair : m_CidBattle)
    {
        BattleVt.push_back(pair.second);
    }
    
    std::sort(BattleVt.begin(), BattleVt.end());
    
    int32_t count = 0;
    int64_t total = 0;
    
    for (auto it = BattleVt.rbegin(); it != BattleVt.rend() && count < 10; ++it, ++count)
    {
        total += *it;
    }
    
    if (count <= 0) return 0;
    
    return static_cast<int32_t>(total / count);
}

int32_t CActivityMap::HaveAliveMonster()
{
    int32_t Count = 0;
    
    for (auto pMonster : m_monsters)
    {
        if (pMonster && pMonster->isAlive())
        {
            ++Count;
        }
    }
    
    return Count;
}

int32_t CActivityMap::HaveAlivePet()
{
    int32_t Count = 0;
    
    for (auto pPet : m_pets)
    {
        if (pPet && pPet->isAlive())
        {
            ++Count;
        }
    }
    
    return Count;
}

bool CActivityMap::IsActivityMap()
{
    int32_t mapType = m_cfgmap ? m_cfgmap->type : 0;
    return (mapType == 0) || (m_pActivity != nullptr);
}

bool CActivityMap::OnSitRevive(Player* player)
{
    if (m_pActivity && m_pActivity->IsRuning())
    {
        return m_pActivity->OnSitRevive(player);
    }
    return Map::OnSitRevive(player);
}

void CActivityMap::SendUltimateChallengeInfo(Player* player)
{
    if (!m_pActivity) return;
    
    if (player)
    {
        int8_t ConnId = player->getConnId();
        GameService* gameService = GameService::GetInstance();
        Answer::NetPacket* packet = gameService->popNetpacket(ConnId, static_cast<Answer::PackType>(1), 0x2E26);
        
        if (packet)
        {
            int32_t Id = m_pActivity->GetId();
            packet->writeInt32(Id);
            packet->writeInt32(HaveAliveMonster());
            packet->writeInt32(HaveAlivePet());
            packet->writeInt32(m_pActivity->GetLeftTime());
            packet->setSize(packet->getWOffset());
            
            gameService->sendPacketTo(player->getConnId(), player->getGateIndex(), packet);
        }
    }
    else
    {
        GameService* gameService = GameService::GetInstance();
        Answer::NetPacket* packet = gameService->popNetpacket(0, static_cast<Answer::PackType>(1), 0x2E26);
        
        if (packet)
        {
            packet->writeInt32(m_pActivity->GetId());
            packet->writeInt32(HaveAliveMonster());
            packet->writeInt32(HaveAlivePet());
            packet->writeInt32(m_pActivity->GetLeftTime());
            packet->setSize(packet->getWOffset());
            
            broadcast(packet);
        }
    }
}

bool CActivityMap::SpecialSitRevive(Player* player)
{
    if (m_pActivity)
    {
        return m_pActivity->SpecialSitRevive();
    }
    return Map::SpecialSitRevive(player);
}

void CActivityMap::AddMonsterHPEventInfo(MonsterActivity* pMonster, const CfgMapEvent* mapEvent)
{
    if (!pMonster || !mapEvent) return;
    
    if (mapEvent->trigger_type == 8 && 
        mapEvent->trigger_param.size() >= 4 &&
        mapEvent->trigger_param[0] == 2)
    {
        int32_t monsterId = mapEvent->trigger_param[1];
        if (monsterId == pMonster->GetActivityMonsterId())
        {
            int32_t minhp = mapEvent->trigger_param[2];
            int32_t maxhp = mapEvent->trigger_param[3];
            
            if (minhp >= 0 && maxhp > 0)
            {
                MonsterActivityHPEvent hpEvent;
                hpEvent.id = mapEvent->trigger_id;
                hpEvent.minhp = minhp;
                hpEvent.maxhp = maxhp;
                pMonster->addEventHp(&hpEvent);
            }
        }
    }
}

void CActivityMap::AddPlayer(Player* player, int32_t x, int32_t y)
{
    sendEnterMap(player, x, y);
    
    if (m_pActivity)
    {
        m_pActivity->OnPlayerEnter(player);
    }
    
    if (m_cfgmap && m_cfgmap->param == 5)
    {
        SendUltimateChallengeInfo(player);
    }
    
    int64_t cid = player->getCid();
    player->CalBattle();
    m_CidBattle[cid] = 0;
}

bool CActivityMap::CanEnter(Player* player)
{
    if (!player) return false;
    
    if (m_pActivity)
    {
        return m_pActivity->CanEnter(player, this);
    }
    
    if (m_cfgmap && (m_cfgmap->type & 1) != 0)
    {
        return true;
    }
    
    return Map::CanEnter(player);
}

void CActivityMap::CheckAddMonster(int64_t curTick)
{
    auto it = m_waitMonster.begin();
    while (it != m_waitMonster.end())
    {
        if (FlashMonster(*it, curTick))
        {
            ++it;
        }
        else
        {
            it = m_waitMonster.erase(it);
        }
    }
}

void CActivityMap::CheckEvent(CfgMapEvent* mapEvent, int64_t curTick)
{
    if (!mapEvent || !mapEvent->IsOpen() || mapEvent->IsDone()) return;
    
    switch (mapEvent->trigger_type)
    {
    case 1:  // 时间触发
        if (!mapEvent->trigger_param.empty())
        {
            int64_t elapsed = getTick() - mapEvent->nOpenTime;
            if (elapsed >= 1000 * (mapEvent->trigger_param[0]))
            {
                TriggerEvent(mapEvent, nullptr);
            }
        }
        break;
        
    case 2:  // 怪物波次触发
        if (!mapEvent->trigger_param.empty())
        {
            int32_t waveId = mapEvent->trigger_param[0];
            auto it = m_monsterWave.find(waveId);
            if (it != m_monsterWave.end() && !it->second.alive)
            {
                TriggerEvent(mapEvent, nullptr);
            }
        }
        break;
        
    case 3:  // 数量触发
        // 实现数量检查逻辑
        break;
        
    case 4:  // 波次存活时间触发
        if (mapEvent->trigger_param.size() >= 2)
        {
            int32_t waveId = mapEvent->trigger_param[0];
            auto it = m_monsterWave.find(waveId);
            if (it != m_monsterWave.end())
            {
                int64_t elapsed = getTick() - it->second.startTime;
                if (elapsed <= 1000 * (mapEvent->trigger_param[1]))
                {
                    TriggerEvent(mapEvent, nullptr);
                }
            }
        }
        break;
        
    case 7:  // 区域触发
        if (mapEvent->trigger_param.size() >= 4)
        {
            Position ps1 = {mapEvent->trigger_param[0], mapEvent->trigger_param[1]};
            Position ps2 = {mapEvent->trigger_param[2], mapEvent->trigger_param[3]};
            
            for (auto player : m_players)
            {
                if (player && player->isInRectangle(ps1, ps2))
                {
                    TriggerEvent(mapEvent, player);
                }
            }
        }
        break;
        
    default:
        break;
    }
}

void CActivityMap::CheckEvents(int64_t curTick)
{
    for (auto& mapEvent : m_events)
    {
        CheckEvent(&mapEvent, curTick);
    }
}

void CActivityMap::Clear()
{
    IMapEvent::Clear();
    m_lstStack.clear();
    m_nStartTick = 0;
    m_monsterWave.clear();
    m_waitMonster.clear();
    m_CidBattle.clear();
    
    // 清除活动怪物
    for (auto pMonster : m_actMonsters)
    {
        if (pMonster)
        {
            pMonster->remove();
        }
    }
    m_actMonsters.clear();
    
    // 清除活动植物
    for (auto plant : m_actPlants)
    {
        if (plant)
        {
            removePlant(plant->getId());
            CPoolManager::GetInstance()->Push(plant);
        }
    }
    m_actPlants.clear();
    
    // 清除陷阱
    for (auto pTrap : m_traps)
    {
        if (pTrap)
        {
            pTrap->broadcastLeaveMap();
            CPoolManager::GetInstance()->Push(pTrap);
        }
    }
    m_traps.clear();
}

bool CActivityMap::FlashMonster(const MonsterWait& waitMonster, int64_t curTick)
{
    if (!m_pActivity) return false;
    
    CPoolManager* poolMgr = CPoolManager::GetInstance();
    MonsterActivity* pMonster = poolMgr->Pop<MonsterActivity>();
    
    if (!pMonster) return false;
    
    CfgData* cfgData = CfgData::GetInstance();
    const CfgActivityMonster* pCfgActivityMonster = cfgData->getActivityMonster(waitMonster.id);
    
    if (!pCfgActivityMonster)
    {
        poolMgr->Push(pMonster);
        return false;
    }
    
    const CfgMonster* pCfgMonster = cfgData->getMonster(pCfgActivityMonster->mid);
    if (!pCfgMonster)
    {
        poolMgr->Push(pMonster);
        return false;
    }
    
    CfgMapMonster cfgmapmonster;
    memset(&cfgmapmonster, 0, sizeof(cfgmapmonster));
    cfgmapmonster.id = pCfgActivityMonster->id;
    cfgmapmonster.mapid = m_cfgmap ? m_cfgmap->id : 0;
    cfgmapmonster.monsterid = pCfgActivityMonster->mid;
    cfgmapmonster.x = waitMonster.x;
    cfgmapmonster.y = waitMonster.y;
    cfgmapmonster.side = pCfgActivityMonster->side;
    
    // 随机位置
    if (!pCfgActivityMonster->road.empty())
    {
        std::vector<Position> vPos;
        for (const auto& pos : pCfgActivityMonster->road)
        {
            vPos.push_back(pos);
        }
        std::random_shuffle(vPos.begin(), vPos.end());
        cfgmapmonster.x = vPos[0].x;
        cfgmapmonster.y = vPos[0].y;
    }
    
    CfgMonster cfgMonster = *pCfgMonster;
    m_pActivity->AdjustMonsterAttr(&cfgMonster, 0);
    
    pMonster->init(this, pCfgActivityMonster, &cfgMonster, &cfgmapmonster);
    m_pActivity->OnMonsterAdd(pMonster);
    
    addMonster(pMonster, cfgmapmonster.x, cfgmapmonster.y);
    AddMonsterHPEventInfo(pMonster, nullptr);
    pMonster->runOnRoad();
    
    auto it = m_monsterWave.find(pCfgActivityMonster->wave);
    if (it != m_monsterWave.end())
    {
        it->second.count++;
    }
    else
    {
        MonsterWave monsterWave;
        monsterWave.count = 1;
        monsterWave.escaped = 0;
        monsterWave.alive = 1;
        monsterWave.startTime = getTick();
        m_monsterWave[pCfgActivityMonster->wave] = monsterWave;
    }
    
    m_actMonsters.push_back(pMonster);
    return true;
}

void CActivityMap::GenerateMonster(const std::string& str)
{
    std::vector<std::string> strIds;
    StringUtility::split(strIds, str, ":|");
    
    for (const auto& idStr : strIds)
    {
        MonsterWait stu;
        memset(&stu, 0, sizeof(stu));
        stu.id = atoi(idStr.c_str());
        m_waitMonster.push_back(stu);
    }
}

int32_t CActivityMap::GetPkMode()
{
    int32_t pkMode = 0;
    
    if (m_pActivity)
    {
        pkMode = m_pActivity->GetPkMode();
    }
    
    if (pkMode == 0)
    {
        return Map::GetPkMode();
    }
    
    return pkMode;
}

int32_t CActivityMap::GetReive(Player* player)
{
    if (m_pActivity && m_pActivity->GetType() != 2 && m_pActivity->GetType() != 14)
    {
        return m_pActivity->GetRevive(player);
    }
    
    return Map::GetReive(player);
}

int64_t CActivityMap::GetTick()
{
    return Map::getTick();
}

void CActivityMap::Init(CMapRunner* pRunner, const CfgMap* const cfgmap)
{
    Map::init(pRunner, cfgmap);
    Clear();
}

void CActivityMap::OnActivityStart(CActivity* pActivity)
{
    if (!pActivity) return;
    
    m_pActivity = pActivity;
    int64_t Tick = getTick();
    int32_t MapId = m_cfgmap ? m_cfgmap->id : 0;
    int32_t Id = pActivity->GetId();
    
    CfgData* cfgData = CfgData::GetInstance();
    const std::list<CfgMapEvent>* ActivityEvents = cfgData->getActivityEvents(Id, MapId);
    
    if (ActivityEvents)
    {
        IMapEvent::init(ActivityEvents, Tick);
    }
    
    CTimer* timer = CTimer::GetInstance();
    m_nStartTick = timer->GetTick();
}

void CActivityMap::OnBeginGather(PlantActivity* plant, Player* player)
{
    if (m_pActivity)
    {
        m_pActivity->OnBeginGather(plant, player);
    }
}

void CActivityMap::OnMonsterDamaged(MonsterActivity* pMonster, Player* player, int64_t damage, const LauncherInfo* launcher)
{
    if (!m_pActivity || !m_pActivity->IsRuning()) return;
    
    Player* pAttacker = nullptr;
    
    if (launcher && launcher->type == 1)
    {
        int32_t RunnerId = GetRunnerId();
        GameService* gameService = GameService::GetInstance();
        pAttacker = gameService->getPlayer(launcher->id, RunnerId, 1);
    }
    else if (launcher && launcher->type == 4)
    {
        int32_t RunnerId = GetRunnerId();
        GameService* gameService = GameService::GetInstance();
        CObjPet* pPet = gameService->getPet(launcher->id, RunnerId);
        if (pPet)
        {
            pAttacker = pPet->GetPlayer();
        }
    }
    
    if (pAttacker)
    {
        m_pActivity->OnMonsterDamaged(pMonster, static_cast<uint32_t>(damage), pAttacker);
    }
}

void CActivityMap::OnMonsterDie(MonsterActivity* pMonster, Player* killer)
{
    Map::onMonsterDie(pMonster, killer, false);
    
    MonsterActivity* pMonsterAct = dynamic_cast<MonsterActivity*>(pMonster);
    
    if (m_pActivity && pMonsterAct)
    {
        m_pActivity->OnMonsterDie(pMonsterAct, killer);
        
        if (m_cfgmap && m_cfgmap->param == 5 && pMonsterAct->IsBoss())
        {
            SendUltimateChallengeInfo(nullptr);
        }
    }
}

void CActivityMap::OnMonsterHPEvent(MonsterActivity* pMonster, int32_t eventId)
{
    if (!pMonster) return;
    
    for (auto& mapEvent : m_events)
    {
        if (mapEvent.IsOpen() && !mapEvent.IsDone() && mapEvent.trigger_id == eventId)
        {
            TriggerEvent(&mapEvent, pMonster);
        }
    }
}

void CActivityMap::OnPlantGather(PlantActivity* plant, Player* player)
{
    if (!plant || !m_pActivity) return;
    
    m_pActivity->OnPlantGather(plant, player);
    
    for (auto& mapEvent : m_events)
    {
        if (!mapEvent.IsOpen() || mapEvent.IsDone()) continue;
        
        if (mapEvent.trigger_type == 6 && !mapEvent.trigger_param.empty())
        {
            int32_t plantId = mapEvent.trigger_param[0];
            if (plantId == plant->getPlantId())
            {
                TriggerEvent(&mapEvent, nullptr);
            }
        }
    }
}

void CActivityMap::OnPlayerKilled(Player* killer, Player* victim)
{
    if (m_pActivity && m_pActivity->IsRuning())
    {
        m_pActivity->OnPlayerKilled(victim, killer);
    }
}

void CActivityMap::OnPlayerRevive(Player* player, bool bSafe)
{
    if (m_pActivity && m_pActivity->IsRuning())
    {
        m_pActivity->OnPlayerRevive(player, bSafe);
    }
}

void CActivityMap::RemovePlayer(Player* player, bool islogout)
{
    if (m_pActivity)
    {
        m_pActivity->OnPlayerLeave(player, islogout);
    }
    
    broadcastLeaveMap(player);
    m_players.remove(player);
}

void CActivityMap::TriggerEvent(CfgMapEvent* mapEvent, void* target)
{
    // 触发地图事件的具体实现
    if (mapEvent)
    {
        mapEvent->SetDone(true);
        // 执行事件效果
    }
}

void CActivityMap::Update(int64_t curTick)
{
    Map::update();
    
    if (!m_pActivity) return;
    
    m_pActivity->OnMapUpdate(this);
    
    if (m_pActivity->IsRuning())
    {
        CTimer* timer = CTimer::GetInstance();
        curTick = timer->GetTick();
        
        CheckEvents(curTick);
        CheckAddMonster(curTick);
    }
}