#include "Other/CSpecialEquip.h"
#include "Game/Player.h"
#include "GameService.h"
#include "Config/CfgData.h"
#include "Network/NetPacket.h"

CSpecialEquip::CSpecialEquip()
    : CExtSystemBase()
{
    m_SpecialEquipData.clear();
}

CSpecialEquip::~CSpecialEquip()
{
}

void CSpecialEquip::OnCleanUp()
{
}

void CSpecialEquip::OnUpdate(int64_t curTick)
{
}

void CSpecialEquip::OnDaySwitch(int32_t nDiffDays)
{
}

void CSpecialEquip::OnLoadFromDB(const PlayerDBData* dbData)
{
    m_SpecialEquipData = dbData->m_SpecialEquipDBData.m_SpecialEquipData;
}

void CSpecialEquip::OnSaveToDB(PlayerDBData* dbData)
{
    dbData->m_SpecialEquipDBData.m_SpecialEquipData = m_SpecialEquipData;
}

void CSpecialEquip::GetInterestsProtocol(ProcIdList* procList)
{
    procList->push_back(715);
    procList->push_back(716);
    procList->push_back(717);
}

int32_t CSpecialEquip::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket)
        return 2;
    switch (nProcId)
    {
    case 715:
        return OnGetData(inPacket);
    case 716:
        return OnActivate(inPacket);
    case 717:
        return OnUpgrade(inPacket);
    default:
        return 2;
    }
}

int32_t CSpecialEquip::OnGetData(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v2 = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(v2, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D8C);
    if (!packet)
        return 2;
    int32_t nSize = m_SpecialEquipData.size();
    packet->writeInt32(nSize);
    for (auto it = m_SpecialEquipData.begin(); it != m_SpecialEquipData.end(); ++it)
    {
        packet->writeInt32(it->first);
        packet->writeInt32(it->second);
    }
    uint32_t WOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setSize(packet, WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(v2, ConnId, GateIndex, packet);
    return 0;
}

int32_t CSpecialEquip::OnActivate(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nEquipId = inPacket->readInt32();
    if (m_SpecialEquipData.find(nEquipId) != m_SpecialEquipData.end())
        return 2;
    CfgData* v4 = Answer::Singleton<CfgData>::instance();
    const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(v4);
    const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nEquipId);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_ActivateCost, CURRENCY_CHANGE_REASON::GCR_SPECIALEQUIP_ACTIVATE, 0))
        return 2;
    m_SpecialEquipData[nEquipId] = 1;
    CExtBuffManager* pBuffMgr = Player::GetBuffMgr(m_pPlayer);
    CExtBuffManager::AddBuff(pBuffMgr, pCfg->m_ActivateBuff, 0, 0);
    CExtCharBag* pBag = Player::GetBag(m_pPlayer);
    CExtCharBag::CalcProperty(pBag);
    Player::SendMsgUpdate(m_pPlayer);
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v13 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v13, ConnId, GateIndex, Proc, 0);
    return 0;
}

int32_t CSpecialEquip::OnUpgrade(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer)
        return 2;
    int32_t nEquipId = inPacket->readInt32();
    auto it = m_SpecialEquipData.find(nEquipId);
    if (it == m_SpecialEquipData.end())
        return 2;
    int32_t nCurLvl = it->second;
    if (nCurLvl >= 50)
        return 2;
    CfgData* v5 = Answer::Singleton<CfgData>::instance();
    const CfgSpecialEquipLvlTable* pLvlTable = CfgData::GetSpecialEquipLvlTable(v5);
    const CfgSpecialEquipLvl* pCfg = CfgSpecialEquipLvlTable::GetRow(pLvlTable, nCurLvl + 1);
    if (!pCfg)
        return 2;
    if (!Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_UpgradeCost, CURRENCY_CHANGE_REASON::GCR_SPECIALEQUIP_UPGRADE, 0))
        return 2;
    m_SpecialEquipData[nEquipId] = nCurLvl + 1;
    uint16_t Proc = Answer::NetPacket::getProc(inPacket);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* v12 = Answer::Singleton<GameService>::instance();
    GameService::replySuccess(v12, ConnId, GateIndex, Proc, 0);
    return 0;

void CSpecialEquip::SendSpecialEquipIcon()
{
    if (!m_pPlayer)
        return;
    ShowIcon stu;
    GetSpecialEquipIcon(&stu);
    Player::SendIconState(m_pPlayer, &stu);
}

void CSpecialEquip::AddCharAttr()
{
    if (!m_pPlayer)
        return;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    if (nRecord <= 0)
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    for (int32_t i = 0; i < 30; ++i)
    {
        if (nRecord & (1 << i))
        {
            int32_t nDay = i + 1;
            const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
            const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nDay);
            if (pCfg && pCfg->m_AttrId > 0 && pCfg->m_AttrValue > 0)
            {
                Player::AddCharAttrAddon(m_pPlayer, pCfg->m_AttrId, pCfg->m_AttrValue);
            }
        }
    }
}

bool CSpecialEquip::CanActivation(int32_t nIndex)
{
    if (!m_pPlayer)
        return false;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
    const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nIndex);
    if (!pCfg)
        return false;
    int32_t nDay = pCfg->m_NeedDay;
    if (nDay > 0)
    {
        int32_t nLoginDay = Player::getRecord(m_pPlayer, 1148);
        if (nLoginDay < nDay)
            return false;
    }
    CVip* pVip = Player::GetPlayerVip(m_pPlayer);
    if (CVip::GetVipLevel(pVip) < pCfg->m_NeedVip)
        return false;
    if (pCfg->m_CostGold > 0)
    {
        if (!Player::CheckCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_CostGold))
            return false;
    }
    return true;
}

void CSpecialEquip::GetSpecialEquipIcon(IconStateList* IconList)
{
    if (!m_pPlayer)
        return;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 112))
    {
        ShowIcon stu;
        GetSpecialEquipIcon(&stu);
        IconList->push_back(stu);
    }
}

void CSpecialEquip::InitSkill()
{
    if (!m_pPlayer)
        return;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    if (nRecord <= 0)
        return;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    for (int32_t i = 0; i < 30; ++i)
    {
        if (nRecord & (1 << i))
        {
            int32_t nDay = i + 1;
            const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
            const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nDay);
            if (pCfg && pCfg->m_SkillId > 0)
            {
                CExtSkillManager* pSkillMgr = Player::GetSkillMgr(m_pPlayer);
                CExtSkillManager::AddSkill(pSkillMgr, pCfg->m_SkillId, 0);
            }
        }
    }
}

int32_t CSpecialEquip::CanActivationCount()
{
    if (!m_pPlayer)
        return 0;
    int32_t nCount = 0;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    for (int32_t i = 0; i < 30; ++i)
    {
        if (!(nRecord & (1 << i)))
        {
            if (CanActivation(i + 1))
                ++nCount;
        }
    }
    return nCount;
}

void CSpecialEquip::GetSpecialEquipIcon(ShowIcon* retstr)
{
    memset(retstr, 0, sizeof(ShowIcon));
    if (!m_pPlayer)
        return;
    retstr->nId = 115;
    CFunctionOpen* pFuncOpen = Player::GetPlayerFunctionOpen(m_pPlayer);
    if (CFunctionOpen::IsOpened(pFuncOpen, 112) && CanActivationCount() > 0)
        retstr->nState = 2;
    else
        retstr->nState = 4;
    retstr->nEffects = 1;
}

void CSpecialEquip::SendActivationInfo()
{
    if (!m_pPlayer)
        return;
    int8_t ConnId = Player::getConnId(m_pPlayer);
    GameService* pGame = Answer::Singleton<GameService>::instance();
    Answer::NetPacket* packet = GameService::popNetpacket(pGame, ConnId, Answer::PackType::PACK_DISPATCH, 0x2D8D);
    if (!packet)
        return;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    packet->writeInt32(nRecord);
    uint32_t WOffset = packet->getWOffset();
    packet->setSize(WOffset);
    int16_t GateIndex = Player::getGateIndex(m_pPlayer);
    GameService::sendPacketTo(pGame, ConnId, GateIndex, packet);
}

int32_t CSpecialEquip::OnActivation(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket)
        return 10002;
    int32_t nIndex = inPacket->readInt32();
    if (nIndex < 1 || nIndex > 30)
        return 10002;
    if (IsActivation(nIndex))
        return 10002;
    if (!CanActivation(nIndex))
        return 10002;
    CfgData* pCfgData = Answer::Singleton<CfgData>::instance();
    const CfgSpecialEquipTable* pTable = CfgData::GetSpecialEquipTable(pCfgData);
    const CfgSpecialEquip* pCfg = CfgSpecialEquipTable::GetRow(pTable, nIndex);
    if (!pCfg)
        return 10002;
    int32_t nRecord = Player::getRecord(m_pPlayer, 1149);
    Player::updateRecord(m_pPlayer, 1149, nRecord | (1 << (nIndex - 1)));
    if (pCfg->m_CostGold > 0)
    {
        Player::DecCurrency(m_pPlayer, CURRENCY_TYPE::CURRENCY_GOLD, pCfg->m_CostGold, CURRENCY_CHANGE_REASON::GCR_SPECIALEQUIP_ACTIVATE, nIndex);
    }
    SendActivationInfo();
    Player::RecalcAttr(m_pPlayer);
    return 0;
}
