#include "Other/CTianLing.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Other/CFunctionOpen.h"
#include "Character/CExtCharSkill.h"
#include "Character/CExtCurrency.h"
#include "Character/CExtCharBag.h"
#include "Character/CExtOperateLimit.h"
#include "Other/important_system_log.h"
#include "Database/DBService.h"
#include <algorithm>
#include <string>

CTianLing::CTianLing()
    : CExtSystemBase()
{
    m_TianLingData.clear();
}

CTianLing::~CTianLing()
{
}

void CTianLing::OnCleanUp()
{
}

void CTianLing::OnUpdate(int64_t curTick)
{
}

void CTianLing::OnDaySwitch(int32_t nDiffDays)
{
}

void CTianLing::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_TianLingData = dbData->m_TianLingDBData.m_TianLingData;
}

void CTianLing::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_TianLingDBData.m_TianLingData = m_TianLingData;
}

void CTianLing::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(705);
    procList->push_back(706);
    procList->push_back(707);
    procList->push_back(708);
}

int32_t CTianLing::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 705:
        return OnGetData(inPacket);
    case 706:
        return OnActivate(inPacket);
    case 707:
        return OnUpgrade(inPacket);
    case 708:
        return OnDecompose(inPacket);
    default:
        return 2;
    }
}

int32_t CTianLing::OnGetData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D85);
    if (!packet)
        return 2;
    int32_t nSize = m_TianLingData.size();
    packet->writeInt32(nSize);
    for (auto it = m_TianLingData.begin(); it != m_TianLingData.end(); ++it)
    {
        packet->writeInt32(it->first);
        packet->writeInt32(it->second);
    }
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CTianLing::OnActivate(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nTianLingId = inPacket->readInt32();
    if (m_TianLingData.find(nTianLingId) != m_TianLingData.end())
        return 2;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgTianLingTable* pTable = CfgData::GetTianLingTable(v4);
    const CfgTianLing* pCfg = CfgTianLingTable::GetRow(pTable, nTianLingId);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_ActivateCost, CURRENCY_CHANGE_REASON::GCR_TIANLING_ACTIVATE, 0))
        return 2;
    m_TianLingData[nTianLingId] = 1;
    CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
    CExtBuffManager::AddBuff(pBuffMgr, pCfg->m_ActivateBuff, 0, 0);
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::CalcProperty(pBag);
    Player::SendMsgUpdate(m_pPlayer);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v13 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v13, ConnId, GateIndex, Proc, 0);
    return 0;
}

// ==================== 系统初始化 ====================

void CTianLing::InitSystem()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 175) && Player::getRecord(m_pPlayer, 1148) <= 0)
        Player::updateRecord(m_pPlayer, 1148, 1);
    if (CFunctionOpen::IsOpened(pFuncOpen, 235) && Player::getRecord(m_pPlayer, 37501) <= 0)
        Player::updateRecord(m_pPlayer, 37501, 1);
    if (CFunctionOpen::IsOpened(pFuncOpen, 240))
    {
        if (Player::getRecord(m_pPlayer, 37506) <= 0)
            Player::updateRecord(m_pPlayer, 37506, 1);
        int32_t CurLevel = Player::getRecord(m_pPlayer, 37506);
        CfgData* cfg = Answer::Singleton<CfgData>::instance();
        const SunAndMoonCfg* CurCfg = CfgData::GetSunAndMoonCfg(cfg, CurLevel);
        if (CurCfg && CurCfg->nSunTelentId > 0 && CurCfg->nSunTelentLevel > 0)
        {
            CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
            CExtCharSkill::AddOtherSkill(CharSkill, CurCfg->nSunTelentId, CurCfg->nSunTelentLevel);
        }
        if (CurCfg && CurCfg->nMoonTelentId > 0 && CurCfg->nMoonTelentLevel > 0)
        {
            CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
            CExtCharSkill::AddOtherSkill(CharSkill, CurCfg->nMoonTelentId, CurCfg->nMoonTelentLevel);
        }
    }
}

void CTianLing::InitTianLing()
{
    if (m_pPlayer)
        Player::updateRecord(m_pPlayer, 1148, 1);
}

void CTianLing::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    int32_t CurLevel = Player::getRecord(m_pPlayer, 1148);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    TianLingCfg* CurCfg = CfgData::GetTianLingCfg(cfg, CurLevel);
    if (CurCfg)
    {
        for (const AddAttribute& attr : CurCfg->AttrList)
        {
            Unit::AddAttrValue(m_pPlayer, static_cast<CObjAttrs::Index_T>(attr.m_nAddAttrType), attr.m_nAddAttrValue);
        }
    }
    int32_t CurLevel_0 = Player::getRecord(m_pPlayer, 37501);
    const ZhanHunCfg* CurCfg_0 = CfgData::GetZhanHunCfg(cfg, CurLevel_0);
    if (CurCfg_0)
    {
        for (const AddAttribute& attr : CurCfg_0->lAddAttrs)
        {
            Unit::AddAttrValue(m_pPlayer, static_cast<CObjAttrs::Index_T>(attr.m_nAddAttrType), attr.m_nAddAttrValue);
        }
    }
    int32_t CurLevel_1 = Player::getRecord(m_pPlayer, 37506);
    const SunAndMoonCfg* CurCfg_1 = CfgData::GetSunAndMoonCfg(cfg, CurLevel_1);
    if (CurCfg_1)
    {
        for (const AddAttribute& attr : CurCfg_1->lAttr)
        {
            Unit::AddAttrValue(m_pPlayer, static_cast<CObjAttrs::Index_T>(attr.m_nAddAttrType), attr.m_nAddAttrValue);
        }
    }
}

// ==================== 天灵升级 ====================

int32_t CTianLing::OnTianLingLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t CurLevel = Player::getRecord(m_pPlayer, 1148);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    TianLingCfg* CurCfg = CfgData::GetTianLingCfg(cfg, CurLevel);
    TianLingCfg* NextCfg = CfgData::GetTianLingCfg(cfg, CurLevel + 1);
    if (!CurCfg || !NextCfg)
        return 10002;
    if (CurCfg->CostMoney > 0)
    {
        CExtCurrency* Currency = Player::GetCurrency(m_pPlayer);
        if (CExtCurrency::GetMoneyBindAndNoBind(Currency) < CurCfg->CostMoney)
            return 10002;
    }
    if (CurCfg->CostCoin > 0 && Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_VIGOUR) < CurCfg->CostCoin)
        return 10002;
    if (!CurCfg->CostItems.empty())
    {
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::RemoveItem(Bag, &CurCfg->CostItems, ITEM_CHANGE_REASON::ICR_TIAN_LING_LEVEL))
            return 10002;
    }
    if (CurCfg->CostMoney > 0)
    {
        CExtCurrency* Currency = Player::GetCurrency(m_pPlayer);
        if (!CExtCurrency::DecMoneyAndNoBind(Currency, CurCfg->CostMoney, CURRENCY_CHANGE_REASON::MCR_TIAN_LING_LEVEL_UP, 0))
            return 10002;
    }
    if (CurCfg->CostCoin > 0 && !Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_VIGOUR, CurCfg->CostCoin, CURRENCY_CHANGE_REASON::MCR_TIAN_LING_LEVEL_UP, 0))
        return 10002;
    Player::updateRecord(m_pPlayer, 1148, CurLevel + 1);
    Player::RecalcAttr(m_pPlayer);
    Player::SetNeedSyncAround(m_pPlayer);
    if (NextCfg->GongGaoId > 0)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(NextCfg->GongGaoId);
            MString name;
            Player::getPlayerName(m_pPlayer, &name);
            packet->writeUTF8(name);
            CharId_t Cid = Player::getCid(m_pPlayer);
            packet->writeInt64(Cid);
            packet->writeInt32(CurLevel + 1);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::worldBroadcast(gs, Player::getConnId(m_pPlayer), packet);
        }
    }
    return 0;
}

// ==================== 战魂 ====================

void CTianLing::InitZhanHun()
{
    if (m_pPlayer)
        Player::updateRecord(m_pPlayer, 37501, 1);
}

int32_t CTianLing::OnZhanHunLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t CurLevel = Player::getRecord(m_pPlayer, 37501);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const ZhanHunCfg* CurCfg = CfgData::GetZhanHunCfg(cfg, CurLevel);
    const ZhanHunCfg* NextCfg = CfgData::GetZhanHunCfg(cfg, CurLevel + 1);
    if (!CurCfg || !NextCfg)
        return 10002;
    if (CurCfg->nMoney > 0)
    {
        CExtCurrency* Currency = Player::GetCurrency(m_pPlayer);
        if (CExtCurrency::GetMoneyBindAndNoBind(Currency) < CurCfg->nMoney)
            return 10002;
    }
    if (CurCfg->nSyb > 0 && Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_VIGOUR) < CurCfg->nSyb)
        return 10002;
    if (CurCfg->nMoney > 0)
    {
        CExtCurrency* Currency = Player::GetCurrency(m_pPlayer);
        if (!CExtCurrency::DecMoneyAndNoBind(Currency, CurCfg->nMoney, CURRENCY_CHANGE_REASON::MCR_TIAN_LING_LEVEL_UP, 0))
            return 10002;
    }
    if (CurCfg->nSyb > 0 && !Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_VIGOUR, CurCfg->nSyb, CURRENCY_CHANGE_REASON::MCR_TIAN_LING_LEVEL_UP, 0))
        return 10002;
    Player::updateRecord(m_pPlayer, 37501, CurLevel + 1);
    Player::RecalcAttr(m_pPlayer);
    if (NextCfg->nGongGaoId > 0)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(NextCfg->nGongGaoId);
            MString name;
            Player::getPlayerName(m_pPlayer, &name);
            packet->writeUTF8(name);
            CharId_t Cid = Player::getCid(m_pPlayer);
            packet->writeInt64(Cid);
            packet->writeInt32(CurLevel + 1);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::worldBroadcast(gs, Player::getConnId(m_pPlayer), packet);
        }
    }
    return 0;
}

// ==================== 日月 ====================

void CTianLing::InitSunAndMoon()
{
    if (!m_pPlayer)
        return;
    Player::updateRecord(m_pPlayer, 37506, 1);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const SunAndMoonCfg* CurCfg = CfgData::GetSunAndMoonCfg(cfg, 1);
    if (CurCfg)
    {
        if (CurCfg->nSunTelentId > 0 && CurCfg->nSunTelentLevel > 0)
        {
            CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
            CExtCharSkill::AddOtherSkill(CharSkill, CurCfg->nSunTelentId, CurCfg->nSunTelentLevel);
        }
        if (CurCfg->nMoonTelentId > 0 && CurCfg->nMoonTelentLevel > 0)
        {
            CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
            CExtCharSkill::AddOtherSkill(CharSkill, CurCfg->nMoonTelentId, CurCfg->nMoonTelentLevel);
        }
    }
}

int32_t CTianLing::OnSunAndMoonLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int8_t nAutoBuy = inPacket->readInt8();
    Int32Vector vSlot;
    Player::queryBagInfo(m_pPlayer, inPacket, &vSlot);
    int32_t CurLevel = Player::getRecord(m_pPlayer, 37506);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const SunAndMoonCfg* CurCfg = CfgData::GetSunAndMoonCfg(cfg, CurLevel);
    const SunAndMoonCfg* NextCfg = CfgData::GetSunAndMoonCfg(cfg, CurLevel + 1);
    if (!CurCfg || !NextCfg)
        return 10002;
    if (nAutoBuy)
    {
        int32_t nCostGold = 0;
        ItemDataList costItemList = CurCfg->lConstItems;
        for (auto iter = costItemList.begin(); iter != costItemList.end(); )
        {
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            int32_t nItemCount = CExtCharBag::GetItemCount(Bag, &vSlot, iter->m_nClass, iter->m_nId);
            int32_t nBuyCount = 0;
            if (iter->m_nCount > nItemCount)
            {
                nBuyCount = iter->m_nCount - nItemCount;
                iter->m_nCount = nItemCount;
            }
            if (nBuyCount > 0)
            {
                const CfgGameShop* pCfgGameShop = CfgData::GetGameShopItem(cfg, iter->m_nClass, iter->m_nId);
                if (!pCfgGameShop)
                    return 2;
                nCostGold += nBuyCount * pCfgGameShop->Price;
            }
            if (iter->m_nCount > 0)
                ++iter;
            else
                iter = costItemList.erase(iter);
        }
        if (nCostGold > 0 && Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < nCostGold)
            return 10002;
        if (!costItemList.empty())
        {
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            if (!CExtCharBag::RemoveItem(Bag, &vSlot, &costItemList, ITEM_CHANGE_REASON::IDCR_SUN_AND_MOON_LEVEL_UP))
                return 10002;
        }
        if (nCostGold > 0 && !Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, nCostGold, CURRENCY_CHANGE_REASON::GCR_SUN_AND_MOON_LEVEL_UP, 0))
            return 10002;
    }
    else
    {
        if (!CurCfg->lConstItems.empty())
        {
            CExtCharBag* Bag = Player::GetBag(m_pPlayer);
            if (!CExtCharBag::RemoveItem(Bag, &vSlot, &CurCfg->lConstItems, ITEM_CHANGE_REASON::IDCR_SUN_AND_MOON_LEVEL_UP))
                return 10002;
        }
    }
    Player::updateRecord(m_pPlayer, 37506, CurLevel + 1);
    if (CurCfg->nSunTelentId > 0 && CurCfg->nSunTelentLevel > 0)
    {
        CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
        CExtCharSkill::removeSkillByTalent(CharSkill, CurCfg->nSunTelentId, CurCfg->nSunTelentLevel);
    }
    if (NextCfg->nSunTelentId > 0 && NextCfg->nSunTelentLevel > 0)
    {
        CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
        CExtCharSkill::AddOtherSkill(CharSkill, NextCfg->nSunTelentId, NextCfg->nSunTelentLevel);
    }
    if (CurCfg->nMoonTelentId > 0 && CurCfg->nMoonTelentLevel > 0)
    {
        CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
        CExtCharSkill::removeSkillByTalent(CharSkill, CurCfg->nMoonTelentId, CurCfg->nMoonTelentLevel);
    }
    if (NextCfg->nMoonTelentId > 0 && NextCfg->nMoonTelentLevel > 0)
    {
        CExtCharSkill* CharSkill = Player::GetCharSkill(m_pPlayer);
        CExtCharSkill::AddOtherSkill(CharSkill, NextCfg->nMoonTelentId, NextCfg->nMoonTelentLevel);
    }
    if (NextCfg->nGongGaoId > 0)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* gs = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(gs, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6u);
        if (packet)
        {
            packet->writeInt32(NextCfg->nGongGaoId);
            MString name;
            Player::getPlayerName(m_pPlayer, &name);
            packet->writeUTF8(name);
            CharId_t Cid = Player::getCid(m_pPlayer);
            packet->writeInt64(Cid);
            packet->writeInt32(CurLevel + 1);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::worldBroadcast(gs, Player::getConnId(m_pPlayer), packet);
        }
    }
    Player::RecalcAttr(m_pPlayer);
    return 0;
}

bool CTianLing::GetMonsterItem(MemChrBag item)
{
    if (!m_pPlayer || item.itemId <= 0 || item.itemClass <= 0 || item.itemCount <= 0)
        return false;
    int32_t CurLevel = Player::getRecord(m_pPlayer, 37506);
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const SunAndMoonCfg* CurCfg = CfgData::GetSunAndMoonCfg(cfg, CurLevel);
    if (!CurCfg)
        return false;
    if (CurCfg->nGetItemTimes - Player::getRecord(m_pPlayer, 2131) <= 0)
    {
        Player::TiShiInfo(m_pPlayer, 32, 0);
        return false;
    }
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (CExtCharBag::AddItem(Bag, &item, ITEM_CHANGE_REASON::IDCR_MOON_SKILL_GET))
    {
        CExtOperateLimit* OperateLimit = Player::GetOperateLimit(m_pPlayer);
        CExtOperateLimit::AddLimitCount(OperateLimit, 2131, 1);
        return true;
    }
    return false;
}

int32_t CTianLing::OnUpgrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nTianLingId = inPacket->readInt32();
    auto it = m_TianLingData.find(nTianLingId);
    if (it == m_TianLingData.end())
        return 2;
    int32_t nCurLvl = it->second;
    if (nCurLvl >= 50)
        return 2;
    CfgData* v5 = Answer::Singleton<CfgData>::instance();
    const CfgTianLingLvlTable* pLvlTable = CfgData::GetTianLingLvlTable(v5);
    const CfgTianLingLvl* pCfg = CfgTianLingLvlTable::GetRow(pLvlTable, nCurLvl + 1);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_UpgradeCost, CURRENCY_CHANGE_REASON::GCR_TIANLING_UPGRADE, 0))
        return 2;
    m_TianLingData[nTianLingId] = nCurLvl + 1;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v12 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v12, ConnId, GateIndex, Proc, 0);
    return 0;
}

int32_t CTianLing::OnDecompose(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nTianLingId = inPacket->readInt32();
    auto it = m_TianLingData.find(nTianLingId);
    if (it == m_TianLingData.end())
        return 2;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgTianLingTable* pTable = CfgData::GetTianLingTable(v4);
    const CfgTianLing* pCfg = CfgTianLingTable::GetRow(pTable, nTianLingId);
    if (!pCfg)
        return 2;

