#include "Game/CFaBao.h"
#include "Game/Player.h"
#include "Game/GameService.h"
#include "Game/DBService.h"
#include "Other/CfgData.h"
#include "Other/FaBaoTable.h"
#include "Other/BuyFaBaoResCfg.h"
#include "Other/BenefitType.h"
#include "Other/CExtCharPet.h"
#include "Other/CExtSystemBase.h"
#include "Other/ItemChangeReason.h"
#include "Utility/AttriButie.h"

#include <cstring>
#include <algorithm>
#include <string>

CFaBao::CFaBao()
{
    OnCleanUp();
}

CFaBao::~CFaBao()
{
}

void CFaBao::OnCleanUp()
{
    memset(m_FaBao, 0, sizeof(m_FaBao));
}

void CFaBao::OnLoadFromDB(const PlayerDBData *dbData)
{
    memcpy(m_FaBao, dbData->m_FaBaoData.m_FaBao, sizeof(m_FaBao));
}

void CFaBao::OnSaveToDB(PlayerDBData *dbData)
{
    memcpy(dbData->m_FaBaoData.m_FaBao, m_FaBao, sizeof(dbData->m_FaBaoData.m_FaBao));
}

void CFaBao::GetInterestsProtocol(ProcIdList *procList)
{
    procList->push_back(447);
    procList->push_back(439);
    procList->push_back(446);
    procList->push_back(440);
    procList->push_back(441);
}

int32_t CFaBao::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket *inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 2;

    switch (nProcId)
    {
        case 0x1B9:
            return onFaBaoPeiYang(inPacket);
        case 0x1BE:
            return onBuyFaBaoRes(inPacket);
        case 0x1B8:
            return onAskFaBaoInfo(inPacket);
        case 0x1B7:
            return onAskFaBaoInfo(inPacket);
    }
    return 2;
}

int32_t CFaBao::onAskFaBaoInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t nType = Answer::NetPacket::readInt8(inPacket);
    sendFaBaoInfo(nType);
    return 0;
}

int32_t CFaBao::onBuyFaBaoRes(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int32_t id = Answer::NetPacket::readInt32(inPacket);
    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const BuyFaBaoResCfg *pResCfg = CfgData::GetBuyFaBaoResCfg(pCfgData, id);

    if (!pResCfg)
        return 10002;

    if (!IsFaBaoType(pResCfg->FaBaoResType))
        return 10002;

    if (pResCfg->GetResValues <= 0 || pResCfg->NeedGold <= 0)
        return 10002;

    if (Player::DecCurrency(m_pPlayer, CURRENCY_GOLD, pResCfg->NeedGold))
    {
        AddFaBaoRes(pResCfg->FaBaoResType, pResCfg->GetResValues);
    }
    return 2;
}

int32_t CFaBao::onFaBaoPeiYang(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t nType = Answer::NetPacket::readInt8(inPacket);
    if (!IsFaBaoType(nType))
        return 10002;

    int32_t curLevel = m_FaBao[nType].FaBaoLevel;
    CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
    const FaBaoTable *pFaBaoTable = CfgData::GetFaBaoTable(pCfgData);
    const CfgFaBaoLevel *pFaBaoLevel = FaBaoTable::GetFaBaoLevel(pFaBaoTable, nType, curLevel);

    if (!pFaBaoLevel)
        return 10002;

    int32_t needLevel = pFaBaoLevel->NeedLevel;
    int32_t playerLevel = Player::getLevel(m_pPlayer);
    if (playerLevel < needLevel)
        return 10002;

    int32_t needItem = pFaBaoLevel->NeedItemId;
    if (needItem > 0)
    {
        // 检查是否有足够的道具
    }

    int32_t needGold = pFaBaoLevel->NeedGold;
    if (needGold > 0)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_GOLD, needGold))
            return 10002;
    }

    m_FaBao[nType].FaBaoLevel = curLevel + 1;

    // 添加属性
    AddPlayerAttr();

    // 发送更新信息
    sendFaBaoInfo(nType);

    // 检查是否需要世界公告
    const CfgFaBaoLevel *pNextLevel = FaBaoTable::GetFaBaoLevel(pFaBaoTable, nType, curLevel + 1);
    if (pNextLevel && pNextLevel->GongGaoId > 0)
    {
        broadcastLevelUp(nType, pNextLevel->GongGaoId);
        int32_t GongGaoId = pNextLevel->GongGaoId;
        (void)GongGaoId;
        // 记录日志
    }

    // 记录培养日志
    PlatformLog::logFaBaoPeiYang(m_pPlayer, nType, curLevel, curLevel + 1);

    return 0;
}

void CFaBao::broadcastLevelUp(int8_t Type, int32_t GongGaoId)
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, ConnId, Answer::PackType::PACK_DISPATCH, 0x1B7);
    if (!packet)
        return;

    CharId_t Cid = Player::getCid(m_pPlayer);
    Answer::NetPacket::writeInt8(packet, Type);
    Answer::NetPacket::writeInt64(packet, Cid);
    Answer::NetPacket::writeInt32(packet, m_FaBao[Type].FaBaoLevel);
    Answer::NetPacket::writeInt32(packet, 0);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int8_t v7 = Player::getConnId(m_pPlayer);
    GameService *v8 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v8, v7, 0, packet);

    // 世界公告
    if (GongGaoId > 0)
    {
        std::string val;
        // 格式化公告
        GameService::broadcastSystem(GongGaoId, &val);
    }
}

void CFaBao::AddPlayerAttr()
{
    if (!m_pPlayer)
        return;

    for (int8_t i = 0; i < 4; ++i)
    {
        int32_t FaBaoLevel = m_FaBao[i].FaBaoLevel;
        if (FaBaoLevel <= 0)
            continue;

        CfgData *pCfgData = Answer::Singleton<CfgData>::instance();
        const FaBaoTable *pFaBaoTable = CfgData::GetFaBaoTable(pCfgData);
        const CfgFaBaoLevel *pCfgLevel = FaBaoTable::GetFaBaoLevel(pFaBaoTable, i, FaBaoLevel);

        if (!pCfgLevel)
            continue;

        for (size_t j = 0; j < pCfgLevel->vAttr.size(); ++j)
        {
            int32_t addon = pCfgLevel->vAttr[j].addon;
            int32_t index = pCfgLevel->vAttr[j].index;
            Player::AddOtherAttr(m_pPlayer, static_cast<CObjAttrs::Index_T>(index), addon);
        }
    }
}

void CFaBao::sendFaBaoInfo(int8_t nType)
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, ConnId, Answer::PackType::PACK_DISPATCH, 0x1B7);
    if (!packet)
        return;

    Answer::NetPacket::writeInt8(packet, nType);
    Answer::NetPacket::writeInt32(packet, m_FaBao[nType].FaBaoLevel);
    Answer::NetPacket::writeInt32(packet, m_FaBao[nType].FaBaoRes);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v6 = Player::getConnId(m_pPlayer);
    GameService *v7 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v7, v6, GateIndex, packet);
}

void CFaBao::SendAllFaBaoInfo()
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGameService = Answer::Singleton<GameService>::instance();

    Answer::NetPacket *packet = GameService::popNetpacket(pGameService, ConnId, Answer::PackType::PACK_DISPATCH, 0x1B6);
    if (!packet)
        return;

    for (int32_t i = 0; i < 4; ++i)
    {
        Answer::NetPacket::writeInt32(packet, m_FaBao[i].FaBaoLevel);
        Answer::NetPacket::writeInt32(packet, m_FaBao[i].FaBaoRes);
    }

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);

    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v5 = Player::getConnId(m_pPlayer);
    GameService *v6 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v6, v5, GateIndex, packet);
}

void CFaBao::AddFaBaoRes(int8_t nType, int32_t Values)
{
    if (!m_pPlayer || !IsFaBaoType(nType) || Values <= 0)
        return;

    m_FaBao[nType].FaBaoRes += Values;

    // 检查双十一活动加成
    BenefitType benefit = ::GetBenefitType(m_pPlayer, BENEFIT_FABAO);
    int32_t extraRes = static_cast<int32_t>(m_FaBao[nType].FaBaoRes * benefit / 100);
    if (extraRes > 0)
    {
        m_FaBao[nType].FaBaoRes += extraRes;
    }

    sendFaBaoInfo(nType);
}

int32_t CFaBao::GetFaBaoLevel(int8_t Type)
{
    if (!IsFaBaoType(Type))
        return 0;
    return m_FaBao[Type].FaBaoLevel;
}

bool CFaBao::IsFaBaoType(int8_t nType)
{
    return nType >= 0 && nType < 4;
}
