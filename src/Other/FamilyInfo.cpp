#include "Other/FamilyInfo.h"

#include <cstring>

FamilyInfo::FamilyInfo()
{
    CleanUp();
}

FamilyInfo::~FamilyInfo()
{
}

void FamilyInfo::CleanUp()
{
    nId = 0;
    nLevel = 0;
    nMemberCard = 0;
    strName.clear();
    FamilyTaskCount = 0;
    nBossPoints = 0;
    FamilyLeaderCid = 0;
    FamilyLeaderName.clear();
    DeclareFamilies.clear();
}

bool FamilyInfo::IsDeclareWarFamily(FamilyId_t nFamilyId)
{
    return DeclareFamilies.find(nFamilyId) != DeclareFamilies.end();
}

void FamilyInfo::UnPackageData(Answer::NetPacket *packet)
{
    if (!packet)
        return;

    nId = packet->readInt64();
    std::string val = packet->readUTF8();
    strName = val;
    nLevel = packet->readInt8();
    nMemberCard = packet->readInt32();
    FamilyTaskCount = packet->readInt32();
    nBossPoints = packet->readInt32();
    FamilyLeaderCid = packet->readInt64();
    std::string leaderName = packet->readUTF8();
    FamilyLeaderName = leaderName;
    int32_t nWarSize = packet->readInt32();
    for (int32_t i = 0; i < nWarSize; i++)
    {
        FamilyId_t nFid = packet->readInt64();
        DeclareFamilies.insert(nFid);
    }
}

FamilyInfo::FamilyInfo(const FamilyInfo *const a2)
{
    nId = a2->nId;
    nLevel = a2->nLevel;
    nMemberCard = a2->nMemberCard;
    strName = a2->strName;
    FamilyTaskCount = a2->FamilyTaskCount;
    nBossPoints = a2->nBossPoints;
    FamilyLeaderCid = a2->FamilyLeaderCid;
    FamilyLeaderName = a2->FamilyLeaderName;
    DeclareFamilies = a2->DeclareFamilies;
}
