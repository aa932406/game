#include "Other/TencentInfo.h"
#include "Network/NetPacket.h"

TencentInfo::TencentInfo()
{
    CleanUp();
}

TencentInfo::TencentInfo(const TencentInfo& other)
    : m_pf(other.m_pf)
    , m_bYellowVip(other.m_bYellowVip)
    , m_bYellowYearVip(other.m_bYellowYearVip)
    , m_bYellowHighVip(other.m_bYellowHighVip)
    , m_nYellowVipLevel(other.m_nYellowVipLevel)
    , m_bBlueVip(other.m_bBlueVip)
    , m_bBlueYearVip(other.m_bBlueYearVip)
    , m_bBlueHighVip(other.m_bBlueHighVip)
    , m_nBlueVipLevel(other.m_nBlueVipLevel)
    , m_nYearVipEndTime(other.m_nYearVipEndTime)
    , m_nSuperVipEndTime(other.m_nSuperVipEndTime)
    , m_nExpendVipEndTime(other.m_nExpendVipEndTime)
{
}

TencentInfo& TencentInfo::operator=(const TencentInfo& other)
{
    if (this != &other)
    {
        m_pf = other.m_pf;
        m_bYellowVip = other.m_bYellowVip;
        m_bYellowYearVip = other.m_bYellowYearVip;
        m_bYellowHighVip = other.m_bYellowHighVip;
        m_nYellowVipLevel = other.m_nYellowVipLevel;
        m_bBlueVip = other.m_bBlueVip;
        m_bBlueYearVip = other.m_bBlueYearVip;
        m_bBlueHighVip = other.m_bBlueHighVip;
        m_nBlueVipLevel = other.m_nBlueVipLevel;
        m_nYearVipEndTime = other.m_nYearVipEndTime;
        m_nSuperVipEndTime = other.m_nSuperVipEndTime;
        m_nExpendVipEndTime = other.m_nExpendVipEndTime;
    }
    return *this;
}

void TencentInfo::CleanUp()
{
    m_pf.clear();
    m_bYellowVip = 0;
    m_bYellowYearVip = 0;
    m_bYellowHighVip = 0;
    m_nYellowVipLevel = 0;
    m_bBlueVip = 0;
    m_bBlueYearVip = 0;
    m_bBlueHighVip = 0;
    m_nBlueVipLevel = 0;
    m_nYearVipEndTime = 0;
    m_nSuperVipEndTime = 0;
    m_nExpendVipEndTime = 0;
}

void TencentInfo::UnPacketInfo(Answer::NetPacket* packet)
{
    std::string str;
    packet->readUTF8(str);
    m_pf = str;
    m_bYellowVip = packet->readInt8();
    m_bYellowYearVip = packet->readInt8();
    m_bYellowHighVip = packet->readInt8();
    m_nYellowVipLevel = packet->readInt8();
    m_bBlueVip = packet->readInt8();
    m_bBlueYearVip = packet->readInt8();
    m_bBlueHighVip = packet->readInt8();
    m_nBlueVipLevel = packet->readInt8();
    m_nYearVipEndTime = packet->readInt32();
    m_nSuperVipEndTime = packet->readInt32();
    m_nExpendVipEndTime = packet->readInt32();
}

int32_t TencentInfo::GetEndTime() const
{
    if (m_nYearVipEndTime <= m_nSuperVipEndTime)
    {
        if (m_nSuperVipEndTime <= m_nExpendVipEndTime)
            return m_nExpendVipEndTime;
        else
            return m_nSuperVipEndTime;
    }
    else if (m_nYearVipEndTime <= m_nExpendVipEndTime)
    {
        return m_nExpendVipEndTime;
    }
    else
    {
        return m_nYearVipEndTime;
    }
}

bool TencentInfo::isYellowEnter() const
{
    return m_pf == "qzone" || m_pf == "pengyou";
}

bool TencentInfo::isBlueEnter() const
{
    return m_pf == "qqgame" || m_pf == "3366";
}

bool TencentInfo::isTGPEnter() const
{
    return m_pf == "union-10153-5" || m_pf == "qqgame";
}

int32_t TencentInfo::getTGPIcon() const
{
    if (m_pf == "union-10153-5")
        return 79;
    if (m_pf == "qqgame")
        return 81;
    return -1;
}
