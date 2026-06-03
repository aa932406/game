#include "common.h"
#include "Other/CShiZhuang.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "DBService.h"

CShiZhuang::CShiZhuang()
    : CExtSystemBase()
{
    m_ShiZhuang.Clear();
}

CShiZhuang::~CShiZhuang()
{
}

void CShiZhuang::OnCleanUp()
{
}

void CShiZhuang::OnUpdate(int64_t curTick)
{
}

void CShiZhuang::OnDaySwitch(int32_t nDiffDays)
{
}

void CShiZhuang::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_ShiZhuang = dbData->m_ShiZhangDBData.m_ShiZhuang;
}

void CShiZhuang::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_ShiZhangDBData.m_ShiZhuang = m_ShiZhuang;
}

void CShiZhuang::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(701);
    procList->push_back(702);
    procList->push_back(703);
    procList->push_back(704);
}

int32_t CShiZhuang::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 701:
        return OnGetData(inPacket);
    case 702:
        return OnActivate(inPacket);
    case 703:
        return OnUpgrade(inPacket);
    case 704:
        return OnDecompose(inPacket);
    default:
        return 2;
    }
}

int32_t CShiZhuang::OnGetData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D80);
    if (!packet)
        return 2;
    m_ShiZhuang.WriteToPacket(packet);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CShiZhuang::OnActivate(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nShiZhuangId = inPacket->readInt32();
    if (m_ShiZhuang.HasData(nShiZhuangId))
        return 2;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuangTable* pTable = CfgData::GetShiZhuangTable(v4);
    const CfgShiZhuang* pCfg = CfgShiZhuangTable::GetRow(pTable, nShiZhuangId);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_ActivateCost, CURRENCY_CHANGE_REASON::GCR_SHIZHUANG_ACTIVATE, 0))
        return 2;
    m_ShiZhuang.AddData(nShiZhuangId, 1);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v9 = Player::getConnId(m_pPlayer);
    GameService* v10 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v10, v9, GateIndex, Proc, 0);
    DBService::GetInstance()->ReplaceShiZhuang(Player::getConnId(m_pPlayer), nShiZhuangId, 1);
    return 0;
}

int32_t CShiZhuang::OnUpgrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nShiZhuangId = inPacket->readInt32();
    if (!m_ShiZhuang.HasData(nShiZhuangId))
        return 2;
    int32_t nCurLvl = m_ShiZhuang.GetData(nShiZhuangId);
    if (nCurLvl >= 10)
        return 2;
    CfgData* v5 = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuangLvlTable* pLvlTable = CfgData::GetShiZhuangLvlTable(v5);
    const CfgShiZhuangLvl* pCfg = CfgShiZhuangLvlTable::GetRow(pLvlTable, nCurLvl + 1);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_UpgradeCost, CURRENCY_CHANGE_REASON::GCR_SHIZHUANG_UPGRADE, 0))
        return 2;
    m_ShiZhuang.AddData(nShiZhuangId, nCurLvl + 1);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v10 = Player::getConnId(m_pPlayer);
    GameService* v11 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v11, v10, GateIndex, Proc, 0);
    DBService::GetInstance()->ReplaceShiZhuang(Player::getConnId(m_pPlayer), nShiZhuangId, nCurLvl + 1);
    return 0;
}

int32_t CShiZhuang::OnDecompose(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nShiZhuangId = inPacket->readInt32();
    if (!m_ShiZhuang.HasData(nShiZhuangId))
        return 2;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuangTable* pTable = CfgData::GetShiZhuangTable(v4);
    const CfgShiZhuang* pCfg = CfgShiZhuangTable::GetRow(pTable, nShiZhuangId);
    if (!pCfg)
        return 2;
    int32_t nRewardId = pCfg->m_DecomposeReward;
    if (nRewardId > 0)
    {
        const CfgRewardTable* pRewardTable = CfgData::GetRewardTable(v4);
        MemChrBagVector rewardItems;
        CfgRewardTable::GetRewardById(pRewardTable, &rewardItems, nRewardId);
        CExtCharBag* pBag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::AddItemsAndMingGe(pBag, &rewardItems, ITEM_CHANGE_REASON::ICR_SHIZHUANG_DECOMPOSE))
            return 2;
    }
    m_ShiZhuang.RemoveData(nShiZhuangId);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v13 = Player::getConnId(m_pPlayer);
    GameService* v14 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v14, v13, GateIndex, Proc, 0);
    DBService::GetInstance()->DeleteShiZhuang(Player::getConnId(m_pPlayer), nShiZhuangId);
    return 0;
}

void CShiZhuang::CheckSuitInfo()
{
    if (!m_pPlayer)
        return;
    // 检查套装信息
    for (int8_t i = 0; i <= 2; ++i)
    {
        int32_t nWearId = GetWearId(i);
        if (nWearId > 0)
        {
            CfgData* cfg = Answer::Singleton<CfgData>::instance();
            const CfgShiZhuang* pCfg = CfgShiZhuangTable::GetRow(
                CfgData::GetShiZhuangTable(cfg), nWearId);
            if (pCfg && pCfg->nSuitId > 0)
            {
                // 记录套装信息
            }
        }
    }
    CheckSuitEffect();
}

bool CShiZhuang::ActiveWithItem(int32_t nId, int32_t nAddTime)
{
    if (!m_pPlayer)
        return false;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuangTable* pTable = CfgData::GetShiZhuangTable(cfg);
    const CfgShiZhuang* pCfg = CfgShiZhuangTable::GetRow(pTable, nId);
    if (!pCfg)
        return false;

    int8_t nType = (int8_t)pCfg->nType;
    if (nType > 2)
        return false;

    int32_t nEndTime = 0;
    if (nAddTime > 0)
        nEndTime = Unit::getNow(m_pPlayer) + nAddTime;

    m_ShiZhuang.AddData(nId, nEndTime);
    wear(nType, nId);
    return true;
}

int32_t CShiZhuang::GetShiZhuangLevel(int8_t nType)
{
    if ((uint8_t)nType > 2u)
        return 0;
    int32_t nWearId = GetWearId(nType);
    if (nWearId <= 0)
        return 0;
    return m_ShiZhuang.GetData(nWearId);
}

void CShiZhuang::PackShiZhuangInfo(Answer::NetPacket* packet)
{
    if (!packet)
        return;
    for (int8_t i = 0; i <= 2; ++i)
        appendShiZhuangInfo(i, packet);
}

int32_t CShiZhuang::onRequestInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t nType = inPacket->readInt8();
    sendShiZhuangInfo(nType);
    return 0;
}

int32_t CShiZhuang::onPeiYang(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t nType = inPacket->readInt8();
    int32_t nCostItemId = inPacket->readInt32();
    int32_t nCostCount = inPacket->readInt32();

    if ((uint8_t)nType > 2u)
        return 2;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuangLvlTable* pLvlTable = CfgData::GetShiZhuangLvlTable(cfg);
    int32_t nCurLevel = GetShiZhuangLevel(nType);
    if (nCurLevel <= 0)
        return 2;

    // 检查消耗物品
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (!pBag->HasItem(nCostItemId, 4, nCostCount))
        return 2;

    // 消耗物品
    ItemData costItem;
    costItem.m_nId = nCostItemId;
    costItem.m_nClass = 4;
    costItem.m_nCount = nCostCount;
    if (!CExtCharBag::RemoveItem(pBag, &costItem, ITEM_CHANGE_REASON::ICR_BAG_USE))
        return 2;

    // 计算培养经验
    const CfgItem* pItemCfg = CfgData::GetItem(cfg, nCostItemId);
    int32_t nAddExp = pItemCfg ? pItemCfg->in_value : 0;
    if (nAddExp <= 0)
        nAddExp = nCostCount;

    addShiZhuangExp(nType, nAddExp);

    // 判断是否暴击
    int8_t nCrit = 0;
    if (pItemCfg)
    {
        Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
        int32_t nRand = Answer::Random::generate(pRand, 1, 100);
        if (nRand <= 10)
            nCrit = 2;  // 大暴击
        else if (nRand <= 30)
            nCrit = 1;  // 小暴击
    }

    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(gs, ConnId, GateIndex, Proc, 0);
    sendShiZhuangPeiYangResult(nType, nAddExp, nCrit);
    return 0;
}

void CShiZhuang::sendShiZhuangInfo(int8_t nType)
{
    if (!m_pPlayer)
        return;
    if ((uint8_t)nType > 2u)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D80);
    if (!packet)
        return;

    appendShiZhuangInfo(nType, packet);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, ConnId, GateIndex, packet);
}

void CShiZhuang::addShiZhuangExp(int8_t nType, int32_t nAddExp)
{
    if ((uint8_t)nType > 2u || !m_pPlayer)
        return;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nCurLevel = GetShiZhuangLevel(nType);
    if (nCurLevel <= 0)
        return;

    int32_t nWearId = GetWearId(nType);
    if (nWearId <= 0)
        return;

    int32_t nNewLevel = GetNewShiZhuangLevel(nWearId);
    if (nNewLevel > nCurLevel)
    {
        // 升级了
        m_ShiZhuang.AddData(nWearId, nNewLevel);
        CheckSuitEffect();
        AddPlayerAttr();
        sendShiZhuangInfo(nType);
        return;
    }
}

int32_t CShiZhuang::GetNewShiZhuangLevel(int32_t nId)
{
    if (!m_pPlayer || nId <= 0)
        return 0;

    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuang* pCfg = CfgShiZhuangTable::GetRow(
        CfgData::GetShiZhuangTable(cfg), nId);
    if (!pCfg)
        return 0;

    int32_t nCurLevel = m_ShiZhuang.GetData(nId);
    int32_t nType = pCfg->nType;

    if (nCurLevel >= 10)
        return nCurLevel;

    const CfgShiZhuangLvlTable* pLvlTable = CfgData::GetShiZhuangLvlTable(cfg);
    const CfgShiZhuangLvl* pLevelCfg = CfgShiZhuangLvlTable::GetRow(pLvlTable, nCurLevel + 1);
    if (!pLevelCfg)
        return nCurLevel;

    // 检查等级限制
    if (pLevelCfg->nNeedLevel > Player::getLevel(m_pPlayer))
        return nCurLevel;

    return nCurLevel + 1;
}

void CShiZhuang::wear(int8_t nType, int32_t nId)
{
    if (!m_pPlayer || (uint8_t)nType > 2u)
        return;
    m_WearId[nType] = nId;
    CheckSuitInfo();
    AddPlayerAttr();
    sendShiZhuangInfo(nType);
}

int32_t CShiZhuang::GetWearId(int8_t nType)
{
    if ((uint8_t)nType > 2u)
        return 0;
    return m_WearId[nType];
}

void CShiZhuang::sendShiZhuangPeiYangResult(int8_t nType, int32_t nExp, int8_t nCrit)
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D81);
    if (!packet)
        return;

    packet->writeInt8(nType);
    packet->writeInt32(nExp);
    packet->writeInt8(nCrit);
    int32_t nCurLevel = GetShiZhuangLevel(nType);
    packet->writeInt32(nCurLevel);

    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(gs, ConnId, GateIndex, packet);
}

int32_t CShiZhuang::onWear(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t nType = inPacket->readInt8();
    int32_t nId = inPacket->readInt32();

    if ((uint8_t)nType > 2u)
        return 2;

    if (!m_ShiZhuang.HasData(nId))
        return 2;

    wear(nType, nId);
    return 0;
}

int32_t CShiZhuang::onLevelUp(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t nType = inPacket->readInt8();

    if ((uint8_t)nType > 2u)
        return 2;

    int32_t nWearId = GetWearId(nType);
    if (nWearId <= 0)
        return 2;

    int32_t nOldLevel = GetShiZhuangLevel(nType);
    int32_t nNewLevel = GetNewShiZhuangLevel(nWearId);

    if (nNewLevel <= nOldLevel)
        return 2;

    // 扣费
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuangLvlTable* pLvlTable = CfgData::GetShiZhuangLvlTable(cfg);
    const CfgShiZhuangLvl* pLevelCfg = CfgShiZhuangLvlTable::GetRow(pLvlTable, nOldLevel + 1);
    if (!pLevelCfg)
        return 2;

    if (pLevelCfg->nCostItemId > 0)
    {
        CExtCharBag* pBag = Player::GetBag(m_pPlayer);
        if (!pBag->HasItem(pLevelCfg->nCostItemId, 4, pLevelCfg->nCostCount))
            return 2;
        ItemData costItem;
        costItem.m_nId = pLevelCfg->nCostItemId;
        costItem.m_nClass = 4;
        costItem.m_nCount = pLevelCfg->nCostCount;
        if (!CExtCharBag::RemoveItem(pBag, &costItem, ITEM_CHANGE_REASON::ICR_BAG_USE))
            return 2;
    }

    m_ShiZhuang.AddData(nWearId, nNewLevel);
    CheckSuitEffect();
    AddPlayerAttr();
    sendShiZhuangInfo(nType);
    return 0;
}

void CShiZhuang::appendShiZhuangInfo(int8_t nType, Answer::NetPacket* packet)
{
    if (!packet || (uint8_t)nType > 2u)
        return;

    int32_t nWearId = GetWearId(nType);
    packet->writeInt8(nType);
    packet->writeInt32(nWearId);
    packet->writeInt32(m_ShiZhuang.GetData(nWearId));
}

void CShiZhuang::CheckSuitEffect()
{
    if (!m_pPlayer)
        return;

    // 清除旧套装效果
    m_SuitAttr.clear();

    // 检查当前穿戴的时装是否有套装
    int32_t nSuitIds[3] = {0, 0, 0};
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const CfgShiZhuangTable* pTable = CfgData::GetShiZhuangTable(cfg);

    for (int8_t i = 0; i <= 2; ++i)
    {
        int32_t nWearId = GetWearId(i);
        if (nWearId > 0)
        {
            const CfgShiZhuang* pCfg = CfgShiZhuangTable::GetRow(pTable, nWearId);
            if (pCfg)
                nSuitIds[i] = pCfg->nSuitId;
        }
    }

    // 如果三个部位的套装ID一致,激活套装效果
    if (nSuitIds[0] > 0 && nSuitIds[0] == nSuitIds[1] && nSuitIds[1] == nSuitIds[2])
    {
        const CfgShiZhuangSuitTable* pSuitTable = CfgData::GetShiZhuangSuitTable(cfg);
        const CfgShiZhuangSuit* pSuit = CfgShiZhuangSuitTable::GetRow(pSuitTable, nSuitIds[0]);
        if (pSuit)
        {
            m_SuitAttr = pSuit->vAttr;
        }
    }
}

void CShiZhuang::AddPlayerAttr()
{
    if (!m_pPlayer)
        return;

    // 重新计算所有属性
    for (int8_t i = 0; i <= 2; ++i)
    {
        int32_t nWearId = GetWearId(i);
        if (nWearId > 0)
        {
            int32_t nLevel = GetShiZhuangLevel(i);
            CfgData* cfg = Answer::Singleton<CfgData>::instance();
            const CfgShiZhuangLvlTable* pLvlTable = CfgData::GetShiZhuangLvlTable(cfg);
            const CfgShiZhuangLvl* pLevelCfg = CfgShiZhuangLvlTable::GetRow(pLvlTable, nLevel);
            if (pLevelCfg)
            {
                for (const auto& attr : pLevelCfg->vAttr)
                    Player::AddCharAttr(m_pPlayer, attr.index, attr.addon);
            }
        }
    }

    // 应用套装属性
    for (const auto& attr : m_SuitAttr)
        Player::AddCharAttr(m_pPlayer, attr.index, attr.addon);

    Player::RecalcAttr(m_pPlayer);
}

void CShiZhuang::SendShiZhuangInfo()
{
    for (int8_t i = 0; i <= 2; ++i)
        sendShiZhuangInfo(i);
}

bool CShiZhuang::IsForever(int8_t nType, int32_t nId)
{
    if ((uint8_t)nType > 2u)
        return false;
    return m_ShiZhuang.GetEndTime(nId) == 0;
}
