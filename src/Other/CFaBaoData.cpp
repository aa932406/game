#include "Other/CFaBaoData.h"
#include "Network/NetPacket.h"
#include "Game/CFaBao.h"
#include <cstring>
#include <sstream>

CFaBaoData::CFaBaoData()
{
    CleanUp();
}

CFaBaoData::~CFaBaoData()
{
}

void CFaBaoData::CleanUp()
{
    memset(m_FaBao, 0, sizeof(m_FaBao));
}

void CFaBaoData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt8(4);
    for (int32_t i = 0; i < 4; ++i)
        packet->writeInt8(i);
}

void CFaBaoData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    int8_t nSize = inPacket->readInt8();
    for (int32_t i = 0; i < nSize; ++i)
        inPacket->readInt8();
}

std::string CFaBaoData::GetFaBaoLevelString()
{
    std::ostringstream ss;
    FaBaoInfo* info = reinterpret_cast<FaBaoInfo*>(m_FaBao);
    for (int32_t i = 0; i <= 3; ++i)
    {
        ss << i << ":" << info[i].FaBaoLevel << ":" << info[i].FaBaoRes << "|";
    }
    return ss.str();
}

void CFaBaoData::ParesFaBaoLevelString(std::string *p_FaBaoLevelString)
{
    if (p_FaBaoLevelString->empty())
        return;

    std::string delims = "|";
    std::vector<std::string> vStr;
    StringUtility::split(vStr, *p_FaBaoLevelString, delims);

    FaBaoInfo* info = reinterpret_cast<FaBaoInfo*>(m_FaBao);
    for (size_t i = 0; i < vStr.size(); ++i)
    {
        std::string delims2 = ":";
        std::vector<std::string> tv;
        StringUtility::split(tv, vStr[i], delims2);
        if (tv.size() == 3)
        {
            int32_t nType = atoi(tv[0].c_str());
            if (CFaBao::IsFaBaoType(nType))
            {
                info[nType].FaBaoLevel = atoi(tv[1].c_str());
                info[nType].FaBaoRes = atoi(tv[2].c_str());
            }
        }
    }
}
