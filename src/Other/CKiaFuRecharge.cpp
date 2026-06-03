#include "common.h"
#include "Other/CKiaFuRecharge.h"

#include <cstring>
#include <algorithm>

CKiaFuRecharge::CKiaFuRecharge()
{
    m_nDay = -1;
    m_nMinute = 0;
}

CKiaFuRecharge::~CKiaFuRecharge()
{
}

void CKiaFuRecharge::Init(int32_t line)
{
    CfgData *pCfg = CfgData::instance();
    m_nDay = CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL);
    CTimer *pTimer = CTimer::instance();
    tm nowTime;
    CTimer::GetLocalNow(&nowTime, pTimer);
    m_nMinute = nowTime.tm_hour * 60 + nowTime.tm_min;

    initCfgData();
}

void CKiaFuRecharge::OnDaySwitch()
{
    CfgData *pCfg = CfgData::instance();
    m_nDay = CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL);

    if (m_nDay == m_nEndDay)
        hideIcon(m_nIcon);
    if (m_nDay == m_vEndDay[3])
        hideIcon(m_ChouJiangIcon);
}

void CKiaFuRecharge::OnGetRechargeSumReward(Player *player, int32_t nIndex)
{
    if (!player)
        return;

    if (!IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_RECHARGE_SUM))
        return;

    CfgData *pCfg = CfgData::instance();
    int32_t nRecord = Player::getRecord(player, 1033);
    int32_t nRechargeSum = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 1044);

    if (nIndex == 1)
    {
        if (nRecord > 0 || nRechargeSum < 60)
            return;

        std::vector<MemChrBag> *pGift = nullptr;
        CExtCharBag *pBag = Player::GetBag(player);
        if (pBag && pGift)
        {
            CExtCharBag::AddItem(pBag, pGift, ITEM_CHANGE_REASON::ICR_KAIFU_RECHARGE_REWARD);
        }

        Player::updateRecord(player, 1033, 1);
        GongGao(3012, player);
        SendIconState(player);
    }
}

void CKiaFuRecharge::OnGetChouJiangReward(Player *player, int32_t nIndex)
{
    if (!player)
        return;

    if (!IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_CHOU_JIANG_TIMES))
        return;

    CfgData *pCfg = CfgData::instance();
    int32_t nRecord = Player::getRecord(player, 1034);

    const TreasureHunterCfgMap *pMap = CfgData::GetTreasureHunterCfgMap(pCfg);
    const TreasureHunterCfg *pCurCfg = nullptr;
    int32_t nCount = 0;
    for (auto &pair : *pMap)
    {
        if (pair.second.nType == 4)
        {
            if (nCount == nRecord)
            {
                pCurCfg = &pair.second;
                break;
            }
            nCount++;
        }
    }

    if (!pCurCfg)
        return;

    if (nIndex <= 0 || nIndex > (int32_t)pCurCfg->vReward.size())
        return;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return;

    const MemChrBag *pReward = &pCurCfg->vReward[nIndex - 1];
    MemChrBagVector items;
    items.push_back(*pReward);
    if (CExtCharBag::AddItem(pBag, &items, ITEM_CHANGE_REASON::ICR_KAIFU_CHOUJIANG_REWARD))
    {
        int32_t nNewRecord = nRecord | (1 << (nIndex - 1));
        Player::updateRecord(player, 1034, nNewRecord);
        if (pCurCfg->GongGaoId > 0)
            GongGao(pCurCfg->GongGaoId, player);
        SendChouJiangIconState(player);
    }
}

void CKiaFuRecharge::OnGetXiaoFeiReward(Player *player, int32_t nIndex)
{
    if (!player)
        return;

    if (!IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_XIAO_FEI_SUM))
        return;

    int32_t nRecord = Player::getRecord(player, 1035);
    if ((nRecord >> nIndex) & 1)
        return;

    int32_t nXiaoFeiSum = Player::getRecord(player, 1036);
    if (nIndex >= 0 && nIndex < (int32_t)m_vXiaoFeiSumGift.size())
    {
        std::vector<MemChrBag> *pGift = &m_vXiaoFeiSumGift[nIndex];
        CExtCharBag *pBag = Player::GetBag(player);
        if (pBag)
        {
            CExtCharBag::AddItem(pBag, pGift, ITEM_CHANGE_REASON::ICR_KAIFU_XIAOFEI_REWARD);
        }
    }

    int32_t nNewRecord = nRecord | (1 << nIndex);
    Player::updateRecord(player, 1035, nNewRecord);

    int32_t nGongGaoId = m_vXiaoFeiGongGaoId[nIndex];
    if (nGongGaoId > 0)
        GongGao(nGongGaoId, player);

    SendIconState(player);
}

void CKiaFuRecharge::OnXiaoFeiSum(Player *player, int32_t AddTimes)
{
    if (!player)
        return;

    bool bNeedSendIcon = false;
    if (IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_XIAO_FEI_SUM))
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(pLimit, 1045, AddTimes);
        int32_t nTotal = CExtOperateLimit::GetLimitCount(pLimit, 1045);
        bNeedSendIcon = nTotal >= 50;
    }

    if (bNeedSendIcon)
        SendIconState(player);
}

void CKiaFuRecharge::OnRecharge(Player *player, int32_t OldValue, int32_t AddValue)
{
    if (!player)
        return;

    bool bNeedSendIcon = false;

    if (IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_RECHARGE_SUM))
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(pLimit, 1044, AddValue);
        int32_t nValue = CExtOperateLimit::GetLimitCount(pLimit, 1044);
        if (!Player::getRecord(player, 1033) && nValue >= 60)
            bNeedSendIcon = true;
    }

    if (IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_LIAN_RECHARGE))
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(pLimit, 1046, AddValue);
    }

    if (bNeedSendIcon)
        SendIconState(player);
}

void CKiaFuRecharge::OnAddChouJiangTimes(Player *player, int32_t AddTimes)
{
    bool bNeedSendIcon = false;
    if (IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_CHOU_JIANG_TIMES))
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(pLimit, 1047, AddTimes);
        bNeedSendIcon = true;
    }

    if (bNeedSendIcon)
        SendChouJiangIconState(player);
}

void CKiaFuRecharge::SendChouJiangIconState(Player *player)
{
    if (!player)
        return;

    if (needShowShouJinagIcon())
    {
        ShowIcon icon;
        getChouJiangIconState(&icon, player);
        Player::SendIconState(player, &icon);
    }
}

bool CKiaFuRecharge::needShowShouJinagIcon()
{
    CfgData *pCfg = CfgData::instance();
    if (CfgData::getServerStartDayZeroTime(pCfg, SERVER_TYPE::SVT_NORMAL) <= 1565107199)
        return false;

    return m_nDay >= m_vStartDay[3] && m_nDay < m_vEndDay[3];
}

void CKiaFuRecharge::getChouJiangIconState(ShowIcon *retstr, Player *player)
{
    memset(retstr, 0, sizeof(*retstr));
    retstr->nId = m_ChouJiangIcon;
    if (needShowShouJinagIcon())
    {
        if (HaveChouJiangCount(player) > 0)
            retstr->nState = 2;
        else
            retstr->nState = 1;
    }
    else
    {
        retstr->nState = 4;
    }
    retstr->nIconRight = HaveChouJiangCount(player);
    retstr->nLeftTime = GetChouJiangLeftTime();
}

int32_t CKiaFuRecharge::GetChouJiangLeftTime()
{
    if (!IsInTime(KAI_FU_RECHARGE_TYPE::KFRT_CHOU_JIANG_TIMES))
        return 0;

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);
    return 86400 * (m_vEndDay[3] - m_nDay) - (nNowTime - Answer::DayTime::dayzero());
}

bool CKiaFuRecharge::IsInTime(KAI_FU_RECHARGE_TYPE nType)
{
    CfgData *pCfg = CfgData::instance();

    if (nType == KAI_FU_RECHARGE_TYPE::KFRT_XIAO_FEI_SUM)
    {
        if (CfgData::getServerStartTime(pCfg, SERVER_TYPE::SVT_NORMAL) <= 1566316799)
            return false;
    }

    if (nType == KAI_FU_RECHARGE_TYPE::KFRT_CHOU_JIANG_TIMES)
    {
        if (CfgData::getServerStartDayZeroTime(pCfg, SERVER_TYPE::SVT_NORMAL) <= 1565107199)
            return false;
    }

    return m_nDay >= 0 && m_nDay < m_nEndDay && m_nDay >= m_vStartDay[nType] && m_nDay < m_vEndDay[nType];
}

void CKiaFuRecharge::initCfgData()
{
    m_LianRechargeCfgVt.clear();
    m_vSumGolds.clear();
    m_vSumGift.clear();
    m_nValues.clear();
    m_vXiaoFeiGongGaoId.clear();
    m_vXiaoFeiSumGift.clear();

    std::string file = Answer::Inifile::getFilePath("KaifuRecharge.ini");
    Answer::Inifile ini;
    Answer::Inifile::Inifile(&ini, &file);

    char szCol[64];
    m_nIcon = Answer::Inifile::getInt(&ini, "Icon", "IconId", 0);
    m_nEndDay = Answer::Inifile::getInt(&ini, "Time", "EndDay", 0);
    m_ChouJiangIcon = Answer::Inifile::getInt(&ini, "ChouJiang", "IconId", 0);

    for (int32_t i = 0; i < 6; i++)
    {
        snprintf(szCol, sizeof(szCol), "StartDay%d", i + 1);
        m_vStartDay[i] = Answer::Inifile::getInt(&ini, "Time", szCol, 0);
        snprintf(szCol, sizeof(szCol), "EndDay%d", i + 1);
        m_vEndDay[i] = Answer::Inifile::getInt(&ini, "Time", szCol, 0);
    }

    int32_t nSize = Answer::Inifile::getInt(&ini, "RechargeSum", "Size", 0);
    for (int32_t i = 0; i < nSize; i++)
    {
        snprintf(szCol, sizeof(szCol), "Golds%d", i + 1);
        int32_t nGold = Answer::Inifile::getInt(&ini, "RechargeSum", szCol, 0);
        m_vSumGolds.push_back(nGold);

        snprintf(szCol, sizeof(szCol), "Gift%d", i + 1);
        std::string strGift = Answer::Inifile::getString(&ini, "RechargeSum", szCol, "");
        std::vector<MemChrBag> vGift;
        Answer::Inifile::parseGift(&strGift, &vGift);
        m_vSumGift.push_back(vGift);
    }

    m_nXiaoFeiSize = Answer::Inifile::getInt(&ini, "XiaoFeiSum", "Size", 0);
    for (int32_t i = 0; i < m_nXiaoFeiSize; i++)
    {
        snprintf(szCol, sizeof(szCol), "Values%d", i + 1);
        int32_t nValue = Answer::Inifile::getInt(&ini, "XiaoFeiSum", szCol, 0);
        m_nValues.push_back(nValue);

        snprintf(szCol, sizeof(szCol), "GongGaoId%d", i + 1);
        int32_t nGongGaoId = Answer::Inifile::getInt(&ini, "XiaoFeiSum", szCol, 0);
        m_vXiaoFeiGongGaoId.push_back(nGongGaoId);

        snprintf(szCol, sizeof(szCol), "Gift%d", i + 1);
        std::string strGift = Answer::Inifile::getString(&ini, "XiaoFeiSum", szCol, "");
        std::vector<MemChrBag> vGift;
        Answer::Inifile::parseGift(&strGift, &vGift);
        m_vXiaoFeiSumGift.push_back(vGift);
    }

    LianRechargeCfg stu;
    for (int32_t i = 0; i < 7; i++)
    {
        snprintf(szCol, sizeof(szCol), "Golds%d", i + 1);
        stu.nGolds = Answer::Inifile::getInt(&ini, "LianRecharge", szCol, 0);
        snprintf(szCol, sizeof(szCol), "Gift%d", i + 1);
        std::string strGift = Answer::Inifile::getString(&ini, "LianRecharge", szCol, "");
        Answer::Inifile::parseGift(&strGift, &stu.vGift);
        m_LianRechargeCfgVt.push_back(stu);
    }
}

bool CKiaFuRecharge::IsInTime()
{
    CfgData *pCfg = CfgData::instance();
    CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL);
    return m_nDay >= 0 && m_nDay < m_nEndDay;
}

void CKiaFuRecharge::GetIconState(Player *player, IconStateList *IconList)
{
    if (!player)
        return;

    if (needShowIcon())
    {
        ShowIcon stu;
        getIconState(&stu, player);
        IconList->push_back(stu);
    }
}

void CKiaFuRecharge::SendIconState(Player *player)
{
    if (!player)
        return;

    if (needShowIcon())
    {
        ShowIcon icon;
        getIconState(&icon, player);
        Player::SendIconState(player, &icon);
    }
}

void CKiaFuRecharge::getIconState(ShowIcon *retstr, Player *player)
{
    memset(retstr, 0, sizeof(*retstr));
    retstr->nId = m_nIcon;

    if (m_nDay >= m_nStartDay && m_nDay < m_nEndDay)
    {
        int32_t nCount = HaveRechargeRewardCount(player);
        if (nCount > 0)
            retstr->nState = 2;
        else
            retstr->nState = 1;
    }
    else
    {
        retstr->nState = 4;
    }

    retstr->nIconRight = HaveRechargeRewardCount(player);
    retstr->nLeftTime = getLeftTime();
}

bool CKiaFuRecharge::needShowIcon()
{
    CfgData *pCfg = CfgData::instance();
    if (CfgData::getServerStartDayZeroTime(pCfg, SERVER_TYPE::SVT_NORMAL) <= 1565107199)
        return false;

    return m_nDay >= 0 && m_nDay < m_nEndDay;
}

void CKiaFuRecharge::hideIcon(int32_t nIconId)
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
    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, 0, packet);
}

int32_t CKiaFuRecharge::getLeftTime()
{
    if (!IsInTime())
        return 0;

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);
    return 86400 * (m_nEndDay - m_nDay) - (nNowTime - Answer::DayTime::dayzero());
}

void CKiaFuRecharge::GongGao(int32_t GongGaoId, Player *player)
{
    if (!player)
        return;

    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, Player::getConnId(player), Answer::PackType::PACK_DISPATCH, 0x2CB3);
    if (!packet)
        return;

    packet->writeInt16(Player::getGateIndex(player));
    CharId_t nCid = Player::getCid(player);
    packet->writeInt64(nCid);
    std::string val;
    Player::getName(player, &val);
    packet->writeUTF8(&val);
    packet->writeInt32(GongGaoId);

    uint32_t nWOffset = packet->getWOffset();
    packet->setSize(nWOffset);
    GameService::sendPacket(pGame, Player::getConnId(player), packet);
}

int32_t CKiaFuRecharge::HaveRechargeRewardCount(Player *player)
{
    if (!player)
        return 0;

    int32_t nCount = 0;
    int32_t nRechargeSum = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 1044);
    int32_t nRecord = Player::getRecord(player, 1033);

    for (size_t i = 0; i < m_vSumGolds.size(); i++)
    {
        if (nRechargeSum >= m_vSumGolds[i])
        {
            if (!((nRecord >> i) & 1))
                nCount++;
        }
    }
    return nCount;
}

int32_t CKiaFuRecharge::HaveChouJiangCount(Player *player)
{
    if (!player)
        return 0;

    int32_t nCount = 0;
    CfgData *pCfg = CfgData::instance();
    const TreasureHunterCfgMap *pMap = CfgData::GetTreasureHunterCfgMap(pCfg);
    int32_t nRecord = Player::getRecord(player, 1034);

    int32_t nIdx = 0;
    for (auto &pair : *pMap)
    {
        if (pair.second.nType == 4)
        {
            if (!((nRecord >> nIdx) & 1))
                nCount++;
            nIdx++;
        }
    }
    return nCount;
}
