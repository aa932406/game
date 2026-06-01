#include "Other/MonsterDungeon.h"

#include "Game/Dungeon.h"
#include "Game/Monster.h"
#include "Game/CPoolManager.h"
#include "Game/GameService.h"
#include "Config/CfgDungeonMonster.h"
#include "Config/CfgMonster.h"
#include "Config/CfgMapMonster.h"
#include "Game/CObjPet.h"
#include "Character/Player.h"
#include "Utility/Position.h"
#include "Utility/UnitHandle.h"
#include "Game/StaticObj.h"

MonsterDungeon::MonsterDungeon()
    : m_dungeon(nullptr)
    , m_delFlag(0)
{
    m_cfgDungeonMonster = CfgDungeonMonster();
}

MonsterDungeon::~MonsterDungeon()
{
}

bool MonsterDungeon::refresh()
{
    int64_t curHp = getHp();
    int64_t maxHp = getMaxHp();
    int32_t hpPercent = roundInt((float)((float)curHp * 100.0f / (float)maxHp));

    for (auto it = m_eventHp.begin(); it != m_eventHp.end(); ++it)
    {
        if (it->minhp <= hpPercent && it->maxhp > hpPercent)
        {
            Dungeon::onMonsterHPEvent(m_dungeon, this, it->id);
        }
    }
    return Monster::refresh(this);
}

void MonsterDungeon::postDamage(int32_t damge, UnitHandle launcher, int32_t Mid)
{
    if (m_dungeon)
    {
        const Player *player = nullptr;
        if (launcher.type == 1)
        {
            int32_t RunnerId = StaticObj::GetRunnerId(this);
            player = GameService::instance()->getPlayer(launcher.id, RunnerId, 1);
        }
        else if (launcher.type == 4)
        {
            int32_t RunnerId = StaticObj::GetRunnerId(this);
            CObjPet *pPet = GameService::instance()->getPet(launcher.id, RunnerId);
            if (pPet)
                player = pPet->GetPlayer();
        }
        if (player)
        {
            CharId_t Cid = player->getCid();
            Dungeon::AddPlayerDamage(m_dungeon, Cid, damge);
        }
    }
    Monster::postDamage(this, damge, launcher, Mid);
}

void MonsterDungeon::reset()
{
    Monster::reset(this);
    m_dungeon = nullptr;
    m_eventHp.clear();
    m_road.clear();
}

int8_t MonsterDungeon::getDungeonHard()
{
    if (m_dungeon)
        return Dungeon::GetHard(m_dungeon);
    return 0;
}

int8_t MonsterDungeon::getDungeonQuality()
{
    if (m_dungeon)
        return Dungeon::GetQuality(m_dungeon);
    return 0;
}

void MonsterDungeon::addEventHp(HPEvent *hpEvent)
{
    m_eventHp.push_back(*hpEvent);
}

void MonsterDungeon::die()
{
    Monster::die(this);
    Dungeon::onMonsterDie(m_dungeon, this);
}

void MonsterDungeon::destroy()
{
    CPoolManager::instance()->push<MonsterDungeon>(this);
}

void MonsterDungeon::remove()
{
    Monster::leaveMap(this);
    GameService::instance()->removeMonster(this);
    m_delFlag = 1;
}

void MonsterDungeon::onArriveTarget()
{
    if (getState() == MonsterState::MS_RUN_ON_ROAD)
    {
        if (!m_road.empty())
        {
            Position curPos = StaticObj::getCurrentTile(this);
            Position tarPos = m_road.front();
            m_road.pop_front();
            while (!m_road.empty())
            {
                if (!(curPos == tarPos))
                    break;
                tarPos = m_road.front();
                m_road.pop_front();
            }
            setTargetTile(tarPos.x, tarPos.y);
        }
        else
        {
            setState(MonsterState::MS_STAND);
            if (m_dungeon)
                Dungeon::onMonsterArriveRoadEnd(m_dungeon, this);
        }
    }
}

void MonsterDungeon::runOnRoad()
{
    if (!m_road.empty())
    {
        m_target.clear();
        Position curPos = StaticObj::getCurrentTile(this);
        Position tarPos = m_road.front();
        m_road.pop_front();
        while (!m_road.empty())
        {
            if (!(curPos == tarPos))
                break;
            tarPos = m_road.front();
            m_road.pop_front();
        }
        setTargetTile(tarPos.x, tarPos.y);
        setState(MonsterState::MS_RUN_ON_ROAD);
    }
}
