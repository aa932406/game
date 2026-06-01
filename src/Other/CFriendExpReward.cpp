#include "Other/CFriendExpReward.h"
#include "GameService.h"
#include "Player.h"
#include "Character/CExtOperateLimit.h"
#include "Network/NetPacket.h"
#include "Other/StaticObj.h"

CFriendExpReward::CFriendExpReward()
{
    OnCleanUp();
}

CFriendExpReward::~CFriendExpReward()
{
}

void CFriendExpReward::OnCleanUp()
{
    m_DieRecord.clear();
}

void CFriendExpReward::OnUpdate(int64_t curTick)
{
}

void CFriendExpReward::OnDaySwitch(int32_t nDiffDays)
{
}

void CFriendExpReward::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CFriendExpReward::OnSaveToDB(PlayerDBData* dbData)
{
}

void CFriendExpReward::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(20085);
}

int32_t CFriendExpReward::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    if (nProcId == 20085)
        return OnZuiZhong(inPacket);
    return 0;
}

void CFriendExpReward::AddDieRecord(int64_t Cid, std::string* p_Name, int32_t MapId, int32_t Mid)
{
    if (!m_pPlayer)
        return;

    DieRecord stu;
    stu.KillTime = m_pPlayer->getNow();
    stu.MapId = MapId;
    stu.MonsterId = Mid;
    stu.Cid = Cid;
    stu.Name = *p_Name;

    if (m_DieRecord.size() > 19)
        m_DieRecord.pop_front();
    m_DieRecord.push_back(stu);
    SendOneDieRecord(&stu);
}

void CFriendExpReward::SendDieBoard()
{
    if (!m_pPlayer || m_DieRecord.empty() || m_pPlayer->isAlive())
        return;

    DieRecord& stu = m_DieRecord.back();
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2E95);
    if (!packet)
        return;

    packet->writeInt32(stu.MapId);
    if (stu.MonsterId <= 0)
    {
        packet->writeInt8(2);
        packet->writeInt64(stu.Cid);
        packet->writeUTF8(&stu.Name);
    }
    else
    {
        packet->writeInt8(1);
        packet->writeInt32(stu.MonsterId);
    }

    int32_t nNowTime = Unit::getNow(m_pPlayer);
    CExtOperateLimit* operateLimit = Player::GetOperateLimit(m_pPlayer);
    int32_t nReviveTime = operateLimit->GetLimitCount(1111);
    if (nNowTime >= nReviveTime)
        packet->writeInt32(0);
    else
        packet->writeInt32(nReviveTime - nNowTime);

    int32_t limitCount = Player::GetOperateLimit(m_pPlayer)->GetLimitCount(1112);
    packet->writeInt32(limitCount);

    uint32_t wOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

void CFriendExpReward::SendOneDieRecord(DieRecord* Stu)
{
    if (!m_pPlayer)
        return;

    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* pNetData = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2D00);
    if (!pNetData)
        return;

    pNetData->writeInt8(1);
    pNetData->writeInt8(1);
    pNetData->writeInt32(Stu->KillTime);
    pNetData->writeInt32(Stu->MapId);
    if (Stu->MonsterId <= 0)
    {
        pNetData->writeInt8(2);
        pNetData->writeInt64(Stu->Cid);
        pNetData->writeUTF8(&Stu->Name);
    }
    else
    {
        pNetData->writeInt8(1);
        pNetData->writeInt32(Stu->MonsterId);
    }

    uint32_t wOffset = Answer::NetPacket::getWOffset(pNetData);
    Answer::NetPacket::setSize(pNetData, wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, pNetData);
}

void CFriendExpReward::SendDieRecord()
{
    if (!m_pPlayer)
        return;

    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* pNetData = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2D00);
    if (!pNetData)
        return;

    pNetData->writeInt8(2);
    pNetData->writeInt8(static_cast<int8_t>(m_DieRecord.size()));
    for (auto& record : m_DieRecord)
    {
        pNetData->writeInt32(record.KillTime);
        pNetData->writeInt32(record.MapId);
        if (record.MonsterId <= 0)
        {
            pNetData->writeInt8(2);
            pNetData->writeInt64(record.Cid);
            pNetData->writeUTF8(&record.Name);
        }
        else
        {
            pNetData->writeInt8(1);
            pNetData->writeInt32(record.MonsterId);
        }
    }

    uint32_t wOffset = Answer::NetPacket::getWOffset(pNetData);
    Answer::NetPacket::setSize(pNetData, wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, pNetData);
}

int32_t CFriendExpReward::OnZuiZhong(Answer::NetPacket* pData)
{
    if (!pData || !m_pPlayer)
        return 10002;

    int64_t TargetId = pData->readInt64();
    int32_t bagslot = pData->readInt32();
    int32_t RunnerId = StaticObj::GetRunnerId(m_pPlayer);

    GameService* gs = Answer::Singleton<GameService>::instance();
    Player* pTarget = GameService::getPlayer(gs, TargetId, RunnerId, 1);
    if (!pTarget)
        return 10002;

    if (Unit::HasBuffById(pTarget, 160))
    {
        int16_t gateIndex = Player::getGateIndex(m_pPlayer);
        int8_t connId = Player::getConnId(m_pPlayer);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        return GameService::replyfailure(gs2, connId, gateIndex, 0xC33, 12, 0);
    }

    if (StaticObj::InActivity(pTarget) || StaticObj::InDungeon(pTarget))
    {
        int16_t gateIndex = Player::getGateIndex(m_pPlayer);
        int8_t connId = Player::getConnId(m_pPlayer);
        GameService* gs3 = Answer::Singleton<GameService>::instance();
        return GameService::replyfailure(gs3, connId, gateIndex, 0xC33, 13, 0);
    }

    int32_t result = 0;
    if (bagslot < 0)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, 100, CURRENCY_CHANGE_REASON::GCR_ZHUI_ZONG, 0))
            return 10002;
    }
    else
    {
        Int32Vector vSlot;
        vSlot.push_back(bagslot);
        ItemData data;
        data.m_nId = 0x100000245;
        data.m_nCount = 1;
        CExtCharBag* bag = Player::GetBag(m_pPlayer);
        if (!bag->RemoveItem(&vSlot, &data, ITEM_CHANGE_REASON::ICR_ZHUI_ZONG))
        {
            result = 10002;
        }
    }

    if (result != 0)
        return result;

    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs4 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* pNetData = GameService::popNetpacket(gs4, connId2, Answer::PackType::PACK_DISPATCH, 0x2D01);
    if (pNetData)
    {
        pNetData->writeInt64(pTarget->getCid());
        pNetData->writeInt32(StaticObj::getMapId(pTarget));
        pNetData->writeInt32(StaticObj::GetPosX(pTarget));
        pNetData->writeInt32(StaticObj::GetPosY(pTarget));

        uint32_t wOffset = Answer::NetPacket::getWOffset(pNetData);
        Answer::NetPacket::setSize(pNetData, wOffset);
        int16_t gateIndex2 = Player::getGateIndex(m_pPlayer);
        int8_t connId3 = Player::getConnId(m_pPlayer);
        GameService* gs5 = Answer::Singleton<GameService>::instance();
        GameService::sendPacketTo(gs5, connId3, gateIndex2, pNetData);
    }

    return 0;
}
