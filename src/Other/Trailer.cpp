#include "Other/Trailer.h"

#include "Game/Map.h"
#include "Game/GameService.h"
#include "Game/StaticObj.h"
#include "Game/CMoneyRewardTask.h"
#include "Game/Unit.h"
#include "Game/CObjPet.h"
#include "Game/CTimer.h"
#include "Character/Player.h"
#include "Character/CExtFightChecker.h"
#include "Character/CExtCharFamily.h"
#include "Character/CFaBao.h"
#include "Config/CfgData.h"
#include "Config/CfgTrailerTable.h"
#include "Network/NetPacket.h"
#include "Utility/Answer.h"
#include "Utility/Position.h"

Trailer::Trailer()
    : m_DieTick(0)
    , m_Level(0)
    , m_Owner(0)
    , m_RewardTimes(0)
    , m_RobberyId(0)
    , m_State(0)
    , m_nEndTime(0)
    , m_pMap(nullptr)
    , m_SubTime(0)
{
}

Trailer::~Trailer()
{
}

void Trailer::reset()
{
    Unit::reset(this);
    m_nEndTime = 0;
    m_Owner = 0;
    m_State = 0;
    m_DieTick = 0;
    m_RobberyName = "";
    m_RobberyId = 0;
    m_RewardTimes = 0;
}

bool Trailer::refresh()
{
    CheckTrailerEnd();
    if (m_DieTick > 0 && getTick() - m_DieTick > 3000)
    {
        m_DieTick = 0;
        addHp(100);
        if (m_pMap)
            Map::broadcastIntoMap(m_pMap, this);
    }
    return false;
}

FamilyId_t Trailer::GetFamilyId()
{
    int32_t RunnerId = StaticObj::GetRunnerId(this);
    CharId_t m_Owner = this->m_Owner;
    const Player *Owner = GameService::instance()->getPlayer(m_Owner, RunnerId, 1);
    if (Owner)
        return Owner->getFamilyId();
    return 0;
}

void Trailer::postDamage(int32_t damge, UnitHandle launcher, int32_t Mid)
{
    if (Robbery())
        return;

    Player *pPlayer = nullptr;
    if (launcher.type == 1)
    {
        int32_t RunnerId = StaticObj::GetRunnerId(this);
        pPlayer = GameService::instance()->getPlayer(launcher.id, RunnerId, 1);
    }
    else if (launcher.type == 4)
    {
        int32_t RunnerId = StaticObj::GetRunnerId(this);
        CObjPet *pPet = GameService::instance()->getPet(launcher.id, RunnerId);
        if (pPet)
            pPlayer = pPet->GetPlayer();
    }

    if (pPlayer)
    {
        int32_t v8 = StaticObj::GetRunnerId(this);
        CharId_t m_Owner = this->m_Owner;
        Player *Owner = GameService::instance()->getPlayer(m_Owner, v8, 1);
        if (Owner)
        {
            bool v14 = false;
            if (!Owner->GetFightChecker()->IsBuleName()
                && Owner->getPkValue() <= 99
                && pPlayer->getPkValue() <= 99)
            {
                FamilyId_t FamilyId = pPlayer->getFamilyId();
                if (!Owner->GetCharFamily()->IsDeclareWarFamily(FamilyId))
                    v14 = true;
            }
            if (v14 && !StaticObj::InActivity(this))
            {
                pPlayer->GetFightChecker()->ChangeBuleName(true);
            }
        }

        if (!isAlive())
        {
            m_State = 1;
            m_DieTick = 1;
            Unit::broadcastLeave(this);

            CMoneyRewardTask *CMoneyRewardTask = pPlayer->GetCMoneyRewardTask();
            int32_t JieBiaoTimes = CMoneyRewardTask->GetJieBiaoTimes();
            if (JieBiaoTimes < CMoneyRewardTask->GetJieBiaoLimit())
            {
                CMoneyRewardTask->AddJieBiaoTimes();
                int32_t TrailerQuality = m_cfgTrailer.TrailerQuality;
                const CfgTrailerInfo *pTrailerInfo = CfgData::instance()->GetTrailerTable()->GetTrailerInfo(TrailerQuality);
                if (!pTrailerInfo)
                    return;

                float RobberMoney = (float)pTrailerInfo->RobberMoney;
                float v55 = RobberMoney * GetRate();
                int32_t RewardTimes = GetRewardTimes();
                pPlayer->AddCurrency(CURRENCY_TYPE::CURRENCY_BIND_MONEY, (int)(RewardTimes * v55), CURRENCY_CHANGE_REASON::MCR_YAO_BIAO, 0);

                int32_t RobberWeiWang = pTrailerInfo->RobberWeiWang;
                int32_t v25 = GetRewardTimes();
                pPlayer->AddCurrency(CURRENCY_TYPE::CURRENCY_BIND_MONEY, RobberWeiWang * v25, CURRENCY_CHANGE_REASON::MCR_YAO_BIAO, 0);

                int32_t RobberFuWen = pTrailerInfo->RobberFuWen;
                int32_t v27 = GetRewardTimes() * RobberFuWen;
                pPlayer->GetPlayerFaBao()->AddFaBaoRes(2, v27);

                Answer::NetPacket *packet_1 = GameService::instance()->popNetpacket(pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2D2F);
                if (packet_1)
                {
                    packet_1->writeInt8(2);
                    packet_1->writeInt8(GetRewardTimes() - 1);
                    packet_1->writeInt32(m_cfgTrailer.TrailerQuality);
                    packet_1->writeInt64(m_Owner);
                    packet_1->writeUTF8(m_Name);
                    packet_1->writeInt32(m_Level);
                    uint32_t WOffset = packet_1->getWOffset();
                    packet_1->setSize(WOffset);
                    GameService::instance()->sendPacketTo(pPlayer->getConnId(), pPlayer->getGateIndex(), packet_1);
                }
            }

            Player::getName(this, m_RobberyName);
            Player *Owner_0 = GameService::instance()->getPlayer(m_Owner, StaticObj::GetRunnerId(this), 1);
            if (Owner_0)
            {
                SendInfo(Owner_0);
                Answer::NetPacket *packet_0 = GameService::instance()->popNetpacket(Owner_0->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2D2F);
                if (packet_0)
                {
                    packet_0->writeInt8(1);
                    packet_0->writeInt8(GetRewardTimes() - 1);
                    packet_0->writeInt32(m_cfgTrailer.TrailerQuality);
                    packet_0->writeInt64(pPlayer->getCid());
                    packet_0->writeUTF8(pPlayer->getName());
                    packet_0->writeInt32(m_Level);
                    uint32_t WOffset = packet_0->getWOffset();
                    packet_0->setSize(WOffset);
                    GameService::instance()->sendPacketTo(Owner_0->getConnId(), Owner_0->getGateIndex(), packet_0);
                }
            }

            Answer::NetPacket *packet = GameService::instance()->popNetpacket(pPlayer->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2CD6);
            if (packet)
            {
                packet->writeInt32(454);
                packet->writeInt64(m_Owner);
                packet->writeUTF8(m_Name);
                packet->writeInt32(StaticObj::getMapId(this));
                packet->writeInt16(StaticObj::getCurrentTile(this).x);
                packet->writeInt16(StaticObj::getCurrentTile(this).y);
                packet->writeInt64(pPlayer->getCid());
                packet->writeUTF8(pPlayer->getName());
                uint32_t WOffset = packet->getWOffset();
                packet->setSize(WOffset);
                GameService::instance()->worldBroadcast(packet);
            }
        }
    }
}

bool Trailer::appendInfo(Answer::NetPacket *packet)
{
    if (!packet)
        return false;

    packet->writeInt8(getType());
    packet->writeInt64(getId());
    packet->writeInt32(m_cfgTrailer.nId);
    packet->writeInt8(m_State);
    packet->writeInt64(m_Owner);
    packet->writeUTF8(m_Name);
    packet->writeInt16(StaticObj::getCurrentTile(this).x);
    packet->writeInt16(StaticObj::getCurrentTile(this).y);
    packet->writeInt16(getTargetTile().x);
    packet->writeInt16(getTargetTile().y);
    packet->writeInt32(getSpeed());
    packet->writeInt32(getMaxSpeed());
    packet->writeInt32(getHp());
    return true;
}

void Trailer::LeaveMap()
{
    if (m_pMap)
    {
        Map::removeTrailer(m_pMap, this);
        Unit::broadcastLeave(this);
        m_pMap = nullptr;
    }
}

void Trailer::EnterMap(Map *pMap, int32_t tx, int32_t ty)
{
    if (pMap)
    {
        Map::addTrailer(pMap, this, tx, ty);
        m_pMap = pMap;
    }
}

void Trailer::SendInfo(Player *player)
{
    if (player && m_pMap)
    {
        Answer::NetPacket *packet = GameService::instance()->popNetpacket(player->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2EF0);
        if (packet)
        {
            packet->writeInt32(m_cfgTrailer.TrailerQuality);
            int32_t v5 = m_nEndTime - getNow();
            packet->writeInt32(v5);
            packet->writeInt32(m_Level);
            packet->writeInt8(Robbery());
            packet->writeUTF8(m_RobberyName);
            packet->writeInt64(m_RobberyId);
            packet->writeInt32(m_RewardTimes);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::instance()->sendPacketTo(player->getConnId(), player->getGateIndex(), packet);
        }
    }
}

void Trailer::SendPos(Player *player)
{
    if (player && m_pMap)
    {
        Answer::NetPacket *packet = GameService::instance()->popNetpacket(player->getConnId(), Answer::PackType::PACK_DISPATCH, 0x2EF1);
        if (packet)
        {
            packet->writeInt32(Map::GetMapId(m_pMap));
            packet->writeInt32(StaticObj::getCurrentTile(this).x);
            packet->writeInt32(StaticObj::getCurrentTile(this).y);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::instance()->sendPacketTo(player->getConnId(), player->getGateIndex(), packet);
        }
    }
}

bool Trailer::isLifeEnd()
{
    return m_nEndTime > 0 && getNow() > m_nEndTime;
}

void Trailer::CheckTrailerEnd()
{
    if (m_nEndTime > 0 && getNow() > m_nEndTime)
    {
        Player *pPlayer = GameService::instance()->getPlayer(m_Owner, StaticObj::GetRunnerId(this), 1);
        if (pPlayer)
        {
            pPlayer->GetCMoneyRewardTask()->SendTRailerEnd();
            pPlayer->GetCMoneyRewardTask()->SetTrailerTime(0);
            pPlayer->setTrailer(nullptr);
            pPlayer->GetCMoneyRewardTask()->ResetTrailer();
        }
    }
}

float Trailer::GetRate()
{
    if (m_Level <= 299)
        return 1.0f;
    if (m_Level <= 399)
        return 1.125f;
    if (m_Level <= 499)
        return 1.25f;
    if (m_Level > 599)
        return 1.5f;
    return 1.375f;
}

int32_t Trailer::GetRewardTimes()
{
    if (m_RewardTimes <= 0 || m_RewardTimes > 2)
        m_RewardTimes = 1;
    return m_RewardTimes;
}
