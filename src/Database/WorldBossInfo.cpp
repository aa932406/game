#include "Database/WorldBossInfo.h"

WorldBossInfo::WorldBossInfo()
{
    KillerInfo *vKiller; // r13
    __int64 j; // r12
    int32_t i; // [rsp+1Ch] [rbp-24h]

    vKiller = this->vKiller;
    for ( j = 4; j != -1; --j )
    KillerInfo::KillerInfo(vKiller++);
    this->nBossId = 0;
    this->nLevel = 0;
    this->nExp = 0;
    this->nMid = 0;
    this->nBossType = 0;
    this->nMapId = 0;
    this->nReviveTime = 0;
    this->nState = 0;
    this->pPlant = 0;
    for ( i = 0; i <= 4; ++i )
    {
    this->vKiller[i].nCharId = 0;
    std::string::operator=(&this->vKiller[i].strName, byte_8CFE00);
    this->vKiller[i].nTime = 0;
    }
}

void WorldBossInfo::PackageBossInfo(const WorldBossInfo *const this, Answer::NetPacket *packet)
{
    int32_t i; // [rsp+1Ch] [rbp-4h]

    Answer::NetPacket::writeInt32(packet, this->nBossId);
    Answer::NetPacket::writeInt32(packet, this->nLevel);
    Answer::NetPacket::writeInt32(packet, this->nExp);
    Answer::NetPacket::writeInt32(packet, this->nMid);
    Answer::NetPacket::writeInt32(packet, this->nBossType);
    Answer::NetPacket::writeInt32(packet, this->nMapId);
    Answer::NetPacket::writeInt32(packet, this->nReviveTime);
    Answer::NetPacket::writeInt8(packet, this->nState);
    for ( i = 0; i <= 4; ++i )
    {
    Answer::NetPacket::writeInt64(packet, this->vKiller[i].nCharId);
    Answer::NetPacket::writeUTF8(packet, &this->vKiller[i].strName);
    Answer::NetPacket::writeInt32(packet, this->vKiller[i].nTime);
    }
}

void WorldBossInfo::AddKiller(const KillerInfo *const info)
{
    bool bFind; // [rsp+17h] [rbp-9h]
    int32_t i; // [rsp+18h] [rbp-8h]
    int32_t i_0; // [rsp+1Ch] [rbp-4h]

    bFind = 0;
    for ( i = 0; i <= 4; ++i )
    {
    if ( !this->vKiller[i].nCharId )
    {
    KillerInfo::operator=(&this->vKiller[i], info);
    bFind = 1;
    break;
    }
    }
    if ( !bFind )
    {
    for ( i_0 = 0; i_0 <= 3; ++i_0 )
    KillerInfo::operator=(&this->vKiller[i_0], &this->vKiller[i_0 + 1]);
    KillerInfo::operator=(&this->vKiller[4], info);
    }
}

void WorldBossInfo::PackageBossInfo(const WorldBossInfo *const this, Answer::NetPacket *packet, int32_t nNowTime)
{

    int32_t nCount; // [rsp+20h] [rbp-10h]
    int32_t nOldOffset; // [rsp+24h] [rbp-Ch]
    int32_t nNewOffset; // [rsp+28h] [rbp-8h]
    int32_t i; // [rsp+2Ch] [rbp-4h]

    Answer::NetPacket::writeInt32(packet, this->nBossId);
    Answer::NetPacket::writeInt32(packet, this->nMid);
    Answer::NetPacket::writeInt32(packet, this->nMapId);
    Answer::NetPacket::writeInt8(packet, this->nState);
    if ( this->nState )
    v3 = this->nReviveTime - nNowTime;
    else
    v3 = 0;
    Answer::NetPacket::writeInt32(packet, v3);
    nCount = 0;
    nOldOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::writeInt32(packet, 0);
    for ( i = 0; i <= 4; ++i )
    {
    if ( this->vKiller[i].nCharId > 0 )
    {
    Answer::NetPacket::writeInt64(packet, this->vKiller[i].nCharId);
    Answer::NetPacket::writeUTF8(packet, &this->vKiller[i].strName);
    Answer::NetPacket::writeInt32(packet, this->vKiller[i].nTime);
    ++nCount;
    }
    }
    Answer::NetPacket::writeInt32(packet, this->nLevel);
    Answer::NetPacket::writeInt32(packet, this->nExp);
    nNewOffset = Answer::NetPacket::getWOffset(packet);
    Answer::NetPacket::setWOffset(packet, nOldOffset);
    Answer::NetPacket::writeInt32(packet, nCount);
    Answer::NetPacket::setWOffset(packet, nNewOffset);
}

（内容由AI生成，仅供参考）
