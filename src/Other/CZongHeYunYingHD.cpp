#include "Other/CZongHeYunYingHD.h"

#include <cstring>
#include <algorithm>

CZongHeYunYingHD::CZongHeYunYingHD()
{
    m_nDay = -1;
    m_nMinute = 0;
}

CZongHeYunYingHD::~CZongHeYunYingHD()
{
}

void CZongHeYunYingHD::OnDaySwitch()
{
    if (m_nDay == m_nEndDay)
        hideIcon(m_nIcon);

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_RANK_DAILY))
        sendRechargeDailyRankReward();
}

void CZongHeYunYingHD::OnNewMinute(int32_t nMinute)
{
    IsInTime();
}

void CZongHeYunYingHD::Init(int32_t line)
{
    CfgData *pCfg = CfgData::instance();
    m_nDay = CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL);
    CTimer *pTimer = CTimer::instance();
    tm nowTime;
    CTimer::GetLocalNow(&nowTime, pTimer);
    m_nMinute = nowTime.tm_hour * 60 + nowTime.tm_min;

    initCfgData();
    loadRankData();
}

void CZongHeYunYingHD::initOpenList(std::string *str)
{
    m_vOpen.clear();
    size_t pos = 0;
    std::string s = *str;
    while ((pos = s.find(',')) != std::string::npos)
    {
        std::string token = s.substr(0, pos);
        m_vOpen.push_back(atoi(token.c_str()));
        s.erase(0, pos + 1);
    }
    if (!s.empty())
        m_vOpen.push_back(atoi(s.c_str()));
}

void CZongHeYunYingHD::initCfgData()
{
    CfgData *pCfg = CfgData::instance();
    m_nStartDay = 0;
    m_nEndDay = 9999;
    m_nIcon = 100;

    m_vStartDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_DAILY] = 1;
    m_vEndDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_DAILY] = 999;
    m_vRechargeDailyReward.clear();
    m_nRechargeDailyValue = 500;
    m_nRechargeDailyRewardLimit = 10;

    m_vStartDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_RANK_DAILY] = 1;
    m_vEndDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_RANK_DAILY] = 999;
    m_nRechargeRankDailyArea = 0;
    m_nRechargeRankDailySize = 10;
    m_vRechargeRankDailyReward.clear();
    m_vRechargeRankDailyMail.clear();
    m_vRechargeRankDailyIndex = 0;

    m_vStartDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_TEAM_SHOP_DAILY] = 1;
    m_vEndDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_TEAM_SHOP_DAILY] = 999;
    m_vRechargeTeamShopDailyReward.clear();
    m_vRechargeTeamShopDailyValue.clear();

    m_vStartDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_ONCE_SHOP] = 1;
    m_vEndDay[ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_ONCE_SHOP] = 999;
    m_nOnceShopSize = 0;
    m_vOnceShopItem.clear();
    m_vOnceShopPrice.clear();

    std::string file = Answer::Inifile::getFilePath("ZongHeYunYingHD.ini");
    Answer::Inifile ini;
    Answer::Inifile::Inifile(&ini, &file);

    m_nStartDay = Answer::Inifile::getInt(&ini, "Time", "StartDay", 0);
    m_nEndDay = Answer::Inifile::getInt(&ini, "Time", "EndDay", 0);
    m_nIcon = Answer::Inifile::getInt(&ini, "Icon", "IconId", 0);

    char szCol[64];
    for (int32_t i = 0; i < 4; ++i)
    {
        snprintf(szCol, sizeof(szCol), "StartDay%d", i + 1);
        m_vStartDay[i] = Answer::Inifile::getInt(&ini, "Time", szCol, 0);
        snprintf(szCol, sizeof(szCol), "EndDay%d", i + 1);
        m_vEndDay[i] = Answer::Inifile::getInt(&ini, "Time", szCol, 0);
    }

    std::string strOpen = Answer::Inifile::getString(&ini, "Open", "List", "");
    initOpenList(&strOpen);

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_DAILY))
    {
        m_nRechargeDailyValue = Answer::Inifile::getInt(&ini, "RechargeDaily", "Value", 0);
        m_nRechargeDailyRewardLimit = Answer::Inifile::getInt(&ini, "RechargeDaily", "Limit", 0);
        int32_t nSize = Answer::Inifile::getInt(&ini, "RechargeDaily", "Size", 0);
        for (int32_t i = 0; i < nSize; ++i)
        {
            snprintf(szCol, sizeof(szCol), "Gift%d", i + 1);
            std::string strGift = Answer::Inifile::getString(&ini, "RechargeDaily", szCol, "");
            std::vector<MemChrBag> vGift;
            Answer::Inifile::parseGift(&strGift, &vGift);
            m_vRechargeDailyReward.push_back(vGift);
        }
    }

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_RANK_DAILY))
    {
        m_nRechargeRankDailyArea = Answer::Inifile::getInt(&ini, "RechargeRankDaily", "Area", 0);
        m_nRechargeRankDailySize = Answer::Inifile::getInt(&ini, "RechargeRankDaily", "Size", 0);
        int32_t nSize = Answer::Inifile::getInt(&ini, "RechargeRankDaily", "Size", 0);
        for (int32_t i = 0; i < nSize; ++i)
        {
            snprintf(szCol, sizeof(szCol), "Reward%d", i + 1);
            std::string strReward = Answer::Inifile::getString(&ini, "RechargeRankDaily", szCol, "");
            std::vector<MemChrBag> vReward;
            Answer::Inifile::parseGift(&strReward, &vReward);
            m_vRechargeRankDailyReward.push_back(vReward);

            snprintf(szCol, sizeof(szCol), "Mail%d", i + 1);
            int32_t nMailId = Answer::Inifile::getInt(&ini, "RechargeRankDaily", szCol, 0);
            m_vRechargeRankDailyMail.push_back(nMailId);
        }
    }

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_TEAM_SHOP_DAILY))
    {
        int32_t nSize = Answer::Inifile::getInt(&ini, "RechargeTeamShopDaily", "Size", 0);
        for (int32_t i = 0; i < nSize; ++i)
        {
            snprintf(szCol, sizeof(szCol), "Value%d", i + 1);
            int32_t nValue = Answer::Inifile::getInt(&ini, "RechargeTeamShopDaily", szCol, 0);
            m_vRechargeTeamShopDailyValue.push_back(nValue);

            snprintf(szCol, sizeof(szCol), "Reward%d", i + 1);
            std::string strReward = Answer::Inifile::getString(&ini, "RechargeTeamShopDaily", szCol, "");
            std::vector<MemChrBag> vReward;
            Answer::Inifile::parseGift(&strReward, &vReward);
            m_vRechargeTeamShopDailyReward.push_back(vReward);
        }
    }

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_ONCE_SHOP))
    {
        m_nOnceShopSize = Answer::Inifile::getInt(&ini, "OnceShop", "Size", 0);
        for (int32_t i = 0; i < m_nOnceShopSize; ++i)
        {
            snprintf(szCol, sizeof(szCol), "Item%d", i + 1);
            std::string strItem = Answer::Inifile::getString(&ini, "OnceShop", szCol, "");
            std::vector<MemChrBag> vItem;
            Answer::Inifile::parseGift(&strItem, &vItem);
            m_vOnceShopItem.push_back(vItem);

            snprintf(szCol, sizeof(szCol), "Price%d", i + 1);
            int32_t nPrice = Answer::Inifile::getInt(&ini, "OnceShop", szCol, 0);
            m_vOnceShopPrice.push_back(nPrice);
        }
    }
}

void CZongHeYunYingHD::loadRankData()
{
}

void CZongHeYunYingHD::SendIconState(Player *player)
{
    if (!player)
        return;

    if (needShowIcon())
    {
        ShowIcon icon;
        getIconState(icon, player);
        Player::SendIconState(player, &icon);
    }
}

void CZongHeYunYingHD::hideIcon(int32_t nIconId)
{
    GameService *pGame = GameService::instance();
    int8_t nConnId = 0;
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, nConnId, Answer::PackType::PACK_DISPATCH, 0x2C6C);
    if (!packet)
        return;

    ShowIcon icon;
    memset(&icon, 0, sizeof(icon));
    icon.nId = nIconId;
    icon.nState = 4;
    icon.nLeftTime = -1;

    Player::fillIconState(packet, &icon);
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, 0, packet);
}

void CZongHeYunYingHD::SendActivityInfo(Player *player)
{
    if (!player)
        return;

    int8_t ConnId = Player::getConnId(player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CF6);
    if (!packet)
        return;

    packet->writeInt32(m_nDay);
    packet->writeInt32(m_nStartDay);
    packet->writeInt32(m_nEndDay);

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_DAILY))
    {
        int32_t nValue = Player::getRecord(player, 1041);
        int32_t nRecord = Player::getRecord(player, 1042);
        packet->writeInt8(1);
        packet->writeInt32(m_nRechargeDailyValue);
        packet->writeInt32(m_nRechargeDailyRewardLimit);
        packet->writeInt32(nValue);
        for (int32_t i = 0; i < m_nRechargeDailyRewardLimit; ++i)
        {
            if ((nRecord >> i) & 1)
                packet->writeInt8(1);
            else
                packet->writeInt8(0);
        }
    }

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_RANK_DAILY))
    {
        checkRankInvalid(ConnId, 2);
        packet->writeInt8(1);
        packet->writeInt32(m_nRechargeRankDailySize);
        packet->writeInt32(m_nRechargeRankDailyArea);
        for (auto &pair : m_mRechargeDailyRank)
        {
            packet->writeInt32(pair.second);
        }
    }

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_TEAM_SHOP_DAILY))
    {
        int32_t RechargePlayer = getRechargePlayer(ConnId);
        packet->writeInt8(1);
        int32_t nSize = (int32_t)m_vRechargeTeamShopDailyValue.size();
        packet->writeInt32(nSize);
        for (int32_t i = 0; i < nSize; ++i)
        {
            packet->writeInt32(m_vRechargeTeamShopDailyValue[i]);
            if (RechargePlayer >= m_vRechargeTeamShopDailyValue[i])
                packet->writeInt8(1);
            else
                packet->writeInt8(0);
        }
    }

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_ONCE_SHOP))
    {
        packet->writeInt8(1);
        packet->writeInt32(m_nOnceShopSize);
        for (int32_t i = 0; i < m_nOnceShopSize; ++i)
        {
            packet->writeInt32((int32_t)m_vOnceShopItem[i].size());
            for (auto &bag : m_vOnceShopItem[i])
                packet->writeMemChrBag(&bag);
            packet->writeInt32(m_vOnceShopPrice[i]);
        }
    }

    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void CZongHeYunYingHD::OnRecharge(Player *player, int32_t nValue)
{
    if (!player || !IsInTime())
        return;

    if (IsInTime(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_DAILY))
    {
        int32_t nOld = Player::getRecord(player, 1041);
        Player::updateRecord(player, 1041, nOld + nValue);
    }

    if (IsInTime(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_RANK_DAILY))
    {
        int8_t ConnId = Player::getConnId(player);
        RankInfo info;
        info.nValue = nValue;
        updateRankInfo(ConnId, 2, &info);
        sendLineSocialPlayerInfo(player, 2, &info);
    }

    if (IsInTime(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_TEAM_SHOP_DAILY))
    {
        int8_t ConnId = Player::getConnId(player);
        int32_t nTeamValue = Player::getRecord(player, 1043);
        Player::updateRecord(player, 1043, nTeamValue + nValue);
        if (true)
            SendIconState(player);
    }
}

void CZongHeYunYingHD::checkRankInvalid(int8_t connid, int8_t nType)
{
}

int32_t CZongHeYunYingHD::GetRechargeDailyReward(Player *player)
{
    if (!player || !IsInTime(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_DAILY))
        return 2;

    int32_t nValue = Player::getRecord(player, 1041);
    int32_t nRecord = Player::getRecord(player, 1042);

    int32_t nIndex = 0;
    if (nValue >= m_nRechargeDailyValue)
        nIndex = nValue / m_nRechargeDailyValue;

    if (nIndex <= 0)
        return 2;

    if (nIndex > m_nRechargeDailyRewardLimit)
        nIndex = m_nRechargeDailyRewardLimit;

    int32_t nNewRecord = 0;
    for (int32_t i = 0; i < nIndex; ++i)
        nNewRecord |= (1 << i);

    if ((nRecord & nNewRecord) == nNewRecord)
        return 2;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return 2;

    for (int32_t i = 0; i < nIndex; ++i)
    {
        if (!((nRecord >> i) & 1))
        {
            std::vector<MemChrBag> *pGift = &m_vRechargeDailyReward[i];
            if (!CExtCharBag::AddItem(pBag, pGift, ITEM_CHANGE_REASON::ICR_ZONGHE_YUNYING_REWARD))
                return 2;
        }
    }

    Player::updateRecord(player, 1042, nNewRecord | nRecord);
    SendIconState(player);
    return 0;
}

int32_t CZongHeYunYingHD::BuyOnceShopItem(Player *player, int32_t nIndex)
{
    if (!player || !IsInTime(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_ONCE_SHOP))
        return 2;

    if (nIndex < 0 || nIndex >= m_nOnceShopSize)
        return 2;

    int32_t nRecord = Player::getRecord(player, 1044);
    if ((nRecord >> nIndex) & 1)
        return 2;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return 2;

    if (!Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, m_vOnceShopPrice[nIndex], CURRENCY_CHANGE_REASON::GCR_BUY_ONCE_SHOP, nIndex))
        return 2;

    if (!CExtCharBag::AddItem(pBag, &m_vOnceShopItem[nIndex], ITEM_CHANGE_REASON::ICR_BUY_ONCE_SHOP))
        return 2;

    Player::updateRecord(player, 1044, nRecord | (1 << nIndex));
    return 0;
}

bool CZongHeYunYingHD::IsInTime()
{
    return m_nDay >= 0 && m_nDay < m_nEndDay;
}

bool CZongHeYunYingHD::needShowIcon()
{
    return IsInTime();
}

bool CZongHeYunYingHD::IsInTime(ZONG_HE_YUN_YING_HD_TYPE nType)
{
    return IsOpen(nType) && IsInTime();
}

bool CZongHeYunYingHD::IsOpen(ZONG_HE_YUN_YING_HD_TYPE nType)
{
    return IsInTime() && isOpen(nType);
}

bool CZongHeYunYingHD::isOpen(ZONG_HE_YUN_YING_HD_TYPE nType)
{
    for (size_t i = 0; i < m_vOpen.size(); ++i)
    {
        if (m_vOpen[i] == (int32_t)nType)
            return true;
    }
    return false;
}

int32_t CZongHeYunYingHD::getLeftTime()
{
    if (!IsInTime())
        return 0;

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);
    return 86400 * (m_nEndDay - m_nDay) - (nNowTime - Answer::DayTime::dayzero());
}

void CZongHeYunYingHD::sendRechargeDailyRankReward()
{
}

int32_t CZongHeYunYingHD::getRechargePlayer(int8_t connid)
{
    auto it = m_mRechargeDailyPlayer.find(connid);
    if (it != m_mRechargeDailyPlayer.end())
        return it->second;
    return 0;
}

void CZongHeYunYingHD::getIconState(ShowIcon &retstr, Player *player)
{
    memset(&retstr, 0, sizeof(retstr));
    if (!player)
        return;

    retstr.nId = m_nIcon;
    retstr.nState = needShowIcon() ? 2 : 4;
    retstr.nLeftTime = getLeftTime();

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_DAILY))
    {
        int32_t nValue = Player::getRecord(player, 1041);
        int32_t nRecord = Player::getRecord(player, 1042);
        int32_t nIndex = nValue / m_nRechargeDailyValue;
        if (nIndex > m_nRechargeDailyRewardLimit)
            nIndex = m_nRechargeDailyRewardLimit;
        for (int32_t i = 0; i < nIndex; ++i)
        {
            if (!((nRecord >> i) & 1))
                ++retstr.nIconRight;
        }
    }

    if (IsOpen(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_TEAM_SHOP_DAILY))
    {
        int8_t ConnId = Player::getConnId(player);
        int32_t nRechargePlayer = getRechargePlayer(ConnId);
        for (size_t i = 0; i < m_vRechargeTeamShopDailyValue.size(); ++i)
        {
            if (nRechargePlayer >= m_vRechargeTeamShopDailyValue[i])
                ++retstr.nIconRight;
        }
    }
}

void CZongHeYunYingHD::GetIconState(Player *player, IconStateList *IconList)
{
    if (!player)
        return;

    if (needShowIcon())
    {
        ShowIcon icon;
        getIconState(icon, player);
        IconList->push_back(icon);
    }
}

void CZongHeYunYingHD::updateRankInfo(int8_t connid, int8_t nType, RankInfo *info)
{
    checkRankInvalid(connid, nType);
}

void CZongHeYunYingHD::UpdateRankInfo(int32_t id, int32_t nValue, int8_t connid, RankInfo *info, RechargeDailyRank *vRank)
{
    checkRankInvalid(connid, (int8_t)info->nValue);
}

void CZongHeYunYingHD::UpdatePlayerInfo(int32_t id, Answer::NetPacket *inPacket)
{
}

void CZongHeYunYingHD::sendLineSocialPlayerInfo(Player *player, int32_t nType, RankInfo *info)
{
}

int32_t CZongHeYunYingHD::GetRechargeTeamShopDailyReward(Player *player, int32_t nIndex)
{
    if (!player || !IsInTime(ZONG_HE_YUN_YING_HD_TYPE::ZHYYHDT_RECHARGE_TEAM_SHOP_DAILY))
        return 2;

    int8_t ConnId = Player::getConnId(player);
    int32_t RechargePlayer = getRechargePlayer(ConnId);

    if (nIndex < 0 || nIndex >= (int32_t)m_vRechargeTeamShopDailyValue.size())
        return 2;

    if (RechargePlayer < m_vRechargeTeamShopDailyValue[nIndex])
        return 2;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return 2;

    if (!CExtCharBag::AddItem(pBag, &m_vRechargeTeamShopDailyReward[nIndex], ITEM_CHANGE_REASON::ICR_ZONGHE_YUNYING_REWARD))
        return 2;

    SendIconState(player);
    return 0;
}
