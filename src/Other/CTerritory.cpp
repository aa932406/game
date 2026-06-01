#include "Other/CTerritory.h"

#include "Game/Map.h"
#include "Game/Monster.h"
#include "Game/Npc.h"
#include "Game/Plant.h"
#include "Game/Trap.h"
#include "Game/CDropItem.h"
#include "Game/CPoolManager.h"
#include "Game/GameService.h"
#include "Character/Player.h"
#include "Character/FamilyManager.h"
#include "Config/CfgData.h"
#include "Config/CfgFamilyTable.h"

CTerritory::CTerritory()
    : m_FamilyId(0)
    , m_Piglet(nullptr)
{
}

CTerritory::~CTerritory()
{
}

void CTerritory::removePlayer(Player *player, bool islogout)
{
    if (player)
        Map::removePlayer(this, player, islogout);
}

void CTerritory::reset()
{
    m_players.clear();

    for (auto it = m_monsters.begin(); it != m_monsters.end(); ++it)
    {
        Monster *monster = *it;
        if (monster)
        {
            GameService::instance()->removeMonster(monster);
            CPoolManager::instance()->push<Monster>(monster);
        }
    }
    m_monsters.clear();

    for (auto it_0 = m_plants.begin(); it_0 != m_plants.end(); ++it_0)
    {
        Plant *plant = *it_0;
        if (plant)
        {
            CPoolManager::instance()->push<Plant>(plant);
        }
    }
    m_plants.clear();

    for (auto it_1 = m_dropItems.begin(); it_1 != m_dropItems.end(); ++it_1)
    {
        CDropItem *pDropItem = *it_1;
        if (pDropItem)
        {
            CPoolManager::instance()->push<CDropItem>(pDropItem);
        }
    }
    m_dropItems.clear();

    for (auto it_2 = m_traps.begin(); it_2 != m_traps.end(); ++it_2)
    {
        Trap *trap = *it_2;
        if (trap)
            CPoolManager::instance()->push<Trap>(trap);
    }
    m_traps.clear();

    for (auto it_3 = m_npcs.begin(); it_3 != m_npcs.end(); ++it_3)
    {
        Npc *npc = *it_3;
        if (npc)
            CPoolManager::instance()->push<Npc>(npc);
    }
    m_npcs.clear();

    m_FamilyId = 0;
    m_Piglet = nullptr;
}

void CTerritory::AddFamilyMonster(bool bInit)
{
    FamilyId_t m_FamilyId = this->m_FamilyId;
    FamilyManager *v3 = FamilyManager::instance();
    FamilyInfo FamilyStu;
    v3->GetFamilyInfo(&FamilyStu, m_FamilyId);

    if ((bInit && FamilyStu.nBossState == 1) || (!bInit && FamilyStu.nBossState == 3))
    {
        if (m_Piglet)
        {
            Monster::leaveMap(m_Piglet);
            m_Piglet->addHp(-(int32_t)m_Piglet->getHp());
            Monster::SetReviveTime(m_Piglet, 0);

            int32_t level = FamilyStu.nLevel;
            const CfgFamilyBoss *pFamilyBoss = CfgData::instance()->GetFamilyTable()->GetFamilyBoss(level, 0);
            if (pFamilyBoss)
            {
                int32_t BossMid = pFamilyBoss->BossMid;
                CfgMonster *pCfgMonster = CfgData::instance()->getMonster(BossMid);
                if (pCfgMonster)
                {
                    Monster *monster = CPoolManager::instance()->pop<Monster>();
                    if (monster)
                    {
                        CfgMapMonster MapMonster;
                        memset(&MapMonster, 0, sizeof(MapMonster));
                        MapMonster.id = CfgData::instance()->GetMapMonsterId();
                        MapMonster.mapid = Map::GetMapId(this);
                        MapMonster.x = StaticObj::GetPosX(m_Piglet);
                        MapMonster.y = StaticObj::GetPosY(m_Piglet);
                        std::vector<AttrAddon> vAttrAddon;
                        Monster::init(monster, pCfgMonster, &MapMonster, MonsterState::MS_STAND, &vAttrAddon);
                        addMonster(monster, MapMonster.x, MapMonster.y);
                    }
                }
            }
        }
    }
}

void CTerritory::addPlayer(Player *player, int32_t x, int32_t y)
{
    if (player)
        Map::addPlayer(this, player, x, y);
}

void CTerritory::addMonster(Monster *monster, int32_t x, int32_t y)
{
    if (monster)
    {
        if (Monster::getMid(monster) == 50001)
        {
            m_Piglet = monster;
            std::string p_strFamilyName("unk_8F22A0");
            Monster::SetFamilyId(m_Piglet, m_FamilyId, &p_strFamilyName);
        }
        Map::addMonster(this, monster, x, y);
    }
}

void CTerritory::onMonsterDie(Monster *monster, Player *player, bool IsGroupMonster)
{
    if (monster)
    {
        int32_t Mid = Monster::getMid(monster);
        const CfgFamilyBoss *pFamilyBoss = CfgData::instance()->GetFamilyTable()->GetFamilyBoss(0, Mid);
        if (pFamilyBoss && m_Piglet)
        {
            int32_t Now = CTimer::instance()->GetNow();
            Monster::SetReviveTime(m_Piglet, Now + 600);
            Monster::Revive(m_Piglet);
            AddFamilyMonster(false);

            int8_t ConnId = player->getConnId();
            Answer::NetPacket *packet = GameService::instance()->popNetpacket(ConnId, Answer::PackType::PACK_DISPATCH, 0x2F4Fu);
            if (packet)
            {
                packet->writeInt32(Mid);
                packet->writeInt32(FamilyStu.nLevel);
                uint32_t WOffset = packet->getWOffset();
                packet->setSize(WOffset);
                int16_t GateIndex = player->getGateIndex();
                int8_t v14 = player->getConnId();
                GameService::instance()->sendPacketTo(v14, GateIndex, packet);
            }
        }

        Map::onMonsterDie(this, monster, player, IsGroupMonster);
    }
}
