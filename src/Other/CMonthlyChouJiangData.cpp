#include "Other/CMonthlyChouJiangData.h"
#include "Network/NetPacket.h"

CMonthlyChouJiangData::CMonthlyChouJiangData()
{
    CleanUp();
}

CMonthlyChouJiangData::~CMonthlyChouJiangData()
{
}

void CMonthlyChouJiangData::CleanUp()
{
    m_Score = 0;
    m_AllScore = 0;
    m_GetItemList.clear();
    m_GetTimes.clear();
}

void CMonthlyChouJiangData::PackageData(Answer::NetPacket *packet)
{
    packet->writeInt32(m_Score);
    packet->writeInt32(m_AllScore);
    packet->writeInt32(static_cast<int32_t>(m_GetItemList.size()));
    for (auto &it : m_GetItemList)
        packet->writeInt32(it);
    packet->writeInt32(static_cast<int32_t>(m_GetTimes.size()));
    for (auto &it : m_GetTimes)
    {
        packet->writeInt32(it.first);
        packet->writeInt32(it.second);
    }
}

std::string CMonthlyChouJiangData::getItemListString() { return std::string(); }
void CMonthlyChouJiangData::ParesItemListString(std::string *p) { }
std::string CMonthlyChouJiangData::getScoreTime() { return std::string(); }
