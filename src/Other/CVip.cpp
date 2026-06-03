#include "common.h"
﻿#include "Other/CVip.h"

CVip::CVip()
    : CExtSystemBase()
    , m_PlatinumVipEndTime(0)
    , m_DiamVipEndTime(0)
    , m_StarVipEndTime(0)
    , m_Club(0)
    , m_ClubDropTimes(0)
    , m_BuyTimes(0)
{
}

CVip::~CVip()
{
}

void CVip::OnCleanUp()
{
    m_PlatinumVipEndTime = 0;
    m_DiamVipEndTime = 0;
    m_StarVipEndTime = 0;
    m_Club = 0;
    m_ClubDropTimes = 0;
    m_BuyTimes = 0;
    m_VipLuckyDropMap.clear();
}

void CVip::OnUpdate(int64_t curTick)
{
    CheckVipEnd(curTick);
}

void CVip::OnDaySwitch(int32_t nDiffDays)
{
}

void CVip::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CVip::OnSaveToDB(PlayerDBData* dbData)
{
}

void CVip::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(531);
    procList->push_back(532);
    procList->push_back(533);
    procList->push_back(534);
    procList->push_back(535);
    procList->push_back(536);
}

int32_t CVip::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    switch (nProcId)
    {
        case 531: return OnBuyVipCard(inPacket);
        case 532: return OnEnterClub(inPacket);
        case 533: return OnBuyDropTimes(inPacket);
        case 534: return OnBuyAllVipCard(inPacket);
        case 535: return OnGetVipCardGift(inPacket);
        case 536: return OnGetVipGift(inPacket);
    }
    return 10002;
}

bool CVip::AddVipLuckyDrop(int32_t Type, int32_t Times)
{
    return true;
}

int32_t CVip::OnBuyVipCard(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CVip::OnEnterClub(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CVip::OnBuyDropTimes(Answer::NetPacket* inPacket)
{
    return 10002;
}

int32_t CVip::OnBuyAllVipCard(Answer::NetPacket* inPacket)
{
    return 10002;
}

bool CVip::GetVipFlg(int8_t VipType)
{
    if (!m_pPlayer)
        return false;
    int32_t Now = Unit::getNow(m_pPlayer);
    switch (VipType)
    {
        case 1: return m_PlatinumVipEndTime > Now;
        case 2: return m_DiamVipEndTime > Now;
        case 3: return m_StarVipEndTime > Now;
    }
    return false;
}

int32_t CVip::GetVipEquipBackRate()
{
    return 0;
}

int32_t CVip::OnGetVipCardGift(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CVip::BuyGongGao(int32_t GongGaoId, int8_t VipType, int8_t VipLevel)
{
}

int32_t CVip::OnGetVipGift(Answer::NetPacket* inPacket)
{
    return 10002;
}

void CVip::AddVipExp(int32_t VipExp)
{
    if (m_pPlayer)
    {
        int32_t nExp = Player::getRecord(m_pPlayer, 1020);
        Player::updateRecord(m_pPlayer, 1020, nExp + VipExp);
    }
}

bool CVip::CanSiteRevive()
{
    return false;
}

void CVip::GetClubDrop(int32_t Mid)
{
}

void CVip::GetLuckDropType(int32_t Mid)
{
}

void CVip::AddDropTimes(int32_t Type, int32_t VipLevel)
{
    if (VipLevel > 0 && VipLevel <= 14)
    {
        if (Type == 1)
            m_VipLuckyDropMap[VipLevel].DropOrdinaryTimes++;
        else if (Type == 2)
            m_VipLuckyDropMap[VipLevel].DropSpecialTimes++;
    }
}

int32_t CVip::GetDropVipLevel(int32_t Type)
{
    for (int32_t i = 1; i <= 14; i++)
    {
        int8_t level = GetVipLevel();
        if (level < i)
            break;
        if (Type == 1)
        {
            if (m_VipLuckyDropMap[i].DropOrdinaryTimes < m_VipLuckyDropMap[i].CanDropOrdinaryTimes)
                return i;
        }
        else if (Type == 2)
        {
            if (m_VipLuckyDropMap[i].DropSpecialTimes < m_VipLuckyDropMap[i].CanDropSpecialTimes)
                return i;
        }
    }
    return 0;
}

int32_t CVip::GetTreasureTimes()
{
    int8_t VipLevel = GetVipLevel();
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    VipTable* pTable = CfgData::GetVipTable(pCfg);
    VipCfg* pVipCfg = VipTable::GetVipCfg(pTable, VipLevel);
    if (pVipCfg)
        return pVipCfg->TreasureTimes;
    return 0;
}

int32_t CVip::GetCycleTowerTimes()
{
    int8_t VipLevel = GetVipLevel();
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    VipTable* pTable = CfgData::GetVipTable(pCfg);
    VipCfg* pVipCfg = VipTable::GetVipCfg(pTable, VipLevel);
    if (pVipCfg)
        return pVipCfg->CycleTower;
    return 0;
}

void CVip::SendLuckDrop(int32_t nType, MemChrBag stu, int32_t Mid)
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CDA);
    if (packet)
    {
        Answer::NetPacket::writeInt32(packet, 0);
        Answer::NetPacket::writeInt8(packet, nType);
        Answer::NetPacket::writeInt32(packet, stu.itemId);
        Answer::NetPacket::writeInt8(packet, stu.itemClass);
        Answer::NetPacket::writeInt32(packet, stu.itemCount);
        uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
        Answer::NetPacket::setSize(packet, WOffset);
        int16_t GateIndex = Player::getGateIndex(m_pPlayer);
        GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
    }
    int32_t GongGaoId = 0;
    if (nType == 1)
        GongGaoId = 456;
    else if (nType == 2)
        GongGaoId = 457;
    if (GongGaoId > 0)
    {
        Answer::NetPacket* packet2 = GameService::popNetpacket(pGame, 0, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet2)
        {
            Answer::NetPacket::writeInt32(packet2, GongGaoId);
            CharId_t Cid = Player::getCid(m_pPlayer);
            Answer::NetPacket::writeInt64(packet2, Cid);
            std::string name;
            Player::getName(m_pPlayer, &name);
            Answer::NetPacket::writeUTF8(packet2, &name);
            int32_t MapId = StaticObj::getMapId(m_pPlayer);
            Answer::NetPacket::writeInt32(packet2, MapId);
            Answer::NetPacket::writeInt32(packet2, Mid);
            Answer::NetPacket::writeInt8(packet2, stu.itemClass);
            Answer::NetPacket::writeInt32(packet2, stu.itemId);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet2);
            Answer::NetPacket::setSize(packet2, WOffset);
            GameService::worldBroadcast(pGame, 0, packet2);
        }
    }
}

int8_t CVip::GetVipLevel()
{
    int32_t VipExp = GetVipExp();
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    VipTable* pTable = CfgData::GetVipTable(pCfg);
    return VipTable::GetVipLevel(pTable, VipExp);
}

int32_t CVip::GetVipExp()
{
    if (m_pPlayer)
        return Player::getRecord(m_pPlayer, 1020);
    return 0;
}

void CVip::AddVipTime(int8_t VipType, int32_t Seconds)
{
    if (m_pPlayer)
    {
        int32_t Now = Unit::getNow(m_pPlayer);
        switch (VipType)
        {
            case 1:
                if (m_PlatinumVipEndTime <= Now)
                    m_PlatinumVipEndTime = Seconds + Now;
                else
                    m_PlatinumVipEndTime = Seconds + m_PlatinumVipEndTime;
                break;
            case 2:
                if (m_DiamVipEndTime <= Now)
                    m_DiamVipEndTime = Seconds + Now;
                else
                    m_DiamVipEndTime = Seconds + m_DiamVipEndTime;
                break;
            case 3:
                if (m_StarVipEndTime <= Now)
                    m_StarVipEndTime = Seconds + Now;
                else
                    m_StarVipEndTime = Seconds + m_StarVipEndTime;
                break;
        }
    }
}

int8_t CVip::GetVipType()
{
    if (!m_pPlayer)
        return 0;
    int32_t Now = Unit::getNow(m_pPlayer);
    if (m_StarVipEndTime > Now)
        return 1;
    if (m_DiamVipEndTime > Now)
        return 2;
    if (m_StarVipEndTime > Now)
        return 3;
    return 0;
}

void CVip::AddVipAttr()
{
    for (int8_t i = 0; i <= 2; i++)
    {
        if (GetVipFlg(i + 1))
        {
            CfgData* pCfg = Answer::Singleton<CfgData>::instance();
            VipCardCfg* pVip = CfgData::GetVipCardCfg(pCfg, i + 1);
            if (pVip)
            {
                for (auto& attr : pVip->AddAttr)
                {
                    Unit::AddAttrValue(m_pPlayer, (CObjAttrs::Index_T)attr.m_nAddAttrType, attr.m_nAddAttrValue);
                }
            }
        }
    }
    int8_t VipLevel = GetVipLevel();
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    VipTable* pTable = CfgData::GetVipTable(pCfg);
    VipCfg* pVipCfg = VipTable::GetVipCfg(pTable, VipLevel);
    if (pVipCfg)
    {
        for (auto& addon : pVipCfg->AtttVector)
        {
            Unit::AddAttrValue(m_pPlayer, (CObjAttrs::Index_T)addon.index, addon.addon);
        }
    }
}

void CVip::SendVipInfo()
{
    if (m_pPlayer)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD9);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, m_PlatinumVipEndTime);
            Answer::NetPacket::writeInt32(packet, m_DiamVipEndTime);
            Answer::NetPacket::writeInt32(packet, m_StarVipEndTime);
            Answer::NetPacket::writeInt32(packet, GetVipExp());
            Answer::NetPacket::writeInt32(packet, Player::getRecord(m_pPlayer, 1021));
            Answer::NetPacket::writeInt32(packet, Player::getRecord(m_pPlayer, 2018));
            Answer::NetPacket::writeInt32(packet, Player::getRecord(m_pPlayer, 1911));
            Answer::NetPacket::writeInt32(packet, Player::getRecord(m_pPlayer, 1912));
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
        }
    }
}

void CVip::OnLoadVipEndTime(const std::string* VipEndTime)
{
    if (m_pPlayer && VipEndTime)
    {
        StringVector endTime;
        Answer::StringUtility::split(&endTime, VipEndTime, ":", 0);
        if (endTime.size() == 3)
        {
            m_PlatinumVipEndTime = atoi(endTime[0].c_str());
            m_DiamVipEndTime = atoi(endTime[1].c_str());
            m_StarVipEndTime = atoi(endTime[2].c_str());
        }
    }
}

std::string CVip::OnSaveVipEndTime()
{
    std::stringstream ss;
    ss << m_PlatinumVipEndTime << ":" << m_DiamVipEndTime << ":" << m_StarVipEndTime;
    return ss.str();
}

void CVip::CheckVipEnd(int64_t Tick)
{
    if (m_pPlayer)
    {
        int32_t Now = Unit::getNow(m_pPlayer);
        if (m_PlatinumVipEndTime > 0 && m_PlatinumVipEndTime < Now)
        {
            CharId_t Cid = Player::getCid(m_pPlayer);
            DBService* pDB = Answer::Singleton<DBService>::instance();
            std::string empty;
            DBService::OnSendSysMail(pDB, 0, Cid, 6387, &empty, 0);
            m_PlatinumVipEndTime = 0;
            Player::RecalcAttr(m_pPlayer);
        }
        if (m_DiamVipEndTime > 0 && m_DiamVipEndTime < Now)
        {
            CharId_t Cid = Player::getCid(m_pPlayer);
            DBService* pDB = Answer::Singleton<DBService>::instance();
            std::string empty;
            DBService::OnSendSysMail(pDB, 0, Cid, 6388, &empty, 0);
            m_DiamVipEndTime = 0;
            Player::RecalcAttr(m_pPlayer);
        }
        if (m_StarVipEndTime > 0 && m_StarVipEndTime < Now)
        {
            CharId_t Cid = Player::getCid(m_pPlayer);
            DBService* pDB = Answer::Singleton<DBService>::instance();
            std::string empty;
            DBService::OnSendSysMail(pDB, 0, Cid, 6389, &empty, 0);
            m_StarVipEndTime = 0;
            Player::RecalcAttr(m_pPlayer);
        }
    }
}

void CVip::LoadDropString(std::string* p_String)
{
    if (m_pPlayer && p_String)
    {
        StringVector DropString;
        Answer::StringUtility::split(&DropString, p_String, "|", 0);
        for (auto it = DropString.begin(); it != DropString.end(); ++it)
        {
            StringVector DropTimesString;
            Answer::StringUtility::split(&DropTimesString, &(*it), ":", 0);
            if (DropTimesString.size() == 3)
            {
                int32_t level = atoi(DropTimesString[0].c_str());
                m_VipLuckyDropMap[level].DropOrdinaryTimes = atoi(DropTimesString[1].c_str());
                m_VipLuckyDropMap[level].DropSpecialTimes = atoi(DropTimesString[2].c_str());
            }
        }
    }
}

std::string CVip::SaveDropString()
{
    std::stringstream ss;
    for (int32_t i = 1; i <= 14; i++)
    {
        ss << i << ":" << m_VipLuckyDropMap[i].DropOrdinaryTimes << ":" << m_VipLuckyDropMap[i].DropSpecialTimes << "|";
    }
    return ss.str();
}

bool CVip::HaveVipPrivilege()
{
    if (!m_pPlayer)
        return false;
    int32_t Now = Unit::getNow(m_pPlayer);
    if (m_DiamVipEndTime > Now)
        return true;
    if (m_PlatinumVipEndTime > Now)
        return true;
    return m_StarVipEndTime > Now;
}

bool CVip::CanAuction()
{
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    if (CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL) + 1 <= 10)
    {
        if (CfgData::getServerDiffDay(pCfg, SERVER_TYPE::SVT_NORMAL) + 1 > 1 && GetVipLevel() <= 0)
            return false;
    }
    else if (GetVipLevel() <= 1)
    {
        return false;
    }
    return true;
}

int32_t CVip::GetStorePage()
{
    int8_t VipLevel = GetVipLevel();
    CfgData* pCfg = Answer::Singleton<CfgData>::instance();
    VipTable* pTable = CfgData::GetVipTable(pCfg);
    VipCfg* pVipCfg = VipTable::GetVipCfg(pTable, VipLevel);
    if (pVipCfg)
        return pVipCfg->StorePage;
    return 0;
}

void CVip::AddClubDropTime()
{
    if (m_Club && m_ClubDropTimes <= 0)
    {
        m_ClubDropTimes = 1;
        SendClubInfo();
    }
}

void CVip::SendClubInfo()
{
    if (m_pPlayer)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CF7);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, m_Club);
            Answer::NetPacket::writeInt32(packet, m_ClubDropTimes);
            Answer::NetPacket::writeInt32(packet, m_BuyTimes);
            CVipClub* pClub = Answer::Singleton<CVipClub>::instance();
            CVipClub::AppendClubWallInfo(pClub, packet);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            int16_t GateIndex = Player::getGateIndex(m_pPlayer);
            GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
        }
    }
}
