#include "Other/CShenWei.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "DBService.h"

CShenWei::CShenWei()
    : CExtSystemBase()
{
    memset(m_ShenWei, 0, sizeof(m_ShenWei));
    m_CurShenWeiId = 0;
}

CShenWei::~CShenWei()
{
}

void CShenWei::OnCleanUp()
{
}

void CShenWei::OnUpdate(int64_t curTick)
{
}

void CShenWei::OnDaySwitch(int32_t nDiffDays)
{
}

void CShenWei::OnLoadFromDB(const PlayerDBData* dbData)
{
    const ShenWeiDBData& db = dbData->m_ShenWeiDBData;
    memcpy(m_ShenWei, db.m_ShenWei, sizeof(m_ShenWei));
    m_CurShenWeiId = db.m_CurShenWeiId;
}

void CShenWei::OnSaveToDB(PlayerDBData* dbData)
{
    ShenWeiDBData& db = dbData->m_ShenWeiDBData;
    memcpy(db.m_ShenWei, m_ShenWei, sizeof(m_ShenWei));
    db.m_CurShenWeiId = m_CurShenWeiId;
}

void CShenWei::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(439);
    procList->push_back(440);
    procList->push_back(441);
    procList->push_back(442);
    procList->push_back(443);
}

int32_t CShenWei::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 439:
        return OnGetShenWeiData(inPacket);
    case 440:
        return OnActivateShenWei(inPacket);
    case 441:
        return OnUpgradeShenWei(inPacket);
    case 442:
        return OnEquipShenWei(inPacket);
    case 443:
        return OnUnEquipShenWei(inPacket);
    default:
        return 2;
    }
}

int32_t CShenWei::OnGetShenWeiData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D66);
    if (!packet)
        return 2;
    for (int32_t i = 0; i < 8; ++i)
    {
        packet->writeInt32(m_ShenWei[i]);
    }
    packet->writeInt32(m_CurShenWeiId);
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CShenWei::OnActivateShenWei(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t ShenWeiId = inPacket->readInt32();
    if (ShenWeiId <= 0 || ShenWeiId > 8)
        return 2;
    if (m_ShenWei[ShenWeiId - 1] > 0)
        return 2;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiTable* pTable = CfgData::GetShenWeiTable(v4);
    const CfgShenWei* pCfg = CfgShenWeiTable::GetRow(pTable, ShenWeiId);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_ActivateCost, CURRENCY_CHANGE_REASON::GCR_SHENWEI_ACTIVATE, 0))
        return 2;
    m_ShenWei[ShenWeiId - 1] = 1;
    CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
    CExtBuffManager::AddBuff(pBuffMgr, pCfg->m_BuffId, 0, 0);
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::CalcProperty(pBag);
    Player::SendMsgUpdate(m_pPlayer);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v13 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v13, ConnId, GateIndex, Proc, 0);
    DBService* v14 = Answer::Singleton<DBService>::instance();
    DBService::ReplaceShenWei(v14, Player::getConnId(m_pPlayer), ShenWeiId, 1);
    return 0;
}

int32_t CShenWei::OnUpgradeShenWei(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t ShenWeiId = inPacket->readInt32();
    if (ShenWeiId <= 0 || ShenWeiId > 8)
        return 2;
    if (m_ShenWei[ShenWeiId - 1] <= 0)
        return 2;
    int32_t nCurLvl = m_ShenWei[ShenWeiId - 1];
    if (nCurLvl >= 10)
        return 2;
    CfgData* v5 = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiLvlTable* pLvlTable = CfgData::GetShenWeiLvlTable(v5);
    int32_t nNextLvl = nCurLvl + 1;
    int32_t nLvlKey = ShenWeiId * 100 + nNextLvl;
    const CfgShenWeiLvl* pCfg = CfgShenWeiLvlTable::GetRow(pLvlTable, nLvlKey);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_UpgradeCost, CURRENCY_CHANGE_REASON::GCR_SHENWEI_UPGRADE, 0))
        return 2;
    m_ShenWei[ShenWeiId - 1] = nNextLvl;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v12 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v12, ConnId, GateIndex, Proc, 0);
    DBService* v13 = Answer::Singleton<DBService>::instance();
    DBService::ReplaceShenWei(v13, ConnId, ShenWeiId, nNextLvl);
    return 0;
}

int32_t CShenWei::OnEquipShenWei(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t ShenWeiId = inPacket->readInt32();
    if (ShenWeiId <= 0 || ShenWeiId > 8)
        return 2;
    if (m_ShenWei[ShenWeiId - 1] <= 0)
        return 2;
    if (m_CurShenWeiId == ShenWeiId)
        return 2;
    int32_t nOldId = m_CurShenWeiId;
    if (nOldId > 0)
    {
        CfgData* v5 = Answer::Singleton<CfgData>::instance();
        const CfgShenWeiTable* pTable = CfgData::GetShenWeiTable(v5);
        const CfgShenWei* pOldCfg = CfgShenWeiTable::GetRow(pTable, nOldId);
        if (pOldCfg)
        {
            CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
            CExtBuffManager::DelBuffById(pBuffMgr, pOldCfg->m_BuffId);
        }
    }
    m_CurShenWeiId = ShenWeiId;
    CfgData* v7 = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiTable* pTable2 = CfgData::GetShenWeiTable(v7);
    const CfgShenWei* pNewCfg = CfgShenWeiTable::GetRow(pTable2, ShenWeiId);
    if (pNewCfg)
    {
        CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
        CExtBuffManager::AddBuff(pBuffMgr, pNewCfg->m_BuffId, 0, 0);
    }
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::CalcProperty(pBag);
    Player::SendMsgUpdate(m_pPlayer);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v14 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v14, ConnId, GateIndex, Proc, 0);
    DBService* v15 = Answer::Singleton<DBService>::instance();
    DBService::ReplaceCurShenWei(v15, ConnId, ShenWeiId);
    return 0;
}

int32_t CShenWei::OnUnEquipShenWei(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    if (m_CurShenWeiId <= 0)
        return 2;
    CfgData* v3 = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiTable* pTable = CfgData::GetShenWeiTable(v3);
    const CfgShenWei* pCfg = CfgShenWeiTable::GetRow(pTable, m_CurShenWeiId);
    if (pCfg)
    {
        CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
        CExtBuffManager::DelBuffById(pBuffMgr, pCfg->m_BuffId);
    }
    int32_t nOldId = m_CurShenWeiId;
    m_CurShenWeiId = 0;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::CalcProperty(pBag);
    Player::SendMsgUpdate(m_pPlayer);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v11 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v11, ConnId, GateIndex, Proc, 0);
    DBService* v12 = Answer::Singleton<DBService>::instance();
    DBService::ReplaceCurShenWei(v12, ConnId, 0);
    return 0;

void CShenWei::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (!CFunctionOpen::IsOpened(pFuncOpen, 188))
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    int32_t nLevel = GetShenWeiLevel();
    if (nLevel <= 0)
        return;
    const CfgShenWeiTable* pTable = CfgData::GetShenWeiTable(pCfgData);
    const CfgShenWei* pCfg = CfgShenWeiTable::GetRow(pTable, nLevel);
    if (pCfg && pCfg->m_AttrId > 0 && pCfg->m_AttrValue > 0)
    {
        Player::AddCharAttrAddon(m_pPlayer, pCfg->m_AttrId, pCfg->m_AttrValue * nLevel);
    }
}

int32_t CShenWei::GetRankAttr(int64_t Exp)
{
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiRankTable* pRankTable = CfgData::GetShenWeiRankTable(pCfgData);
    for (auto it = pRankTable->begin(); it != pRankTable->end(); ++it)
    {
        if (Exp >= it->second.m_NeedExp)
            return it->second.m_AttrValue;
    }
    return 0;
}

int32_t CShenWei::GetShenWeiLevel()
{
    if (!m_pPlayer)
        return 0;
    int64_t nCurrency = Player::GetCurrencyValue(m_pPlayer, CURRENCY_TYPE::CURRENCY_HONOR);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgShenWeiTable* pTable = CfgData::GetShenWeiTable(pCfgData);
    int32_t nLevel = 0;
    for (auto it = pTable->begin(); it != pTable->end(); ++it)
    {
        if (nCurrency >= it->second.m_NeedHonor)
            nLevel = it->first;
    }
    return nLevel;
}
