// CExtMagicBox.cpp
#include "CExtMagicBox.h"
#include "Game/GameService.h"
#include "Network/NetPacket.h"
#include "Game/Player.h"
#include "Config/CfgData.h"
#include <cstring>

CExtMagicBox::CExtMagicBox()
    : CExtSystemBase()
    , m_nCombiPoints(0)
    , m_nLastReviveTime(0)
{
    OnCleanUp();
}

CExtMagicBox::~CExtMagicBox()
{
}

void CExtMagicBox::OnCleanUp()
{
    m_nCombiPoints = 0;
    m_nLastReviveTime = 0;
    m_lActiveScrolls.clear();
    m_lSuccessIds.clear();
}

void CExtMagicBox::OnUpdate(int64_t curTick)
{
    if (!m_pPlayer) return;

    int32_t nMaxPoints = getMaxPoints();
    if (m_nCombiPoints < nMaxPoints)
    {
        int32_t nReviveValue = getPointsReviveValue();
        if (nReviveValue > 0)
        {
            m_nCombiPoints += nReviveValue;
            if (m_nCombiPoints > nMaxPoints)
                m_nCombiPoints = nMaxPoints;
        }
    }
}

void CExtMagicBox::OnDaySwitch(int32_t nDiffDays)
{
}

void CExtMagicBox::OnLoadFromDB(const PlayerDBData* dbData)
{
    if (dbData)
    {
        // Load magic box data from DB
    }
}

void CExtMagicBox::OnSaveToDB(PlayerDBData* dbData)
{
    if (dbData && m_pPlayer)
    {
        // Save magic box data to DB
    }
}

void CExtMagicBox::GetInterestsProtocol(ProcIdList* procList)
{
    if (!procList) return;
}

int32_t CExtMagicBox::DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket* inPacket)
{
    if (!inPacket || !m_pPlayer) return -1;

    switch (nProcId)
    {
        case 0:
            return onRequestInfo(inPacket);
        case 1:
            return onItemDecompose(inPacket);
        case 2:
            return onItemCombi(inPacket);
        case 3:
            return onOrnamentCombi(inPacket);
        case 4:
            return onEquipCombi(inPacket);
        default:
            break;
    }
    return -1;
}

int32_t CExtMagicBox::onRequestInfo(Answer::NetPacket* inPacket)
{
    SendMagicBoxInfo();
    return 0;
}

int32_t CExtMagicBox::onItemDecompose(Answer::NetPacket* inPacket)
{
    return 0;
}

int32_t CExtMagicBox::onItemCombi(Answer::NetPacket* inPacket)
{
    return 0;
}

int32_t CExtMagicBox::onOrnamentCombi(Answer::NetPacket* inPacket)
{
    return 0;
}

int32_t CExtMagicBox::onEquipCombi(Answer::NetPacket* inPacket)
{
    return 0;
}

int32_t CExtMagicBox::randomIndex(const void* vParam)
{
    return 0;
}

bool CExtMagicBox::AddPoints(int32_t nValue)
{
    if (nValue <= 0) return false;

    m_nCombiPoints += nValue;
    int32_t nMaxPoints = getMaxPoints();
    if (m_nCombiPoints > nMaxPoints)
        m_nCombiPoints = nMaxPoints;

    SendItemCombiPoint();
    return true;
}

bool CExtMagicBox::ActiveScroll(int32_t nId)
{
    if (isActive(nId)) return false;

    m_lActiveScrolls.push_back(nId);
    return true;
}

void CExtMagicBox::SendMagicBoxInfo()
{
    if (!m_pPlayer) return;
}

void CExtMagicBox::SendItemCombiPoint()
{
    if (!m_pPlayer) return;
}

void CExtMagicBox::broadcastItemCombi(int32_t ItemId, int8_t ItemClass)
{
}

void CExtMagicBox::sendItemCombiResult(int32_t nId, int32_t nSuccess, int32_t nFail)
{
}

int32_t CExtMagicBox::getMaxPoints()
{
    return 1000;
}

int32_t CExtMagicBox::getPointsReviveValue()
{
    return 1;
}

bool CExtMagicBox::isActive(int32_t nId)
{
    for (auto it = m_lActiveScrolls.begin(); it != m_lActiveScrolls.end(); ++it)
    {
        if (*it == nId) return true;
    }
    return false;
}

std::string CExtMagicBox::saveString(const void* inList)
{
    return "";
}

void CExtMagicBox::loadString(void* outList, const std::string* inStr)
{
}
