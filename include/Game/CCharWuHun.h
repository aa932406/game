#ifndef _CCHARWUHUN_H_
#define _CCHARWUHUN_H_

#include <cstdint>
#include <string>
#include <vector>
#include <list>
#include <map>

class Player;
class Map;
class Unit;
class Monster;
class Npc;
class CActivity;
class CActivityMap;
class CfgActivity;
class CfgMonster;
class CfgMap;
class Answer_NetPacket;
class Position;

class CCharWuHun
{
public:
    CCharWuHun();
    ~CCharWuHun();

    void OnLoadFromDB(PlayerDBData * dbData);
    void OnSaveToDB(PlayerDBData * dbData);
    void GetInterestsProtocol(ProcIdList * procList);
    int32_t DispatchNetDatas(ProcId_t nProcId, Answer::NetPacket *inPacket);
    void AddCharAttr();
    int32_t onAskWuHunInfo(Answer::NetPacket *inPacket);
    int32_t onDressWuHun(Answer::NetPacket *inPacket);
    int32_t onUnDressWuHun(Answer::NetPacket *inPacket);
    void OnRemoveTalent(int32_t TalentId, int32_t TalentLevel);
    void OnAddTalent(int32_t TalentId, int32_t TalentLevel);
    int32_t onCreateWuHun(Answer::NetPacket *inPacket);
    void sendWuHunInfo();
    void sendWuHunSlotInfo(int32_t Level, int32_t Slot);
    bool checkWuHunPlace(int32_t Type, int32_t Slot);
    int32_t GetSuitLevel(int32_t nLevel);

private:
        // TODO: 确认类型 m_WuHun
        // TODO: 确认类型 m_pPlayer
};

#endif // _CCHARWUHUN_H_