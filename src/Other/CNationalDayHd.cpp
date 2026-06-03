#include "common.h"
#include "Other/CNationalDayHd.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Answer/StringUtility.h"
#include "Other/Param2.h"

#include <sstream>

CNationalDayHd::CNationalDayHd()
    : CExtSystemBase()
{
    OnCleanUp();
}

CNationalDayHd::~CNationalDayHd()
{
}

void CNationalDayHd::OnCleanUp()
{
    m_Level = 0;
    m_Exp = 0;
    m_UnLockReward = 0;
    m_OrdinaryReward = 0;
    m_SeniorReward = 0;
    m_NationalInfo.clear();
}

void CNationalDayHd::OnUpdate(int64_t curTick)
{
}

void CNationalDayHd::OnDaySwitch(int32_t nDiffDays)
{
}

void CNationalDayHd::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(530);
    procList->push_back(531);
    procList->push_back(532);
    procList->push_back(533);
    procList->push_back(534);
}

int32_t CNationalDayHd::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 534:
        return OnAskNationalInfo(inPacket);
    case 530:
        return OnGetOrdinaryReward(inPacket);
    case 531:
        return OnGetSeniorReward(inPacket);
    case 532:
        return OnBuyLevel(inPacket);
    case 533:
        return OnGetExp(inPacket);
    }
    return 0;
}

void CNationalDayHd::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_Level = dbData->m_NationalDayData.m_Level;
    m_Exp = dbData->m_NationalDayData.m_Exp;
    m_UnLockReward = dbData->m_NationalDayData.m_UnLockReward;
    m_OrdinaryReward = dbData->m_NationalDayData.m_OrdinaryReward;
    m_SeniorReward = dbData->m_NationalDayData.m_SeniorReward;
    std::string p_NationalInfo(dbData->m_NationalDayData.m_NationalInfo);
    paraseNational(&p_NationalInfo);
}

void CNationalDayHd::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_NationalDayData.m_Level = m_Level;
    dbData->m_NationalDayData.m_Exp = m_Exp;
    dbData->m_NationalDayData.m_UnLockReward = m_UnLockReward;
    dbData->m_NationalDayData.m_OrdinaryReward = m_OrdinaryReward;
    dbData->m_NationalDayData.m_SeniorReward = m_SeniorReward;
    dbData->m_NationalDayData.m_NationalInfo = SaveNationalInfo();
}

void CNationalDayHd::paraseNational(std::string* p_NationalInfo)
{
    std::string delims("|");
    StringVector mineString;
    Answer::StringUtility::split(&mineString, p_NationalInfo, &delims, 0);
    for (auto& str : mineString)
    {
        StringVector StringVt;
        std::string delim(":");
        Answer::StringUtility::split(&StringVt, &str, &delim, 0);
        if (StringVt.size() >= 3)
        {
            int32_t nType = atoi(StringVt[0].c_str());
            int32_t nId = atoi(StringVt[1].c_str());
            int32_t nValue = atoi(StringVt[2].c_str());
            m_NationalInfo[nType].Param1 = nId;
            m_NationalInfo[nType].Param2 = nValue;
        }
    }
}

std::string CNationalDayHd::SaveNationalInfo()
{
    std::ostringstream ss;
    for (auto& pair : m_NationalInfo)
    {
        ss << pair.first << ":" << pair.second.Param1 << ":" << pair.second.Param2 << "|";
    }
    return ss.str();
}

bool CNationalDayHd::GetNationalExp(int32_t nType, int32_t nId)
{
    auto it = m_NationalInfo.find(nType);
    if (it != m_NationalInfo.end())
    {
        if (it->second.Param1 == nId && it->second.Param2 <= 0)
            return false;
    }
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const NationalDayExpCfg* pCfg = CfgData::GetNationalDayExpCfg(cfg, nType, nId);
    if (!pCfg)
        return false;
    int32_t nOldExp = 0;
    auto itExp = m_NationalInfo.find(nType);
    if (itExp != m_NationalInfo.end() && itExp->second.Param1 == nId)
        nOldExp = itExp->second.Param2;
    if (nOldExp >= pCfg->nMaxTimes)
        return false;
    m_Exp += pCfg->nExp;
    m_NationalInfo[nType].Param1 = nId;
    m_NationalInfo[nType].Param2 = nOldExp + 1;
    CheckLevelUp();
    return true;
}

bool CNationalDayHd::UnLockHighReward()
{
    if (m_UnLockReward > 0)
        return false;
    if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < 500)
        return false;
    Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, 500, CURRENCY_CHANGE_REASON::GCR_NATIONAL_HD, 0);
    m_UnLockReward = 1;
    CheckLevelUp();
    return true;
}

bool CNationalDayHd::GetSeniorReward(int32_t nLevel)
{
    if (m_SeniorReward & (1 << (nLevel - 1)))
        return false;
    if (m_UnLockReward <= 0)
        return false;
    if (m_Level < nLevel)
        return false;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const NationalDayLevelCfg* pCfg = CfgData::GetNationalDayLevelCfg(cfg, nLevel);
    if (!pCfg)
        return false;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->SeniorItem, ITEM_CHANGE_REASON::ICR_NATIONAL_HD))
        return false;
    m_SeniorReward |= (1 << (nLevel - 1));
    return true;
}

bool CNationalDayHd::GetOrdinaryReward(int32_t nLevel)
{
    if (m_OrdinaryReward & (1 << (nLevel - 1)))
        return false;
    if (m_Level < nLevel)
        return false;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const NationalDayLevelCfg* pCfg = CfgData::GetNationalDayLevelCfg(cfg, nLevel);
    if (!pCfg)
        return false;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->OrdinaryItem, ITEM_CHANGE_REASON::ICR_NATIONAL_HD))
        return false;
    m_OrdinaryReward |= (1 << (nLevel - 1));
    return true;
}

bool CNationalDayHd::BuyLevel(int32_t BuyLv)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const NationalDayLevelCfg* pCfg = CfgData::GetNationalDayLevelCfg(cfg, BuyLv);
    if (!pCfg)
        return false;
    if (Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) < pCfg->nBuyPrice)
        return false;
    Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->nBuyPrice, CURRENCY_CHANGE_REASON::GCR_NATIONAL_HD, 0);
    m_Level += BuyLv;
    CheckLevelUp();
    return true;
}

void CNationalDayHd::AddExp(int32_t AddExp)
{
    m_Exp += AddExp;
    CheckLevelUp();
    SendLevelInfo();
}

void CNationalDayHd::CheckLevelUp()
{
    if (!m_pPlayer)
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const NationalDayLevelCfg* pCfg = CfgData::GetNationalDayLevelCfg(cfg, m_Level + 1);
    while (pCfg && m_Exp >= pCfg->nNeedExp)
    {
        m_Exp -= pCfg->nNeedExp;
        ++m_Level;
        pCfg = CfgData::GetNationalDayLevelCfg(cfg, m_Level + 1);
    }
}

void CNationalDayHd::SendLevelInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CE4);
    if (!packet)
        return;
    PackHdInfo(packet);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

void CNationalDayHd::PackHdInfo(Answer::NetPacket* packet)
{
    packet->writeInt32(m_Level);
    packet->writeInt32(m_Exp);
    packet->writeInt8(static_cast<int8_t>(m_UnLockReward));
    packet->writeInt32(m_OrdinaryReward);
    packet->writeInt32(m_SeniorReward);
}

int32_t CNationalDayHd::CanGetCount()
{
    int32_t Count = 0;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    for (int i = 1; i <= 60; ++i)
    {
        if (!(m_OrdinaryReward & (1 << (i - 1))) && m_Level >= i)
            ++Count;
    }
    return Count;
}
