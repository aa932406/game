#include "Other/CVipClub.h"
#include "Network/NetPacket.h"

#include <cstring>
#include <algorithm>
#include <list>

struct ClbAimCfg
{
    int32_t nNeedVipLevel;
    int32_t nNeedCount;
    int32_t nAddDropTimes;
};
typedef std::list<ClbAimCfg> ClbAimCfgList;

class GameService;

namespace Answer
{
    template<typename T>
    class Singleton
    {
    public:
        static T* instance();
    };

    class DBPool;
    class MySqlDBGuard;
    class MySqlQuery;
}

class CfgData
{
public:
    static ClbAimCfgList* GetClbAimCfg(CfgData* self);
    static CfgData* instance();
};

static bool GreaterClubData(const ClubData& left, const ClubData& right)
{
    return left.nVip7Time < right.nVip7Time;
}

CVipClub::CVipClub()
    : m_ClubLevel(0)
{
    m_ClubDataMap.clear();
    m_LevelCountMap.clear();
}

CVipClub::~CVipClub()
{
}

void CVipClub::Init(int32_t line)
{
    if (line == 9)
        return;

    m_ClubDataMap.clear();
    (void)line;
}

int32_t CVipClub::CalculationClubLevel()
{
    int32_t nLevel = 0;
    m_LevelCountMap.clear();

    CfgData* v1 = Answer::Singleton<CfgData>::instance();
    ClbAimCfgList* ClbAimCfgTable = CfgData::GetClbAimCfg(v1);
    if (!ClbAimCfgTable)
        return 1;

    for (ClbAimCfgList::const_iterator it = ClbAimCfgTable->begin(); it != ClbAimCfgTable->end(); ++it)
    {
        int32_t nCount = 0;
        int32_t nAddLevel = 0;

        for (std::map<int64_t, ClubData>::iterator iter = m_ClubDataMap.begin(); iter != m_ClubDataMap.end(); ++iter)
        {
            if (iter->second.nVipLevel >= it->nNeedVipLevel)
                ++nCount;

            if (it->nNeedCount <= nCount && nAddLevel <= 0)
                nAddLevel = it->nAddDropTimes;
        }

        m_LevelCountMap[it->nNeedVipLevel] = nCount;
        nLevel += nAddLevel;
    }

    return nLevel + 1;
}

void CVipClub::CalculationClubWallInfo()
{
    int32_t Vip10Time = 0;
    m_Vip7ClubDataVt.clear();

    for (std::map<int64_t, ClubData>::iterator it = m_ClubDataMap.begin(); it != m_ClubDataMap.end(); ++it)
    {
        if (it->second.nVipLevel > 9)
        {
            if (!Vip10Time || it->second.nVip10Time < Vip10Time)
            {
                m_Vip10ClubData = it->second;
                Vip10Time = it->second.nVip10Time;
            }
        }

        if (it->second.nVipLevel > 6)
        {
            m_Vip7ClubDataVt.push_back(it->second);
        }
    }

    if (m_Vip10ClubData.nCid > 0)
    {
        for (std::vector<ClubData>::iterator iter = m_Vip7ClubDataVt.begin(); iter != m_Vip7ClubDataVt.end(); ++iter)
        {
            if (iter->nCid == m_Vip10ClubData.nCid)
            {
                m_Vip7ClubDataVt.erase(iter);
                break;
            }
        }
    }

    if (!m_Vip7ClubDataVt.empty())
    {
        std::sort(m_Vip7ClubDataVt.begin(), m_Vip7ClubDataVt.end(), GreaterClubData);
    }
}

void CVipClub::AppendClubWallInfo(Answer::NetPacket* packet)
{
    if (!packet)
        return;

    if (m_Vip10ClubData.nCid > 0)
    {
        packet->writeInt32(1);
        packet->writeInt64(m_Vip10ClubData.nCid);
        packet->writeUTF8(m_Vip10ClubData.strName);
    }
    else
    {
        packet->writeInt32(0);
    }

    int32_t nSize = static_cast<int32_t>(m_Vip7ClubDataVt.size());
    int32_t v2 = (nSize <= 3) ? nSize : 3;
    nSize = v2;
    packet->writeInt32(v2);

    int32_t Count = 0;
    for (std::vector<ClubData>::iterator it = m_Vip7ClubDataVt.begin(); it != m_Vip7ClubDataVt.end(); ++it)
    {
        packet->writeInt64(it->nCid);
        packet->writeUTF8(it->strName);
        if (++Count > 2)
            break;
    }

    packet->writeInt32(m_ClubLevel);
    int32_t v5 = static_cast<int32_t>(m_LevelCountMap.size());
    packet->writeInt32(v5);

    for (std::map<int, int32_t>::iterator itInt = m_LevelCountMap.begin(); itInt != m_LevelCountMap.end(); ++itInt)
    {
        packet->writeInt32(itInt->first);
        packet->writeInt32(itInt->second);
    }
}
