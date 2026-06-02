// CExtCharSkill.cpp
#include "CExtCharSkill.h"
#include "Database/PlayerDBData.h"
#include <cstring>

CExtCharSkill::CExtCharSkill()
{
    OnCleanUp();
}

CExtCharSkill::~CExtCharSkill()
{
}

void CExtCharSkill::OnCleanUp()
{
    m_SkillLevelMap.clear();
    m_lAddonSkills.clear();
    m_nLastAddonTick = 0;
    m_PowerLastTick = 0;
    m_lastActionTick = 0;
    m_nLastEnergyTick = 0;
    m_mSkillCD.clear();
    m_lActiveSkills.clear();
    m_lPassiveSkills.clear();
    m_lTrigSkills.clear();
    m_nPower = 0;
    m_nDropMoneyRate = 0;
    m_lSummonSkills.clear();
    m_mActiveSkills.clear();
    m_lActiveTalent.clear();
    m_SkillBattle = 0;
    m_OthreSkillLevelMap.clear();
    m_SkillPublicCD = 0;
    m_lActive.clear();
}

void CExtCharSkill::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        loadTalentPoints(&dbData->skillData.talents);
        m_nPower = dbData->skillData.m_nPower;
    }
    CallSkillBattle();
}

void CExtCharSkill::InitSystem()
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData)
    {
        dbData->skillData.talents = saveTalentPoints();
        dbData->skillData.m_nPower = m_nPower;
    }
}

std::string CExtCharSkill::saveTalentPoints()
{
    std::string result;
    return result;
}

void CExtCharSkill::loadTalentPoints(const std::string* str)
{
    if (!str || !m_pPlayer) return;
}

void CExtCharSkill::OnUpdate(int64_t curTick)
{
    checkAddonSkills(curTick);
    checkSummonSkills(curTick);
    checkSuitSkillEnergy(curTick);
    checkActiveList();
    
    if (curTick - m_PowerLastTick > 59999)
    {
        m_PowerLastTick = curTick;
        AddPower(60);
    }
}

void CExtCharSkill::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 40;
    ProcId_t v4 = 38;
    ProcId_t v5 = 37;
    procList->push_back(v3);
    procList->push_back(v4);
    procList->push_back(v5);
}

int32_t CExtCharSkill::onAddTalentPoint(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

bool CExtCharSkill::CanUseSkillAndMove(int64_t nCurTick, bool ResetTick)
{
    return true;
}

int32_t CExtCharSkill::onDoUnitSkill(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharSkill::onDoAddonSkill(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket) return 10002;
    return 0;
}

void CExtCharSkill::AddCharAttr()
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::SendTalentInfo()
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::SendTalentAddon()
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::CheckHPPecentTrigBuff(int32_t nLastPecent, int32_t nNowPecent)
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::CheckPhaseDamageTrigBuff(int8_t nPhase)
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::CheckSelfTrigBuff(int8_t nTrigType)
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::CheckSummonBuff()
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::initSkills(int32_t TalentId, int32_t TalentLevel)
{
}

void CExtCharSkill::removeSkillByTalent(int32_t TalentId, int32_t TalentLevel)
{
}

void CExtCharSkill::checkActiveList()
{
    if (!m_pPlayer || m_lActive.empty()) return;
    m_lActive.clear();
}

void CExtCharSkill::activeSkill(int32_t nId, const void* handler)
{
}

void CExtCharSkill::doSkillActive(const void* pCfgSkill, void* pTarget)
{
}

void CExtCharSkill::AddPower(int32_t nValue)
{
    m_nPower += nValue;
    if (m_nPower < 0) m_nPower = 0;
    if (m_nPower > 3000) m_nPower = 3000;
}

bool CExtCharSkill::addAddonSkill(const AddonSkill* addonSkill)
{
    return 1;
}

bool CExtCharSkill::removeAddonSkill(int32_t nSkillFlag)
{
    return 0;
}

void CExtCharSkill::checkAddonSkills(int64_t nCurTick)
{
    if (nCurTick - m_nLastAddonTick > 999)
    {
        m_nLastAddonTick = nCurTick;
    }
}

void CExtCharSkill::checkSummonSkills(int64_t nCurTick)
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::checkSuitSkillEnergy(int64_t nCurTick)
{
    if (nCurTick - m_nLastEnergyTick > 999)
    {
        m_nLastEnergyTick = nCurTick;
        AddSuitSkillEnergy();
    }
}

int32_t CExtCharSkill::calActiveSkillCD(int32_t nSkillId)
{
    return 800;
}

void CExtCharSkill::recalTalentAddon()
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::getTalentAddon(void* addonList)
{
}

void CExtCharSkill::addTalentAddon(const void* stu)
{
}

void CExtCharSkill::recalAcviveSkill()
{
    m_mActiveSkills.clear();
}

void CExtCharSkill::addActiveSkill(const void* pCfgSkill)
{
}

void CExtCharSkill::recalDropMoneyRate()
{
    m_nDropMoneyRate = 0;
}

void CExtCharSkill::AddSkillPoint(int32_t Id, int32_t Points)
{
}

bool CExtCharSkill::UseSkillBook(int32_t Id)
{
    return 0;
}

bool CExtCharSkill::AddSkillBookPoint(int32_t Id, int32_t Points)
{
    return 0;
}

void CExtCharSkill::SendlearedSkill(int32_t TalentId)
{
    if (!m_pPlayer) return;
}

void CExtCharSkill::AddOtherSkill(int32_t TalentId, int32_t TalentLevel)
{
    if (TalentLevel > 0 && TalentId > 0)
    {
        initSkills(TalentId, TalentLevel);
        CallSkillBattle();
    }
}

void CExtCharSkill::CallSkillBattle()
{
    recalAcviveSkill();
    recalTalentAddon();
    recalDropMoneyRate();
}

void CExtCharSkill::AddSuitSkillEnergy()
{
}

void CExtCharSkill::checkSkillTrigBuff(const void* pCfgSkill, void* results)
{
}

int32_t CExtCharSkill::doSkill(Answer::NetPacket* inPacket, const void* pCfgSkill, 
                                void* results, void* srcPos, int32_t nSkillId, 
                                int32_t nSkillFlag, int32_t bCheck)
{
    return 0;
}

void CExtCharSkill::addSkillBuffTo(int32_t nBuffId, int32_t nSkillId, 
                                    int32_t nGroupId, void* pTarget)
{
}

void CExtCharSkill::OnDaySwitch(int32_t nDiffDays)
{
}