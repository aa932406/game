#include "common.h"
#include "Other/CRankMirror.h"

#include <cstring>

CRankMirror::CRankMirror()
{
}

CRankMirror::~CRankMirror()
{
}

int32_t CRankMirror::GetBattleIndex(CharId_t nCharId)
{
    Answer::MutexGuard lock(&m_lock);
    auto it = m_mPlayerRank.find(nCharId);
    if (it != m_mPlayerRank.end())
        return it->second.nBattleIndex;
    return 0;
}

int32_t CRankMirror::GetLevelIndex(CharId_t nCharId)
{
    Answer::MutexGuard lock(&m_lock);
    auto it = m_mPlayerRank.find(nCharId);
    if (it != m_mPlayerRank.end())
        return it->second.nLevelIndex;
    return 0;
}

bool CRankMirror::IsJobBattleFirst(CharId_t nCharId)
{
    Answer::MutexGuard lock(&m_lock);
    auto it = m_mPlayerRank.find(nCharId);
    return it != m_mPlayerRank.end() && it->second.nJobBattleFirst > 0;
}

int32_t CRankMirror::GetWeiWangIndex(CharId_t nCharId)
{
    Answer::MutexGuard lock(&m_lock);
    auto it = m_mPlayerRank.find(nCharId);
    if (it != m_mPlayerRank.end())
        return it->second.nWeiWangIndex;
    return 0;
}

void CRankMirror::OnSocialUpdateRank(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    int8_t nType = inPacket->readInt8();
    if (nType == 2)
    {
        resetLevelRank();
        int16_t nSize = inPacket->readInt16();
        for (int32_t i = 0; i < nSize; i++)
        {
            CharId_t nCharId = inPacket->readInt64();
            int32_t nIndex = inPacket->readInt8();
            setLevelRank(nCharId, nIndex);
        }
    }
    else if (nType == 4)
    {
        resetWeiWangRank();
        int16_t nSize = inPacket->readInt16();
        for (int32_t i = 0; i < nSize; i++)
        {
            CharId_t nCharId = inPacket->readInt64();
            int32_t nIndex = inPacket->readInt8();
            setWeiWangRank(nCharId, nIndex);
        }
    }
    else if (nType == 1)
    {
        m_nLastRefreshTime = inPacket->readInt32();
        resetBattleRank();
        int16_t nSize = inPacket->readInt16();
        for (int32_t i = 0; i < nSize; i++)
        {
            CharId_t nCharId = inPacket->readInt64();
            int32_t nIndex = inPacket->readInt8();
            int8_t nIsJobFirst = inPacket->readInt8();
            setBattleRank(nCharId, nIndex, nIsJobFirst);
        }
    }
}

void CRankMirror::resetBattleRank()
{
    Answer::MutexGuard lock(&m_lock);
    memset(m_vBattleRank, 0, sizeof(m_vBattleRank));
    for (auto &pair : m_mPlayerRank)
    {
        pair.second.nBattleIndex = 0;
        if (pair.second.nJobBattleFirst > 0)
        {
            GameService::onRemoveTitle(GameService::instance(), pair.second.nCharId, 3);
            pair.second.nJobBattleFirst = 0;
        }
    }
}

void CRankMirror::resetLevelRank()
{
    Answer::MutexGuard lock(&m_lock);
    memset(m_vLevelRank, 0, sizeof(m_vLevelRank));
    for (auto &pair : m_mPlayerRank)
        pair.second.nLevelIndex = 0;
}

void CRankMirror::resetWeiWangRank()
{
    Answer::MutexGuard lock(&m_lock);
    memset(m_vWeiWangRank, 0, sizeof(m_vWeiWangRank));
    for (auto &pair : m_mPlayerRank)
        pair.second.nWeiWangIndex = 0;
}

void CRankMirror::setBattleRank(CharId_t nCharId, int32_t nIndex, int8_t nJobFrist)
{
    Answer::MutexGuard lock(&m_lock);
    auto it = m_mPlayerRank.find(nCharId);
    if (it != m_mPlayerRank.end())
    {
        it->second.nBattleIndex = nIndex;
        it->second.nJobBattleFirst = nJobFrist;
    }
    else
    {
        PlayerRankData data;
        memset(&data, 0, sizeof(data));
        data.nCharId = nCharId;
        data.nBattleIndex = nIndex;
        data.nJobBattleFirst = nJobFrist;
        m_mPlayerRank[nCharId] = data;
    }

    if (nJobFrist > 0)
        GameService::onCheckTitle(GameService::instance(), nCharId, 3, nJobFrist);

    if (nIndex > 0 && nIndex <= 100)
        m_vBattleRank[nIndex] = nCharId;
}

void CRankMirror::setLevelRank(CharId_t nCharId, int32_t nIndex)
{
    Answer::MutexGuard lock(&m_lock);
    auto it = m_mPlayerRank.find(nCharId);
    if (it != m_mPlayerRank.end())
    {
        it->second.nLevelIndex = nIndex;
    }
    else
    {
        PlayerRankData data;
        memset(&data, 0, sizeof(data));
        data.nCharId = nCharId;
        data.nLevelIndex = nIndex;
        m_mPlayerRank[nCharId] = data;
    }

    if (nIndex > 0 && nIndex <= 100)
        m_vLevelRank[nIndex] = nCharId;
}

void CRankMirror::setWeiWangRank(CharId_t nCharId, int32_t nIndex)
{
    Answer::MutexGuard lock(&m_lock);
    auto it = m_mPlayerRank.find(nCharId);
    if (it != m_mPlayerRank.end())
    {
        it->second.nWeiWangIndex = nIndex;
    }
    else
    {
        PlayerRankData data;
        memset(&data, 0, sizeof(data));
        data.nCharId = nCharId;
        data.nWeiWangIndex = nIndex;
        m_mPlayerRank[nCharId] = data;
    }

    if (nIndex > 0 && nIndex <= 100)
        m_vWeiWangRank[nIndex] = nCharId;
}
