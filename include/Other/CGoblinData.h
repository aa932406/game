#ifndef _CGOBLINDATA_H_
#define _CGOBLINDATA_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace Answer { class NetPacket; }

struct GoblinInfo
{
    int32_t level;
    int32_t exp;
};

class CGoblinData
{
public:
    CGoblinData();
    ~CGoblinData();

    void CleanUp();
    void PackageData(Answer::NetPacket *packet);
    void UnPackageData(Answer::NetPacket *inPacket, int64_t nCid);
    std::string GetGoblinInfoString();
    void ParesGoblinInfoString(std::string *p_GoblinInfoString);
    std::string GetShouHuRefiningString();
    void ParesShouHuRefiningString(std::string *p_ShouHuRefiningString);
    std::string GetWingEquipPolishString();
    void ParesWingEquipPolishString(std::string *p_WingEquipPolishString);
    std::string GetMoFuString();
    void ParesMoFuString(std::string *p_MoFuString);
    std::string GetWingEquipRefiningString();
    void ParesWingEquipRefiningString(std::string *p_WingEquipRefiningString);
    std::string GetVipPosString();
    void ParesVipPosString(std::string *p_VipPosString);

private:
    std::map<int32_t, GoblinInfo> m_GoblinInfoMap;
    std::map<int32_t, int32_t>    m_ShouHuRefiningLeveMap;
    std::map<int32_t, int32_t>    m_WingEquipPolishMap;
    std::map<int32_t, int32_t>    m_MoFuZhuNengMap;
    std::map<int32_t, int32_t>    m_WingEquipRefiningMap;
    std::map<int32_t, int32_t>    m_VipEquipPosLevelMap;
};

#endif
