#include "Other/CHuoYueDuData.h"
#include "Network/NetPacket.h"
#include "Utility/StringUtility.h"
#include <sstream>
#include <cstring>
#include <cstdlib>

CHuoYueDuData::CHuoYueDuData()
{
}

CHuoYueDuData::~CHuoYueDuData()
{
}

void CHuoYueDuData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(static_cast<int32_t>(m_HuoYueDuRecord.size()));
    for (auto &it : m_HuoYueDuRecord)
    {
        packet->writeInt32(it.first);
        packet->writeInt32(it.second.state);
        packet->writeInt8(it.second.day);
    }
}

void CHuoYueDuData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    m_HuoYueDuRecord.clear();
    int32_t nSize = inPacket->readInt32();
    for (int32_t i = 0; i < nSize; ++i)
    {
        int32_t key = inPacket->readInt32();
        HuoYueDuRecord rec;
        rec.state = inPacket->readInt32();
        rec.day = inPacket->readInt8();
        m_HuoYueDuRecord[key] = rec;
    }
}

void CHuoYueDuData::InitHuoYueDu(std::string *p_HuoYueDuString)
{
    if (p_HuoYueDuString->empty())
        return;

    std::vector<std::string> vt;
    std::string delims = "|";
    StringUtility::split(vt, *p_HuoYueDuString, delims);

    for (auto &it : vt)
    {
        std::vector<std::string> subVt;
        std::string subDelims = ":";
        StringUtility::split(subVt, it, subDelims);

        if (subVt.size() == 3)
        {
            HuoYueDuRecord rec;
            int32_t key = std::atoi(subVt[0].c_str());
            rec.state = std::atoi(subVt[1].c_str());
            rec.day = static_cast<int8_t>(std::atoi(subVt[2].c_str()));
            m_HuoYueDuRecord[key] = rec;
        }
    }
}

std::string CHuoYueDuData::GetHuoYueDuString()
{
    std::stringstream ss;
    for (auto &it : m_HuoYueDuRecord)
    {
        ss << it.first << ":" << it.second.state << ":" << static_cast<int32_t>(it.second.day) << "|";
    }
    return ss.str();
}