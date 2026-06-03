#include "common.h"
#include "Other/FamilyManager.h"

#include <cstring>

FamilyManager::FamilyManager()
{
}

FamilyManager::~FamilyManager()
{
}

void FamilyManager::Init(int32_t line)
{
    if (line == 9)
        return;

    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    char szSQL[4096];
    memset(szSQL, 0, sizeof(szSQL));
    snprintf(szSQL, sizeof(szSQL), "SELECT * FROM `mem_family` WHERE `delflag`=0");
    Answer::MySqlQuery *pQuery = Answer::MySqlDBGuard::query(&db, szSQL);
    Answer::MySqlQuery result(pQuery);

    CTimer *pTimer = CTimer::instance();
    int32_t nNowTime = CTimer::GetNow(pTimer);

    while (!result.eof())
    {
        FamilyInfo info;
        info.nId = result.getInt64Value("id", 0);
        std::string strName = result.getStringValue("name", "");
        info.strName = strName;
        info.nLevel = result.getIntValue("level", 0);
        info.nMemberCard = result.getIntValue("member_card", 0);
        info.FamilyLeaderCid = result.getIntValue("leader_cid", 0);
        info.MedalLevel = result.getIntValue("medl_level", 0);
        info.Territory = result.getIntValue("territory_open", 0);

        memset(szSQL, 0, sizeof(szSQL));
        snprintf(szSQL, sizeof(szSQL), "SELECT name FROM `mem_character` WHERE `cid`=%lld", info.FamilyLeaderCid);
        Answer::MySqlQuery *pLeaderQuery = Answer::MySqlDBGuard::query(&db, szSQL);
        Answer::MySqlQuery leaderResult(pLeaderQuery);
        if (!leaderResult.eof())
        {
            std::string leaderName = leaderResult.getStringValue("name", "");
            info.FamilyLeaderName = leaderName;
        }

        m_mFamilyInfo[info.nId] = info;
        result.nextRow();
    }

    Answer::MySqlQuery *pDungeonQuery = Answer::MySqlDBGuard::query(&db, "SELECT * FROM `mem_family_dungeon`");
    result = Answer::MySqlQuery(pDungeonQuery);
    while (!result.eof())
    {
        FamilyId_t nFamilyId = result.getInt64Value("id", 0);
        FamilyDungeonStu stu;
        stu.StartCounts = result.getIntValue("times", 0);
        stu.StartTime = result.getIntValue("start_time", 0);
        stu.nMaxHard = result.getIntValue("finish_hard", 0);
        m_FamilyDungeonStuMap[nFamilyId] = stu;
        result.nextRow();
    }
}

void FamilyManager::OnUpdateFamilyInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    int8_t nReason = inPacket->readInt8();
    FamilyInfo info;
    info.UnPackageData(inPacket);

    switch (nReason)
    {
    case 1:
        addFamilyInfo(&info);
        break;
    case 2:
        updateFamilyInfo(&info);
        break;
    case 3:
        deleteFamilyInfo(info.nId);
        break;
    }
}

void FamilyManager::OnInitFamilyInfo(Answer::NetPacket *inPacket)
{
    if (!inPacket)
        return;

    FamilyInfo info;
    int32_t nSize = inPacket->readInt32();
    for (int32_t i = 0; i < nSize; i++)
    {
        info.CleanUp();
        info.UnPackageData(inPacket);
        addFamilyInfo(&info);
    }
}

void FamilyManager::addFamilyInfo(FamilyInfo *info)
{
    if (info->nId == 0 && info->strName.empty())
        return;

    Answer::MutexGuard lock(&m_Lock);
    m_mFamilyInfo[info->nId] = *info;
}

void FamilyManager::updateFamilyInfo(FamilyInfo *info)
{
    if (info->nId == 0 && info->strName.empty())
        return;

    Answer::MutexGuard lock(&m_Lock);
    auto it = m_mFamilyInfo.find(info->nId);
    if (it != m_mFamilyInfo.end())
        it->second = *info;
}

void FamilyManager::deleteFamilyInfo(FamilyId_t nFamilyId)
{
    Answer::MutexGuard lock(&m_Lock);
    m_mFamilyInfo.erase(nFamilyId);
}

int8_t FamilyManager::GetTerritoryState(FamilyId_t nFamilyId)
{
    Answer::MutexGuard lock(&m_Lock);
    auto it = m_mFamilyInfo.find(nFamilyId);
    if (it != m_mFamilyInfo.end())
        return it->second.Territory;
    return 0;
}

int32_t FamilyManager::GetMedlLevel(FamilyId_t nFamilyId)
{
    Answer::MutexGuard lock(&m_Lock);
    auto it = m_mFamilyInfo.find(nFamilyId);
    if (it != m_mFamilyInfo.end())
        return it->second.MedalLevel;
    return 0;
}

Int64List FamilyManager::GetTop10Family()
{
    Answer::MutexGuard lock(&m_Lock);
    std::vector<FamilyInfo> vFamily;
    vFamily.reserve(m_mFamilyInfo.size());
    for (auto &pair : m_mFamilyInfo)
        vFamily.push_back(pair.second);

    std::sort(vFamily.begin(), vFamily.end(),
        [](const FamilyInfo &a, const FamilyInfo &b) { return a.nBossPoints > b.nBossPoints; });

    Int64List result;
    int32_t nCount = 0;
    for (auto &info : vFamily)
    {
        if (nCount >= 10)
            break;
        result.push_back(info.nId);
        nCount++;
    }
    return result;
}

void FamilyManager::SaveFamilyDungeonStuMap(FamilyId_t nFamilyId)
{
    Answer::DBPool *pDB = Answer::DBPool::instance();
    Answer::MySqlDBGuard db(pDB);
    char szSQL[4096];

    auto it = m_FamilyDungeonStuMap.find(nFamilyId);
    if (it != m_FamilyDungeonStuMap.end())
    {
        snprintf(szSQL, sizeof(szSQL),
            "REPLACE INTO `mem_family_dungeon` (`id`, `times`, `start_time`, `finish_hard`) VALUES (%lld, %d, %d, %d)",
            nFamilyId, it->second.StartCounts, it->second.StartTime, it->second.nMaxHard);
    }
    else
    {
        snprintf(szSQL, sizeof(szSQL),
            "REPLACE INTO `mem_family_dungeon` (`id`, `times`, `start_time`, `finish_hard`) VALUES (%lld, 0, 0, 0)",
            nFamilyId);
    }
    Answer::MySqlDBGuard::query(&db, szSQL);
}

FamilyDungeonStu *FamilyManager::GetFamilyDungeonInfo(FamilyId_t nFamilyId)
{
    auto it = m_FamilyDungeonStuMap.find(nFamilyId);
    if (it != m_FamilyDungeonStuMap.end())
        return &it->second;
    return nullptr;
}

Dungeon *FamilyManager::GetFamilyDungeon(FamilyId_t nFamilyId)
{
    auto it = m_FamilyDungeonMap.find(nFamilyId);
    if (it != m_FamilyDungeonMap.end())
        return it->second;
    return nullptr;
}

void FamilyManager::DeleteFamilyDungeon(FamilyId_t nFamilyId)
{
    Answer::MutexGuard lock(&m_Lock);
    auto it = m_FamilyDungeonMap.find(nFamilyId);
    if (it != m_FamilyDungeonMap.end())
    {
        m_FamilyDungeonMap.erase(it);
    }
    SaveFamilyDungeonStuMap(nFamilyId);
}

void FamilyManager::SetFamilyDungeonHard(FamilyId_t nFamilyId, int32_t Hard)
{
    Answer::MutexGuard lock(&m_Lock);
    auto it = m_FamilyDungeonStuMap.find(nFamilyId);
    if (it != m_FamilyDungeonStuMap.end())
    {
        if (it->second.nMaxHard < Hard)
            it->second.nMaxHard = Hard;
    }
    SaveFamilyDungeonStuMap(nFamilyId);
}
