#include "Other/CQiFu.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"

CQiFu::CQiFu()
    : CExtSystemBase()
{
    m_QiFu.clear();
}

CQiFu::~CQiFu()
{
}

void CQiFu::OnCleanUp()
{
}

void CQiFu::OnUpdate(int64_t curTick)
{
}

void CQiFu::OnDaySwitch(int32_t nDiffDays)
{
}

void CQiFu::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_QiFu = dbData->m_QiFuDBData.m_QiFu;
}

void CQiFu::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_QiFuDBData.m_QiFu = m_QiFu;
}

void CQiFu::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(405);
    procList->push_back(406);
}

int32_t CQiFu::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    if (nProcId == 405)
        return OnGetQiFuData(inPacket);
    if (nProcId == 406)
        return OnUseQiFu(inPacket);
    return 2;
}

int32_t CQiFu::OnGetQiFuData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2C95);
    if (!packet)
        return 2;
    int32_t nSize = m_QiFu.size();
    packet->writeInt32(nSize);
    for (auto it = m_QiFu.begin(); it != m_QiFu.end(); ++it)
    {
        packet->writeInt32(it->second);
    }
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t v6 = Player::getConnId(m_pPlayer);
    GameService* v7 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(v7, v6, GateIndex, packet);
    return 0;
}

int32_t CQiFu::OnUseQiFu(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t QiFuId = inPacket->readInt32();
    auto it = m_QiFu.find(QiFuId);
    if (it == m_QiFu.end())
        return 2;
    it->second = 1;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgQiFuTable* pTable = CfgData::GetQiFuTable(v4);
    const CfgQiFu* pQiFuCfg = CfgQiFuTable::GetRow(pTable, QiFuId);
    if (!pQiFuCfg)
        return 2;
    const CfgBuffTable* pBuffTable = CfgData::GetBuffTable(v4);
    int32_t nBuffId = pQiFuCfg->m_BuffId;
    const CfgBuff* pBuff = CfgBuffTable::GetRow(pBuffTable, nBuffId);
    if (pBuff)
    {
        CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
        CExtBuffManager::AddBuff(pBuffMgr, nBuffId, 0, 0);
    }
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v12 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v12, ConnId, GateIndex, Proc, 0);
    return 0;

int32_t CQiFu::OnQiFu(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    int32_t QiFuId = inPacket->readInt32();
    if (!CheckQiFuType(QiFuId))
        return 10002;
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (CVip::GetVipLevel(pVip) <= 0)
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgQiFuTable* pTable = CfgData::GetQiFuTable(pCfgData);
    const CfgQiFu* pCfg = CfgQiFuTable::GetRow(pTable, QiFuId);
    if (!pCfg)
        return 10002;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (pCfg->m_CostStone > 0)
    {
        if (!CExtCharBag::RemoveItemByCfgId(pBag, pCfg->m_CostStone, pCfg->m_CostCount, ITEM_CHANGE_REASON::ICR_QIFU_COST))
            return 10002;
    }
    if (pCfg->m_CostGold > 0)
    {
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_CostGold, CURRENCY_CHANGE_REASON::GCR_QIFU_COST, QiFuId))
            return 10002;
    }
    int32_t nAddCount = pCfg->m_AddCount;
    int32_t nDouble = 0;
    if (Answer::Singleton<Answer::Random>::instance()->generate(1, 10000) <= pCfg->m_DoubleRate)
    {
        nDouble = 1;
        nAddCount *= pCfg->m_DoubleTimes;
    }
    Player::AddCurrency(m_pPlayer, (CURRENCY_TYPE)pCfg->m_CurrencyType, nAddCount, CURRENCY_CHANGE_REASON::GCR_QIFU_REWARD, 0);
    SendQiFuSuccess(QiFuId, nAddCount, nDouble);
    uint16_t Proc = inPacket->getProc();
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService::replySuccess(Answer::Singleton<GameService>::instance(), ConnId, GateIndex, Proc, 0);
    return 0;
}

void CQiFu::SendQiFuSuccess(int8_t Type, int32_t AddCount, int8_t Double)
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CC7);
    if (!packet)
        return;
    packet->writeInt8(Type);
    packet->writeInt32(AddCount);
    packet->writeInt8(Double);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

void CQiFu::SendQiFuInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CC8);
    if (!packet)
        return;
    for (auto it = m_QiFu.begin(); it != m_QiFu.end(); ++it)
    {
        packet->writeInt32(it->first);
        packet->writeInt32(it->second);
    }
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

void CQiFu::GetQiFuStu(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 35))
    {
        ShowIcon stu;
        GetShowIconStu(&stu);
        IconList->push_back(stu);
    }
}

void CQiFu::SendQIFuIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 35))
    {
        ShowIcon stu;
        GetShowIconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}

void CQiFu::GetShowIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (!m_pPlayer)
        return;
    retstr->nId = 104;
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (CVip::GetVipLevel(pVip) > 0)
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nLeftTime = -1;
}
