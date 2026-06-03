#include "common.h"
#include "Other/COpenBeta.h"

#include <cstring>
#include <algorithm>

COpenBeta::COpenBeta()
{
    m_nDay = -1;
    m_nMinute = 0;
    m_nOnlineTimeArea = 0;
    m_MapId = 0;
    m_MonsterId = 0;
    m_bDie = true;
    m_nMaxCount = 0;
    m_LiQuanLimitCount = 0;
}

COpenBeta::~COpenBeta()
{
}

void COpenBeta::Init(int32_t line)
{
    CfgData *pCfg = CfgData::instance();
    m_nDay = CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL);
    CTimer *pTimer = CTimer::instance();
    tm nowTime;
    CTimer::GetLocalNow(&nowTime, pTimer);
    m_nMinute = nowTime.tm_hour * 60 + nowTime.tm_min;
    m_nOnlineTimeArea = 0;

    std::string file = Answer::Inifile::getFilePath("OpenBeta.ini");
    Answer::Inifile ini;
    Answer::Inifile::Inifile(&ini, &file);

    m_nStartDay = Answer::Inifile::getInt(&ini, "Time", "StartDay", 0);
    m_nEndDay = Answer::Inifile::getInt(&ini, "Time", "EndDay", 0);
    m_nIcon = Answer::Inifile::getInt(&ini, "Icon", "IconId", 0);

    for (int32_t i = 0; i < 16; ++i)
    {
        char szCol[64];
        snprintf(szCol, sizeof(szCol), "StartDay%d", i + 1);
        m_vStartDay[i] = Answer::Inifile::getInt(&ini, "Time", szCol, 0);
        snprintf(szCol, sizeof(szCol), "EndDay%d", i + 1);
        m_vEndDay[i] = Answer::Inifile::getInt(&ini, "Time", szCol, 0);
    }

    loadXiaoFeiSumRank();
    LoadYanHuaValue();
}

int32_t COpenBeta::GetFlopType(int32_t nFlopId)
{
    if (nFlopId >= 0 && nFlopId < (int32_t)m_FlopType.size())
        return m_FlopType[nFlopId];
    return 0;
}

bool COpenBeta::CanFlopDraw(Player *pPlayer, int32_t FlopId)
{
    if (!pPlayer || !IsInTime(11))
        return false;

    if (FlopId < 0 || FlopId >= (int32_t)m_FlopSize)
        return false;

    int32_t FlopTimes = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(pPlayer), 1056);
    if (FlopTimes >= GetMaxFlopTimes(pPlayer))
        return false;

    if (m_FlopCost[FlopId].itemCount <= 0)
        return false;

    return Player::GetCurrency(pPlayer, CURRENCY_TYPE::CURRENCY_GOLD) >= (int64_t)m_FlopCost[FlopId].itemCount;
}

int32_t COpenBeta::GetMaxFlopTimes(Player *pPlayer)
{
    if (!pPlayer)
        return 0;
    return m_nMaxCount;
}

void COpenBeta::OnDaySwitch()
{
    if (m_nDay == m_nEndDay)
    {
        hideIcon(m_nIcon);
        if (IsInTime(16))
            sendXiaoFeiSumRankMail();
    }
    if (IsOpenBetaTime())
        updateStartTime();
}

void COpenBeta::OnNewMinute(int32_t nMinute)
{
    m_nMinute = nMinute;
    CheckBoss();
    checkXiaoFeiSumRank();
}

void COpenBeta::updateStartTime()
{
    if (!IsOpenBetaTime())
        return;

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);
    int32_t nDiffDay = m_nDay - m_nStartDay;
    m_nOnlineStartMinute = 0;
    m_nOnlineEndMinute = 1440;
    m_nBossStartBroadcast = 0;
    m_nBossEndBroadcast = 0;
    m_nBossKillBroadcast = 0;
    m_nBossIcon = 0;
    m_nDailyLimitShopSize = 0;

    CfgData *pCfg = CfgData::instance();
    m_nShowSize = 0;
    m_nStartDay = 0;
    m_nEndDay = 9999;
    m_nRechargeDaysSize = 0;
    m_nShiZhuangShopSize = 0;
    m_nTitleShopSize = 0;
}

void COpenBeta::GetIconState(Player *player, IconStateList *IconList)
{
    if (!player)
        return;

    if (GameService::getLine(GameService::instance()) != 9 && IsOpenBetaTime())
    {
        ShowIcon icon;
        getIconState(icon, player);
        IconList->push_back(icon);
    }
}

void COpenBeta::SendIconState(Player *player)
{
    if (!player)
        return;

    if (GameService::getLine(GameService::instance()) != 9 && IsOpenBetaTime())
    {
        ShowIcon stu;
        getIconState(stu, player);
        Player::SendIconState(player, &stu);
    }
}

void COpenBeta::getIconState(ShowIcon &retstr, Player *player)
{
    memset(&retstr, 0, sizeof(retstr));
    if (!player)
        return;

    retstr.nId = m_nIcon;
    retstr.nState = IsOpenBetaTime() ? 2 : 4;
    retstr.nLeftTime = getLeftTime();

    if (IsInTime(2))
        retstr.nIconRight += HaveRechargeRewardCount(player);

    if (canGetOnlineGift(player))
        ++retstr.nIconRight;

    if (IsInTime(12))
    {
        int32_t nXiaoFeiSum = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 1053);
        int32_t nRewardTimes = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 1054);
        int32_t nCanGet = nXiaoFeiSum / 5000 - nRewardTimes;
        if (nCanGet > 0)
            retstr.nIconRight += nCanGet;
    }

    if (IsInTime(13))
    {
        int32_t nSum = Player::getRecord(player, 1057);
        int32_t nRecord = Player::getRecord(player, 1058);
        for (int32_t i = 0; i < m_nRechargeDaysSize; ++i)
        {
            if (nSum >= m_vRechargeValue[i] && !((nRecord >> i) & 1))
                ++retstr.nIconRight;
        }
    }

    if (IsInTime(11))
    {
        int32_t MaxFlopTimes = GetMaxFlopTimes(player);
        int32_t nFlopTimes = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 1056);
        retstr.nIconRight += MaxFlopTimes - nFlopTimes;
    }

    if (IsInTime(16))
    {
        int32_t nXiaoFeiRankSum = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 1053);
        for (int32_t i = 0; i < m_nXiaoFeiSumRankSize; ++i)
        {
            if (nXiaoFeiRankSum > 0)
                ++retstr.nIconRight;
        }
    }
}

void COpenBeta::SendActivityInfo(Player *player)
{
    if (!player)
        return;

    int8_t ConnId = Player::getConnId(player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CFF);
    if (!packet)
        return;

    packet->writeInt32(m_nDay);
    packet->writeInt32(m_nStartDay);
    packet->writeInt32(m_nEndDay);
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void COpenBeta::GetBossIconState(IconStateList *IconList)
{
    if (GameService::getLine(GameService::instance()) != 9 && IsInBossTime())
    {
        ShowIcon icon;
        getBossIconState(icon);
        IconList->push_back(icon);
    }
}

void COpenBeta::SendBossIconState()
{
    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return;

    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, 0, Answer::PackType::PACK_DISPATCH, 0x2CC3);
    if (!packet)
        return;

    ShowIcon stu;
    getBossIconState(stu);
    packet->writeInt32(stu.nId);
    packet->writeInt8(stu.nState);
    packet->writeInt32(stu.nLeftTime);
    packet->writeInt8(stu.nIconLeft);
    packet->writeInt32(stu.nIconRight);
    packet->writeInt8(stu.nEffects);
    packet->setSize(packet->getWOffset());
    GameService::worldBroadcast(pGame, 0, packet);
}

void COpenBeta::getBossIconState(ShowIcon &retstr)
{
    memset(&retstr, 0, sizeof(retstr));
    retstr.nId = m_nBossIcon;
    if (IsInBossTime() && !m_bDie)
        retstr.nState = 2;
    else
        retstr.nState = 4;
    retstr.nLeftTime = -1;
}

bool COpenBeta::IsInBossTime()
{
    return GameService::getLine(GameService::instance()) != 9 && IsInTime(9);
}

void COpenBeta::BossDie(int32_t MonsterId, std::string *p_name, CharId_t Cid)
{
    if (MonsterId == m_MonsterId)
    {
        m_bDie = true;
        BroadcastBossKilled(p_name, Cid);
        SendBossIconState();
    }
}

void COpenBeta::BroadcastBossKilled(std::string *name, CharId_t cid)
{
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::GetInstance()->popNetpacket(pGame, Answer::PackType::PACK_DISPATCH, 0x2CB3);
    if (!packet)
        return;

    packet->writeInt32(m_MonsterId);
    packet->writeUTF8(name);
    packet->writeInt64(cid);
    packet->setSize(packet->getWOffset());
    GameService::broadcast(pGame, packet);
}

void COpenBeta::CheckBoss()
{
}

void COpenBeta::broadcastBossStart()
{
}

void COpenBeta::broadcastBossEnd()
{
}

int32_t COpenBeta::GetRechargeGift(Player *player, int32_t nIndex)
{
    if (!player || !IsInTime(2))
        return 2;

    if (nIndex < 0 || nIndex >= m_nRechargeDaysSize)
        return 2;

    int32_t nSum = Player::getRecord(player, 1057);
    if (nSum < m_vRechargeValue[nIndex])
        return 2;

    int32_t nRecord = Player::getRecord(player, 1058);
    int32_t nNewRecord = nRecord | (1 << nIndex);
    if (nRecord == nNewRecord)
        return 2;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return 2;

    if (!CExtCharBag::AddItem(pBag, &m_vRechargeGift[nIndex], ITEM_CHANGE_REASON::ICR_OPEN_BETA_RECHARGE))
        return 2;

    Player::updateRecord(player, 1058, nNewRecord);
    SendIconState(player);
    SendResult(player, 2, nIndex, 0);
    return 0;
}

int32_t COpenBeta::BuyGiftShopItem(Player *player, int8_t nIndex)
{
    return 2;
}

int32_t COpenBeta::BuyTitleShopItem(Player *player, int8_t nIndex)
{
    return 2;
}

int32_t COpenBeta::BuyShiZhuangShopItem(Player *player, int8_t nIndex)
{
    return 2;
}

int32_t COpenBeta::BuyLiQuan(Player *player, int8_t nIndex)
{
    return 2;
}

void COpenBeta::AddOnlineRecord(Player *player)
{
    if (!player || !IsInTime(1))
        return;

    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return;

    int32_t nOldTime = Player::getRecord(player, 1052);
    int32_t nNow = Unit::getNow(player);
    if (nOldTime <= 0)
    {
        Player::updateRecord(player, 1052, nNow);
    }
    else
    {
        int32_t nTimeDiff = nNow - nOldTime;
        if (nTimeDiff <= 60)
        {
            m_nOnlineTimeArea += nTimeDiff;
            Player::updateRecord(player, 1052, nNow);
            if (canGetOnlineGift(player))
                SendIconState(player);
        }
    }
}

bool COpenBeta::canGetOnlineGift(Player *player)
{
    if (!player || !IsInTime(1))
        return false;

    int32_t nOnlineTime = m_nOnlineTimeArea;
    int32_t nRecord = Player::getRecord(player, 1063);
    for (int32_t i = 0; i < m_nShowSize; ++i)
    {
        if (nOnlineTime > 0 && !((nRecord >> i) & 1))
            return true;
    }
    return false;
}

int32_t COpenBeta::GetOnlineTimeRewardTime(Player *player)
{
    if (!player)
        return 0;

    int32_t nOnlineTime = m_nOnlineTimeArea;
    int32_t nRecord = Player::getRecord(player, 1063);
    for (int32_t i = 0; i < m_nShowSize; ++i)
    {
        if (!((nRecord >> i) & 1))
            return i + 1;
    }
    return 0;
}

int32_t COpenBeta::GetOnlineGift(Player *player)
{
    if (!player || !canGetOnlineGift(player))
        return 2;

    int32_t nNewRecord = Player::getRecord(player, 1063);
    int32_t OnlineTimeRewardTime = GetOnlineTimeRewardTime(player);
    nNewRecord |= (1 << (OnlineTimeRewardTime - 1));
    Player::updateRecord(player, 1063, nNewRecord);
    SendIconState(player);
    SendResult(player, 1, OnlineTimeRewardTime, 0);
    return 0;
}

bool COpenBeta::IsInTime(int8_t Type)
{
    return m_nDay >= m_vStartDay[Type] && m_nDay < m_vEndDay[Type];
}

int32_t COpenBeta::getLeftTime()
{
    if (!IsOpenBetaTime())
        return 0;

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);
    return 86400 * (m_nEndDay - m_nDay) - (nNowTime - Answer::DayTime::dayzero());
}

bool COpenBeta::IsOpenBetaTime()
{
    return m_nDay >= 0 && m_nDay < m_nEndDay;
}

int32_t COpenBeta::GetEndCollectDropTime()
{
    if (!IsInTime(8))
        return 0;

    CfgData *pCfg = CfgData::instance();
    return 86400 * m_vEndDay[8] + CfgData::getServerStartDayZeroTime(pCfg, SERVER_TYPE::SVT_NORMAL) - 1;
}

int32_t COpenBeta::GetChouJiangType()
{
    return 0;
}

void COpenBeta::LoadYanHuaValue()
{
}

void COpenBeta::UpdateYanHuaValue()
{
}

void COpenBeta::AddYanHuaPoint(int32_t Values)
{
    if (!IsInTime(4))
        return;

    m_CurrYanHuaPoint += Values;
    UpdateYanHuaValue();
}

int32_t COpenBeta::GetYanHuaPointReward(Player *player, int8_t nIndex)
{
    return 2;
}

int32_t COpenBeta::GetQianDaoReward(Player *player, int8_t nIndex, int8_t nType)
{
    return 2;
}

void COpenBeta::loadXiaoFeiSumRank()
{
}

void COpenBeta::updateXiaoFeiSumRank(Player *player, int32_t AddValues)
{
}

void COpenBeta::checkXiaoFeiSumRankInvalid(int8_t connid)
{
}

void COpenBeta::checkXiaoFeiSumRank()
{
}

void COpenBeta::sendXiaoFeiSumRankMail()
{
}

void COpenBeta::SendResult(Player *pPlayer, int8_t Type, int32_t Index, int32_t param)
{
}
