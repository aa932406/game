#include "Other/CMingGeData.h"
#include "Network/NetPacket.h"
#include "Utility/StringUtility.h"
#include <sstream>
#include <cstring>
#include <cstdlib>

CMingGeData::CMingGeData()
{
    CleanUp();
}

CMingGeData::~CMingGeData()
{
}

void CMingGeData::CleanUp()
{
    m_MingGeExp = 0;
    m_MingChip = 0;
    memset(m_MingGeBag, 0, sizeof(m_MingGeBag));
    memset(m_LieMingBag, 0, sizeof(m_LieMingBag));
    memset(m_MingGeEquip, 0, sizeof(m_MingGeEquip));
    memset(m_MingGeNpc, 0, sizeof(m_MingGeNpc));
}

std::string CMingGeData::GetNpcString()
{
    std::stringstream ss;
    for (int32_t i = 0; i < 5; ++i)
    {
        ss << i << ":" << m_MingGeNpc[i] << "|";
    }
    return ss.str();
}

void CMingGeData::parseMingGeNpc(std::string *p_NpcString)
{
    if (p_NpcString->empty())
        return;

    std::vector<std::string> vt;
    std::string delims = "|";
    StringUtility::split(vt, *p_NpcString, delims);

    int32_t iSize = static_cast<int32_t>(vt.size());
    for (int32_t i = 0; i < iSize; ++i)
    {
        std::vector<std::string> subVt;
        std::string subDelims = ":";
        StringUtility::split(subVt, vt[i], subDelims);

        if (subVt.size() == 2)
        {
            int32_t nId = std::atoi(subVt[0].c_str());
            if (nId >= 0 && nId < 5)
                m_MingGeNpc[nId] = std::atoi(subVt[1].c_str());
        }
    }
}

void CMingGeData::PackageData(Answer::NetPacket *packet)
{
    int32_t slotSaveOffset = packet->getWOffset();
    int32_t slotCount = 0;
    packet->writeInt32(0);
    for (int32_t slot = 0; slot < 100; ++slot)
    {
        if (m_MingGeBag[slot].nId > 0)
        {
            packet->writeInt32(slot);
            packet->writeInt32(m_MingGeBag[slot].nId);
            packet->writeInt32(m_MingGeBag[slot].IsLock);
            ++slotCount;
        }
    }
    int32_t afterBag = packet->getWOffset();
    packet->setWOffset(slotSaveOffset);
    packet->writeInt32(slotCount);
    packet->setWOffset(afterBag);

    int32_t slotSaveOffset2 = packet->getWOffset();
    int32_t slotCount2 = 0;
    packet->writeInt32(0);
    for (int32_t slot = 0; slot < 30; ++slot)
    {
        if (m_LieMingBag[slot].nId > 0)
        {
            packet->writeInt32(slot);
            packet->writeInt32(m_LieMingBag[slot].nId);
            packet->writeInt32(m_LieMingBag[slot].IsLock);
            ++slotCount2;
        }
    }
    int32_t afterLie = packet->getWOffset();
    packet->setWOffset(slotSaveOffset2);
    packet->writeInt32(slotCount2);
    packet->setWOffset(afterLie);

    int32_t slotSaveOffset3 = packet->getWOffset();
    int32_t slotCount3 = 0;
    packet->writeInt32(0);
    for (int32_t slot = 0; slot < 8; ++slot)
    {
        if (m_MingGeEquip[slot].nId > 0)
        {
            packet->writeInt32(slot);
            packet->writeInt32(m_MingGeEquip[slot].nId);
            packet->writeInt32(m_MingGeEquip[slot].IsLock);
            ++slotCount3;
        }
    }
    int32_t afterEquip = packet->getWOffset();
    packet->setWOffset(slotSaveOffset3);
    packet->writeInt32(slotCount3);
    packet->setWOffset(afterEquip);

    packet->writeInt32(m_MingGeExp);
    packet->writeInt32(m_MingChip);
    for (int32_t i = 0; i < 5; ++i)
        packet->writeInt32(m_MingGeNpc[i]);
}

void CMingGeData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    int32_t slotCount = inPacket->readInt32();
    for (int32_t i = 0; i < slotCount; ++i)
    {
        MingGe stu;
        int32_t slot = inPacket->readInt32();
        stu.nId = inPacket->readInt32();
        stu.IsLock = inPacket->readInt32();
        m_MingGeBag[slot] = stu;
    }

    int32_t slotCount2 = inPacket->readInt32();
    for (int32_t i = 0; i < slotCount2; ++i)
    {
        MingGe stu;
        int32_t slot = inPacket->readInt32();
        stu.nId = inPacket->readInt32();
        stu.IsLock = inPacket->readInt32();
        m_LieMingBag[slot] = stu;
    }

    int32_t slotCount3 = inPacket->readInt32();
    for (int32_t i = 0; i < slotCount3; ++i)
    {
        MingGe stu;
        int32_t slot = inPacket->readInt32();
        stu.nId = inPacket->readInt32();
        stu.IsLock = inPacket->readInt32();
        m_MingGeEquip[slot] = stu;
    }

    m_MingGeExp = inPacket->readInt32();
    m_MingChip = inPacket->readInt32();
    for (int32_t i = 0; i < 5; ++i)
        m_MingGeNpc[i] = inPacket->readInt32();
}