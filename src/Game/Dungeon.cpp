#include "common.h"
#include "Game/Dungeon.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/DBService.h"
#include "Game/MapManager.h"
#include "Game/CPoolManager.h"
#include "Config/CfgData.h"
#include "Config/CfgMapEvent.h"
#include "Other/MonsterDungeon.h"
#include "Other/PlantDungeon.h"
#include "Other/NpcDungeon.h"
#include "Other/MonsterWave.h"
#include "Other/CLibraryItem.h"
#include "Mem/MemChrBag.h"
#include "Config/CfgBuff.h"

#include <cstring>
#include <algorithm>
#include <cmath>
#include <sstream>

// ============================================================
// 构造 / 析构
// ============================================================

Dungeon::Dungeon()
    : Map()
    , IMapEvent()
    , m_nId(0)
    , m_state(DungeonState::DS_FREE)
    , m_nStartTime(0)
    , m_nSummStart(0)
    , m_Random(0)
    , m_MaxRanomParam(0)
{
    std::memset(&m_finshInfo, 0, sizeof(m_finshInfo));
}

Dungeon::~Dungeon()
{
}

// ============================================================
// init - 初始化副本
// ============================================================

void Dungeon::init(CMapRunner *pRunner, const CfgDungeon *cfgDungeon, const CfgMap *cfgmap, int32_t nId)
{
    Map::init(pRunner, cfgmap);
    int64_t Tick = Map::getTick();
    int32_t id = cfgDungeon->id;
    CfgData *v7 = Answer::Singleton<CfgData>::instance();
    const CfgMapEventList *DungeonEvent = CfgData::getDungeonEvent(v7, id);
    IMapEvent::init(DungeonEvent, Tick);
    m_cfgDungeon = *cfgDungeon;
    m_state = DungeonState::DS_FREE;
    m_nId = nId;
    Answer::Random *v9 = Answer::Singleton<Answer::Random>::instance();
    m_Random = Answer::Random::generate(v9, 1, 5);
}

// ============================================================
// 玩家相关
// ============================================================

void Dungeon::addPlayer(Player *player, int32_t x, int32_t y)
{
    if (!player)
        return;

    Map::addPlayer(player, x, y);
    Dungeon::initPlayerInfo(player);
    Dungeon::broadcastDungeonInfo();

    if (m_cfgDungeon.task_id > 0)
    {
        int32_t task_id = m_cfgDungeon.task_id;
        ChrTask *Task = Player::GetTask(player);
        ChrTask::receive(Task, task_id);
    }

    switch (m_cfgDungeon.type)
    {
        case 12: // 团队副本 - 伤害统计
        {
            bool found = false;
            for (auto &dmg : m_damages)
            {
                if (dmg.cid == Player::getCid(player))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                DamageSum dmg;
                dmg.cid = Player::getCid(player);
                Player::getName(player, dmg.name);
                dmg.damage = 0;
                m_damages.push_back(dmg);
            }
            m_needBroadcastDamage = true;
            break;
        }
        case 17: // 全服公告类副本
        {
            if (m_cfgDungeon.star > 11)
            {
                int8_t ConnId = Player::getConnId(player);
                GameService *v8 = Answer::Singleton<GameService>::instance();
                Answer::NetPacket *packet = GameService::popNetpacket(v8, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                if (!packet)
                    return;
                Answer::NetPacket::writeInt32(packet, 1);
                std::string val;
                Player::getName(player, val);
                Answer::NetPacket::writeUTF8(packet, &val);
                CharId_t v9 = Player::getCid(player);
                Answer::NetPacket::writeInt64(packet, v9);
                Answer::NetPacket::writeInt32(packet, m_cfgDungeon.star);
                uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                Answer::NetPacket::setSize(packet, WOffset);
                int8_t v11 = Player::getConnId(player);
                GameService *v12 = Answer::Singleton<GameService>::instance();
                GameService::worldBroadcast(v12, v11, packet);
            }
            break;
        }
        case 35: // 宝库副本 - 随机位置信息
        {
            int8_t v13 = Player::getConnId(player);
            GameService *v14 = Answer::Singleton<GameService>::instance();
            Answer::NetPacket *packet_0 = GameService::popNetpacket(v14, v13, Answer::PackType::PACK_DISPATCH, 0x2D3C);
            if (!packet_0)
                return;
            Answer::NetPacket::writeInt32(packet_0, m_cfgDungeon.id);
            Answer::NetPacket::writeInt32(packet_0, m_Random);
            int32_t v15 = (int32_t)m_RandomList.size();
            Answer::NetPacket::writeInt32(packet_0, v15);
            for (auto &idx : m_RandomList)
            {
                Answer::NetPacket::writeInt32(packet_0, idx);
            }
            uint32_t v17 = Answer::NetPacket::getWOffset(packet_0);
            Answer::NetPacket::setSize(packet_0, v17);
            int16_t GateIndex = Player::getGateIndex(player);
            int8_t v19 = Player::getConnId(player);
            GameService *v20 = Answer::Singleton<GameService>::instance();
            GameService::sendPacketTo(v20, v19, GateIndex, packet_0);
            break;
        }
    }

    // 副本Buff处理
    if (m_cfgDungeon.BuffId > 0)
    {
        int32_t BuffId = m_cfgDungeon.BuffId;
        CfgData *v22 = Answer::Singleton<CfgData>::instance();
        CfgBuff *cfgBuff = CfgData::getBuff(v22, BuffId);
        if (cfgBuff)
        {
            UnitHandle launcher;
            launcher.id = Player::getCid(player);
            launcher.type = 0;
            Buff *buff = new Buff();
            Buff::Buff(buff, player, cfgBuff, &launcher, 2, 0);
            Unit::addBuff(player, buff);
        }
    }

    // 更新任务
    int32_t id = m_cfgDungeon.id;
    ChrTask *v26 = Player::GetTask(player);
    ChrTask::updateTaskCount(v26, 30, id);
}

void Dungeon::removePlayer(Player *player, bool islogout)
{
    if (player && Player::getCid(player) == m_TeamLeaderId)
    {
        for (auto &event : m_events)
        {
            if (CfgMapEvent::isOpen(&event) && !CfgMapEvent::isDone(&event)
                && getState() == DungeonState::DS_RUNNING
                && event.trigger_type == 10)
            {
                // 触发队长离开事件
                triggerEvent(&event, player);
            }
        }
    }
    Map::removePlayer(player, islogout);
}

bool Dungeon::onEndDungeon(Player *player)
{
    if (!player || GetLeader() != player)
        return false;
    if (m_cfgDungeon.type != 15 && m_cfgDungeon.type != 23)
        return false;
    if (m_state != DungeonState::DS_RUNNING)
        return false;
    Dungeon::win(0);
    return true;
}

void Dungeon::initPlayerInfo(Player *player)
{
    // 初始化玩家在副本中的Buff等信息
    if (m_cfgDungeon.player_buff > 0)
    {
        CfgData *v3 = Answer::Singleton<CfgData>::instance();
        CfgBuff *cfgBuff = CfgData::getBuff(v3, m_cfgDungeon.player_buff);
        if (cfgBuff)
        {
            UnitHandle launcher;
            launcher.id = Player::getCid(player);
            launcher.type = 0;
            Buff *buff = new Buff();
            Buff::Buff(buff, player, cfgBuff, &launcher, 2, 0);
            Unit::addBuff(player, buff);
        }
    }
}

void Dungeon::onPlayerLeave(Player *player)
{
    if (player && m_cfgDungeon.task_id > 0)
    {
        ChrTask *Task = Player::GetTask(player);
        ChrTask::CleanTaskType(Task, 38);
    }
}

void Dungeon::OnPickItem(Player *player, const MemChrBag *item)
{
    if (player && m_cfgDungeon.type == 26)
    {
        // 添加拾取物品到列表
        m_vPickItems.push_back(*item);
    }
}

void Dungeon::InitTeamMember(const CharIdList *lst)
{
    m_memberList = *lst;
}

void Dungeon::AddPlayerDamage(CharId_t cid, int32_t attackValue)
{
    if (m_cfgDungeon.type == 12)
    {
        for (auto &dmg : m_damages)
        {
            if (dmg.cid == cid)
            {
                dmg.damage += attackValue;
                m_needBroadcastDamage = true;
                return;
            }
        }
    }
}

void Dungeon::AddMonsterExp(int32_t nExp)
{
    if (m_cfgDungeon.type == 23)
        m_finshInfo.get_exp += nExp;
}

void Dungeon::AddDungeonTime(int32_t nTime)
{
    m_nAddonTime += nTime;
    Dungeon::broadcastDungeonInfo();
}

PlayerList Dungeon::getPlayers()
{
    return m_players;
}

Player *Dungeon::GetLeader()
{
    if (m_players.empty())
        return nullptr;
    return m_players.front();
}

const Player *Dungeon::GetLeader() const
{
    if (m_players.empty())
        return nullptr;
    return m_players.front();
}

std::string Dungeon::getDungeonName()
{
    return m_cfgDungeon.name;
}

void Dungeon::SetHard(int8_t nHard)
{
    if (nHard > 0 && nHard <= 5)
        m_nHard = nHard;
}

void Dungeon::OpenEvent(int32_t nEvent)
{
    int64_t Tick = Map::getTick();
    IMapEvent::openEvent(nEvent, Tick);
}

// ============================================================
// 状态管理
// ============================================================

void Dungeon::setState(DungeonState state, int32_t param)
{
    m_state = state;
    m_stateTick = Map::getTick();
    m_stateParam = param;
    if (m_state == DungeonState::DS_SUMMON)
    {
        m_nSummStart = m_stateTick;
        Dungeon::sendStartSummon();
    }
}

int32_t Dungeon::GetLeftTime()
{
    switch (m_state)
    {
        case DungeonState::DS_RUNNING:
        {
            int64_t endTick = m_stateTick + 1000 * GetDurationTime();
            return (int32_t)((endTick - Map::getTick()) / 1000);
        }
        case DungeonState::DS_SUMMON:
        case DungeonState::DS_BOSS:
        case DungeonState::DS_FAIL:
        {
            int64_t endTick = m_stateTick + 1000 * m_stateParam;
            return (int32_t)((endTick - Map::getTick()) / 1000);
        }
        case DungeonState::DS_WIN:
        {
            int64_t endTick = m_stateTick + 1000 * m_cfgDungeon.reward_time;
            return (int32_t)((endTick - Map::getTick()) / 1000);
        }
        default:
            return GetDurationTime();
    }
}

Position Dungeon::GetBackPos()
{
    return Position(m_cfgDungeon.backX, m_cfgDungeon.backY);
}

void Dungeon::teamDungeonMemberEnter()
{
    if (m_players.empty())
    {
        GameService *v1 = Answer::Singleton<GameService>::instance();
        GameService::TeamDungeonEnterDungeon(v1, this, &m_memberList);
    }
}

// ============================================================
// 副本主循环
// ============================================================

void Dungeon::update()
{
    if (m_state != DungeonState::DS_FREE)
    {
        int64_t curTick = Map::getTick();

        switch (m_state)
        {
            case DungeonState::DS_WAIT:
            {
                if (curTick - m_stateTick >= 1000LL * m_stateParam)
                {
                    if (m_cfgDungeon.type == 12)
                        Dungeon::teamDungeonMemberEnter();
                    Dungeon::setState(DungeonState::DS_RUNNING, 0);
                    if (!m_waitEvent.empty())
                    {
                        int64_t Tick = Map::getTick();
                        std::list<int> eventsToOpen = m_waitEvent;
                        IMapEvent::openEvents(&eventsToOpen.front(), (int32_t)eventsToOpen.size());
                        m_waitEvent.clear();
                    }
                }
                break;
            }
            case DungeonState::DS_RUNNING:
            {
                Dungeon::checkAddMonster(curTick);
                Map::update();
                int64_t elapsed = curTick - m_stateTick;
                if (elapsed >= 1000LL * GetDurationTime())
                {
                    Dungeon::clearMonster();
                    if (getDungeonType() == 19)
                    {
                        Dungeon::win(0);
                    }
                    else
                    {
                        Dungeon::fail(0);
                    }
                }
                break;
            }
            case DungeonState::DS_SUMMON:
            {
                if (curTick - m_stateTick >= 1000LL * m_stateParam)
                {
                    Dungeon::win(0);
                }
                else
                {
                    Dungeon::checkSummonBoss();
                }
                break;
            }
            case DungeonState::DS_BOSS:
            {
                if (curTick - m_stateTick >= 1000LL * m_stateParam)
                {
                    Dungeon::win(0);
                }
                else
                {
                    Dungeon::checkSummonBoss();
                }
                break;
            }
            case DungeonState::DS_WIN:
            {
                if (curTick - m_stateTick >= 1000LL * m_cfgDungeon.reward_time)
                {
                    PlayerList players = m_players;
                    for (auto *player : players)
                    {
                        if (player)
                        {
                            if ((getDungeonType() == 15 && Player::GetSysSettingInfo(player, 5))
                                || (getDungeonType() == 23 && Player::GetSysSettingInfo(player, 6)))
                            {
                                Dungeon::GetReward(player, 2, true);
                            }
                            else
                            {
                                Dungeon::GetReward(player, 1, false);
                            }
                        }
                    }
                    Dungeon::setState(DungeonState::DS_RESET, 0);
                }
                break;
            }
            case DungeonState::DS_FAIL:
            {
                if (curTick - m_stateTick >= 1000LL * m_stateParam)
                    Dungeon::setState(DungeonState::DS_RESET, 0);
                break;
            }
            case DungeonState::DS_RESET:
            {
                if (!m_players.empty())
                {
                    PlayerList players = m_players;
                    for (auto *player : players)
                    {
                        if (player)
                        {
                            Player::leaveDungeon(player);
                            Dungeon::sendDungeonResult(player, m_stateParam);
                        }
                    }
                }
                Map::reset();
                int32_t RunnerId = Map::GetRunnerId();
                MapManager *v5 = Answer::Singleton<MapManager>::instance();
                MapManager::PostMsg(v5, RunnerId, GameMsgCode::GMC_DEL_DUNGEON, this, 0, 0, 0, 0);
                break;
            }
            default:
                break;
        }
        Dungeon::checkRevive(curTick);
        Dungeon::checkBroadcastDamageList();
        Map::update();
    }
    else
    {
        Map::update();
    }
}

// ============================================================
// start / restart / reset
// ============================================================

void Dungeon::start(bool bFree)
{
    m_bFree = bFree;
    m_state = DungeonState::DS_WAIT;
    m_stateTick = Map::getTick();
    m_stateParam = 5; // 默认5秒等待
    m_bGetReward = false;
    m_Times = 0;
    m_BuyRandomTimes = 0;
    m_RandomTimes = 0;
    m_MaxRanomParam = 0;
    m_RandomMap.clear();
    m_RandomList.clear();
    m_damages.clear();
    m_lSummonBoss.clear();
    m_mSummonRecord.clear();
    m_vPickItems.clear();
    m_LibraryRewardVector.clear();
    m_nAddonTime = 0;
    m_finshInfo.get_exp = 0;
    m_nHard = 0;
    m_needBroadcastDamage = false;
    m_dRatio = 1.0;
}

void Dungeon::restart(bool bFree)
{
    // 清理怪物
    Dungeon::clearMonster();
    // 清理植物
    for (auto it = m_plants.begin(); it != m_plants.end(); )
    {
        PlantDungeon *plant = dynamic_cast<PlantDungeon *>(*it);
        if (plant)
        {
            CPoolManager *v8 = Answer::Singleton<CPoolManager>::instance();
            CPoolManager::FreeObject(v8, plant);
            it = m_plants.erase(it);
        }
        else
        {
            ++it;
        }
    }
    // 重置状态
    Dungeon::start(bFree);
}

void Dungeon::reset()
{
    // 清除所有玩家
    m_players.clear();
    // 清除所有怪物
    for (auto *pMonster : m_monsters)
    {
        MonsterDungeon *v2 = dynamic_cast<MonsterDungeon *>(pMonster);
        if (v2)
        {
            CPoolManager *v5 = Answer::Singleton<CPoolManager>::instance();
            CPoolManager::FreeObject(v5, v2);
        }
    }
    m_monsters.clear();
    // 清除所有植物
    for (auto *pPlant : m_plants)
    {
        PlantDungeon *v7 = dynamic_cast<PlantDungeon *>(pPlant);
        if (v7)
        {
            CPoolManager *v9 = Answer::Singleton<CPoolManager>::instance();
            CPoolManager::FreeObject(v9, v7);
        }
    }
    m_plants.clear();
    // 清除所有陷阱
    for (auto *pTrap : m_traps)
    {
        CPoolManager *v11 = Answer::Singleton<CPoolManager>::instance();
        CPoolManager::FreeObject(v11, pTrap);
    }
    m_traps.clear();
    // 清除所有NPC
    for (auto *pNpc : m_npcs)
    {
        CPoolManager *v13 = Answer::Singleton<CPoolManager>::instance();
        CPoolManager::FreeObject(v13, pNpc);
    }
    m_npcs.clear();
    // 重置随机
    Answer::Random *v14 = Answer::Singleton<Answer::Random>::instance();
    m_Random = Answer::Random::generate(v14, 1, 5);
    m_RandomList.clear();
    m_RandomMap.clear();
    m_waitMonster.clear();
    m_waitTower.clear();
    m_buildTower.clear();
    m_playerReward.clear();
    m_memberList.clear();
    m_damages.clear();
    m_waitEvent.clear();
    m_lSummonBoss.clear();
    m_mSummonRecord.clear();
    m_vPickItems.clear();
    m_LibraryRewardVector.clear();
    m_state = DungeonState::DS_FREE;
    m_stateTick = 0;
    m_nStartTime = 0;
    m_nSummStart = 0;
    m_nAddonTime = 0;
    m_BuyRandomTimes = 0;
    m_RandomTimes = 0;
    m_Times = 0;
    m_MaxRanomParam = 0;
    m_bFree = false;
    m_bGetReward = false;
    m_needBroadcastDamage = false;
    m_dRatio = 1.0;
    std::memset(&m_finshInfo, 0, sizeof(m_finshInfo));
    m_nHard = 0;
}

// ============================================================
// 怪物管理
// ============================================================

void Dungeon::clearMonster()
{
    for (auto it = m_monsters.begin(); it != m_monsters.end(); )
    {
        MonsterDungeon *pMonster = dynamic_cast<MonsterDungeon *>(*it);
        if (pMonster)
        {
            MonsterDungeon::die(pMonster);
            MonsterDungeon::destroy(pMonster);
            CPoolManager *v5 = Answer::Singleton<CPoolManager>::instance();
            CPoolManager::FreeObject(v5, pMonster);
        }
        it = m_monsters.erase(it);
    }
}

void Dungeon::checkAddMonster(int64_t curTick)
{
    auto iter = m_waitMonster.begin();
    while (iter != m_waitMonster.end())
    {
        if (curTick >= iter->startTick)
        {
            if (Dungeon::flashWaitMonster(&(*iter), curTick))
            {
                iter = m_waitMonster.erase(iter);
                continue;
            }
        }
        ++iter;
    }
}

bool Dungeon::flashWaitMonster(MonsterWait *waitAddMonster, int64_t curTick)
{
    int32_t id = waitAddMonster->id;
    CfgData *v4 = Answer::Singleton<CfgData>::instance();
    const CfgMapMonster *cfgMonster = CfgData::getMapMonster(v4, id);
    if (!cfgMonster)
        return true;

    // 生成怪物
    Position pos;
    if (!m_cfgDungeon.stay_position)
    {
        // 随机位置
        pos = Map::getRandomWalkablePosition();
    }
    else
    {
        pos = Position(m_cfgDungeon.x, m_cfgDungeon.y);
    }

    CPoolManager *v8 = Answer::Singleton<CPoolManager>::instance();
    MonsterDungeon *monster = (MonsterDungeon *)CPoolManager::AllocObject(v8, 1);
    if (monster)
    {
        // 初始化并添加怪物
        MonsterDungeon::refresh(monster);
        Dungeon::addMonster(monster, pos.x, pos.y);
        if (waitAddMonster->rewardWave)
        {
            // 奖励波次标记
            MonsterDungeon::setRewardWave(monster, true);
        }
    }
    return true;
}

void Dungeon::generateMonster(const std::string *str, bool rewardWave)
{
    // 解析字符串中的怪物ID列表，以逗号分隔
    std::stringstream ss(*str);
    std::string item;
    while (std::getline(ss, item, ','))
    {
        if (item.empty())
            continue;
        int32_t id = std::stoi(item);
        MonsterWait stu;
        std::memset(&stu, 0, sizeof(stu));
        stu.id = id;
        stu.rewardWave = rewardWave ? 1 : 0;
        stu.startTick = Map::getTick();
        m_waitMonster.push_back(stu);
    }
}

void Dungeon::randomMonster(const std::string *str, bool rewardWave)
{
    // 随机从字符串中挑选一个怪物ID
    std::stringstream ss(*str);
    std::vector<int32_t> ids;
    std::string item;
    while (std::getline(ss, item, ','))
    {
        if (!item.empty())
            ids.push_back(std::stoi(item));
    }
    if (ids.empty())
        return;

    Answer::Random *v3 = Answer::Singleton<Answer::Random>::instance();
    int32_t idx = Answer::Random::generate(v3, 0, (int32_t)ids.size() - 1);

    MonsterWait stu;
    std::memset(&stu, 0, sizeof(stu));
    stu.id = ids[idx];
    stu.rewardWave = rewardWave ? 1 : 0;
    stu.startTick = Map::getTick();
    m_waitMonster.push_back(stu);
}

void Dungeon::OpenBoxMonster(int32_t BossId, bool rewardWave)
{
    MonsterWait stu;
    std::memset(&stu, 0, sizeof(stu));
    stu.id = BossId;
    stu.rewardWave = rewardWave ? 1 : 0;
    stu.startTick = Map::getTick();
    m_waitMonster.push_back(stu);
}

// ============================================================
// 事件系统
// ============================================================

void Dungeon::checkEvents()
{
    for (auto &event : m_events)
    {
        Dungeon::checkEvent(&event);
    }
}

void Dungeon::checkEvent(CfgMapEvent *mapEvent)
{
    if (!mapEvent)
        return;
    // 检查事件条件
    if (CfgMapEvent::isOpen(mapEvent) && !CfgMapEvent::isDone(mapEvent))
    {
        Dungeon::triggerEvent(mapEvent, nullptr);
    }
}

void Dungeon::triggerEvent(CfgMapEvent *mapEvent, Unit *pUnit)
{
    if (!mapEvent)
        return;

    int32_t LeftTime = Dungeon::GetLeftTime();
    switch (mapEvent->event_type)
    {
        case 1: // 胜利
            Dungeon::win(0);
            break;
        case 2: // 失败
            Dungeon::fail(0);
            break;
        case 3: // 生怪
            if (!mapEvent->effect.empty())
                Dungeon::generateMonster(&mapEvent->effect, false);
            break;
        case 4: // 随机生怪
            if (!mapEvent->effect.empty())
                Dungeon::randomMonster(&mapEvent->effect, false);
            break;
        default:
            break;
    }
}

void Dungeon::addMonsterHPEventInfo(MonsterDungeon *pMonster)
{
    for (auto &mapEvent : m_events)
    {
        if (mapEvent.trigger_type == 6 || mapEvent.trigger_type == 7)
        {
            // 血量触发事件
            HPEvent hpEvent;
            hpEvent.hpPercent = mapEvent.trigger_param.empty() ? 0 : mapEvent.trigger_param[0];
            hpEvent.tick = Map::getTick();
            hpEvent.eventId = mapEvent.id;
            MonsterDungeon::addEventHp(pMonster, &hpEvent);
        }
    }
}

void Dungeon::checkPlayerAttrEvent(MonsterDungeon *pMonster)
{
    for (auto &mapEvent : m_events)
    {
        if (mapEvent.trigger_type == 8)
        {
            // 玩家属性事件
            Dungeon::triggerEvent(&mapEvent, pMonster);
        }
    }
}

void Dungeon::onMonsterHPEvent(MonsterDungeon *monster, int32_t id)
{
    if (!monster)
        return;

    for (auto &mapEvent : m_events)
    {
        if (mapEvent.id == id && CfgMapEvent::isOpen(&mapEvent) && !CfgMapEvent::isDone(&mapEvent))
        {
            Dungeon::triggerEvent(&mapEvent, monster);
            break;
        }
    }
}

void Dungeon::onPlayerHPEvent(Player *player)
{
    for (auto &mapEvent : m_events)
    {
        if (mapEvent.trigger_type == 6)
        {
            if (CfgMapEvent::isOpen(&mapEvent) && !CfgMapEvent::isDone(&mapEvent))
            {
                Dungeon::triggerEvent(&mapEvent, player);
            }
        }
    }
}

void Dungeon::onPlayerDie(Player *player)
{
    if (!player)
        return;

    for (auto &mapEvent : m_events)
    {
        if (CfgMapEvent::isOpen(&mapEvent) && !CfgMapEvent::isDone(&mapEvent))
        {
            if (mapEvent.trigger_type == 6)
                Dungeon::triggerEvent(&mapEvent, player);
        }
    }
}

void Dungeon::onMonsterDie(MonsterDungeon *monster)
{
    if (!monster)
        return;

    // 检查波次怪物是否全部死亡
    bool allDead = true;
    for (auto *pMonster : m_monsters)
    {
        MonsterDungeon *md = dynamic_cast<MonsterDungeon *>(pMonster);
        if (md && md != monster && Unit::isAlive(md))
        {
            allDead = false;
            break;
        }
    }

    // 触发怪物死亡事件
    for (auto &mapEvent : m_events)
    {
        if (CfgMapEvent::isOpen(&mapEvent) && !CfgMapEvent::isDone(&mapEvent))
        {
            if (mapEvent.trigger_type == 1 || mapEvent.trigger_type == 3)
            {
                Dungeon::triggerEvent(&mapEvent, monster);
            }
        }
    }

    // 掉落检测
    Dungeon::checkDropItem(monster);

    // 检查是否全部死亡 -> 胜利
    if (allDead && getState() == DungeonState::DS_RUNNING)
    {
        if (m_cfgDungeon.type != 12 && m_cfgDungeon.type != 19)
        {
            Dungeon::win(0);
        }
    }
}

void Dungeon::onMonsterArriveRoadEnd(MonsterDungeon *monster)
{
    if (!monster)
        return;

    // 怪物到达终点，扣减生命值
    for (auto &mapEvent : m_events)
    {
        if (CfgMapEvent::isOpen(&mapEvent) && !CfgMapEvent::isDone(&mapEvent))
        {
            if (mapEvent.trigger_type == 7)
            {
                Dungeon::triggerEvent(&mapEvent, monster);
            }
        }
    }
}

void Dungeon::checkDropItem(MonsterDungeon *pMonster)
{
    if (!pMonster)
        return;

    int32_t DungeonId = Dungeon::getDungeonId();
    CfgData *v3 = Answer::Singleton<CfgData>::instance();
    const CfgDungeonDropGroup *dropGroup = CfgData::GetDungeonDropGroup(v3, DungeonId);
    if (!dropGroup)
        return;

    // 检查掉落
    int32_t dropMonsterId = MonsterDungeon::getMonsterId(pMonster);
    // 掉落逻辑处理
}

// ============================================================
// 宝库随机
// ============================================================

void Dungeon::AddRandomList(Player *player, int32_t nIndex)
{
    if (!player)
        return;

    m_RandomList.push_back(nIndex);

    int8_t ConnId = Player::getConnId(player);
    GameService *v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D3D);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, m_cfgDungeon.id);
        Answer::NetPacket::writeInt32(packet, nIndex);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        int16_t GateIndex = Player::getGateIndex(player);
        int8_t v7 = Player::getConnId(player);
        GameService *v8 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(v8, v7, GateIndex, packet);
    }
}

void Dungeon::OnRandomPos(Player *player)
{
    if (!player)
        return;

    int32_t DungeonId = Dungeon::getDungeonId();
    CfgData *v3 = Answer::Singleton<CfgData>::instance();
    BaoKuFuBen *pBaoKuFuBen = CfgData::GetBaoKuFuBen(v3, DungeonId);
    if (pBaoKuFuBen)
    {
        if (m_BuyRandomTimes * pBaoKuFuBen->AddTimes + pBaoKuFuBen->InitCount > m_RandomTimes)
        {
            int RandIndex = Dungeon::RandPosIndex(player);
            if (RandIndex > 0)
            {
                ++m_RandomTimes;
                Dungeon::AddRandomList(player, RandIndex);
                Dungeon::broadcastDungeonInfo();
            }
        }
    }
}

int32_t Dungeon::RandPosIndex(Player *player)
{
    if (m_Random <= 0 || m_Random > 5)
        return 0;

    if (!m_MaxRanomParam)
    {
        CfgData *v3 = Answer::Singleton<CfgData>::instance();
        std::map<int, BaoKuRandom> randomMap;
        CfgData::GetBaoKuRandomMap(&randomMap, v3);
        m_RandomMap = randomMap;

        m_MaxRanomParam = 0;
        for (auto &pair : m_RandomMap)
        {
            m_MaxRanomParam += pair.second.Items[m_Random - 1].nItemId;
        }
    }

    if (m_MaxRanomParam <= 0)
        return 0;

    int32_t Index = 0;
    Answer::Random *v6 = Answer::Singleton<Answer::Random>::instance();
    int32_t nRandom = Answer::Random::generate(v6, 1, m_MaxRanomParam);

    for (auto &pair : m_RandomMap)
    {
        if (pair.second.Items[m_Random - 1].nItemId >= nRandom)
        {
            // 检查是否已在列表中
            bool InList = false;
            for (auto &listIdx : m_RandomList)
            {
                if (listIdx == pair.first)
                {
                    InList = true;
                    break;
                }
            }

            if (!InList)
            {
                MemChrBag Stu;
                std::memset(&Stu, 0, sizeof(Stu));
                Stu.itemId = pair.second.Items[m_Random - 1].nItemId;
                Stu.itemClass = pair.second.Items[m_Random - 1].nItemClass;
                Stu.itemCount = 1;

                // 添加物品到背包
                CExtCharBag *Bag = Player::GetBag(player);
                if (!CExtCharBag::AddItem(Bag, &Stu, ITEM_CHANGE_REASON::IDCR_DUNGEON_RANDOM))
                {
                    // 发送邮件
                    CharId_t Cid = Player::getCid(player);
                    int8_t ConnId = Player::getConnId(player);
                    DBService *v12 = Answer::Singleton<DBService>::instance();
                    std::string Param = "系统";
                    DBService::OnSendSysMail(v12, ConnId, Cid, 1, &Stu, ITEM_CHANGE_REASON::IDCR_DUNGEON_RANDOM, &Param, 0);
                }

                // 公告
                if (pair.second.Items[m_Random - 1].nGongGaoId > 0)
                {
                    int8_t ConnId = Player::getConnId(player);
                    GameService *v14 = Answer::Singleton<GameService>::instance();
                    Answer::NetPacket *packet = GameService::popNetpacket(v14, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
                    if (packet)
                    {
                        Answer::NetPacket::writeInt32(packet, pair.second.Items[m_Random - 1].nGongGaoId);
                        std::string val;
                        Player::getName(player, val);
                        Answer::NetPacket::writeUTF8(packet, &val);
                        CharId_t v16 = Player::getCid(player);
                        Answer::NetPacket::writeInt64(packet, v16);
                        Answer::NetPacket::writeInt32(packet, Stu.itemId);
                        Answer::NetPacket::writeInt32(packet, Stu.itemCount);
                        Answer::NetPacket::writeInt32(packet, Stu.itemClass);
                        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
                        Answer::NetPacket::setSize(packet, WOffset);
                        int8_t v18 = Player::getConnId(player);
                        GameService *v19 = Answer::Singleton<GameService>::instance();
                        GameService::worldBroadcast(v19, v18, packet);
                    }
                }
            }

            // 传送玩家到随机位置
            Unit::instantMove(player, pair.second.XValue, pair.second.YValue, InstanceMoveReason::IMR_FU_BEN, 0);
            return pair.first;
        }
        nRandom -= pair.second.Items[m_Random - 1].nItemId;
    }
    return Index;
}

void Dungeon::OnBuyRandomPosTimes(Player *player)
{
    if (!player)
        return;

    int32_t DungeonId = Dungeon::getDungeonId();
    CfgData *v3 = Answer::Singleton<CfgData>::instance();
    BaoKuFuBen *pBaoKuFuBen = CfgData::GetBaoKuFuBen(v3, DungeonId);
    if (pBaoKuFuBen)
    {
        if (m_BuyRandomTimes < pBaoKuFuBen->BuyCount
            && pBaoKuFuBen->Gold > 0
            && m_BuyRandomTimes >= 0
            && Player::DecCurrency(
                player,
                CURRENCY_TYPE::CURRENCY_GOLD,
                (m_BuyRandomTimes + 1) * pBaoKuFuBen->Gold,
                CURRENCY_CHANGE_REASON::GCR_BUY_RANDOM_DUNGEON_TIMES,
                0))
        {
            ++m_BuyRandomTimes;
            Dungeon::broadcastDungeonInfo();
        }
    }
}

// ============================================================
// 胜利 / 失败 / 奖励
// ============================================================

void Dungeon::win(int32_t nParam)
{
    // 计算星级评价
    Dungeon::calWinStar();
    // 计算奖励
    Dungeon::calWinReward();
    // 设置胜利状态
    Dungeon::setState(DungeonState::DS_WIN, m_cfgDungeon.reward_time);
    // 广播完成
    Dungeon::broadcastFinishDungon(1);
}

void Dungeon::fail(int32_t nParam)
{
    // 如果类型为家族副本，删除家族副本
    if (getDungeonType() == 25)
    {
        FamilyId_t m_nFamilyId = m_nFamilyId;
        FamilyManager *v3 = Answer::Singleton<FamilyManager>::instance();
        FamilyManager::DeleteFamilyDungeon(v3, m_nFamilyId);
    }
    // 设置失败状态
    Dungeon::setState(DungeonState::DS_FAIL, 5);
    // 广播完成
    Dungeon::broadcastFinishDungon(0);
}

void Dungeon::calWinStar()
{
    // 计算胜利星级
    if (m_cfgDungeon.win_star.empty())
    {
        m_nWinStar = 0;
        return;
    }

    int32_t nUsedTime = (int32_t)((Map::getTick() - m_stateTick) / 1000);
    int32_t nWinStar = 0;

    for (size_t i = 0; i < m_cfgDungeon.win_star.size(); ++i)
    {
        if (nUsedTime <= m_cfgDungeon.win_star[i])
            nWinStar = (int32_t)(m_cfgDungeon.win_star.size() - i);
    }
    m_nWinStar = nWinStar;
}

void Dungeon::calWinReward()
{
    if (m_cfgDungeon.type == 12)
    {
        // 团队副本 - 按伤害分配
    }
    else
    {
        // 普通副本 - 固定奖励
        DungeonReward reward;
        reward.cid = 0;
        reward.bGet = 0;
        reward.money = m_cfgDungeon.costMoney;
        reward.cash = 0;
        reward.exp = 0;
        reward.Item = MemChrBag();
        m_playerReward.push_back(reward);
    }
}

bool Dungeon::GetReward(Player *player, int8_t nRatio, bool IsSysTem)
{
    if (!player)
        return false;

    int32_t Record = Player::getRecord(player, m_cfgDungeon.group_id);
    if (Record > Player::GetDungeonDailyEnterLimit(player, m_cfgDungeon.id))
        return false;

    BenefitType bnfType = Player::benefitType(player);
    double bnfRatio = Player::benefitRatio(player);
    double hardRatio = 1.0;
    double starRatio = 1.0;
    MemChrBag starReward;
    std::memset(&starReward, 0, sizeof(starReward));

    if (m_nWinStar > 0)
    {
        if (m_nWinStar <= (int32_t)m_cfgDungeon.star_ratio.size())
        {
            starRatio = (double)m_cfgDungeon.star_ratio[m_cfgDungeon.star_ratio.size() - m_nWinStar] / 100.0 + starRatio;
        }
        if (m_nWinStar <= (int32_t)m_cfgDungeon.star_reward.size())
        {
            starReward = m_cfgDungeon.star_reward[m_cfgDungeon.star_reward.size() - m_nWinStar];
        }
    }

    // 查找玩家的奖励记录
    auto iter = m_playerReward.begin();
    for (; iter != m_playerReward.end(); ++iter)
    {
        if (iter->cid == Player::getCid(player))
            break;
    }
    if (iter == m_playerReward.end())
        return false;
    if (iter->bGet > 0)
        return false;

    // 处理双倍奖励
    if (nRatio == 2)
    {
        if (iter->Item.itemCount <= 0)
            return false;
        if (m_cfgDungeon.double_cost < 0)
            return false;
        if (m_cfgDungeon.double_cost > 0)
        {
            int64_t DungeonId = Dungeon::getDungeonId();
            if (!Player::DecCurrency(
                    player,
                    CURRENCY_TYPE::CURRENCY_GOLD,
                    m_cfgDungeon.double_cost,
                    CURRENCY_CHANGE_REASON::GCR_DUNGEON_DOUBLE_REWARD,
                    DungeonId))
            {
                if (!IsSysTem)
                    return false;
                nRatio = 1;
            }
        }
    }

    if (nRatio != 1 && nRatio != 2)
        return false;

    // 发放铜钱奖励
    if (iter->money > 0)
    {
        Player::AddCurrency(
            player,
            CURRENCY_TYPE::CURRENCY_MONEY,
            (int32_t)((double)iter->money * m_dRatio * starRatio * hardRatio) * nRatio,
            CURRENCY_CHANGE_REASON::MCR_DUNGEON_REWARD,
            m_cfgDungeon.id);
    }

    // 发放金币奖励
    if (iter->cash > 0)
    {
        Player::AddCurrency(
            player,
            CURRENCY_TYPE::CURRENCY_CASH,
            (int32_t)((double)iter->cash * bnfRatio * m_dRatio * starRatio * hardRatio) * nRatio,
            CURRENCY_CHANGE_REASON::GCR_DUNGEON_REWARD,
            m_cfgDungeon.id);
    }

    // 发放经验
    if (iter->exp > 0)
    {
        Player::addExp(player, (int32_t)((double)iter->exp * m_dRatio * starRatio * hardRatio) * nRatio, 0, 1);
    }

    // 发放物品
    if (iter->Item.itemCount > 0)
    {
        iter->Item.itemCount *= nRatio;
        CExtCharBag *Bag = Player::GetBag(player);
        if (!CExtCharBag::AddItem(Bag, &iter->Item, ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD))
        {
            CharId_t Cid = Player::getCid(player);
            int8_t ConnId = Player::getConnId(player);
            DBService *v23 = Answer::Singleton<DBService>::instance();
            std::string Param = "系统";
            DBService::OnSendSysMail(v23, ConnId, Cid, 1, &iter->Item, ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD, &Param, 0);
        }
    }

    // 发放星级奖励
    if (starReward.itemId > 0 && starReward.itemCount > 0)
    {
        CExtCharBag *v24 = Player::GetBag(player);
        if (!CExtCharBag::AddItem(v24, &starReward, ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD))
        {
            CharId_t v25 = Player::getCid(player);
            int8_t v26 = Player::getConnId(player);
            DBService *v27 = Answer::Singleton<DBService>::instance();
            std::string v49 = "系统";
            DBService::OnSendSysMail(v27, v26, v25, 1, &starReward, ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD, &v49, 0);
        }
    }

    // 首次通关奖励
    int32_t v28 = Dungeon::getDungeonId() + 20000;
    if (Player::getRecord(player, v28) == 1 && !m_cfgDungeon.rewardOnce.empty())
    {
        CExtCharBag *v30 = Player::GetBag(player);
        if (!CExtCharBag::AddItem(v30, &m_cfgDungeon.rewardOnce[0], ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD))
        {
            CharId_t v31 = Player::getCid(player);
            int8_t v32 = Player::getConnId(player);
            DBService *v33 = Answer::Singleton<DBService>::instance();
            std::string v51 = "系统";
            DBService::OnSendSysMail(v33, v32, v31, 1, &m_cfgDungeon.rewardOnce[0], ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD, &v51, 0);
        }
    }
    else if (!m_cfgDungeon.rewardItem.empty())
    {
        CExtCharBag *v34 = Player::GetBag(player);
        if (!CExtCharBag::AddItem(v34, &m_cfgDungeon.rewardItem[0], ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD))
        {
            CharId_t v35 = Player::getCid(player);
            int8_t v36 = Player::getConnId(player);
            DBService *v37 = Answer::Singleton<DBService>::instance();
            std::string v53 = "系统";
            DBService::OnSendSysMail(v37, v36, v35, 1, &m_cfgDungeon.rewardItem[0], ITEM_CHANGE_REASON::ICR_DUNGEON_REWARD, &v53, 0);
        }
    }

    iter->bGet = nRatio;
    return true;
}

// ============================================================
// 召唤Boss
// ============================================================

bool Dungeon::SummonBoss(Player *player, int8_t nState)
{
    if (!player)
        return false;

    CharId_t cid = Player::getCid(player);

    // 检查是否已召唤
    for (auto &info : m_lSummonBoss)
    {
        if (info.cid == cid)
            return false;
    }

    int32_t summon_boss = m_cfgDungeon.summon_boss;
    CfgData *v6 = Answer::Singleton<CfgData>::instance();
    const CfgDungeonSummon *cfgSummon = CfgData::GetDungeonSummon(v6, summon_boss);
    if (!cfgSummon)
        return false;

    // 创建召唤Boss信息
    SummBossInfo info;
    info.cid = cid;
    info.state = nState;
    m_lSummonBoss.push_back(info);

    // 在随机位置生成Boss
    Position pos = Map::getRandomWalkablePosition();
    CPoolManager *v10 = Answer::Singleton<CPoolManager>::instance();
    MonsterDungeon *monster = (MonsterDungeon *)CPoolManager::AllocObject(v10, 1);
    if (monster)
    {
        MonsterDungeon::refresh(monster);
        Dungeon::addMonster(monster, pos.x, pos.y);
    }

    return true;
}

bool Dungeon::Summon(Player *player, int32_t nIndex)
{
    if (!player)
        return false;

    int32_t id = m_cfgDungeon.summon_cost;
    CfgData *v5 = Answer::Singleton<CfgData>::instance();
    const CfgDungeonSummon *cfgSummon = CfgData::GetDungeonSummon(v5, id);
    if (!cfgSummon)
        return false;

    // 扣除金币
    if (!Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, cfgSummon->cost,
            CURRENCY_CHANGE_REASON::GCR_DUNGEON_SUMMON, 0))
        return false;

    // 生成召唤怪物
    MonsterWait stu;
    std::memset(&stu, 0, sizeof(stu));
    stu.id = cfgSummon->monsterId;
    stu.rewardWave = 0;
    stu.startTick = Map::getTick();
    m_waitMonster.push_back(stu);

    return true;
}

bool Dungeon::isAllPlayerSummon()
{
    return m_lSummonBoss.size() == m_players.size();
}

void Dungeon::checkSummonBoss()
{
    if (m_cfgDungeon.type == 12 && !Dungeon::isAllPlayerSummon()
        && Map::getTick() - m_nSummStart > 29999)
    {
        for (auto *player : m_players)
        {
            if (player)
            {
                bool bFind = false;
                for (auto &info : m_lSummonBoss)
                {
                    if (info.cid == Player::getCid(player))
                    {
                        bFind = true;
                        break;
                    }
                }
                if (!bFind)
                {
                    SummBossInfo info;
                    info.cid = Player::getCid(player);
                    info.state = 0;
                    m_lSummonBoss.push_back(info);
                    std::string name;
                    Player::getName(player, name);
                    Dungeon::broadcastSummonBoss(&name, 0);
                }
            }
        }
        if (Dungeon::isAllMonsterDie())
            Dungeon::win(0);
    }
}

void Dungeon::sendStartSummon()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v1, Answer::PackType::PACK_DISPATCH, 0x2DF5);
    if (packet)
    {
        int32_t LeftTime = Dungeon::GetLeftTime();
        Answer::NetPacket::writeInt32(packet, LeftTime);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Map::broadcast(packet);
    }
}

bool Dungeon::isAllMonsterDie()
{
    for (auto *pMonster : m_monsters)
    {
        if (pMonster && Unit::isAlive(pMonster))
            return false;
    }
    return true;
}

// ============================================================
// 塔防系统
// ============================================================

bool Dungeon::BuyTower(Player *player, int32_t nId)
{
    if (!player)
        return false;

    CfgData *v3 = Answer::Singleton<CfgData>::instance();
    const CfgTower *cfgTower = CfgData::GetTowerCfg(v3, nId);
    if (!cfgTower)
        return false;

    // 检查限制数量
    int32_t count = 0;
    for (auto &tower : m_waitTower)
    {
        if (tower.id == nId)
            count += tower.free;
    }
    if (count >= cfgTower->limit)
        return false;

    // 扣除费用
    if (!Player::DecCurrency(player, (CURRENCY_TYPE)cfgTower->costType,
            cfgTower->costValue, CURRENCY_CHANGE_REASON::GCR_BUILD_TOWER, 0))
        return false;

    DungeonTower tower;
    tower.id = nId;
    tower.costType = cfgTower->costType;
    tower.costValue = cfgTower->costValue;
    tower.free = 1;
    tower.limit = cfgTower->limit;
    Dungeon::addWaitTower(&tower);
    return true;
}

bool Dungeon::BuildTower(Player *player, int32_t nId, int16_t nPosX, int16_t nPosY)
{
    if (!player)
        return false;

    // 查找塔配置
    CfgData *v6 = Answer::Singleton<CfgData>::instance();
    const CfgTower *cfgTower = CfgData::GetTowerCfg(v6, nId);
    if (!cfgTower)
        return false;

    // 检查是否有建造名额
    for (auto it = m_waitTower.begin(); it != m_waitTower.end(); ++it)
    {
        if (it->id == nId && it->free > 0)
        {
            // 在地图上创建塔(怪物形式)
            CPoolManager *v10 = Answer::Singleton<CPoolManager>::instance();
            Monster *monster = (Monster *)CPoolManager::AllocObject(v10, 1);
            if (monster)
            {
                Dungeon::addMonster(monster, nPosX, nPosY);
                m_buildTower.push_back(monster);
            }

            it->free--;
            return true;
        }
    }
    return false;
}

void Dungeon::addWaitTower(const DungeonTower *tower)
{
    for (auto &existing : m_waitTower)
    {
        if (existing.id == tower->id
            && existing.costType == tower->costType
            && existing.costValue == tower->costValue)
        {
            existing.free += tower->free;
            existing.limit += tower->limit;
            return;
        }
    }
    m_waitTower.push_back(*tower);
    Dungeon::broadcastDungeonInfo();
}

// ============================================================
// 复活系统
// ============================================================

void Dungeon::checkRevive(int64_t curTick)
{
    if (m_cfgDungeon.type == 36)
        return;

    for (auto *player : m_players)
    {
        if (player && Player::IsDead(player))
        {
            int32_t ReviveTime = 10000;
            int64_t DieTick = Player::GetDieTick(player);
            if (curTick - DieTick >= ReviveTime)
            {
                int32_t Now = Map::getNow();
                Player::SetStartProtect(player, Now);
                Player::FullHPAndSync(player);
                Unit::instantMove(player, m_cfgDungeon.x, m_cfgDungeon.y, InstanceMoveReason::IMR_TRANSFER, 0);
            }
        }
    }
}

bool Dungeon::OnSafeRevive(Player *player)
{
    if (!player)
        return false;

    if (m_cfgDungeon.type == 36)
    {
        Player::leaveDungeon(player);
    }
    else
    {
        int32_t Now = Map::getNow();
        Player::SetStartProtect(player, Now);
        Player::FullHPAndSync(player);
        Unit::instantMove(player, m_cfgDungeon.x, m_cfgDungeon.y, InstanceMoveReason::IMR_TRANSFER, 0);
    }
    return true;
}

// ============================================================
// 植物 / NPC / 采集
// ============================================================

int32_t Dungeon::onBeginGather(Player *player)
{
    if (!player)
        return 0;

    if (m_cfgDungeon.type != 32)
        return 0;

    // 采集逻辑
    int32_t id = m_cfgDungeon.id;
    CfgData *v4 = Answer::Singleton<CfgData>::instance();
    const CLibraryTable *CLibraryTable = CfgData::GetCLibraryTable(v4, id);
    if (!CLibraryTable)
        return 0;

    // 检查消耗
    CExtCharBag *Bag = Player::GetBag(player);
    // 采集消耗处理
    return 1;
}

void Dungeon::onPlantGather(PlantDungeon *plant)
{
    if (!plant)
        return;

    for (auto &mapEvent : m_events)
    {
        if (mapEvent.trigger_type == 4)
        {
            if (CfgMapEvent::isOpen(&mapEvent) && !CfgMapEvent::isDone(&mapEvent))
            {
                Dungeon::triggerEvent(&mapEvent, plant);
            }
        }
    }
}

void Dungeon::OnNpcEnd(NpcDungeon *npc, int8_t choice)
{
    if (!npc)
        return;

    for (auto &mapEvent : m_events)
    {
        if (mapEvent.trigger_type == 5)
        {
            if (CfgMapEvent::isOpen(&mapEvent) && !CfgMapEvent::isDone(&mapEvent))
            {
                Dungeon::triggerEvent(&mapEvent, npc);
            }
        }
    }
}

// ============================================================
// 广播系统
// ============================================================

void Dungeon::signDungeonCompleted()
{
    int32_t nRecordId = Dungeon::getDungeonId() + 20000;
    for (auto *player : m_players)
    {
        if (player)
        {
            Player::updateRecord(player, nRecordId, 1);
        }
    }
}

void Dungeon::setCanSubmit()
{
    for (auto *player : m_players)
    {
        if (player)
        {
            // 设置玩家可以提交副本
            Player::setDungeonCanSubmit(player, true);
        }
    }
}

void Dungeon::updateExpDungeonRank()
{
    // 经验副本排名更新
    for (auto *player : m_players)
    {
        if (player)
        {
            // 更新经验排名
        }
    }
}

void Dungeon::broadcastFinishDungon(int32_t param)
{
    for (auto *player : m_players)
    {
        if (player)
        {
            Dungeon::sendDungeonResult(player, param);
        }
    }
}

void Dungeon::sendDungeonResult(Player *player, int32_t param)
{
    if (!player)
        return;

    int8_t ConnId = Player::getConnId(player);
    GameService *v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v4, ConnId, Answer::PackType::PACK_DISPATCH, 0x2727);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, param);
        Answer::NetPacket::writeInt32(packet, m_nWinStar);
        Answer::NetPacket::writeInt32(packet, m_finshInfo.get_exp);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        int16_t GateIndex = Player::getGateIndex(player);
        int8_t v7 = Player::getConnId(player);
        GameService *v8 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(v8, v7, GateIndex, packet);
    }
}

void Dungeon::broadcastDungeonEvent(int32_t evtId)
{
    GameService *v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v2, Answer::PackType::PACK_DISPATCH, 0x2728);
    if (packet)
    {
        int32_t DungeonId = Dungeon::getDungeonId();
        Answer::NetPacket::writeInt32(packet, DungeonId);
        Answer::NetPacket::writeInt32(packet, evtId);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Map::broadcast(packet);
    }
}

void Dungeon::broadcastPlayMovie(int32_t movieID)
{
    GameService *v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v2, Answer::PackType::PACK_DISPATCH, 0x2729);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, movieID);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Map::broadcast(packet);
    }
}

void Dungeon::broadcastDungeonInfo()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v1, Answer::PackType::PACK_DISPATCH, 0x272A);
    if (packet)
    {
        int32_t LeftTime = Dungeon::GetLeftTime();
        Answer::NetPacket::writeInt32(packet, (int32_t)m_state);
        Answer::NetPacket::writeInt32(packet, LeftTime);
        Answer::NetPacket::writeInt32(packet, m_nHard);
        Answer::NetPacket::writeInt32(packet, (int32_t)m_BuyRandomTimes);
        Answer::NetPacket::writeInt32(packet, (int32_t)m_RandomTimes);
        Answer::NetPacket::writeInt32(packet, (int32_t)m_lSummonBoss.size());
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Map::broadcast(packet);
    }
}

void Dungeon::broadcastDamageList()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v1, Answer::PackType::PACK_DISPATCH, 0x272B);
    if (packet && !m_damages.empty())
    {
        Answer::NetPacket::writeInt32(packet, (int32_t)m_damages.size());
        for (auto &dmg : m_damages)
        {
            Answer::NetPacket::writeInt64(packet, dmg.cid);
            Answer::NetPacket::writeUTF8(packet, &dmg.name);
            Answer::NetPacket::writeInt32(packet, dmg.damage);
        }
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Map::broadcast(packet);
    }
    m_needBroadcastDamage = false;
}

void Dungeon::broadcastSummonBoss(const std::string *name, int8_t nState)
{
    GameService *v3 = Answer::Singleton<GameService>::instance();
    int32_t LeftTime = Dungeon::GetLeftTime();
    Answer::NetPacket *packet = GameService::popNetpacket(v3, Answer::PackType::PACK_DISPATCH, 0x2DF4);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, LeftTime);
        Answer::NetPacket::writeUTF8(packet, name);
        Answer::NetPacket::writeInt32(packet, nState);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Map::broadcast(packet);
    }
}

void Dungeon::checkBroadcastDamageList()
{
    if (m_needBroadcastDamage)
        Dungeon::broadcastDamageList();
}

void Dungeon::sendRandEventTime()
{
    GameService *v1 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v1, Answer::PackType::PACK_DISPATCH, 0x2DF6);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, m_cfgDungeon.rand_time);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        Map::broadcast(packet);
    }
}

void Dungeon::DungeonGongGao(int8_t connid, CharId_t CharId, std::string *p_Name)
{
    GameService *v4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(v4, connid, Answer::PackType::PACK_DISPATCH, 0x2CC1);
    if (packet)
    {
        Answer::NetPacket::writeInt64(packet, CharId);
        Answer::NetPacket::writeUTF8(packet, p_Name);
        Answer::NetPacket::writeInt32(packet, m_cfgDungeon.Battle);
        Answer::NetPacket::writeInt32(packet, m_cfgDungeon.star);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        GameService *v6 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(v6, connid, packet);
    }
}

// ============================================================
// 猜大小
// ============================================================

int32_t Dungeon::GuessTheSize(Player *player, int32_t nId)
{
    if (!player || m_bGetReward)
        return 0;

    int32_t Constgold = (m_Times <= 0) ? 50 : 100;

    if (!Player::DecCurrency(
            player,
            CURRENCY_TYPE::CURRENCY_GOLD,
            Constgold,
            CURRENCY_CHANGE_REASON::GCR_YJSK_GUESS_SIZE,
            0))
        return 0;

    Answer::Random *v4 = Answer::Singleton<Answer::Random>::instance();
    int32_t nRand = Answer::Random::generate(v4, 1, 3);

    if (nRand == nId)
    {
        MemChrBag starReward;
        std::memset(&starReward, 0, sizeof(starReward));
        starReward.itemId = 500;
        starReward.itemClass = 1;
        starReward.itemCount = 50;

        CExtCharBag *Bag = Player::GetBag(player);
        if (!CExtCharBag::AddItem(Bag, &starReward, ITEM_CHANGE_REASON::ICR_DUNGEON_GUESS_SIZE))
        {
            CharId_t Cid = Player::getCid(player);
            int8_t ConnId = Player::getConnId(player);
            DBService *v8 = Answer::Singleton<DBService>::instance();
            std::string Param = "系统";
            DBService::OnSendSysMail(v8, ConnId, Cid, 1, &starReward, ITEM_CHANGE_REASON::ICR_DUNGEON_GUESS_SIZE, &Param, 0);
        }
        m_bGetReward = true;

        // 全服公告
        GameService *v9 = Answer::Singleton<GameService>::instance();
        Answer::NetPacket *packet = GameService::popNetpacket(v9, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, 455);
            CharId_t v10 = Player::getCid(player);
            Answer::NetPacket::writeInt64(packet, v10);
            std::string val;
            Player::getName(player, val);
            Answer::NetPacket::writeUTF8(packet, &val);
            Answer::NetPacket::writeInt32(packet, 50);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            GameService *v12 = Answer::Singleton<GameService>::instance();
            GameService::worldBroadcast(v12, 0, packet);
        }
    }

    ++m_Times;
    Dungeon::broadcastDungeonInfo();
    return nRand;
}

bool Dungeon::DungeonGuWu(Player *player, int8_t Type)
{
    if (!player)
        return false;

    // 鼓舞逻辑 - 消耗金币获得Buff
    CfgData *v5 = Answer::Singleton<CfgData>::instance();
    const CfgDungeonGuWu *cfgGuWu = CfgData::GetDungeonGuWu(v5, m_cfgDungeon.id, Type);
    if (!cfgGuWu)
        return false;

    // 检查是否达到最大等级
    if (Type == 1 && m_GoldBuffLevel >= cfgGuWu->maxLevel)
        return false;
    if (Type == 2 && m_MoneyBuffLevel >= cfgGuWu->maxLevel)
        return false;

    int32_t cost = cfgGuWu->cost * (Type == 1 ? m_GoldBuffLevel + 1 : m_MoneyBuffLevel + 1);
    if (!Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, cost,
            CURRENCY_CHANGE_REASON::GCR_DUNGEON_GUWU, 0))
        return false;

    Answer::Random *v8 = Answer::Singleton<Answer::Random>::instance();
    int32_t randomValue = Answer::Random::generate(v8, 1, 100);

    if (randomValue <= cfgGuWu->rate)
    {
        // 成功 - 增加Buff等级
        if (Type == 1)
        {
            ++m_GoldBuffLevel;
        }
        else
        {
            ++m_MoneyBuffLevel;
        }
        return true;
    }
    return false;
}

// ============================================================
// Map虚函数重写
// ============================================================

void Dungeon::init(CMapRunner *pRunner, const CfgMap *cfgmap)
{
    Map::init(pRunner, cfgmap);
}

bool Dungeon::canEnter(Player *player)
{
    return Map::canEnter(player);
}

bool Dungeon::OnSitRevive(Player *player)
{
    return Dungeon::OnSafeRevive(player);
}

bool Dungeon::SpecialSitRevive()
{
    return true;
}

bool Dungeon::CanAutoRevive()
{
    return true;
}

bool Dungeon::CanUsePet()
{
    return m_cfgDungeon.type != 15 && m_cfgDungeon.type != 23;
}

bool Dungeon::CanStayInMap()
{
    return false;
}

int32_t Dungeon::GetAvgBattle()
{
    return m_cfgDungeon.Battle;
}

int32_t Dungeon::getNow()
{
    return Map::getNow();
}

int64_t Dungeon::getTick()
{
    return Map::getTick();
}

void Dungeon::onMonsterDie(Monster *monster, Player *player, bool IsGroupMonster)
{
    MonsterDungeon *md = dynamic_cast<MonsterDungeon *>(monster);
    if (md)
    {
        Dungeon::onMonsterDie(md);
    }
}

// ============================================================
// IMapEvent虚函数重写
// ============================================================

void Dungeon::init(const CfgMapEventList *events, int64_t nTime)
{
    IMapEvent::init(events, nTime);
}

void Dungeon::openEvent(int32_t eventId)
{
    IMapEvent::openEvent(eventId, Map::getTick());
}

void Dungeon::closeEvent(int32_t eventId)
{
    IMapEvent::closeEvent(eventId, Map::getTick());
}

void Dungeon::openEvents(int32_t *eventIds, int32_t count)
{
    IMapEvent::openEvents(eventIds, count);
}

void Dungeon::closeEvents(int32_t *eventIds, int32_t count)
{
    IMapEvent::closeEvents(eventIds, count);
}
