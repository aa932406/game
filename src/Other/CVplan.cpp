#include "common.h"
﻿#include "Other/CVplan.h"

CVplan::CVplan()
    : CExtSystemBase()
{
}

CVplan::~CVplan()
{
}

void CVplan::OnCleanUp()
{
}

void CVplan::OnUpdate(int64_t curTick)
{
}

void CVplan::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CVplan::OnSaveToDB(PlayerDBData* dbData)
{
}

void CVplan::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(290);
    procList->push_back(291);
    procList->push_back(292);
    procList->push_back(293);
    procList->push_back(294);
}

int32_t CVplan::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    switch (nProcId)
    {
        case 290: return OnGetSWVipReward(inPacket);
        case 291: return OnGetSWVipBarReward(inPacket);
        case 292: return OnGetYYLevelReward(inPacket);
        case 293: return OnGetYYVipGift(inPacket);
        case 294: return OnLoginFromYYgameApp(inPacket);
    }
    return 10002;
}

void CVplan::OnDaySwitch(int32_t nDiffDays)
{
    SendVplanInfo();
    SendVplanIcon();
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "duowanclouds")
        {
            CfgData* pCfg = Answer::Singleton<CfgData>::instance();
            const CfgYYVipMap* YYVipMap = CfgData::GetCfgYYVipMap(pCfg);
            for (auto& kv : *YYVipMap)
            {
                if (kv.second.nType == 4)
                {
                    int32_t opId = kv.first + 1500;
                    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(m_pPlayer);
                    CExtOperateLimit::Reset(OperateLimit, opId);
                }
            }
            SendYYVipIcon();
        }
        else if (platform == "taojike")
        {
            CfgData* pCfg = Answer::Singleton<CfgData>::instance();
            const LuDaShiVipMap* LuDaShi = CfgData::GetLaDaShiHuiYuanMap(pCfg);
            for (auto& kv : *LuDaShi)
            {
                if (kv.second.nType == 6)
                {
                    int32_t opId = kv.first + 1551;
                    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(m_pPlayer);
                    CExtOperateLimit::Reset(OperateLimit, opId);
                }
            }
            SendLuDaShiIcon();
        }
        else if (platform == "swjoy")
        {
            CheckSwBarLoginCount();
            resetSwVipDailyReward();
            resetSwBarDailyReward();
            sendSwVipIcon();
            sendSwVipBarIcon();
        }
        if (platform == "niuxyx")
        {
            int32_t NewRecord = 0;
            int32_t Record = Player::getRecord(m_pPlayer, 1156);
            CfgData* pCfg = Answer::Singleton<CfgData>::instance();
            const XunLeiCfgMap* XunLeiTable = CfgData::GetXunLeiTable(pCfg);
            for (auto& kv : *XunLeiTable)
            {
                if (kv.second.nType != 2)
                {
                    if ((Record >> (kv.first - 1)) & 1)
                        NewRecord |= 1 << (kv.first - 1);
                }
            }
            Player::updateRecord(m_pPlayer, 1156, NewRecord);
            SendXunLeiIcon();
        }
    }
}

void CVplan::OnWeekSwitch(int32_t nDiffWeeks)
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "duowanclouds")
        {
            CfgData* pCfg = Answer::Singleton<CfgData>::instance();
            const CfgYYVipMap* YYVipMap = CfgData::GetCfgYYVipMap(pCfg);
            for (auto& kv : *YYVipMap)
            {
                if (kv.second.nType == 5)
                {
                    int32_t opId = kv.first + 1500;
                    CExtOperateLimit* OperateLimit = Player::GetOperateLimit(m_pPlayer);
                    CExtOperateLimit::Reset(OperateLimit, opId);
                }
            }
        }
    }
}

int32_t CVplan::OnLoginFromYYgameApp(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CVplan::AddAttr()
{
}

int32_t CVplan::GetExpRatio()
{
    return 0;
}

int32_t CVplan::OnGetReward(Answer::NetPacket* inPacket)
{
    return 10002;
}

bool CVplan::HaveEveryDayGift()
{
    return false;
}

bool CVplan::HaveLevelGift()
{
    return false;
}

bool CVplan::HaveVplanGift()
{
    return false;
}

void CVplan::SendVplanInfo()
{
}

void CVplan::SendVplanIcon()
{
}

void CVplan::GetVplanIconState(IconStateList* IconList)
{
}

void CVplan::GetVplanIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    retstr->nState = 4;
    retstr->nLeftTime = -1;
    retstr->nEffects = 1;
}

int32_t CVplan::OnGetYYLevelReward(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CVplan::HaveYYLevelReward()
{
    return 0;
}

void CVplan::SendYYInfo()
{
}

int32_t CVplan::OnGetYYVipGift(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CVplan::SendYYVipInfo()
{
}

void CVplan::GetYYVipIconState(IconStateList* IconList)
{
}

void CVplan::SendYYVipIcon()
{
    if (m_pPlayer)
    {
        ShowIcon stu;
        GetYYVipIconStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}

void CVplan::GetYYVipIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const CfgYYVipMap* YYVipMap = CfgData::GetCfgYYVipMap(pCfg);
        for (auto& kv : *YYVipMap)
        {
            if (kv.second.nType == NULL)
            {
                retstr->nId = kv.second.nIconId;
                int32_t opId = kv.first + 1500;
                CExtOperateLimit* pLimit = Player::GetOperateLimit(m_pPlayer);
                if (CExtOperateLimit::CanDo(pLimit, opId))
                    retstr->nState = 2;
                else
                    retstr->nState = 4;
                break;
            }
        }
        retstr->nLeftTime = -1;
        retstr->nEffects = 1;
    }
}

int32_t CVplan::HaveYYVipLeftGift()
{
    return 0;
}

void CVplan::AppendYYInfo(Answer::NetPacket* packet)
{
}

int32_t CVplan::OnGetSWVipReward(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CVplan::OnGetSWVipBarReward(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CVplan::SendSwVipReward()
{
}

void CVplan::AppendSwVipInfo(Answer::NetPacket* packet)
{
}

void CVplan::CheckSwBarLoginCount()
{
}

void CVplan::GetSwVipIconState(IconStateList* IconList)
{
    if (m_pPlayer)
    {
        if (isGetAllSwVipReward())
            return;
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "swjoy")
        {
            ShowIcon __x;
            getSwVipIconStu(&__x);
            IconList->push_back(__x);
        }
    }
}

void CVplan::GetSwVipBarIconState(IconStateList* IconList)
{
    if (m_pPlayer)
    {
        if (isGetAllSwVipBarReward())
            return;
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "swjoy")
        {
            ShowIcon __x;
            getSwVipBarIconStu(&__x);
            IconList->push_back(__x);
        }
    }
}

void CVplan::sendSwVipIcon()
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "swjoy" && !isGetAllSwVipReward())
        {
            ShowIcon stu;
            getSwVipIconStu(&stu);
            Player::SendIconState(m_pPlayer, &stu);
        }
    }
}

void CVplan::getSwVipIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        retstr->nId = CfgData::GetSwVipCfg(pCfg, 1)->nIconId;
        retstr->nState = 2;
        retstr->nLeftTime = -1;
        retstr->nEffects = 1;
    }
}

int32_t CVplan::getSwVipRewardCount()
{
    return 0;
}

bool CVplan::isGetAllSwVipReward()
{
    return true;
}

bool CVplan::checkSwVipRewardCondition(int8_t nType, int32_t nCondition)
{
    return false;
}

void CVplan::resetSwVipDailyReward()
{
    if (m_pPlayer)
    {
        Player::updateRecord(m_pPlayer, 2019, 0);
        Player::updateRecord(m_pPlayer, 2020, 0);
    }
}

void CVplan::sendSwVipBarIcon()
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "swjoy" && !isGetAllSwVipBarReward())
        {
            ShowIcon stu;
            getSwVipBarIconStu(&stu);
            Player::SendIconState(m_pPlayer, &stu);
        }
    }
}

void CVplan::getSwVipBarIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        retstr->nId = CfgData::GetSwVipBarCfg(pCfg, 1)->nIconId;
        if (Player::getRecord(m_pPlayer, 1055))
            retstr->nState = 4;
        else
            retstr->nState = 2;
        retstr->nLeftTime = -1;
        retstr->nEffects = 1;
    }
}

int32_t CVplan::getSwVipBarRewardCount()
{
    return 0;
}

bool CVplan::isGetAllSwVipBarReward()
{
    return false;
}

bool CVplan::checkSwBarRewardCondition(int8_t nType, int32_t nCondition)
{
    return false;
}

void CVplan::resetSwBarDailyReward()
{
    if (m_pPlayer)
    {
        Player::updateRecord(m_pPlayer, 1055, 0);
    }
}

int32_t CVplan::OnGetSgGameApp(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CVplan::OnGetXunLeiReward(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CVplan::SendXunLieInfo()
{
}

void CVplan::SendSgGameAppInfo()
{
}

int32_t CVplan::HaveSgGameAppReward()
{
    return 0;
}

bool CVplan::HaveSgDownLoadGift()
{
    return false;
}

void CVplan::getSgDownLoadIconStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
}

void CVplan::SendSgDownLoadIcon()
{
}

void CVplan::GetSgDownLoadIocnState(IconStateList* IconList)
{
}

void CVplan::GetXunLeiIconState(IconStateList* IconList)
{
    if (m_pPlayer)
    {
        ShowIcon __x;
        GetXunLeiDownlondIcon(&__x);
        if (__x.nState == 2)
            IconList->push_back(__x);
        GetXunLeiDaTingIcon(&__x);
        if (__x.nState == 2)
            IconList->push_back(__x);
        GetXunLeiVipIcon(&__x);
        if (__x.nState == 2)
            IconList->push_back(__x);
    }
}

void CVplan::SendXunLeiIcon()
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "niuxyx")
        {
            ShowIcon stu;
            GetXunLeiDownlondIcon(&stu);
            if (stu.nState == 2)
                Player::SendIconState(m_pPlayer, &stu);
            GetXunLeiDaTingIcon(&stu);
            if (stu.nState == 2)
                Player::SendIconState(m_pPlayer, &stu);
            GetXunLeiVipIcon(&stu);
            if (stu.nState == 2)
                Player::SendIconState(m_pPlayer, &stu);
        }
    }
}

void CVplan::GetXunLeiDownlondIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const XunLeiCfg* pXunLeiCfg = CfgData::GetXunLeiCfg(pCfg, 1);
        if (pXunLeiCfg)
        {
            retstr->nId = pXunLeiCfg->nIconId;
            int32_t Record = Player::getRecord(m_pPlayer, 1156);
            if ((Record & 1) == 0)
                retstr->nState = 2;
            else
                retstr->nState = 4;
            retstr->nLeftTime = -1;
            retstr->nEffects = 1;
        }
    }
}

void CVplan::GetXunLeiDaTingIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const XunLeiCfg* pXunLeiCfg = CfgData::GetXunLeiCfg(pCfg, 2);
        if (pXunLeiCfg)
        {
            retstr->nId = pXunLeiCfg->nIconId;
            int32_t Record = Player::getRecord(m_pPlayer, 1156);
            if ((Record & 2) == 0)
                retstr->nState = 2;
            else
                retstr->nState = 4;
            retstr->nLeftTime = -1;
            retstr->nEffects = 1;
        }
    }
}

void CVplan::GetXunLeiVipIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const XunLeiCfg* pXunLeiCfg = CfgData::GetXunLeiCfg(pCfg, 3);
        if (pXunLeiCfg)
        {
            retstr->nId = pXunLeiCfg->nIconId;
            int32_t Record = Player::getRecord(m_pPlayer, 1156);
            if ((Record & 4) == 0)
                retstr->nState = 2;
            else
                retstr->nState = 4;
            retstr->nLeftTime = -1;
            retstr->nEffects = 1;
            retstr->nIconRight = HaveXunLeiVipReward();
        }
    }
}

bool CVplan::IsGetAllXunLeiReward(int32_t Type)
{
    return true;
}

int32_t CVplan::HaveXunLeiVipReward()
{
    return 0;
}

int32_t CVplan::OnGetLuDaShiVipGift(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CVplan::SendLuDaShiVipInfo()
{
}

int32_t CVplan::HaveLuDaShiVipLeftGift()
{
    return 0;
}

void CVplan::GetLuDaShiStu(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (m_pPlayer)
    {
        CfgData* pCfg = Answer::Singleton<CfgData>::instance();
        const LuDaShiVipMap* LuDaShi = CfgData::GetLaDaShiHuiYuanMap(pCfg);
        for (auto& kv : *LuDaShi)
        {
            if (kv.second.nType == 6)
            {
                retstr->nId = kv.second.nIconId;
                int32_t opId = kv.first + 1551;
                CExtOperateLimit* pLimit = Player::GetOperateLimit(m_pPlayer);
                if (CExtOperateLimit::CanDo(pLimit, opId))
                    retstr->nState = 2;
                else
                    retstr->nState = 4;
                break;
            }
        }
        retstr->nLeftTime = -1;
        retstr->nEffects = 1;
    }
}

void CVplan::GetLuDaShiIconState(IconStateList* IconList)
{
    if (m_pPlayer)
    {
        std::string platform;
        Player::GetPlatform(m_pPlayer, &platform);
        if (platform == "taojike" && HaveLuDaShiVipLeftGift())
        {
            ShowIcon __x;
            GetLuDaShiStu(&__x);
            IconList->push_back(__x);
        }
    }
}

void CVplan::SendLuDaShiIcon()
{
    if (m_pPlayer)
    {
        ShowIcon stu;
        GetLuDaShiStu(&stu);
        Player::SendIconState(m_pPlayer, &stu);
    }
}
