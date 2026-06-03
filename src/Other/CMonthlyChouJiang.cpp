#include "common.h"
#include "Other/CMonthlyChouJiang.h"
#include "GameService.h"
#include "Player.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "Character/CExtCharBag.h"
#include "Other/CFunctionOpen.h"
#include "Other/CMonthlyChouJiangData.h"
#include "Other/CMonthlyChouJiangTable.h"

#include <ctime>

CMonthlyChouJiang::CMonthlyChouJiang()
    : CExtSystemBase()
{
    OnCleanUp();
}

CMonthlyChouJiang::~CMonthlyChouJiang()
{
}

void CMonthlyChouJiang::OnLoadFromDB(const PlayerDBData* const dbData)
{
    m_Score = dbData->m_CMonthlyChouJiangData.m_Score;
    m_AllScore = dbData->m_CMonthlyChouJiangData.m_AllScore;
    m_GetItemList = dbData->m_CMonthlyChouJiangData.m_GetItemList;
    m_GetTimes = dbData->m_CMonthlyChouJiangData.m_GetTimes;
}

void CMonthlyChouJiang::OnSaveToDB(PlayerDBData* const dbData)
{
    dbData->m_CMonthlyChouJiangData.m_Score = m_Score;
    dbData->m_CMonthlyChouJiangData.m_AllScore = m_AllScore;
    dbData->m_CMonthlyChouJiangData.m_GetItemList = m_GetItemList;
    dbData->m_CMonthlyChouJiangData.m_GetTimes = m_GetTimes;
}

void CMonthlyChouJiang::GetInterestsProtocol(ProcIdList* const procList)
{
    procList->push_back(522);
    procList->push_back(523);
}

void CMonthlyChouJiang::OnCleanUp()
{
    m_Score = 0;
    m_AllScore = 0;
    m_GetItemList.clear();
    m_GetTimes.clear();
}

void CMonthlyChouJiang::OnUpdate(int64_t curTick)
{
}

void CMonthlyChouJiang::OnDaySwitch(int32_t nDiffDays)
{
    SendIcon();
    SendMonthlyChouJiangInfo();
}

int32_t CMonthlyChouJiang::GetKaiFuMonth()
{
    if (!m_pPlayer)
        return 0;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nKaiFuTime = CfgData::getServerOpenTime(cfg, SERVER_TYPE::SVT_NORMAL);
    int32_t nNowTime = Unit::getNow(m_pPlayer);
    tm kaiFuTm = {};
    tm nowTm = {};
    localtime_s(&kaiFuTm, &reinterpret_cast<const time_t&>(nKaiFuTime));
    localtime_s(&nowTm, &reinterpret_cast<const time_t&>(nNowTime));
    int32_t DiffMonth = (nowTm.tm_year - kaiFuTm.tm_year) * 12 + (nowTm.tm_mon - kaiFuTm.tm_mon);
    return DiffMonth + 1;
}

int32_t CMonthlyChouJiang::OnMonthlyChouJiang(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t KaiFuMonth = GetKaiFuMonth();
    if (KaiFuMonth <= 0)
        return 2;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const MonthlyChouJiangTable* MonthlyChouJiangTable = CfgData::GetMonthlyChouJiangTable(cfg);
    const MonthlyChouJiangCfg* pCfg = MonthlyChouJiangTable::GetCfg(MonthlyChouJiangTable, KaiFuMonth);
    if (!pCfg)
        return 2;
    int32_t nNeed = pCfg->nNeed;
    int32_t nHave = Player::getRecord(m_pPlayer, 1104);
    int32_t nToday = Player::getRecord(m_pPlayer, 1105);
    if (nHave <= nToday)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!Bag->AddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_MONTHLY_CHOU_JIANG))
        return 10002;
    Player::updateRecord(m_pPlayer, 1105, nToday + 1);
    m_AllScore += pCfg->nScore;
    m_GetItemList.push_back(pCfg->nId);
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (packet)
    {
        packet->writeInt32(460);
        std::string val;
        Player::getName(m_pPlayer, &val);
        packet->writeUTF8(&val);
        packet->writeInt64(Player::getCid(m_pPlayer));
        packet->writeInt8(static_cast<int8_t>(pCfg->Items.m_nClass));
        packet->writeInt32(pCfg->Items.m_nId);
        packet->writeInt32(pCfg->Items.m_nCount);
        uint32_t wOffset = packet->getWOffset();
        packet->setSize(wOffset);
        int8_t connId2 = Player::getConnId(m_pPlayer);
        GameService* gs2 = Answer::Singleton<GameService>::instance();
        GameService::worldBroadcast(gs2, connId2, packet);
    }
    SendMonthlyChouJiangResult(pCfg->nId);
    SendMonthlyChouJiangInfo();
    SendIcon();
    return 0;
}

void CMonthlyChouJiang::OnMonthlySwitch()
{
    SendMonthlyChouJiangInfo();
    SendIcon();
}

void CMonthlyChouJiang::OnChongZhi(int32_t NewValues, int32_t OldValues)
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    int32_t nAdd = NewValues - OldValues;
    const MonthlyChouJiangTable* MonthlyChouJiangTable = CfgData::GetMonthlyChouJiangTable(cfg);
    int32_t nMonth = GetKaiFuMonth();
    if (nMonth <= 0)
        return;
    const MonthlyChouJiangCfg* pCfg = MonthlyChouJiangTable::GetCfg(MonthlyChouJiangTable, nMonth);
    if (!pCfg)
        return;
    m_Score += nAdd;
    int32_t SocreTimes = GetSocreTimes(pCfg->nId);
    if (SocreTimes > 0)
    {
        int32_t nCanAdd = m_Score / pCfg->nPerScore - SocreTimes;
        for (int i = 0; i < nCanAdd; ++i)
        {
            m_GetTimes[pCfg->nId] += 1;
            BagAddItem(&pCfg->Items, ITEM_CHANGE_REASON::ICR_MONTHLY_CHOU_JIANG);
            SendMonthlyChouJiangResult(pCfg->nId);
        }
    }
}

void CMonthlyChouJiang::SendMonthlyChouJiangInfo()
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CF7);
    if (!packet)
        return;
    packet->writeInt32(m_Score);
    packet->writeInt32(m_AllScore);
    packet->writeInt32(static_cast<int32_t>(m_GetItemList.size()));
    for (auto& id : m_GetItemList)
        packet->writeInt32(id);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

void CMonthlyChouJiang::SendMonthlyChouJiangResult(int32_t nId)
{
    if (!m_pPlayer)
        return;
    int8_t connId = Player::getConnId(m_pPlayer);
    GameService* gs = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(gs, connId, Answer::PackType::PACK_DISPATCH, 0x2CF8);
    if (!packet)
        return;
    packet->writeInt32(nId);
    uint32_t wOffset = packet->getWOffset();
    packet->setSize(wOffset);
    int16_t gateIndex = Player::getGateIndex(m_pPlayer);
    int8_t connId2 = Player::getConnId(m_pPlayer);
    GameService* gs2 = Answer::Singleton<GameService>::instance();
    GameService::sendPacketTo(gs2, connId2, gateIndex, packet);
}

int32_t CMonthlyChouJiang::GetSocreTimes(int32_t nId)
{
    auto it = m_GetTimes.find(nId);
    if (it != m_GetTimes.end())
        return it->second;
    return 0;
}

void CMonthlyChouJiang::GetIcon(IconStateList* const IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (!CFunctionOpen::IsOpened(PlayerFunctionOpen, 210))
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    if (GetKaiFuMonth() <= CfgData::GetMonthlyChouJiangMaxMonth(cfg))
    {
        ShowIcon icon;
        GetIcon(&icon);
        IconList->push_back(icon);
    }
}

void CMonthlyChouJiang::SendIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (!CFunctionOpen::IsOpened(PlayerFunctionOpen, 210))
        return;
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    if (GetKaiFuMonth() > CfgData::GetMonthlyChouJiangMaxMonth(cfg))
        return;
    ShowIcon icon;
    GetIcon(&icon);
    Player::SendIconState(m_pPlayer, &icon);
}

void CMonthlyChouJiang::GetIcon(ShowIcon* __return_ptr retstr)
{
    retstr->nId = 0;
    retstr->nLeftTime = 0;
    retstr->nIconRight = 0;
    if (!m_pPlayer)
        return;
    retstr->nId = 126;
    if (static_cast<int32_t>(m_GetItemList.size()) <= 11)
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
}

bool CMonthlyChouJiang::TodaySocreIsAllGet()
{
    CfgData* cfg = Answer::Singleton<CfgData>::instance();
    const MonthlyChouJiangTable* MonthlyChouJiangTable = CfgData::GetMonthlyChouJiangTable(cfg);
    int32_t nMonth = GetKaiFuMonth();
    if (nMonth <= 0)
        return true;
    const MonthlyChouJiangCfgMap* MonthlyChouJiangCfgMap = MonthlyChouJiangTable::GetCfgMap(MonthlyChouJiangTable);
    MonthlyChouJiangCfgMap stu(*MonthlyChouJiangCfgMap);
    for (auto& pair : stu)
    {
        int32_t SocreTimes = GetSocreTimes(pair.first);
        if (m_Score / pair.second.nPerScore > SocreTimes)
            return false;
    }
    return true;
}
