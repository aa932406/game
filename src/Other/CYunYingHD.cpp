#include "Other/CYunYingHD.h"

#include <cstring>
#include <algorithm>

CYunYingHD::CYunYingHD()
{
    m_TotalChongZhiDay = 0;
}

CYunYingHD::~CYunYingHD()
{
}

void CYunYingHD::OnSaveToDB(PlayerDBData *dbData)
{
    // m_TotalChongZhiDay 从配置读取，无需持久化到 DB
}

void CYunYingHD::OnLoadFromDB(const PlayerDBData *dbData)
{
    CfgData *pCfg = CfgData::instance();
    CfgTotalChongZhi *pCfgInfo = CfgData::GetTotalChongZhiCfg(pCfg, 1);
    if (pCfgInfo)
        m_TotalChongZhiDay = pCfgInfo->NewServerday;
}

void CYunYingHD::GetInterestsProtocol(ProcIdList *procList)
{
    if (!procList)
        return;

    procList->push_back(482);
    procList->push_back(483);
    procList->push_back(484);
    procList->push_back(485);
    procList->push_back(486);
    procList->push_back(487);
    procList->push_back(489);
    procList->push_back(515);
    procList->push_back(518);
    procList->push_back(20257);
    procList->push_back(519);
}

int32_t CYunYingHD::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return 2;

    if (nProcId == 487)
        return OnGetEveryDayChongZhiGift(inPacket);
    if (nProcId > 0x1E7)
    {
        if (nProcId == 518)
            return OnGetMobilePhoneGift(inPacket);
        if (nProcId > 0x206)
        {
            if (nProcId == 519)
                return OnGetZeroBuyPetGift(inPacket);
            if (nProcId == 20257)
                return OnDBGetMobilePhoneGift(inPacket);
        }
        else
        {
            if (nProcId == 489)
                return OnGetTotalChongZhi(inPacket);
            if (nProcId == 515)
                SendTotalChongZhiInfo();
        }
    }
    else if (nProcId == 484)
    {
        SendTeHuiInfo();
    }
    else if (nProcId > 0x1E4)
    {
        if (nProcId == 485)
            return OnGetBuyTeHuiItem(inPacket);
        if (nProcId == 486)
        {
            int8_t Int8 = inPacket->readInt8();
            SendEveryDayChongZhiInfo(Int8);
        }
    }
    else
    {
        if (nProcId == 482)
            return OnGetShouChongReward(inPacket);
        if (nProcId == 483)
            SendShouChongInfo();
    }
    return 0;
}

void CYunYingHD::OnCleanUp()
{
    m_TotalChongZhiDay = 0;
    m_nLastTick = 0;
    m_nLastTeHui = 0;
}

void CYunYingHD::OnUpdate(int64_t curTick)
{
    if (curTick - m_nLastTick <= 999)
        return;

    m_nLastTick = curTick;
    checkTeHuiTime();

    if (!m_pPlayer)
        return;

    int32_t now = Unit::getNow(m_pPlayer);
    int32_t EndTime = Player::getRecord(m_pPlayer, 37502);
    if (EndTime > 0 && EndTime < now)
    {
        Player::updateRecord(m_pPlayer, 37502, 0);
        SendZeroBuyPetIcon();
    }

    int32_t BackTime = Player::getRecord(m_pPlayer, 37503);
    if (BackTime > 0 && BackTime < now)
    {
        CfgData *pCfg = CfgData::instance();
        const ZeroBuyPetCfg *pCfgInfo = CfgData::GetZeroBuyPetCfg(pCfg);
        if (pCfgInfo)
        {
            MemChrBag mailItem;
            memset(&mailItem, 0, sizeof(mailItem));
            mailItem.itemId = 4;
            mailItem.itemCount = pCfgInfo->nGold;

            CharId_t Cid = Player::getCid(m_pPlayer);
            std::string Param;
            DBService *pDB = Answer::Singleton<DBService>::instance();
            DBService::OnSendSysMail(pDB, 0, Cid, pCfgInfo->nMailId, &mailItem, ITEM_CHANGE_REASON::ICR_ZERO_BY_PET, &Param, 0);
        }
        Player::updateRecord(m_pPlayer, 37503, 0);
    }
}

void CYunYingHD::OnDaySwitch(int32_t nDiffDays)
{
    if (!m_pPlayer)
        return;

    SendEveryDayChongZhiInfo(3);
    SendEveryDayChongZhiInfo(2);
    SendEveryDayChongZhiIcon(3);
    SendEveryDayChongZhiIcon(2);
    SendTotalChongZhiIcon();

    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    CfgData *pCfg = CfgData::instance();
    const CfgSuperMember *pSuperMember = CfgData::GetSuperMember(pCfg, &platform);
    if (pSuperMember)
    {
        CExtOperateLimit *pLimit = Player::GetOperateLimit(m_pPlayer);
        int32_t LimitCount = CExtOperateLimit::GetLimitCount(pLimit, 1114);
        if (Answer::DayTime::monthdiff(LimitCount) > 0)
        {
            int32_t Record = Player::getRecord(m_pPlayer, 1115);
            if (Record < pSuperMember->nGold)
            {
                CExtOperateLimit *pLimit2 = Player::GetOperateLimit(m_pPlayer);
                CExtOperateLimit::Reset(pLimit2, 1115);
            }
        }
    }
}

int32_t CYunYingHD::OnGetShouChongReward(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t nIndex = inPacket->readInt8();
    int32_t nRecord = Player::getRecord(m_pPlayer, 1026);
    int32_t nNewRecord = nRecord | (1 << (nIndex - 1));
    if (nRecord == nNewRecord)
        return 2;

    CfgData *pCfg = CfgData::instance();
    CfgShouChongTable *pTable = CfgData::GetShouChongLiBao(pCfg);
    const CfgShouChong *pCfgInfo = CfgShouChongTable::GetShouChong(pTable, nIndex);
    if (!pCfgInfo)
        return 2;

    if (Player::GetTotalPayGold(m_pPlayer) < pCfgInfo->nGold)
        return 2;

    MemChrBagVector addItemVt;
    MemEquipVector vEquip;

    for (auto it = pCfgInfo->vReward.begin(); it != pCfgInfo->vReward.end(); ++it)
    {
        Job_t Job = Player::getJob(m_pPlayer);
        if (Job != it->job && it->job)
            continue;

        MemChrBag chrbag;
        memset(&chrbag, 0, sizeof(chrbag));
        chrbag.itemId = it->id;
        chrbag.itemClass = it->type;
        chrbag.itemCount = it->count;
        chrbag.bind = it->bind;
        chrbag.endTime = 0;

        if (chrbag.itemClass == 2)
        {
            std::string p_name;
            Player::getName(m_pPlayer, &p_name);
            CharId_t nCreaterId = Player::getCid(m_pPlayer);
            int32_t Sid = Player::getSid(m_pPlayer);
            int32_t nBaseId = chrbag.itemId;
            int8_t ConnId = Player::getConnId(m_pPlayer);
            CEquipManager *pEquipMgr = Answer::Singleton<CEquipManager>::instance();
            MemEquip equip;
            CEquipManager::CreateMemEquip(&equip, pEquipMgr, ConnId, 601, nBaseId, Sid, nCreaterId, &p_name, 0, 0, it->star, 0, 0);
            if (equip.base <= 0 || equip.id <= 0)
                return 0;
            chrbag.srcId = equip.id;
            vEquip.push_back(equip);
        }
        addItemVt.push_back(chrbag);
    }

    CExtCharBag *Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &addItemVt, ITEM_CHANGE_REASON::ICR_SEVEN_LOGIN_REWARD))
        return 2;

    if (!vEquip.empty())
    {
        CEquipManager *pEquipMgr = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::SendPlayerEquipInfo(pEquipMgr, m_pPlayer, &vEquip);
    }

    Player::updateRecord(m_pPlayer, 1026, nNewRecord);
    GongGao(pCfgInfo->nBroadcast);
    SendShouChongIcon();
    SendShouChongInfo();
    SendEveryDayChongZhiIcon(3);

    CExtCharMysteryGift *pGift = Player::GetCharMysteryGift(m_pPlayer);
    CExtCharMysteryGift::SendMysteryGiftInfo(pGift, 1);

    LogDailyGift logDailyGift;
    memset(&logDailyGift, 0, sizeof(logDailyGift));
    logDailyGift.cid = Player::getCid(m_pPlayer);
    logDailyGift.type = 1;
    logDailyGift.giftid = 0;
    logDailyGift.time = Unit::getNow(m_pPlayer);
    int8_t v17 = Player::getConnId(m_pPlayer);
    DBService *pDB = Answer::Singleton<DBService>::instance();
    DBService::InsertDailyGiftLog(pDB, v17, &logDailyGift);

    return 0;
}

int32_t CYunYingHD::OnGetBuyTeHuiItem(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t Index = inPacket->readInt8();
    CfgData *pCfg = CfgData::instance();
    NewServerFavorable *pCfgInfo = CfgData::GetNewServerFavorable(pCfg, Index);
    if (!pCfgInfo)
        return 2;

    int32_t OldRecord = Player::getRecord(m_pPlayer, 1027);
    int32_t NewRecord = OldRecord | (1 << (Index - 1));
    if (OldRecord == NewRecord)
        return 2;

    int32_t v4 = Player::getCreateTime(m_pPlayer) + pCfgInfo->nLimitTime;
    if (v4 < Unit::getNow(m_pPlayer))
        return 2;

    if (pCfgInfo->NeedGold <= 0)
        return 2;

    int64_t HaveMoney = Player::GetCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD);
    if (pCfgInfo->NeedGold > HaveMoney)
        return 2;

    MemChrBagVector addItemVt;
    MemEquipVector vEquip;

    for (auto it = pCfgInfo->ItemVector.begin(); it != pCfgInfo->ItemVector.end(); ++it)
    {
        Job_t Job = Player::getJob(m_pPlayer);
        if (Job != it->job && it->job)
            continue;

        MemChrBag chrbag;
        memset(&chrbag, 0, sizeof(chrbag));
        chrbag.itemId = it->id;
        chrbag.itemClass = it->type;
        chrbag.itemCount = it->count;
        chrbag.bind = it->bind;

        if (chrbag.itemClass == 2)
        {
            std::string p_name;
            Player::getName(m_pPlayer, &p_name);
            CharId_t nCreaterId = Player::getCid(m_pPlayer);
            int32_t Sid = Player::getSid(m_pPlayer);
            int32_t nBaseId = chrbag.itemId;
            int8_t ConnId = Player::getConnId(m_pPlayer);
            CEquipManager *pEquipMgr = Answer::Singleton<CEquipManager>::instance();
            MemEquip equip;
            CEquipManager::CreateMemEquip(&equip, pEquipMgr, ConnId, 602, nBaseId, Sid, nCreaterId, &p_name, 0, 0, it->star, 0, 0);
            if (equip.base <= 0 || equip.id <= 0)
                return 0;
            chrbag.srcId = equip.id;
            vEquip.push_back(equip);
        }
        addItemVt.push_back(chrbag);
    }

    CExtCharBag *Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &addItemVt, ITEM_CHANGE_REASON::ICR_NEW_SERVER_TE_HUI))
        return 2;

    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfgInfo->NeedGold, CURRENCY_CHANGE_REASON::GCR_BUY_TE_HUI_LI_BAO, Index))
        return 2;

    if (!vEquip.empty())
    {
        CEquipManager *pEquipMgr = Answer::Singleton<CEquipManager>::instance();
        CEquipManager::SendPlayerEquipInfo(pEquipMgr, m_pPlayer, &vEquip);
    }

    Player::updateRecord(m_pPlayer, 1027, NewRecord);
    GongGao(pCfgInfo->GongGaoInfo.begin()->GongGaoId);
    SendTeHuiInfo();
    SendTeHuiIcon();
    return 0;
}

int32_t CYunYingHD::OnGetEveryDayChongZhiGift(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t nType = inPacket->readInt8();
    int8_t nIndex = inPacket->readInt8();

    CfgData *pCfg = CfgData::instance();
    const CfgEveryDayChongZhi *pCfgInfo = CfgData::GetEveryDayChongZhi(pCfg, nType, nIndex);
    if (!pCfgInfo)
        return 2;

    int32_t OldRecord = Player::getRecord(m_pPlayer, pCfgInfo->nRecord);
    int32_t nNewRecord = OldRecord | (1 << (nIndex - 1));
    if (OldRecord == nNewRecord)
        return 2;

    int32_t nTodayPayGold = Player::GetTodayPayGold(m_pPlayer);
    if (nTodayPayGold < pCfgInfo->nGolds)
        return 2;

    CExtCharBag *Bag = Player::GetBag(m_pPlayer);
    if (!Bag)
        return 2;

    MemChrBagVector vGift;
    vGift.push_back(pCfgInfo->vReward);
    if (!CExtCharBag::AddItem(Bag, &vGift, ITEM_CHANGE_REASON::ICR_TOTAL_CHONG_ZHI))
        return 2;

    Player::updateRecord(m_pPlayer, pCfgInfo->nRecord, nNewRecord);
    GongGao(pCfgInfo->nBroadcast);
    SendEveryDayChongZhiInfo(nType);
    SendEveryDayChongZhiIcon(nType);
    return 0;
}

int32_t CYunYingHD::OnGetTotalChongZhi(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;

    int8_t nIndex = inPacket->readInt8();

    if (!IsInTime())
        return 2;

    CfgData *pCfg = CfgData::instance();
    const CfgTotalChongZhi *pCfgInfo = CfgData::GetTotalChongZhiCfg(pCfg, nIndex);
    if (!pCfgInfo)
        return 2;

    int32_t OldRecord = Player::getRecord(m_pPlayer, 1028);
    int32_t nNewRecord = OldRecord | (1 << (nIndex - 1));
    if (OldRecord == nNewRecord)
        return 2;

    CfgData *pCfg2 = CfgData::instance();
    CfgData::getServerDiffDay(pCfg2, SERVER_TYPE::SVT_NORMAL);

    int32_t nTotalChongZhi = Player::getRecord(m_pPlayer, 1037);
    if (nTotalChongZhi < pCfgInfo->nGolds)
        return 2;

    CExtCharBag *Bag = Player::GetBag(m_pPlayer);
    if (!Bag)
        return 2;

    MemChrBagVector vGift;
    vGift.push_back(pCfgInfo->vReward);
    if (!CExtCharBag::AddItem(Bag, &vGift, ITEM_CHANGE_REASON::ICR_BUY_TOTAL_CHONG_ZHI))
        return 2;

    Player::updateRecord(m_pPlayer, 1028, nNewRecord);
    GongGao(pCfgInfo->nBroadcast);
    SendTotalChongZhiIcon();
    SendTotalChongZhiInfo();
    return 0;
}

int8_t CYunYingHD::GetShouChongIconState()
{
    if (!m_pPlayer)
        return 0;

    CfgData *pCfg = CfgData::instance();
    CfgShouChongTable *pTable = CfgData::GetShouChongLiBao(pCfg);
    int32_t nRecord = Player::getRecord(m_pPlayer, 1026);
    int32_t nTotalCount = CfgShouChongTable::GetCount(pTable);

    for (int8_t first = 1; first <= nTotalCount; ++first)
    {
        if (GetShouChongState(first) != 1)
        {
            if (GetShouChongState(first) > 1)
                continue;
            return 0;
        }
        return 2;
    }
    return 0;
}

int8_t CYunYingHD::GetShouChongState(int8_t nIndex)
{
    if (!m_pPlayer)
        return 0;

    CfgData *pCfg = CfgData::instance();
    CfgShouChongTable *pTable = CfgData::GetShouChongLiBao(pCfg);
    const CfgShouChong *pCfgInfo = CfgShouChongTable::GetShouChong(pTable, nIndex);
    if (!pCfgInfo)
        return 0;

    int32_t nRecord = Player::getRecord(m_pPlayer, 1026);
    if ((nRecord >> (nIndex - 1)) & 1)
        return 1;

    if (Player::GetTotalPayGold(m_pPlayer) >= pCfgInfo->nGold)
        return 2;

    return 0;
}

void CYunYingHD::SendShouChongInfo()
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x1E2);
    if (!packet)
        return;

    CfgData *pCfg = CfgData::instance();
    CfgShouChongTable *pTable = CfgData::GetShouChongLiBao(pCfg);
    int32_t nTotalCount = CfgShouChongTable::GetCount(pTable);

    packet->writeInt8((int8_t)nTotalCount);
    for (int8_t first = 1; first <= nTotalCount; ++first)
    {
        const CfgShouChong *pCfgInfo = CfgShouChongTable::GetShouChong(pTable, first);
        packet->writeInt32(pCfgInfo->nGold);
        int8_t ShouChongState = GetShouChongState(first);
        packet->writeInt8(ShouChongState);
    }
    packet->writeInt32(Player::GetTotalPayGold(m_pPlayer));
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void CYunYingHD::SendShouChongIcon()
{
    if (!m_pPlayer)
        return;

    ShowIcon stu;
    GetShouChongIconStu(&stu);
    Player::SendIconState(m_pPlayer, &stu);
}

void CYunYingHD::GetShouChongIconState(IconStateList *IconList)
{
    if (!m_pPlayer)
        return;

    if (CFunctionOpen::IsOpened(Player::GetFunctionOpen(m_pPlayer), 62) && !GetShouChongIconState())
    {
        ShowIcon icon;
        GetShouChongIconStu(&icon);
        IconList->push_back(icon);
    }
}

void CYunYingHD::GetShouChongIconStu(ShowIcon *retstr)
{
    memset(retstr, 0, sizeof(*retstr));
    retstr->nId = 34;
    if (GetShouChongIconState())
        retstr->nState = 2;
    else
        retstr->nState = 1;
    retstr->nLeftTime = -1;
}

void CYunYingHD::SendTeHuiInfo()
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x1E4);
    if (!packet)
        return;

    CfgData *pCfg = CfgData::instance();
    int32_t nRecord = Player::getRecord(m_pPlayer, 1027);

    packet->writeInt32(m_nLastTeHui);
    packet->writeInt32(Player::getCreateTime(m_pPlayer));

    for (int32_t i = 0; i < 3; ++i)
    {
        const NewServerFavorable *pFavorable = CfgData::GetNewServerFavorable(pCfg, i + 1);
        if (pFavorable)
        {
            packet->writeInt32(pFavorable->NeedGold);
            packet->writeInt8((nRecord >> i) & 1);
        }
        else
        {
            packet->writeInt32(0);
            packet->writeInt8(0);
        }
    }
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

int32_t CYunYingHD::getTeHuiLimitTime()
{
    if (!m_pPlayer)
        return 0;

    CfgData *pCfg = CfgData::instance();
    int32_t nMaxLimit = 0;
    for (int32_t i = 0; i < 3; ++i)
    {
        const NewServerFavorable *pFavorable = CfgData::GetNewServerFavorable(pCfg, i + 1);
        if (pFavorable && pFavorable->nLimitTime > nMaxLimit)
            nMaxLimit = pFavorable->nLimitTime;
    }
    return Player::getCreateTime(m_pPlayer) + nMaxLimit - Unit::getNow(m_pPlayer);
}

void CYunYingHD::checkTeHuiTime()
{
    if (!m_pPlayer)
        return;

    bool bSend = false;
    if (getTeHuiLimitTime() <= 0 && Player::getRecord(m_pPlayer, 1027) > 0)
    {
        m_nLastTeHui++;
        bSend = true;
    }
    if (bSend)
        SendTeHuiIcon();
}

void CYunYingHD::SendTeHuiIcon()
{
    if (!m_pPlayer)
        return;

    ShowIcon stu;
    GetTeHuiIconStu(&stu);
    Player::SendIconState(m_pPlayer, &stu);
}

void CYunYingHD::GetTeHuiIconState(IconStateList *IconList)
{
    if (!m_pPlayer || !CFunctionOpen::IsOpened(Player::GetFunctionOpen(m_pPlayer), 63))
        return;

    if (IsHaveTeHuiGift())
    {
        ShowIcon icon;
        GetTeHuiIconStu(&icon);
        IconList->push_back(icon);
    }
}

void CYunYingHD::GetTeHuiIconStu(ShowIcon *retstr)
{
    memset(retstr, 0, sizeof(*retstr));
    retstr->nId = 35;
    if (!IsHaveTeHuiGift() || getTeHuiLimitTime() <= 0)
    {
        retstr->nState = 4;
        retstr->nLeftTime = -1;
    }
    else
    {
        retstr->nState = 2;
        retstr->nLeftTime = getTeHuiLimitTime();
    }
}

bool CYunYingHD::IsHaveTeHuiGift()
{
    if (!m_pPlayer)
        return false;

    CfgData *pCfg = CfgData::instance();
    int32_t nRecord = Player::getRecord(m_pPlayer, 1027);
    for (int32_t i = 0; i < 3; ++i)
    {
        const NewServerFavorable *pFavorable = CfgData::GetNewServerFavorable(pCfg, i + 1);
        if (pFavorable)
        {
            if (!((nRecord >> i) & 1))
                return true;
        }
    }
    return false;
}

void CYunYingHD::SendEveryDayChongZhiInfo(int8_t nType)
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x1E6);
    if (!packet)
        return;

    CfgData *pCfg = CfgData::instance();
    int32_t nTodayPayGold = Player::GetTodayPayGold(m_pPlayer);
    packet->writeInt8(nType);
    packet->writeInt32(nTodayPayGold);

    for (int32_t i = 1; i <= 10; ++i)
    {
        const CfgEveryDayChongZhi *pCfgInfo = CfgData::GetEveryDayChongZhi(pCfg, nType, i);
        if (!pCfgInfo)
            break;

        int32_t nRecord = Player::getRecord(m_pPlayer, pCfgInfo->nRecord);
        packet->writeInt32(pCfgInfo->nGolds);
        if ((nRecord >> (i - 1)) & 1)
            packet->writeInt8(1);
        else
            packet->writeInt8(0);
    }
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void CYunYingHD::SendEveryDayChongZhiIcon(int8_t nType)
{
    if (!m_pPlayer)
        return;

    if (CanShowEveryChongZhiIcon())
    {
        ShowIcon stu;
        int32_t nIcon = getEveryDayChongZhiIcon(nType);
        memset(&stu, 0, sizeof(stu));
        stu.nId = nIcon;
        stu.nState = 2;
        stu.nLeftTime = -1;
        Player::SendIconState(m_pPlayer, &stu);
    }
}

bool CYunYingHD::CanShowEveryChongZhiIcon()
{
    return true;
}

void CYunYingHD::GetEveryDayChongZhiIconState(IconStateList *IconList)
{
    if (!m_pPlayer)
        return;

    for (int8_t nType = 0; nType < 4; ++nType)
    {
        if (CanShowEveryChongZhiIcon())
        {
            int32_t nCount = 0;
            if (HaveEveryDayChongZhiGiftCount(nType, &nCount))
            {
                ShowIcon stu;
                int32_t nIcon = getEveryDayChongZhiIcon(nType);
                memset(&stu, 0, sizeof(stu));
                stu.nId = nIcon;
                stu.nState = 2;
                stu.nIconRight = nCount;
                stu.nLeftTime = -1;
                IconList->push_back(stu);
            }
        }
    }
}

int32_t CYunYingHD::getEveryDayChongZhiIcon(int8_t nType)
{
    if (nType == 1 || nType == 2)
        return 46;
    if (nType == 3)
        return 47;
    return 46;
}

bool CYunYingHD::HaveEveryDayChongZhiGiftCount(int8_t nType, int32_t *Count)
{
    if (!m_pPlayer)
        return false;

    *Count = 0;
    CfgData *pCfg = CfgData::instance();
    int32_t nTodayPayGold = Player::GetTodayPayGold(m_pPlayer);

    for (int32_t i = 1; i <= 10; ++i)
    {
        const CfgEveryDayChongZhi *pCfgInfo = CfgData::GetEveryDayChongZhi(pCfg, nType, i);
        if (!pCfgInfo)
            break;

        int32_t nRecord = Player::getRecord(m_pPlayer, pCfgInfo->nRecord);
        if (!((nRecord >> (i - 1)) & 1) && nTodayPayGold >= pCfgInfo->nGolds)
            ++(*Count);
    }
    return *Count > 0;
}

void CYunYingHD::GongGao(int32_t GongGaoId)
{
    if (!m_pPlayer || GongGaoId <= 0)
        return;

    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, Player::getConnId(m_pPlayer), Answer::PackType::PACK_DISPATCH, 0x2CB3);
    if (!packet)
        return;

    packet->writeInt16(Player::getGateIndex(m_pPlayer));
    packet->writeInt64(Player::getCid(m_pPlayer));
    std::string val;
    Player::getName(m_pPlayer, &val);
    packet->writeUTF8(&val);
    packet->writeInt32(GongGaoId);
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, Player::getConnId(m_pPlayer), packet);
}

int32_t CYunYingHD::HaveTotalChongZhiRewardCount()
{
    if (!m_pPlayer)
        return 0;

    CfgData *pCfg = CfgData::instance();
    int32_t nTotalChongZhi = Player::getRecord(m_pPlayer, 1037);

    int32_t nCount = 0;
    for (int32_t i = 1; ; ++i)
    {
        const CfgTotalChongZhi *pCfgInfo = CfgData::GetTotalChongZhiCfg(pCfg, i);
        if (!pCfgInfo)
            break;

        int32_t nRecord = Player::getRecord(m_pPlayer, 1028);
        if (!((nRecord >> (i - 1)) & 1) && nTotalChongZhi >= pCfgInfo->nGolds)
            ++nCount;
    }
    return nCount;
}

void CYunYingHD::SendTotalChongZhiIcon()
{
    if (!m_pPlayer)
        return;

    ShowIcon stu;
    GetTotalChongZhiIconStu(&stu);
    Player::SendIconState(m_pPlayer, &stu);
}

void CYunYingHD::SendTotalChongZhiInfo()
{
    if (!m_pPlayer)
        return;

    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService *pGame = GameService::instance();
    Answer::NetPacket *packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x1E9);
    if (!packet)
        return;

    CfgData *pCfg = CfgData::instance();
    int32_t nTotalChongZhi = Player::getRecord(m_pPlayer, 1037);
    packet->writeInt32(nTotalChongZhi);

    for (int32_t i = 1; ; ++i)
    {
        const CfgTotalChongZhi *pCfgInfo = CfgData::GetTotalChongZhiCfg(pCfg, i);
        if (!pCfgInfo)
            break;

        int32_t nRecord = Player::getRecord(m_pPlayer, 1028);
        packet->writeInt32(pCfgInfo->nGolds);
        if ((nRecord >> (i - 1)) & 1)
            packet->writeInt8(1);
        else
            packet->writeInt8(0);
    }
    packet->setSize(packet->getWOffset());
    GameService::sendPacket(pGame, ConnId, packet);
}

void CYunYingHD::GetTotalChongZhiIconState(IconStateList *IconList)
{
    if (!m_pPlayer)
        return;

    if (!AllGetTotalChongZhiReward() && HaveTotalChongZhiRewardCount() > 0)
    {
        ShowIcon stu;
        GetTotalChongZhiIconStu(&stu);
        IconList->push_back(stu);
    }
}

bool CYunYingHD::AllGetTotalChongZhiReward()
{
    if (!m_pPlayer)
        return true;

    CfgData *pCfg = CfgData::instance();
    for (int32_t i = 1; ; ++i)
    {
        const CfgTotalChongZhi *pCfgInfo = CfgData::GetTotalChongZhiCfg(pCfg, i);
        if (!pCfgInfo)
            break;
        if (!pCfgInfo)
            break;

        int32_t nRecord = Player::getRecord(m_pPlayer, 1028);
        if (!((nRecord >> (i - 1)) & 1))
            return false;
    }
    return true;
}

void CYunYingHD::GetTotalChongZhiIconStu(ShowIcon *retstr)
{
    memset(retstr, 0, sizeof(*retstr));
    retstr->nId = 102;
    if (HaveTotalChongZhiRewardCount() > 0)
        retstr->nState = 2;
    else
        retstr->nState = 1;
    retstr->nLeftTime = -1;
}

void CYunYingHD::AddTotalChongZhiCount(int32_t AddCount)
{
    if (!m_pPlayer)
        return;

    int32_t nTotalChongZhi = Player::getRecord(m_pPlayer, 1037);
    Player::updateRecord(m_pPlayer, 1037, nTotalChongZhi + AddCount);
}

int32_t CYunYingHD::OnGetMobilePhoneGift(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    return 0;
}

int32_t CYunYingHD::OnDBGetMobilePhoneGift(Answer::NetPacket *inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    return 0;
}

void CYunYingHD::GetMobilePhoneGiftIconState(IconStateList *IconList)
{
}

void CYunYingHD::SendMobilePhoneGiftIcon()
{
}

void CYunYingHD::GetAdultGiftIconState(IconStateList *IconList)
{
}

void CYunYingHD::SendAdultGiftIcon()
{
}

void CYunYingHD::GetAdultGiftIconStu(ShowIcon *retstr, int32_t nIcon)
{
    memset(retstr, 0, sizeof(*retstr));
}

void CYunYingHD::SuperMemberRecharge(int32_t nGold)
{
}

void CYunYingHD::GetSuperMemberIconState(IconStateList *IconList)
{
}

int32_t CYunYingHD::OnGetZeroBuyPetGift(Answer::NetPacket *inPacket)
{
    return 0;
}

void CYunYingHD::OnZeroBuyPetOpen()
{
}

void CYunYingHD::SendZeroBuyPetIcon()
{
}

void CYunYingHD::GetZeroBuyPetIconState(IconStateList *IconList)
{
}

void CYunYingHD::GetZeroBuyPetIconStu(ShowIcon *retstr)
{
    memset(retstr, 0, sizeof(*retstr));
}
