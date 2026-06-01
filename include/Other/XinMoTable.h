#ifndef _XINMOTABLE_H_
#define _XINMOTABLE_H_

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
class XinMoCfg;
class QiQingCfg;
class EquipJinHua;
class XinMoQiQingLevelUpCfg;
struct XinQingCfg;
struct XinQingReward;

class XinMoTable
{
public:
    XinMoTable();
    ~XinMoTable();

    void AddXingMoCfg(XinMoCfg *p_Stu);
    void AddQiQingCfg(QiQingCfg *p_Stu);
    void AddEquipJinHuaCfg(EquipJinHua *p_Stu);
    void AddXinMoQiQingLevelUpCfg(XinMoQiQingLevelUpCfg *p_Stu);
    void AddXinQingCfg(XinQingCfg stu);
    void AddXinQingReward(int32_t nId, XinQingReward *p_stu);
    const XinMoCfg* GetXingMoCfg(int32_t nlevel);
    const QiQingCfg* GetQiQingCfg(int32_t nLevel);
    const EquipJinHua* GetJinHuaCfg(int32_t EquipId);
    int32_t GetAppearCount(int32_t nLeftCount);
    const XinQingReward* GetXinQingReward(int32_t nId);

private:
    std::map<int, EquipJinHua> m_EquipJinHuaMap;
    std::map<int, XinQingReward> m_MemChrBagVectorMap;
    std::map<int, QiQingCfg> m_QiQingCfgMap;
    std::map<int, XinMoCfg> m_XinMoCfgMap;
    std::map<std::pair<int, int>, XinMoQiQingLevelUpCfg> m_XinMoQiQingLevelUpCfgMap;
    std::list<XinQingCfg> m_XinQingCfgList;
};

#endif // _XINMOTABLE_H_
