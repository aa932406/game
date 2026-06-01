#include "Other/CLittleHelperData.h"
#include "Network/NetPacket.h"
#include "Utility/StringUtility.h"
#include <sstream>
#include <cstring>
#include <cstdlib>

CLittleHelperData::CLittleHelperData()
{
    CleanUp();
}

CLittleHelperData::~CLittleHelperData()
{
}

void CLittleHelperData::CleanUp()
{
    m_LittleHelperId = 0;
    m_ActLittleHelperInfoMap.clear();
}

std::string CLittleHelperData::getRewardState()
{
    std::stringstream ss;
    for (auto &it : m_ActLittleHelperInfoMap)
    {
        ss << it.first << ":" << it.second.nSkin << ":" << it.second.nTimes << "|";
    }
    return ss.str();
}

void CLittleHelperData::SaveRewardState(std::string *p_String)
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

        if (subVt.size() == 3)
        {
            ActLittleHelperInfo rec;
            rec.nId = std::atoi(subVt[0].c_str());
            rec.nSkin = std::atoi(subVt[1].c_str());
            rec.nTimes = std::atoi(subVt[2].c_str());
            m_ActLittleHelperInfoMap[rec.nId] = rec;
        }
    }
}

void CLittleHelperData::PackageData(Answer::NetPacket *packet)
{
    if (!packet) return;
    packet->writeInt32(m_LittleHelperId);
    packet->writeInt32(static_cast<int32_t>(m_ActLittleHelperInfoMap.size()));
    for (auto &it : m_ActLittleHelperInfoMap)
    {
        packet->writeInt32(it.first);
        packet->writeInt32(it.second.nSkin);
        packet->writeInt32(it.second.nTimes);
    }
}

void CLittleHelperData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    if (!inPacket) return;
    m_LittleHelperId = inPacket->readInt32();
    m_ActLittleHelperInfoMap.clear();
    int32_t nSize = inPacket->readInt32();
    for (int32_t i = 0; i < nSize; ++i)
    {
        ActLittleHelperInfo rec;
        rec.nId = inPacket->readInt32();
        rec.nSkin = inPacket->readInt32();
        rec.nTimes = inPacket->readInt32();
        m_ActLittleHelperInfoMap[rec.nId] = rec;
    }
}