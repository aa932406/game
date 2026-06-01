#ifndef _KUNLINGTABLE_H_
#define _KUNLINGTABLE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Npc;
class CActivity;
class KunLevelUpCfg;
class LingZhuCfg;
struct LingZhuPosCfg;
class LingZhuSuit;
class DanTianCfg;
class LingZhuSkill;

class KunLingTable
{
public:
    KunLingTable();
    ~KunLingTable();

    const LingZhuCfg* GetLingZhuCfg(int32_t nId);
    void AddKunLevelUpCfg(KunLevelUpCfg *p_Stu);
    void AddLingZhuCfg(LingZhuCfg *p_Stu);
    void AddLingZhuPos(LingZhuPosCfg stu);
    void AddLingZhuSuit(LingZhuSuit *p_Stu);
    void AddDanTianCfg(DanTianCfg *p_Stu);
    void AddLingZhuSkill(int32_t SuitLevel, LingZhuSkill Stu);
    const KunLevelUpCfg* GetKunLevelUpCfg(int32_t nLevel);
    const LingZhuPosCfg* GetLingZhuPosId(int32_t nType);
    const LingZhuSuit* GetLingZhuSuit(int32_t nId);
    const DanTianCfg* GetDanTianCfg(int32_t Mid);
    const LingZhuSkill* GetLingZhuSkill(int32_t SuitId);

private:
    std::map<int, DanTianCfg> m_DanTianCfgMap;
    std::map<int, KunLevelUpCfg> m_KunLevelUpCfgMap;
    std::map<int, LingZhuCfg> m_LingZhuCfgMap;
    std::map<int, LingZhuPosCfg> m_LingZhuPosMap;
    std::map<int, LingZhuSkill> m_LingZhuSkillMap;
    std::map<int, LingZhuSuit> m_LingZhuSuitMap;
};

#endif // _KUNLINGTABLE_H_
