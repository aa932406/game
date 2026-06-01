#include "Other/CWan360.h"

CWan360::CWan360()
{
    OnCleanUp();
}

CWan360::~CWan360()
{
}

void CWan360::OnCleanUp()
{
    IsGameAppLogin = 0;
    IsSpeed360Start = 0;
    m_bSouGouSkinLogin = 0;
    LastUpdateTiem = 0;
}

void CWan360::OnUpdate(int64_t curTick)
{
    if (m_pPlayer && curTick - LastUpdateTiem > 999)
    {
        LastUpdateTiem = curTick;
        int32_t Now = Unit::getNow(m_pPlayer);
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        Speed360Reward* pReward = CfgData::Get360Reward(pCfg);
        Speed360Reward stu(*pReward);
        if (stu.StartTime > Now || stu.EndTime < Now)
        {
            if (IsSpeed360Start)
            {
                IsSpeed360Start = 0;
                SendSpeed360Icon();
            }
        }
        else if (!IsSpeed360Start)
        {
            IsSpeed360Start = 1;
            SendSpeed360Icon();
        }
    }
}

void CWan360::OnDaySwitch(int32_t nDiffDays)
{
    SendSpeed360Icon();
    SendGameAppIcon();
}

void CWan360::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CWan360::OnSaveToDB(PlayerDBData* dbData)
{
}

void CWan360::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(530);
    procList->push_back(531);
    procList->push_back(532);
    procList->push_back(533);
    procList->push_back(534);
}

int32_t CWan360::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    switch (nProcId)
    {
        case 0x212: return OnGetJiaSuQiuReward(inPacket);
        case 0x213: return OnGetGameAppReward(inPacket);
        case 0x214: return OnLoginFromGameApp(inPacket);
        case 0x215: return OnGetSpeed360Reward(inPacket);
        case 0x216: return OnLoginFromSouGouSkin(inPacket);
    }
    return 0;
}

int32_t CWan360::OnGetJiaSuQiuReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    if (platform != "w360")
        return 10002;
    if (Player::getRecord(m_pPlayer, 1050) > 0)
        return 10002;
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    Wan360Reward* pReward = CfgData::Get360RewardCfg(pCfg, 100);
    if (!pReward)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pReward->Items, ITEM_CHANGE_REASON::ICR_360_JIA_SU_QIU))
        return 10002;
    Player::updateRecord(m_pPlayer, 1050, 1);
    SendJiaShuQiuIcon();
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(pGame, ConnId, GateIndex, Proc, 0);
    return 0;
}

int32_t CWan360::OnGetGameAppReward(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 10002;
    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::Get360RewardIcon(pCfg, &platform) == 0)
        return 10002;
    if (!IsGameAppLogin)
        return 10002;
    int32_t GetRewardTimes = Player::getRecord(m_pPlayer, 1051);
    if (GetRewardTimes > 6)
        return 10002;
    if (Player::getRecord(m_pPlayer, 2028) > 0)
        return 10002;
    int32_t v5 = GetRewardTimes + 1;
    Wan360Reward* pReward = CfgData::Get360RewardCfg(pCfg, v5);
    if (!pReward)
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pReward->Items, ITEM_CHANGE_REASON::ICR_360_GAME_APP))
        return 10002;
    Player::updateRecord(m_pPlayer, 2028, 1);
    Player::updateRecord(m_pPlayer, 1051, GetRewardTimes + 1);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(pGame, ConnId, GateIndex, Proc, 0);
    SendGameAppIcon();
    return 0;
}

int32_t CWan360::OnLoginFromGameApp(Answer::NetPacket* inPacket)
{
    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::Get360RewardIcon(pCfg, &platform) == 0)
        return 10002;
    IsGameAppLogin = 1;
    return 0;
}

int32_t CWan360::OnLoginFromSouGouSkin(Answer::NetPacket* inPacket)
{
    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    const CfgSouGouSkin* pSkin = CfgData::GetSouGouSkin(pCfg, &platform);
    if (!pSkin)
        return 10002;
    if (Player::getRecord(m_pPlayer, 1118))
        return 10002;
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    if (!CExtCharBag::AddItem(Bag, &pSkin->vReward, ITEM_CHANGE_REASON::ICR_SOU_GOU_SKIN_LOGIN))
        return 10002;
    Player::updateRecord(m_pPlayer, 1118, 1);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(pGame, ConnId, GateIndex, Proc, 0);
    SendSouGouSkinIcon();
    return 0;
}

void CWan360::SendSouGouSkinIcon()
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const CfgSouGouSkin* pSkin = CfgData::GetSouGouSkin(pCfg, &platform);
        if (pSkin)
        {
            ShowIcon stu;
            GetSouGouSkinIconStu(&stu, pSkin->nIcon);
            Player::SendIconState(m_pPlayer, &stu);
        }
    }
}

void CWan360::GetSouGouSkinIconState(IconStateList* IconList)
{
    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    const CfgSouGouSkin* pSkin = CfgData::GetSouGouSkin(pCfg, &platform);
    if (pSkin && m_pPlayer && !Player::getRecord(m_pPlayer, 1118))
    {
        ShowIcon __x;
        GetSouGouSkinIconStu(&__x, pSkin->nIcon);
        IconList->push_back(__x);
    }
}

void CWan360::GetSouGouSkinIconStu(ShowIcon* retstr, int32_t nIcon)
{
    memset(retstr, 0, sizeof(ShowIcon));
    retstr->nId = nIcon;
    if (Player::getRecord(m_pPlayer, 1118))
        retstr->nState = 4;
    else
        retstr->nState = 2;
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
}

void CWan360::SendGameAppIcon()
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        if (CfgData::Get360RewardIcon(pCfg, &platform) != 0)
        {
            ShowIcon stu;
            GetGameAppIconStu(&stu);
            Player::SendIconState(m_pPlayer, &stu);
        }
    }
}

void CWan360::GetGameAppIconState(IconStateList* IconList)
{
    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::Get360RewardIcon(pCfg, &platform) != 0 && m_pPlayer && Player::getRecord(m_pPlayer, 2028) <= 6)
    {
        ShowIcon __x;
        GetGameAppIconStu(&__x);
        IconList->push_back(__x);
    }
}

void CWan360::GetGameAppIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        int32_t iconId = CfgData::Get360RewardIcon(pCfg, &platform);
        if (iconId)
        {
            retstr->nId = iconId;
            if (platform == "w360")
            {
                if (Player::getRecord(m_pPlayer, 1051) > 6)
                    retstr->nState = 4;
                else
                    retstr->nState = 2;
            }
            else if (platform == "duowanclouds")
            {
                retstr->nState = 2;
                CVplan* Vplan = Player::GetVplan(m_pPlayer);
                retstr->nIconRight = CVplan::HaveYYLevelReward(Vplan);
            }
            else if (platform == "sogou")
            {
                retstr->nState = 2;
                CVplan* Vplan = Player::GetVplan(m_pPlayer);
                retstr->nIconRight = CVplan::HaveSgGameAppReward(Vplan);
            }
            else
            {
                retstr->nState = 2;
            }
            retstr->nLeftTime = -1;
            retstr->nEffects = 1;
        }
    }
}

void CWan360::SendJiaShuQiuIcon()
{
    bool cleanup = false;
    bool skip = true;
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        cleanup = true;
        if (platform != "w360")
            skip = false;
    }
    if (!skip)
    {
        ShowIcon stu;
        GetJiaShuQiuIconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}

void CWan360::GetJiaShuQiuIconState(IconStateList* IconList)
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        bool notW360 = (platform != "w360");
        if (!notW360 && Player::getRecord(m_pPlayer, 1050) <= 0)
        {
            ShowIcon __x;
            GetJiaShuQiuIconStu(&__x);
            IconList->push_back(__x);
        }
    }
}

void CWan360::GetJiaShuQiuIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    retstr->nId = 40;
    if (Player::getRecord(m_pPlayer, 1050))
        retstr->nState = 4;
    else
        retstr->nState = 2;
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
}

void CWan360::SendWeiXinIcon()
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const CfgWeiXinTable* WeiXinTable = CfgData::GetWeiXinTable(pCfg);
        if (CfgWeiXinTable::GetWeiXinGift(WeiXinTable, &platform))
        {
            if (platform != "duowanclouds")
            {
                ShowIcon stu;
                GetWeiXinIconStu(&stu);
                Player::SendIconState(m_pPlayer, &stu);
            }
            else
            {
                CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
                if (CFunctionOpen::IsOpened(PlayerFunctionOpen, 211))
                {
                    ShowIcon stu;
                    GetWeiXinIconStu(&stu);
                    Player::SendIconState(m_pPlayer, &stu);
                }
            }
        }
    }
}

void CWan360::GetWeiXinIconState(IconStateList* IconList)
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const CfgWeiXinTable* WeiXinTable = CfgData::GetWeiXinTable(pCfg);
        if (CfgWeiXinTable::GetWeiXinGift(WeiXinTable, &platform))
        {
            bool canShow = true;
            if (platform == "duowanclouds")
            {
                CFunctionOpen* PlayerFunctionOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
                if (!CFunctionOpen::IsOpened(PlayerFunctionOpen, 211))
                    canShow = false;
            }
            if (canShow && Player::getRecord(m_pPlayer, 1052) <= 0)
            {
                ShowIcon __x;
                GetWeiXinIconStu(&__x);
                IconList->push_back(__x);
            }
        }
    }
}

void CWan360::GetWeiXinIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    std::string platform;
    Player::GetPlatform(m_pPlayer, &platform);
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    const CfgWeiXinTable* WeiXinTable = CfgData::GetWeiXinTable(pCfg);
    const CfgWeiXingGift* pCfgWeiXin = CfgWeiXinTable::GetWeiXinGift(WeiXinTable, &platform);
    if (pCfgWeiXin)
    {
        retstr->nId = pCfgWeiXin->nIconId;
        if (Player::getRecord(m_pPlayer, 1052))
            retstr->nState = 4;
        else
            retstr->nState = 2;
        retstr->nLeftTime = -1;
        retstr->nEffects = 1;
    }
}

int32_t CWan360::OnGetSpeed360Reward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 2;
    if (!InSpeed360Time())
        return 2;
    if (Player::getRecord(m_pPlayer, 2048) > 0)
        return 2;
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    Speed360Reward* pReward = CfgData::Get360Reward(pCfg);
    Speed360Reward stu(*pReward);
    CExtCharBag* Bag = Player::GetBag(m_pPlayer);
    int32_t FreeSlotCount = CExtCharBag::GetFreeSlotCount(Bag);
    int32_t result;
    if (FreeSlotCount >= (int)stu.Rewards.size())
    {
        Bag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::AddItem(Bag, &stu.Rewards, ITEM_CHANGE_REASON::ICR_SPEED_360_REWARD))
        {
            result = 2;
        }
        else
        {
            Player::updateRecord(m_pPlayer, 2048, 1);
            SendSpeed360Icon();
            result = 0;
        }
    }
    else
    {
        result = 2;
    }
    return result;
}

void CWan360::SendSpeed360Icon()
{
    bool cleanup = false;
    bool skip = true;
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        cleanup = true;
        if (platform != "w360")
            skip = false;
    }
    if (!skip)
    {
        ShowIcon stu;
        GetSpeed360IconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}

void CWan360::GetSpeed360State(IconStateList* IconList)
{
    if (InSpeed360Time())
    {
        ShowIcon __x;
        GetSpeed360IconStu(&__x);
        IconList->push_back(__x);
    }
}

void CWan360::GetSpeed360IconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    retstr->nId = 60;
    if (!Player::getRecord(m_pPlayer, 2048) && InSpeed360Time())
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
    if (Player::getRecord(m_pPlayer, 2048) <= 0)
        retstr->nIconRight = 1;
}

bool CWan360::InSpeed360Time()
{
    if (!m_pPlayer)
        return false;
    int32_t Now = Unit::getNow(m_pPlayer);
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    Speed360Reward* pReward = CfgData::Get360Reward(pCfg);
    Speed360Reward stu(*pReward);
    return stu.StartTime <= Now && stu.EndTime >= Now;
}
