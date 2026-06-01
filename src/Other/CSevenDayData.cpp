#include "Other/CSevenDayData.h"
#include "Network/NetPacket.h"
#include "Utility/StringUtility.h"
#include <sstream>
#include <cstring>
#include <cstdlib>

CSevenDayData::CSevenDayData()
{
    CleanUp();
}

CSevenDayData::~CSevenDayData()
{
}

void CSevenDayData::CleanUp()
{
    m_OpenTime = 0;
    m_RewardState.clear();
    m_SumRewardState = 0;
}

std::string CSevenDayData::getRewardState()
{
    std::stringstream ss;
    for (auto &it : m_RewardState)
    {
        ss << it.first << ":" << it.second << "|";
    }
    return ss.str();
}

void CSevenDayData::SaveRewardState(std::string *p_String)
{
    if (p_String->empty())
        return;

    std::vector<std::string> vt;
    std::string delims = "|";
    StringUtility::split(vt, *p_String, delims);

    for (auto &it : vt)
    {
        std::vector<std::string> subVt;
        std::string subDelims = ":";
        StringUtility::split(subVt, it, subDelims);

        if (subVt.size() == 2)
        {
            int32_t nId = std::atoi(subVt[0].c_str());
            m_RewardState[nId] = std::atoi(subVt[1].c_str());
        }
    }
}

void CSevenDayData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;
    packet->writeInt32(static_cast<int32_t>(m_RewardState.size()));
    for (auto &it : m_RewardState)
    {
        packet->writeInt32(it.first);
        packet->writeInt32(it.second);
    }
    packet->writeInt32(m_OpenTime);
    packet->writeInt32(m_SumRewardState);
}

void CSevenDayData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    if (!inPacket) return;
    m_RewardState.clear();
    int32_t nSize = inPacket->readInt32();
    for (int32_t i = 0; i < nSize; ++i)
    {
        int32_t nId = inPacket->readInt32();
        int32_t nState = inPacket->readInt32();
        m_RewardState[nId] = nState;
    }
    m_OpenTime = inPacket->readInt32();
    m_SumRewardState = inPacket->readInt32();
}