// CExtCharFamily.cpp
#include "CExtCharFamily.h"
#include <cstring>

CExtCharFamily::CExtCharFamily()
{
    OnCleanUp();
}

CExtCharFamily::~CExtCharFamily()
{
}

void CExtCharFamily::OnCleanUp()
{
    m_nFamilyId = 0;
    m_nPosition = 0;
    m_nContribution = 0;
    m_nDailyContribution = 0;
    m_MedLevel = 0;
    m_MedRes = 0;
    m_Hoe = 0;
    m_EnterDungeionFamilyId = 0;
    m_EnterCount = 0;
    m_EnterTime = 0;
    m_lSkills.clear();
}

void CExtCharFamily::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_nFamilyId = dbData->m_FamilyData.nFamilyId;
        m_nPosition = dbData->m_FamilyData.nPosition;
        m_nContribution = dbData->m_FamilyData.nContribution;
        m_nDailyContribution = dbData->m_FamilyData.nDailyContribution;
        m_MedLevel = dbData->m_FamilyData.nMedLevel;
        m_MedRes = dbData->m_FamilyData.nMedRes;
        m_Hoe = dbData->m_FamilyData.nHoe;
        m_EnterDungeionFamilyId = dbData->m_FamilyData.EnterDungeionFamilyId;
        m_EnterCount = dbData->m_FamilyData.EnterCount;
        m_EnterTime = dbData->m_FamilyData.EnterTime;
        loadSkillString(&dbData->m_FamilyData.strSkills);
    }
}

void CExtCharFamily::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->m_FamilyData.nFamilyId = m_nFamilyId;
        dbData->m_FamilyData.nPosition = m_nPosition;
        dbData->m_FamilyData.nContribution = m_nContribution;
        dbData->m_FamilyData.nDailyContribution = m_nDailyContribution;
        dbData->m_FamilyData.strSkills = saveSkillString();
        dbData->m_FamilyData.nMedLevel = m_MedLevel;
        dbData->m_FamilyData.nMedRes = m_MedRes;
        dbData->m_FamilyData.nHoe = m_Hoe;
        dbData->m_FamilyData.EnterDungeionFamilyId = m_EnterDungeionFamilyId;
        dbData->m_FamilyData.EnterCount = m_EnterCount;
        dbData->m_FamilyData.EnterTime = m_EnterTime;
    }
}

void CExtCharFamily::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t procs[] = {779, 778, 777, 776, 798, 747, 748, 20059, 20060, 783,
                        785, 784, 786, 20063, 935, 787, 20065, 781, 782, 788,
                        789, 936, 20062, 20064, 20066};
    for (int i = 0; i < 25; ++i)
    {
        procList->push_back(procs[i]);
    }
}

int32_t CExtCharFamily::onContribution(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (GetFamilyId() <= 0) return 10002;
    return 0;
}

int32_t CExtCharFamily::onOpenTerritory(Answer::NetPacket* inPacket)
{
    if (!inPacket) return 10002;
    if (GetFamilyId() <= 0) return 10002;
    if (GetPosition() != 3) return 10002;
    return 0;
}

int32_t CExtCharFamily::onEnterTerritory(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    if (GetFamilyId() <= 0) return 10002;
    return 0;
}

int32_t CExtCharFamily::onFamilyLevelUpMedal(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 2;
    return 0;
}

int32_t CExtCharFamily::onFamilyBuyHoe(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharFamily::EndMining()
{
    if (m_Hoe <= 0 || !m_pPlayer) return 0;
    return 0;
}

int32_t CExtCharFamily::onWearHoe(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 2;
    return 0;
}

void CExtCharFamily::SendHoe()
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::onSocialCreateFamily(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharFamily::onSocialChangeFamilyName(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharFamily::onSocialUpdateFamilyInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharFamily::onReceiveFamilyTask(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 10002;
    if (GetFamilyId() <= 0) return 10002;
    return 0;
}

int32_t CExtCharFamily::onQuickSubmitFamilyTask(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharFamily::onGetFamilyTaskReward(Answer::NetPacket* inPacket)
{
    if (!inPacket) return 10002;
    if (GetFamilyId() <= 0) return 10002;
    return 0;
}

bool CExtCharFamily::IsDeclareWarFamily(FamilyId_t nFamilyId)
{
    return false;
}

void CExtCharFamily::SendAddFamilyTaskCount()
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::onImpeach(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 2;
    if (m_nFamilyId <= 0) return 10002;
    return 0;
}

void CExtCharFamily::sendSocialImpeach()
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::onSocialImpeachResult(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharFamily::onSendRedPacket(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

void CExtCharFamily::sendSocialSendRedPacket(int32_t nTotalValue, int32_t nTimes)
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::onEnterFamilyDungeon(Answer::NetPacket* inPacket)
{
    if (m_pPlayer) EnterFamilyDungeon();
    return 10002;
}

int32_t CExtCharFamily::EnterFamilyDungeon()
{
    if (!m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharFamily::onBindRedPacketTake(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

int32_t CExtCharFamily::onRequestSkillInfo(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    sendSkillInfo();
    return 0;
}

int32_t CExtCharFamily::onLearnSkill(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_nFamilyId <= 0) return 10002;
    return 0;
}

void CExtCharFamily::sendSkillInfo()
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::getSkillLevel(int32_t nSkillId)
{
    for (auto& skill : m_lSkills)
    {
        if (skill.nSkillId == nSkillId)
            return skill.nSkillLevel;
    }
    return 0;
}

void CExtCharFamily::addSkillLevel(int32_t nSkillId)
{
    for (auto& skill : m_lSkills)
    {
        if (skill.nSkillId == nSkillId)
        {
            skill.nSkillLevel++;
            return;
        }
    }
    
    FamilySkill skill;
    skill.nSkillId = nSkillId;
    skill.nSkillLevel = 1;
    m_lSkills.push_back(skill);
}

void CExtCharFamily::AddSkillAttr()
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::GetDefRdc(void* Target)
{
    return 0;
}

int32_t CExtCharFamily::onSetTalkTask(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_nFamilyId <= 0) return 10002;
    return 0;
}

int32_t CExtCharFamily::onGetTalkTaskReward(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_nFamilyId <= 0) return 10002;
    return 0;
}

void CExtCharFamily::loadSkillString(const std::string* strSkills)
{
    m_lSkills.clear();
}

std::string CExtCharFamily::saveSkillString()
{
    std::string result;
    return result;
}

int32_t CExtCharFamily::onAddBossPoint(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    if (m_nFamilyId <= 0) return 10002;
    return 0;
}

int32_t CExtCharFamily::onSocialStartBoss(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

void CExtCharFamily::StartFamilyAct(int32_t Param)
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::onSocialInviteAll(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

bool CExtCharFamily::AddMedRes(int32_t AddValues)
{
    if (AddValues <= 0) return 0;
    m_MedRes += AddValues;
    sendSkillInfo();
    return 1;
}

int32_t CExtCharFamily::onStartFamlyDungeon(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

void CExtCharFamily::SendFamilyDungeonInfo()
{
    if (!m_pPlayer) return;
}

int32_t CExtCharFamily::onGetFamilyDungeonInfo(Answer::NetPacket* inPacket)
{
    SendFamilyDungeonInfo();
    return 0;
}

void CExtCharFamily::AddContribution(int32_t nValue, int32_t nReason)
{
    m_nContribution += nValue;
    if (nValue > 0)
    {
        m_nDailyContribution += nValue;
    }
}

int32_t CExtCharFamily::GetFamilyId()
{
    return m_nFamilyId;
}

int32_t CExtCharFamily::GetPosition()
{
    return m_nPosition;
}

int32_t CExtCharFamily::GetFamilyLevel()
{
    return 1;
}

void CExtCharFamily::OnUpdate(int64_t curTick)
{
}