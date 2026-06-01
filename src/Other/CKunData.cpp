#include "Other/CKunData.h"
#include "Network/NetPacket.h"
#include "Utility/StringUtility.h"
#include <sstream>
#include <cstring>
#include <cstdlib>

CKunData::CKunData()
{
    CleanUp();
}

CKunData::~CKunData()
{
}

void CKunData::CleanUp()
{
    m_nBuyTime = 0;
    m_nLianHuaTime = 0;
    m_nKunLevel = 0;
    m_nExp = 0;
    memset(m_DanTian, 0, sizeof(m_DanTian));
    memset(m_LingZhuBag, 0, sizeof(m_LingZhuBag));
    memset(m_LingZhuPos, 0, sizeof(m_LingZhuPos));
}

void CKunData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;
    packet->writeInt32(m_nBuyTime);
    packet->writeInt32(m_nLianHuaTime);
    packet->writeInt32(m_nKunLevel);
    packet->writeInt32(m_nExp);
    for (int32_t i = 0; i < 6; ++i)
    {
        packet->writeInt32(m_DanTian[i].nMid);
        packet->writeInt32(m_DanTian[i].nId);
        packet->writeInt32(m_DanTian[i].nEndTime);
    }
    for (int32_t j = 0; j < 100; ++j)
    {
        packet->writeInt32(m_LingZhuBag[j].nId);
        packet->writeInt32(m_LingZhuBag[j].nCount);
    }
    for (int32_t k = 0; k < 20; ++k)
        packet->writeInt32(m_LingZhuPos[k]);
}

void CKunData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    if (!inPacket) return;
    m_nBuyTime = inPacket->readInt32();
    m_nLianHuaTime = inPacket->readInt32();
    m_nKunLevel = inPacket->readInt32();
    m_nExp = inPacket->readInt32();
    for (int32_t i = 0; i < 6; ++i)
    {
        m_DanTian[i].nMid = inPacket->readInt32();
        m_DanTian[i].nId = inPacket->readInt32();
        m_DanTian[i].nEndTime = inPacket->readInt32();
    }
    for (int32_t j = 0; j < 100; ++j)
    {
        m_LingZhuBag[j].nId = inPacket->readInt32();
        m_LingZhuBag[j].nCount = inPacket->readInt32();
    }
    for (int32_t k = 0; k < 20; ++k)
        m_LingZhuPos[k] = inPacket->readInt32();
}

std::string CKunData::getLingZhuPosString()
{
    std::stringstream ss;
    for (int32_t i = 0; i < 20; ++i)
    {
        ss << i << ":" << m_LingZhuPos[i] << "|";
    }
    return ss.str();
}

void CKunData::ParesLingZhuPosString(std::string *p_LingZhuPos)
{
    if (p_LingZhuPos->empty())
        return;

    std::vector<std::string> vt;
    std::string delims = "|";
    StringUtility::split(vt, *p_LingZhuPos, delims);

    for (auto &it : vt)
    {
        std::vector<std::string> subVt;
        std::string subDelims = ":";
        StringUtility::split(subVt, it, subDelims);

        if (subVt.size() == 2)
        {
            int32_t nId = std::atoi(subVt[0].c_str());
            if (nId >= 0 && nId < 20)
                m_LingZhuPos[nId] = std::atoi(subVt[1].c_str());
        }
    }
}

std::string CKunData::getDanTianString()
{
    std::stringstream ss;
    for (int32_t i = 0; i < 6; ++i)
    {
        ss << i << ":" << m_DanTian[i].nMid << ":" << m_DanTian[i].nId << ":" << m_DanTian[i].nEndTime << "|";
    }
    return ss.str();
}

void CKunData::ParesDanTianString(std::string *p_DanTianString)
{
    if (p_DanTianString->empty())
        return;

    std::vector<std::string> vt;
    std::string delims = "|";
    StringUtility::split(vt, *p_DanTianString, delims);

    for (auto &it : vt)
    {
        std::vector<std::string> subVt;
        std::string subDelims = ":";
        StringUtility::split(subVt, it, subDelims);

        if (subVt.size() == 4)
        {
            int32_t nId = std::atoi(subVt[0].c_str());
            if (nId >= 0 && nId < 6)
            {
                m_DanTian[nId].nMid = std::atoi(subVt[1].c_str());
                m_DanTian[nId].nId = std::atoi(subVt[2].c_str());
                m_DanTian[nId].nEndTime = std::atoi(subVt[3].c_str());
            }
        }
    }
}