#include "Other/CGoblinData.h"
#include "Network/NetPacket.h"
#include <cstring>

CGoblinData::CGoblinData()
{
    CleanUp();
}

CGoblinData::~CGoblinData()
{
}

void CGoblinData::CleanUp()
{
    m_GoblinInfoMap.clear();
    m_ShouHuRefiningLeveMap.clear();
    m_WingEquipPolishMap.clear();
    m_MoFuZhuNengMap.clear();
    m_WingEquipRefiningMap.clear();
    m_VipEquipPosLevelMap.clear();
}

void CGoblinData::PackageData(Answer::NetPacket *packet)
{
    auto writeMap = [packet](auto &m) {
        packet->writeInt32(static_cast<int32_t>(m.size()));
        for (auto &it : m)
        {
            packet->writeInt32(it.first);
            packet->writeInt32(it.second);
        }
    };
    writeMap(m_GoblinInfoMap);
    writeMap(m_ShouHuRefiningLeveMap);
    writeMap(m_WingEquipPolishMap);
    writeMap(m_MoFuZhuNengMap);
    writeMap(m_WingEquipRefiningMap);
    writeMap(m_VipEquipPosLevelMap);
}

void CGoblinData::UnPackageData(Answer::NetPacket *inPacket, int64_t nCid)
{
    auto readMap = [inPacket](auto &m) {
        m.clear();
        int32_t nSize = inPacket->readInt32();
        for (int32_t i = 0; i < nSize; ++i)
        {
            int32_t key = inPacket->readInt32();
            int32_t val = inPacket->readInt32();
            m[key] = val;
        }
    };
    readMap(m_GoblinInfoMap);
    readMap(m_ShouHuRefiningLeveMap);
    readMap(m_WingEquipPolishMap);
    readMap(m_MoFuZhuNengMap);
    readMap(m_WingEquipRefiningMap);
    readMap(m_VipEquipPosLevelMap);
}

std::string CGoblinData::GetGoblinInfoString() { return std::string(); }
void CGoblinData::ParesGoblinInfoString(std::string *p) { }
std::string CGoblinData::GetShouHuRefiningString() { return std::string(); }
void CGoblinData::ParesShouHuRefiningString(std::string *p) { }
std::string CGoblinData::GetWingEquipPolishString() { return std::string(); }
void CGoblinData::ParesWingEquipPolishString(std::string *p) { }
std::string CGoblinData::GetMoFuString() { return std::string(); }
void CGoblinData::ParesMoFuString(std::string *p) { }
std::string CGoblinData::GetWingEquipRefiningString() { return std::string(); }
void CGoblinData::ParesWingEquipRefiningString(std::string *p) { }
std::string CGoblinData::GetVipPosString() { return std::string(); }
void CGoblinData::ParesVipPosString(std::string *p) { }
