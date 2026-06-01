// CExtCharTitle.cpp
#include "CExtCharTitle.h"
#include <cstring>

CExtCharTitle::CExtCharTitle()
{
    OnCleanUp();
}

CExtCharTitle::~CExtCharTitle()
{
}

void CExtCharTitle::Init(Player* pPlayer)
{
    CExtSystemBase::Init(pPlayer);
    initTitleMap();
}

void CExtCharTitle::OnCleanUp()
{
    m_nDressTitle = 0;
    m_nDressNormalTitle = 0;
    m_nBattleRankTop = 0;
    m_bShowActivityTitle = 0;
    m_LastUpdateTick = 0;
    memset(m_vDress, 0, sizeof(m_vDress));
    memset(m_vActivityTitle, 0, sizeof(m_vActivityTitle));
    resetTitleMap();
}

void CExtCharTitle::GetInterestsProtocol(ProcIdList* procList)
{
    ProcId_t v3 = 861;
    procList->push_back(v3);
}

int32_t CExtCharTitle::onDressTitle(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

int32_t CExtCharTitle::onUnDressTitle(Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return 10002;
    return 0;
}

bool CExtCharTitle::dressTitle(int32_t nId)
{
    return 0;
}

bool CExtCharTitle::unDressTitle(int32_t nId)
{
    return 0;
}

void CExtCharTitle::SendTitleInfo()
{
    if (!m_pPlayer) return;
}

void CExtCharTitle::AppendDressTitle(Answer::NetPacket* packet)
{
    if (!packet) return;
}

void CExtCharTitle::AddCharAttr()
{
    if (!m_pPlayer) return;
}

void CExtCharTitle::addAttr(const void* attrAddons)
{
    if (!m_pPlayer) return;
}

void CExtCharTitle::InitTitle()
{
    if (!m_pPlayer) return;
}

void CExtCharTitle::CheckAddTitle(int8_t type, int32_t param)
{
    if (!m_pPlayer) return;
}

void CExtCharTitle::RemoveTitle(int32_t type, int32_t param)
{
    if (!m_pPlayer) return;
}

void CExtCharTitle::OnUpdate(int64_t curTick)
{
    if (!m_pPlayer) return;
    
    if (curTick - m_LastUpdateTick > 59999)
    {
        m_LastUpdateTick = curTick;
        // Update titles
    }
}

void CExtCharTitle::autoDressTitle(int32_t nId)
{
    if (!m_pPlayer) return;
}

void CExtCharTitle::initTitleMap()
{
}

void CExtCharTitle::resetTitleMap()
{
    m_mCharTitle.clear();
}

void CExtCharTitle::HideActivityTitle(bool bCleanActivityTitle)
{
    m_bShowActivityTitle = 0;
    if (bCleanActivityTitle)
    {
        memset(m_vActivityTitle, 0, sizeof(m_vActivityTitle));
    }
}

bool CExtCharTitle::SetActivityTitle(int8_t nPos, int32_t nTitle)
{
    if (!m_pPlayer) return 0;
    if (nPos > 2) return 0;
    m_vActivityTitle[nPos] = nTitle;
    return 1;
}

int32_t CExtCharTitle::GetDressTitleId()
{
    for (int i = 0; i <= 2; ++i)
    {
        if (m_vDress[i]) return 1;
    }
    return 0;
}

void CExtCharTitle::OnLevelUp(int32_t nLevel)
{
    if (!m_pPlayer) return;
}

bool CExtCharTitle::isTitleHide(int32_t nId)
{
    return false;
}

void CExtCharTitle::hideTitle(int32_t nId)
{
}

void CExtCharTitle::unHideTitle(int32_t nId)
{
}

void CExtCharTitle::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtCharTitle::OnLoadFromDB(const PlayerDBData* dbData)
{
}

void CExtCharTitle::OnSaveToDB(PlayerDBData* dbData)
{
}