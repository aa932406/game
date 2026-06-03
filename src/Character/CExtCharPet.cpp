// CExtCharPet.cpp
#include "CExtCharPet.h"
#include "Database/PlayerDBData.h"
#include <cstring>

CExtCharPet::CExtCharPet()
{
    m_pFightPet = 0;
    m_nStar = 0;
    m_HuanHua = 0;
    m_nLastTick = 0;
    memset(m_vSummonPet, 0, sizeof(m_vSummonPet));
    m_lSummons.clear();
}

CExtCharPet::~CExtCharPet()
{
}

void CExtCharPet::OnCleanUp()
{
    m_nStar = 0;
    m_HuanHua = 0;
    m_nLastTick = 0;
    
    if (m_pFightPet)
    {
        m_pFightPet = 0;
    }
    clearSummonPet();
}

void CExtCharPet::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        m_nStar = dbData->m_CharPets.nStar;
        m_HuanHua = dbData->m_CharPets.nHuanHua;
        if (!m_HuanHua) m_HuanHua = m_nStar;
    }
}

void CExtCharPet::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData && m_pFightPet)
    {
        dbData->m_CharPets.nStar = m_nStar;
        dbData->m_CharPets.nHuanHua = m_HuanHua;
    }
}

void CExtCharPet::OnUpdate(int64_t curTick)
{
    if (m_pFightPet)
    {
        // Update pet
    }
    
    for (int i = 0; i <= 3; ++i)
    {
        if (m_vSummonPet[i])
        {
            // Update summon pet
        }
    }
}

void CExtCharPet::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t procs[] = {731, 732, 733, 734, 736, 737, 738, 739, 740, 750};
    for (int i = 0; i < 10; ++i)
    {
        procList->push_back(procs[i]);
    }
}

int32_t CExtCharPet::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket) return 2;
    
    switch (nProcId)
    {
        case 730: return onFight(inPacket);
        case 731: return onRest(inPacket);
        case 732: return onUpStar(inPacket);
        case 733: return onDressEquip(inPacket);
        case 734: return onUnDressEquip(inPacket);
        case 736: return onChangeName(inPacket);
        case 737: return onChangeAi(inPacket);
        case 738: return onPetHuanHua(inPacket);
        case 739: return onFitting(inPacket);
        case 740: return onSkillUpLevel(inPacket);
        default: return 0;
    }
}

int32_t CExtCharPet::onFight(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

bool CExtCharPet::Fight()
{
    if (!m_pFightPet) return 0;
    return 1;
}

int32_t CExtCharPet::onRest(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

void CExtCharPet::onRestPet()
{
    if (!m_pPlayer || !m_pFightPet) return;
}

int32_t CExtCharPet::onFitting(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

int32_t CExtCharPet::onPetHuanHua(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

int32_t CExtCharPet::onUpStar(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

void CExtCharPet::SendPetStaus()
{
    if (!m_pFightPet) return;
}

int32_t CExtCharPet::onDressEquip(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

int32_t CExtCharPet::onUnDressEquip(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

int32_t CExtCharPet::onSkillUpLevel(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

int32_t CExtCharPet::onChangeName(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

int32_t CExtCharPet::onChangeAi(Answer::NetPacket* inPacket)
{
    if (!m_pPlayer || !inPacket || !m_pFightPet) return 10002;
    return 0;
}

void CExtCharPet::OnLevelUp()
{
    if (!m_pFightPet) return;
    sendPetInfo();
}

bool CExtCharPet::IsFunctionOpen()
{
    return true;
}

void CExtCharPet::OnInitPet()
{
    if (!m_pPlayer || m_pFightPet) return;
    m_nStar = 1;
    m_HuanHua = 1;
}

void CExtCharPet::OnPlayerDie()
{
    if (!m_pPlayer) return;
    clearSummonPet();
}

void CExtCharPet::clearSummonPet()
{
    memset(m_vSummonPet, 0, sizeof(m_vSummonPet));
    m_lSummons.clear();
}

void CExtCharPet::OnSummon(int32_t nSkillId, const void* pos)
{
    if (!m_pPlayer) return;
}

bool CExtCharPet::HaveSummonAlive()
{
    for (int i = 0; i <= 3; ++i)
    {
        if (m_vSummonPet[i]) return 1;
    }
    return 0;
}

void CExtCharPet::AddCharAttr()
{
    if (!m_pPlayer) return;
}

void CExtCharPet::AddPetAttr(void* pObjPet)
{
}

void CExtCharPet::RecalPetAttr()
{
    if (!m_pFightPet) return;
}

void CExtCharPet::AddTarget(void* launcher, bool bForce)
{
}

void CExtCharPet::SetTargetTile(int32_t tx, int32_t ty, Direction dir)
{
}

void CExtCharPet::SendPetInfo()
{
    if (IsFunctionOpen())
    {
        if (!m_pFightPet) OnInitPet();
        sendPetInfo();
    }
}

void CExtCharPet::sendPetInfo()
{
    if (!m_pPlayer || !m_pFightPet) return;
}

bool CExtCharPet::addSummonPet(void* pPet)
{
    return 0;
}

void CExtCharPet::removeSummonPet(void* pPet)
{
}

void CExtCharPet::OnDaySwitch(int32_t nDiffDays)
{
}