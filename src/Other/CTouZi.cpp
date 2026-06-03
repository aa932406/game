#include "common.h"
#include "Other/CTouZi.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"
#include "DBService.h"

CTouZi::CTouZi()
    : CExtSystemBase()
{
    m_TouZiData.clear();
    m_TouZiCount = 0;
    m_TouZiDay = 0;
}

CTouZi::~CTouZi()
{
}

void CTouZi::OnCleanUp()
{
}

void CTouZi::OnUpdate(int64_t curTick)
{
}

void CTouZi::OnDaySwitch(int32_t nDiffDays)
{
    SendTouZiInfo();
    SendTouZiIcon();
}

void CTouZi::OnLoadFromDB(const PlayerDBData* dbData)
{
    const TouZiDBData& db = dbData->m_TouZiDBData;
    m_TouZiData = db.m_TouZiData;
    m_TouZiCount = db.m_TouZiCount;
    m_TouZiDay = db.m_TouZiDay;
}

void CTouZi::OnSaveToDB(PlayerDBData* dbData)
{
    TouZiDBData& db = dbData->m_TouZiDBData;
    db.m_TouZiData = m_TouZiData;
    db.m_TouZiCount = m_TouZiCount;
    db.m_TouZiDay = m_TouZiDay;
}

void CTouZi::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(490);
    procList->push_back(491);
    procList->push_back(492);
}

int32_t CTouZi::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 490:
        return OnGetTouZiReward(inPacket);
    case 491:
        return OnTouZi(inPacket);
    case 492:
        return OnAskTouZiInfo(inPacket);
    }
    return 0;
}

int32_t CTouZi::OnTouZi(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    int8_t nType = inPacket->readInt8();
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (CVip::GetVipLevel(pVip) <= 0)
        return 10002;
    int32_t GongGaoId = 0;
    if (nType == 1)
    {
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        if (CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL) + 1 > 7)
            return 10002;
        if (m_SevenDayTouZiTime > 0)
            return 10002;
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, 10000, CURRENCY_CHANGE_REASON::GCR_TOU_ZHI_SHI_QU, nType))
            return 10002;
        m_SevenDayTouZiTime = Unit::getNow(m_pPlayer);
        GongGaoId = 499;
    }
    else if (nType == 2)
    {
        if (m_MonthTouZiTime > 0)
            return 10002;
        if (m_SevenDayTouZiTime > 0 && !IsAllGetSevenDayTouZi())
            return 10002;
        if (m_SevenDayTouZiTime <= 0)
        {
            CfgData* pCfg = Answer::Singleton<CfgData>::instance();
            if (CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL) + 1 <= 7)
                return 10002;
        }
        if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, 50000, CURRENCY_CHANGE_REASON::GCR_TOU_ZHI_SHI_QU, nType))
            return 10002;
        m_MonthTouZiTime = Unit::getNow(m_pPlayer);
        GongGaoId = 500;
    }
    else
    {
        return 10002;
    }
    SendTouZiInfo();
    SendTouZiIcon();
    uint16_t Proc = inPacket->getProc();
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService::replySuccess(Answer::Singleton<GameService>::instance(), ConnId, GateIndex, Proc, nType);
    if (GongGaoId > 0)
    {
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::GetInstance()->popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            packet->writeInt32(GongGaoId);
            packet->writeInt64(Player::getCid(m_pPlayer));
            std::string val;
            Player::getName(m_pPlayer, &val);
            packet->writeUTF8(&val);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::worldBroadcast(pGame, packet);
        }
    }
    return 0;
}

int32_t CTouZi::OnGetTouZiReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    int8_t nType = inPacket->readInt8();
    int16_t nIndex = inPacket->readInt16();
    if (nIndex <= 0)
        return 10002;
    if (nType == 1)
        return GetSevenTouZiReward(nIndex);
    if (nType == 2)
        return GetMonthTouZiReward(nIndex);
    return 0;
}

int32_t CTouZi::OnAskTouZiInfo(Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    SendTouZiInfo();
    return 0;
}

int32_t CTouZi::GetSevenTouZiReward(int16_t nIndex)
{
    if (!m_pPlayer)
        return 10002;
    if (m_SevenDayTouZiTime <= 0)
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgTouZiTable* TouZiTable = CfgData::GetTouZiTable(pCfgData);
    const SevenTouZi* pCfg = CfgTouZiTable::GetSevenTouZi(TouZiTable, nIndex);
    if (!pCfg)
        return 10002;
    int32_t nNewRecord = (1 << (nIndex - 1)) | m_SevenDayRecord;
    if (m_SevenDayRecord == nNewRecord)
        return 10002;
    if (pCfg->nType == 1)
    {
        if (Answer::DayTime::daydiff(m_SevenDayTouZiTime) + 1 < pCfg->nCondition)
            return 10002;
    }
    else if (pCfg->nType == 2)
    {
        int32_t nVipLvl = CVip::GetVipLevel(Player::GetPlayerVip(m_pPlayer));
        if (nVipLvl < pCfg->nCondition)
            return 10002;
    }
    else
    {
        return 10002;
    }
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(pBag, &pCfg->vItem, ITEM_CHANGE_REASON::ICR_WEEK_TOU_ZI))
        return 10002;
    m_SevenDayRecord = nNewRecord;
    SendTouZiInfo();
    SendTouZiIcon();
    return 0;
}

int32_t CTouZi::GetMonthTouZiReward(int16_t nIndex)
{
    if (!m_pPlayer)
        return 10002;
    if (m_MonthTouZiTime <= 0)
        return 10002;
    int32_t nNewRecord = (1 << (nIndex - 1)) | m_MonthTouZiRecord;
    if (m_MonthTouZiRecord == nNewRecord)
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgTouZiTable* TouZiTable = CfgData::GetTouZiTable(pCfgData);
    const MonthTouZi* pCfg = CfgTouZiTable::GetMonthTouZi(TouZiTable, nIndex);
    if (!pCfg)
        return 10002;
    int32_t nDiffDay = Answer::DayTime::daydiff(m_MonthTouZiTime);
    if (nDiffDay + 1 < pCfg->nDay)
        return 10002;
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(pBag, &pCfg->vItem, ITEM_CHANGE_REASON::ICR_WEEK_TOU_ZI))
        return 10002;
    m_MonthTouZiRecord = nNewRecord;
    SendTouZiInfo();
    SendTouZiIcon();
    if (pCfg->nGongGaoId > 0)
    {
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::GetInstance()->popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            packet->writeInt32(pCfg->nGongGaoId);
            packet->writeInt64(Player::getCid(m_pPlayer));
            std::string val;
            Player::getName(m_pPlayer, &val);
            packet->writeUTF8(&val);
            uint32_t WOffset = packet->getWOffset();
            packet->setSize(WOffset);
            GameService::worldBroadcast(pGame, packet);
        }
    }
    return 0;
}



void CTouZi::SendTouZiInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::GetInstance()->popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CE8);
    if (!packet)
        return;
    packet->writeInt32(m_SevenDayTouZiTime);
    packet->writeInt32(m_SevenDayRecord);
    packet->writeInt32(m_MonthTouZiTime);
    packet->writeInt32(m_MonthTouZiRecord);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

void CTouZi::GetTouZiIconState(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 65))
    {
        ShowIcon stu;
        GetTouZiIconStu(&stu);
        IconList->push_back(stu);
    }
}

void CTouZi::SendTouZiIcon()
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 65))
    {
        ShowIcon stu;
        GetTouZiIconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
        if (stu.nId == 133)
        {
            ShowIcon StuTmp;
            memset(&StuTmp, 0, sizeof(StuTmp));
            StuTmp.nId = 134;
            StuTmp.nState = 0;
            Player::SendIconState(m_pPlayer, &StuTmp);
        }
    }
}

bool CTouZi::IsAllGetSevenDayTouZi()
{
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgTouZiTable* TouZiTable = CfgData::GetTouZiTable(pCfgData);
    return CfgTouZiTable::IsAllGetSevenDay(TouZiTable, m_SevenDayRecord);
}

bool CTouZi::IsAllGetMonthTouZi()
{
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgTouZiTable* TouZiTable = CfgData::GetTouZiTable(pCfgData);
    return CfgTouZiTable::IsAllGetMonthTouZi(TouZiTable, m_MonthTouZiRecord);
}

int32_t CTouZi::GetSevenDayRewardCount()
{
    if (!m_pPlayer)
        return 10002;
    if (m_SevenDayTouZiTime <= 0)
        return 0;
    int32_t Count = 0;
    int32_t nDiffDay = Answer::DayTime::daydiff(m_SevenDayTouZiTime);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgTouZiTable* TouZiTable = CfgData::GetTouZiTable(pCfgData);
    const SevenTouZiMap* SevenDayTable = CfgTouZiTable::GetSevenDayTable(TouZiTable);
    for (auto it = SevenDayTable->begin(); it != SevenDayTable->end(); ++it)
    {
        if (it->second.nType == 1)
        {
            if (nDiffDay + 1 < it->second.nCondition)
                continue;
        }
        else if (it->second.nType == 2)
        {
            int32_t nVipLvl = CVip::GetVipLevel(Player::GetPlayerVip(m_pPlayer));
            if (nVipLvl < it->second.nCondition)
                continue;
        }
        int32_t nNewRecord = m_SevenDayRecord | (1 << (it->second.nId - 1));
        if (m_SevenDayRecord != nNewRecord)
            ++Count;
    }
    return Count;
}

int32_t CTouZi::GetMonthRewardCount()
{
    if (!m_pPlayer)
        return 10002;
    if (m_MonthTouZiTime <= 0)
        return 0;
    int32_t Count = 0;
    int32_t nDiffDay = Answer::DayTime::daydiff(m_MonthTouZiTime);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgTouZiTable* TouZiTable = CfgData::GetTouZiTable(pCfgData);
    const MonthTouZiMap* MonthTable = CfgTouZiTable::GetMonthTable(TouZiTable);
    for (auto it = MonthTable->begin(); it != MonthTable->end(); ++it)
    {
        if (nDiffDay + 1 >= it->second.nDay)
        {
            int32_t nNewRecord = m_MonthTouZiRecord | (1 << (it->second.nDay - 1));
            if (m_MonthTouZiRecord != nNewRecord)
                ++Count;
        }
    }
    return Count;
}

void CTouZi::GetTouZiIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    retstr->nId = 134;
    if (m_SevenDayTouZiTime <= 0)
    {
        CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
        if (CfgData::getServerDiffDay(pCfgData, SERVER_TYPE::SVT_NORMAL) + 1 > 7)
            retstr->nId = 133;
    }
    else if (IsAllGetSevenDayTouZi())
    {
        retstr->nId = 133;
    }
    if (IsAllGetMonthTouZi())
        retstr->nState = 4;
    else
        retstr->nState = 2;
    if (retstr->nId == 134)
        retstr->nIconRight = GetSevenDayRewardCount();
    else
        retstr->nIconRight = GetMonthRewardCount();
    retstr->nLeftTime = -1;
}