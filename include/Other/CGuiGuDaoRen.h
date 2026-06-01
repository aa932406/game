#ifndef _CGUIGUDAOREN_H_
#define _CGUIGUDAOREN_H_

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

class CGuiGuDaoRen
{
public:
    CGuiGuDaoRen();
    ~CGuiGuDaoRen();

    void Init(int32_t line);
    int32_t OnAskBackItemCount(Player *pPlayer, Answer::NetPacket *packet);
    int32_t OnBackItem(Player *pPlayer, Answer::NetPacket *packet);
    void SendBackItemCount(Player *pPlayer, int32_t NpcId);
    void UpdateBackItemCount(int32_t NpcId);
    int32_t OnAskBackEquipCount(Player *pPlayer, Answer::NetPacket *packet);
    int32_t OnBackEquipCount(Player *pPlayer, Answer::NetPacket *InPacket);
    void SendBackEquipCount(Player *pPlayer);
    void UpdateEquipCount();
    int32_t GetMaxCount();
    void OnNewMinute(int32_t nMinute);
    int32_t OnAskEquipBackRank(Player *pPlayer, Answer::NetPacket *packet);
    void SendBackEquipRank(Player *pPlayer);
    void UpdateRankDate(EquipBackRankCfg *p_Stu);
    void AddRongHeRecord(RongHeRecord *p_Stu);
    void SendAllRongHeRecord(Player *pPlayer);
    void SendOneRongHeRecord(RongHeRecord *p_Stu);
    void AddRongHeCount(int32_t nCostCount, int32_t nGetCount);
    void UpdateRongHeCount();
    void SaveRecordToDB();

private:
        int32_t m_CostCount;
        // TODO: 确认类型 m_EquipBackRank
        int32_t m_GetCount;
        // TODO: 确认类型 m_RankLock
        // TODO: 确认类型 m_Rate
        // TODO: 确认容器类型 m_RongHeRecordList
};

#endif // _CGUIGUDAOREN_H_
