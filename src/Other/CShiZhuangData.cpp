#include "Other/CShiZhuangData.h"
#include "Network/NetPacket.h"
#include "Utility/StringUtility.h"
#include <sstream>
#include <cstring>
#include <cstdlib>

CShiZhuangData::CShiZhuangData()
{
    CleanUp();
}

CShiZhuangData::~CShiZhuangData()
{
}

void CShiZhuangData::CleanUp()
{
    for (int32_t i = 0; i < 3; ++i)
        m_vShiZhuang[i].CleanUp();
    m_ShiZhuangLevel.clear();
    m_EffectId = 0;
}

void CShiZhuangData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;
    for (int32_t i = 0; i < 3; ++i)
    {
        packet->writeInt32(m_vShiZhuang[i].m_nLevel);
        packet->writeInt32(m_vShiZhuang[i].m_nExp);
        packet->writeInt32(m_vShiZhuang[i].m_nWear);
        packet->writeInt32(static_cast<int32_t>(m_vShiZhuang[i].m_lActive.size()));
        for (auto &it : m_vShiZhuang[i].m_lActive)
        {
            packet->writeInt32(it.first);
            packet->writeInt32(it.second);
        }
    }
    packet->writeInt32(static_cast<int32_t>(m_ShiZhuangLevel.size()));
    for (auto &it : m_ShiZhuangLevel)
    {
        packet->writeInt32(it.first);
        packet->writeInt32(it.second);
    }
    packet->writeInt32(m_EffectId);
}

void CShiZhuangData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    if (!inPacket) return;
    for (int32_t i = 0; i < 3; ++i)
    {
        m_vShiZhuang[i].m_nLevel = inPacket->readInt32();
        m_vShiZhuang[i].m_nExp = inPacket->readInt32();
        m_vShiZhuang[i].m_nWear = inPacket->readInt32();
        int32_t activeSize = inPacket->readInt32();
        for (int32_t j = 0; j < activeSize; ++j)
        {
            int32_t nId = inPacket->readInt32();
            int32_t nTime = inPacket->readInt32();
            m_vShiZhuang[i].m_lActive[nId] = nTime;
        }
    }
    m_ShiZhuangLevel.clear();
    int32_t nSize = inPacket->readInt32();
    for (int32_t i = 0; i < nSize; ++i)
    {
        int32_t nId = inPacket->readInt32();
        int32_t nLevel = inPacket->readInt32();
        m_ShiZhuangLevel[nId] = nLevel;
    }
    m_EffectId = inPacket->readInt32();
}

std::string CShiZhuangData::GetShiZhuangLevelString()
{
    std::stringstream ss;
    for (auto &it : m_ShiZhuangLevel)
    {
        ss << it.first << ":" << it.second << "|";
    }
    return ss.str();
}

void CShiZhuangData::ParesShiZhuangLevelString(std::string *p_ShiZhuangLevel)
{
    if (p_ShiZhuangLevel->empty())
        return;

    std::vector<std::string> vt;
    std::string delims = "|";
    StringUtility::split(vt, *p_ShiZhuangLevel, delims);

    for (auto &it : vt)
    {
        std::vector<std::string> subVt;
        std::string subDelims = ":";
        StringUtility::split(subVt, it, subDelims);

        if (subVt.size() == 2)
        {
            int32_t nId = std::atoi(subVt[0].c_str());
            int32_t nLevel = std::atoi(subVt[1].c_str());
            m_ShiZhuangLevel[nId] = nLevel;
        }
    }
}