#include "common.h"
#include "Other/CUniteServer.h"

#include <cstring>
#include <algorithm>

CUniteServer::CUniteServer()
{
    m_nDay = -1;
    m_nMinute = 0;
    m_bDie = true;
    m_MonsterId = 0;
}

CUniteServer::~CUniteServer()
{
}

void CUniteServer::OnDaySwitch()
{
    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return;

    CfgData *pCfg = CfgData::instance();
    if (!CfgData::isUniteServer(pCfg))
        return;

    m_nDay = CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_UNITE);
    if (m_nDay == m_nEndDay)
    {
        Answer::NetPacket *packet = GameService::popNetpacket(pGame, 0, Answer::PackType::PACK_DISPATCH, 0x2CC3);
        if (packet)
        {
            packet->writeInt32(m_nIcon);
            packet->writeInt8(4);
            packet->writeInt32(0);
            packet->writeInt8(0);
            packet->writeInt32(0);
            packet->writeInt8(0);
            packet->setSize(packet->getWOffset());
            GameService::worldBroadcast(pGame, 0, packet);
        }
    }
}

void CUniteServer::OnNewMinute(int32_t nMinute)
{
    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) == 9)
        return;

    CfgData *pCfg = CfgData::instance();
    if (!CfgData::isUniteServer(pCfg))
        return;

    m_nMinute = nMinute;
    checkSpiderQueen();
    checkMysteryShopRefreshBroad();
}

int32_t CUniteServer::GetEndCollectDropTime()
{
    if (!IsInTime(Unit_Server_Activity_typ::US_COLLECT_DROP))
        return 0;

    CfgData *pCfg = CfgData::instance();
    return 86400 * m_vEndDay[7] + CfgData::getServerStartDayZeroTime(pCfg, SERVER_TYPE::SVT_UNITE) - 1;
}

bool CUniteServer::RefreshMysteryShopItem(Player *player)
{
    return player && Player::DecCurrency(player, CURRENCY_TYPE::CURRENCY_GOLD, m_sMysteryShopRefreshCost, CURRENCY_CHANGE_REASON::GCR_MYSTERY_SHOP_REFRESH, 0);
}

void CUniteServer::Init(int32_t line)
{
    if (line == 9)
        return;

    CfgData *pCfg = CfgData::instance();
    if (!CfgData::isUniteServer(pCfg))
        return;

    m_nDay = CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_UNITE);
    CTimer *pTimer = CTimer::instance();
    tm nowTime;
    CTimer::GetLocalNow(&nowTime, pTimer);
    m_nMinute = nowTime.tm_min + 60 * nowTime.tm_hour;

    Answer::Inifile ini;
    ini.parse("./ServerConfig/Tables/UnitServer.cfg");

    m_nStartDay = ini.getIntValue("CONFIG", "start_day");
    m_nEndDay = ini.getIntValue("CONFIG", "end_day");
    m_nIcon = ini.getIntValue("CONFIG", "icon");

    m_vStartDay[1] = ini.getIntValue("HUO_YUE_DU", "start_day");
    m_vEndDay[1] = ini.getIntValue("HUO_YUE_DU", "end_day");
    m_nHuoYueSize = ini.getIntValue("HUO_YUE_DU", "size");

    m_vHuoYueValue.reserve(m_nHuoYueSize);
    m_vHuoYueGift.reserve(m_nHuoYueSize);
    for (int32_t i = 0; i < m_nHuoYueSize; ++i)
    {
        char szCol[64];
        snprintf(szCol, sizeof(szCol), "limit%d", i + 1);
        int32_t nValue = ini.getIntValue("HUO_YUE_DU", szCol);
        m_vHuoYueValue.push_back(nValue);

        snprintf(szCol, sizeof(szCol), "gift%d", i + 1);
        std::string strGift = ini.getStrValue("HUO_YUE_DU", szCol);
        MemChrBagVector vGift;
        CItemHelper::parseItemVectorString(&vGift, &strGift);
        m_vHuoYueGift.push_back(vGift);
    }

    m_vStartDay[2] = ini.getIntValue("RECHARGE", "start_day");
    m_vEndDay[2] = ini.getIntValue("RECHARGE", "end_day");
    m_nRechargeDaysSize = ini.getIntValue("RECHARGE", "size");

    m_vRechargeValue.reserve(m_nRechargeDaysSize);
    m_vRechargeGift.reserve(m_nRechargeDaysSize);
    for (int32_t i = 0; i < m_nRechargeDaysSize; ++i)
    {
        char szCol[64];
        snprintf(szCol, sizeof(szCol), "limit%d", i + 1);
        int32_t nValue = ini.getIntValue("RECHARGE", szCol);
        m_vRechargeValue.push_back(nValue);

        snprintf(szCol, sizeof(szCol), "gift%d", i + 1);
        std::string strGift = ini.getStrValue("RECHARGE", szCol);
        MemChrBagVector vGift;
        CItemHelper::parseItemVectorString(&vGift, &strGift);
        m_vRechargeGift.push_back(vGift);
    }

    m_vStartDay[0] = ini.getIntValue("LIAN_RECHARGE", "start_day");
    m_vEndDay[0] = ini.getIntValue("LIAN_RECHARGE", "end_day");
    LianRechargeCfg stu;
    for (int32_t i = 0; i < 7; ++i)
    {
        char szCol[64];
        snprintf(szCol, sizeof(szCol), "Golds%d", i + 1);
        stu.nValues = ini.getIntValue("LIAN_RECHARGE", szCol);
        snprintf(szCol, sizeof(szCol), "Gift%d", i + 1);
        std::string strGift = ini.getStrValue("LIAN_RECHARGE", szCol);
        CItemHelper::parseItemVectorString(&stu.vGiftVector, &strGift);
        m_LianRechargeCfgVt.push_back(stu);
    }

    m_vStartDay[3] = ini.getIntValue("CHOU_JIANG", "start_day");
    m_vEndDay[3] = ini.getIntValue("CHOU_JIANG", "end_day");

    m_XiaoFeiDrawSize = ini.getIntValue("XIAO_FEI_DRAW", "size");
    m_nXiaoFeiDrawValue.reserve(m_XiaoFeiDrawSize);
    for (int32_t i = 0; i < m_XiaoFeiDrawSize; ++i)
    {
        char szCol[64];
        snprintf(szCol, sizeof(szCol), "limit%d", i + 1);
        int32_t nValue = ini.getIntValue("XIAO_FEI_DRAW", szCol);
        m_nXiaoFeiDrawValue.push_back(nValue);
    }

    m_nSize = ini.getIntValue("WING_LEVEL_UP", "size");
    m_UseItemCount.reserve(m_nSize);
    m_Items.reserve(m_nSize);
    for (int32_t i = 0; i < m_nSize; ++i)
    {
        char szCol[64];
        snprintf(szCol, sizeof(szCol), "limit%d", i + 1);
        int32_t nValue = ini.getIntValue("WING_LEVEL_UP", szCol);
        m_UseItemCount.push_back(nValue);

        snprintf(szCol, sizeof(szCol), "item%d", i + 1);
        std::string strItem = ini.getStrValue("WING_LEVEL_UP", szCol);
        MemChrBag item;
        memset(&item, 0, sizeof(item));
        CItemHelper::parseItemString(&item, &strItem);
        m_Items.push_back(item);
    }

    m_nSpiderQueenIcon = ini.getIntValue("SPIDER_QUEEN", "icon");
    m_nSpiderQueenStartBroadcast = ini.getIntValue("SPIDER_QUEEN", "start_broadcast");
    m_nSpiderQueenEndBroadcast = ini.getIntValue("SPIDER_QUEEN", "end_broadcast");
    nSpiderQueenCount = ini.getIntValue("SPIDER_QUEEN", "count");
    m_vSpiderQueenMinute.reserve(nSpiderQueenCount);
    for (int32_t i = 0; i < nSpiderQueenCount; ++i)
    {
        char szCol[64];
        snprintf(szCol, sizeof(szCol), "minute%d", i + 1);
        std::string strMinute = ini.getStrValue("SPIDER_QUEEN", szCol);
        TimeArea tarea;
        sscanf(strMinute.c_str(), "%d-%d", &tarea.nStart, &tarea.nEnd);
        m_vSpiderQueenMinute.push_back(tarea);
    }

    m_nPrice = ini.getIntValue("MYSTERY_SHOP", "price");
    m_vStartDay[4] = ini.getIntValue("MYSTERY_SHOP", "start_day");
    m_vEndDay[4] = ini.getIntValue("MYSTERY_SHOP", "end_day");

    m_vStartDay[5] = ini.getIntValue("DISCOUNT_GIFT", "start_day");
    m_vEndDay[5] = ini.getIntValue("DISCOUNT_GIFT", "end_day");

    m_vStartDay[6] = ini.getIntValue("CHANGE_NAME", "start_day");
    m_vEndDay[6] = ini.getIntValue("CHANGE_NAME", "end_day");

    m_vStartDay[7] = ini.getIntValue("COLLECT_DROP", "start_day");
    m_vEndDay[7] = ini.getIntValue("COLLECT_DROP", "end_day");

    m_nMysteryShopRefreshBroad = ini.getIntValue("MYSTERY_SHOP", "refresh_broadcast");

    std::string strOpen = ini.getStrValue("MYSTERY_SHOP", "open");
    if (strOpen == "true" || strOpen == "1")
        m_bShowMysteryShop = true;
    else
        m_bShowMysteryShop = false;
}

void CUniteServer::OnGetChouJiangReward(Player *player, int32_t nIndex)
{
    if (!player)
        return;

    if (!IsInTime(Unit_Server_Activity_typ::US_CHOU_JIANG_TIMES))
        return;

    CfgData *pCfg = CfgData::instance();
    const TreasureHunterCfg *CurCfg = CfgData::GetTreasureHunterCfg(pCfg, nIndex);
    if (!CurCfg)
        return;

    if (CurCfg->nType != 2 || Player::getRecord(player, 1412) < CurCfg->nRequire)
        return;

    int32_t Record = Player::getRecord(player, 1413);
    int32_t NewRecord = Record | (1 << (nIndex - 1));
    if (NewRecord == Record)
        return;

    CExtCharBag *Bag = Player::GetBag(player);
    if (CExtCharBag::AddItem(Bag, &CurCfg->Items, ITEM_CHANGE_REASON::IDCR_HE_FU_CHOU_JIANG_TIMES))
    {
        Player::updateRecord(player, 1413, NewRecord);
        GongGao(CurCfg->GongGaoId, player);
        SendIconState(player);
    }
}

void CUniteServer::OnAddChouJiangTimes(Player *player, int32_t AddTimes)
{
    bool NeedSendIcon = false;
    if (IsInTime(Unit_Server_Activity_typ::US_CHOU_JIANG_TIMES))
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(pLimit, 1412, AddTimes);
        NeedSendIcon = true;
    }
    if (NeedSendIcon)
        SendIconState(player);
}

void CUniteServer::OnRecharge(Player *player, int32_t AddValue)
{
    if (!player)
        return;

    bool NeedSendIcon = false;

    if (IsInTime(Unit_Server_Activity_typ::US_LIAN_RECHARGE))
    {
        int32_t OldValue = Player::getRecord(player, 2132);
        if (m_LianRechargeCfgVt[0].nValues > OldValue && OldValue + AddValue >= m_LianRechargeCfgVt[0].nValues)
        {
            CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
            CExtOperateLimit::AddLimitCount(pLimit, 1408, 1);
            NeedSendIcon = true;
        }
        if (m_LianRechargeCfgVt[1].nValues > OldValue && OldValue + AddValue >= m_LianRechargeCfgVt[1].nValues)
        {
            CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
            CExtOperateLimit::AddLimitCount(pLimit, 1409, 1);
            NeedSendIcon = true;
        }
    }

    if (IsInUnitServerTime())
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(pLimit, 2132, AddValue);
    }

    if (NeedSendIcon)
        SendIconState(player);
}

void CUniteServer::GongGao(int32_t GongGaoId, Player *player)
{
    if (!player)
        return;

    int8_t ConnId = Player::getConnId(player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
    if (!packet)
        return;

    packet->writeInt32(GongGaoId);
    std::string val;
    Player::getName(player, &val);
    packet->writeUTF8(&val);
    CharId_t Cid = Player::getCid(player);
    packet->writeInt64(Cid);
    packet->setSize(packet->getWOffset());
    int8_t v7 = Player::getConnId(player);
    GameService::worldBroadcast(pGame, v7, packet);
}

void CUniteServer::AddHuoYueDu(Player *player, int32_t Values)
{
    if (player && Values > 0 && IsInTime(Unit_Server_Activity_typ::US_HUO_YUE_DU))
    {
        int32_t v4 = Values + Player::getRecord(player, 2089);
        Player::updateRecord(player, 2089, v4);
        SendIconState(player);
    }
}

void CUniteServer::AddXiaoFeiValue(Player *player, int32_t Value)
{
    if (player && Value > 0 && IsInTime(Unit_Server_Activity_typ::US_XIAO_FEI_SUM_DRAW))
    {
        int32_t v4 = Value + Player::getRecord(player, 1402);
        Player::updateRecord(player, 1402, v4);
        if (GetCanXiaoFeiDrawTime(player) > 0)
            SendIconState(player);
    }
}

void CUniteServer::AddWingLevelUp(Player *player, int32_t Value)
{
    if (player && Value > 0 && IsInTime(Unit_Server_Activity_typ::US_WING_LEVEL_UP))
    {
        int32_t OldCount = GetWingLevelUpRewardCount(player);
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        CExtOperateLimit::AddLimitCount(pLimit, 1406, Value);
        if (OldCount != GetWingLevelUpRewardCount(player))
            SendIconState(player);
    }
}

int32_t CUniteServer::GetCanXiaoFeiDrawTime(Player *player)
{
    if (!IsInTime(Unit_Server_Activity_typ::US_XIAO_FEI_SUM_DRAW) || !player)
        return 0;

    CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
    int32_t nXiaoFeiSum = CExtOperateLimit::GetLimitCount(pLimit, 1402);
    int32_t nRewardTimes = CExtOperateLimit::GetLimitCount(pLimit, 1403);

    int32_t nCanGetTime = 0;
    for (int32_t i = 0; i < m_XiaoFeiDrawSize; ++i)
    {
        if (m_nXiaoFeiDrawValue[i] <= nXiaoFeiSum)
            nCanGetTime = i + 1;
    }

    int32_t nResult = nCanGetTime - nRewardTimes;
    return nResult > 0 ? nResult : 0;
}

int32_t CUniteServer::GetWingLevelUpRewardCount(Player *player)
{
    if (!IsInTime(Unit_Server_Activity_typ::US_WING_LEVEL_UP) || !player)
        return 0;

    int32_t RewardCount = 0;
    int32_t UseCount = Player::getRecord(player, 1406);
    int32_t nOldRecord = Player::getRecord(player, 1407);

    for (int32_t i = 0; i < m_nSize; ++i)
    {
        if (m_UseItemCount[i] <= UseCount && (nOldRecord & (1 << i)) == 0)
            ++RewardCount;
    }
    return RewardCount;
}

void CUniteServer::GetIconState(Player *player, IconStateList *IconList)
{
    CfgData *pCfg = CfgData::instance();
    if (!CfgData::isUniteServer(pCfg) || !player)
        return;

    if (m_nDay >= m_nStartDay && m_nDay < m_nEndDay)
    {
        ShowIcon icon;
        getIconState(icon, player);
        IconList->push_back(icon);
    }
}

void CUniteServer::GetSpiderQueenIconState(IconStateList *IconList)
{
    if (!IsInTime(Unit_Server_Activity_typ::US_BOSS))
        return;

    ShowIcon icon;
    getSpiderQueenIconState(icon);
    IconList->push_back(icon);
}

void CUniteServer::SendIconState(Player *player)
{
    if (!player)
        return;

    CfgData *pCfg = CfgData::instance();
    if (CfgData::isUniteServer(pCfg) && IsInUnitServerTime())
    {
        ShowIcon stu;
        getIconState(stu, player);
        Player::SendIconState(player, &stu);
    }
}

void CUniteServer::SendSpiderQueenIconState()
{
    GameService *pGame = GameService::instance();
    if (GameService::getLine(pGame) != 1)
        return;

    Answer::NetPacket *packet = GameService::popNetpacket(pGame, 0, Answer::PackType::PACK_DISPATCH, 0x2CC3);
    if (!packet)
        return;

    ShowIcon stu;
    getSpiderQueenIconState(stu);
    packet->writeInt32(stu.nId);
    packet->writeInt8(stu.nState);
    packet->writeInt32(stu.nLeftTime);
    packet->writeInt8(stu.nIconLeft);
    packet->writeInt32(stu.nIconRight);
    packet->writeInt8(stu.nEffects);
    packet->setSize(packet->getWOffset());
    GameService::worldBroadcast(pGame, 0, packet);
}

void CUniteServer::getIconState(ShowIcon &retstr, Player *player)
{
    memset(&retstr, 0, sizeof(retstr));
    if (!player)
        return;

    retstr.nId = m_nIcon;
    retstr.nState = (m_nDay >= m_nStartDay && m_nDay < m_nEndDay) ? 2 : 4;
    retstr.nLeftTime = getLeftTime();

    if (IsInTime(Unit_Server_Activity_typ::US_DAILY_RECHARGE))
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(player);
        int32_t nRewardRecord = CExtOperateLimit::GetLimitCount(pLimit, 2029);
        for (int32_t i = 0; i < m_nRechargeDaysSize; ++i)
        {
            int32_t TodayPayGold = Player::GetTodayPayGold(player);
            int32_t nNewRecord = nRewardRecord | (1 << i);
            if (TodayPayGold >= m_vRechargeValue[i] && nRewardRecord != nNewRecord)
                ++retstr.nIconRight;
        }
    }

    if (IsInTime(Unit_Server_Activity_typ::US_LIAN_RECHARGE))
    {
        int32_t ActCount = Player::getRecord(player, 1408);
        int32_t OldRecord = Player::getRecord(player, 1410);
        for (int32_t i = 0; i < ActCount; ++i)
        {
            int32_t NewRecord = OldRecord | (1 << i);
            if (NewRecord != OldRecord)
                ++retstr.nIconRight;
        }
        int32_t ActCount2 = Player::getRecord(player, 1409);
        int32_t OldRecord2 = Player::getRecord(player, 1411);
        for (int32_t i = 0; i < ActCount2; ++i)
        {
            int32_t NewRecord2 = OldRecord2 | (1 << i);
            if (NewRecord2 != OldRecord2)
                ++retstr.nIconRight;
        }
    }

    if (IsInTime(Unit_Server_Activity_typ::US_CHOU_JIANG_TIMES))
    {
        int32_t ChouJiangTimes = Player::getRecord(player, 1412);
        int32_t RewardRecord = Player::getRecord(player, 1413);
        CfgData *pCfg = CfgData::instance();
        const TreasureHunterCfgMap *TreasureMap = CfgData::GetTreasureHunterCfgMap(pCfg);
        for (auto &pair : *TreasureMap)
        {
            if (pair.second.nRequire <= ChouJiangTimes && pair.second.nType == 2)
            {
                int32_t NewRecord = RewardRecord | (1 << ((uint8_t)pair.first - 1));
                if (NewRecord != RewardRecord)
                    ++retstr.nIconRight;
            }
        }
    }

    if (IsInTime(Unit_Server_Activity_typ::US_XIAO_FEI_SUM_DRAW))
        retstr.nIconRight += GetCanXiaoFeiDrawTime(player);

    if (IsInTime(Unit_Server_Activity_typ::US_WING_LEVEL_UP))
        retstr.nIconRight += GetWingLevelUpRewardCount(player);

    if (IsInTime(Unit_Server_Activity_typ::US_HUO_YUE_DU))
    {
        int32_t nHuoYueDu = Player::getRecord(player, 2089);
        int32_t nOldRecord = Player::getRecord(player, 1068);
        for (int32_t i = 0; i < m_nHuoYueSize; ++i)
        {
            if (m_vHuoYueValue[i] <= nHuoYueDu && (nOldRecord & (1 << i)) == 0)
                ++retstr.nIconRight;
        }
    }
}

void CUniteServer::getSpiderQueenIconState(ShowIcon &retstr)
{
    memset(&retstr, 0, sizeof(retstr));
    retstr.nId = m_nSpiderQueenIcon;
    retstr.nState = (IsInSpiderQueenTime() && !m_bDie) ? 2 : 4;
    retstr.nLeftTime = -1;
}

void CUniteServer::SendUniteServerInfo(Player *player)
{
    if (!player)
        return;

    int8_t ConnId = Player::getConnId(player);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CB5);
    if (!packet)
        return;

    packet->writeInt32(m_nDay);
    packet->writeInt32(m_nStartDay);
    packet->writeInt32(m_nEndDay);
    packet->writeInt32(m_nIcon);

    for (int32_t i = 0; i < 8; ++i)
    {
        packet->writeInt32(m_vStartDay[i]);
        packet->writeInt32(m_vEndDay[i]);
    }

    SendIconState(player);
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

bool CUniteServer::IsInUnitServerTime()
{
    return m_nDay >= 0 && m_nDay < m_nEndDay;
}

bool CUniteServer::IsInTime(Unit_Server_Activity_typ nType)
{
    return IsInUnitServerTime() && m_nDay >= m_vStartDay[nType] && m_nDay < m_vEndDay[nType];
}

int32_t CUniteServer::GetRechargeGift(Player *player, int32_t nIndex)
{
    if (!player || !IsInTime(Unit_Server_Activity_typ::US_DAILY_RECHARGE))
        return 2;

    if (nIndex < 0 || nIndex >= m_nRechargeDaysSize)
        return 2;

    int32_t nRewardRecord = CExtOperateLimit::GetLimitCount(Player::GetOperateLimit(player), 2029);
    if ((nRewardRecord >> nIndex) & 1)
        return 2;

    int32_t TodayPayGold = Player::GetTodayPayGold(player);
    if (TodayPayGold < m_vRechargeValue[nIndex])
        return 2;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return 2;

    if (!CExtCharBag::AddItem(pBag, &m_vRechargeGift[nIndex], ITEM_CHANGE_REASON::IDCR_HE_FU_DAILY_RECHARGE))
        return 2;

    int32_t nNewRecord = nRewardRecord | (1 << nIndex);
    CExtOperateLimit::Set(Player::GetOperateLimit(player), 2029, nNewRecord);
    SendIconState(player);
    return 0;
}

int32_t CUniteServer::BuyDiscountGift(Player *player, int16_t nIndex)
{
    if (!player || !IsInTime(Unit_Server_Activity_typ::US_DISCOUNT_GIFT))
        return 2;

    return 2;
}

int32_t CUniteServer::GetHuoYueDuGift(Player *player, int32_t nIndex)
{
    if (!player || !IsInTime(Unit_Server_Activity_typ::US_HUO_YUE_DU))
        return 2;

    if (nIndex < 0 || nIndex >= m_nHuoYueSize)
        return 2;

    int32_t nHuoYueDu = Player::getRecord(player, 2089);
    if (nHuoYueDu < m_vHuoYueValue[nIndex])
        return 2;

    int32_t nOldRecord = Player::getRecord(player, 1068);
    int32_t nNewRecord = nOldRecord | (1 << nIndex);
    if (nOldRecord == nNewRecord)
        return 2;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return 2;

    if (!CExtCharBag::AddItem(pBag, &m_vHuoYueGift[nIndex], ITEM_CHANGE_REASON::IDCR_HE_FU_HUO_YUE_DU))
        return 2;

    Player::updateRecord(player, 1068, nNewRecord);
    SendIconState(player);
    return 0;
}

int32_t CUniteServer::BuyChangeNameCard(Player *player)
{
    if (!player || !IsInTime(Unit_Server_Activity_typ::US_CHANGE_NAME))
        return 2;

    return 2;
}

int32_t CUniteServer::GetWingLevelUpGift(Player *player, int32_t nIndex)
{
    if (!player || !IsInTime(Unit_Server_Activity_typ::US_WING_LEVEL_UP))
        return 2;

    if (nIndex < 0 || nIndex >= m_nSize)
        return 2;

    int32_t UseCount = Player::getRecord(player, 1406);
    if (UseCount < m_UseItemCount[nIndex])
        return 2;

    int32_t nOldRecord = Player::getRecord(player, 1407);
    int32_t nNewRecord = nOldRecord | (1 << nIndex);
    if (nOldRecord == nNewRecord)
        return 2;

    CExtCharBag *pBag = Player::GetBag(player);
    if (!pBag)
        return 2;

    MemChrBagVector items;
    items.push_back(m_Items[nIndex]);
    if (!CExtCharBag::AddItem(pBag, &items, ITEM_CHANGE_REASON::IDCR_HE_FU_WING_LEVEL_UP))
        return 2;

    Player::updateRecord(player, 1407, nNewRecord);
    SendIconState(player);
    return 0;
}

bool CUniteServer::IsInSpiderQueenTime()
{
    if (!IsInTime(Unit_Server_Activity_typ::US_BOSS))
        return false;

    for (size_t i = 0; i < m_vSpiderQueenMinute.size(); ++i)
    {
        if (m_nMinute >= m_vSpiderQueenMinute[i].nStart && m_nMinute < m_vSpiderQueenMinute[i].nEnd)
            return true;
    }
    return false;
}

void CUniteServer::BossDie(int32_t MonsterId, std::string *p_name, CharId_t Cid)
{
}

void CUniteServer::checkSpiderQueen()
{
}

void CUniteServer::broadcastSpiderQueenStart()
{
}

void CUniteServer::broadcastSpiderQueenEnd()
{
}

void CUniteServer::appendRechargeInfo(Answer::NetPacket *packet, Player *player)
{
}

void CUniteServer::appendDiscountGiftInfo(Answer::NetPacket *packet, Player *player)
{
}

void CUniteServer::appendHuoYueDuInfo(Answer::NetPacket *packet, Player *player)
{
}

int32_t CUniteServer::getLeftTime()
{
    if (!IsInUnitServerTime())
        return 0;

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);
    return 86400 * (m_nEndDay - m_nDay) - (nNowTime - Answer::DayTime::dayzero());
}

void CUniteServer::checkMysteryShopRefreshBroad()
{
}

int32_t CUniteServer::GetChouJiangType()
{
    return 0;
}

int32_t CUniteServer::GetChouJiangSpecialTime()
{
    return 0;
}
