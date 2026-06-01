#include "Other/Curse.h"

#include <cstring>
#include <algorithm>

Curse::Curse()
    : CExtSystemBase()
{
}

Curse::~Curse()
{
}

void Curse::OnCleanUp()
{
}

void Curse::OnUpdate(int64_t curTick)
{
}

void Curse::OnDaySwitch(int32_t nDiffDays)
{
}

void Curse::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void Curse::OnSaveToDB(PlayerDBData* dbData)
{
}

void Curse::GetInterestsProtocol(ProcIdList* procList)
{
    uint16_t proc = 268;
    procList->push_back(proc);
}

int32_t Curse::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 10002;
    if (nProcId == 268)
        return OnCurseLevelUp(inPacket);
    return 10002;
}

void Curse::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    int32_t CurLevel = Player::getRecord(m_pPlayer, 1150);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    DamnationCfg* CurCfg = CfgData::GetDamnationCfg(pCfgData, CurLevel);
    if (!CurCfg)
        return;
    for (auto& attr : CurCfg->AttrList)
    {
        Unit::AddAttrValue(m_pPlayer, (CObjAttrs::Index_T)attr.m_nAddAttrType, attr.m_nAddAttrValue);
    }
}

int32_t Curse::OnCurseLevelUp(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t CurLevel = Player::getRecord(m_pPlayer, 1150);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    DamnationCfg* CurCfg = CfgData::GetDamnationCfg(pCfgData, CurLevel);
    DamnationCfg* NextCfg = CfgData::GetDamnationCfg(pCfgData, CurLevel + 1);
    if (!CurCfg || !NextCfg)
        return 10002;
    if (CurCfg->CostMoney > 0)
    {
        CExtCurrency* Currency = Player::GetCurrency(m_pPlayer);
        if (CExtCurrency::GetMoneyBindAndNoBind(Currency) < CurCfg->CostMoney)
            return 10002;
    }
    if (!CurCfg->CostItems.empty())
    {
        CExtCharBag* Bag = Player::GetBag(m_pPlayer);
        if (!CExtCharBag::RemoveItem(Bag, &CurCfg->CostItems, ITEM_CHANGE_REASON::ICR_CURSE_LEVEL_UP))
            return 10002;
    }
    if (CurCfg->CostMoney > 0)
    {
        CExtCurrency* Currency = Player::GetCurrency(m_pPlayer);
        if (!CExtCurrency::DecMoneyAndNoBind(Currency, CurCfg->CostMoney, CURRENCY_CHANGE_REASON::MCR_CURSE_LEVEL_UP, 0))
            return 10002;
    }
    Player::updateRecord(m_pPlayer, 1150, CurLevel + 1);
    Player::sendUpdateSocialPlayerInfo(m_pPlayer, PlayerInfoIndex::PII_ZU_ZHOU, CurLevel + 1);
    Player::RecalcAttr(m_pPlayer);
    if (NextCfg->GongGaoId > 0)
    {
        int8_t ConnId = Player::getConnId(m_pPlayer);
        GameService* pGame = Answer::Singleton<GameService>::instance();
        Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2CD6);
        if (packet)
        {
            Answer::NetPacket::writeInt32(packet, NextCfg->GongGaoId);
            std::string name;
            Player::getName(m_pPlayer, &name);
            Answer::NetPacket::writeUTF8(packet, &name);
            CharId_t Cid = Player::getCid(m_pPlayer);
            Answer::NetPacket::writeInt64(packet, Cid);
            Answer::NetPacket::writeInt32(packet, CurLevel + 1);
            uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
            Answer::NetPacket::setSize(packet, WOffset);
            int8_t connId = Player::getConnId(m_pPlayer);
            GameService::worldBroadcast(pGame, connId, packet);
        }
    }
    important_system_log stu;
    important_system_log::important_system_log(&stu);
    stu.log_Sid = Player::getSid(m_pPlayer);
    stu.log_time = Unit::getNow(m_pPlayer);
    std::string passport;
    Player::GetPassport(m_pPlayer, &passport);
    stu.passport = passport;
    stu.cid = Player::getCid(m_pPlayer);
    Player::getName(m_pPlayer, &stu.name);
    stu.up_time = Unit::getNow(m_pPlayer);
    stu.sys_name = 11;
    stu.oper_id = 1;
    stu.level = CurLevel + 1;
    int8_t connId = Player::getConnId(m_pPlayer);
    DBService* pDB = Answer::Singleton<DBService>::instance();
    DBService::AddPlatformLog(pDB, connId, PLATFORM_LOG_DATA_TYPE::PLDT_SYSTEM, &stu);
    important_system_log::~important_system_log(&stu);
    return 0;
}

int32_t Curse::DamageValue(int64_t MaxHp)
{
    if (!m_pPlayer)
        return 0;
    int32_t CurLevel = Player::getRecord(m_pPlayer, 1150);
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    DamnationCfg* CurCfg = CfgData::GetDamnationCfg(pCfgData, CurLevel);
    if (!CurCfg || CurCfg->HpPecent <= 0 || CurCfg->Probability <= 0)
        return 0;
    Answer::Random* pRand = Answer::Singleton<Answer::Random>::instance();
    int32_t nRand = Answer::Random::generate(pRand, 1, 1000);
    if (Player::getRecord(m_pPlayer, 1155) > 0)
        return 0;
    if (nRand <= CurCfg->Probability)
        return (int32_t)(MaxHp * CurCfg->HpPecent / 100);
    return 0;
}
